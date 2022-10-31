#include "../inc/Parse.hpp"
#include "../inc/Server.hpp"
#include "../inc/Location.hpp"
#include "../inc/Constants.hpp"
#include <fstream>
#include <sstream>
#include <string.h>
#include <stack>

/* CONSTRUCTORS */
Parse::Parse() { return ; }
Parse::~Parse() { return ; }

/*** 
 * MAIN PARSING FUNCTION
***/
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

std::vector<Line>	extractFile(std::string& file)
{
	File				file;
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
				continue ;
			}
			else
			{
				file.push_back(tokLine);
			}
		}
	}
	return (file);
}

Line 		splitLineWithStrtok(std::string& line)
{
	char	*c_line = strdup(const_cast<char *>(line.c_str()));
	char	*word;
	std::vector<std::string> ret;
	char delimit[]= " \t\r\n\v\f";

	word = strtok(c_line, delimit);
	while (word != NULL)
	{
		ret.push_back(word);
		word = strtok(NULL, delimit);
	}
	if (DEBUG)
	{
		for (size_t i = 0; i < ret.size(); i++)
			std::cout  << "[word " << i << "]: " << ret[i] << '\n';
	}
	free(c_line);
	return ret;
}

std::vector<Server>&	Parse::parseNetwork(std::string& file, std::vector<Server>& servers) //parse whole config, puts it into blocks
{
	File				file = extractFile(file);
	ServerBlock			buff;
	std::stack<char> 	stack;

	for (File::iterator it = file.begin(); it != file.end(); it++)
	{
		Line line = *it;
		if (line[0] == "server" && line.size() == 1)
		{
			buff.clear();
		}
		else if (line[0] == "{" && line.size() == 1)
		{
			stack.push("{")
		}
		else if (line[0] == "}" && line.size() == 1)
		{
			if (!stack.empty())
			{
				throw std::exception("Closing bracket before opening bracket");
			}
			stack.pop();
			if (stack.empty())
			{
				Server server;
				servers.push_back(parseServer(buff, server))
				buff.clear();
			}
		}
		else
		{
			buff.push_back(*line);
		}
	}
	if (stack.empty() == false) //check if the amount of brackets were correct
	{
		throw std::exception("Brackets incorrect");
	}
	return servers;
}

Server&		Parse::parseServer(ServerBlock& server_block, Server& server)
{
	ServerBlock::iterator it = server_block.begin();

	while(it != server_block.end())
	{
		Line line = *it;
		if (isServerDirective(line[0]))
		{
			parseServerDirective(line, server);
		}
		else if (line[0] == "location")
		{
			parseLocationBlock(server, line, server_block);
		}
		else
		{
			throw std::exception("Error in server block" + convertFromVector(line)); // Add global line number var
		}
		it++;
	}
	return server;
}


LocationBlock	extractLocationBlock(ServerBlock::iterator&	it, ServerBlock::iterator& end)//, std::vector<std::string>::iterator&	ite)//std::vector<std::string>& server_block)//, std::vector<std::string>::iterator& separatedWords)
{
	LocationBlock 				ret;
	std::stack<char> 			stack;

	it++;
	while (it != end)
	{
		Line line = *it;
		if (line[0] == "[" && line.size() == 1)
		{
			stack.push('[');
		}
		else if (line[0] == "]" && line.size() == 1)
		{
			if (!stack.empty())
			{
				throw std::exception("Closing bracket before opening bracket");
			}
			stack.pop();
			if (stack.empty())
			{
				return ret;
			}
		}
		else
		{
			ret.push_back(line);
		}
		it++;
	}
	return ret;
}

void		Parse::parseLocationBlock(Server& server, ServerBlock::iterator& line, ServerBlock& server_block)
{
	if (line.size() != 2)
	{
		throw std::exception("Incorrect location name" + convertFromVector(line));
	}

	Location		location;
	LocationBlock 	location_block = extractLocationBlock(line, server_block.end());

	server.addToLocations("TODO ADD LOCATION NAME", parseLocation(location_block, location));
}


Location&	Parse::parseLocation(LocationBlock& location_block, Location& location)
{
	LocationBlock::iterator it = location_block.begin();
	
	while(it != location_block.end())
	{
		Line line = *it;
		if (isLocationDirective(line[0]))
		{
			parseLocationDirective(line, location);
		}
		else 
		{
			throw std::exception("Error in LocationBlock" + convertFromVector(line));
		}
		it++;
	}
	return location;
}

/* Should be made better and cleare and moved to utils*/
void	printLine(Line line)
{
	for (vecIt it = line.begin(); it != line.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
}

/* GONNA BE OBSOLETE WITH NEW IMPLEMENTATION*/
std::string convertFromVectorServerBlock(std::vector<std::string>& line)
{
	std::string ret = "";

	ret = ret + line[1];//line[1]; //this is the second
	for (size_t i = 2; i < line.size(); i++) //go up until there is no other locationdirective found
	{
		ret = ret + " " + line[i];
 	}
	return (ret);
}
/* GONNA BE OBSOLETE WITH NEW IMPLEMENTATION*/
std::string convertFromVector(std::vector<std::string>& line) //just add an extra function argument with counter of line[0] or 1 for locationblock, no need for two separate functions
{
	std::string ret = "";

	ret = ret + line[0];//line[1]; //this is the second
	for (size_t i = 1; i < line.size(); i++) //go up until there is no other locationdirective found
	{
		ret = ret + " " + line[i];
 	}
	return (ret);
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();

    while (it != s.end() && std::isdigit(*it))
	{
		it++;
	}
    return (!s.empty() && it == s.end());
}

void    Parse::parseServerDirective(Line& line, Server& server)
{

	// TODORewrite so it uses a line
	std::string restOfLine;

	Line	line; // new Line to be used
	
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
			restOfLine = convertFromVectorServerBlock(line);
			ServerDirectiveFunction = dTable[i].ServerDirectiveFunction;
			(this->*ServerDirectiveFunction)(server, line);
		}
	}
}

void    Parse::parseLocationDirective(Line& line, Location& location)
{
	void	(Parse::*LocationDirectiveFunction)(Location&, Line&) = NULL;

	const t_dTableLocation dTable[] = //map instead, name of the table typedef is not descriptive enough
	{
			{"root", &Parse::parseLocationRoot},
			{"index", &Parse::parseLocationIndex},
			{"allow_methods", &Parse::parseLocationAllowMethod},
			{"autoindex", &Parse::parseLocationAutoIndex},
			{"cgi_name", &Parse::parseLocationCgiName},
			{"cgi_file_extension", &Parse::parseLocationCgiExt}
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

bool    Parse::isServerDirective(std::string& currentWord)
{
	//have to replace this in a more appropriate spot, 	//what happens if the word is empty? wouldnt == overload return true?
	currentWord.erase(remove(currentWord.begin(), currentWord.end(), ';'), currentWord.end());
	return (currentWord == "server_name" || currentWord == "root" || \
			currentWord == "index" || currentWord == "client_body_size" || \
			currentWord == "listen" || currentWord == "error_page" || currentWord == "allow_methods");
}

bool    Parse::isLocationDirective(std::string& currentWord)
{
	currentWord.erase(remove(currentWord.begin(), currentWord.end(), ';'), currentWord.end());
	return (currentWord == "root" || currentWord == "index" ||
			currentWord == "allow_methods" || currentWord == "autoindex" ||
			currentWord == "cgi_name" || currentWord == "cgi_file_extension");
}


/* SEPARATE PARSERS FOR THE RESPECTIVE DIRECTIVES */

void    Parse::parseRoot(Server& server, Line& line)
{
	// TODO Add error checks

	server.setRoot(line[1]);
}

void    Parse::parseListen(Server& server, Line& line)
{
	// TODO Add error check for numbers isdigit

	/*--OLD--*/
	// std::size_t firstFoundDigit = currentLine.find_first_of("0123456789");
	// if (firstFoundDigit != std::string::npos)
	// {
	// 	std::string portNumber = currentLine.substr(firstFoundDigit);
	// 	portNumber.erase(remove(portNumber.begin(), portNumber.end(), ';'), portNumber.end());
	// 	int portNrInt = std::stoi(portNumber, nullptr, 10);
	// 	server.setPort(portNrInt); //this could be a vector of ports since there are more ports which could b involved
	// 	server.setListenFlag();
	// }
}

void    Parse::parseIndex(Server& server, Line& line)
{
	// TODO Add error checks

	server.setIndex(line[1]);
}

void    Parse::parseServerName(Server& server, Line& line)
{
	// TODO Add error check for server names and loop parser

	/*--OLD--*/
	// char *remainingLine = const_cast<char *>(currentLine.c_str());
	// char *spaceSeparatedWord = strtok (remainingLine, " ");

	// while (spaceSeparatedWord != NULL)
	// {
	// 	std::string serverNameToAdd(spaceSeparatedWord);
	// 	serverNameToAdd.erase(remove(serverNameToAdd.begin(), serverNameToAdd.end(), ';'), serverNameToAdd.end());
	// 	server.addToName(serverNameToAdd);
	// 	server.setServerNameFlag();
	// 	spaceSeparatedWord = strtok (NULL, " ");
	// }
}

void    Parse::parseClientBodySize(Server& server, Line& line)
{
	// TODO Add error checks for numbers

	// std::size_t firstFoundDigit = currentLine.find_first_of("0123456789");
	// if (firstFoundDigit != std::string::npos)
	// {
	// 	std::string bodySize = currentLine.substr(firstFoundDigit);
	// 	bodySize.erase(remove(bodySize.begin(), bodySize.end(), ';'), bodySize.end());
	// 	int bodySizeInt = std::stoi(bodySize, nullptr, 10);
	// 	server.setClientBody(bodySizeInt);
	// }
}


void	Parse::parseAllowedMethods(Server& server, Line& line)
{
	// TODO Add error checks for words

	/*--OLD--*/
	// char delimit[]= " \t\r\n\v\f";
	// char *remainingLine = const_cast<char *>(currentLine.c_str());
	// char *spaceSeparatedWord = strtok (remainingLine, delimit);

	// while (spaceSeparatedWord != NULL)
	// {
	// 	std::string methodToAdd(spaceSeparatedWord);
	// 	methodToAdd.erase(remove(methodToAdd.begin(), methodToAdd.end(), ';'), methodToAdd.end());
	// 	server.addToMethod(methodToAdd);
	// 	spaceSeparatedWord = strtok (NULL, delimit);
	// }
}


// NEED TO BE REWRITTEN
void	Parse::parseErrorPage(Server& server, Line& line)
{
	// TODO Add error pages directive parser

	/*--OLD--*/
	// char delimit[]= " \t\r\n\v\f";
	// char *remainingLine = const_cast<char *>(currentLine.c_str());
	// char *spaceSeparatedWord = strtok (remainingLine, delimit);
	// std::vector<int> error_codes;

	// while (spaceSeparatedWord != NULL)
	// {
	// 	std::string errorToAdd(spaceSeparatedWord);
	// 	errorToAdd.erase(remove(errorToAdd.begin(), errorToAdd.end(), ';'), errorToAdd.end());

	// 	if (is_number(errorToAdd) == true)
	// 	{
	// 		int currentErrorCodeToAdd = std::stoi(errorToAdd);
	// 		error_codes.push_back(currentErrorCodeToAdd);
	// 	}
	// 	else //what if there are more names? eg:beginning before codes, and after?
	// 	{
	// 		server.addToErrorPages(error_codes)
	// 	}
	// 	spaceSeparatedWord = strtok (NULL, delimit);
	// }
}


void	Parse::parseLocationRoot(Location& location, Line& line)
{

}

void	Parse::parseLocationIndex(Location& location, Line& line)
{
	
}

void	Parse::parseLocationAllowMethod(Location& location, Line& line)
{
	
}

void	Parse::parseLocationAutoIndex(Location& location, Line& line)
{
	
}

void	Parse::parseLocationCgiName(Location& location, Line& line)
{
	
}

void	Parse::parseLocationCgiExt(Location& location, Line& line)
{
	
}