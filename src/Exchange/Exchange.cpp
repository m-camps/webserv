//
// Created by Xander Voorvaart on 10/10/22.
//

#include "Exchange.hpp"
#include "Request.hpp"

Exchange::~Exchange(void) {}

Exchange::Exchange(Servers& servers, int32_t socketFd, const HashMap& requestData)
    : _socketFd(socketFd)
{
    Server server;
    Location location;

    server = matchServer(servers, requestData);
    location = matchLocation(server, requestData);

    Respond	response(server, location);
    response.buildResponse(requestData);
    sendToClient(response);
	if (response.getStatusCode() ==	e_Badrequest)
		throw(Exchange::ClearException());
}

/* //////////////////////////// */

Server	Exchange::matchServer(Servers servers, HashMap requestData)
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

/* //////////////////////////// */

Location   Exchange::matchLocation(const Server& server, HashMap requestData)
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

/* //////////////////////////// */

void	Exchange::sendToClient(Respond& response)
{
    sendNormal(response.getHeader());
    sendNormal(CRLF);
    if (response.IsChunked() == false)
        sendNormal(response.getBody());
    else
        sendChunked(response.getBody());
}

/* //////////////////////////// */

inline ssize_t ft_write(int32_t _fd, const std::string& buff, size_t _nbyte)
{
    ssize_t ret;

    ret = write(_fd, buff.data(), _nbyte);
    if (ret < 0 || ret != (ssize_t)buff.length())
        throw (Exchange::WriteException());
    return (ret);
}

/* //////////////////////////// */

void	Exchange::sendChunked(std::string str) const
{
    std::string Body = Generator::generateChunk(str);

    while (Body.length() > 7)
    {
        ft_write(_socketFd, Body, Body.length());
        Body.clear();
        Body = Generator::generateChunk(str);
    }
    ft_write(_socketFd, Body, Body.length());
}

/* //////////////////////////// */

void    Exchange::sendNormal(const std::string& str) const
{
    ft_write(_socketFd, str, str.length());
}

/* //////////////////////////// */

const char* Exchange::WriteException::what() const throw()
{
    return ("An error in write. Closing client socket");
}

const char* Exchange::ClearException::what() const throw()
{
	return ("Buffer Cleared");
}
