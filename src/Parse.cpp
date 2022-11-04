#include "../inc/Parse.hpp"
#include "../inc/Server.hpp"
#include "../inc/Location.hpp"
#include "../inc/Constants.hpp"
#include <fstream>
#include <sstream>
#include <string.h>
#include <stack>

/* CONSTRUCTORS */

/***
 * Not sure if we should precreate a dispatch table upon constructon or not,
 * for now we create it inside the selectParseFunction function, later maybe could be even static
 ***/
Parse::Parse()
{
	return ;
}


Parse::Parse(const Parse& src)
{
	*this = src;
	return;
}

Parse& Parse::operator=(const Parse& rhs)
{
	if (this != &rhs)
	{
	}
	return *this;
}

Parse::~Parse()
{
	return ;
}


/* PARSER FUNCTIONS */

/*** 
 * MAIN PARSING FUNCTION
***/
void Parse::openFile(std::ifstream& configStream, std::string configName)
{
	configStream.open(configName);
	if (!configStream.is_open())
	{
		std::cout << "Failed to open config file." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::cout << "Config file opened." << std::endl;
}

std::vector<std::string> splitLineWithStrtok(std::string& line)
{
	char						*word;
	std::vector<std::string>	ret;
	char						delimit[]= " \t\r\n\v\f";
	char						*c_line = strdup(line.c_str());

	if (!c_line)
	{
		std::perror("malloc: ");
		std::exit(EXIT_FAILURE);
	}
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


Location&	Parse::parseLocation(std::vector<std::string>& location_block, Location& location)
{
	std::vector<std::string>::iterator currentWordInLocationBlock = location_block.begin();
	std::vector<std::string>::iterator endOfLocationBlock = location_block.end();

	std::stack<char>				  stack;
	
	while(currentWordInLocationBlock != endOfLocationBlock) //currentWordInLocationBlock here is just separate words in the location blocks
	{
		if (isLocationDirective(*currentWordInLocationBlock))
		{
			parseLocationDirective(currentWordInLocationBlock, endOfLocationBlock, location);
		}
		else if (*currentWordInLocationBlock == "[")
		{
			stack.push('[');
		}
		else if (*currentWordInLocationBlock == "]") //what happens if its in the same line?
		{
			stack.pop();
			if (stack.empty() == true)
			{
				return location;
			}
			else
			{
				std::cout << "Input is incorrect inside block of location, cuz not empty: " << *currentWordInLocationBlock << std::endl;
				std::exit(ERROR);
			}
		}
		/*
		else if (*it != "location")
		{
			std::cout << "Input is incorrect inside block of location: " << *it << std::endl;
			std::exit(ERROR);
		}
		*/
		currentWordInLocationBlock++;
	}
	return location;
}

std::vector<std::string>	extractLocationBlock(std::vector<std::vector<std::string> >::iterator&	it, std::vector<std::vector<std::string> >::iterator&	ite)//, std::vector<std::string>::iterator&	ite)//std::vector<std::string>& server_block)//, std::vector<std::string>::iterator& separatedWords)
{
	std::vector<std::string> 	ret;
	std::stack<char> 			stack;

	while (it != ite)
	{	
		std::vector<std::string>::iterator	smallIt = it->begin();
		std::vector<std::string>::iterator	smallIte = it->end();

		for (vecIt itLine = smallIt; itLine != smallIte; itLine++)
		{
			if (*itLine == "[")
			{
				stack.push('[');
			}
			else if (*itLine == "]")
			{
				stack.pop();
				if (stack.empty())
				{
					return ret;
				}
				else
				{
					//error
				}
			}
			ret.push_back(*itLine);
		}
		it++;
	}
	return ret;
}

void		Parse::parseLocationDirectiveBlock(Server& server, std::vector<std::vector<std::string> >::iterator& currentPosInLines, std::vector<std::vector<std::string> >::iterator& endPosOfLines, std::string& locationName)
{
	Location		location;
	size_t			sizeOfLocationLine = currentPosInLines->size();

	if (sizeOfLocationLine > 2)
	{
		std::cout << "Input is incorrect, expected maximum one name for location block inside block."  << std::endl;
		std::exit(ERROR);
	}
	std::string nameLocation = (locationName == "" ? "/" : locationName);
	std::vector<std::string> location_block = extractLocationBlock(currentPosInLines, endPosOfLines);
	server.addToLocations(nameLocation, parseLocation(location_block, location));
	return ;
}

Server&		Parse::parseServer(std::vector<std::vector<std::string> >& server_block, Server& server)
{
	std::stack<char> 									stack;
	std::vector<std::vector<std::string> >::iterator	currentPosInLines = server_block.begin();
	std::vector<std::vector<std::string> >::iterator	endPosOfLines = server_block.end();

	while(currentPosInLines != endPosOfLines)
	{
		bool					isLineEmpty = currentPosInLines->front().empty();

		for (vecIt currentWord = currentPosInLines->begin(); currentWord != currentPosInLines->end(); currentWord++)
		{
			if (isLineEmpty == false && isServerDirective(*currentWord))
			{
				parseServerDirective(*currentPosInLines, server);
				break ;
			}
			else if (isLineEmpty == false && *currentWord == "location")
			{
				currentWord++; //the next word after location in the line
				parseLocationDirectiveBlock(server, currentPosInLines, endPosOfLines, *currentWord);
				break ;
			}
			else
			{
				if (isLineEmpty == false && *currentWord != "{" && *currentWord != "}")
				{
					std::cout << "Input is incorrect inside block of: " << *currentWord << std::endl;
					std::exit(ERROR);
				}
			}
		}
		currentPosInLines++;
	}
	if (server.minimumRequiredAttributesProvided() == false) //minimum is server_name, listen(port)
	{
		std::cout << "Minimum required input is not or incorrectly provided." << std::endl;
		std::exit(ERROR);
	}
	return server;
}


/* 	Function to check a tokenized line word by word, if we find the correct amount of opening/closing curly brackets, 
***	we proceed to parse the server block.
*/
void Parse::checkWordByWord(std::vector<Server>& servers, std::vector<std::string>& line, std::vector<std::vector<std::string> >& buff, std::stack<char>& stack)
{
	for (vecIt currentWord = line.begin(); currentWord != line.end(); currentWord++)
	{
		if (*currentWord == "{")
		{
			stack.push('{');
		}
		else if (*currentWord == "}")
		{
			stack.pop();
			if (stack.empty())
			{
				Server server;
				servers.push_back(parseServer(buff, server));
				buff.clear();
			}
		}
		else
		{
			//error?
		}
	}
	return ;
}


/***
 * Parse the configFile into individual Server Blocks // Server Block type vector<std::string>
 * 
 * @return value right now is a vector of Server blocks. In the future should just return a std::vector<Server>
 * instantiated by calling Server(std::vector<std::string>)
***/
std::vector<Server>&	Parse::parseNetwork(std::string& file, std::vector<Server>& servers) //parse whole config, puts it into blocks
{	
	std::vector<std::vector<std::string> >	buff;
	std::string								currentLine;
	std::ifstream							configStream;
	std::stack<char> 						stack;

	openFile(configStream, file);
	while(!configStream.eof())
	{
		getline(configStream, currentLine);
		if (configStream.good())
		{
			std::vector<std::string>	tokenizedLine = splitLineWithStrtok(currentLine);
			bool						isLineEmpty = tokenizedLine.empty();

			if (isLineEmpty == false && tokenizedLine[0] == "server" && tokenizedLine.size() != 1) //server server server
			{
				std::cout << "Incorrect input, exciting program." << std::endl;
				std::exit(ERROR);
			}

			else if (isLineEmpty == false && tokenizedLine[0] != "server") //this check is too costly every time, use flag once server was found?
			{
				buff.push_back(tokenizedLine);
			}
			checkWordByWord(servers, tokenizedLine, buff, stack);
		}
	}
	if (stack.empty() == false) //check if the amount of brackets were correct
	{
		std::cout << "Incorrect input, exciting program." << std::endl;
		std::exit(ERROR);
	}

	return(servers);
}

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



/*** 
 * Based on the currentWord, an associated parsing function will be called
 * The parsing function will start to look for input after the currentWord in the currentLine
 * This is skipping location for now entirely, have to alter struct a bit
***/
void    Parse::parseServerDirective(std::vector<std::string>& line, Server& server)
{
	std::string restOfLine;
	//std::vector<std::string> line = splitLineWithStrtok(currentLine);
	
	void	(Parse::*pointerToParseServerDirectives)(Server&, std::string&) = NULL;
	const t_selectServer myDispatch[] = //map instead, name of the table typedef is not descriptive enough
	{
			{"listen", &Parse::parseListen},
			{"server_name", &Parse::parseServerName},
			{"root", &Parse::parseRoot},
			{"index", &Parse::parseIndex},
			{"client_body_size", &Parse::parseClientBodySize},
			{"allow_methods", &Parse::parseAllowedMethods},
			{"error_page", &Parse::parseErrorPage},
	};
	//later this could be a separate function
	for (int i = 0; i < NR_OF_SERVER_DIRECTIVES; i++)
	{
		if (line[0] == myDispatch[i]._name)
		{
			restOfLine = convertFromVectorServerBlock(line);
			pointerToParseServerDirectives = myDispatch[i].pointerToParseServerDirectives;
			break ;
		}
	}
	if (pointerToParseServerDirectives)
	{
		(this->*pointerToParseServerDirectives)(server, restOfLine);
	}
}

void    Parse::parseLocationDirective(std::vector<std::string>::iterator& it, std::vector<std::string>::iterator& ite, Location& locationInstance)
{
	std::string restOfLine;

	void	(Location::*pointerToLocation)(std::string&) = NULL;
	const t_selectLocation myDispatch[] = //map instead, name of the table typedef is not descriptive enough
	{
			{"root", &Location::setLocationRoot},
			{"index", &Location::setLocationIndex},
			{"allow_methods", &Location::setLocationAllowMethod},
			{"autoindex", &Location::setLocationAutoindex},
			{"cgi_name", &Location::setCgiName},
			{"cgi_file_extension", &Location::setCgiFileExtension}
	};
	//later this could be a separate function
	for (int i = 0; i < NR_OF_LOCATION_DIRECTIVES; i++)
	{
		if (*it == myDispatch[i]._name)
		{
			it++;
			std::vector<std::string> restOf;
			for (vecIt currentWord = it; currentWord < ite; currentWord++)
			{
				if (isLocationDirective(*currentWord))
				{
					break ;
				}
				restOf.push_back(*currentWord);
			}
			restOfLine = convertFromVector(restOf);
			pointerToLocation = myDispatch[i].pointerToLocation;
			break ;
		}
	}
	if (pointerToLocation)
	{
		(&locationInstance->*pointerToLocation)(restOfLine);
	}
	if (locationInstance.getLocationCgiName() != "" && locationInstance.getLocationCgiFileExtension() != "")
	{
		locationInstance.setCgiCompleteBool(true);
	}
}

/*** 
 * Checks if a word is a directive so we can select an associated function later
***/
bool    Parse::isServerDirective(std::string& currentWord)
{
	//have to replace this in a more appropriate spot, 	//what happens if the word is empty? wouldnt == overload return true?
	currentWord.erase(remove(currentWord.begin(), currentWord.end(), ';'), currentWord.end());
	return (currentWord == "server_name" || currentWord == "root" || \
			currentWord == "index" || currentWord == "client_body_size" || \
			currentWord == "listen" || currentWord == "error_page" || currentWord == "allow_methods");
}

/*** 
 * Checks if a word is a directive of location so we can select an associated function later
***/
bool    Parse::isLocationDirective(std::string& currentWord)
{
	currentWord.erase(remove(currentWord.begin(), currentWord.end(), ';'), currentWord.end());
	return (currentWord == "root" || currentWord == "index" ||
			currentWord == "allow_methods" || currentWord == "autoindex" ||
			currentWord == "cgi_name" || currentWord == "cgi_file_extension");
}

/* SEPARATE PARSERS FOR THE RESPECTIVE DIRECTIVES */
void    Parse::parseRoot(Server& server, std::string& currentLine)
{
		currentLine.erase(remove(currentLine.begin(), currentLine.end(), ';'), currentLine.end());
		server.setRoot(currentLine);
		return ;
}

void    Parse::parseListen(Server& server, std::string& currentLine)
{
	std::size_t firstFoundDigit = currentLine.find_first_of("0123456789");
	if (firstFoundDigit != std::string::npos)
	{
		std::string portNumber = currentLine.substr(firstFoundDigit);
		portNumber.erase(remove(portNumber.begin(), portNumber.end(), ';'), portNumber.end());
		int portNrInt = std::stoi(portNumber, nullptr, 10);
		server.setPort(portNrInt); //this could be a vector of ports since there are more ports which could b involved
		server.setListenFlag();
	}
	return ;
}

void    Parse::parseIndex(Server& server, std::string& currentLine)
{
	currentLine.erase(remove(currentLine.begin(), currentLine.end(), ';'), currentLine.end()); //index.html
	server.setIndex(currentLine);
	return ;
}

void    Parse::parseServerName(Server& server, std::string& currentLine)
{
	char *remainingLine = const_cast<char *>(currentLine.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, " ");

	while (spaceSeparatedWord != NULL)
	{
		std::string serverNameToAdd(spaceSeparatedWord);
		serverNameToAdd.erase(remove(serverNameToAdd.begin(), serverNameToAdd.end(), ';'), serverNameToAdd.end());
		server.addToName(serverNameToAdd);
		server.setServerNameFlag();
		spaceSeparatedWord = strtok (NULL, " ");
	}
	return ;
}

void    Parse::parseClientBodySize(Server& server, std::string& currentLine)
{
	std::size_t firstFoundDigit = currentLine.find_first_of("0123456789");
	if (firstFoundDigit != std::string::npos)
	{
		std::string bodySize = currentLine.substr(firstFoundDigit);
		bodySize.erase(remove(bodySize.begin(), bodySize.end(), ';'), bodySize.end());
		int bodySizeInt = std::stoi(bodySize, nullptr, 10);
		server.setClientBody(bodySizeInt);
	}
	return ;
}

void	Parse::parseAllowedMethods(Server& server, std::string& currentLine)
{
	char delimit[]= " \t\r\n\v\f";
	char *remainingLine = const_cast<char *>(currentLine.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, delimit);

	while (spaceSeparatedWord != NULL)
	{
		std::string methodToAdd(spaceSeparatedWord);
		methodToAdd.erase(remove(methodToAdd.begin(), methodToAdd.end(), ';'), methodToAdd.end());
		server.getMethodsReference().push_back(methodToAdd);
		spaceSeparatedWord = strtok (NULL, delimit);
	}
	return ;
}

void	Parse::parseErrorPage(Server& server, std::string& currentLine)
{
	char delimit[]= " \t\r\n\v\f";
	char *remainingLine = const_cast<char *>(currentLine.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, delimit);
	std::vector<int> error_codes;

	while (spaceSeparatedWord != NULL)
	{
		std::string errorToAdd(spaceSeparatedWord);
		errorToAdd.erase(remove(errorToAdd.begin(), errorToAdd.end(), ';'), errorToAdd.end());

		if (is_number(errorToAdd) == true)
		{
			int currentErrorCodeToAdd = std::stoi(errorToAdd);
			error_codes.push_back(currentErrorCodeToAdd);
		}
		else //what if there are more names? eg:beginning before codes, and after?
		{
			server.getErrorPageRef().insert(std::pair<std::vector<int>, std::string>(error_codes, errorToAdd));
		}
		spaceSeparatedWord = strtok (NULL, delimit);
	}
	return ;
}
