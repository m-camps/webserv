#include "../inc/Server.hpp"
#include <iostream>

Server::Server(): _clientBodySize("10")
{
    this->_name.push_back("localhost");
    this->_port.push_back("8080");
    this->_root.push_back("data/www");
    this->_index.push_back("index.html");
    this->_methods->push_back("GET");
    this->_methods->push_back("POST");
    this->_methods->push_back("DELETE");
    this->_addressLen = sizeof(this->_address);
    memset((char *)&this->_locations, 0, sizeof(this->_locations));
	memset((char *)&this->_address, 0, this->_addressLen);
    return;
}

Server::Server(const Server& src)
{
    *this = src;
    return;
}

Server& Server::operator=(const Server& rhs)
{
    if (this != &rhs)
    {
        this->_name = rhs._name;
        this->_port = rhs._port;
        this->_root = rhs._root;
        this->_index = rhs._index;
        this->_clientBodySize = rhs._clientBodySize;
        //methods?this->_methods = rhs._methods;
    }
    return *this;
}

Server::~Server()
{
    return;
}

/* ACCESSORS FOR SERVER */
std::vector<std::string>&	Server::getPort(void)
{
    return _port;
}

std::vector<std::string>&	Server::getName(void)
{
    return _name;
}

std::vector<std::string>&	Server::getRoot(void)
{
    return _root;
}

std::vector<std::string>&   Server::getIndex(void)
{
    return _index;
}

std::string&				Server::getClientBodySize(void)
{
    return _clientBodySize;
}

struct sockaddr_in	        Server::getSockAddr(void) const
{
    return _address;
}

size_t    			        Server::getSockAddrLen(void) const
{
    return _addressLen;
}

std::vector<t_location>& Server::getLocations(void)
{
    return _locations;
}

/* ACCESSORS FOR LOCATION */
std::vector<std::string>&	Server::getLocationRoot(void)
{
    return this->getLocations().back()._root;
}

std::vector<std::string>&	Server::getLocationIndex(void)
{
    return this->getLocations().back()._index;
}

bool&		Server::getLocationAutoIndex(void)
{
    return this->getLocations().back()._autoIndex;
}

std::vector<std::string>&	Server::getLocationAllowMethods(void)
{
    return this->getLocations().back()._methods;
}

std::string&	Server::getLocationCgiFileExtension(void)
{
    return this->getLocations().back()._cgi_file_extension;
}

std::string&	Server::getLocationCgiName(void)
{
    return this->getLocations().back()._cgi_name;
}

/* MUTATORS */
void	Server::setPort(Server& server, std::string& portToAdd)
{
    server.getPort().push_back(portToAdd);
    return ;
}

void	Server::setName(Server& server, std::string& serverNameToAdd)
{
    server.getName().push_back(serverNameToAdd);
    return ;
}

void	Server::setRoot(Server& server, std::string& rootLocationToAdd)
{
    server.getRoot().push_back(rootLocationToAdd);
    return ;
}

void	Server::setIndex(Server& server, std::string& indexToAdd)
{
    server.getIndex().push_back(indexToAdd);
    return ;
}

/*
void	Server::setLocationIndex(Server& server, std::string& LocationIndexToAdd)
{
    //t_location current = server.getLocations().back(); //push_back(methodToAdd);
    std::vector<std::string>::iterator it;
    std::vector<std::string>& myvector = server.getLocationIndex();
    it = myvector.begin();
    it = myvector.insert(it, LocationIndexToAdd);
    std::cout << server.getLocationIndex().back() << "IS LOC INDEX!!!!!" << std::endl;
    return ;
}
*/

void	Server::setClientBody(Server& server, std::string& BodySize)
{
    server.getClientBodySize() = BodySize;
    return ;
}

void	Server::setMethods(Server& server, std::string& methodToAdd)
{
    //validate input?
    (void)server;
    methodToAdd.erase(std::remove_if(methodToAdd.begin(), methodToAdd.end(), ::isspace), methodToAdd.end());
    if (methodToAdd == "GET" || methodToAdd == "POST" || methodToAdd == "DELETE")
    {
        //std::cout << methodToAdd << " IS SET METHODS " << std::endl;
       // server.getLocations().back()._methods.push_back(methodToAdd);
        //std::cout << server.getLocations().back()._methods.back() << " is the address of methods " << std::endl; //._methods << " is the address of methods " << std::endl; //.push_back(methodToAdd);
    }
    else
    {
//        std::cout << "incorrent method, did not add." << std::endl;
    }
    //t_location current = server.getLocations().back(); //push_back(methodToAdd);
    return ;
}


/* LOCATION MUTATORS */


void	setLocationIndex(Server& server, std::string& LocationIndexToAdd)
{
    char *remainingLine = const_cast<char *>(LocationIndexToAdd.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, " ");

	while (spaceSeparatedWord != NULL)
	{
		std::string indexToAdd(spaceSeparatedWord);
		indexToAdd.erase(remove(indexToAdd.begin(), indexToAdd.end(), ';'), indexToAdd.end());
		//server.setIndex(server, indexToAdd);

        //here coems another function to handle the insert, for now keep it 
        std::vector<std::string>::iterator it;
        std::vector<std::string>& myvector = server.getLocationIndex();
        it = myvector.begin();
        it = myvector.insert(it, indexToAdd);
        //
		spaceSeparatedWord = strtok (NULL, " ");
	}
    return ;
}

void	setCgiName(Server& server, std::string& cgiName)
{
    cgiName.erase(remove(cgiName.begin(), cgiName.end(), ';'), cgiName.end());
    server.getLocationCgiName() = cgiName;
    return ;
}


void	setCgiFileExtension(Server& server, std::string& cgiFileExtension)
{
    cgiFileExtension.erase(remove(cgiFileExtension.begin(), cgiFileExtension.end(), ';'), cgiFileExtension.end());
    server.getLocationCgiName() = cgiFileExtension;
    return ;
}

void	setLocationRoot(Server& server, std::string& locationRootToAdd)
{
    char *remainingLine = const_cast<char *>(locationRootToAdd.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, " ");

	while (spaceSeparatedWord != NULL)
	{
		std::string rootToAdd(spaceSeparatedWord);
		rootToAdd.erase(remove(rootToAdd.begin(), rootToAdd.end(), ';'), rootToAdd.end());
		//server.setIndex(server, indexToAdd);
        //here coems another function to handle the insert, for now keep it 
        std::vector<std::string>::iterator it;
        std::vector<std::string>& myvector = server.getLocationRoot();
        it = myvector.begin();
        it = myvector.insert(it, rootToAdd);
        //
		spaceSeparatedWord = strtok (NULL, " ");
	}
}

void	setLocationAllowMethod(Server& server, std::string& locationAllowedMethods)
{
    char *remainingLine = const_cast<char *>(locationAllowedMethods.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, " ");

	while (spaceSeparatedWord != NULL)
	{
		std::string methodToAdd(spaceSeparatedWord);
		methodToAdd.erase(remove(methodToAdd.begin(), methodToAdd.end(), ';'), methodToAdd.end());
		//server.setIndex(server, indexToAdd);

        //here coems another function to handle the insert, for now keep it 
        std::vector<std::string>::iterator it;
        std::vector<std::string>& myvector = server.getLocationAllowMethods();
        it = myvector.begin();
        it = myvector.insert(it, methodToAdd);
		spaceSeparatedWord = strtok (NULL, " ");
	}
}

void	setLocationAutoindex(Server& server, std::string& AutiondexToSet)
{
    AutiondexToSet.erase(remove(AutiondexToSet.begin(), AutiondexToSet.end(), ';'), AutiondexToSet.end());
    if (AutiondexToSet == "off")
    {
        server.getLocationAutoIndex() = false;
    }
    else
    {
        server.getLocationAutoIndex() = true;
    }
    return ;
}


/* REST */
void	Server::printServerAttributes(Server& server)
{
    (void)server;
    /*
    for(int i=0; i < server.getPort().size(); i++) //print Ports
    {
        std::cout << server.getPort().at(i) << " ";
    }
    std::cout << "are ports of the server." << std::endl;

    for(int i=0; i < server.getName().size(); i++) //print server_names
    {
        std::cout << server.getName().at(i) << " ";
    }
    std::cout << " are the server names." << std::endl;

    for(int i=0; i < server.getRoot().size(); i++) //print root locations
    {
        std::cout << server.getRoot().at(i) << " ";
    }
    std::cout << " are the root names." << std::endl;

     for(int i=0; i < server.getIndex().size(); i++) //print indexes
    {
        std::cout << server.getIndex().at(i) << " ";
    }
    std::cout << " are the index names." << std::endl;
    std::cout << server.getClientBodySize() << " is the client body size." << std::endl;
    */
    return ;
}

/*
std::ostream& operator<<(std::ostream& stream, const Server& Server)
{
    //have to create a function which displays all the servers attributes
    stream << std::endl;
    return stream;
}
*/
