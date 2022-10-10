//
// Created by Xander Voorvaart on 10/10/2022.
//

#include "Respond.hpp"

////////////// Ctor & Dtor //////////////

Respond::Respond(Server NewServer, HashMap NewDictHeader, const int32_t ListenSocket)
	: _server(NewServer), _dictHeader(NewDictHeader), _SocketFD(ListenSocket)
{
	RespondToClient();
}

Respond::Respond(const Respond& ref)
		: _server(ref._server), _dictHeader(ref._dictHeader), _SocketFD(ref._SocketFD)
{
}

Respond::~Respond(void)
{
}

//////////// Responder ////////////

bool Respond::CheckConnectionStatus(void)
{
	try
	{
		HashMap::iterator ConnectionValue = _dictHeader.find("Connection");

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

	File.open(RespondedFile);
	if (!File.is_open())
	{
		std::cerr << "404 Error" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	while (std::getline(File, line))
		FileContent += line;

	return (FileContent);
}

std::string Respond::insertBody(std::vector<std::string>& ServerRoot)
{
	std::string relativePath = ServerRoot.back() + _dictHeader.find("Path")->second;
	const std::string FileContent = readFile(relativePath);

	if ("favicon.ico" == _dictHeader.find("Path")->second)
		return (getFavicon());

	return (FileContent);
}

void Respond::RespondToClient(void)
{
	std::string response =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: 160\r\n"
			"Keep - Alive: timeout=1, max=1\r\n"
			"Accept-Ranges: bytes\r\n"
			"Connection: close\r\n"
			"\r\n\r\n";

	if (!CheckConnectionStatus())
		std::exit(EXIT_FAILURE);
	response += insertBody(_server.getRoot());
	std::cout << response;
	send(_SocketFD, response.c_str(), response.length(), 0);
}

////////////// Geter //////////////

std::size_t Respond::getBodySize(std::string& Body) const
{
	return (Body.length());
}

const std::string Respond::getFavicon(void)
{
	return (readFile("data/www/favicon.html"));
}
