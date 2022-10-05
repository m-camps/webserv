//
// Created by Xander Voorvaart on 9/30/22.
//

#ifndef WEBSERV_EXCHANGE_HPP
#define WEBSERV_EXCHANGE_HPP

#include <iostream>
#include <map>
#include <sstream>

class Exchange
{
public:
    typedef std::map<std::string, std::string> map;

    // Ctor & Dtor
    explicit Exchange(const std::string&);
    Exchange(const Exchange&);
    ~Exchange(void);

    // Operators
    Exchange& operator=(const Exchange& ref);

    // Functions
    void PrintHeaderMap(void) const;
    const map& getHeader(void) const;
private:
    map _dictHeader;

    void MapTheHeader(const std::string&);
    std::string AppendRequest(const std::string& Request) const;
};


#endif //WEBSERV_EXCHANGE_HPP
