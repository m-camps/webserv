//
// Created by Xander Voorvaart on 11/1/22.
//

#pragma once


#include "Utilities.hpp"
#include "../src/Exchange/Respond.hpp"
#include <iostream>
#include <dirent.h> // DIR
#include <sys/stat.h>

#define MAXBYTES 200000
#define CRLF "\r\n"

class Respond; // Need to include this otherwise compile error

class Generator
{
public:
    Generator(void);
    ~Generator(void);

    static void generateStatus(Respond&);
    static void generateContentLength(Respond&, std::size_t);
    static void generateLocation(Respond&, const std::string);
    static void generateContentType(Respond&);
    static void generateTransferEncoding(Respond&);
    static std::string generateChunk(Respond&);
    static std::string generateBoundry(Respond&);
    static std::string generateDefaulPage(uint32_t);
    static std::string generateAutoIndex(Respond&);
    static std::string generateUploadSucces(void);
};

