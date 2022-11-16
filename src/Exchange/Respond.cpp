//
// Created by Xander Voorvaart on 10/10/2022.
//

#include "Respond.hpp"

Respond::Respond(const Server& server, const Location& location)
    : _server(server), _location(location) ,_status_code(200), _isChunked(false) {}

Respond::~Respond(void) {}

/* //////////////////////////// */

std::string		Respond::getHeader(void) const { return _header; }
std::string		Respond::getBody(void) const { return _body; }
HashMap			Respond::getRequestData(void) const { return _requestData; }
int				Respond::getStatusCode(void) const { return _status_code; }
Server			Respond::getServer(void) const { return _server; }
Location        Respond::getLocation() const { return _location; }
bool			Respond::IsChunked(void) const { return _isChunked; }

void			Respond::setBody(std::string body) { _body = body; }

void 		    Respond::addToHeader(const std::string& NewLine)
{
	std::string NewHeader = _header + NewLine;

	_header = NewHeader;
}

/* //////////////////////////// */

bool isForbiddenPath(const std::string& Path)
{
    std::size_t found = Path.find("../");
    if (found == std::string::npos)
        return (false);
    return (true);
}

/* //////////////////////////// */

void Respond::modifyStatuscode(const std::string& Path, const std::string& relativePath)
{
    if (isForbiddenPath(Path) == true)
    {
        _status_code = e_Forbidden;
        return ;
    }
//    if ("/" == Path || Path.empty() == true)
//    {
//        _status_code = e_Redir;
//        return ;
//    }
    if (access(relativePath.c_str(), R_OK) != 0)
    {
        _status_code = e_NotFound;
        return ;
    }
    _status_code = e_OK;
}

/* //////////////////////////// */

void    Respond::createResponse(const std::string& FileContent)
{
    addToHeader(Generator::generateStatus(*this));
    setBody(FileContent);
    if (FileContent.length() > MAXBYTES)
    {
        _isChunked = true;
        addToHeader(Generator::generateTransferEncoding());
    }
    addToHeader(Generator::generateContentLength(FileContent.length()));
}

/* //////////////////////////// */

bool MethodIsAllowed(const std::string& Method, std::vector<std::string> AllowedMethods)
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

bool MethodIsImplemented(const std::string& Method)
{
    std::string ImplementedMethods[] = {
            "GET",
            "POST",
            "DELETE"
    };

    for (int32_t i = 0; i < 3 ; i++)
    {
        if (Method == ImplementedMethods[i])
            return (true);
    }
    return (false);
}

/* //////////////////////////// */

void 	Respond::buildResponse(HashMap requestData)
{
	_requestData = requestData;

    try
    {
        void (Respond::*FuncPointer)(void) = NULL;
        std::string Method = getEntryFromMap("HTTPMethod");

        const s_Methods CompareMethods[3] = {
                {"GET",    &Respond::buildGet},
                {"POST",   &Respond::buildPost},
                {"DELETE", &Respond::buildDelete}
        };

        if (MethodIsAllowed(Method, _location.getAllowMethods()) == true)
        {
            for (int32_t i = 0; i < 3; i++) {
                if (Method == CompareMethods[i].Method) {
                    FuncPointer = CompareMethods[i].FuncPointer;
                    (this->*FuncPointer)();
                    return;
                }
            }
        }

        if (MethodIsImplemented(Method) == true)
            _status_code = e_MethodNotFound;
        else if (Method == "ERROR")
            _status_code = e_Badrequest;
        else
            _status_code = e_NotImplemented;
        createResponse(Generator::generateDefaulPage(_status_code));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        _status_code = e_InternalServerError;
        createResponse(Generator::generateDefaulPage(_status_code));
    }
}

/* //////////////////////////// */

std::string Respond::getEntryFromMap(const std::string& entry)
{
	HashMap::iterator it = _requestData.find(entry);

        if (it == _requestData.end())
            throw (std::invalid_argument("Invalid string"));
        return (it->second);
}

/* //////////////////////////// */

std::string Respond::getValidFile(const std::string& relativePath)
{
	std::string FileContent;
    std::string LocationIndex = _location.getRoot() + "/" + _location.getIndex();
    ErrorPageMap ErrorPages = _server.getErrorPage();
    ErrorPageMap::iterator it = ErrorPages.find(_status_code);

	try
	{
        // Needs to be fixed
        if (it != ErrorPages.end())
        {
            if (access(it->second.c_str(), R_OK) == 0)
            {
                FileContent = readFile(_location.getRoot() + it->second);
                return (FileContent);
            }
        }
        switch (_status_code)
		{
			case e_OK:
                if (isDirectory(relativePath) == true)
                    FileContent = Generator::generateDirectoryPage(relativePath);
                else
				    FileContent = readFile(relativePath);
				break ;
			case e_Redir:
                buildRedir();
                break ;
            case e_NotFound:
                if (_location.getAutoIndex() == true && LocationIndex == relativePath)
                    FileContent = Generator::generateAutoIndex(*this);
                else
                    FileContent = Generator::generateDefaulPage(_status_code);
                break ;
            default:
                FileContent = Generator::generateDefaulPage(_status_code);
                break ;
		}
	}
	catch (const std::exception& e)
	{
        std::cerr << e.what() << std::endl;
        _status_code = e_InternalServerError;
		FileContent = Generator::generateDefaulPage(_status_code);
	}
	return (FileContent);
}
