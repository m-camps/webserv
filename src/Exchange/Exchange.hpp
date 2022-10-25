//
// Created by Xander Voorvaart on 10/10/22.
//

#ifndef WEBSERV_EXCHANGE_HPP
#define WEBSERV_EXCHANGE_HPP

#include <iostream>

#include "Server.hpp" // Server class

typedef std::map<std::string, std::string> HashMap;

class Exchange
{
public:
    Exchange(Server, int32_t);
    Exchange(const Exchange&);
    ~Exchange(void);

    Exchange& operator=(const Exchange&);

    // Getters
    HashMap getHashMap(void) const;
    std::string getHashMapString(const std::string&) const;
	std::string getHeader(void) const;
    std::string getBody(void) const;
    uint32_t getStatusCode(void) const;
    Server getServer(void) const;
    int32_t getSocketFD(void) const;

    // Setters
    void setHashMap(const HashMap);
    void setHeader(const std::string);
    void setBody(const std::string);
    void setStatusCode(const uint32_t);

    // adders
	void addLineToHeader(const std::string);
    void addHashMapNode(const std::string, const std::string);

private:
    Exchange(void);

    HashMap _dictHeader;
    std::string _header;
    std::string _body;
    uint32_t _statusCode;
    const Server _server;
    const int32_t _SocketFD;
};

std::ostream& operator<<(std::ostream&, const Exchange&);

enum e_statusCode
{
    e_OK = 200,
	e_Accepted = 202,
	e_NoContent = 204,
    e_Redir = 301,
    e_Forbidden = 401,
    e_NotFound = 404,
    e_MethodNotFound = 405
};

#endif //WEBSERV_EXCHANGE_HPP
