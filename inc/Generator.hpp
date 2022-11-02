//
// Created by Xander Voorvaart on 11/1/22.
//

#ifndef GENERATE_HPP
#define GENERATE_HPP

#include "Exchange.hpp"
#include "Utilities.hpp"

#include <iostream>
#include <dirent.h> // DIR

#define MAXBYTES 100000

class Generator
{
public:
    Generator(void);
    ~Generator(void);

    static void generateStatus(Exchange&);
    static void generateContentLength(Exchange&, std::size_t);
    static void generateLocation(Exchange&, const std::string);
    static void generateContentType(Exchange&);
    static void generateTransferEncoding(Exchange&);
    static std::string generateChunk(Exchange&);
    static std::string generateBoundry(Exchange&);
    static std::string generateDefaulPage(uint32_t);
    static std::string generateAutoIndex(Exchange&);
};


#endif //GENERATE_HPP
