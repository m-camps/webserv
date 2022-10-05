/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 16:55:08 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/05 11:15:49 by mcamps        ########   odam.nl         */
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

const int32_t SUCCES = 0;
const int32_t ERROR = 1;

int32_t GetSocket(void)
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

	address.sin_family = AF_INET;
	address.sin_port = htons(std::stol(server.getPort().back())); // <- Converts from host byte order to network byte order.
	address.sin_addr.s_addr = INADDR_ANY;
	memset(address.sin_zero, 0, sizeof(address.sin_zero));
	return (address);
}

/**
 * @Bind
 * When we talk about naming a socket, we are talking about assigning a
 * transport address to the socket (a port number in IP networking).
 * In sockets, this operation is called binding an address and the bind system call is used for this.
 *
 * The analogy is that of assigning a phone number to the line that you requested from the phone company
 * @Listen will "say" to the socket to check for incoming connections.
 *
 * @param address The address of the socket_fd
 * @param socket_fd
 */
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

int32_t	main(int argc, char *argv[])
{
	if (argc != 2)
		std::exit(EXIT_FAILURE);

	Server server;
	startParse(server, argv[1]);
	struct sockaddr_in address = GetSockaddr(server);
	int32_t socket_fd = GetSocket();
	int32_t addrlen = sizeof(address);

	SetupSocket(&address, &socket_fd);


	std::string hello = "Hello World";
    int32_t ListenSocket = 0;

    fcntl(ListenSocket, F_SETFL, O_NONBLOCK);
	while (true) {
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
