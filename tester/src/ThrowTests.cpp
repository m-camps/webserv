//
// Created by Xander Voorvaart on 10/27/22.
//

#include <catch2/catch_test_macros.hpp>

#include "../../inc/Server.hpp"
#include "../../src/Exchange/Exchange.hpp"


#include <vector>

	TEST_CASE("Adding the same word twice throws an exception")
	{
		Server server;
		Exchange exchange(server, 0);

		REQUIRE_THROWS(exchange.getHashMapString("StrDoesNotExists"));
	}
