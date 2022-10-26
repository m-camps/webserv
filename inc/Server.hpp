/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:56:05 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/19 14:08:08 by mcamps        ########   odam.nl         */
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

typedef std::map<std::string, Location>::iterator locIt;
typedef std::map<std::vector<int>, std::string>::iterator errorIt;
typedef std::map<std::vector<int>, std::string> ErrorMap;

class Server
{
	public:
		Server();
		~Server();
		Server(const Server& src);

		/* Getters */
		//int								getPort(void) const;
		std::vector<int>					getPorts(void) const;

		std::vector<std::string>			getNames(void) const;
		std::string							getRoot(void) const;
		std::string							getIndex(void) const;
		int									getClientBodySize(void) const;
		std::vector<std::string>			getMethods(void) const;
		std::vector<std::string>&			getMethodsReference(void);
		std::map<std::string, Location>		getLocations(void) const;
		int									getSocketFd(void) const;
		struct sockaddr_in*					getSockAddr(void) const;
		std::vector<int>					getClientFds(void) const;
		ErrorMap&	                        getErrorPageRef(void); //reference?
		ErrorMap                          	getErrorPage(void) const; //reference?

		/* Setters */
		//void	setPort(int& port);
		void	setPort(int& port);
		void	setRoot(std::string& root);
		void	setIndex(std::string& index);
		void	setClientBody(int& client_body_size);

		void	setListenFlag(void); //new setters for minimum required flags
		void	setServerNameFlag(void); //new setters for minimum required flags
		bool	minimumRequiredAttributesProvided(void);


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
		std::vector<int>							_port;
		std::vector<std::string>					_names; 					// can be searching for multiple names
		std::string									_root; 						// default = "/"
		std::string									_index; 					// default = "index.html"
		int											_client_body_size;			// default = 10;
		std::vector<std::string>					_methods;					// default = ["GET", "POST", "DELETE"]
		struct sockaddr_in*							_address_in; 				// Optional can be deleted
		int											_socket_fd; 				// Socket FD the server is running on
		std::vector<int>							_client_fds;				// Client FD's currently associated to this server
		std::map<std::string, Location>				_locations;					// All locations of the Server
		std::map<std::vector<int>, std::string>		_error_pages;				//should be vector of ints as the first part of the map?
		bool										_listen_set; 				//setters for these?
		bool										_servername_set;  			//setters for these?

		//struct to check if all flags etc was correct?
		/* Helper Functions */
		void	makeSocketAddr();
		void	createSocket();
		void	setupSocket();
};

/* Stream overload */
std::ostream& operator<<(std::ostream& stream, const Server& Server);

#endif
