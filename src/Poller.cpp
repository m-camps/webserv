/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Poller.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 17:27:20 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/15 18:33:58 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Poller.hpp"

Poller::Poller()
{
	readString = "";
	readyToWrite = false;
	contentLength = 0;
	readLength = 0;
}

Poller::~Poller()
{

}

