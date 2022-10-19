//
// Created by Xander Voorvaart on 9/30/22.
//

#include "Request.hpp"

typedef std::map<std::string, std::string> map;

////////////// Ctor & Dtor //////////////

/**
 * @param Request will be the header & the body in one string
 */
Request::Request(const std::string Request, Exchange NewExchanger)
		: _Exchanger(NewExchanger)
{
    try
    {
	    const std::string Header = AppendRequest(Request);

        HeaderToMap(Header);
	    Respond Responder(_Exchanger);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/* //////////////////////////// */

Request::Request(const Request& ref)
		: _Exchanger(ref._Exchanger)
{
}

/* //////////////////////////// */

Request::~Request(void)
{
}

////////////// Operators //////////////

Request &Request::operator=(const Request& ref)
{
	if (this != &ref)
	{
		_Exchanger = ref._Exchanger;
	}
	return (*this);
}

////////////// Functions //////////////

/**
 * I look for the "\\r\\n\\r\\n" seperator by using std::find.
 * Which I will then append to the Header string & the Body string.
 */
std::string Request::AppendRequest(const std::string& Request)
{
    std::string Body;
	std::string Header;

	std::size_t found = Request.find("\r\n\r\n");
	if (found == std::string::npos)
		throw (std::invalid_argument("No seperator found"));

	Header.append(Request, 0, found);

    Body.append(Request, found, Request.length() - found);
    _Exchanger.addHashMapNode("Body", Body);

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
        _Exchanger.addHashMapNode("HTTPMethod", line.substr(0, *it));
        _Exchanger.addHashMapNode("Path", line.substr(*it + 1, *++it - 4));
        _Exchanger.addHashMapNode("HTTPVersion", line.substr(*it + 1, line.length()));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::exit(EXIT_FAILURE);
	}
}

/*
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
		_Exchanger.addHashMapNode(line.substr(0, found),
                                  line.substr(found + 2, (line.size() - found - 3)));
	}
}
