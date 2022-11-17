/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:56:05 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/15 11:36:13 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"
#include "Constants.hpp"

#include <string>
#include <map>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>

typedef std::map<std::string, Location>::iterator locIt;

class Server
{
	public:
		Server();
		~Server();

		/* Getters */
		std::vector<int>					getPorts(void) const;
		std::vector<std::string>			getNames(void) const;
		int									getClientBodySize(void) const;
		std::map<std::string, Location>		getLocations(void) const;
		std::map<int, std::string>          getErrorPage(void) const;

		/* Setters */
		void	setClientBody(int client_body_size);
		void	setLocations(std::map<std::string, Location>& locations);

		/* Adders */
		void	addToPorts(int &port);
		void	addToNames(std::string& name);
		void	addToLocations(std::string& name, Location& location);
		void	addToErrorPages(int& error_nb, std::string& error_page);

	private:
		std::vector<int>							_ports;
		std::vector<std::string>					_names; 					// can be searching for multiple names
		int											_client_body_size;			// default = 10;
		std::vector<std::string>					_methods;					// default = ["GET", "POST", "DELETE"]
		std::map<std::string, Location>				_locations;					// All locations of the Server
		std::map<int, std::string>					_error_pages;				// Map of error pages
};

typedef std::vector<Server>	Servers;
/* Stream overload */
std::ostream& operator<<(std::ostream& stream, const Server& Server);
