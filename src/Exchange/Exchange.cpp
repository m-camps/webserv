//
// Created by Xander Voorvaart on 10/10/22.
//

#include "Exchange.hpp"
#include "Request.hpp"
#include "Respond.hpp"

#pragma region "ctor & dtor"

Exchange::Exchange(void) : _socketFd(0) {}
Exchange::~Exchange(void) {}

Exchange::Exchange(Servers servers, int32_t socketFd, const HashMap& requestData)
    : _socketFd(socketFd)
{
	try
	{
        Server server;
        Location location;

		server = matchServer(servers, requestData);
		location = matchLocation(server, requestData);

		Respond	response(server, location);
		response.buildResponse(requestData);
		sendToClient(response);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

Server		Exchange::matchServer(Servers servers, HashMap requestData)
{
	Server		server = servers.front();
	std::string host = requestData.find("Host")->second;

	for (Servers::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::vector<std::string> names = (*it).getNames();
		if (std::find(names.begin(), names.end(), host) != names.end())
			return (*it);
	}
	return (server);
}

Location	Exchange::matchLocation(Server server, HashMap requestData)
{
    int32_t longestMatch = 0;
    std::string loc = "/";
    std::string route = requestData.find("Path")->second;
    Locations locations = server.getLocations();

    std::vector<std::string> splitRoute = splitLineWithStrtok(route, "/");

    for (Locations::iterator it = locations.begin(); it != locations.end(); it++)
    {
        std::string name = it->first;
        std::vector<std::string> splitName = splitLineWithStrtok(name, "/");
        int match = 0;

        for (size_t i = 0; i < splitRoute.size() && i < splitName.size(); i++)
        {
            if (splitRoute[i] == splitName[i])
                match++;
        }
        if (match > longestMatch)
        {
            longestMatch = match;
            loc = name;
        }
    }
    return (server.getLocations().find(loc)->second);
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
    try
    {
        sendNormal(response.getHeader());
        sendNormal(CRLF);
        if (response.IsChunked() == false)
            sendNormal(response.getBody());
        else
            sendChunked(response.getBody());
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void	Exchange::sendChunked(std::string str) const
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
    ret = write(_socketFd, Body.data(), Body.length());
    if (ret < 0 || ret != (ssize_t)Body.length())
    {
        std::string StrError = std::strerror(errno);
        throw (std::runtime_error(StrError));
    }
}

void    Exchange::sendNormal(const std::string& str) const
{
	ssize_t ret;

    ret = write(_socketFd, str.data(), str.length());
    if (ret < 0 || ret != (ssize_t)str.length())
    {
        std::string ErrorMsg = std::strerror(errno);
        throw (std::runtime_error(ErrorMsg));
    }
}

#pragma endregion adders
