/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/11 13:19:10 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Network.hpp"
#include "../inc/Server.hpp"
#include "../inc/Constants.hpp"
#include "Exchange/Exchange.hpp"
#include "Exchange/Request.hpp"
#include <iostream>

#define BUFF 200000
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
		std::vector<Server> servers;
		
		_servers = parser.parseNetwork(file, servers);
		std::cout << "----PARSING COMPLETE----" << std::endl;
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::exit(0);
	}

	setupSocketFds();
	linkSocketsToServers();
	createPoll();
}

void			Network::linkSocketsToServers(void)
{
	for (std::vector<Server>::iterator serv_it = _servers.begin(); serv_it != _servers.end(); serv_it++)
	{
		std::vector<int> ports = (*serv_it).getPorts();
		for (std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++)
		{
			std::map<int, int>::iterator found = _port_fds.find(*it);
			if (found != _port_fds.end())
				(*serv_it).addToSocketFds(found->second);
		}
	}
	for (std::map<int,int>::iterator it = _port_fds.begin(); it != _port_fds.end(); it++)
	{
		std::cout << "Port: "<< it->first << " linked to fd: " << it->second << std::endl;
	}
}

void			Network::setupSocketFds(void)
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
		setupServersInSocket(port, socket_fd);
		_port_fds.insert(std::pair<int, int>(port, socket_fd));
	}
}

void	Network::setupServersInSocket(int port, int socket_fd)
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
	_servers_in_socket.insert(std::pair<int, Servers >(socket_fd, servers));
}

void	Network::addClientToServersInSocket(int socket_fd, int client_fd)
{
	Servers servers = _servers_in_socket.find(socket_fd)->second;

	_servers_in_socket.insert(std::pair<int, Servers >(client_fd, servers));
}

void	Network::delClientFromServersInSocket(int client_fd)
{
	_servers_in_socket.erase(client_fd);
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

struct sockaddr_in *	Network::makeSocketAddr(int port)
{
	struct sockaddr_in *address = new sockaddr_in();

	address->sin_family = AF_INET;
	address->sin_port = htons(port);
	address->sin_addr.s_addr = INADDR_ANY;
	memset(address->sin_zero, 0, sizeof(address->sin_zero));
	return address;
}

std::vector<int> Network::extractListens(void)
{
	std::vector<int>	listens;
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
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

/* Poll() loop of the network */
/* do we need to use select with the bitflags? */
void Network::run()
{
	// for (size_t i = 0; i < _servers.size(); i++)
	// {
	// 	std::cout << _servers.at(i) << "\n";
	// }
    char buff[BUFF]; //  test buffer (can change later or keep it here)
    std::string RequestStr;
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
                    Server *server = getServerBySocketFd(cur.fd);
                    int client_fd = server->acceptConnection(cur.fd);
					_poll.push_back(newPoll(client_fd));
                    std::cout << "New connection" << "\n";
                    std::cout << "On FD " << client_fd << std::endl;
                }
                else
                {
                    ssize_t ret = recv(cur.fd, buff, sizeof(buff), 0);
                    if (ret <= 0)
                    {
                        RequestStr.clear();
                        if (ret == 0)
                            std::cout << "Client closed connection" << "\n";
                        else
                            std::perror("In recv: ");
                        close(cur.fd);
						_poll.erase(_poll.begin() + i);
						break ;
                    }
                    else
                    {
                        RequestStr.append(buff, ret);
                        //                    std::size_t found = RequestStr.find(SEPERATOR);
                        if (ret != BUFF) {
                            Exchange exchange(*getServerByClientFd(cur.fd), cur.fd, RequestStr);
                            RequestStr.erase();
                        }
                    }
                }
            }
        }
    }
}

void	Network::createPoll(void)
{
	for (std::map<int, int>::iterator it = _port_fds.begin(); it != _port_fds.end();it++)
		_poll.push_back(newPoll(it->second));
}

struct pollfd Network::newPoll(int fd)
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
	for (int i = 0; i < (int)_servers.size(); i++)
	{
		if (_servers.at(i).isSocketFdInServer(fd))
			return true;
	}
	return false;
}

Server*	Network::getServerBySocketFd(int fd)
{
	for (int i = 0; i < (int)_servers.size(); i++)
	{
		if (_servers.at(i).isSocketFdInServer(fd))
			return &_servers.at(i);
	}
	return NULL;
}

Server*	Network::getServerByClientFd(int fd)
{
	for (int i = 0; i < (int)_servers.size(); i++)
	{
		if (_servers.at(i).isClientFdInServer(fd))
			return &_servers.at(i);
	}
	return NULL;
}
