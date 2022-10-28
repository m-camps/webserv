/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/28 16:05:55 by mcamps        ########   odam.nl         */
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
/* do we need to use select with the bitflags? */
void Network::run()
{
	char buff[BUFF]; //  test buffer (can change later or keep it here)
	std::string RequestStr;
	//check if both location is seen here
	while (true)
	{
		if (poll(_fds, _total_fd, -1) == -1)
		{
			perror("In poll: ");
			free(_fds);
			exit(ERROR);
		}
		for (int i = 0; i < _total_fd; i++)
		{
			std::cout << _total_fd << _fds[i].fd << '\n';
 			struct pollfd cur = _fds[i];
			if ((cur.revents & POLLIN))
			{
				if (isSocketFd(cur.fd))
				{
					std::cout << "New connection" << "\n";
					Server *server = getServerBySocketFd(cur.fd);
					int newFd = server->acceptConnection();
					addToPollFds(newFd);
					// std::cout << *server << std::endl;
					std::cout << "On FD " << newFd << std::endl;
				}
				else
				{
					bzero(buff, BUFF);
					int ret = recv(cur.fd, buff, sizeof(buff), 0);
					std::cout << "\nret = " << ret;
					if (ret <= 0)
					{
						RequestStr.clear();
						if (ret == 0)
							;
						else
							perror("In recv: ");
						close(cur.fd);
						delFromPollFds(i);
					}
					RequestStr.append(buff, ret);
					if (ret != BUFF && ret > 0)
					{
						Exchange exchange(*getServerByClientFd(cur.fd),cur.fd);
						Request request(RequestStr, exchange);
                        RequestStr.erase();
					}
//					bzero(buff, 10); // lol size is zero
				}
				cur.revents = 0;
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
	_buffer.insert(std::pair<int, std::string>(fd, ""));
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
