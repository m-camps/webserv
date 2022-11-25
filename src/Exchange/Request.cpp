//
// Created by Xander Voorvaart on 9/30/22.
//

#include "Request.hpp"

////////////// Ctor & Dtor //////////////


Request::Request(void) {}
Request::~Request(void) {}

////////////// Functions //////////////

HashMap		Request::parseRequest(const std::string& requestStr)
{
	try
	{
		const std::string Header = AppendRequest(requestStr);
		stringToMap(Header);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (_requestData);
}
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
    addHashMapNode("Body", Body);
	return (Header);
}

void removeNonAlnum(std::string& str)
{
    for (int32_t i = 0; str[i]; i++)
    {
        if (std::isalnum(str[i]) == false && str[i] != '.' && str[i] != '/')
        {
            str.erase(i, 1);
        }
    }
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
	size_t found;
	std::string temp;

	found = line.find_first_of(' ');
	if (found == std::string::npos)
	{
		throw (std::runtime_error("In splitMethod: The HTTPMethod is not complete"));
	}

	temp = line.substr(0, found);
	addHashMapNode("HTTPMethod", temp);
	line.erase(0, found + 1);
	temp.clear();

	found = line.find_last_of(' ');
	if (found == std::string::npos)
	{
		throw (std::runtime_error("In splitMethod: The HTTPMethod is not complete"));
	}

	temp = line.substr(found + 1, line.length() - found);
    removeNonAlnum(temp);
	addHashMapNode("HTTPVersion", temp);
	line.erase(found);
	temp.clear();

	found = line.find('/');
	if (found == std::string::npos)
	{
		throw (std::runtime_error("In splitMethod: The HTTPMethod is not complete"));
	}
	addHashMapNode("Path", line);
}

/*
 * I created a HashMap by using std::map.
 * You can use std::map::find() to find the data you are looking for.
 */
void Request::stringToMap(const std::string& Header)
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
		addHashMapNode(line.substr(0, found),
                                  line.substr(found + 2, (line.size() - found - 3)));
	}
}

void Request::addHashMapNode(const std::string& NameNode, const std::string& ContentNode)
{
    _requestData[NameNode] = ContentNode;
}
