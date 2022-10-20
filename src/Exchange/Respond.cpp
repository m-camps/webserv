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

void Respond::BuildGet(void)
{
	std::string FileContent;
	std::string relativePath;
	std::string Root = _Exchanger.getServer().getRoot();
	std::string Path = _Exchanger.getHashMapString("Path");

    std::cout << "GET" << std::endl;
	try
	{
		relativePath = Root + Path;
		uint32_t StatusCode = modifyStatusCode(Path, relativePath);
		_Exchanger.setStatusCode(StatusCode);

		FileContent = getValidFile(Root, relativePath, _Exchanger.getStatusCode());
        if (StatusCode == 301)
        {
            const std::string NewLocation = _Exchanger.getServer().getIndex();

            generateStatus();
            generateLocation(NewLocation);
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
    std::string relativePath;
    std::string Root = _Exchanger.getServer().getRoot();
    std::string Path = _Exchanger.getHashMapString("Path");

    std::cout << "DELETE" << std::endl;
    try
    {
        relativePath = Root + Path;
        uint32_t StatusCode = modifyStatusCode(Path, relativePath);
        _Exchanger.setStatusCode(StatusCode);

        generateStatus();
        _Exchanger.setBody("");
        deleteFile(relativePath);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << '\n';
        std::exit(EXIT_FAILURE);
    }
}

/* //////////////////////////// */

void Respond::putBodyInFile(std::string& MetaData, std::string& Body)
{
    std::string ContentType;
    std::size_t found = MetaData.find("Content-Type:");
    std::ofstream File("data/www/test.html");

    (void) found;
    File << Body;
}

/* //////////////////////////// */

std::string Respond::getDataOfBody(void)
{
    std::size_t found;
    std::string ContentFile;
    std::string RequestBody = _Exchanger.getHashMapString("Body");
    try
    {
        std::string Boundry = generateBoundry();
        RequestBody = RequestBody.substr(Boundry.length() + 8,
                                         RequestBody.length() - (Boundry.length() * 2) - 16);

        while ((found = RequestBody.find(CRLF)) != std::string::npos)
        {
            _MetaData += RequestBody.substr(0, found) + "\n";
            RequestBody = RequestBody.substr(found + 2, RequestBody.length());
        }
        ContentFile = RequestBody.substr(0, RequestBody.length());
    }
    catch (const std::exception& e)
    {
        while ((found = RequestBody.find(CRLF)) != std::string::npos)
        {
            _MetaData += RequestBody.substr(0, found) + "\n";
            RequestBody = RequestBody.substr(found + 2, RequestBody.length());
        }
        ContentFile = RequestBody.substr(0, RequestBody.length());
    }

    return (ContentFile);
}

/* //////////////////////////// */

void Respond::BuildPost()
{
    std::string MetaData;
    std::string Body = _Exchanger.getHashMapString("Body");

    std::cout << "POST" << std::endl;
    try
    {
        generateStatus();
        Body = getDataOfBody();

        putBodyInFile(_MetaData, Body);
        _Exchanger.setBody(Body);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

/* //////////////////////////// */

struct s_Methods
{
    std::string Method;
    void (Respond::*FuncPointer)(void);
};

bool MethodIsAllowed(const std::string& Method, std::vector<std::string>& ConfMethods)
{
    std::vector<std::string>::iterator it = ConfMethods.begin();

    std::cout << "Method: " << Method << std::endl;
    for (; it != ConfMethods.end(); it++)
    {
        if (Method == *it)
            return (true);
    }
    return (false);
}

/* //////////////////////////// */

void Respond::ResponseBuilder(void)
{
    void (Respond::*FuncPointer)(void) = nullptr;
	std::string Method = _Exchanger.getHashMapString("HTTPMethod");
    std::vector<std::string> ConfMethods = _Exchanger.getServer().getMethods();

	const s_Methods CompareMethods [3] = {
			{ "GET", &Respond::BuildGet },
			{ "POST", &Respond::BuildPost },
			{ "DELETE", &Respond::BuildDelete }
	};

//    if (!MethodIsAllowed(Method, ConfMethods))
//        throw (std::logic_error("Method not allowed"));

	for (int32_t i = 0; i < 3; i++)
	{
		if (Method == CompareMethods[i].Method)
		{
			FuncPointer = CompareMethods[i].FuncPointer;
            (this->*FuncPointer)();
            return ;
		}
	}
}

/* //////////////////////////// */

void Respond::RespondToClient(void)
{
	std::string Header;
    std::string Body;

    try
    {
        ResponseBuilder();

        Header = _Exchanger.getHeader();
        Body = _Exchanger.getBody();

        std::string response =
                Header +
                "\r\n" +
                Body;

        std::cout << "Response: \n" << response << std::endl;
        send(_Exchanger.getSocketFD(), response.c_str(), response.length(), 0);
    }
    catch (const std::exception& e)
    {
        // This creates a segfault. @mcamps, plz help
        std::cerr << e.what() << '\n';
        send(_Exchanger.getSocketFD(), "", 0, MSG_DONTWAIT);
    }
}

#pragma region "NonClass functions"

/* ///////// External Functions ////////// */

bool isForbidden(const std::string& Path)
{
    std::size_t found = Path.find("../");
    if (found == std::string::npos)
        return (false);
    return (true);
}

/* //////////////////////////// */

uint32_t modifyStatusCode(std::string Path, const std::string& relativePath)
{
    if (isForbidden(Path))
        return (e_FORBIDDEN);
	if ("/" == Path)
		return (e_REDIR);
    if (access(relativePath.c_str(), F_OK) != 0)
		return (e_NOTFOUND);
	return (e_OK);
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
            default:
                FileContent = defaultStatusPage(StatusCode);
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
    uint32_t StatusCode = _Exchanger.getStatusCode();
    std::string StatusLine = tempHash.find("HTTPVersion")->second;

    StatusLine += " " + TOSTRING( StatusCode ) + " \r\n";
	_Exchanger.setHeader(StatusLine);
}

/* //////////////////////////// */

void Respond::generateContentLength(std::size_t BodyLength)
{
    std::string ContentLength = "Content-Length: " + TOSTRING(BodyLength) + "\r\n";
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
        throw (std::logic_error("No boundry found"));

    return (ContentType.substr(found + 1, ContentType.length() - found) + "\r\n");
}

#pragma endregion generators
