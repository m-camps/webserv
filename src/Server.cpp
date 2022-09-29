#include "../inc/Server.hpp"
#include <iostream>

Server::Server(): _clientBodySize("10")
{
    this->_name.push_back("localhost");
    this->_port.push_back("8080");
    this->_root.push_back("/");
    this->_index.push_back("index.html");
    this->_methods->push_back("GET");
    this->_methods->push_back("POST");
    this->_methods->push_back("DELETE");
    this->_addressLen = sizeof(this->_address);
	memset((char *)&this->_address, 0, this->_addressLen);
    //std::cout << "Constructor of Server is called." << std::endl;
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
    //std::cout << "Destructor of Server is called." << std::endl;
    return;
}

/* ACCESSORS */
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

void	Server::setClientBody(Server& server, std::string& BodySize)
{
    server.getClientBodySize() = BodySize;
    return ;
}

/* REST */
void	Server::printServerAttributes(Server& server)
{
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
    return ;
}

std::ostream& operator<<(std::ostream& stream, const Server& Server)
{
    //have to create a function which displays all the servers attributes
    stream << std::endl;
    return stream;
}