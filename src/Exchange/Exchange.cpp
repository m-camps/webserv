//
// Created by Xander Voorvaart on 9/30/22.
//

#include "Exchange.hpp"

////////////// Ctor & Dtor //////////////

/**
 * @param Request will be the header & the body in one string
 */
Exchange::Exchange(const std::string& Request)
{
    const std::string Header = AppendRequest(Request);

    MapTheHeader(Header);
    PrintHeaderMap();
}

Exchange::Exchange(const Exchange& ref)
    : _dictHeader(ref._dictHeader)
{
}

Exchange::~Exchange(void)
{
}

////////////// Operators //////////////

Exchange &Exchange::operator=(const Exchange& ref)
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
std::string Exchange::AppendRequest(const std::string& Request) const
{
    std::string Header;

    std::size_t found = Request.find("\r\n\r\n");
//    if (found == std::string::npos)
//    {
//        std::cerr << "No separator found" << std::endl;
//        std::exit(EXIT_FAILURE);
//    }
    Header.append(Request, 0, found);
    return (Header);
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
 * HTTPMethod <-- Here you can find obviously the method and the address that is requested
 * Host <-- What is the hostname
 * Connection <-- To check connection
 * User-Agent <-- Information of the user
 */
void Exchange::MapTheHeader(const std::string& Header)
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

/*
 * For debugging purposes
 */
void Exchange::PrintHeaderMap(void) const
{
   map::const_iterator it = _dictHeader.begin();

    for ( ; it != _dictHeader.end(); it++)
        std::cout <<
            std::endl <<
            "Key: "<< it->first <<
            std::endl <<
            "Value: " << it->second <<
            std::endl;
}

////////////// Geter //////////////

const Exchange::map& Exchange::getHeader(void) const
{
    return (_dictHeader);
}
