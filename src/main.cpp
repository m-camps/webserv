/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 16:55:08 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/19 15:32:42 by xvoorvaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::
#include <sys/socket.h> // socket(), bind()
#include <netinet/in.h> // INADDR_ANY
#include <unistd.h> // Write & Read
#include <fcntl.h>
#define DEBUG false

#include "Exchange/Request.hpp"
#include "../inc/Server.hpp"
#include "../inc/Parse.hpp"
#include "../inc/Network.hpp"

int32_t	main(int argc, char *argv[])
{
	(void)argc;
	if (argc != 2)
	{
		std::cerr << "Need a config file" << std::endl;
		std::exit(EXIT_FAILURE);
    // (void) argv;

	Network network;

	network.setup(argv[1]);
	network.run();
	
	return (0);
}
