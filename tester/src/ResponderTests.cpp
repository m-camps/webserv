///* ************************************************************ */
///* Created by Xander Voorvaart on 11/17/22.                     */
///* ************************************************************ */
//
//#include <catch2/catch_test_macros.hpp>
//
//#include "Respond.hpp"
//#include "Request.hpp"
//#include "Parse.hpp"
//#include "Network.hpp"
//
//#define VALID_CONF "tester/confs/Valid/basic_no_location.conf"
//
//void	ASSERT_VALID_RESPONDER(const std::string& requestString)
//{
//    std::string ConfFile = VALID_CONF;
//
//    SECTION("--- " + requestString + " ---")
//    {
//        Server server;
//        Location location;
//        Request request;
//        std::vector<Server> tmp;
//        Parse parse;
//        Network network;
//
//        network.setup(ConfFile);
//        tmp = parse.parseNetwork(ConfFile, tmp);
//        location = tmp.begin()->getLocations().begin()->second;
//        Respond Responder(*tmp.begin(), location);
//        HashMap requestData = request.parseRequest(requestString);
////        REQUIRE_NOTHROW(Responder.buildResponse(requestData));
//    }
//}
//
//TEST_CASE("Valid Requests")
//{
//    ASSERT_VALID_RESPONDER("GET /index.html HTTP1.1\r\n");
//}
