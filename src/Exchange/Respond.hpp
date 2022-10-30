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
#include "../../inc/Macros.hpp" // Macros
#include "../../inc/Server.hpp" // Server class
#include "../../inc/Utilities.hpp" // findCharLocation, readFile, deleteFile & defaultPage

#define CRLF "\r\n"

typedef std::map<std::string, std::string> HashMap;

uint32_t modifyStatusCode(std::string, const std::string&);
std::string getValidFile(std::string, std::string, uint32_t);
bool MethodIsAllowed(const std::string& Method, std::vector<std::string>& AllowedMethods);

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

    // Tijdelijk
    std::string _MetaData;

	std::size_t getBodySize(std::string&) const;

    void ResponseBuilder(void);
    void BuildGet_Redir(void);
	void BuildGet(void);
	void BuildPost(void);
	void BuildDelete(void);

	void RespondToClient(void);
    void putBodyInFile(std::string&, std::string&);
    std::string getDataOfBody(void);
    std::string getBodyDataCurl(void);

    // Generate
    void generateStatus(void);
    void generateContentLength(std::size_t);
	void generateLocation(const std::string);
	void generateContentType(void);
    std::string generateBoundry(void);
};

#endif //WEBSERV_RESPOND_HPP
