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
#include "Cgi.hpp"

#define CRLF "\r\n"
#define SEPERATOR "\r\n\r\n"

typedef std::map<std::string, std::string> 	HashMap;
typedef std::map<int, std::string> 			ErrorPageMap;

bool MethodIsAllowed(const std::string& Method, std::vector<std::string> AllowedMethods);

class Respond
{
	public:
		explicit Respond(Server& server, Location& location);
		~Respond(void);

		/* Getters */
		std::string		getHeader(void) const;
		std::string		getBody(void) const;
		HashMap			getRequestData(void) const;
		int				getStatusCode(void) const;
		Server			getServer(void) const;
        Location        getLocation(void) const;

		/* Flags */
		bool			IsChunked(void) const;

		/* Setters */
		void			setBody(std::string body);
		void			setStatusCode(int statusCode);
		/* Adders */
		void			addToHeader(const std::string& NewLine);

		/* Public build function*/
		void 			buildResponse(HashMap	requestData);

		/* Interface */
		std::string 	getEntryFromMap(const std::string&	entry);

	private:
		Respond(void);
		Respond(const Respond&);
		Respond& operator=(const Respond&);

		const Server	_server;
        const Location  _location;
		HashMap			_requestData;
		std::string 	_header;
		std::string 	_body;
		int32_t			_status_code;
		bool			_isChunked;

        /* POST Functions */
		void 			buildPost(void);
		void 			putBodyInFile(std::string&, std::string&);
        bool            isPostValid(void);
		std::string		sendSuccesfulUpload(std::string);
        std::string     removeBoundry();
        std::string     parseMetadata(std::string&);

        /* GET Functions */
		void 			buildGet(void);
		void 			buildRedir(void);
		bool			correctCgiRequestAllowed(void);
		std::string 	getValidFile(const std::string&);

        /* DELETE Functions */
		void 			buildDelete(void);

		/* Helper functions */
		void			createResponse(const std::string&);
        void            parsePath(std::string&);
        void            modifyStatuscode(const std::string&, const std::string&);

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
    e_Unauthorized = 401,
    e_Forbidden = 403,
    e_NotFound = 404,
    e_MethodNotFound = 405,
    e_PayloadTooLarge = 413,
    e_UnsupportedMediaType = 415,
	e_InternalServerError = 500,
    e_NotImplemented = 501,
	e_BadGateway = 502
};
