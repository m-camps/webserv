/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 16:53:31 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/31 18:02:37 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include "Server.hpp"

typedef std::vector<std::string>::iterator 	vecIt;

typedef std::vector<std::string>			Line;
typedef std::vector<Line>					LocationBlock;
typedef std::vector<Line>					ServerBlock;
typedef std::vector<Line>					File;

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

		void					parseLocationBlock(Server& server, ServerBlock::iterator& it, ServerBlock& server_block);
		LocationBlock			extractLocationBlock(ServerBlock::iterator&	it, ServerBlock& server_block);
		ServerBlock				extractServerBlock(File::iterator& it, File& file);
		
	private:
		/* Dispatch Table Server Functions */
		void    				parseServerDirective(Line& line, Server &server);
		void    				parseListen(Server& server, Line& line);
		void    				parseServerName(Server& server, Line& line);
		void    				parseRoot(Server& server, Line& line);
		void    				parseIndex(Server& server, Line& line);
		void    				parseClientBodySize(Server& server, Line& line);
		void					parseAllowedMethods(Server& server, Line& line);
		void					parseErrorPage(Server& server, Line& line);

		/* Dispatch Table Location Functions */
		void    				parseLocationDirective(Line &line, Location& location);
		void					parseLocationRoot(Location& location, Line& line);
		void					parseLocationIndex(Location& location, Line& line);
		void					parseLocationAllowMethod(Location& location, Line& line);
		void					parseLocationAutoIndex(Location& location, Line& line);
		void					parseLocationCgiName(Location& location, Line& line);
		void					parseLocationCgiExt(Location& location, Line& line);

		/* File I/O */
		void 					openFile(std::ifstream& configStream, std::string configName);
		File					extractFile(std::string& file);	

		/* Helper Functions */
		std::invalid_argument 	ExceptionBuilder(std::string error);
		Line 					splitLineWithStrtok(std::string& line);
		bool 					isNumber(const std::string& s);
		bool					isServerDirective(std::string& currentWord);
		bool					isLocationDirective(std::string& currentWord);
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
