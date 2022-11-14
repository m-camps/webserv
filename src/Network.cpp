/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/14 17:38:48 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Network.hpp"
#include "../inc/Server.hpp"
#include "../inc/Constants.hpp"
#include "Exchange/Exchange.hpp"
#include "Exchange/Request.hpp"
#include <iostream>

#define BUFF 10000
/* Default constructor */
Network::Network() {}

/* Default deconstructor */
Network::~Network() {}

/***
 * Setup for the network
 * (right now just starting a basic server with default constructor)
 * @param file the file to be parsed
***/
void Network::setup(std::string file)
{
	Parse				parser;
	
	try
	{
		Servers servers;
		
		_servers = parser.parseNetwork(file, servers);
		std::cout << "----PARSING COMPLETE----" << std::endl;
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::exit(0);
	}
	setupSockets();
	createPoll();
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << _servers.at(i) << "\n";
	}
}

/* Poll() loop of the network */
/* do we need to use select with the bitflags? */
void Network::run()
{
    char buff[BUFF]; //  test buffer (can change later or keep it here)
	std::map<int, std::string> requestStrings;
    //check if both location is seen here
    while (true)
    {
        if (poll(_poll.data(), _poll.size(), -1) == -1)
        {
            perror("In poll: ");
            exit(ERROR);
        }
        for (size_t i = 0; i < _poll.size(); i++)
        {
            struct pollfd cur = _poll.at(i);
            if ((cur.revents & POLLIN))
            {
                if (isSocketFd(cur.fd))
                {
					int client_fd = acceptConnection(cur.fd);
					requestStrings.insert(std::pair<int, std::string>(client_fd, ""));
                    std::cout << "New connection" << "\n";
                    std::cout << "On FD " << client_fd << std::endl;
                }
                else
                {
                    ssize_t ret = recv(cur.fd, buff, sizeof(buff), 0);
                    if (ret <= 0)
                    {
                        if (ret == 0)
                            std::cout << "Client closed connection fd: " << cur.fd <<"\n";
                        else
                            std::perror("In recv: ");
                        close(cur.fd);
						_poll.erase(_poll.begin() + i);
						requestStrings.erase(cur.fd);
						break ;
                    }
                    else
                    {
						requestStrings.find(cur.fd)->second.append(buff, ret);
                    	std::size_t found = requestStrings.find(cur.fd)->second.find(SEPERATOR);
                        if (ret != BUFF && found != std::string::npos) 
						{
                            Exchange exchange(getServersByFd(cur.fd), cur.fd, requestStrings.find(cur.fd)->second);
							requestStrings.find(cur.fd)->second.clear();
							std::cout << "Reponse on fd: " << cur.fd << std::endl;
                        }
                    }
                }
            }
        }
    }
}

void	Network::setupFds(int port, int socket_fd)
{
	Servers servers;

	for (Servers::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		std::vector<int> ports = (*it).getPorts();
		if (std::find(ports.begin(), ports.end(), port) != ports.end())
		{
			servers.push_back(*it);
		}
	}
	_fds.insert(std::pair<int, Servers >(socket_fd, servers));
}

void	Network::addClientToFds(int socket_fd, int client_fd)
{
	Servers servers = _fds.find(socket_fd)->second;

	_fds.insert(std::pair<int, Servers >(client_fd, servers));
}

void	Network::delClientFromFds(int client_fd)
{
	_fds.erase(client_fd);
}

int		Network::acceptConnection(int socket_fd)
{
	struct  sockaddr_in			client_addr;
	socklen_t					client_addrlen = sizeof(client_addr);
	
	int	client_fd = accept(socket_fd, (struct sockaddr*)(&client_addr), &client_addrlen);
	if (client_fd == -1)
	{
		std::perror("In accept: ");
		std::exit(EXIT_FAILURE);
	}
	addClientToFds(socket_fd, client_fd);
	_poll.push_back(newPoll(client_fd));
	return (client_fd);
}

std::vector<int> Network::extractListens(void)
{
	std::vector<int>	listens;
	for (Servers::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		std::vector<int> ports = (*it).getPorts();
		for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++)
		{
			if (std::find(listens.begin(), listens.end(), *it) == listens.end())
				listens.push_back(*it);
		}
	}
	return (listens);
}

void	Network::setupSockets(void)
{
	std::vector<int> listens = extractListens();

	for (std::vector<int>::iterator it = listens.begin(); it != listens.end(); it++)
	{
		int 				port = *it;
		int					socket_fd = createSocket();
		struct sockaddr_in* addr_in = makeSocketAddr(port);

		bind(socket_fd, addr_in);
		listen(socket_fd);
		fcntl(socket_fd, F_SETFL, O_NONBLOCK);
		setupFds(port, socket_fd);
		_socket_fds.push_back(socket_fd);
	}
}

int		Network::createSocket(void)
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);\
	
	if (socket_fd < 0) {
		std::exit(EXIT_FAILURE);
	}
	int reuse = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
   		std::perror("setsockopt(SO_REUSEADDR) failed");
	return socket_fd;
}

struct sockaddr_in *	Network::makeSocketAddr(int port)
{
	struct sockaddr_in *address = new sockaddr_in();

	address->sin_family = AF_INET;
	address->sin_port = htons(port);
	address->sin_addr.s_addr = INADDR_ANY;
	memset(address->sin_zero, 0, sizeof(address->sin_zero));
	return address;
}

void	Network::bind(int socket_fd, struct sockaddr_in* address_in)
{
	if (::bind(socket_fd, (const struct sockaddr *)address_in, sizeof(*address_in)) < 0)
	{
		std::perror("In Bind: ");
		std::exit(ERROR);
	}
}

void	Network::listen(int socket_fd)
{
	if (::listen(socket_fd, 5) < 0)
	{
		std::perror("In listen: ");
		std::exit(ERROR);
	}
}

void			Network::createPoll(void)
{
	for (std::map<int, Servers>::iterator it = _fds.begin(); it != _fds.end(); it++)
		_poll.push_back(newPoll(it->first));
}

struct pollfd 	Network::newPoll(int fd)
{
	struct pollfd newFd;

	newFd.fd = fd;
	newFd.events = POLLIN;
	newFd.revents = 0;
	return(newFd);
}

/* Helper functions */ 
bool	Network::isSocketFd(int fd)
{
	for (std::vector<int>::iterator it = _socket_fds.begin(); it != _socket_fds.end(); it++)
	{
		if (*it == fd)
			return true;
	}
	return false;
}

Servers	Network::getServersByFd(int fd)
{
	return (_fds.find(fd)->second);
}
