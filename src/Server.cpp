/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 15:36:19 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/09 17:59:47 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Location.hpp"
#include "../inc/Constants.hpp"
#include <iostream>

Server::Server()
{
	_client_body_size = -1;
}

Server::~Server() { return; }

/* Getters */
std::vector<int>								Server::getPorts(void)  const { return _ports; }
std::vector<std::string>						Server::getNames(void) const { return _names; }
int												Server::getClientBodySize(void) const { return _client_body_size; }
std::map<std::string, Location> 				Server::getLocations(void) const { return _locations; }
std::vector<int>								Server::getSocketFds(void) const { return _socket_fds; }
std::vector<int>								Server::getClientFds(void) const { return _client_fds; }
std::map<int, std::string> 						Server::getErrorPage(void) const { return _error_pages; }

/* Setters */
void	Server::setClientBody(int& client_body_size) { _client_body_size = client_body_size; }
void	Server::setLocations(std::map<std::string, Location>& locations) { _locations = locations; }

/* Adders */
void	Server::addToNames(std::string& name) { _names.push_back(name); }
void	Server::addToPorts(int &port) { _ports.push_back(port); }
void	Server::addToSocketFds(int socket_fd) { _socket_fds.push_back(socket_fd); }

void	Server::addToLocations(std::string& name, Location& location)
{
	_locations.insert(std::pair<std::string, Location>(name, location));
}

void	Server::addToErrorPages(int& status, std::string& page)
{
	_error_pages.insert(std::pair<int, std::string>(status, page));
}

/* Public Functions */
int		Server::acceptConnection(int& socket_fd)
{
	struct  sockaddr_in			client_addr;
	socklen_t					client_addrlen = sizeof(client_addr);
	
	int	clientFd = accept(socket_fd, (struct sockaddr*)(&client_addr), &client_addrlen);
	if (clientFd == -1)
	{
		std::perror("In accept: ");
		std::exit(EXIT_FAILURE);
	}
	if (clientFd != -1 && DEBUG)
	{
		std::cout << "["<<_names.back() << ":" << _ports.front() <<   "] "; //previously used with _port instead of +.back()
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

bool	Server::isSocketFdInServer(int fd)
{
	if (std::find(_socket_fds.begin(), _socket_fds.end(), fd) != _socket_fds.end())
		return true;
	else
		return false;
}

/* Stream overload */

std::ostream& operator<<(std::ostream& stream, const Server& server)
{
	stream << "Name(s) [";
	for(size_t i = 0; i < server.getNames().size(); i++)
        stream << server.getNames().at(i) << " ";
	stream << "]\n";
	stream << "Port(s): [";
	for(size_t i = 0; i < server.getPorts().size(); i++)
        stream << server.getPorts().at(i) << " ";
	stream << "]\n";
	stream << "ClientBodySize: [" << server.getClientBodySize() << "]\n";
	stream << "SocketFD(s) [";
	for(size_t i = 0; i < server.getSocketFds().size(); i++) 
        stream << server.getSocketFds().at(i) << " ";
	stream << "]\n";
	stream << "ClientFD(s) [";
	for(size_t i = 0; i < server.getClientFds().size(); i++) 
        stream << server.getClientFds().at(i) << " ";
	stream << "]\n";
	stream << "Error Pages: [";

	std::map<int, std::string> error_pages = server.getErrorPage(); 
	
	for (std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
		stream << "{"<< it->first << ", " << it->second << "} ";
	stream << "]\n";
	
	std::map<std::string, Location> locations = server.getLocations();

	for (locIt it = locations.begin(); it != locations.end(); it++)
		stream << it->second << "\n";

    return stream;
}

