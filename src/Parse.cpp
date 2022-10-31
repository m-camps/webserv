/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 13:00:05 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/31 18:02:21 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Parse.hpp"
#include "../inc/Server.hpp"
#include "../inc/Location.hpp"
#include "../inc/Constants.hpp"
#include "../inc/Macros.hpp"
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <exception>
#include <stack>

int		g_line_count = 1;

/* CONSTRUCTORS */
Parse::Parse() { return ; }
Parse::~Parse() { return ; }

void Parse::openFile(std::ifstream& configStream, std::string configName)
{
	configStream.open(configName);
	if (configStream.is_open())
	{
		std::cout << "Config file opened." << std::endl;
	}
	else
	{
		std::cout << "Failed to open config file." << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

File		Parse::extractFile(std::string& file)
{
	File				ret;
	std::string			line;
	std::ifstream		stream;

	openFile(stream, file);
	while(!stream.eof())
	{
		getline(stream, line);
		if (stream.good())
		{
			Line	tokLine = splitLineWithStrtok(line);
			if (tokLine.empty())
			{
				throw(ExceptionBuilder("Newlines not accepted"));
			}
			else
			{
				ret.push_back(tokLine);
			}
			g_line_count++;
		}
	}
	g_line_count = 1;
	return (ret);
}

std::vector<Server>&	Parse::parseNetwork(std::string& file_name, std::vector<Server>& servers) //parse whole config, puts it into blocks
{
	File				file = extractFile(file_name);

	for (File::iterator it = file.begin(); it != file.end(); it++)
	{
		Line line = *it;
		if (line[0] == "server")
		{
			ServerBlock  	server_block = extractServerBlock(it, file);
			Server			server;
			servers.push_back(parseServer(server_block, server));
		}
	}
	return servers;
}

Server&		Parse::parseServer(ServerBlock& server_block, Server& server)
{
	ServerBlock::iterator it = server_block.begin();

	g_line_count += 2;
	while(it != server_block.end())
	{
		Line line = *it;
		if (isServerDirective(line[0]))
		{
			parseServerDirective(line, server);
		}
		else if (line[0] == "location")
		{
			parseLocationBlock(server, it, server_block);
		}
		else
		{
			throw (ExceptionBuilder("Invalid directive"));
		}
		g_line_count++;
		if (it != server_block.end())
			it++;
	}
	g_line_count++;
	return server;
}

ServerBlock		Parse::extractServerBlock(File::iterator& it, File& file)
{
	ServerBlock			ret;
	std::stack<char>	stack;
	bool				bracket_check = false;

	it++;
	while(it != file.end())
	{
		Line line = *it;
		if (line[0] == "{" && line.size() == 1)
		{
			bracket_check = true;
			stack.push('{');
		}
		else if (line[0] == "}" && line.size() == 1)
		{
			bracket_check = true;
			if (stack.empty())
			{
				throw (ExceptionBuilder("Incorrect brackets in server"));
			}
			stack.pop();
			break ;
		}
		else
		{
			ret.push_back(line);
		}
		it++;
	}
	if (stack.empty() == false || bracket_check == false)
	{
		throw (ExceptionBuilder("Incorrect brackets in server"));
	}
	return ret;
}

LocationBlock	Parse::extractLocationBlock(ServerBlock::iterator&	it, ServerBlock& server_block)
{
	LocationBlock 				ret;
	std::stack<char> 			stack;
	bool						bracket_check = false;

	it++;
	while (it != server_block.end())
	{
		Line line = *it;
		if (line[0] == "[" && line.size() == 1)
		{
			bracket_check = true;
			stack.push('[');
		}
		else if (line[0] == "]" && line.size() == 1)
		{
			bracket_check = true;
			if (stack.empty())
			{
				throw (ExceptionBuilder("Incorrect brackets in location"));
			}
			stack.pop();
			break ;
		}
		else
		{
			ret.push_back(line);
		}
		it++;
	}
	if (stack.empty() == false || bracket_check == false)
	{
		throw (ExceptionBuilder("Incorrect brackets in location"));
	}
	return ret;
}

void		Parse::parseLocationBlock(Server& server, ServerBlock::iterator& it, ServerBlock& server_block)
{
	Line line = *it;
	if (line.size() != 2)
	{
		throw (ExceptionBuilder("Incorrect location name")); 
	}
	LocationBlock 	location_block = extractLocationBlock(it, server_block);
	Location		location;
	std::string name = line[1];
	
	location = parseLocation(location_block, location);
	location.setName(name);
	server.addToLocations(name, location);
}

Location&	Parse::parseLocation(LocationBlock& location_block, Location& location)
{
	LocationBlock::iterator it = location_block.begin();
	
	g_line_count += 2;
	while(it != location_block.end())
	{
		Line line = *it;
		if (isLocationDirective(line[0]))
		{
			parseLocationDirective(line, location);
		}
		else 
		{
			throw (ExceptionBuilder("Invalid directive in location"));
		}
		g_line_count++;
		it++;
	}
	return location;
}

void    Parse::parseServerDirective(Line& line, Server& server)
{	
	void	(Parse::*ServerDirectiveFunction)(Server&, Line&) = NULL;

	const t_dTableServer dTable[] = 
	{
			{"listen", &Parse::parseListen},
			{"server_name", &Parse::parseServerName},
			{"root", &Parse::parseRoot},
			{"index", &Parse::parseIndex},
			{"client_body_size", &Parse::parseClientBodySize},
			{"allow_methods", &Parse::parseAllowedMethods},
			{"error_page", &Parse::parseErrorPage},
	};
	
	for (int i = 0; i < NR_OF_SERVER_DIRECTIVES; i++)
	{
		if (line[0] == dTable[i]._name)
		{
			ServerDirectiveFunction = dTable[i].ServerDirectiveFunction;
			(this->*ServerDirectiveFunction)(server, line);
		}
	}
}

void    Parse::parseLocationDirective(Line& line, Location& location)
{
	void	(Parse::*LocationDirectiveFunction)(Location&, Line&) = NULL;

	const t_dTableLocation dTable[] = 
	{
			{"root", &Parse::parseLocationRoot},
			{"index", &Parse::parseLocationIndex},
			{"allow_methods", &Parse::parseLocationAllowMethod},
			{"autoindex", &Parse::parseLocationAutoIndex},
			{"cgi_name", &Parse::parseLocationCgiName},
			{"cgi_ext", &Parse::parseLocationCgiExt}
	};

	for (int i = 0; i < NR_OF_LOCATION_DIRECTIVES; i++)
	{
		if (line[0] == dTable[i]._name)
		{
			LocationDirectiveFunction = dTable[i].LocationDirectiveFunction;
			(this->*LocationDirectiveFunction)(location, line);
		}
	}
}

/* Dispatch Table Server Functions */
void    Parse::parseRoot(Server& server, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("root directive incorrect"));
	else if(server.getRoot() != "")
		throw (ExceptionBuilder("duplicate root"));
	server.setRoot(line[1]);
}

void    Parse::parseListen(Server& server, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("listen directive incorrect"));
	else if (!isNumber(line[1]))
		throw (ExceptionBuilder("listen directive not a number"));
		
	int port = strtol(line[1].c_str(), nullptr, 10); // CHECK THIS
	std::vector<int> ports = server.getPorts();
	
	if (std::find(ports.begin(), ports.end(), port) != ports.end())
		throw (ExceptionBuilder("Duplicate port"));
	server.addToPorts(port);
}

void    Parse::parseIndex(Server& server, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("index directive incorrect"));
	else if(server.getIndex() != "")
		throw (ExceptionBuilder("duplicate index"));
	server.setIndex(line[1]);
}

void    Parse::parseServerName(Server& server, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("server_name directive incorrect"));

	std::string server_name = line[1];
	std::vector<std::string> names = server.getNames();

	if (std::find(names.begin(), names.end(), server_name) != names.end())
		throw (ExceptionBuilder("Duplicate server_name"));
		
	server.addToNames(server_name);
}

void    Parse::parseClientBodySize(Server& server, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("client_body_size directive incorrect"));
	else if (!isNumber(line[1]))
		throw (ExceptionBuilder("client_body_size directive not a number"));

	int clientBodySize = std::stoi(line[1], nullptr, 10);
	server.setClientBody(clientBodySize);
}


void	Parse::parseAllowedMethods(Server& server, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("allow_methods directive incorrect"));

	std::string method = line[1];
	std::vector<std::string> methods = server.getMethods();
	
	if (method == "GET" || method == "POST" || method == "DELETE")
	{
		if (std::find(methods.begin(), methods.end(), method) != methods.end())
			throw (ExceptionBuilder("Duplicate port"));
		server.addToMethods(method);
	}
}

void	Parse::parseErrorPage(Server& server, Line& line)
{
	if (line.size() != 3)
		throw (ExceptionBuilder("error_page directive incorrect"));
	else if (!isNumber(line[1]))
		throw (ExceptionBuilder("status_code is not a number"));

	int status_code = std::stoi(line[1], nullptr, 10);
	std::string page = line[2];
	server.addToErrorPages(status_code, page);
}

/* Dispatch Table Location Functions */
void	Parse::parseLocationRoot(Location& location, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("root location directive incorrect"));
	else if(location.getRoot() != "")
		throw (ExceptionBuilder("duplicate root in location"));
	location.setRoot(line[1]);
}

void	Parse::parseLocationIndex(Location& location, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("index location directive incorrect"));
	else if(location.getIndex() != "")
		throw (ExceptionBuilder("duplicate index in location"));
	location.setIndex(line[1]);
}

void	Parse::parseLocationAllowMethod(Location& location, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("allow_methods directive incorrect"));

	std::string method = line[1];
	std::vector<std::string> methods = location.getAllowMethods();
	
	if (method == "GET" || method == "POST" || method == "DELETE")
	{
		if (std::find(methods.begin(), methods.end(), method) != methods.end())
			throw (ExceptionBuilder("Duplicate port"));
		location.addToAllowMethod(method);
	}
}

void	Parse::parseLocationAutoIndex(Location& location, Line& line)
{
	int		autoindex = -1;
	
	if (line.size() != 2)
		throw (ExceptionBuilder("autoindex location directive incorrect"));
	else if(location.getAutoIndex() != -1)
		throw (ExceptionBuilder("duplicate autoindex in location"));
	if (line[1] == "on")
		autoindex = 1;
	else if (line[1] == "off")
		autoindex = 0;
	else
		throw (ExceptionBuilder("autoindex value incorrect in location"));
	location.setAutoIndex(autoindex);
}

void	Parse::parseLocationCgiName(Location& location, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("cgi_name location directive incorrect"));
	else if(location.getCgiName() != "")
		throw (ExceptionBuilder("duplicate cgi_name in location"));
	location.setCgiName(line[1]);
}

void	Parse::parseLocationCgiExt(Location& location, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("cgi_ext location directive incorrect"));
	else if(location.getCgiFileExtension() != "")
		throw (ExceptionBuilder("duplicate cgi_ext in location"));
	location.setCgiExt(line[1]);
}

/* Helper Functions */
std::invalid_argument Parse::ExceptionBuilder(std::string error)
{
	std::string str;
	
	str.append("Parsing Error | Line: " + TOSTRING(g_line_count) + " | ");
	str.append(error);
	return std::invalid_argument(str);
}

Line 	Parse::splitLineWithStrtok(std::string& line)
{
	char	*c_line = strdup(const_cast<char *>(line.c_str()));
	char	*word;
	Line 	ret;
	char 	delimit[]= " \t\r\n\v\f";

	word = strtok(c_line, delimit);
	while (word != NULL)
	{
		ret.push_back(word);
		word = strtok(NULL, delimit);
	}
	if (DEBUG)
	{
		for (size_t i = 0; i < ret.size(); i++)
			std::cout  << "[word " << i << "]: " << ret[i] << std::endl;
	}
	free(c_line);
	return ret;
}

bool Parse::isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();

    while (it != s.end() && std::isdigit(*it))
	{
		it++;
	}
    return (!s.empty() && it == s.end());
}

bool    Parse::isServerDirective(std::string& directive)
{
	return (directive == "server_name" || directive == "root" || \
			directive == "index" || directive == "client_body_size" || \
			directive == "listen" || directive == "error_page" || directive == "allow_methods");
}

bool    Parse::isLocationDirective(std::string& directive)
{
	return (directive == "root" || directive == "index" ||
			directive == "allow_methods" || directive == "autoindex" ||
			directive == "cgi_name" || directive == "cgi_ext");
}
