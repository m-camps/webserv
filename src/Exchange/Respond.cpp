//
// Created by Xander Voorvaart on 10/10/2022.
//

#include "Respond.hpp"

#define ERROR 1

#pragma region "ctor & dtor"

////////// Ctor & Dtor ///////////

Respond::Respond(Server& server) : _server(server), _isChunked(false) {}
Respond::~Respond(void) {}

#pragma endregion "ctor & dtor"

std::string		Respond::getHeader(void) const { return _header; }
std::string		Respond::getBody(void) const { return _body; }
HashMap			Respond::getRequestData(void) const { return _requestData; }
int				Respond::getStatusCode(void) const { return _status_code; }
Server			Respond::getServer(void) const { return _server; }

bool			Respond::IsChunked(void) const { return _isChunked; }

void			Respond::setBody(std::string body) { _body = body; }


void 		Respond::addToHeader(const std::string NewLine)
{
	std::string NewHeader = _header + NewLine;

	_header = NewHeader;
}
//////////// Responder ////////////

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

        for (int32_t i = 0; i < 3; i++) {
            if (Method == CompareMethods[i].Method) {
                FuncPointer = CompareMethods[i].FuncPointer;
                (this->*FuncPointer)();
                return;
            }
        }

//        BuildHeader(); <-- 405 Method is not allowed
    }
    catch (const std::exception &e)
    {
        throw (e);
    }
}

std::string Respond::getEntryFromMap(std::string entry)
{
	HashMap::iterator it = _requestData.find(entry);

        if (it == _requestData.end())
            throw (std::invalid_argument("Invalid string"));
        return (it->second);
}

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
        std::string Root = getServer().getRoot();
        std::ofstream File(Root + "/" + getFilename(MetaData));

        File << Body;
    }
    catch (const std::exception& e)
    {
        throw (e);
    }
}

std::string Respond::getValidFile(std::string Root, std::string relativePath, uint32_t StatusCode)
{
	std::string FileContent;
    ErrorPageMap ErrorPages = getServer().getErrorPage();
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
                BuildRedir();
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

