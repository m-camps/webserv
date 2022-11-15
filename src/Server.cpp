/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 15:36:19 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/15 11:36:17 by mcamps        ########   odam.nl         */
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
std::map<int, std::string> 						Server::getErrorPage(void) const { return _error_pages; }

/* Setters */
void	Server::setClientBody(int client_body_size) { _client_body_size = client_body_size; }
void	Server::setLocations(std::map<std::string, Location>& locations) { _locations = locations; }

/* Adders */
void	Server::addToNames(std::string& name) { _names.push_back(name); }
void	Server::addToPorts(int &port) { _ports.push_back(port); }

void	Server::addToLocations(std::string& name, Location& location)
{
	_locations.insert(std::pair<std::string, Location>(name, location));
}

void	Server::addToErrorPages(int& status, std::string& page)
{
	_error_pages.insert(std::pair<int, std::string>(status, page));
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

