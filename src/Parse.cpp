#include "../inc/Parse.hpp"
#include "../inc/Server.hpp"
#include "../inc/Location.hpp"
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
/***
 * Parse the configFile into individual Server Blocks // Server Block type vector<std::string>
 * 
 * @return value right now is a vector of Server blocks. In the future should just return a std::vector<Server>
 * instantiated by calling Server(std::vector<std::string>)
***/
std::vector<Server>&	Parse::parseNetwork(std::string& file, std::vector<Server>& servers) //parse whole config, puts it into blocks
{
	std::vector<std::string>				buff;
	std::string								currentLine;
	std::ifstream							configStream;

	//create stack for open closing brackets,
	openFile(configStream, file);
	//create a stack
	std::stack<char> stack;
	//in the loop if we find { - >push to stack, run loop until we find }, if we found,pop it and if the stack is empty, the input was correct, proceed to parse the rest
	while(!configStream.eof())
	{
		getline(configStream, currentLine);
		if (configStream.good()) //we should check word by word?
		{	
			char *remainingLine = const_cast<char *>(currentLine.c_str());
			char *spaceSeparatedWord = strtok (remainingLine, " ");
			if (spaceSeparatedWord != NULL && strcmp(spaceSeparatedWord, "server") != 0)
			{
				buff.push_back(currentLine);
			}
			while (spaceSeparatedWord != NULL)
			{
				if (!strcmp(spaceSeparatedWord, "{")) //not sure if this always works but ok for now
				{
					stack.push('{');
				}
				if (!strcmp(spaceSeparatedWord, "}"))
				{
					stack.pop();
					if (stack.empty() == 1) // Always true if no { in first line
					{
						Server server;
						servers.push_back(parseServer(buff, server)); //this should happen if we got "server already",
						buff.clear();
					}
				}

				spaceSeparatedWord = strtok (NULL, " ");
			}
		}
	}
	return(servers);
}


Server&		Parse::parseServer(std::vector<std::string>& server_block, Server& server)
{
	std::vector<std::string>			buff;
	std::vector<Location>				locations;
	std::vector<std::string>::iterator	it = server_block.begin();
	std::stack<char> stack;

	// size_t i = 0;
	// size_t block_size = server_block.size();
	// while(i < block_size)
	// {
	// 	std::cout << it[i] << " " << std::endl;
	// 	i++;
	// }



	while(it != server_block.end())
	{
		//at this point we should set somehow if there was a "location line found"
		std::cout << *it << " is line" << std::endl;
		//std::string								currentLine;
		//int i = 0 ;

		char *remainingLine = const_cast<char *>((*it).c_str());
		char *spaceSeparatedWord = strtok (remainingLine, "\t");
		while (spaceSeparatedWord != NULL)
		{
			std::string directiveCheck(spaceSeparatedWord);
			if (isDirective(directiveCheck))
			{
				//std::cout << " giving " << *it << " to parsedirectives" << std::endl;
				parseDirective(*it, server);
				break ;
			}
			spaceSeparatedWord = strtok (NULL, " ");
		}
			
		// //if (isDirective(*it)) //if its server directive, it is essentially a a line
		// if (true) // Need to implement isLocationBlock(); //how can we check if its a location block?
		// {
		// 	Location location; //server locatons i guess is enough
			//we have to do the same, push stuff into buffer until there is an opening and closing bracket
			//
			/*
			while(!configStream.eof())
			{
				getline(configStream, currentLine);
				if (configStream.good()) //we should check word by word?
				{
					buff.push_back(currentLine);
					char *remainingLine = const_cast<char *>(currentLine.c_str());
					char *spaceSeparatedWord = strtok (remainingLine, " ");
					while (spaceSeparatedWord != NULL)
					{
						if (!strcmp(spaceSeparatedWord, "{")) //not sure if this always works but ok for now
						{
							stack.push(spaceSeparatedWord[0]);
						}
						if (!strcmp(spaceSeparatedWord, "}"))
						{
							stack.pop();
							if (stack.empty() == true) // Always true if no { in first line
							{
								Server server;
								servers.push_back(parseServer(buff, server)); //this should happen if we got "server already",
								buff.clear();
							}
						}
						spaceSeparatedWord = strtok (NULL, " ");
					}
				}
			}
			*/
			// locations.push_back(parseLocation(extractLocationBlock(*it), location)); // Need To make extractLocationBlock
		// }
		// else
		// {
		// 	// throw error return = line where parsing error happend
		// }
		it++;
	}
	return server;
	
	//here call it until the end of the block of "server"
	//Parse::selectParseFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server)//get the buffer (block here)
	//check line by line, if its server stuff

	//otehrwite location

}

Location&	Parse::parseLocation(std::vector<std::string>& location_block, Location& location) //call it till the end of location block
{
	std::vector<std::string>::iterator it = location_block.begin();

	while(it != location_block.end())
	{
		if (isLocationDirective(*it))
			parseLocationDirective(*it, location);
		else
		{
			//error print line where parsing error happend
		}
	}
	return location;
}

/*** 
 * Based on the currentWord, an associated parsing function will be called
 * The parsing function will start to look for input after the currentWord in the currentLine
 * This is skipping location for now entirely, have to alter struct a bit
***/
void    Parse::parseDirective(std::string& currentLine, Server& server)
{
	std::string restOfLine;
	std::cout << currentLine << " is currentline in parsedirectives " << std::endl;
	void	(Parse::*pointerToParseServerDirectives)(Server&, std::string&) = NULL;
	const t_selectServer myDispatch[] = 	//map instead, name of the table typedef is not descriptive enough
	{
			{"listen", &Parse::parseListen},
			{"server_name", &Parse::parseServerName},
			{"root", &Parse::parseRoot},
			{"index", &Parse::parseIndex},
			{"client_body_size", &Parse::parseClientBodySize},
	};

	char *remainingLine = const_cast<char *>(currentLine.c_str());
	std::cout << *remainingLine << " IS REMAININGLINE " << std::endl;

	char *spaceSeparatedWord = strtok (remainingLine, " ");
		std::cout << *spaceSeparatedWord << " IS SPACESEPARATED " << std::endl;
	///PROBLEM IS HERE, THIS DOES NOT SEE ANYTHING, NO REMAINNIN NEITHER SPACESEPARATED
	while (spaceSeparatedWord != NULL)
	{
		std::string currentWord(spaceSeparatedWord);
		for (int i = 0; i < NR_OF_DIRECTIVES_TO_LOOK_FOR; i++)
		{
			if (currentWord == myDispatch[i]._name)
			{
				size_t      posInLine = currentWord.length();
				restOfLine = currentLine.substr(posInLine + 1); //index index.html
				std::cout << currentWord << " IS CURRENTWORD " << myDispatch[i]._name << " IS NAME" << std::endl;
				pointerToParseServerDirectives = myDispatch[i].pointerToParseServerDirectives;
				break ;
			}
		}
		spaceSeparatedWord = strtok (remainingLine, " ");
	}
	(this->*pointerToParseServerDirectives)(server, restOfLine);
}

//write static const dispatch table
///
///
//
//asdasd

void    Parse::parseLocationDirective(std::string& currentLine, Location& locationInstance)
{
	//location ptr server.getlocaton
	//location structure, we can just get the one (current from server, declare it here and use that var to pull data from)
	//create here and add to server?
	//locationInstance.
	//if this would be static 
	void	(Location::*pointerToLocation)(std::string&) = NULL;

	const t_selectLocation myDispatch[] = 	//map instead, name of the table typedef is not descriptive enough
	{
			{"root", &Location::setLocationRoot},
			{"index", &Location::setLocationIndex},
			{"allow_methods", &Location::setLocationAllowMethod},
			{"autoindex", &Location::setLocationAutoindex},
			{"cgi_name", &Location::setCgiName},
			{"cgi_file_extension", &Location::setCgiFileExtension}
	};

	//not sure if this
	char *remainingLine = const_cast<char *>(currentLine.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, " ");
	while (spaceSeparatedWord != NULL)
	{
		std::string currentWord(spaceSeparatedWord);
		for (int i = 0; i < NR_OF_DIRECTIVES_TO_LOOK_FOR + 1; i++) //have to alter macro
		{
			if (currentWord == myDispatch[i]._name)
			{
				size_t      posInLine = currentWord.length();
				std::string restOfLine = currentLine.substr(posInLine + 2);
				pointerToLocation = myDispatch[i].pointerToLocation;
				(&locationInstance->*pointerToLocation)(restOfLine);
			}
		}
		spaceSeparatedWord = strtok (NULL, " ");
	}
}



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
		
// 				if (isDirective(currentWord) == true) //find any keyword for server currentWord == server
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


/*** 
 * Checks if a word is a directive so we can select an associated function later
***/
bool    Parse::isDirective(std::string& currentWord)
{
	//what happens if the word is empty? wouldnt == overload return true? 
	return (currentWord == "server_name" || currentWord == "root" || currentWord == "index" || currentWord == "client_body_size");
			//currentWord == "location"); adding location later
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
			//currentWord == "location"); adding location later
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
	//server.setIndex(currentLine);
	
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
