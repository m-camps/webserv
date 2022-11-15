/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Poller.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 17:27:20 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/15 17:35:00 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Poller.hpp"

Poller::Poller()
{
	readString = "";
	readyToWrite = false;
}

Poller::~Poller()
{

}

