//
// Created by Xander Voorvaart on 11/1/22.
//

#include <catch2/catch_test_macros.hpp>

#include "Server.hpp"
#include "Network.hpp"
#include "Request.hpp"

#define VALID_PATH "tester/confs/Valid/"
#define INVALID_PATH "tester/confs/Invalid/"

void	ASSERT_VALID_CONFIG(const std::string& file)
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
	ASSERT_VALID_CONFIG("Minimal.conf");
	ASSERT_VALID_CONFIG("basic_no_location.conf");
	ASSERT_VALID_CONFIG("basic_location.conf");
	ASSERT_VALID_CONFIG("MultipleListen.conf");
	ASSERT_VALID_CONFIG("MultipleServers.conf");
	ASSERT_VALID_CONFIG("ComplexConfig.conf");
	ASSERT_VALID_CONFIG("EmptyLocation.conf");
    ASSERT_VALID_CONFIG("ShortIndex.conf");
	ASSERT_VALID_CONFIG("NoServerName.conf"); // <-- Default?
	ASSERT_VALID_CONFIG("EmptyLine.conf"); // Is this possible?
}

TEST_CASE("Invalid Configs")
{
	ASSERT_INVALID_CONFIG("ClientBodySizeWithoutDigit.conf");
	ASSERT_INVALID_CONFIG("DuplicateServerName.conf");
	ASSERT_INVALID_CONFIG("DuplicateListen.conf");
	ASSERT_INVALID_CONFIG("NoBracketLocation.conf");
	ASSERT_INVALID_CONFIG("NoBracketServer.conf");
	ASSERT_INVALID_CONFIG("NoListenValue.conf");
	ASSERT_INVALID_CONFIG("EmptyServer.conf");
	ASSERT_INVALID_CONFIG("NoLocationAutoIndex.conf");
	ASSERT_INVALID_CONFIG("NoLocationCgiExt.conf");
	ASSERT_INVALID_CONFIG("NoLocationCgiName.conf");
	ASSERT_INVALID_CONFIG("NoNameLocation.conf");
	ASSERT_INVALID_CONFIG("NotImplementedMethod.conf");
	ASSERT_INVALID_CONFIG("ConfThatDoesNotExists");
}
