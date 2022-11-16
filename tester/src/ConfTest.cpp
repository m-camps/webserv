//
// Created by Xander Voorvaart on 11/1/22.
//

#include <catch2/catch_test_macros.hpp>

#include "Server.hpp"
#include "Network.hpp"
#include "Request.hpp"

#define VALID_PATH "tester/confs/Valid/"
#define INVALID_PATH "tester/confs/Invalid/"

void	ASSERT_VALID_CONFIG(std::string file)
{
	std::string configFile = VALID_PATH + file;
	SECTION("--- " + file + " ---")
	{
		Parse parser;
		std::vector<Server> tmp;
		REQUIRE_NOTHROW(parser.parseNetwork(configFile, tmp));
	}
}

void	ASSERT_INVALID_CONFIG(std::string file)
{
	std::string configFile = INVALID_PATH + file;
	SECTION("--- " + file + " ---")
	{
		Parse parser;
		std::vector<Server> tmp;
		REQUIRE_THROWS(parser.parseNetwork(configFile, tmp));
	}
}

TEST_CASE("Valid Configs")
{
	ASSERT_VALID_CONFIG("Basic.conf");
	ASSERT_VALID_CONFIG("MultipleListen.conf");
	ASSERT_VALID_CONFIG("MultipleServers.conf");
	ASSERT_VALID_CONFIG("NoLocation.conf");
}

TEST_CASE("Invalid Configs")
{
	ASSERT_INVALID_CONFIG("DuplicateListen.conf");
}