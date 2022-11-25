/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 16:53:31 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/22 16:55:06 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <stack>
#include <vector>

#include "Server.hpp"
#include "Utilities.hpp"

typedef std::vector<std::string>			Line;
typedef std::vector<Line>					LocationBlock;
typedef std::vector<Line>					ServerBlock;
typedef std::vector<Line>					Block;
typedef std::vector<Line>					File;

#define NR_OF_LOCATION_DIRECTIVES 8

class Parse
{
	public:
		Parse();
		~Parse();

		/* Parsing */
		std::vector<Server>&	parseNetwork(std::string& file, std::vector<Server>& servers);
		Server&					parseServer(ServerBlock& server_block, Server& server);
		Location&				parseLocation(LocationBlock& location_block, Location& location);

		void					parseLocationName(Server& server, ServerBlock::iterator& it, ServerBlock& server_block);
		Block					extractBlock(Block::iterator& it, Block& block, std::string type);
		
	private:
		/* Dispatch Table Server Functions */
		void    				parseServerDirective(Line& line, Server &server);
		void    				parseListen(Server& server, Line& line);
		void    				parseServerName(Server& server, Line& line);
		void    				parseClientBodySize(Server& server, Line& line);
		void					parseErrorPage(Server& server, Line& line);

		/* Dispatch Table Location Functions */
		void    				parseLocationDirective(Line &line, Location& location);
		void					parseLocationRoot(Location& location, Line& line);
		void					parseLocationIndex(Location& location, Line& line);
		void					parseLocationAllowMethod(Location& location, Line& line);
		void					parseLocationAutoIndex(Location& location, Line& line);
		void					parseLocationCgi(Location& location, Line& line);
		void					parseLocationReturn(Location& location, Line& line);
		void					parseLocationDefaultFile(Location& location, Line& line);

		/* File I/O */
		File					extractFile(std::string& file);

		/* Validator */
		std::invalid_argument 	ValidateException(std::string error, int block);
		void					validate(std::vector<Server>& servers);
		void					validateServer(Server& server, int block);
		void					validateLocation(Location& location, int block);
		
		/* Helper Functions */
		std::invalid_argument 	ExceptionBuilder(std::string error);
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
