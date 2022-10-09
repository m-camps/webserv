//
// Created by Xander Voorvaart on 9/30/22.
//

#ifndef WEBSERV_EXCHANGE_HPP
#define WEBSERV_EXCHANGE_HPP

#include <map>
#include <iostream> // std::cout
#include <fstream> // std::istream
#include <sstream> // issstringstream()

#include "../../inc/Server.hpp" // Server class
#include "../../inc/Utilities.hpp"

class Exchange
{
public:
    typedef std::map<std::string, std::string> map;

    // Ctor & Dtor
    explicit Exchange(const std::string&, const Server&, int32_t);
    Exchange(const Exchange&);
    ~Exchange(void);

    // Operators
    Exchange& operator=(const Exchange& ref);

    // Functions
    const map& getHeader(void) const;

private:
    Server _server;
    map _dictHeader;
    const int32_t _ListenSocket;

    void HeaderToMap(const std::string&);
    void splitMethod(std::string line);
    std::string AppendRequest(const std::string& Request) const;

    std::size_t getBodySize(std::string&) const;
    const std::string getFavicon(void);

    std::string readFile(const std::string&);
    std::string insertBody(std::vector<std::string>&);
    void RespondToClient(void);
    bool CheckConnectionStatus(void);
};

std::ostream& operator<<(std::ostream&, const Exchange&);

#endif //WEBSERV_EXCHANGE_HPP
