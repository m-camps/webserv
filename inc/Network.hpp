/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Network.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:38:04 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/05 17:24:05 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"
#include "Parse.hpp"
#include <poll.h>
#include <iostream>
#include <fstream>

#define	MAX_FD 5

class Network
{
	public:
		Network();
		~Network();
		void setup(std::string file);
		void run();

	private:
		Network(const Network &obj);
		Network &operator=(const Network &obj);

		void	createFds();

		std::vector<Server> 					_servers; // All the servers
		struct pollfd*							_fds; //Struct used for the FD's poll monitors
		std::vector<std::vector<std::string> > 	_serverConfigs; // Vector that holds the server configs
		int										_total_fd;
		int										_max_fd;

		/* Helper methods */
		bool		isSocketFd(int fd);
		Server* 	getServerBySocketFd(int fd);
		void		addToPollFds(int fd);
};
