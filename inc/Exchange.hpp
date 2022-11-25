//
// Created by Xander Voorvaart on 10/10/22.
//

#pragma once

#include <iostream>

#include "Server.hpp"
#include "Respond.hpp"

typedef std::map<std::string, std::string>  HashMap;

class Respond ;

class Exchange
{
public:
    Exchange(Servers&, int32_t, const HashMap&);
    ~Exchange(void);

	/* Send */
	void			sendToClient(Respond& response);
	void			sendChunked(std::string str) const;
	void			sendNormal(const std::string& str) const;

	/* Matching */
	Server	    matchServer(Servers servers, HashMap requestData);
	Location	matchLocation(const Server& server, HashMap requestData);

    class WriteException : public std::exception
    {
    public:
        const char* what() const throw();
    };
	class ClearException : public std::exception
    {
    public:
        const char* what() const throw();
    };

private:
    Exchange(void);
    Exchange& operator=(const Exchange&);

	std::string		_requestStr;
    HashMap 		_requestData;
    Server 			_server;
    const int32_t 	_socketFd;
};

