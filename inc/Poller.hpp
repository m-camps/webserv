/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Poller.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 17:26:47 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/15 18:31:06 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../src/Exchange/Request.hpp"

class Poller {
	private:
		
	public:
		Poller();
		~Poller();

		std::string		readString;
		bool			readyToWrite;
		HashMap			requestData;
		size_t			contentLength;
		size_t			readLength;
};

