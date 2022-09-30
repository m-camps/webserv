/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/30 15:50:41 by mcamps        #+#    #+#                 */
/*   Updated: 2022/09/30 15:56:57 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::printServerAttributes(Server& server)
{
    for(int i=0; i < server._port.size(); i++) //print Ports
    {
        std::cout << server._port << " ";
    }
    std::cout << "are ports of the server." << std::endl;

    for(int i=0; i < server._name.size(); i++) //print server_names
    {
        std::cout << server._name.at(i) << " ";
    }
    std::cout << " are the server names." << std::endl;

    for(int i=0; i < server._root.size(); i++) //print root locations
    {
        std::cout << server._root.at(i) << " ";
    }
    std::cout << " are the root names." << std::endl;
    
     for(int i=0; i < server._index.size(); i++) //print indexes
    {
        std::cout << server._index.at(i) << " ";
    }
    std::cout << " are the index names." << std::endl;
    std::cout << server._clientBodySize << " is the client body size." << std::endl;
    return ;
}