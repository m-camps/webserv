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
#define SEPERATOR "\r\n\r\n"

typedef std::map<std::string, std::string> 	HashMap;
typedef std::map<int, std::string> 			ErrorPageMap;

uint32_t modifyStatusCode(const std::string&, const std::string&);
bool MethodIsAllowed(const std::string& Method, std::vector<std::string>& AllowedMethods);

class Respond
{
	public:
		explicit Respond(Server& server);
		~Respond(void);

		/* Getters */
		std::string		getHeader(void) const;
		std::string		getBody(void) const;
		HashMap			getRequestData(void) const;
		int				getStatusCode(void) const;
		Server			getServer(void) const;

		/* Flags */
		bool			IsChunked(void) const;

		/* Setters */
		void			setBody(std::string body);

		/* Adders */
		void			addToHeader(const std::string& NewLine);

		/* Public build function*/
		void 			buildResponse(HashMap	requestData);

		/* Interface */
		std::string 	getEntryFromMap(std::string	entry);

	private:
		Respond(void);
		Respond(const Respond&);
		Respond& operator=(const Respond&);

		Server			_server;
		HashMap			_requestData;
		std::string 	_header;
		std::string 	_body;
		int				_status_code;
		bool			_isChunked;

        // Tijdeijk
        std::string _MetaData;

		/* Build functions */
		void 			BuildRedir(void);
		void 			buildGet(void);
		void 			buildPost(void);
		void 			buildDelete(void);

		/* Helper functions */
		std::string 	getValidFile(std::string, const std::string&, uint32_t);
		std::string		sendSuccesfulUpload(std::string);
		std::string 	ParseBody(void);
		void 			putBodyInFile(std::string&, std::string&);
		void			createResponse(const std::string&);
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
    e_MethodNotFound = 405,
	e_InternalServerError = 500
};
