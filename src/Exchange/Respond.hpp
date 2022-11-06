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
#include "Server.hpp" // Server class
#include "Utilities.hpp" // findCharLocation, readFile, deleteFile & defaultPage
#include "Generator.hpp"

#define CRLF "\r\n"

typedef std::map<std::string, std::string> HashMap;
typedef std::map<int, std::string> ErrorPageMap;

uint32_t modifyStatusCode(std::string, const std::string&);
bool MethodIsAllowed(const std::string& Method, std::vector<std::string>& AllowedMethods);

class Respond
{
public:
	Respond(Exchange&);
	Respond(const Respond&);
	~Respond(void);

private:
	Respond(void);
	Respond& operator=(const Respond&);

    Exchange& _Exchanger;

    // Tijdelijk
    std::string _MetaData;

    void ResponseBuilder(void);
    void BuildGet_Redir(void);
	void BuildGet(void);
	void BuildPost(void);
	void BuildDelete(void);

    std::string getValidFile(std::string, std::string, uint32_t);
	void RespondToClient(void);
    void putBodyInFile(std::string&, std::string&);
    std::string getDataOfBody(void);

    void sendAsChunked(void);
};

#endif //WEBSERV_RESPOND_HPP
