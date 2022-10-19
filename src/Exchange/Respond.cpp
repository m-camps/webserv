//
// Created by Xander Voorvaart on 10/10/2022.
//

#include "Respond.hpp"

////////////// Ctor & Dtor //////////////

Respond::Respond(Exchange& ExchangeRef)
	: _Exchanger(ExchangeRef)
{
	std::cout << "CONSTRUCTED RESPONSE\n";
	RespondToClient();
}

Respond::Respond(const Respond& ref)
		: _Exchanger(ref._Exchanger)
{
}

Respond::~Respond(void)
{
}

//////////// Operator ////////////

Respond& Respond::operator=(const Respond& ref)
{
    if (this != &ref)
    {
    }
    return (*this);
}

//////////// Responder ////////////

bool Respond::CheckConnectionStatus(void)
{
    HashMap tempMap = _Exchanger.getHashMap();

	try
	{
		HashMap::iterator ConnectionValue = tempMap.find("Connection");

		if (ConnectionValue->second == "keep-alive")
		{
			std::cout << "Client is connected" << "\n";
			return (true);
		}
		throw (std::runtime_error("Client Disconnected"));
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (false);
}

void Respond::insertBody(std::vector<std::string>& ServerRoot)
{
    std::string FileContent;
    std::string relativePath;
    HashMap tempMap = _Exchanger.getHashMap();

    if ("/favicon.ico" == tempMap.find("Path")->second)
        FileContent = readFile(getFavicon());
    else
    {
        if ("/" == tempMap.find("Path")->second)
        {
            tempMap["Path"] = "/index.html";
            _Exchanger.setHashMap(tempMap);
        }
	    relativePath = ServerRoot.back() + tempMap.find("Path")->second;
        FileContent = readFile(relativePath);
    }
    _Exchanger.setBody(FileContent);
}

uint32_t modifyStatusCode(HashMap Map, const std::string& relativePath)
{
    std::string Path = Map.find("Path")->second;

    if ("/" == Path)
        return (301);
    try
    {
        readFile(relativePath);
    }
    catch (const std::exception& e)
    {
        return (404);
    }
    return (200);
}

void Respond::ResponseBuilder(void)
{
    std::string FileContent;
    std::string relativePath;
    std::string Root = _Exchanger.getServer().getRoot();
    HashMap tempMap = _Exchanger.getHashMap();

    try
    {
        relativePath = Root + tempMap.find("Path")->second;
        uint32_t StatusCode = modifyStatusCode(tempMap, relativePath);
        _Exchanger.setStatusCode(StatusCode);

        switch (_Exchanger.getStatusCode())
        {
            case e_OK:
                std::cout << "STATUS 200" << std::endl;
                FileContent = readFile(relativePath);
                break ;
            case e_REDIR:
                std::cout << "STATUS 301" << std::endl;
                std::cout << "Still working on" << std::endl;
                break ;
            case e_ERROR:
                std::cout << "STATUS 404" << std::endl;
                FileContent = readFile(Root + "/Error404.html");
                std::cout << FileContent << std::endl;
                break ;
        }
        _Exchanger.setBody(FileContent);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        _Exchanger.setStatusCode(404);
    }
}

void Respond::RespondToClient(void)
{
    std::string Body;
    Server tempServer = _Exchanger.getServer();

	// if (!CheckConnectionStatus())
		// std::exit(EXIT_FAILURE);

    ResponseBuilder();
//	insertBody(tempServer.getRoot());
    Body = _Exchanger.getBody();

	std::string response =
			setStatus() +
            setContentLength(Body.length()) +
			"\r\n\r\n" +
            Body;

	send(_Exchanger.getSocketFD(), response.c_str(), response.length(), 0);
}

////////////// Getter //////////////

std::size_t Respond::getBodySize(std::string& Body) const
{
	return (Body.length());
}

const std::string Respond::getFavicon(void)
{
	return (readFile("data/www/favicon.html"));
}

////////////// Setter //////////////

std::string Respond::setStatus(void)
{
    HashMap tempHash = _Exchanger.getHashMap();
    HashMap::iterator it = tempHash.find("HTTPVersion");
    std::string StatusLine = it->second + " " + std::to_string(_Exchanger.getStatusCode());

    switch (_Exchanger.getStatusCode())
    {
        case e_OK:
            StatusLine + " OK\r\n";
            break ;
        case e_REDIR:
            break ;
        case e_ERROR:
            StatusLine + " Not Found\r\n";
            break ;
    }

    std::cout << StatusLine << std::endl;
    return (StatusLine);
}

std::string Respond::setContentType(void)
{
    std::string ContentType = "Content-Type: text/html\r\n";

    return (ContentType);
}

std::string Respond::setContentLength(std::size_t BodyLength)
{
    std::string ContentLength = "Content-Length: " + std::to_string(BodyLength) + "\r\n";

    return (ContentLength);
}
