/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:04 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/12 15:13:51 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Parse.hpp"
//#include "../src/Exchange/Exchange.hpp"
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
		ServerConfig 							_serverConfigs; 	// Vector that holds the server configs (can be deleted later on)
		struct pollfd*							_fds; 				// Struct used for the FD's poll monitors
		int										_total_fd;			// Total active fd's
		int										_max_fd; 			// Max total fd's

		/* Orthodox canonical class BS */
		Network(const Network &obj);
		Network &operator=(const Network &obj);

		/* Setup methods */
		void	createFds();

		/* _fds manipulation */
		void			addToPollFds(int fd);
		void			delFromPollFds(int i);
		struct pollfd 	createNewPollfd(int fd);
		
		/* Helper methods */
		bool		isSocketFd(int fd);
		Server* 	getServerBySocketFd(int fd);
		Server*		getServerByClientFd(int fd);
};
