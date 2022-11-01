#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <stack>
#include <vector>
#include "Server.hpp"

typedef std::vector<std::string>::iterator vecIt;

#define NR_OF_SERVER_DIRECTIVES 7 //will be more later
#define NR_OF_LOCATION_DIRECTIVES 6 //will be more later

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
		Server&					parseServer(std::vector<std::vector<std::string> >& server_block, Server& server);
		Location&				parseLocation(std::vector<std::string>& location_block, Location& location);

		void    				parseServerDirective(std::vector<std::string>& line, Server &server);
		void    				parseLocationDirective(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& ite, Location& location);

		void    				parseListen(Server& server, std::string& currentLine);
		void    				parseServerName(Server& server, std::string& currentLine);
		void    				parseRoot(Server& server, std::string& currentLine);
		void    				parseIndex(Server& server, std::string& currentLine);
		void    				parseClientBodySize(Server& server, std::string& currentLine);
		void					parseAllowedMethods(Server& server, std::string& currentLine);
		void					parseErrorPage(Server& server, std::string& currentLine);

		void 					openFile(std::ifstream& configStream, std::string configName);

		void    				selectParseFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server);
		bool					isServerDirective(std::string& currentWord);
		bool					isLocationDirective(std::string& currentWord);
		void					selectLocationParserFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server);

		//void					checkWordByWord(std::vector<Server>& servers, std::vector<std::string>& line, std::vector<std::string>& buff, std::stack<char>& stack);
		void 					checkWordByWord(std::vector<Server>& servers, std::vector<std::string>& line, std::vector<std::vector<std::string> >& buff/*std::vector<std::string>& buff*/, std::stack<char>& stack);

		void					parseLocationDirectiveBlock(Server& server, std::vector<std::vector<std::string> >::iterator& currentPosInLines, std::vector<std::vector<std::string> >::iterator& endPosOfLines, std::string& locationName);

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