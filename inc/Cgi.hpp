#ifndef CGI_HPP
#define CGI_HPP
#include <string>
#define NR_OF_CGI_ENV_VARS 24
#include "../src/Exchange/Exchange.hpp"

class Cgi
{
	public:
		Cgi();
		std::string*	getEnvTable(void);
		void			printEnv(void);
		void			createCgiResponse(void);
		void			executeScript(Exchange& ExchangeRef);
		void			parentProcess(int* fds, int& stat);
		void			childProcess(int* fds);

		//i need to construct a header separate from the script
			// VERSION & STATUSCODE FIRST
			// ContentLength, if there is a body
			// Content-Type
		//call the script which will send me the body
		//
	private:
		// std::string _ENV[24] = 
		// {
		// 	"COMSPEC",
		// 	"DOCUMENT_ROOT", //The root directory of your server
		// 	"GATEWAY_INTERFACE",
		// 	"HTTP_ACCEPT",
		// 	"HTTP_ACCEPT_ENCODING",
		// 	"HTTP_ACCEPT_LANGUAGE",
		// 	"HTTP_CONNECTION",
		// 	"HTTP_HOST", //The hostname of the page being attempted
		// 	"HTTP_USER_AGENT", //The browser type of the visitor
		// 	"PATH", //The system path your server is running under
		// 	"QUERY_STRING", //The query string (see GET, below)
		// 	"REMOTE_ADDR", //The IP address of the visitor
		// 	"REMOTE_PORT", //The port the visitor is connected to on the web server
		// 	"REQUEST_METHOD", //GET or POST
		// 	"REQUEST_URI", //The interpreted pathname of the requested document or CGI (relative to the document root)
		// 	"SCRIPT_FILENAME", //The full pathname of the current CGI
		// 	"SCRIPT_NAME", //The interpreted pathname of the current CGI (relative to the document root)
		// 	"SERVER_ADDR",
		// 	"SERVER_ADMIN", //	The email address for your server's webmaster
		// 	"SERVER_NAME", //Your server's fully qualified domain name (e.g. www.cgi101.com)
		// 	"SERVER_PORT", //The port number your server is listening on
		// 	"SERVER_PROTOCOL",
		// 	"SERVER_SIGNATURE",
		// 	"SERVER_SOFTWARE" //The server software you're using (e.g. Apache 1.3)
		// };
};

#endif
