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

    generateStatus();
    generateLocation(NewLocation);
    _Exchanger.setBody("");
    generateContentLength(0);
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
        if (StatusCode == e_Redir)
        {
            BuildGet_Redir();
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
        generateContentLength(0);
        deleteFile(relativePath);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << '\n';
        std::exit(EXIT_FAILURE);
    }
}

/* //////////////////////////// */

/*
 * Gonna make this a bit more elegant.
 */
std::string getFilename(std::string& MetaData)
{
    std::string line;
    std::size_t found = MetaData.find("filename=");
    if (found == std::string::npos)
    {
        std::cerr << "File has no name" << std::endl;
        return ("UserUpload.txt");
    }

    std::istringstream issFile(MetaData.substr(found + 10, MetaData.length()));
    std::getline(issFile, line);

    line = line.substr(0, line.length() - 1);
    return (line);
}

void Respond::putBodyInFile(std::string& MetaData, std::string& Body)
{
    std::string ContentType;
    std::string Root = _Exchanger.getServer().getRoot();
    std::ofstream File(Root + "/" + getFilename(MetaData));

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
        RequestBody = RequestBody.substr(Boundry.length() + 5,
                                         RequestBody.length() - (Boundry.length() * 2) - 11);

        for (int32_t i = 0; i < 3; i++)
        {
            found = RequestBody.find(CRLF);
            _MetaData += RequestBody.substr(0, found) + "\n";
            RequestBody = RequestBody.substr(found + 2, RequestBody.length());
        }
        ContentFile = RequestBody.substr(0, RequestBody.length());
    }
    catch (const std::exception& e)
    {
        throw(std::logic_error("POST could not be handled"));
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
        generateContentLength(_Exchanger.getBody().length());
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
    void (Respond::*FuncPointer)(void) = nullptr;
	std::string Method = _Exchanger.getHashMapString("HTTPMethod");
    std::vector<std::string> AllowedMethods = _Exchanger.getServer().getMethods();

	const s_Methods CompareMethods [3] = {
			{ "GET", &Respond::BuildGet },
			{ "POST", &Respond::BuildPost },
			{ "DELETE", &Respond::BuildDelete }
	};

    if (!MethodIsAllowed(Method, AllowedMethods))
    {
        _Exchanger.setStatusCode(e_MethodNotFound);
        generateStatus();
        std::string FileContent = defaultStatusPage(_Exchanger.getStatusCode());
        _Exchanger.setBody(FileContent);
        generateContentLength(_Exchanger.getBody().length());
        return ;
    }

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
    std::string Body = generateChunk();

    while (Body.length() > 7)
    {

        ret = send(_Exchanger.getSocketFD(), Body.c_str(), Body.length(), 0);
        if (ret < 0)
        {
            Body = "0\r\n\r\n";
            ret = send(_Exchanger.getSocketFD(), Body.c_str(), Body.length(), 0);
            if (ret < 0)
            {
                std::string StrError = std::strerror(errno);
                throw (std::runtime_error(StrError));
            }
            std::string StrError = std::strerror(errno);
            throw (std::runtime_error(StrError));
        }
        Body = generateChunk();
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
        generateTransferEncoding();
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
    }

    ssize_t ret = send(_Exchanger.getSocketFD(), response.c_str(), response.length(), 0);
    if (ret < 0)
    {
        std::string StrError = std::strerror(errno);
        throw (std::runtime_error(StrError));
    }
    if (IsChunked == true)
        sendAsChunked();
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
        return (e_Forbidden);
	if ("/" == Path)
		return (e_Redir);
    if (access(relativePath.c_str(), R_OK) != 0)
		return (e_NotFound);
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
			case e_Redir:
                break ;
			case e_NotFound:
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

void Respond::generateTransferEncoding(void)
{
    std::string TransferEncoding = "Transfer-Encoding: chunked\r\n";
    _Exchanger.addLineToHeader(TransferEncoding);
}

/* //////////////////////////// */

void Respond::generateContentLength(std::size_t BodyLength)
{
    std::string ContentLength = "Content-Length: " + TOSTRING(BodyLength) + "\r\n";
	_Exchanger.addLineToHeader(ContentLength);
}

/* //////////////////////////// */

std::string ConvertDecimalToHex(size_t decimal)
{
    std::stringstream ss;

    ss << std::hex << decimal;
    return (ss.str());
}

std::string Respond::generateChunk(void)
{
    std::string Chunk;
    std::string Body = _Exchanger.getBody();

    if (Body.length() > MAXBYTES)
    {
        Chunk = ConvertDecimalToHex(MAXBYTES) + "\r\n";
        Chunk += Body.substr(0, MAXBYTES) + "\r\n\r\n";
        Body = Body.substr(MAXBYTES, Body.length() - MAXBYTES);
        _Exchanger.setBody(Body);
    }
    else
    {
        Chunk = ConvertDecimalToHex(Body.length()) + "\r\n";
        Chunk += Body.substr(0, Body.length()) + "\r\n\r\n";
        _Exchanger.setBody("");
    }

//    std::cout << "-------------CHUNK: ------------\n" << Chunk << std::endl;
    return (Chunk);
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
