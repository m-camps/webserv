/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/07 15:18:16 by mcamps        ########   odam.nl         */
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
	std::vector<Server> tmp;
	
	_total_fd = 0;
	try
	{
		_servers = parser.parseNetwork(file, tmp);
		std::cout << "----PARSING COMPLETE----" << std::endl;
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
		std::exit(0);
	}

	for (size_t i = 0; i < _servers.size(); i++)
	{
		_total_fd += _servers.at(i).setup();
		std::cout << _servers.at(i) << "\n";
	}
	createFds();
	_max_fd = _total_fd;
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
            struct pollfd cur = _fds[i];
            if ((cur.revents & POLLIN))
            {
                if (isSocketFd(cur.fd))
                {
                    Server *server = getServerBySocketFd(cur.fd);
                    int newFd = server->acceptConnection(cur.fd);
                    addToPollFds(newFd);
                    std::cout << "New connection" << "\n";
                    std::cout << "On FD " << newFd << std::endl;
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
                        delFromPollFds(i);
                    }
                    else
                    {
                        RequestStr.append(buff, ret);
                        std::size_t found = RequestStr.find(SEPERATOR);
                        if (ret != BUFF && found != std::string::npos)
                        {
                            Exchange exchange(*getServerByClientFd(cur.fd), cur.fd, RequestStr);
                            RequestStr.erase();
                        }
                    }
                }
            }
        }
    }
}

void	Network::createFds(void)
{
	_fds = (pollfd*)malloc((_total_fd) * sizeof(pollfd));
	if (!_fds)
	{
		perror("Malloc: ");
		exit(EXIT_FAILURE);
	}
	
	int j = 0;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::vector<int> fds = _servers.at(i).getSocketFds();
		for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); it++)
		{
			_fds[j] = createNewPollfd(*it);
			j++;
		}
	}
	if (DEBUG)
	{
		std::cout << "File descriptors in use: [";
		for (int i = 0; i < _total_fd; i++)
			std::cout << ((i != 0) ? "," : "") << _fds[i].fd;
		std::cout << "]\n";
	}
}

void	Network::addToPollFds(int fd)
{
	if (_total_fd >= _max_fd)
	{
		_max_fd *= 2;
		struct pollfd *tmp = (pollfd*)realloc(_fds, _max_fd * sizeof(pollfd));
		if (!tmp)
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
