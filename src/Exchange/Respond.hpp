//
// Created by Xander Voorvaart on 10/10/2022.
//

#ifndef WEBSERV_RESPOND_HPP
#define WEBSERV_RESPOND_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <exception>

#include "Exchange.hpp" // Exchange Class
#include "../../inc/Server.hpp" // Server class
#include "../../inc/Utilities.hpp" // findCharLocation

typedef std::map<std::string, std::string> HashMap;

uint32_t modifyStatusCode(HashMap, const std::string&);
std::string getValidFile(std::string, std::string, uint32_t);

class Respond
{
public:
	Respond(Exchange&);
	Respond(const Respond&);
	~Respond(void);

	Respond& operator=(const Respond&);

private:
	Respond(void);

    Exchange _Exchanger;

	std::size_t getBodySize(std::string&) const;
	const std::string getFavicon(void);

    void ResponseBuilder(void);
	void RespondToClient(void);
	bool CheckConnectionStatus(void);

    // Private Seters
    void setStatus(void);
    void setContentLength(std::size_t);
	void setLocation(const std::string);
};

#endif //WEBSERV_RESPOND_HPP
