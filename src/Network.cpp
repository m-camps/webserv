/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:07 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/25 16:01:30 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Network.hpp"
#include "../inc/Request.hpp"
#include "../inc/Client.hpp"
#include "../inc/Constants.hpp"

#define BUFF 10000
/* Default constructor */
Network::Network() {}

/* Default deconstructor */
Network::~Network() {}


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
{
    while (true)
    {
        if (poll(_poll.data(), _poll.size(), -1) == -1)
        {
            std::perror("Error In poll, system failure: ");
			std::exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < _poll.size(); i++)
        {
            struct pollfd cur = _poll.at(i);
            if ((cur.revents & POLLIN))
            {
                if (isSocketFd(cur.fd))
                {
					try
					{
						acceptConnection(cur.fd);
					}
					catch(const std::exception& e)
					{
						std::cerr << e.what() << std::endl;
						break ;
					}
                }
                else
                {
					try
					{
						receiveData(cur.fd);
					}
					catch(const std::exception& e)
					{
						closeConnection(cur.fd, i);
						break ;
					}
                }
            }
			else if ((cur.events & POLLOUT))
			{
                try
                {
					sendResponse(cur.fd);
                }
                catch (const Exchange::WriteException& e)
                {
                    std::cerr << e.what() << std::endl;
					closeConnection(cur.fd, i);
					break ;
                }
				catch (const Exchange::ClearException &e)
				{
					std::cerr << e.what() << std::endl;
					_io.find(cur.fd)->second = Client(_io.find(cur.fd)->second.servers);
				}
                catch (const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                }
			}
        }
    }
}

void	Network::sendResponse(int fd)
{
	if (isSocketFd(fd) == true || _io.find(fd)->second.readyToWrite == false)
		return ;
	Request request;
    Client client = _io.find(fd)->second;
	Servers servers = getServersByFd(fd);

	Exchange exchange(servers, fd, request.parseRequest(client.readString));
	_io.find(fd)->second = Client(_io.find(fd)->second.servers);
}

void 	Network::receiveData(int fd)
{
	char buff[BUFF];
	ssize_t ret = recv(fd, buff, sizeof(buff), 0);
	if (ret <= 0)
	{
		if (ret == 0)
			std::cout << MAGENTA_COLOR << "Client closed connection fd: " << fd << RESET_COLOR <<std::endl;
		else
			std::cerr << "Client connection closed fd: " << fd <<  ": [Error in recv]" << std::endl;
		throw(std::logic_error("Connection closed"));
	}
	else
	{
		Client client = _io.find(fd)->second;
		client.readLength += ret;
		client.readString.append(buff, ret);
		if (client.requestData.empty())
		{
			std::size_t found = client.readString.find(SEPERATOR);
			if (found != std::string::npos)
			{
				Request request;
				client.requestData = request.parseRequest(client.readString);
				if (client.requestData.find("Content-Length") != client.requestData.end())
					client.contentLength = ft_strol(client.requestData.find("Content-Length")->second);
			}
		}
		if (client.requestData.empty() == false && client.contentLength == 0)
		{
			client.readyToWrite = true;
		}
		if (client.requestData.empty() == false && client.contentLength > 0 &&
			client.contentLength <= client.readLength)
		{
			client.readyToWrite = true;
		}
		_io.find(fd)->second = client;
	}
}

void	Network::setupIO(int port, int socket_fd)
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
	_io.insert(std::pair<int, Client >(socket_fd, Client(servers)));
}

void	Network::acceptConnection(int socket_fd)
{
	struct  sockaddr_in			client_addr;
	socklen_t					client_addrlen = sizeof(client_addr);
	
	int	client_fd = accept(socket_fd, (struct sockaddr*)(&client_addr), &client_addrlen);
	if (client_fd == -1)
		throw(std::runtime_error("Could not accept connection"));
	_poll.push_back(newPoll(client_fd));
	_io.insert(std::pair<int, Client>(client_fd, Client(_io.find(socket_fd)->second.servers)));
    std::cout << GREEN_COLOR <<"New connection on FD: " << client_fd << RESET_COLOR <<std::endl;
}

void	Network::closeConnection(int fd, int i)
{
	_poll.erase(_poll.begin() + i);
	_io.erase(fd);
	if (close(fd) < 0)
		std::cerr << "Closing a file descriptor failed." << std::endl;
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
		struct sockaddr_in  addr_in = makeSocketAddr(port);

		bind(socket_fd, addr_in);
		listen(socket_fd);
		setupIO(port, socket_fd);
		_socket_fds.push_back(socket_fd);
	}
}

int		Network::createSocket(void)
{
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		throw std::runtime_error("Socket failed");
	}
	int reuse = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
   		throw std::runtime_error("setsockopt(SO_REUSEADDR) failed");
    if (setsockopt(socket_fd, SOL_SOCKET, SO_NOSIGPIPE, &reuse, sizeof(int)) < 0)
   		throw std::runtime_error("setsockopt(SO_REUSEPIPE) failed");
	return socket_fd;
}

struct sockaddr_in	Network::makeSocketAddr(int port)
{
	struct sockaddr_in address = {};

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(address.sin_zero, 0, sizeof(address.sin_zero));
	return address;
}

void	Network::bind(int socket_fd, struct sockaddr_in address_in)
{
	if (::bind(socket_fd, (const struct sockaddr *) &address_in, sizeof(address_in)) < 0)
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
	for (std::map<int, Client>::iterator it = _io.begin(); it != _io.end(); it++)
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
	return (_io.find(fd)->second.servers);
}
