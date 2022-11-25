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

/* //////////////////////////// */

void			Respond::setBody(std::string body) { _body = body; }
void			Respond::setStatusCode(int statusCode) { _status_code = statusCode; }

/* //////////////////////////// */

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
    }
    else if (_location.getReturnPath().empty() == false || relativePath == _location.getRoot())
    {
        _status_code = e_Redir;
    }
    else if (access(relativePath.c_str(), F_OK) != 0)
    {
        _status_code = e_NotFound;
    }
    else if (access(relativePath.c_str(), R_OK) != 0)
    {
        _status_code = e_Unauthorized;
    }
    else
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
    const std::array<std::string, 3> ImplementedMethods = {
            "GET",
            "POST",
            "DELETE"
    };

	for (uint64_t i = 0; i < ImplementedMethods.size() ; i++)
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
		std::cout << YELLOW_COLOR << Method << " " << getEntryFromMap("Path") << RESET_COLOR << std::endl;

        const std::array<s_Methods, 3> CompareMethods = {{
                {"GET",    &Respond::buildGet},
                {"POST",   &Respond::buildPost},
                {"DELETE", &Respond::buildDelete}
        }};


        if (getEntryFromMap("HTTPVersion") == "HTTP/1.1")
        {
            if (MethodIsAllowed(Method, _location.getAllowMethods()) == true)
            {
                for (uint64_t i = 0; i < CompareMethods.size(); i++) {
                    if (Method == CompareMethods[i].Method) {
                        FuncPointer = CompareMethods[i].FuncPointer;
                        (this->*FuncPointer)();
                        return;
                    }
                }
            }
        }
        else
        {
            _status_code = e_HTTPVersion;
            createResponse(Generator::generateDefaulPage(*this));
            return ;
        }

        if (MethodIsImplemented(Method) == true)
            _status_code = e_MethodNotAllowed;
        else
            _status_code = e_NotImplemented;


        createResponse(Generator::generateDefaulPage(*this));
    }
	catch (const Respond::BadRequest &e)
	{
		std::cerr << e.what() << std::endl;
		_status_code = e_Badrequest;
		createResponse(Generator::generateDefaulPage(*this));
	}
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        _status_code = e_InternalServerError;
        createResponse(Generator::generateDefaulPage(*this));
    }
}

/* //////////////////////////// */

std::string Respond::getEntryFromMap(const std::string& entry)
{
	HashMap::iterator it = _requestData.find(entry);

		if (it == _requestData.end())
			throw (Respond::BadRequest());
		return (it->second);
}

/* //////////////////////////// */

std::string Respond::getValidFile(const std::string& relativePath)
{
	std::string FileContent;
	std::string LocationIndex = _location.getRoot() + _location.getIndex();

    switch (_status_code)
    {
        case e_OK:
            if (isDirectory(relativePath) == true)
            {
                if (_location.getAutoIndex() == true)
                    FileContent = Generator::generateAutoIndex(*this, relativePath);
                else
                    FileContent = Generator::generateDirectoryPage(*this);
            }
            else
                FileContent = readFile(relativePath);
            break ;
        case e_Redir:
            buildRedir();
            break ;
        case e_NotFound:
            std::cout << relativePath << std::endl;
            if (_location.getAutoIndex() == true && (LocationIndex == relativePath || isDirectory(relativePath)))
                FileContent = Generator::generateAutoIndex(*this, relativePath);
            else
                FileContent = Generator::generateDefaulPage(*this);
            break ;
        default:
            FileContent = Generator::generateDefaulPage(*this);
            break ;
    }
	return (FileContent);
}

const char* Respond::BadRequest::what() const throw()
{
	return ("Bad Request");
}

