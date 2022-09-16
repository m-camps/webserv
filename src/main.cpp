/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 16:55:08 by mcamps        #+#    #+#                 */
/*   Updated: 2022/09/16 17:25:44 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>

#include "Config.hpp"

void	print(std::string type,int print)
{
	std::cout << type << print << std::endl;
}


int	main(int argc, char **argv)
{
	(void)argv;
	(void)argc;
	// if (argc != 2)
	// 	std::exit(1);

	Config config = Config();

	struct sockaddr_in saddr;
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&saddr, '\0', sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(config.port);

	bind(socketfd, (struct sockaddr *) &saddr, sizeof(saddr));
	listen(socketfd, 5);

	struct sockaddr_in caddr;
	socklen_t clen = sizeof(caddr);
	
	int newSocket = accept(socketfd, (struct sockaddr *) &caddr, &clen);

	print("Port= ", saddr.sin_port);
	print("SocketFD= ",socketfd);

	// send(socketfd, "Hello world!\n",13, 0);
	char str[100];
	int ret;
	

	// struct pollfd *pollfd;

	// pollfd->fd = socketfd;

	// poll(pollfd,1, 1000);

	ret = 0;
	while(1){
		ret = read(newSocket, str, sizeof(str));
		std::cout << str << std::endl;
		memset(str, '\0', 100);
	};
	std::cout << str << std::endl;
	return (0);
}
