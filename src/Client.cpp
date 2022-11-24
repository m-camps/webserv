/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 17:27:20 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/23 15:01:04 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Client.hpp"

Client::Client(Servers servers) : servers(servers)
{
	readString = "";
	readyToWrite = false;
	contentLength = 0;
	readLength = 0;
}

Client::~Client()
{

}

