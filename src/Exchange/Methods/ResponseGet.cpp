//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

void Respond::BuildRedir(void)
{
    const std::string NewLocation = "index.html"; // Needs to change to location index

    addToHeader(Generator::generateStatus(*this));
    addToHeader(Generator::generateLocation(NewLocation));
    setBody("");
}

void Respond::buildGet(void)
{
    std::cout << "GET" << std::endl;
    try
    {
        std::string FileContent;
        std::string relativePath;
        std::string Root = "data/www"; // Needs to change to location root
        std::string Path = getEntryFromMap("Path");

        relativePath = Root + Path;
        _status_code = modifyStatusCode(Path, relativePath);

        FileContent = getValidFile(Root, relativePath, _status_code);

        createResponse(FileContent);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        std::exit(ERROR);
    }
}
