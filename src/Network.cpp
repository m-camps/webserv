/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/05 18:21:37 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Network.hpp"

/***
 * Temp vector pritn functions
***/
void PrintVector(std::vector<std::string> toPrint) 
{
	
	std::vector<std::string>::iterator it = toPrint.begin();

    for ( ; it != toPrint.end(); it++)
    	std::cout << *it << "\n";
}

/***
 * Default constructor
***/
Network::Network() : _max_fd(MAX_FD) {}

/***
 * Default deconstructor
***/
Network::~Network() {}



/***
 * Setup for the network
 * @param file the file to be parsed
***/
void Network::setup(std::string file)
{
	Parse				parser;
	
	_serverConfigs = parser.parseNetwork(file);
	_servers.push_back(Server());
	_servers.push_back(Server());
	_servers.at(0).changePort("80");
	_servers.at(0).setup();
	_servers.at(1).setup();
	_total_fd = _servers.size();
	createFds();

}

void	Network::createFds()
{
	struct pollfd *fds = (pollfd*)malloc((sizeof(pollfd) * _total_fd) + 1);
	// Add malloc fail
	for (int i = 0; i < (int)_servers.size(); i++)
		fds[i].fd = _servers.at(i).getSocketFd();
	for (int i = 0; i < 2; i++)
		std::cout << fds[i].fd << "\n";
	_fds = fds;
}

void	Network::addToPollFds(int fd)
{
	if (_total_fd >= _max_fd)
	{
		_fds = (pollfd*)realloc(_fds, (_max_fd + MAX_FD) * sizeof(struct pollfd));
		if (_fds == NULL)
			exit(ERROR);
		_max_fd += MAX_FD;
	}
	_total_fd++;

	struct pollfd newFd;

	newFd.fd = fd;
	newFd.events = POLLIN;
	newFd.revents = 0;

	_fds[_total_fd] = newFd;
}


/***
 * Poll() loop of the network
***/
void Network::run()
{
	while (true)
	{
		if (poll(_fds, _total_fd, 0) == -1)
		{
			free(_fds);
			exit(ERROR);
		}
		for (int i = 0; i < _total_fd; i++)
		{
			struct pollfd cur_fd = _fds[i];
			if ((cur_fd.fd & POLLIN) == POLLIN)
			{
				if (isSocketFd(cur_fd.fd)) // If fd is a socket fd accept new connection
				{
					// Server *server = getServerBySocketFd(cur_fd.fd);
					// int newFd = server->acceptConnection();
					// addToPollFds(newFd);
				}
			}
			
		}
	}

}

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
