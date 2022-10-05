/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 16:55:08 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/05 15:05:54 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::
#include <sys/socket.h> // socket(), bind()
#include <netinet/in.h> // INADDR_ANY
#include <unistd.h> // Write & Read
#include <fcntl.h>

#include "Exchange/Exchange.hpp"
#include "../inc/Server.hpp"
#include "../inc/Parse.hpp"
#include "../inc/Network.hpp"

const int32_t SUCCES = 0;


int32_t createSocket(void)
{
	int32_t socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) {
		std::exit(EXIT_FAILURE);
	}
	return (socket_fd);
}

struct sockaddr_in GetSockaddr(Server& server)
{
	struct sockaddr_in address = {};
	(void)server;

	address.sin_family = AF_INET;
	address.sin_port = htons(80); // <- Converts from host byte order to network byte order.
	address.sin_addr.s_addr = INADDR_ANY;
	memset(address.sin_zero, 0, sizeof(address.sin_zero));
	return (address);
}

void SetupSocket(struct sockaddr_in *address, const int32_t *socket_fd)
{
	if (bind(*socket_fd, (const struct sockaddr *)address, sizeof(*address)) < 0)
	{
		std::perror("In Bind: ");
		std::exit(ERROR);
	}

	if (listen(*socket_fd, 4) < 0)
	{
		std::perror("In listen: ");
		std::exit(ERROR);
	}
}


// int32_t	main(int argc, char *argv[])
// {
// 	(void)argc;
// 	(void)argv;
	
// 	Network test;
// 	return (SUCCES);
// }

int32_t	main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	if (argc != 2)
		std::exit(EXIT_FAILURE);

	Server server;
	Network network;

	network.setup(argv[1]);
	network.run();
	// startParse(server, argv[1]);
	struct sockaddr_in address = GetSockaddr(server);
	int32_t socket_fd = createSocket();
	int32_t addrlen = sizeof(address);

	SetupSocket(&address, &socket_fd);

	std::string hello = "Hello World";
    int32_t ListenSocket = 0;

    fcntl(ListenSocket, F_SETFL, O_NONBLOCK);
	while (false) {
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n" << '\n';
		ListenSocket = accept(socket_fd, reinterpret_cast<sockaddr *>(&address),
		                            reinterpret_cast<socklen_t *>(&addrlen));
		if (ListenSocket < 0)
		{
			std::perror("In Accept: " );
			return (ERROR);
		}

		char buffer[30000] = {0};
		if (read( ListenSocket , buffer, 30000) < 0)
			std::exit(EXIT_FAILURE);
        Exchange Base(buffer);

		if (write(ListenSocket , &hello , hello.size() + 1) < 0)
			std::exit(EXIT_FAILURE);

		close(ListenSocket);
	}

	return (SUCCES);
}
