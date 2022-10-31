/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:56:05 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/31 16:05:54 by mcamps        ########   odam.nl         */
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
		std::string							getRoot(void) const;
		std::string							getIndex(void) const;
		int									getClientBodySize(void) const;
		std::vector<std::string>			getMethods(void) const;
		std::map<std::string, Location>		getLocations(void) const;
		int									getSocketFd(void) const;
		struct sockaddr_in*					getSockAddr(void) const;
		std::vector<int>					getClientFds(void) const;
		std::map<int, std::string>          getErrorPage(void) const;

		/* Setters */
		void	setRoot(std::string& root);
		void	setIndex(std::string& index);
		void	setClientBody(int& client_body_size);

		/* Adders */
		void	addToPorts(int &port);
		void	addToNames(std::string& name);
		void	addToMethods(std::string& method);
		void	addToLocations(std::string& name, Location& location);
		void	addToErrorPages(int& error_nb, std::string& error_page);

		/* Delete */
		void	removeFromClientFds(int fd);

		/* Public Functions */
		void	changePort(std::string newPort);
		void	setup();
		int		acceptConnection();
		bool	isClientFdInServer(int fd);


	private:
		std::vector<int>							_ports;
		std::vector<std::string>					_names; 					// can be searching for multiple names
		std::string									_root; 						// default = "/"
		std::string									_index; 					// default = "index.html"
		int											_client_body_size;			// default = 10;
		std::vector<std::string>					_methods;					// default = ["GET", "POST", "DELETE"]
		struct sockaddr_in*							_address_in; 				// Optional can be deleted
		int											_socket_fd; 				// Socket FD the server is running on
		std::vector<int>							_client_fds;				// Client FD's currently associated to this server
		std::map<std::string, Location>				_locations;					// All locations of the Server
		std::map<int, std::string>					_error_pages;				//should be vector of ints as the first part of the map?
		bool										_listen_set; 				//setters for these?
		bool										_servername_set;  			//setters for these?

		/* Helper Functions */
		void	makeSocketAddr();
		void	createSocket();
		void	setupSocket();
};

/* Stream overload */
std::ostream& operator<<(std::ostream& stream, const Server& Server);
