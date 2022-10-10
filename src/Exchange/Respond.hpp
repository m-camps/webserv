//
// Created by Xander Voorvaart on 10/10/2022.
//

#ifndef WEBSERV_RESPOND_HPP
#define WEBSERV_RESPOND_HPP

#include <iostream>
#include <fstream>
#include <map>

#include "Exchange.hpp" // Exchange Class
#include "../../inc/Server.hpp" // Server class
#include "../../inc/Utilities.hpp" // findCharLocation

typedef std::map<std::string, std::string> HashMap;

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

	std::string readFile(const std::string&);
	void insertBody(std::vector<std::string>&);
	void RespondToClient(void);
	bool CheckConnectionStatus(void);

    // Private Seters
    std::string setStatus(void);
    std::string setContentType(void);
    std::string setContentLength(std::size_t);
};


#endif //WEBSERV_RESPOND_HPP
