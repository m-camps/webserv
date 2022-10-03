//
// Created by Xander Voorvaart on 9/30/22.
//

#include "HeaderParse.hpp"

const std::string ParseGet(const std::string& FullHeader)
{
    std::string _get;

    if (FullHeader.compare(0, 3, "GET") == 0)
        _get = FullHeader.substr(5, 20);
    return (_get);
}

HeaderParse::HeaderParse(const std::string& FullHeader)
    : _Get(ParseGet(FullHeader))
{
    std::cout << FullHeader
                << std::endl
                << FullHeader
                << std::endl;
}

HeaderParse::HeaderParse(const HeaderParse& ref)
{
    (void) ref;
}

HeaderParse::~HeaderParse(void)
{
}

////////////// Operators //////////////
HeaderParse &HeaderParse::operator=(const HeaderParse& ref)
{
    if (this != &ref)
    {
    }
    return (*this);
}

// Functions
