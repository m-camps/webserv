/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ThrowTests.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: xvoorvaa <xvoorvaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 13:40:28 by xvoorvaa      #+#    #+#                 */
/*   Updated: 2022/10/30 16:22:39 by xvoorvaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <catch2/catch_test_macros.hpp>
#include <string>

#include "Server.hpp"
#include "Network.hpp"
#include "Exchange.hpp"
#include "Respond.hpp"
#include "Request.hpp"
#include "Generator.hpp"

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

TEST_CASE("Generator Test")
{
    Server server;
    Exchange exchanger(server, 0);

    SECTION("Exchange has a Body")
    {
        exchanger.setBody("Hallo");
        REQUIRE(Generator::generateChunk(exchanger) == "5\r\nHallo\r\n\r\n");
    }
    SECTION("Exchange has a no Body")
    {
        exchanger.setBody("");
        REQUIRE(Generator::generateChunk(exchanger) == "0\r\n\r\n\r\n");
    }
}
