//
// Created by Xander Voorvaart on 11/1/22.
//

#pragma once


#include "Utilities.hpp"
#include "../src/Exchange/Respond.hpp"
#include <iostream>
#include <dirent.h> // DIR
#include <sys/stat.h>

#define MAXBYTES 20000
#define CRLF "\r\n"

class Respond; // Need to include this otherwise compile error

class Generator
{
public:
    Generator(void);
    ~Generator(void);

    static std::string generateStatus(Respond&);
    static std::string generateContentLength(std::size_t);
    static std::string generateLocation(const std::string);
    static std::string generateContentType(Respond&);
    static std::string generateTransferEncoding(void);
    static std::string generateChunk(std::string&);
    static std::string generateBoundry(Respond&);
    static std::string generateDefaulPage(uint32_t);
    static std::string generateAutoIndex(Respond&);
    static std::string generateUploadSucces(void);
};

