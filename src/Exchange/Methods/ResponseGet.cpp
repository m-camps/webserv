//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

void Respond::buildRedir(void)
{
    std::string LocationName = _location.getName();
    std::string NewLocation;
    if (LocationName == "/")
        NewLocation = Generator::generateLocation(_location.getIndex());
    else
        NewLocation = Generator::generateLocation(LocationName + "/" + _location.getIndex());

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

bool	Respond::correctCgiRequestAllowed(void)
{
    std::string cgiExt = _location.getCgiFileExtension();
    std::string cgiName = _location.getCgiName();

    if (cgiName != "" && cgiExt != "")
    {
        return (true);
    }
	return (false);
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

        if (correctCgiRequestAllowed() == true)
        {
            Cgi	cgi;
            FileContent = cgi.executeScript(*this);
            createResponse(FileContent);
            return ;
        }
        parsePath(Path);
        relativePath = Root + Path;
        modifyStatuscode(Path, relativePath); // Change name
        FileContent = getValidFile(relativePath);
        createResponse(FileContent);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Internal Server Error: " << e.what() << std::endl;
        _status_code = e_InternalServerError;
        createResponse(Generator::generateDefaulPage(_status_code));
    }
}
