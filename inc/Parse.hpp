#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>
#include "Server.hpp"

typedef std::vector<std::string>::iterator vecIt;

#define NR_OF_DIRECTIVES_TO_LOOK_FOR 5 //will be more later

class Parse
{
	public:
		Parse();
		Parse(const Parse& src);
		Parse& operator=(const Parse& rhs);
		~Parse();

		/* Parsing */
		std::vector<Server>&	parseNetwork(std::string& file, std::vector<Server>& servers);
		//void					searchForServerBlock(std::vector<std::string> tokenizedLine, std::vector<std::string>& buff ,std::vector<Server>& servers);
		//void					otherf(std::vector<std::string> location_block, std::vector<std::string> line, Server& server);
		Server&					parseServer(std::vector<std::string>& server_block, Server& server);
		Location&				parseLocation(std::vector<std::string>& location_block, Location& location);

		void    				parseDirective(std::string& currentLine, Server &server);
		void    				parseLocationDirective(std::string& currentLine, Location& location);

		void    				parseListen(Server& server, std::string& currentLine);
		void    				parseServerName(Server& server, std::string& currentLine);
		void    				parseRoot(Server& server, std::string& currentLine);
		void    				parseIndex(Server& server, std::string& currentLine);
		void    				parseClientBodySize(Server& server, std::string& currentLine);


		void 					openFile(std::ifstream& configStream, std::string configName);

		void    				selectParseFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server);
		bool					isServerDirective(std::string& currentWord);
		bool					isLocationDirective(std::string& currentWord);
		void					selectLocationParserFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server);

	private:
		/*** 
		* dispatch table, name is for the directives like server/listen/root/location/ etc
		* pointerToParsingFunction is the function associated with the name, in case you find one of those in the config file, 
		* call the this function to parse further and assign values etc to the server class
		***/
		//t_selectParse _dispatchTable;
};

typedef struct selectParsingServer
{
	std::string		_name;
	void			(Parse::*pointerToParseServerDirectives)(Server& server, std::string& currentLine);
}				t_selectServer;
#endif
