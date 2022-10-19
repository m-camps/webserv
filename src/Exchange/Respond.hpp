//
// Created by Xander Voorvaart on 10/10/2022.
//

#ifndef WEBSERV_RESPOND_HPP
#define WEBSERV_RESPOND_HPP

#include <iostream> // std::
#include <fstream> // std::istream
#include <sstream> // std::istringstream
#include <map> // std::map<>
#include <exception> // std::exceptions

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
	void BuildGet(void);
	void BuildPost(void);
	void BuildDelete(void);

	void RespondToClient(void);
	bool CheckConnectionStatus(void);
    void putBodyInFile(std::string&);
    std::string getDataOfBody(void);

    // Generate
    void generateStatus(void);
    void generateContentLength(std::size_t);
	void generateLocation(const std::string);
	void generateContentType(void);
    std::string generateBoundry(void);
};

#endif //WEBSERV_RESPOND_HPP
