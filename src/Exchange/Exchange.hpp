//
// Created by Xander Voorvaart on 10/10/22.
//

#pragma once

#include <iostream>

#include "Server.hpp" // Server class
#include "Respond.hpp"

typedef std::map<std::string, std::string> HashMap;

class Respond ;

class Exchange
{
public:
    Exchange(Server&, int32_t, std::string&);
    ~Exchange(void);

    /* Getters */
	Server 			getServer(void) const;
    HashMap 		getRequestData(void) const;
    int32_t 		getSocketFd(void) const;

	/* Send */
	void			sendToClient(Respond& response);
	void			sendChunked(std::string strToSend);
	void			sendNormal(std::string strToSend);

private:
    Exchange(void);
    Exchange& operator=(const Exchange&);

	std::string		_requestStr;
    HashMap 		_requestData;
    const Server 	_server;
    const int32_t 	_socketFd;
};

