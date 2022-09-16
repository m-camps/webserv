/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/16 15:31:38 by mcamps        #+#    #+#                 */
/*   Updated: 2022/09/16 18:04:05 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>


int main(void)
{
	struct sockaddr_in saddr;
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&saddr, '\0', sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(80);

	connect(socketfd, (struct sockaddr *) &saddr, sizeof(saddr));
	std::cout << "Connected to server" << std::endl;

	while(1){
		std::string msg;
		std::cin >> msg;
		write(socketfd, msg.c_str(), msg.length());
	}
}