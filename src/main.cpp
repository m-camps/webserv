/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 16:55:08 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/30 15:53:06 by xvoorvaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::
#include <fcntl.h>
#define DEBUG false

#include "Network.hpp"

int32_t	main(int argc, char *argv[], char **envp)
{
	if (argc != 2)
	{
		std::cerr << "Need a config file" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	Network network;
	
	network.setup(argv[1]);

	// char *p = "SERVER_NAME=LOCALHOST";
	// putenv(p);
	// for (char **env = envp; *env != 0; env++)
	// {
	// 	char *thisEnv = *env;
	// 	printf("%s\n", thisEnv); 
	// 	//add some serveretc stuff to this env because of CGI  
	// }
	
	network.run();
	return (0);
}
