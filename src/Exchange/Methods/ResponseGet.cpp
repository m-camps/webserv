//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

void Respond::BuildRedir(void)
{
    createResponse("");
    addToHeader(Generator::generateLocation(_location.getIndex()));
}

void Respond::parsePath(std::string& Path)
{
    const std::string LocationName = _location.getName();
    std::size_t found = Path.find(LocationName);

    if (found == std::string::npos)
        return ;
    Path.erase(0, found + LocationName.size());
}

void Respond::buildGet(void)
{
    std::cout << "GET" << std::endl;
    try
    {
        std::string FileContent;
        std::string relativePath;
        std::string Root = _location.getRoot();
        std::string Path = getEntryFromMap("Path");

        parsePath(Path);
        relativePath = Root + Path;
        getStatuscode(Path, relativePath); // Change name
        FileContent = getValidFile(relativePath);
        createResponse(FileContent);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        std::exit(ERROR);
    }
}
