//
// Created by Xander Voorvaart on 9/30/22.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include <map>
#include <iostream> // std::cout
#include <fstream> // std::istream
#include <sstream> // issstringstream()

#include "../../inc/Server.hpp" // Server class
#include "Respond.hpp"

class Request
{
public:
    typedef std::map<std::string, std::string> map;

    // Ctor & Dtor
    explicit Request(const std::string&, const Server&, int32_t);
    Request(const Request&);
    ~Request(void);

    // Operators
    Request& operator=(const Request& ref);

    // Functions
    const map& getHeader(void) const;

private:
    Server _server;
    map _dictHeader;
    const int32_t _SocketFD;

    void HeaderToMap(const std::string&);
    void splitMethod(std::string line);
    std::string AppendRequest(const std::string& Request) const;
};

std::ostream& operator<<(std::ostream&, const Request&);

#endif //WEBSERV_REQUEST_HPP
