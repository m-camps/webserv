/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ThrowTests.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: xvoorvaa <xvoorvaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 13:40:28 by xvoorvaa      #+#    #+#                 */
/*   Updated: 2022/10/30 14:06:00 by xvoorvaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <catch2/catch_test_macros.hpp>

#include "../../inc/Server.hpp"
#include "../../inc/Network.hpp"
#include "../../src/Exchange/Exchange.hpp"
#include "../../src/Exchange/Respond.hpp"
#include "../../src/Exchange/Request.hpp"

#include <string>

TEST_CASE("Using a string that does not exist in getHashMapstring")
{
	Server server;
	Exchange exchange(server, 0);

	REQUIRE_THROWS(exchange.getHashMapString("StrDoesNotExists"));
}

TEST_CASE("Non-Existing File")
{
	REQUIRE_THROWS(readFile("A_Invalid_File"));
}

TEST_CASE("Allowed Method vector is empty")
{
	std::vector<std::string> AllowedMethods;

	REQUIRE_FALSE(MethodIsAllowed("GET", AllowedMethods));
}

TEST_CASE("Output of readFile is valid")
{
	SECTION("Reads the same amount of bites as its file")
	{
		std::string FileContent = readFile("./tester/data/a_file.txt");

		REQUIRE(FileContent.length() == 46);
	}
	SECTION("Reads a picture of 35,207,718 bytes")
	{
		std::string FileContent = readFile("./tester/data/big_picture.png");

		REQUIRE(FileContent.length() == 35'207'718);
	}
}

TEST_CASE("deleteFile should Throw, if file does not exists")
{
	std::string path = "APathThatWillNotExist";

	REQUIRE_THROWS(deleteFile(path));
}

TEST_CASE("Check if function exits out of program")
{
	SECTION("acceptConnection() exits")
	{
		Server tmpServer;
		pid_t ChildPid = fork();

		if (ChildPid < 0)
			return ;
		if (ChildPid == 0)
		{
			REQUIRE_FALSE(tmpServer.acceptConnection());
		}
	}

	SECTION("run() exits")
	{
		Network tmpNetwork;
		pid_t ChildPid = fork();

		if (ChildPid < 0)
			return ;
		if (ChildPid == 0)
		{
			// CHECK(tmpNetwork.run());
		}
	}
}
