/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ThrowTests.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: xvoorvaa <xvoorvaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 13:40:28 by xvoorvaa      #+#    #+#                 */
/*   Updated: 2022/10/30 02:16:24 by xander        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <catch2/catch_test_macros.hpp>

#include "../../inc/Server.hpp"
#include "../../src/Exchange/Exchange.hpp"
#include "../../src/Exchange/Respond.hpp"
#include "../../src/Exchange/Request.hpp"

#define private public

#define CONFIG_CATCH_MAIN

TEST_CASE("Using a string that does not exist in getHashMapstring")
{
	Server server;
	Exchange exchange(server, 0);

	REQUIRE_THROWS(exchange.getHashMapString("StrDoesNotExists"));
}

TEST_CASE("Check Status Code of Responder")
{

}

TEST_CASE("Request class should cancel if there is no actual request")
{

}

TEST_CASE("Allowed Method vector is empty")
{
	std::vector<std::string> AllowedMethods;

	REQUIRE_FALSE(MethodIsAllowed("GET", AllowedMethods));
}
