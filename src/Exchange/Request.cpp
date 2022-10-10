//
// Created by Xander Voorvaart on 9/30/22.
//

#include "Request.hpp"

typedef std::map<std::string, std::string> map;

////////////// Ctor & Dtor //////////////

/**
 * @param Request will be the header & the body in one string
 */
Request::Request(const std::string& Request, const Server& new_server, int32_t new_socket)
		: _server(new_server), _SocketFD(new_socket)
{
	const std::string Header = AppendRequest(Request);

	HeaderToMap(Header);
	Respond Responder(_server, _dictHeader, _SocketFD);
}

Request::Request(const Request& ref)
		: _server(ref._server), _dictHeader(ref._dictHeader), _SocketFD(ref._SocketFD)
{
}

Request::~Request(void)
{
}

////////////// Operators //////////////

Request &Request::operator=(const Request& ref)
{
	if (this != &ref)
	{
		_dictHeader = ref._dictHeader;
		_server = ref._server;
	}
	return (*this);
}

/*
 * Prints only the std::map::_dictHeader
 */
std::ostream& operator<<(std::ostream& out, const Request& ref)
{
	std::map<std::string, std::string> Header = ref.getHeader();
	std::map<std::string, std::string>::iterator it = Header.begin();

	for ( ; it != Header.end(); it++)
	{
		out <<
		    std::endl <<
		    "Key: " << it->first <<
		    "\n" <<
		    "Value: " << it->second <<
		    std::endl;
	}
	return (out);
}

////////////// Functions //////////////

/**
 * I look for the "\\r\\n\\r\\n" seperator by using std::find.
 * Which I will then append to the Header string.
 */
std::string Request::AppendRequest(const std::string& Request) const
{
	std::string Header;

	std::size_t found = Request.find("\r\n\r\n");
	if (found == std::string::npos)
	{
		std::cerr << "No separator found" << std::endl;
//        std::exit(EXIT_FAILURE);
	}
	Header.append(Request, 0, found);
	return (Header);
}

/**
 * splitMethod splits the HTTPMethod to the following std::map
 *
 * HTTPMethod <- The request
 * Path <- Path of the page being requested
 * HTTPVersion <- Speaks for itself
 */
void Request::splitMethod(std::string line)
{
	std::vector<int> AllSpaceLocations = findCharLocation(line, ' ');
	std::vector<int>::iterator it = AllSpaceLocations.begin();

	try
	{
		_dictHeader["HTTPMethod"] = line.substr(0, *it);
		_dictHeader["Path"] = line.substr(*it + 1, *++it - 4);
		_dictHeader["HTTPVersion"] = line.substr(*it + 1, line.length());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::exit(EXIT_FAILURE);
	}
}

/*
 * This is gonna be a long one...
 *
 * I created a HashMap by using std::map.
 * You can use std::map::at() to find the data you are looking for. \n
 *
 * For example:
 * std::cout << std::map::at("HTTPMethod") << std::endl;
 *
 * Output:
 * GET / HTTP/1.1
 *
 * The following keywords are important:
 * Host <-- What is the hostname
 * Connection <-- To check connection
 * User-Agent <-- Information of the user
 */
void Request::HeaderToMap(const std::string& Header)
{
	std::string line;
	std::istringstream issHeader(Header);

	while (std::getline(issHeader, line))
	{
		std::size_t found = line.find(':');
		if (found == std::string::npos)
		{
			splitMethod(line.substr(0, line.size() - 1));
			continue ;
		}
		_dictHeader[line.substr(0, found)] =
				line.substr(found + 2, (line.size() - found - 3));
	}
}

////////////// Geter //////////////

const Request::map& Request::getHeader(void) const
{
	return (_dictHeader);
}
