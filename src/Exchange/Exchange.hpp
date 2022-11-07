//
// Created by Xander Voorvaart on 10/10/22.
//

#pragma once


#include <iostream>

#include "Server.hpp" // Server class

typedef std::map<std::string, std::string> HashMap;

class Exchange
{
public:
    Exchange(Server&, int32_t&, std::string&);
    ~Exchange(void);

    /* Getters */
	Server 			getServer(void) const;
    HashMap 		getRequestData(void) const;
    int32_t 		getSocketFD(void) const;

	/* Send */
	void			sendToClient(std::string response);
	void			sendToClientChunked(std::string response);

private:
    Exchange(void);
    Exchange& operator=(const Exchange&);

	std::string		_requestStr;
    HashMap 		_requestData;
    const Server 	_server;
    const int32_t 	_socketFD;
};

