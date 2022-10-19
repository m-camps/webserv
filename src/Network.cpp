/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/19 14:16:54 by mcamps        ########   odam.nl         */
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
Network::Network() : _max_fd(5) {}

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
	std::vector<Server> tmp;
	
	// (void)file;
	_servers = parser.parseNetwork(file, tmp); // Parse config file into server Blocks

	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << _servers.at(i) << "\n";
		_servers.at(i).setup();
	}

	_total_fd = _servers.size();
	createFds();
}


/* Poll() loop of the network */
void Network::run()
{
	char buff[BUFF]; //  test buffer (can change later or keep it here)
	std::string request;
	//check if both location is seen here
	while (true)
	{
		if (poll(_fds, _total_fd, 0) == -1)
		{
			perror("In poll: ");
			free(_fds);
			exit(ERROR);
		}
		for (int i = 0; i < _total_fd; i++)
		{
 			struct pollfd cur = _fds[i];
			if ((cur.revents & POLLIN))
			{
				if (isSocketFd(cur.fd)) // If fd is a socket fd accept new connection
				{
					Server *server = getServerBySocketFd(cur.fd);
					int newFd = server->acceptConnection();
					addToPollFds(newFd);
				}
				else // Else normal connection 
				// xander this is where the exchange starts fd = cur.fd
				// RIGHT NOW DOESNT WORK AS INTENDED 
				{
					int ret = recv(cur.fd, buff, sizeof(buff), 0);
					if (ret <= 0)
					{
						request.clear();
						if (ret == 0)
							std::cout << "Connection closed\n";
						else
							perror("In recv: ");
						
						close(cur.fd);
						delFromPollFds(i);
					}
					request.append(buff);
					if (ret != BUFF)
					{
						std::cout << "POLLING\n";
						Exchange exchange(*getServerByClientFd(cur.fd),cur.fd);
						Request request(buff, exchange);
					}	

					*buff = '\0';
				}
			}
		}
	}
}

void	Network::createFds(void)
{
	_fds = (pollfd*)malloc(_max_fd * sizeof(pollfd));
	if (!_fds)
	{
		perror("Malloc: ");
		exit(EXIT_FAILURE);
	}
	
	for (int i = 0; i < (int)_servers.size(); i++)
	{
		_fds[i].fd = _servers.at(i).getSocketFd();
		_fds[i].events = POLLIN;
	}
	if (DEBUG)
	{
		std::cout << "File descriptors in use: [";
		for (int i = 0; i < (int)_servers.size(); i++)
			std::cout << ((i != 0) ? "," : "") << _fds[i].fd;
		std::cout << "]\n";
	}
}

void	Network::addToPollFds(int fd)
{
	if (DEBUG)
		std::cout << "total_fd " << _total_fd << " _max_fd " << _max_fd << "\n";
	if (_total_fd >= _max_fd)
	{
		_max_fd *= 2;
		struct pollfd *tmp = (pollfd*)realloc(_fds, _max_fd * sizeof(pollfd));
		if (tmp == NULL)
			exit(ERROR);
		_fds = tmp;
	}
	_fds[_total_fd] = createNewPollfd(fd);
	_total_fd++;
}

void	Network::delFromPollFds(int i)
{
	_fds[i] = _fds[_total_fd - 1];
	_total_fd--;
}

struct pollfd Network::createNewPollfd(int fd)
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
		if (_servers.at(i).getSocketFd() == fd)
			return true;
	}
	return false;
}

Server*	Network::getServerBySocketFd(int fd)
{
	for (int i = 0; i < (int)_servers.size(); i++)
	{
		if (_servers.at(i).getSocketFd() == fd)
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