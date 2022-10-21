/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:56:05 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/17 16:51:58 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

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

class Server
{
	public:
		Server();
		~Server();
		Server(const Server& src);
		Server& operator=(const Server& rhs);

		/* Getters */
		int									getPort(void) const;
		std::vector<std::string>			getNames(void) const;
		std::string							getRoot(void) const;
		std::string							getIndex(void) const;
		int									getClientBodySize(void) const;
		std::vector<std::string>			getMethods(void) const;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		std::map<std::string, Location>&	getLocations(void) ;
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
		std::vector<std::string>&			getMethodsReference(void);

		std::map<std::string, Location>		getLocations(void) const;
>>>>>>> Stashed changes
=======
		std::map<std::string, Location>&	getLocations(void) ;
>>>>>>> Stashed changes
		int									getSocketFd(void) const;
		struct sockaddr_in*					getSockAddr(void) const;
		std::vector<int>					getClientFds(void) const;
		std::string							getErrorPage(int statuscode) const;

		/* Setters */
		void	setPort(int& port);
		void	setRoot(std::string& root);
		void	setIndex(std::string& index);
		void	setClientBody(int& client_body_size);
	

		/* Adders */
		void	addToName(std::string& name);
		void	addToMethod(std::string& method);
		void	addToLocations(std::string &name, Location& location);

		/* DEBUG */
		void	printServerAttributes(Server& server);

		/* Public Functions */
		void	changePort(std::string newPort);
		void	setup();
		int		acceptConnection();
		bool	isClientFdInServer(int fd);

	private:

		int								_port; 						// The port server is running on, can maybe be multiple ports?
		std::vector<std::string>		_names; 					// can be searching for multiple names
		std::string						_root; 						// default = "/"
		std::string						_index; 					// default = "index.html"
		int								_client_body_size;			// default = 10;
		std::vector<std::string>		_methods;					// default = ["GET", "POST", "DELETE"]
		struct sockaddr_in*				_address_in; 				// Optional can be deleted
		int								_socket_fd; 				// Socket FD the server is running on
		std::vector<int>				_client_fds;				// Client FD's currently associated to this server
		std::map<std::string, Location>	_locations;					// All locations of the Server
		std::map<int, std::string>		_erorr_pages;				//

		/* Helper Functions */
		void	makeSocketAddr();
		void	createSocket();
		void	setupSocket();
};

/* Stream overload */
std::ostream& operator<<(std::ostream& stream, const Server& Server);

#endif
