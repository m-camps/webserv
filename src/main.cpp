/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 16:55:08 by mcamps        #+#    #+#                 */
/*   Updated: 2022/09/16 18:06:34 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

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
	
	int socket[2];
	for (int i = 0; i < 2; i++)
	{
		socket[i] = accept(socketfd, (struct sockaddr *) &caddr, &clen);
		std::cout << "Client connected: " << i << std::endl;
		fcntl(socket[i], F_SETFL, O_NONBLOCK);
	}

	print("Port= ", ntohs(saddr.sin_port));
	print("SocketFD= ",socketfd);

	// send(socketfd, "Hello world!\n",13, 0);
	char str[100];
	int ret;
	

	// struct pollfd *pollfd;

	// pollfd->fd = socketfd;

	// poll(pollfd,1, 1000);

	ret = 0;
	memset(str, '\0', 100);
	while(true)
	{
		for (int i = 0; i < 2; i++)
		{
			ret = read(socket[i], str, sizeof(str));
			if (ret > 0)
				std::cout << ret << "\n";
			if (str[0] != '\0')
			{
				std::cout << str << std::endl;
				memset(str, '\0', 100);
			}
		}
	}
	return (0);
}
