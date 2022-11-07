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

void Respond::BuildGet_Redir(void)
{
    const std::string NewLocation = _Exchanger.getServer().getIndex();

    Generator::generateStatus(_Exchanger);
    Generator::generateLocation(_Exchanger, NewLocation);
    _Exchanger.setBody("");
}

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

        Generator::generateStatus(_Exchanger);
		_Exchanger.setBody(FileContent);
        Generator::generateContentLength(_Exchanger, _Exchanger.getBody().length());
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

        Generator::generateStatus(_Exchanger);
        _Exchanger.setBody("");
        Generator::generateContentLength(_Exchanger, 0);
        deleteFile(relativePath);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << '\n';
        std::exit(EXIT_FAILURE);
    }
}

/* //////////////////////////// */

std::string getFilename(std::string& MetaData)
{
    std::string line;
    std::size_t found = MetaData.find("filename=");
    if (found == std::string::npos)
        throw (std::invalid_argument("File has no name"));

    std::istringstream issFile(MetaData.substr(found + 10, MetaData.length()));
    std::getline(issFile, line);

    line.erase(line.length() - 1, 1);
    return (line);
}

void Respond::putBodyInFile(std::string& MetaData, std::string& Body)
{
    try
    {
        std::string Root = _Exchanger.getServer().getRoot();
        std::ofstream File(Root + "/" + getFilename(MetaData));

        File << Body;
    }
    catch (const std::exception& e)
    {
        throw (e);
    }
}

/* //////////////////////////// */

std::string Respond::getDataOfBody(void)
{
    std::size_t found;
    std::string ContentFile;
    std::string RequestBody = _Exchanger.getHashMapString("Body");

    try
    {
        std::string Boundry = Generator::generateBoundry(_Exchanger);
        RequestBody.erase(0, Boundry.length() + 5);
        RequestBody.erase(RequestBody.length() - Boundry.length() - 6, Boundry.length() + 6);

        for (int32_t i = 0; i < 3; i++)
        {
            found = RequestBody.find(CRLF);
            _MetaData += RequestBody.substr(0, found) + "\n";
            RequestBody.erase(0, found + 2);
        }
        ContentFile = RequestBody.substr(0, RequestBody.length());
    }
    catch (const std::exception& e)
    {
        throw (std::logic_error("POST could not be handled"));
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
        Generator::generateStatus(_Exchanger);
        Body = getDataOfBody();

        putBodyInFile(_MetaData, Body);
        _Exchanger.setBody("");
        Generator::generateContentLength(_Exchanger, 0);
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

bool MethodIsAllowed(const std::string& Method, std::vector<std::string>& AllowedMethods)
{
    std::vector<std::string>::iterator it = AllowedMethods.begin();

    for (; it != AllowedMethods.end(); it++)
    {
        if (Method == *it)
            return (true);
    }
    return (false);
}

/* //////////////////////////// */

void Respond::ResponseBuilder(void)
{
    void (Respond::*FuncPointer)(void) = NULL;
	std::string Method = _Exchanger.getHashMapString("HTTPMethod");

	const s_Methods CompareMethods [3] = {
			{ "GET", &Respond::BuildGet },
			{ "POST", &Respond::BuildPost },
			{ "DELETE", &Respond::BuildDelete }
	};

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

void Respond::sendAsChunked(void)
{
    ssize_t ret;
    std::string Body = Generator::generateChunk(_Exchanger);

    while (Body.length() > 7)
    {

        ret = send(_Exchanger.getSocketFD(), Body.data(), Body.length(), 0);
        if (ret < 0)
        {
            std::string StrError = std::strerror(errno);
            throw (std::runtime_error(StrError));
        }
        Body = Generator::generateChunk(_Exchanger);
    }
    Body = "0\r\n\r\n";
    ret = send(_Exchanger.getSocketFD(), Body.c_str(), Body.length(), 0);
    if (ret < 0)
    {
        std::string StrError = std::strerror(errno);
        throw (std::runtime_error(StrError));
    }
}

void Respond::RespondToClient(void)
{
    bool IsChunked = false;
    std::string response;
	std::string Header;
    std::string Body;

    ResponseBuilder();

    Body = _Exchanger.getBody();

    if (Body.length() > MAXBYTES)
    {
        Generator::generateTransferEncoding(_Exchanger);
        IsChunked = true;
        Header = _Exchanger.getHeader();
        response = Header + "\r\n";
    }
    else
    {
        Header = _Exchanger.getHeader();

        response =
            Header +
            "\r\n" +
            Body;
        std::cout << response << std::endl;
    }

    ssize_t ret = write(_Exchanger.getSocketFD(), response.data(), response.length());
    if (ret < 0)
    {
        std::string StrError = std::strerror(errno);
        throw (std::runtime_error(StrError));
    }
    if (IsChunked == true)
        try
        {
            sendAsChunked();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
}

#pragma region "NonClass functions"

/* ///////// External Functions ////////// */

bool isForbiddenPath(const std::string& Path)
{
    std::size_t found = Path.find("../");
    if (found == std::string::npos)
        return (false);
    return (true);
}

/* //////////////////////////// */

uint32_t modifyStatusCode(std::string Path, const std::string& relativePath)
{
    if (isForbiddenPath(Path))
        return (e_Forbidden);
	if ("/" == Path)
		return (e_Redir);
    if (access(relativePath.c_str(), R_OK) != 0)
		return (e_NotFound);
	return (e_OK);
}

/* //////////////////////////// */

std::string Respond::getValidFile(std::string Root, std::string relativePath, uint32_t StatusCode)
{
	std::string FileContent;
    ErrorPageMap ErrorPages = _Exchanger.getServer().getErrorPage();
    ErrorPageMap::iterator it = ErrorPages.find(StatusCode);

	try
	{
        if (it != ErrorPages.end())
        {
            FileContent = readFile(Root + it->second);
            return (FileContent);
        }
        switch (StatusCode)
		{
			case e_OK:
				FileContent = readFile(relativePath);
				break ;
			case e_Redir:
                BuildGet_Redir();
                break ;
            default:
                FileContent = Generator::generateDefaulPage(StatusCode);
                break ;
		}
	}
	catch (const std::exception& e)
	{
		FileContent = Generator::generateDefaulPage(StatusCode);
	}
	return (FileContent);
}

#pragma endregion "NonClass functions"
