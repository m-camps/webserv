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

std::vector<std::string> splitLineWithStrtok(std::string& line)
{
	char	*c_line = strdup(const_cast<char *>(line.c_str()));
	char	*word;
	std::vector<std::string> ret;

	while ((word = strtok(c_line, " \t")))
	{
		ret.push_back(word);
		c_line = NULL;
	}
	if (DEBUG)
	{
		for (size_t i = 0; i < ret.size(); i++)
			std::cout  << "[word " << i << "]: " << ret[i] << '\n';
	}
	free(c_line);
	return ret;
}


/***
 * loops over tokenized lines.
 * The function is using balanced parantheses datastruct to identify
 * if the block is correctly 
 * 
***/
/* will work later, atm some scope issues still
void    Parse::searchForServerBlock(std::vector<std::string> tokenizedLine, std::vector<std::string>& buff ,std::vector<Server>& servers) //splitLineWithStrtok
{
    std::stack<char> stack;

    for (vecIt it = tokenizedLine.begin(); it != tokenizedLine.end(); it++)
    {
        if ((*it).compare("{") == 0)
        {
            stack.push('{');
        }
        if ((*it).compare("}") == 0)
        {
            stack.pop();
            if (stack.empty())
            {
                Server server;
                servers.push_back(parseServer(buff, server));
                buff.clear();
            }
        }
    }
    return ;
}
*/


/***
 * Parse the configFile into individual Server Blocks // Server Block type vector<std::string>
 * 
 * @return value right now is a vector of Server blocks. In the future should just return a std::vector<Server>
 * instantiated by calling Server(std::vector<std::string>)
 * in the loop if we find { - >push to stack, run loop until we find }, if we found,pop it and if the stack is empty, the input was correct, proceed to parse the rest
***/
std::vector<Server>&	Parse::parseNetwork(std::string& file, std::vector<Server>& servers) //parse whole config, puts it into blocks
{
	std::vector<std::string>	buff;
	std::string					currentLine;
	std::ifstream				configStream;
	std::stack<char> 			stack;

	openFile(configStream, file);
	while(!configStream.eof())
	{
		getline(configStream, currentLine);
		if (configStream.good())
		{
			std::vector<std::string> line = splitLineWithStrtok(currentLine);
			if (!line.empty() && line[0].compare("server") != 0)
			{
				buff.push_back(currentLine);
			}
			for (vecIt it = line.begin(); it != line.end(); it++)
			{
				if ((*it).compare("{") == 0)
				{
					stack.push('{');
				}
				if ((*it).compare("}") == 0)
				{
					stack.pop();
					if (stack.empty())
					{
						Server server;
						servers.push_back(parseServer(buff, server));
						std::cout << server.getLocations().begin()->second << std::endl; //it also sees it here
						//std::cout << server.getLocations().end()->second << std::endl; //this doesnt work, does the second overwrite the first location block added?
						buff.clear();
					}
				}
			}
		}
	}

	return(servers);
}

std::vector<std::string>	extractLocation(std::vector<std::string>::iterator& it, std::vector<std::string>& server_block)
{
	std::vector<std::string> ret;
	std::stack<char> stack;
	stack.push('{');
	while (it != server_block.end())
	{
		ret.push_back(*it);
		std::string strippedFromTab = *it;
		strippedFromTab.erase(remove(strippedFromTab.begin(), strippedFromTab.end(), '\t'), strippedFromTab.end());
		if ((strippedFromTab).compare("}") == 0)
		{
			stack.pop();
			if (stack.empty() == true)
			{
				break ; 
			}
		}
		it++;
	}
	return ret;
}

Server&		Parse::parseServer(std::vector<std::string>& server_block, Server& server)
{
	std::vector<std::string>			buff;
	std::vector<Location>				locations;
	std::vector<std::string>::iterator	it = server_block.begin();
	std::stack<char> stack;

	while(it != server_block.end())
	{
		std::vector<std::string> line = splitLineWithStrtok(*it);
		if (!line.empty() && isServerDirective(line[0]))
		{
			parseDirective(*it, server);
		}
		else if(line[0] == "location")
		{
			Location	location;
			std::string nameLocation;
			if (line.size() != 1)
			{
				nameLocation = line[1];
			}
			else
			{
				nameLocation = line[0];
			}
			std::vector<std::string> location_block = extractLocation(it, server_block);
			server.addToLocations(nameLocation, parseLocation(location_block, location));
			//std::map<std::string, Location>::iterator it = server.getLocations().end(); //this will be deleted here
			//server.getLocations().find('/python'); //prob we have to use find, but apart from the syntax which I cant find atm, this worked. Find based on name, will output locationblock results
		}
		else
		{
			// throw(error); and return currentline;
		}
		it++;
	}
	return server;
}

Location&	Parse::parseLocation(std::vector<std::string>& location_block, Location& location)
{
	std::vector<std::string>::iterator it = location_block.begin();
	while(it != location_block.end())
	{
		std::vector<std::string> line = splitLineWithStrtok(*it);
		if (!line.empty() && isLocationDirective(line[0]))
		{
			parseLocationDirective(*it, location);
		}
		else if (line[0] == "}")
		{
			return location;
		}
		else
		{
			//error print line where parsing error happend
		}
		it++;
	}
	return location;
}

std::string convertFromVector(std::vector<std::string> line)
{
	std::string ret = "";

	ret = ret + line[1];
	for (size_t i = 2; i < line.size(); i++)
	{
		ret = ret + " " + line[i];
 	}
	return (ret);
}

/*** 
 * Based on the currentWord, an associated parsing function will be called
 * The parsing function will start to look for input after the currentWord in the currentLine
 * This is skipping location for now entirely, have to alter struct a bit
***/
void    Parse::parseDirective(std::string& currentLine, Server& server)
{
	std::string restOfLine;
	std::vector<std::string> line = splitLineWithStrtok(currentLine);
	
	void	(Parse::*pointerToParseServerDirectives)(Server&, std::string&) = NULL;
	const t_selectServer myDispatch[] = //map instead, name of the table typedef is not descriptive enough
	{
			{"listen", &Parse::parseListen},
			{"server_name", &Parse::parseServerName},
			{"root", &Parse::parseRoot},
			{"index", &Parse::parseIndex},
			{"client_body_size", &Parse::parseClientBodySize},
			{"allow_methods", &Parse::parseAllowedMethods},
	};

	for (int i = 0; i < NR_OF_DIRECTIVES_TO_LOOK_FOR + 1; i++)
	{
		if (line[0] == myDispatch[i]._name)
		{
			restOfLine = convertFromVector(line);
			pointerToParseServerDirectives = myDispatch[i].pointerToParseServerDirectives;
			break ;
		}
	}
	if (pointerToParseServerDirectives)
	{
		(this->*pointerToParseServerDirectives)(server, restOfLine);
	}
}


void    Parse::parseLocationDirective(std::string& currentLine, Location& locationInstance)
{
	std::string restOfLine;
	std::vector<std::string> line = splitLineWithStrtok(currentLine);
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

	for (int i = 0; i < 6; i++)
	{
		if (line[0] == myDispatch[i]._name)
		{
			restOfLine = convertFromVector(line);
			pointerToLocation = myDispatch[i].pointerToLocation;
			break ;
		}
	}
	if (pointerToLocation)
	{
		(&locationInstance->*pointerToLocation)(restOfLine);
	}
<<<<<<< Updated upstream
=======
<<<<<<< Updated upstream

	// void	(Location::*pointerToLocation)(std::string&) = NULL;

	// const t_selectLocation myDispatch[] = 	//map instead, name of the table typedef is not descriptive enough
	// {
	// 		{"root", &Location::setLocationRoot},
	// 		{"index", &Location::setLocationIndex},
	// 		{"allow_methods", &Location::setLocationAllowMethod},
	// 		{"autoindex", &Location::setLocationAutoindex},
	// 		{"cgi_name", &Location::setCgiName},
	// 		{"cgi_file_extension", &Location::setCgiFileExtension}
	// };

	// //not sure if this
	// char *remainingLine = const_cast<char *>(currentLine.c_str());
	// char *spaceSeparatedWord = strtok (remainingLine, " ");
	// while (spaceSeparatedWord != NULL)
	// {
	// 	std::string currentWord(spaceSeparatedWord);
	// 	for (int i = 0; i < NR_OF_DIRECTIVES_TO_LOOK_FOR + 1; i++) //have to alter macro
	// 	{
	// 		if (currentWord == myDispatch[i]._name)
	// 		{
	// 			size_t      posInLine = currentWord.length();
	// 			std::string restOfLine = currentLine.substr(posInLine + 2);
	// 			pointerToLocation = myDispatch[i].pointerToLocation;
	// 			(&locationInstance->*pointerToLocation)(restOfLine);
	// 		}
	// 	}
	// 	spaceSeparatedWord = strtok (NULL, " ");
	// }
=======
>>>>>>> Stashed changes
>>>>>>> Stashed changes
}

/*** 
 * Checks if a word is a directive so we can select an associated function later
***/
bool    Parse::isServerDirective(std::string& currentWord)
{
	//what happens if the word is empty? wouldnt == overload return true? 
	return (currentWord == "server_name" || currentWord == "root" || \
			currentWord == "index" || currentWord == "client_body_size" || \
			currentWord == "listen" || currentWord == "error_page" || currentWord == "allow_methods");
}

/*** 
 * Checks if a word is a directive of location so we can select an associated function later
***/
bool    Parse::isLocationDirective(std::string& currentWord)
{
	//what happens if the word is empty? wouldnt == overload return true?
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

void    Parse::parseListen(Server& server, std::string& currentLine) //listen
{
	std::size_t firstFoundDigit = currentLine.find_first_of("0123456789");
	if (firstFoundDigit != std::string::npos)
	{
		std::string portNumber = currentLine.substr(firstFoundDigit);
		portNumber.erase(remove(portNumber.begin(), portNumber.end(), ';'), portNumber.end());
		int portNrInt = std::stoi(portNumber, nullptr, 10);
		server.setPort(portNrInt);
	}
	return ;
}

void    Parse::parseIndex(Server& server, std::string& currentLine)
{
	(void)server;
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
	char *remainingLine = const_cast<char *>(currentLine.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, "\t");
	while (spaceSeparatedWord != NULL)
	{
		std::string methodToAdd(spaceSeparatedWord);
		methodToAdd.erase(remove(methodToAdd.begin(), methodToAdd.end(), ';'), methodToAdd.end());
		server.getMethodsReference().push_back(methodToAdd);
		//it = myvector.insert(it, methodToAdd); //does this work now?
		spaceSeparatedWord = strtok (NULL, "\t");
	}
	return ;
}


/*
void    parseAllowMethods(Server& server, std::string& currentLine)
{
	//
	(void)server;
	//separate them by whitespace and strok
	char *remainingLine = const_cast<char *>(currentLine.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, " ");

	while (spaceSeparatedWord != NULL)
	{
		std::string methodToAdd(spaceSeparatedWord);
		//trail the last; -> what happens if there are more ;;;;, other error checks are needed
		methodToAdd.erase(remove(methodToAdd.begin(), methodToAdd.end(), ';'), methodToAdd.end());


		// server.setMethods(methodToAdd);
		spaceSeparatedWord = strtok (NULL, " ");
	}
}
*/
/*
void    parseAutoIndex(Server& server, std::string& currentLine)
{
	//strtok, on or off?
	//erase whitespace, on or off after?
	currentLine.erase(std::remove_if(currentLine.begin(), currentLine.end(), ::isspace), currentLine.end());
	if (currentLine == "on")
	{
		server.getLocations().back()._autoIndex = true;
	}
	else if (currentLine == "off")
	{
		server.getLocations().back()._autoIndex = false;   
	}
	//std::cout << server.getLocations().back()._autoIndex << " is cginame now" << std::endl;

	//what happens if the input is incomplete? or there are multiple ;;; in the end? , there has to be a flag or so to mark the inproper input?
	return ;
}

void	parseCgiName(Server& server, std::string& currentLine)
{
	currentLine.erase(std::remove_if(currentLine.begin(), currentLine.end(), ::isspace), currentLine.end());
	currentLine.erase(remove(currentLine.begin(), currentLine.end(), ';'), currentLine.end());
	server.getLocations().back()._cgi_name = currentLine;
	//std::cout << server.getLocations().back()._cgi_name << " is cginame now" << std::endl;
	return ;

}

void	parseCgiFileExtension(Server& server, std::string& currentLine)
{
	currentLine.erase(std::remove_if(currentLine.begin(), currentLine.end(), ::isspace), currentLine.end());
	currentLine.erase(remove(currentLine.begin(), currentLine.end(), ';'), currentLine.end());
	server.getLocations().back()._cgi_file_extension = currentLine;
	//std::cout << server.getLocations().back()._cgi_file_extension << " is cgi_file_extension now" << std::endl;
	return ;
}
*/




// THIS IS NOW A NEW FUNCTION KEEPING HERE FOR REFERENCE
/*** 
 * Loops through all the lines in the given file
 * If a word in a line is a directive, proceeds to select the function to be used for that directive
***/

// void    Parse::parseServer(std::string serverBlock, Server& server)
// {
// 	std::string         currentLine;
// 	std::string         currentWord;
// 	std::istringstream  iss;

// 	while (!configStream.eof())
// 	{
// 		getline(configStream, currentLine); //double check line endings, shouldn't it read until ; ? what happens if there is a newline before ; ? etc
// 		if (configStream.good())
// 		{
// 			iss.clear();
// 			iss.str(currentLine);
// 			while (iss.good())
// 			{
// 				iss >> currentWord;
// 				//could be solved only to look for first words of the line, but that assumes correct input 
// 				//if we fond server, create Server Server;
		
// 				if (isServerDirective(currentWord) == true) //find any keyword for server currentWord == server
// 				{
// 					selectParseFunction(currentLine, currentWord, parseInstance, server);
// 				}
// 				else if (currentWord == "location") //if its location
// 				{
					
// 					//create a location instance here?
// 					//you need the name + instance to push to the server
// 					//you create an instance of class Location, proceed with loactnopareser
// 					proceedToLocationParser(server, configStream, parseInstance);
// 					currentWord = "";
// 					break ;
// 				}
// 			}
// 		}
// 	}
// }

//this is b for location
// void    proceedToLocationParser(Server& server, std::ifstream& configStream, Parse& parseInstance)
// {	
// 	//we might have go to the next line, stirp {}
// 	std::string         currentLine;
// 	std::string         currentWord;
// 	std::istringstream  iss;
// 	//push smth into a new location?

// 	//create a new location, parse it then push it into the location inside the server?
// 	server.getLocations().push_back(newLocation); //this doesnt exist anymore
// 	//at this point we know we should have a location instance added to the server
// 	while (!configStream.eof()) //the other closing bracket is not found
// 	{
// 		getline(configStream, currentLine); //double check line endings, shouldn't it read until ; ? what happens if there is a newline before ; ? etc
// 		if (configStream.good())
// 		{
// 			iss.clear();
// 			iss.str(currentLine);
// 			while (iss.good())
// 			{
// 				iss >> currentWord;
// 				if (isLocationDirective(currentWord) == true)
// 				{
// 					parseInstance.selectLocationParserFunction(currentLine, currentWord, parseInstance, server);
// 				}
// 				else if (currentWord == "}")
// 				{
// 					return ;
// 					//
// 				}
// 			}
// 		}
// 	}
// 	return ;
// }
