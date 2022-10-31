//
// Created by Xander Voorvaart on 10/31/22.
//

#include <iostream>
#include <sstream>

template <typename T>
std::string TOSTRING(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return (ss.str());
}