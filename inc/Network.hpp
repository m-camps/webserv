/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:04 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/11 13:34:39 by mcamps        ########   odam.nl         */
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
		void run();

	private:
		Servers 								_servers; 			// All the servers
		std::vector<struct pollfd>				_poll;				// Vector containing all poll structs
		std::map<int, int>						_port_fds;			// <port, fd>
		std::vector<int>						_socket_fds;				
		std::map<int, std::string>				_buffer;			// All incoming messages in a buffer
		std::map<int, Servers>					_servers_in_socket; // <fd,std::vector<Server>> all servers corrosponding with that fd
		
		/* Orthodox canonical class BS */
		Network(const Network &obj);
		Network &operator=(const Network &obj);

		/* Poll */
		struct pollfd 			newPoll(int fd);
		void					createPoll(void);
		
		/* ServersInSocket */
		void					setupServersInSocket(int port, int socket_fd);
		void					addClientToServersInSocket(int socket_fd, int client_fd);
		void					delClientFromServersInSocket(int client_fd);

		/* Setup Sockets*/
		void					setupSocketFds(void);
		std::vector<int>		extractListens(void);
		int						createSocket(void);
		struct sockaddr_in *	makeSocketAddr(int port);
		void					bind(int socket_fd, struct sockaddr_in* address_in);
		void					listen(int socket_fd);

		int						acceptConnection(int socket_fd);
		void					linkSocketsToServers(void);

		/* Helper methods */
		bool		isSocketFd(int fd);
		Server* 	getServerBySocketFd(int fd);
		Server*		getServerByClientFd(int fd);
		Servers		getServersByFd(int fd);
};
