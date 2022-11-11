/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:04 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/11 12:00:12 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Parse.hpp"
#include "../src/Exchange/Exchange.hpp"
#include <poll.h>
#include <iostream>
#include <fstream>

typedef std::vector<std::vector<std::string> > ServerConfig; 

class Network
{
	public:
		Network();
		~Network();
		void setup(std::string file);
		void run();

	private:
		std::vector<Server> 					_servers; 			// All the servers
		std::vector<struct pollfd>				_poll;
		std::map<int, int>						_port_fds;			// All ports linked to a fd
		std::map<int, std::string>				_buffer;			// All incoming messages in a buffer
		
		/* Orthodox canonical class BS */
		Network(const Network &obj);
		Network &operator=(const Network &obj);

		/* Setup methods */
		void	createFds();

		/* Poll */
		struct pollfd 			newPoll(int fd);
		void					createPoll(void);
		
		/* Setup Sockets*/
		void					setupSocketFds(void);
		std::vector<int>		extractListens(void);
		int						createSocket(void);
		struct sockaddr_in *	makeSocketAddr(int port);
		void					bind(int socket_fd, struct sockaddr_in* address_in);
		void					listen(int socket_fd);

		void					linkSocketsToServers(void);

		/* Helper methods */
		bool		isSocketFd(int fd);
		Server* 	getServerBySocketFd(int fd);
		Server*		getServerByClientFd(int fd);
};
