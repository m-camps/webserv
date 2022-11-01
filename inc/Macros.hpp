//
// Created by Xander Voorvaart on 10/20/22.
//

#ifndef WEBSERV_MACROS_HPP
#define WEBSERV_MACROS_HPP

#include <sstream>

// #define TOSTRING( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

template <typename T>
  std::string TOSTRING ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }
#endif //WEBSERV_MACROS_HPP
