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
#include "../inc/Server.hpp"
#include "../inc/Parse.hpp"
#include "Config.hpp"

void	print(std::string type,int print)
{
	std::cout << type << print << std::endl;
}

int main(int argc, char **argv)
{
		int new_socket = 1;
	if (argc > 2)
	{
		exit(1);
	}
	Server server;
	if (argc == 2)
	{
		startParse(server, argv[1]);
	}
	//server.printServerAttributes(server);

	//http://www.ccplusplus.com/2011/10/struct-sockaddrin.html for sockaddr struct
	struct sockaddr_in address;

	const int PORT = 8081; //port(s) we will take from a config file
	
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		std::cout << "can not create socket" << std::endl;
	}

	/* htonl converts a long integer (e.g. address) to a network representation */ 
	/* htons converts a short integer (e.g. port) to a network representation */

	memset((char *)&address, 0, sizeof(address)); //need to fill the struct
	size_t addrlen = sizeof(address);

	//set address to appropriate stuff
	address.sin_family = AF_INET; //adress of family, we used when setting up
	address.sin_port = htons(PORT); //transport address insteadof port serer.getport.setport
	address.sin_addr.s_addr = htonl(INADDR_ANY); //any adress of any interface

	//should load inet_pton(AF_INET, "given server_ip/name", &(address.sin_addr.s_addr))
	//check for error <= 0
	

	//check if the client is connecting to the server, does this exist?
	//first check the port, then hostname, does that location match,
	//two server same port, different names, if your connecting
	//request headers, called host it should say that example.com
	//use curl resolve

	//should set the socket to nonblocking, use poll later
	//https://jameshfisher.com/2017/04/05/set_socket_nonblocking/
	if (bind(server_fd, (struct sockaddr *)&address, addrlen))
	{
		std::cout << "can not bind socket" << std::endl;
	}

	if (listen(server_fd, 3) < 0) //listen to incoming connection from client
	{
		std::cout << "fail to listen not create socket" << std::endl;
	}

	while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		fcntl(new_socket, F_SETFL, O_NONBLOCK); //is this too late to set it to nonblocking?

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) //accept blocks the socket until a connection is present
        {
            perror("In accept failed\n");
            exit(EXIT_FAILURE);
        }
       // char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"; //minimal http header for sending back, send header back to clinet
        char buffer[30000] = {0};
        size_t valread = read( new_socket , buffer, 30000); //read the clients msg
		(void)valread;
        printf("%s\n",buffer ); //print it
       	//write(new_socket , hello , strlen(hello)); //send back a hello from server
       	printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
	return 0;
}


/*
int	main(int argc, char **argv)
{
	(void)argv;
	(void)argc;
	// if (argc != 2)
	// 	std::exit(1);

	//

	Config config = Config();

	struct sockaddr_in saddr;
	int socketfd = socket(AF_INET, SOCK_STREAM, 0); //server_fd

	memset(&saddr, '\0', sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY); //machines ip address, use any interface os picks
	saddr.sin_port = htons(config.port); //port number

	bind(socketfd, (struct sockaddr *) &saddr, sizeof(saddr)); //creates a file in the file system
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
			print("here/n", 1);
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
*/
