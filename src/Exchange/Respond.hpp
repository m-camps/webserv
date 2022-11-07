//
// Created by Xander Voorvaart on 10/10/2022.
//

#pragma once

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

typedef std::map<std::string, std::string> 	HashMap;
typedef std::map<int, std::string> 			ErrorPageMap;

uint32_t modifyStatusCode(std::string, const std::string&);
bool MethodIsAllowed(const std::string& Method, std::vector<std::string>& AllowedMethods);

class Respond
{
	public:
		Respond(Server& server);
		Respond(const Respond&);
		~Respond(void);

		std::string		getHeader(void) const;
		std::string		getBody(void) const;
		HashMap			getRequestData(void) const;
		int				getStatusCode(void) const;
		Server			getServer(void) const;
		std::string		getResponse(void) const;

		void			setHeader(std::string& header);
		void			setBody(std::string body);

		void 			buildResponse(HashMap	requestData);

		void			addLineToResponse(const std::string NewLine);
		std::string 	getEntryFromMap(std::string	entry);

		bool			isChunked;

	private:
		Respond(void);
		Respond& operator=(const Respond&);

		Server			_server;
		HashMap			_requestData;
		std::string 	_header;
		std::string 	_body;
		std::string		_response;
		int				_status_code;

		void 			BuildGet_Redir(void);
		void 			buildGet(void);
		void 			buildPost(void);
		void 			buildDelete(void);

		std::string 	getValidFile(std::string, std::string, uint32_t);
		void 			putBodyInFile(std::string&, std::string&);
		std::string 	ParseBody(void);


		void 			sendAsChunked(void);
};

/* --- Structs --- */

struct s_Methods
{
    std::string Method;
    void (Respond::*FuncPointer)(void);
};

/* --- Enum --- */
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
