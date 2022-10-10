//
// Created by Xander Voorvaart on 10/10/2022.
//

#ifndef WEBSERV_RESPOND_HPP
#define WEBSERV_RESPOND_HPP

#include <iostream>
#include <fstream>
#include <map>

#include "../../inc/Server.hpp" // Server class
#include "../../inc/Utilities.hpp" // findCharLocation


class Respond
{
public:
	typedef std::map<std::string, std::string> HashMap;

	Respond(Server, HashMap, const int32_t);
	Respond(const Respond&);
	~Respond(void);

	void operator=(const Respond&);
private:
	Respond(void);

	Server _server;
	HashMap _dictHeader;
	int32_t _statusCode;
	const int32_t _SocketFD;

	std::size_t getBodySize(std::string&) const;
	const std::string getFavicon(void);

	std::string readFile(const std::string&);
	std::string insertBody(std::vector<std::string>&);
	void RespondToClient(void);
	bool CheckConnectionStatus(void);
};


#endif //WEBSERV_RESPOND_HPP
