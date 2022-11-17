//
// Created by Xander Voorvaart on 11/1/22.
//

#include "Generator.hpp"

Generator::Generator(void)
{
}

Generator::~Generator(void)
{
}

////////////// Generators //////////////

/*
 * The status of the site should ALWAYS be the first line of the header.
 * The rest can be any line.
 */
std::string Generator::generateStatus(Respond& Responder)
{
    HashMap tempHash = Responder.getRequestData();
    uint32_t StatusCode = Responder.getStatusCode();
    std::string StatusLine = tempHash.find("HTTPVersion")->second;

    StatusLine +=  " " + ToString(StatusCode) + " \r\n";
	return (StatusLine);
}

/* //////////////////////////// */

std::string Generator::generateTransferEncoding(void)
{
    std::string TransferEncoding = "Transfer-Encoding: chunked\r\n";
	return (TransferEncoding);
}

/* //////////////////////////// */

std::string Generator::generateContentLength(std::size_t BodyLength)
{
    std::string ContentLength = "Content-Length: " + ToString(BodyLength) + "\r\n";
    return (ContentLength);
}

/* //////////////////////////// */

std::string ConvertDecimalToHex(size_t decimal)
{
    std::stringstream ss;

    ss << std::hex << decimal;
    return (ss.str());
}

std::string Generator::generateChunk(std::string& Body)
{
    std::string Chunk;

    if (Body.length() > MAXBYTES)
    {
        Chunk = ConvertDecimalToHex(MAXBYTES) + CRLF;
        Chunk += Body.substr(0, MAXBYTES) + CRLF;
        Body = Body.substr(MAXBYTES, Body.length() - MAXBYTES);
    }
    else
    {
        Chunk = ConvertDecimalToHex(Body.length()) + CRLF;
        Chunk += Body.substr(0, Body.length()) + CRLF;
        Body.clear();
    }

    return (Chunk);
}

/* //////////////////////////// */

std::string Generator::generateLocation(const std::string& NewLocation)
{
    const std::string Location = "Location: " + NewLocation + "\r\n";
	return (Location);
}

/* //////////////////////////// */

std::string Generator::generateBoundry(Respond& Responder)
{
    std::string ContentType = Responder.getEntryFromMap("Content-Type");

    std::size_t found = ContentType.find('=');
    if (found == std::string::npos)
        throw (std::logic_error("No boundry found"));

    return (ContentType.substr(found + 1, ContentType.length() - found) + CRLF);
}

/* //////////////////////////// */

std::string StatusMessage(int32_t StatusCode)
{
    switch (StatusCode)
    {
        case e_Badrequest:
            return ("invalid request");
        case e_Forbidden:
            return ("Forbidden");
        case e_NotFound:
            return ("Page not found");
        case e_MethodNotFound:
            return ("Method not found");
        case e_PayloadTooLarge:
            return ("Payload too large");
        case e_UnsupportedMediaType:
            return ("Unsupported Media Type");
        case e_InternalServerError:
            return ("Internal Server Error");
        case e_NotImplemented:
            return ("Method not implemented");
        case e_BadGateway:
            return ("Bad Gateway");
        default:
            return ("");
    }
}

std::string Generator::generateDefaulPage(uint32_t StatusCode)
{
     return (
            "<!DOCTYPE html>\n"
            "<body>\n"
            "<h1>ERROR " + ToString(StatusCode) +
            "</h1>\n" +
            StatusMessage(StatusCode) +
            "</body>\n"
            "</html>"
    );
}

/* //////////////////////////// */

std::vector<std::string> ListDir(DIR **dir, std::string Root)
{
    dirent *t_dirlist;
    std::vector<std::string> DirectoryList;

    *dir = opendir(Root.c_str());
    if (!*dir)
    {
        std::string StrError = std::strerror(errno);
        throw (std::logic_error(StrError));
    }

    while ((t_dirlist = readdir(*dir)) != NULL)
    {
        DirectoryList.push_back(t_dirlist->d_name);
    }
    return (DirectoryList);
}

std::string Generator::generateAutoIndex(Respond& Responder)
{
    DIR *dir;
    std::string AutoIndex;
    std::string Root = Responder.getLocation().getRoot();
    std::vector<std::string> DirectoryList = ListDir(&dir, Root);
    std::vector<std::string>::iterator it = DirectoryList.begin();

    AutoIndex =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "<meta charset=\"UTF-8\">\n"
        "<title>Auto Index of " + Root + "</title>\n"
        "</head>\n"
        "<body>\n"
        "<h1>Auto Index of " + Root + "</h1>";
        for (; it != DirectoryList.end(); it++)
        {
            if (isDirectory(*it) == true)
                *it += "/";
             AutoIndex += "<a href=\"" + *it + "\">" + *it + "</a><br>\n";
        }
        AutoIndex += "</pre><hr></body>\n"
        "</html>";

    if (closedir(dir) < 0)
    {
        std::string StrError = std::strerror(errno);
        throw (std::logic_error(StrError));
    }
    return (AutoIndex);
}

/* //////////////////////////// */

std::string Generator::generateDirectoryPage(const std::string& Path)
{
    return (
            "<!DOCTYPE html>\n"
            "<body>\n"
            "<h1>You are in directory: " + Path + "</h1>"
            "</body>\n"
            "</html>"
            );
}
