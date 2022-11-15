/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/15 18:31:41 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Network.hpp"
#include "../inc/Server.hpp"
#include "../inc/Constants.hpp"
#include "Exchange/Exchange.hpp"
#include "Exchange/Request.hpp"
#include <iostream>
#include "../inc/Cgi.hpp"
#include "Poller.hpp"

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
	Parse		parser;
	Servers 	servers;
	
	_servers = parser.parseNetwork(file, servers);
	std::cout << "----PARSING COMPLETE----" << std::endl;
	setupSockets();
	createPoll();
	std::cout << "----SOCKET SETUP COMPLETE----" << std::endl;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << _servers.at(i) << "\n";
	}
}

/* Poll() loop of the network */
/* do we need to use select with the bitflags? */
void Network::run()
{;
    char buff[BUFF]; //  test buffer (can change later or keep it here)
	std::map<int, Poller> io;
    //check if both location is seen here
    while (true)
    {
        if (poll(_poll.data(), _poll.size(), -1) == -1)
        {
            perror("In poll: ");
        }
        for (size_t i = 0; i < _poll.size(); i++)
        {
            struct pollfd cur = _poll.at(i);
            if ((cur.revents & POLLIN))
            {
                if (isSocketFd(cur.fd))
                {
					int client_fd = acceptConnection(cur.fd);
					io.insert(std::pair<int, Poller>(client_fd, Poller()));
                    std::cout << "New connection" << "\n";
                    std::cout << "On FD " << client_fd << std::endl;
                }
                else
                {
                    ssize_t ret = recv(cur.fd, buff, sizeof(buff), 0);
                    if (ret <= 0)
                    {
                        if (ret == 0)
                            std::cout << "Client closed connection fd: " << cur.fd << "\n";
                        else
                            std::perror("In recv: ");
                        close(cur.fd);
						_poll.erase(_poll.begin() + i);
						io.erase(cur.fd);
						break ;
                    }
                    else
                    {
						Poller poller = io.find(cur.fd)->second;
						
						poller.readLength += ret;
						poller.readString.append(buff, ret);
						if (poller.requestData.empty())
                    	{
							std::size_t found = poller.readString.find(SEPERATOR);
							if (found != std::string::npos)
							{
								Request request;
								poller.requestData = request.parseRequest(poller.readString);
								if (poller.requestData.find("Content-Length") != poller.requestData.end())
									poller.contentLength = ft_strol(poller.requestData.find("Content-Length")->second);
							}
						}
                        if (poller.requestData.empty() == false && poller.contentLength == 0)
                        {
							poller.readyToWrite = true;
                        }
                        if (poller.requestData.empty() == false && poller.contentLength > 0 &&
                            poller.contentLength <= poller.readLength)
                        {
                            poller.readyToWrite = true;
                        }
						if (poller.readLength % 10000000 == 0)
							std::cout << poller.contentLength << " " << poller.readString.size() << std::endl;
						io.find(cur.fd)->second = poller;
                    }
                }
            }
			else if ((cur.events & POLLOUT) && io.find(cur.fd)->second.readyToWrite == true && isSocketFd(cur.fd) == false)
			{
				Poller poller = io.find(cur.fd)->second;
				Request	request;

				Exchange exchange(getServersByFd(cur.fd), cur.fd, request.parseRequest(poller.readString));
				std::cout << "Reponse on fd: " << cur.fd << std::endl;
				io.find(cur.fd)->second = Poller();
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
		setupFds(port, socket_fd);
		_socket_fds.push_back(socket_fd);
	}
}

int		Network::createSocket(void)
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);\
	
	if (socket_fd < 0) {
		throw std::runtime_error("Socket failed");
	}
	int reuse = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
   		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
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
		throw std::runtime_error("Bind failed");
}

void	Network::listen(int socket_fd)
{
	if (::listen(socket_fd, 5) < 0)
		throw std::runtime_error("Listen failed");
	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Fcntl failed");
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
	newFd.events = POLLIN | POLLOUT;
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
