//
// Created by Xander Voorvaart on 11/1/22.
//

#include <catch2/catch_test_macros.hpp>

#include "Server.hpp"
#include "Network.hpp"
#include "Request.hpp"

#define VALID_PATH "tester/confs/Valid/"
#define INVALID_PATH "tester/confs/Invalid/"

TEST_CASE("Valid Configs")
{
    SECTION("--- Basic.conf ---")
    {
        Network network;
        REQUIRE_NOTHROW(network.setup(VALID_PATH"basic.conf"));
    }
    SECTION("--- MoreListen.conf ---")
    {
        Network network;
        REQUIRE_NOTHROW(network.setup(VALID_PATH"MoreListen.conf"));
    }
    SECTION("--- MoreListenDup.conf ---")
    {
        Network network;
        REQUIRE_NOTHROW(network.setup(VALID_PATH"MoreListenDup.conf"));
    }
    SECTION("--- NoLocation.conf ---")
    {
        Network network;
        REQUIRE_NOTHROW(network.setup(VALID_PATH"NoLocation.conf"));
    }
    SECTION("--- NoLocation.conf ---")
    {
        Network network;
        REQUIRE_NOTHROW(network.setup(VALID_PATH"NoLocation.conf"));
    }
    SECTION("--- NoLocation.conf ---")
    {
        Network network;
        REQUIRE_NOTHROW(network.setup(VALID_PATH"NoLocation.conf"));
    }
}

TEST_CASE("Invalid Configs")
{
    SECTION("--- NoStartParentheses.conf -- ")
    {
        Network network;
        REQUIRE_NOTHROW(network.setup(INVALID_PATH"NoStartParentheses.conf"));
    }
    SECTION("--- NoServerName.conf -- ")
    {
        Network network;
        REQUIRE_NOTHROW(network.setup(INVALID_PATH"NoServerName.conf"));
    }
}