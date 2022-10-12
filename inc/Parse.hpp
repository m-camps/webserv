#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>
#include "Server.hpp"

#define NR_OF_DIRECTIVES_TO_LOOK_FOR 5 //will be more later
#define DEBUG false

class Parse
{
	public:
		Parse();
		Parse(const Parse& src);
		Parse& operator=(const Parse& rhs);
		~Parse();

		/* Parsing */
		std::vector<Server>&	parseNetwork(std::string& file, std::vector<Server>& servers);
		Server&					parseServer(std::vector<std::string>& server_block, Server& server);
		Location&				parseLocation(std::vector<std::string>& location_block, Location& location);

		void    				parseDirective(std::string& currentLine, Server &server);
		void    				parseLocationDirective(std::string& currentLine, Location& location);

		void    				parseListen(Server& server, std::string& currentLine); //was outside before, now member function
		void    				parseServerName(Server& server, std::string& currentLine); //was outside before, now member function
		void    				parseRoot(Server& server, std::string& currentLine); //was outside before, now member function
		void    				parseIndex(Server& server, std::string& currentLine); //was outside before, now member function
		void    				parseClientBodySize(Server& server, std::string& currentLine); //was outside before, now member function



		void 					openFile(std::ifstream& configStream, std::string configName);

		void    selectParseFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server);
		bool    isDirective(std::string& currentWord);
		bool	isLocationDirective(std::string& currentWord);
		void    selectLocationParserFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server);

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
