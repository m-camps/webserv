/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:56:05 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/09 12:18:51 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"
#include <string>
#include <map>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>

#define ERROR 1

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

		std::vector<int>					getSocketFds(void) const;
		std::vector<int>					getClientFds(void) const;

		/* Setters */
		void	setClientBody(int& client_body_size);
		void	setLocations(std::map<std::string, Location>& locations);

		/* Adders */
		void	addToPorts(int &port);
		void	addToNames(std::string& name);
		void	addToLocations(std::string& name, Location& location);
		void	addToErrorPages(int& error_nb, std::string& error_page);

		/* Delete */
		void	removeFromClientFds(int fd);

		/* Public Functions */
		void	changePort(std::string newPort);
		int		setup();
		int		acceptConnection(int& socket_fd);
		bool	isClientFdInServer(int fd);
		bool	isSocketFdInServer(int fd);

	private:
		std::vector<int>							_ports;
		std::vector<std::string>					_names; 					// can be searching for multiple names
		int											_client_body_size;			// default = 10;
		std::vector<std::string>					_methods;					// default = ["GET", "POST", "DELETE"]
		std::map<std::string, Location>				_locations;					// All locations of the Server
		std::map<int, std::string>					_error_pages;				// Map of error pages
		
		std::vector<int>							_socket_fds; 				// Socket FD the server is running on
		std::vector<int>							_client_fds;				// Client FD's currently associated to this server

		/* Helper Functions */
		struct sockaddr_in*	makeSocketAddr(int& port);
		int					createSocket(void);
		void				setupSocket(int& socket_fd, struct sockaddr_in* address_in);
};

/* Stream overload */
std::ostream& operator<<(std::ostream& stream, const Server& Server);
