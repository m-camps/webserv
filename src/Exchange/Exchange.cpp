//
// Created by Xander Voorvaart on 9/30/22.
//

#include "Exchange.hpp"

////////////// Ctor & Dtor //////////////

/**
 * @param Request will be the header & the body in one string
 */
Exchange::Exchange(const std::string& Request, const Server& new_server, int32_t new_socket)
    : _server(new_server), _ListenSocket(new_socket)
{
    const std::string Header = AppendRequest(Request);

    HeaderToMap(Header);
    RespondToClient();
}

Exchange::Exchange(const Exchange& ref)
    : _server(ref._server), _dictHeader(ref._dictHeader), _ListenSocket(ref._ListenSocket)
{
}

Exchange::~Exchange(void)
{
}

////////////// Operators //////////////

Exchange &Exchange::operator=(const Exchange& ref)
{
    if (this != &ref)
    {
        _dictHeader = ref._dictHeader;
        _server = ref._server;
    }
    return (*this);
}

/*
 * Prints only the std::map::_dictHeader
 */
std::ostream& operator<<(std::ostream& out, const Exchange& ref)
{
    std::map<std::string, std::string> Header = ref.getHeader();
    std::map<std::string, std::string>::iterator it = Header.begin();

    for ( ; it != Header.end(); it++)
    {
        out <<
            std::endl <<
            "Key: " << it->first <<
            "\n" <<
            "Value: " << it->second <<
            std::endl;
    }
    return (out);
}

////////////// Functions //////////////

/**
 * I look for the "\\r\\n\\r\\n" seperator by using std::find.
 * Which I will then append to the Header string.
 */
std::string Exchange::AppendRequest(const std::string& Request) const
{
    std::string Header;

    std::size_t found = Request.find("\r\n\r\n");
    if (found == std::string::npos)
    {
        std::cerr << "No separator found" << std::endl;
//        std::exit(EXIT_FAILURE);
    }
    Header.append(Request, 0, found);
    return (Header);
}

/*
 * This is gonna be a long one...
 *
 * I created a HashMap by using std::map.
 * You can use std::map::at() to find the data you are looking for. \n
 *
 * For example:
 * std::cout << std::map::at("HTTPMethod") << std::endl;
 *
 * Output:
 * GET / HTTP/1.1
 *
 * The following keywords are important:
 * HTTPMethod <-- Here you can find obviously the method and the address that is requested
 * Host <-- What is the hostname
 * Connection <-- To check connection
 * User-Agent <-- Information of the user
 */
void Exchange::HeaderToMap(const std::string& Header)
{
    std::string line;
    std::istringstream issHeader(Header);

    while (std::getline(issHeader, line))
    {
        std::size_t found = line.find(':');
        if (found == std::string::npos)
        {
            _dictHeader["HTTPMethod"] = line.substr(0, line.size() - 1);
            continue ;
        }
        _dictHeader[line.substr(0, found)] =
                line.substr(found + 2, (line.size() - found - 3));
    }
}

////////////// Geter //////////////

const Exchange::map& Exchange::getHeader(void) const
{
    return (_dictHeader);
}

//////////// Responder ////////////

bool Exchange::CheckConnectionStatus(void)
{
    try
    {
        map::iterator ConnectionValue = _dictHeader.find("Connection");

        if (ConnectionValue->second == "keep-alive")
        {
            std::cout << "Client is connected" << "\n";
            return (true);
        }
        std::cerr << "Client disconnected" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (false);
}

std::string Exchange::insertBody(std::vector<std::string>& ServerRoot)
{
    std::cout << ServerRoot.back() << std::endl;
    std::string test = "<!DOCTYPE html>\n"
                       "<html lang=\"en\">\n"
                       "<head>\n"
                       "<meta charset=\"UTF-8\">\n"
                       "<title>Index</title>\n"
                       "</head>\n"
                       "<body>\n"
                       "<h1>This is the default index yo!</h1>\n"
                       "</body>\n"
                       "</html>";

//    std::cout << test.length() << std::endl;
    return (test);
}

void Exchange::RespondToClient(void)
{
    std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 155\r\n"
            "Keep - Alive: timeout=1, max=1\r\n"
            "Accept-Ranges: bytes\r\n"
            "Connection: close\r\n"
            "\r\n\r\n";

    if (!CheckConnectionStatus())
        std::exit(EXIT_FAILURE);
    response += insertBody(_server.getRoot());
    std::cout << response;
    send(_ListenSocket, response.c_str(), response.length(), 0);
}
