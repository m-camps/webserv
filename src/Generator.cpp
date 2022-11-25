//
// Created by Xander Voorvaart on 11/1/22.
//

#include "../inc/Generator.hpp"

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
	std::string StatusLine;
	uint32_t StatusCode = Responder.getStatusCode();

	try{
    	StatusLine = Responder.getEntryFromMap("HTTPVersion");

		StatusLine +=  " " + ToString(StatusCode) + " \r\n";
	}
	catch(std::exception &e)
	{
		Responder.setStatusCode(e_Badrequest);
		StatusCode = e_Badrequest;
		StatusLine = "HTTP/1.1";
		StatusLine +=  " " + ToString(StatusCode) + " \r\n";
	}
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
            return ("Invalid request");
        case e_Forbidden:
            return ("Forbidden");
        case e_NotFound:
            return ("Page not found");
        case e_MethodNotAllowed:
            return ("Method not allowed");
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
        case e_Unauthorized:
            return ("Unauthorized");
        case e_HTTPVersion:
            return ("HTTP Version Not Supported");
        default:
            return ("");
    }
}

std::string Generator::generateDefaulPage(const Respond& ResponderRef)
{
    int32_t status_code = ResponderRef.getStatusCode();
    ErrorPageMap ErrorPages = ResponderRef.getServer().getErrorPage();
    ErrorPageMap::const_iterator it = ErrorPages.find(status_code);

    if (it != ErrorPages.end())
    {
        return (readFile(ResponderRef.getLocation().getRoot() + it->second));
    }
    return (
        "<!DOCTYPE html>\n"
        "<body>\n"
        "<h1>ERROR " + ToString(status_code) +
        "</h1>\n" +
        StatusMessage(status_code) +
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

std::string Generator::generateAutoIndex(Respond& Responder, const std::string& relativePath)
{
    DIR *dir;
    bool isRoot = false;
    std::string AutoIndex;
    std::string Path = Responder.getEntryFromMap("Path");
    std::vector<std::string> DirectoryList;

    if (isDirectory(relativePath) == true)
        DirectoryList = ListDir(&dir, relativePath);
    else
    {
        isRoot = true;
        DirectoryList = ListDir(&dir, Responder.getLocation().getRoot());
    }
    std::vector<std::string>::iterator it = DirectoryList.begin();

    AutoIndex =
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "<meta charset=\"UTF-8\">\n"
        "<title>Auto Index of " + relativePath + "</title>\n"
        "</head>\n"
        "<body>\n"
        "<h1>Auto Index of " + relativePath + "</h1>\n";
        if (isRoot == true)
        {
            for (; it != DirectoryList.end(); it++) {
                if (isDirectory(*it) == true)
                    *it += "/";
                AutoIndex += "<a href=\"" + *it + "\">" + *it + "</a><br>\n";
            }
        }
        else
        {
            for (; it != DirectoryList.end(); it++)
            {
                if (isDirectory(*it) == true)
                    *it += "/";
                std::string FullPath = Path + "/" + *it;
                size_t found = FullPath.find("//");
                if (found != std::string::npos)
                    FullPath.erase(found, 1);
                AutoIndex += "<a href=\"" + FullPath  + "\">" + *it + "</a><br>\n";
            }
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

std::string Generator::generateDirectoryPage(Respond& Responder)
{
    std::string FileContent;
    std::string Path = Responder.getLocation().getRoot() + Responder.getLocation().getDefaultFile();

    try
    {
        FileContent = readFile(Path);
    }
    catch (const std::exception& e)
    {
        Responder.setStatusCode(e_NotFound);
        FileContent = Generator::generateDefaulPage(Responder);
    }
    return (FileContent);
}
