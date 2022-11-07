//
// Created by Xander Voorvaart on 10/10/22.
//

#include "Exchange.hpp"
#include "Request.hpp"
#include "Respond.hpp"

#pragma region "ctor & dtor"

Exchange::Exchange(void) : _socketFD(0) {}
Exchange::~Exchange(void) {}

Exchange::Exchange(Server& server, int32_t& socketFd, std::string& requestStr)
    : _server(server), _socketFD(socketFd)
{
	Request request;
	Respond	response(server);
	HashMap	requestData;

	requestData = request.parseRequest(requestStr);
	std::cout << requestStr << std::endl;
	response.buildResponse(requestData);
	std::cout << response.getResponse() << std::endl;
	if (response.isChunked)
		sendToClientChunked(response.getResponse());
	sendToClient(response.getResponse());
}

#pragma endregion ctoranddtor

#pragma region getter

/* ////////// Getter //////////// */

Server 		Exchange::getServer(void) const { return (_server); }
HashMap		Exchange::getRequestData(void) const { return (_requestData); }
int32_t 	Exchange::getSocketFD(void) const { return (_socketFD); }

#pragma endregion getter

#pragma region functions

/* ////////// Functions //////////// */

void		Exchange::sendToClient(std::string response)
{
	 ssize_t ret;

    ret = write(getSocketFD(), response.data(), response.length());
    if (ret < 0 || ret != (ssize_t)response.length())
    {
        std::string ErrorMsg = std::strerror(errno);
        throw (std::runtime_error(ErrorMsg));
    }
}

void		Exchange::sendToClientChunked(std::string response)
{
	(void)response;
	return ;
}
#pragma endregion functions
