//
// Created by Xander Voorvaart on 10/10/22.
//

#ifndef WEBSERV_EXCHANGE_HPP
#define WEBSERV_EXCHANGE_HPP

#include <string>

#include "Server.hpp" // Server class

typedef std::map<std::string, std::string> HashMap;

class Exchange
{
public:
    Exchange(Server, int32_t);
    Exchange(const Exchange&);
    ~Exchange(void);

    Exchange& operator=(const Exchange&);

    // Geters
    Server getServer(void) const;
    HashMap getHashMap(void) const;
    std::string getBody(void) const;
    int32_t getStatusCode(void) const;
    int32_t getSocketFD(void) const;

    // Seters
    void setServer(const Server);
    void setHashMap(const HashMap);
    void setHeader(const std::string);
    void setBody(const std::string);
    void setStatusCode(const int32_t);

    // adders
    void addHashMapNode(const std::string, const std::string);

private:
    Exchange(void);

    Server _server;
    HashMap _dictHeader;
    std::string _header;
    std::string _body;
    int32_t _statusCode;
    const int32_t _SocketFD;
};

std::ostream& operator<<(std::ostream&, const Exchange&);

#endif //WEBSERV_EXCHANGE_HPP
