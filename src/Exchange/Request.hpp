//
// Created by Xander Voorvaart on 9/30/22.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include <map>
#include <iostream> // std::cout
#include <fstream> // std::istream
#include <sstream> // issstringstream()

#include "Respond.hpp"
#include "Server.hpp" // Server class

typedef std::map<std::string, std::string> map;

class Request
{
public:
    typedef std::map<std::string, std::string> map;

    // Ctor & Dtor
    explicit Request(std::string, Exchange);
    Request(const Request&);
    ~Request(void);

    // Operators
    Request& operator=(const Request& ref);

    // Functions
private:
    Request(void);

    Exchange _Exchanger;

    void HeaderToMap(const std::string&);
    void splitMethod(std::string line);
    std::string AppendRequest(const std::string& Request);
};

#endif //WEBSERV_REQUEST_HPP
