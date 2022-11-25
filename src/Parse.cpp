/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Parse.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 13:00:05 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/23 13:14:48 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Parse.hpp"
#include "../inc/Server.hpp"
#include "../inc/Location.hpp"
#include "../inc/Constants.hpp"
#include "../inc/Utilities.hpp"
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

File		Parse::extractFile(std::string& file)
{
	File						ret;
	std::string					full_file = readFile(file);
	std::vector<std::string>	splitFile = splitLineWithStrtok(full_file, "\n"); 

	for (std::vector<std::string>::iterator it = splitFile.begin(); it != splitFile.end(); it++)
	{
		Line tokLine = splitLineWithStrtok(*it, " \t\r\n\v\f");
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
			ServerBlock  	server_block = extractBlock(it, file, "server");
			Server			server;
			servers.push_back(parseServer(server_block, server));
		}
		else
		{
			throw(ExceptionBuilder("No server block found"));
		}
	}
	validate(servers);
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
			parseLocationName(server, it, server_block);
		}
		else
		{
			throw (ExceptionBuilder("Invalid directive"));
		}
		g_line_count++;
		if (it != server_block.end())
		{
			it++;
		}
	}
	g_line_count++;
	return server;
}

Block		Parse::extractBlock(Block::iterator& it, Block& block, std::string type)
{
	Block					ret;
	std::stack<std::string>	stack;
	bool					bracket_check = false;
	std::string				o_brack = (type == "server")? "{" : "[";
	std::string				c_brack = (type == "server") ? "}" : "]";

	it++;
	while(it != block.end())
	{
		Line line = *it;
		if (line[0] == o_brack && line.size() == 1)
		{
			bracket_check = true;
			stack.push(o_brack);
		}
		else if (line[0] == c_brack && line.size() == 1)
		{
			bracket_check = true;
			if (stack.empty())
			{
				throw (ExceptionBuilder("Incorrect brackets in " + type));
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
		throw (ExceptionBuilder("Incorrect brackets in " + type));
	}
	return ret;
}

void		Parse::parseLocationName(Server& server, ServerBlock::iterator& it, ServerBlock& server_block)
{
	Line line = *it;
	if (line.size() != 2)
	{
		throw (ExceptionBuilder("Incorrect location name")); 
	}
	
	LocationBlock 	location_block = extractBlock(it, server_block, "location");
	Location		location;
	std::string name = line[1];
	std::map<std::string, Location> locations = server.getLocations();
	
	if (locations.count(name) > 0)
		throw (ExceptionBuilder("duplicate location name in server"));
	location = parseLocation(location_block, location);
	if (name.length() != 1 && name.back() == '/')
		throw (ExceptionBuilder("Incorrect name of locationblock. Please remove the trailing / after the name."));
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
			{"client_body_size", &Parse::parseClientBodySize},
			{"error_page", &Parse::parseErrorPage},
			{"", NULL}
	};
	
	for (int i = 0; !dTable[i]._name.empty(); i++)
	{
		if (line[0] == dTable[i]._name)
		{
			ServerDirectiveFunction = dTable[i].ServerDirectiveFunction;
			(this->*ServerDirectiveFunction)(server, line);
			break ;
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
			{"cgi", &Parse::parseLocationCgi},
			{"return", &Parse::parseLocationReturn},
			{"default_file", &Parse::parseLocationDefaultFile}
	};

	for (int i = 0; i < NR_OF_LOCATION_DIRECTIVES; i++)
	{
		if (line[0] == dTable[i]._name)
		{
			LocationDirectiveFunction = dTable[i].LocationDirectiveFunction;
			(this->*LocationDirectiveFunction)(location, line);
			break ;
		}
	}
}

/* Dispatch Table Server Functions */
void    Parse::parseListen(Server& server, Line& line)
{
	if (line.size() < 2)
		throw (ExceptionBuilder("listen directive empty"));

	for (size_t i = 1; i < line.size(); i++)
	{
		if (!isNumber(line[i]))
			throw (ExceptionBuilder("listen directive not a number"));
		int port;
		std::istringstream(line[i]) >> port;
		std::vector<int> ports = server.getPorts();
		if (std::find(ports.begin(), ports.end(), port) != ports.end())
			throw (ExceptionBuilder("Duplicate port"));
		if (port > MAX_PORT)
			throw (ExceptionBuilder("Port number to high"));
		server.addToPorts(port);
	}
}

void    Parse::parseServerName(Server& server, Line& line)
{
	if (line.size() < 2)
		throw (ExceptionBuilder("server_name directive empty"));

	for (size_t i = 1; i < line.size(); i++)
	{
		std::string server_name = line[i];
		std::vector<std::string> names = server.getNames();
		if (std::find(names.begin(), names.end(), server_name) != names.end())
			throw (ExceptionBuilder("Duplicate server_name"));
			
		server.addToNames(server_name);
	}
}

void    Parse::parseClientBodySize(Server& server, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("client_body_size directive incorrect"));
	else if (!isNumber(line[1]))
		throw (ExceptionBuilder("client_body_size directive not a number"));
	else if(server.getClientBodySize() != -1)
		throw (ExceptionBuilder("Duplicate client_body_size"));

	int clientBodySize;
	std::istringstream(line[1]) >> clientBodySize;
	if (clientBodySize == 0)
		throw (ExceptionBuilder("client_body_size can't be 0"));
	else if (clientBodySize > 100)
		throw (ExceptionBuilder("client_body_size must be in range (1-100)"));
	server.setClientBody(clientBodySize);
}

void	Parse::parseErrorPage(Server& server, Line& line)
{
	if (line.size() != 3)
		throw (ExceptionBuilder("error_page directive incorrect"));
	else if (!isNumber(line[1]))
		throw (ExceptionBuilder("status_code is not a number"));

	int status_code;
	std::istringstream(line[1]) >> status_code;
	std::string page = line[2];

	if (status_code < 400 || status_code > 599)
		throw (ExceptionBuilder("error_page out of range (400-599)"));
	std::map<int, std::string> errorPages = server.getErrorPage();
	if (errorPages.count(status_code) > 0)
		throw (ExceptionBuilder("duplicate error_page in server"));
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
	if (line.size() < 2)
		throw (ExceptionBuilder("location allow_method directive empty"));

	for (size_t i = 1; i < line.size();i++)
	{
		std::vector<std::string> methods = location.getAllowMethods();
		std::string method = line[i];
		if ( method != "GET" && method != "POST" && method != "DELETE")
			throw (ExceptionBuilder("allow_methods variable invalid"));
		if (std::find(methods.begin(), methods.end(), method) != methods.end())
			throw (ExceptionBuilder("Duplicate method"));
		location.addToAllowMethod(line[i]);
	}
}

void	Parse::parseLocationAutoIndex(Location& location, Line& line)
{
	int		autoindex = 0;
	
	if (line.size() != 2)
		throw (ExceptionBuilder("autoindex location directive incorrect"));
	if(location.getAutoIndex() != -1)
		throw (ExceptionBuilder("duplicate autoindex in location"));
	if (line[1] == "on")
		autoindex = 1;
	else if (line[1] == "off")
		autoindex = 0;
	else
		throw (ExceptionBuilder("autoindex value incorrect in location"));
	location.setAutoIndex(autoindex);
}

void	Parse::parseLocationCgi(Location& location, Line& line)
{
	int		cgi = 0;
	
	if (line.size() != 2)
		throw (ExceptionBuilder("cgi location directive incorrect"));
	if(location.getCgi() != -1)
		throw (ExceptionBuilder("duplicate cgi in location"));
	if (line[1] == "on")
		cgi  = 1;
	else if (line[1] == "off")
		cgi = 0;
	else
		throw (ExceptionBuilder("cgi value incorrect in location"));
	location.setCgi(cgi);
}

void	Parse::parseLocationReturn(Location& location, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("return location directive incorrect"));
	else if (location.getReturnPath() != "")
		throw (ExceptionBuilder("duplicate return in location"));
	location.setReturnPath(line[1]);
}

void	Parse::parseLocationDefaultFile(Location &location, Line& line)
{
	if (line.size() != 2)
		throw (ExceptionBuilder("default_file location directive incorrect"));
	else if (location.getDefaultFile() != "")
		throw (ExceptionBuilder("duplicate default_file in location"));
	location.setDefaultFile(line[1]);
}

/* Helper Functions */
std::invalid_argument Parse::ExceptionBuilder(std::string error)
{
	std::string str;
	
	str.append("Parsing Error | Line: " + ToString(g_line_count) + " | ");
	str.append(error);
	return std::invalid_argument(str);
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
	return (directive == "server_name" || directive == "client_body_size" || \
			directive == "listen" || directive == "error_page");
}

bool    Parse::isLocationDirective(std::string& directive)
{
	return (directive == "root" || directive == "index" ||
			directive == "allow_methods" || directive == "autoindex" ||
			directive == "cgi" || directive == "default_file" || directive == "return");
}

std::invalid_argument Parse::ValidateException(std::string error, int block)
{
	std::string str;
	
	str.append("Incomplete Error | ServerBlock " + ToString(block) + " | ");
	str.append(error);
	return std::invalid_argument(str);
}

void	Parse::validate(std::vector<Server>& servers)
{
	std::vector<Server>::iterator 	it = servers.begin();
	int								i = 1;
	
	if (servers.size() == 0)
		throw(ExceptionBuilder("No servers found"));
	for ( ;it != servers.end(); it++)
	{
		validateServer(*it, i);
		i++;
	}
}

void	Parse::validateServer(Server& server, int block)
{
	if (server.getPorts().empty())
		throw(ValidateException("Ports not set", block));
	else if (server.getClientBodySize() == -1)
		server.setClientBody(DEFAULT_CLIENT_BODY_SIZE);

	std::map<std::string, Location>	locations = server.getLocations();
	for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if (it->first == "")
			throw(ValidateException("Location name not set", block));
		validateLocation(it->second, block);
	}
	if (locations.empty() == true)
	{
		locations.insert(std::pair<std::string, Location>("/", Location(true)));
	}
	server.setLocations(locations);
}

void	Parse::validateLocation(Location& location, int block)
{
	if (location.getName() == "")
		throw(ValidateException("Location name not set in", block));
	if (location.getRoot() == "")
		location.setRoot(DEFAULT_ROOT);
	if (location.getDefaultFile() == "")
		location.setDefaultFile(DEFAULT_FILE);
	if (location.getIndex() == "")
		location.setIndex(DEFAULT_INDEX);
	if (location.getAllowMethods().empty())
	{ 
		location.addToAllowMethod("POST");
		location.addToAllowMethod("GET");
		location.addToAllowMethod("DELETE");
	}
	if (location.getAutoIndex() == -1)
		location.setAutoIndex(0);
	if (location.getCgi() == -1)
		location.setCgi(0);
}
