/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 14:23:12 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/23 15:01:29 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Request.hpp"

class Client {
	private:
		
	public:
		Client(Servers servers);
		~Client();

		std::string		readString;
		bool			readyToWrite;
		HashMap			requestData;
		size_t			contentLength;
		size_t			readLength;
		Servers			servers;
};

