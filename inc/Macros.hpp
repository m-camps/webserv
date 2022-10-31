//
// Created by Xander Voorvaart on 10/20/22.
//

#ifndef WEBSERV_MACROS_HPP
#define WEBSERV_MACROS_HPP

#include <iostream>
#include <string>
#include <sstream>

template <typename T>
  std::string TOSTRING ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }
  
#endif //WEBSERV_MACROS_HPP
