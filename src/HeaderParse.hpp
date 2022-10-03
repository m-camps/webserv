//
// Created by Xander Voorvaart on 9/30/22.
//

#ifndef WEBSERV_HEADERPARSE_HPP
#define WEBSERV_HEADERPARSE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sstream>

class HeaderParse
{
public:
    // Ctor & Dtor
    HeaderParse(const std::string&);
    HeaderParse(const HeaderParse&);
    ~HeaderParse(void);

    // Operators
    HeaderParse& operator=(const HeaderParse& ref);

    // Functions

private:
    const std::string _HTTPMethod;
    const std::string _Host;
    const std::string _Useragent;
    const std::string _connection;
};


#endif //WEBSERV_HEADERPARSE_HPP
