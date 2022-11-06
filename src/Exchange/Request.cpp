//
// Created by Xander Voorvaart on 9/30/22.
//

#include "Request.hpp"

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
		throw (std::out_of_range("No seperator found"));

	Header.append(Request, 0, found);

    Body.append(Request, found, Request.length() - found);
    _Exchanger.addHashMapNode("Body", Body);

	return (Header);
}

/**
 * splitMethod splits the HTTPMethod to the following std::map
 *
 * HTTPMethod <- The request method
 * Path <- Path of the page being requested
 * HTTPVersion <- Speaks for itself
 */
void Request::splitMethod(std::string line)
{
    int32_t last_it;
	std::vector<int> AllSpaceLocations = findCharLocation(line, ' ');
	std::vector<int>::iterator it = AllSpaceLocations.begin();

	try
	{
        _Exchanger.addHashMapNode("HTTPMethod", line.substr(0, *it));
        last_it = *it + 1;
        _Exchanger.addHashMapNode("Path", line.substr(*it + 1, *++it - last_it));
        last_it = *it;
        _Exchanger.addHashMapNode("HTTPVersion", line.substr(*it + 1, line.length() - last_it));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::exit(EXIT_FAILURE);
	}
}

/*
 * I created a HashMap by using std::map.
 * You can use std::map::find() to find the data you are looking for. \n
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
			splitMethod(line.erase(line.length()));
			continue ;
		}
		_Exchanger.addHashMapNode(line.substr(0, found),
                                  line.substr(found + 2, (line.size() - found - 3)));
	}
}
