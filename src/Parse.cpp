#include "../inc/Parse.hpp"
#include "../inc/Server.hpp"
#include <fstream>
#include <sstream>
#include <string.h>


/* CONSTRUCTORS */

/***
 * Not sure if we should precreate a dispatch table upon constructon or not,
 * for now we create it inside the selectParseFunction function, later maybe could be even static
 ***/
Parse::Parse()
{
    //this->_dispatchTable._name = "";
    //this->_dispatchTable._pointerToParsingFunction = nullptr;
    //std::cout << "Constructor of Parse is called" << std::endl;
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
       // this->_dispatchTable._name = rhs._dispatchTable._name;
       // this->_dispatchTable._pointerToParsingFunction = rhs._dispatchTable._pointerToParsingFunction;
    }
    return *this;
}

Parse::~Parse()
{
    //std::cout << "Destructor of Parse is called" << std::endl;
    return ;
}


/* PARSER FUNCTIONS */

/*** 
 * MAIN PARSING FUNCTION
***/
void    startParse(Server& server, char *configName)
{
    Parse           parseInstance;
    std::ifstream   config;

    //open and create file
    parseInstance.openFile(config, configName);
    //read line by line
    parseInstance.processFile(config, parseInstance, server);
    return ;
}

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
        //what to return then? exit(1) ?
    }
}

/*** 
 * Loops through all the lines in the given file
 * If a word in a line is a directive, proceeds to select the function to be used for that directive
***/
void    Parse::processFile(std::ifstream& configStream, Parse& parseInstance, Server& server)
{
    std::string         currentLine;
    std::string         currentWord;
    std::istringstream  iss;

    while (!configStream.eof())
    {
        getline(configStream, currentLine); //double check line endings, shouldn't it read until ; ? what happens if there is a newline before ; ? etc
        if (configStream.good())
        {
            iss.clear();
            iss.str(currentLine);
            while (iss.good())
            {
                iss >> currentWord;
                //could be solved only to look for first words of the line, but that assumes correct input 
                if (isDirective(currentWord) == true)
                {
                    selectParseFunction(currentLine, currentWord, parseInstance, server);
                }
            }
            std::cout << std::endl;
        }
    }
}

/*** 
 * Checks if a word is a directive so we can select an associated function later
***/
bool    Parse::isDirective(std::string& currentWord)
{
    //what happens if the word is empty? wouldnt == overload return true? 
    return (currentWord == "listen" || currentWord == "server_name" || currentWord == "root" ||
            currentWord == "root" || currentWord == "index" || currentWord == "client_body_size");
            //currentWord == "location"); adding location later
}

/*** 
 * Based on the currentWord, an associated parsing function will be called
 * The parsing function will start to look for input after the currentWord in the currentLine
 * This is skipping location for now entirely, have to alter struct a bit
***/
void    Parse::selectParseFunction(std::string& currentLine, std::string& currentWord, Parse& parseInstance, Server& server)
{
    const t_selectParse myDispatch[] = 
	{
		{"listen", parseListen},
		{"server_name", parseServerName},
		{"root", parseRoot},
		{"index", parseIndex},
		{"client_body_size", parseClientBodySize},
	};

    for (int i = 0; i < NR_OF_DIRECTIVES_TO_LOOK_FOR; i++)
	{
		if (currentWord == myDispatch[i]._name)
		{
            size_t      posInLine = currentWord.length();
            std::string restOfLine = currentLine.substr(posInLine + 1);
			myDispatch[i]._pointerToParsingFunction(server, restOfLine); 
		}
	}
}

/* SEPARATE PARSERS FOR THE RESPECTIVE DIRECTIVES */
void    parseListen(Server& server, std::string& currentLine)
{
    std::size_t firstFoundDigit = currentLine.find_first_of("0123456789"); //what happens if port start with 0, is that valid?
    if (firstFoundDigit != std::string::npos)
    {
        std::string portNumber = currentLine.substr(firstFoundDigit);
        //trail the last; -> what happens if there are more ;;;;, other error checks are needed
        portNumber.erase(remove(portNumber.begin(), portNumber.end(), ';'), portNumber.end());
        server.setPort(server, portNumber);
    }
    return ;
}

void    parseServerName(Server& server, std::string& currentLine)
{
    char *remainingLine = const_cast<char *>(currentLine.c_str());
    char *spaceSeparatedWord = strtok (remainingLine, " ");

    while (spaceSeparatedWord != NULL)
    {
        std::string serverNameToAdd(spaceSeparatedWord);
        //trail the last; -> what happens if there are more ;;;;, other error checks are needed
        serverNameToAdd.erase(remove(serverNameToAdd.begin(), serverNameToAdd.end(), ';'), serverNameToAdd.end());
        server.setName(server, serverNameToAdd);
        spaceSeparatedWord = strtok (NULL, " ");
    }
    return ;
}

void    parseRoot(Server& server, std::string& currentLine)
{
    char *remainingLine = const_cast<char *>(currentLine.c_str());
    char *spaceSeparatedWord = strtok (remainingLine, " ");

    while (spaceSeparatedWord != NULL)
    {
        std::string rootToAdd(spaceSeparatedWord);
        //trail the last; -> what happens if there are more ;;;;, other error checks are needed
        rootToAdd.erase(remove(rootToAdd.begin(), rootToAdd.end(), ';'), rootToAdd.end());
        server.setRoot(server, rootToAdd);
        spaceSeparatedWord = strtok (NULL, " ");
    }
    return ;
}

void    parseIndex(Server& server, std::string& currentLine)
{
    char *remainingLine = const_cast<char *>(currentLine.c_str());
    char *spaceSeparatedWord = strtok (remainingLine, " ");

    while (spaceSeparatedWord != NULL)
    {
        std::string indexToAdd(spaceSeparatedWord);
        //trail the last; -> what happens if there are more ;;;;, other error checks are needed
        indexToAdd.erase(remove(indexToAdd.begin(), indexToAdd.end(), ';'), indexToAdd.end());
        server.setIndex(server, indexToAdd);
        spaceSeparatedWord = strtok (NULL, " ");
    }
    return ;
}

void    parseClientBodySize(Server& server, std::string& currentLine)
{
    std::size_t firstFoundDigit = currentLine.find_first_of("0123456789"); //what happens if port start with 0, is that valid?
    if (firstFoundDigit != std::string::npos)
    {
        std::string bodySize = currentLine.substr(firstFoundDigit);
        //trail the last; -> what happens if there are more ;;;;, other error checks are needed
        bodySize.erase(remove(bodySize.begin(), bodySize.end(), ';'), bodySize.end());
        server.setClientBody(server, bodySize);
    }
    return ;
}

void    parseLocation(Server& server, std::string& currentLine)
{
    //std::cout << "hello, inside parse location." << std::endl;
    //std::cout << currentLine << " is currentLine in parse location " << std::endl;
    //for now just skip x amount of lines until we find the closing braces
    return ;
}

