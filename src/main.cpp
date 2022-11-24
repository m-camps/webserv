/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 16:55:08 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/15 13:54:20 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::
#include <fcntl.h>
#define DEBUG false

#include "../inc/Network.hpp"

int32_t	main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Need a config file" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	Network network;

	try
	{	
		network.setup(argv[1]);
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
        return (ERROR);
	}
	network.run();
	return (SUCCES);
}
