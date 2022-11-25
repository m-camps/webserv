//
// Created by Xander Voorvaart on 9/30/22.
//

#pragma once

#include <map>
#include <iostream> // std::cout
#include <fstream> // std::istream
#include <sstream> // issstringstream()

#include "Respond.hpp"
#include "Server.hpp"

typedef std::map<std::string, std::string> HashMap;

class Request
{
public:
 	Request(void);
    ~Request(void);

	HashMap		parseRequest(const std::string& requestStr);

private:
    Request(const Request&);
    Request& operator=(const Request& ref);

	HashMap		_requestData;

    void		stringToMap(const std::string&);
    void 		splitMethod(std::string line);
    std::string AppendRequest(const std::string& Request);
	void 		addHashMapNode(const std::string&, const std::string&);
};
