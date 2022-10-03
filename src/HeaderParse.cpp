//
// Created by Xander Voorvaart on 9/30/22.
//

#include "HeaderParse.hpp"

////////////// Ctor & Dtor //////////////

/**
 * @param Request will be the header & the body in one string
 */
HeaderParse::HeaderParse(const std::string& Request)
{
    const std::string Header = AppendRequest(Request);

    MapTheHeader(Header);
}

// Deep copy?
HeaderParse::HeaderParse(const HeaderParse& ref)
    : _dictHeader(ref._dictHeader)
{
}

HeaderParse::~HeaderParse(void)
{
}

////////////// Operators //////////////

HeaderParse &HeaderParse::operator=(const HeaderParse& ref)
{
    if (this != &ref)
        this->_dictHeader = ref._dictHeader;
    return (*this);
}

////////////// Functions //////////////

/**
 * I look for the "\\r\\n\\r\\n" seperator by using std::find.
 * Which I will then append to the Header string.
 */
std::string HeaderParse::AppendRequest(const std::string& Request)
{
    std::string Header;

    std::size_t found = Request.find("\r\n\r\n");
    if (found == std::string::npos)
        std::cerr << "No separator found" << std::endl;
    Header.append(Request, 0, found);
    return (Header);
}

/**
 * This is gonna be a long one... \n
 *
 * I created a HashMap by using std::map.
 * You can use std::map::at() to find the data you are looking for. \n
 * \n
 * For example:
 * std::cout << std::map::at("HTTPMethod") << std::endl; \n
 * \n
 * Output:
 * GET / HTTP/1.1 \n
 *
 * The following keywords are important: \n
 * HTTPMethod <-- Here you can find obviously the method and the address that is requested \n
 * Host <-- What is the hostname \n
 * Connection <-- To check connection \n
 * User-Agent <-- Information of the user \n
 */
void HeaderParse::MapTheHeader(const std::string& Header)
{
    std::string line;
    std::istringstream issHeader(Header);

    while (std::getline(issHeader, line))
    {
        std::size_t found = line.find(':');
        if (found == std::string::npos)
        {
            _dictHeader["HTTPMethod"] = line;
            continue ;
        }
        _dictHeader[line.substr(0, found)] =
                line.substr(found + 2, (line.size() - found + 2));
    }
}
