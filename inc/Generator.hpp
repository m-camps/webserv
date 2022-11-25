//
// Created by Xander Voorvaart on 11/1/22.
//

#pragma once

#include <iostream>
#include <dirent.h> // DIR
#include <sys/stat.h>

#include "Respond.hpp"
#include "Utilities.hpp"
#include "Constants.hpp"

class Respond; // Need to include this otherwise compile error

class Generator
{
public:
    Generator(void);
    ~Generator(void);

    static std::string generateStatus(Respond&);
    static std::string generateContentLength(std::size_t);
    static std::string generateTransferEncoding(void);
    static std::string generateChunk(std::string&);
    static std::string generateDirectoryPage(Respond&);
    static std::string generateDefaulPage(const Respond&);
    static std::string generateBoundry(Respond&);
    static std::string generateAutoIndex(Respond&, const std::string&);
    static std::string generateLocation(const std::string&);

private:
    Generator& operator=(const Generator&);
};

