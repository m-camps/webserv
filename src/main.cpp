/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 16:55:08 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/10 13:06:11 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::
#include <sys/socket.h> // socket(), bind()
#include <netinet/in.h> // INADDR_ANY
#include <unistd.h> // Write & Read
#include <fcntl.h>

//#include "Exchange/Exchange.hpp"
#include "../inc/Server.hpp"
#include "../inc/Parse.hpp"
#include "../inc/Network.hpp"

int32_t	main(int argc, char *argv[])
{
	(void)argc;
	if (argc != 2)
		std::exit(EXIT_FAILURE);
    (void) argv;

	Network network;

	network.setup(argv[1]);
	network.run();
	return (0);
}
