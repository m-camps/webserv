#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>
#include "Server.hpp"

#define NR_OF_DIRECTIVES_TO_LOOK_FOR 5 //will be more later
#define DEBUG false

typedef void 	selectParsingFunction(Server& server, std::string& currentLine);
typedef struct selectParsing
{
	std::string					_name;
	selectParsingFunction*	    _pointerToParsingFunction;
}				t_selectParse;

class Parse
{
	public:
		Parse();
		Parse(const Parse& src);
		Parse& operator=(const Parse& rhs);
		~Parse();

		std::vector<std::vector<std::string> >	parseNetwork(std::string file);

		void 	openFile(std::ifstream& configStream, std::string configName);
		void    processFile(std::ifstream& configStream, Parse& parseInstance, Server& server);
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

/* will have to put these as member functions, messed up syntax so I keep it here for now */
void    startParse(Server& server, char *configName);

void    parseListen(Server& server, std::string& currentLine);
void    parseServerName(Server& server, std::string& currentLine);
void    parseRoot(Server& server, std::string& currentLine);
void    parseIndex(Server& server, std::string& currentLine);
void    parseAutoIndex(Server& server, std::string& currentLine);
void    parseClientBodySize(Server& server, std::string& currentLine);
void    parseLocation(Server& server, std::string& currentLine);
void	parseCgiName(Server& server, std::string& currentLine);
void	parseCgiFileExtension(Server& server, std::string& currentLine);
bool    isLocationDirective(std::string& currentWord);
void    parseAllowMethods(Server& server, std::string& currentLine);
void    proceedToLocationParser(Server& server, std::ifstream& configStream);



#endif
