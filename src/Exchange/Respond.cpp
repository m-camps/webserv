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
	std::string Root = _Exchanger.getServer().getRoot();
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
            generateStatus();
            generateLocation("/index.html");
            return ;
        }

        generateStatus();
		_Exchanger.setBody(FileContent);
        generateContentLength(_Exchanger.getBody().length());
	}
	catch (const std::exception& e)
	{
        std::cerr << "Fatal Error: " << e.what() << std::endl;
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

void Respond::putBodyInFile(std::string& Body)
{
    std::string word;
    std::ofstream File("test.html");
    std::istringstream iss(Body);
    std::string ContentType = _Exchanger.getHashMapString("Content-Type");
    std::string boundry = "--";
    std::size_t found = ContentType.find('=');

    if (found == std::string::npos)
        throw("No boundry");
    boundry += ContentType.substr(found + 1, ContentType.size() - found);
    File << Body;
}

/* //////////////////////////// */

#define CRLF "\r\n"

std::string Respond::getDataOfBody(void)
{
    std::string MetaData;
    std::string ContentFile;
    std::string RequestBody = _Exchanger.getHashMapString("Body");
    std::string Boundry = generateBoundry();


    std::cout << RequestBody << std::endl;
    RequestBody = RequestBody.substr(Boundry.length() + 8, RequestBody.length() - (Boundry.length() * 2) - 16);

    std::size_t found;

    while ((found = RequestBody.find(CRLF)) != std::string::npos)
    {
        std::cout << found << std::endl;
        std::cout << "FOUND: " << RequestBody.substr(0, found) << std::endl;
        MetaData += RequestBody.substr(0, found) + "\n";
        RequestBody = RequestBody.substr(found + 2, RequestBody.length());
    }
    ContentFile = RequestBody.substr(0, RequestBody.length());
     std::cout << "Metadata: \n" << MetaData << std::endl;
     std::cout << "ContentFile: \n" << ContentFile << std::endl;

    return (ContentFile);
}

/* //////////////////////////// */

void Respond::BuildPost()
{
    std::string Body = _Exchanger.getHashMapString("Body");

    std::cout << "POST" << std::endl;
    try
    {
        generateStatus();
        Body = getDataOfBody();

        putBodyInFile(Body);
        _Exchanger.setBody(Body);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }
}

/* //////////////////////////// */

void Respond::ResponseBuilder(void)
{
	struct s_Methods
	{
		std::string Method;
		void (Respond::*FuncPointer)(void);
	};

    void (Respond::*FuncPointer)(void) = nullptr;

	const s_Methods CompareMethods [3] = {
			{ "GET", &Respond::BuildGet },
			{ "POST", &Respond::BuildPost },
			{ "DELETE", &Respond::BuildDelete }
	};

	std::string Method = _Exchanger.getHashMapString("HTTPMethod");

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

    ResponseBuilder();

	Header = _Exchanger.getHeader();
    Body = _Exchanger.getBody();

	std::string response =
			Header +
			"\r\n" +
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

#pragma endregion Getter

#pragma region generators

////////////// Generators //////////////

/*
 * The status of the site should ALWAYS be the first line of the header.
 * The rest can be any line.
 */
void Respond::generateStatus(void)
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

/* //////////////////////////// */

void Respond::generateContentLength(std::size_t BodyLength)
{
    std::string ContentLength = "Content-Length: " + std::to_string(BodyLength) + "\r\n";
	_Exchanger.addLineToHeader(ContentLength);
}

/* //////////////////////////// */

void Respond::generateLocation(const std::string NewLocation)
{
	std::string Location = "Location: " + NewLocation + "\r\n";
	_Exchanger.addLineToHeader(Location);
}

/* //////////////////////////// */

void Respond::generateContentType(void)
{
    std::string line;
    std::string RequestBody = _Exchanger.getHashMapString("Body");
    std::istringstream issBody(RequestBody);

    while (std::getline(issBody, line))
    {
        if (line.compare(0, 13, "Content-Type:") == 0)
        {
            _Exchanger.addLineToHeader(line);
            return ;
        }
    }
}

/* //////////////////////////// */

std::string Respond::generateBoundry(void)
{
    std::string ContentType = _Exchanger.getHashMapString("Content-Type");

    std::size_t found = ContentType.find('=');
    if (found == std::string::npos)
    {
        std::cerr << "No boundry found" << std::endl;
        std::exit(ERROR);
    }
    return (ContentType.substr(found + 1, ContentType.length() - found));
}

#pragma endregion generators
