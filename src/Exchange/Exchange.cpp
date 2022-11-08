//
// Created by Xander Voorvaart on 10/10/22.
//

#include "Exchange.hpp"
#include "Request.hpp"
#include "Respond.hpp"

#pragma region "ctor & dtor"

Exchange::Exchange(void) : _socketFd(0) {}
Exchange::~Exchange(void) {}

Exchange::Exchange(Server& server, int32_t socketFd, std::string& requestStr)
    : _server(server), _socketFd(socketFd)
{
	Request request;
	Respond	response(server);
	HashMap	requestData;

	requestData = request.parseRequest(requestStr);
	response.buildResponse(requestData);
	sendToClient(response);
}

#pragma endregion ctoranddtor

#pragma region getter

/* ////////// Getter //////////// */

Server 		Exchange::getServer(void) const { return (_server); }
HashMap		Exchange::getRequestData(void) const { return (_requestData); }
int32_t 	Exchange::getSocketFd(void) const { return (_socketFd); }

#pragma endregion getter

#pragma region functions

/* ////////// Functions //////////// */

void		Exchange::sendToClient(Respond& response)
{
	sendNormal(response.getHeader());
	if (response.IsChunked())
		sendNormal(response.getBody());
	else
		sendChunked(response.getBody());
}

void		Exchange::sendChunked(std::string str)
{
	(void)str;
	return ;
}

void		Exchange::sendNormal(std::string str)
{
	ssize_t ret;

    std::cout << str << std::endl;
    ret = write(_socketFd, str.data(), str.length());
    if (ret < 0 || ret != (ssize_t)str.length())
    {
        std::string ErrorMsg = std::strerror(errno);
        throw (std::runtime_error(ErrorMsg));
    }
}
#pragma endregion functions
