/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/06 15:55:10 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Network.hpp"

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
	std::string			server1 = "Server 1";
	std::string			server2 = "Server 2";
	std::string			server3 = "Server 3";
	
	_serverConfigs = parser.parseNetwork(file); // Parse config file into server Blocks

	_servers.push_back(Server());
	_servers.push_back(Server());
	_servers.push_back(Server());
	_servers.at(0).changePort("80");
	_servers.at(0).setName(_servers.at(0),server1);
	_servers.at(0).setup();
	
	_servers.at(1).changePort("81");
	_servers.at(1).setName(_servers.at(1),server2);
	_servers.at(1).setup();

	_servers.at(2).changePort("82");
	_servers.at(2).setName(_servers.at(2), server3);
	_servers.at(2).setup();
	_total_fd = _servers.size();
	createFds();
}

/* Poll() loop of the network */
void Network::run()
{
	char buff[256]; //  test buffer (can change later or keep it here)

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
				else // Else normal connection // xander this is where the exchange starts fd = cur.fd
				// RIGHT NOW DOESNT WORK AS INTENDED
				{
					int ret = recv(cur.fd, buff, sizeof(buff), 0);
	
					if (ret <= 0)
					{
						if (ret == 0)
							std::cout << "Connection closed\n";
						else	
							perror("In recv: ");
						
						close(cur.fd);
						delFromPollFds(i);
					}
					std::cout << buff;
					std::cout << "\nret = " << ret << "\n";
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
