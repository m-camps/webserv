//
// Created by Xander Voorvaart on 11/1/22.
//

#ifndef GENERATE_HPP
#define GENERATE_HPP

#include "Exchange/Exchange.hpp"
#include "Utilities.hpp"

#include <iostream>

#define MAXBYTES 30000

class Generator
{
public:
    Generator(void);
    ~Generator(void);

    void generateStatus(Exchange&);
    void generateContentLength(Exchange&, std::size_t);
    void generateLocation(Exchange&, const std::string);
    void generateContentType(Exchange&);
    void generateTransferEncoding(Exchange&);
    std::string generateChunk(Exchange&);
    std::string generateBoundry(Exchange&);
};


#endif //GENERATE_HPP
