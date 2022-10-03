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
    void MapTheHeader(const std::string&);
    std::string AppendRequest(const std::string& Request);

private:
    std::map<std::string, std::string> _dictHeader;
};


#endif //WEBSERV_HEADERPARSE_HPP
