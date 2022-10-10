//
// Created by Xander Voorvaart on 10/10/2022.
//

#include "Respond.hpp"

////////////// Ctor & Dtor //////////////

Respond::Respond(Exchange& ExchangeRef)
	: _Exchanger(ExchangeRef)
{
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
		std::cerr << "Client disconnected" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (false);
}

/*
 * /data/www/ does not work...
 * data/www/ does... lol
*/
std::string Respond::readFile(const std::string& RespondedFile)
{
	std::ifstream File;
	std::string line;
	std::string FileContent;

    std::cout << RespondedFile << std::endl;
	File.open(RespondedFile);
	if (!File.is_open())
	{
		_Exchanger.setStatusCode(404);
		std::exit(EXIT_FAILURE);
	}

	while (std::getline(File, line))
		FileContent += line;

	return (FileContent);
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

void Respond::RespondToClient(void)
{
    std::string Body;
    Server tempServer = _Exchanger.getServer();

	if (!CheckConnectionStatus())
		std::exit(EXIT_FAILURE);

	insertBody(tempServer.getRoot());
    Body = _Exchanger.getBody();

	std::string response =
			setStatus() +
            setContentLength(Body.length()) +
            setContentType() +
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

    std::string StatusLine = it->second + std::to_string(_Exchanger.getStatusCode()) + "OK\r\n";

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
