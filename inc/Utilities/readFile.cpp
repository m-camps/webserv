//
// Created by Xander Voorvaart on 10/11/22.
//

#include <iostream>
#include <fstream>

/*
 * /data/www/ does not work...
 * data/www/ does... lol
 *
 * Please protect readFile with Try-Catch.
*/
std::string readFile(const std::string& RespondedFile)
{
    std::ifstream File;
    std::string line;
    std::string FileContent;

    File.open(RespondedFile);
    if (!File.is_open())
        throw (std::invalid_argument("File does not exist."));

    while (std::getline(File, line))
        FileContent += line;

    return (FileContent);
}
