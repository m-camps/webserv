//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

void Respond::buildRedir(void)
{
    std::string LocationName = _location.getName();
    std::string NewLocation;

    NewLocation = Generator::generateLocation(_location.getReturnPath());
    createResponse("");
    addToHeader(NewLocation);
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
        Cgi	cgi;
        std::string FileContent;
        std::string relativePath;
        std::string Root = _location.getRoot();
        std::string Path = getEntryFromMap("Path");

        parsePath(Path);
        relativePath = Root + Path;
        modifyStatuscode(Path, relativePath);
        if (_location.getCgi() == 1 && _status_code != e_Redir)
            FileContent = cgi.executeScript(*this);
        else
            FileContent = getValidFile(relativePath);
        createResponse(FileContent);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Internal Server Error: " << e.what() << std::endl;
        _status_code = e_InternalServerError;
        createResponse(Generator::generateDefaulPage(*this));
    }
}
