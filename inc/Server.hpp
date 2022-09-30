/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:48:50 by mcamps        #+#    #+#                 */
/*   Updated: 2022/09/30 15:51:05 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <list>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
	public:
		Server();
		void	printServerAttributes(Server& server);
		std::vector<std::string>				_port; //listen
		std::vector<std::string>				_name; //can be searching for multiple names
		std::vector<std::string>				_root; //default = "/"
		std::vector<std::string>				_index; //default = "index.html"
		std::string								_clientBodySize;	//default = 10;
		std::list<std::string>					_methods[3]; // default = ["GET", "POST", "DELETE"]
		int										_socket_fd;
		int										_sockets[4];
		struct sockaddr_in						_address;
		size_t                                  _addressLen;
		//std::map<std::string, struct location>	locations<location_path, location>;
};