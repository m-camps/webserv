/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 15:36:19 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/17 17:54:36 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Location.hpp"
#include "../inc/Constants.hpp"
#include <iostream>

Server::Server()
{
	this->_root = "/";
	this->_index = "index.html";
}

Server::~Server() { return; }


Server::Server(const Server& src)
{
    *this = src;
    return;
}

Server& Server::operator=(const Server& rhs)
{
    if (this != &rhs)
    {
        this->_names = rhs._names;
        this->_port = rhs._port;
        this->_root = rhs._root;
        this->_index = rhs._index;
        this->_client_body_size = rhs._client_body_size;
        //methods?this->_methods = rhs._methods;
    }
    return *this;
}



/* Getters */
int								Server::getPort(void) const { return _port; }
std::vector<std::string>		Server::getNames(void) const { return _names; }
std::string						Server::getRoot(void) const { return _root; }
std::string  					Server::getIndex(void) const { return _index; }
int								Server::getClientBodySize(void) const { return _client_body_size; }
std::vector<std::string>		Server::getMethods(void) const { return _methods; }
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
std::map<std::string, Location>& Server::getLocations(void)  { return _locations; } //shouldnt this be a poiter?
=======
=======
>>>>>>> Stashed changes
std::vector<std::string>&		Server::getMethodsReference(void) { return _methods; }

std::map<std::string, Location> Server::getLocations(void) const { return _locations; } //shouldnt this be a poiter?
=======
std::map<std::string, Location> Server::getLocations(void) const { return _locations; } //shouldnt this be a poiter?
=======
std::map<std::string, Location>& Server::getLocations(void)  { return _locations; } //shouldnt this be a poiter?
>>>>>>> Stashed changes
>>>>>>> Stashed changes
int								Server::getSocketFd(void) const { return _socket_fd; }
struct sockaddr_in*       		Server::getSockAddr(void) const { return _address_in; }
std::vector<int>				Server::getClientFds(void) const { return _client_fds; }

/* Setters */
void	Server::setPort(int& port) { _port = port; }
void	Server::setRoot(std::string& root) { _root = root; }
void	Server::setIndex(std::string& index) { _index = index; }
void	Server::setClientBody(int& client_body_size) { _client_body_size = client_body_size; }

/* Adders */
void	Server::addToName(std::string& name) { _names.push_back(name); }
void	Server::addToMethod(std::string& method) {_methods.push_back(method); }
void	Server::addToLocations(std::string &name, Location& location)
{
	_locations.insert(std::pair<std::string, Location>(name, location));
}

/* Public Functions */ 
void	Server::setup()
{
	makeSocketAddr();
	createSocket();
	setupSocket();
	fcntl(_socket_fd, F_SETFL, O_NONBLOCK);
	std::cout << "Server configured at: " << _names.at(0) << ":" << _port << "\n";
}

int		Server::acceptConnection()
{
	struct  sockaddr_in			client_addr;
	socklen_t					client_addrlen = sizeof(client_addr);
	

	int	clientFd = accept(_socket_fd, (struct sockaddr*)(&client_addr), &client_addrlen);
	if (clientFd != -1 && DEBUG)
	{
		std::cout << "["<<_names.back() << ":" << _port <<   "] ";
		std::cout << "Accepted connection from adress: " << client_addr.sin_addr.s_addr <<  "\n";
	}
	_client_fds.push_back(clientFd);
	return (clientFd);
}

bool	Server::isClientFdInServer(int fd)
{
	if (std::find(_client_fds.begin(), _client_fds.end(), fd) != _client_fds.end())
		return true;
	else
		return false;
}

/* Helper Functions */

void	Server::setupSocket()
{
	if (bind(_socket_fd, (const struct sockaddr *)_address_in, sizeof(*_address_in)) < 0)
	{
		std::perror("In Bind: ");
		std::exit(ERROR);
	}

	if (listen(_socket_fd, 5) < 0)
	{
		std::perror("In listen: ");
		std::exit(ERROR);
	}
}

void	Server::createSocket(void)
{
	int32_t socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		std::exit(EXIT_FAILURE);
	}
	int reuse = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
   		std::perror("setsockopt(SO_REUSEADDR) failed");
	_socket_fd = socket_fd;
}

void	Server::makeSocketAddr()
{
	struct sockaddr_in *address = new sockaddr_in();

	address->sin_family = AF_INET;
	address->sin_port = htons(_port); // <- Converts from host byte order to network byte order.
	address->sin_addr.s_addr = INADDR_ANY;
	memset(address->sin_zero, 0, sizeof(address->sin_zero));
	_address_in = address;
}

/* Stream overload */

std::ostream& operator<<(std::ostream& stream, const Server& server)
{
	stream << "Name(s) [";
	for(size_t i = 0; i < server.getNames().size(); i++)
        stream << server.getNames().at(i) << " ";
	stream << "]\n";
	stream << "Port: [" << server.getPort() << "]\n";
	stream << "Root: [" << server.getRoot() << "]\n";
	stream << "Index: ["  << server.getIndex() << "]\n";
	stream << "ClientBodySize: [" << server.getClientBodySize() << "]\n";
	stream << "Methods [";
	for(size_t i = 0; i < server.getMethods().size(); i++) 
        stream << server.getMethods().at(i) << " ";
	stream << "]\n";
	stream << "SocketFD: [" << server.getSocketFd() << "]\n";
	stream << "ClientFD(s) [";
	for(size_t i = 0; i < server.getClientFds().size(); i++) 
        stream << server.getClientFds().at(i) << " ";
	stream << "]\n";

<<<<<<< Updated upstream
	//std::cout << server.getLocations().begin()->second << std::endl;

=======
<<<<<<< Updated upstream
	std::map<std::string, Location> locations = server.getLocations(); //this will be deleted here
	std::cout << locations.size() << " locsize\n";
	for (locIt it = locations.begin(); it != locations.end(); it++)
		stream << it->second << "\n";

=======
	//std::cout << server.getLocations().begin()->second << std::endl;

>>>>>>> Stashed changes
	//std::map<std::string, Location>::iterator it = server.getLocations().begin(); //this will be deleted here
	//std::cout << server.getLocations().size() << " locsize\n";
	//server.getLocations().begin()
	//while (it != server.getLocations().end())
	//	stream << &it->second << "\n";
<<<<<<< Updated upstream
=======
>>>>>>> Stashed changes
>>>>>>> Stashed changes
    return stream;
}

