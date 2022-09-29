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


#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    char hello[18] = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form //essentially here could be the adress where we have to connect to servers[i].ip:port
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) //makes 127.0.1 into a binary format into the sin_addr
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent from client!\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}

/*
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
*/