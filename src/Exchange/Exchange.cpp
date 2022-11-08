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
	try
	{
		Request request;
		Respond	response(server);
		HashMap	requestData;

		requestData = request.parseRequest(requestStr);
		response.buildResponse(requestData);
		sendToClient(response);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
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

void	Exchange::sendToClient(Respond& response)
{
	sendNormal(response.getHeader());
	sendNormal(CRLF);
	if (response.getBody().length() < MAXBYTES)
		sendNormal(response.getBody());
	else
		sendChunked(response.getBody());
}

void	Exchange::sendChunked(std::string str)
{
    ssize_t ret;
    std::string Body = Generator::generateChunk(str);

    while (Body.length() > 7)
    {
        ret = write(_socketFd, Body.data(), Body.length());
        if (ret < 0 || ret != (ssize_t) Body.length())
        {
            std::string StrError = std::strerror(errno);
            throw (std::runtime_error(StrError));
        }
		Body.clear();
        Body = Generator::generateChunk(str);
    }
	Body = "0\r\n\r\n";
    ret = write(_socketFd, Body.data(), Body.length());
    if (ret < 0)
    {
        std::string StrError = std::strerror(errno);
        throw (std::runtime_error(StrError));
    }
}

void		Exchange::sendNormal(std::string str)
{
	ssize_t ret;

    ret = write(_socketFd, str.data(), str.length());
    if (ret < 0 || ret != (ssize_t)str.length())
    {
        std::string ErrorMsg = std::strerror(errno);
        throw (std::runtime_error(ErrorMsg));
    }
}
#pragma endregion functions
