/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ThrowTests.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: xvoorvaa <xvoorvaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 13:40:28 by xvoorvaa      #+#    #+#                 */
/*   Updated: 2022/10/28 13:41:55 by xvoorvaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <catch2/catch_test_macros.hpp>

#include "webserv.h"

TEST_CASE("Using a string that does not exist in getHashMapstring")
{
	Server server;
	Exchange exchange(server, 0);

	REQUIRE_THROWS(exchange.getHashMapString("StrDoesNotExists"));
}
