/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:04 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/23 15:33:09 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Parse.hpp"
#include "Exchange.hpp"
#include "Client.hpp"
#include <poll.h>
#include <iostream>
#include <fstream>

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
		std::map<int, Client>					_io;
		
		/* Orthodox canonical class BS */
		Network(const Network &obj);
		Network &operator=(const Network &obj);
		
		/* IO */
		void 					receiveData(int fd);
		void					sendResponse(int fd);
		void					setupIO(int port, int socket_fd);

		/* Setup Sockets*/
		void					acceptConnection(int socket_fd);
		void					closeConnection(int fd, int i);

		std::vector<int>		extractListens(void);
		void					setupSockets(void);
		int						createSocket(void);
		struct sockaddr_in  	makeSocketAddr(int port);
		void					bind(int socket_fd, struct sockaddr_in address_in);
		void					listen(int socket_fd);

		/* Poll */
		void					createPoll(void);
		struct pollfd 			newPoll(int fd);

		/* Helper methods */
		bool		isSocketFd(int fd);
		Servers		getServersByFd(int fd);
};
