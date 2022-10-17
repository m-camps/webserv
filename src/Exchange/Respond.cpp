//
// Created by Xander Voorvaart on 10/10/2022.
//

#include "Respond.hpp"

#define ERROR 1

#pragma region "ctor & dtor"

////////// Ctor & Dtor ///////////

Respond::Respond(Exchange& ExchangeRef)
	: _Exchanger(ExchangeRef)
{
	RespondToClient();
}

/* //////////////////////////// */

Respond::Respond(const Respond& ref)
		: _Exchanger(ref._Exchanger)
{
	RespondToClient();
}

/* //////////////////////////// */

Respond::~Respond(void)
{
}

#pragma endregion "ctor & dtor"

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
		std::cerr << "Client disconnected" << std::endl;
	}
	return (false);
}

/* //////////////////////////// */

void Respond::BuildGet(void)
{
	std::string FileContent;
	std::string relativePath;
	std::string Root = _Exchanger.getServer().getRoot().back();
	HashMap tempMap = _Exchanger.getHashMap();

    std::cout << "GET" << std::endl;
	try
	{
		relativePath = Root + tempMap.find("Path")->second;
		uint32_t StatusCode = modifyStatusCode(tempMap, relativePath);
		_Exchanger.setStatusCode(StatusCode);

		FileContent = getValidFile(Root, relativePath, _Exchanger.getStatusCode());
        if (StatusCode == 301)
        {
            setStatus();
            setLocation("/index.html");
            return ;
        }

		setStatus();
		_Exchanger.setBody(FileContent);
		setContentLength(_Exchanger.getBody().length());
	}
	catch (const std::exception& e)
	{
        std::cerr << "Fatal Error" << std::endl;
        std::exit(ERROR);
	}
}

/* //////////////////////////// */

void Respond::BuildDelete()
{
    std::cout << "DELETE" << std::endl;
    return ;
}

/* //////////////////////////// */

void Respond::BuildPost()
{
    std::cout << "POST" << std::endl;
    return ;
}

/* //////////////////////////// */

void Respond::ResponseBuilder(void)
{
	struct s_Methods
	{
		std::string Method;
		void (Respond::*FuncPointer)(void);
	}   t_Methods;

    void (Respond::*FuncPointer)(void) = nullptr;

	const s_Methods CompareMethods [3] = {
			{ "GET", &Respond::BuildGet },
			{ "POST", &Respond::BuildPost },
			{ "DELETE", &Respond::BuildDelete }
	};

	HashMap HashMap = _Exchanger.getHashMap();
	std::string Method = HashMap.find("HTTPMethod")->second;

	for (int32_t i = 0; i < 3; i++)
	{
		if (Method == CompareMethods[i].Method)
		{
			FuncPointer = CompareMethods[i].FuncPointer;
            (this->*FuncPointer)();
            break ;
		}
	}
}

/* //////////////////////////// */

void Respond::RespondToClient(void)
{
    std::string Body;
	std::string Header;
    Server tempServer = _Exchanger.getServer();

	if (!CheckConnectionStatus())
		std::exit(EXIT_FAILURE);

    ResponseBuilder();

	Header = _Exchanger.getHeader();
    Body = _Exchanger.getBody();

	std::string response =
			Header +
			"\r\n\r\n" +
            Body;

	send(_Exchanger.getSocketFD(), response.c_str(), response.length(), 0);
}

#pragma region "NonClass functions"

/* ///////// External Functions ////////// */

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

/* //////////////////////////// */

std::string getValidFile(std::string Root, std::string relativePath, uint32_t StatusCode)
{
	std::string FileContent;

	try
	{
		switch (StatusCode)
		{
			case e_OK:
				FileContent = readFile(relativePath);
				break ;
			case e_REDIR:
                break ;
			case e_NOTFOUND:
				FileContent = readFile(Root + "/Error404.html");
				break ;
		}
	}
	catch (const std::exception& e)
	{
		FileContent = defaultStatusPage(StatusCode);
	}
	return (FileContent);
}

#pragma endregion "NonClass functions"

#pragma region Getter

////////////// Getter //////////////

std::size_t Respond::getBodySize(std::string& Body) const
{
	return (Body.length());
}

const std::string Respond::getFavicon(void)
{
	return (readFile("data/www/favicon.html"));
}

#pragma endregion Getter

#pragma region Setter

////////////// Setter //////////////

/*
 * The status of the site should ALWAYS be the first line of the header.
 * The rest can be any line.
 */
void Respond::setStatus(void)
{
    HashMap tempHash = _Exchanger.getHashMap();
    std::string StatusLine = tempHash.find("HTTPVersion")->second;

    switch (_Exchanger.getStatusCode())
    {
        case e_OK:
		    StatusLine += " 200\r\n";
			break ;
        case e_REDIR:
			StatusLine += " 301\r\n";
            break ;
        case e_NOTFOUND:
		    StatusLine += " 404\r\n";
			break ;
    }
	_Exchanger.setHeader(StatusLine);
}

void Respond::setContentLength(std::size_t BodyLength)
{
    std::string ContentLength = "Content-Length: " + std::to_string(BodyLength) + "\r\n";
	_Exchanger.addLineToHeader(ContentLength);
}

void Respond::setLocation(const std::string NewLocation)
{
	std::string Location = "Location: " + NewLocation + "\r\n";
	_Exchanger.addLineToHeader(Location);
}

#pragma endregion Setter
