//
// Created by Xander Voorvaart on 9/30/22.
//

#include "HeaderParse.hpp"

std::map<std::string, std::string> MapTheHeader(const std::string& Header)
{
    return ();
}

// 1. Append data
// 2. Check if data has \r\n\r\n
//  - Yes: Split the buffer at that point.
//  - No: Back to step 1.
// 3. For Part A of split buffer, getline it into a dictonary
// 4. Make sure the data var only has the body left.

/**
 * I look for the "\\r\\n\\r\\n" seperator by using find.
 * Which I will then append to the Header string.
 */
std::string AppendRequest(const std::string& Request)
{
    std::string Header;

    std::size_t found = Request.find("\r\n\r\n");
    if (found == std::string::npos)
        std::cerr << "Error" << std::endl;
    Header.append(Request, 0, found);
    return (Header);
}

////////////// Ctor & Dtor //////////////

/**
 *
 * @param Request will be the header & the body in one string
 */
HeaderParse::HeaderParse(const std::string& Request)
{
    const std::string Header = AppendRequest(Request);;

    std::cout << Header << std::endl;
//    SeperatedHeader = SeperateHeader(Header);

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

////////////// Functions //////////////
