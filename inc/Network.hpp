/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:04 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/11 14:26:42 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Parse.hpp"
#include "../src/Exchange/Exchange.hpp"
#include <poll.h>
#include <iostream>
#include <fstream>

typedef std::vector<Server>	Servers;

class Network
{
	public:
		Network();
		~Network();
		void setup(std::string file);
		void run(void);

	private:
		Servers 								_servers; 			// All the servers
		std::vector<struct pollfd>				_poll;				// Vector containing all poll structs
		std::vector<int>						_socket_fds;				
		std::map<int, std::string>				_buffer;			// All incoming messages in a buffer
		std::map<int, Servers>					_fds; // <fd,std::vector<Server>> all servers corrosponding with that fd
		
		/* Orthodox canonical class BS */
		Network(const Network &obj);
		Network &operator=(const Network &obj);
		
		/* Fds */
		void					setupFds(int port, int socket_fd);
		void					addClientToFds(int socket_fd, int client_fd);
		void					delClientFromFds(int client_fd);

		/* Setup Sockets*/
		int						acceptConnection(int socket_fd);
		std::vector<int>		extractListens(void);
		void					setupSockets(void);
		int						createSocket(void);
		struct sockaddr_in *	makeSocketAddr(int port);
		void					bind(int socket_fd, struct sockaddr_in* address_in);
		void					listen(int socket_fd);

		/* Poll */
		void					createPoll(void);
		struct pollfd 			newPoll(int fd);

		/* Helper methods */
		bool		isSocketFd(int fd);
		Servers		getServersByFd(int fd);
};
