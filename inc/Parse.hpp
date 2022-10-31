/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 16:53:31 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/28 18:10:46 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include "Server.hpp"

typedef std::vector<std::string>::iterator vecIt;
typedef std::vector<std::string>	Line;
typedef std::vector<Line>			LocationBlock;
typedef std::vector<Line>			ServerBlock;
typedef std::vector<Line>			File;

#define NR_OF_SERVER_DIRECTIVES 7 //will be more later
#define NR_OF_LOCATION_DIRECTIVES 6 //will be more later

class Parse
{
	public:
		Parse();
		~Parse();

		/* Parsing */
		std::vector<Server>&	parseNetwork(std::string& file, std::vector<Server>& servers);
		Server&					parseServer(ServerBlock& server_block, Server& server);
		Location&				parseLocation(LocationBlock& location_block, Location& location);

		void    				parseServerDirective(Line& line, Server &server);
		void    				parseLocationDirective(Line &line, Location& location);

		void 					openFile(std::ifstream& configStream, std::string configName);

		void    				selectParseFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server);
		bool					isServerDirective(std::string& currentWord);
		bool					isLocationDirective(std::string& currentWord);
		void					selectLocationParserFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server);

		void 					checkWordByWord(std::vector<Server>& servers, std::vector<std::string>& line, std::vector<std::vector<std::string> >& buff/*std::vector<std::string>& buff*/, std::stack<char>& stack);
		void					parseLocationDirectiveBlock(Server& server, std::vector<std::vector<std::string> >::iterator& currentPosInLines, std::vector<std::vector<std::string> >::iterator& endPosOfLines, std::string& locationName);

	private:
		/* Dispatch Table Server Functions*/
		void    				parseListen(Server& server, Line& line);
		void    				parseServerName(Server& server, Line& line);
		void    				parseRoot(Server& server, Line& line);
		void    				parseIndex(Server& server, Line& line);
		void    				parseClientBodySize(Server& server, Line& line);
		void					parseAllowedMethods(Server& server, Line& line);
		void					parseErrorPage(Server& server, Line& line);

		/* Dispatch Table Location Functions*/
		void					parseLocationRoot(Location& location, Line& line);
		void					parseLocationIndex(Location& location, Line& line);
		void					parseLocationAllowMethod(Location& location, Line& line);
		void					parseLocationAutoIndex(Location& location, Line& line);
		void					parseLocationCgiName(Location& location, Line& line);
		void					parseLocationCgiExt(Location& location, Line& line);
		
};

typedef struct dTableServer
{
	std::string		_name;
	void			(Parse::*ServerDirectiveFunction)(Server&, Line&);
}				t_dTableServer;

typedef struct dTableLocation
{
	std::string		_name;
	void			(Parse::*LocationDirectiveFunction)(Location&, Line&);
}				t_dTableLocation;
