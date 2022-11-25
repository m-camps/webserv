//
// Created by Xander Voorvaart on 11/7/22.
//

#include "../../../inc/Respond.hpp"

void Respond::buildRedir(void)
{
    std::string NewLocation;
    std::string LocationName = _location.getName();

    if (_location.getReturnPath().empty() == false)
        NewLocation = Generator::generateLocation(_location.getReturnPath());
    else if (LocationName == "/")
        NewLocation = Generator::generateLocation(_location.getIndex());
    else
        NewLocation = Generator::generateLocation(_location.getName() + "/" + _location.getIndex());

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

std::string Respond::parseRelativePath(std::string& Path, std::string& Root)
{
    size_t found = Root.find_last_of('/');
    if (found == std::string::npos || Root.size() - 1 > found)
        Root.append("/");
    _location.setRoot(Root);

    std::string relativePath = Root + Path;
    found = relativePath.find("//");
    if (found != std::string::npos)
        relativePath.erase(found, 1);

    if (Path == "/")
        return (Root);
    return (relativePath);
}

bool    Respond::isPyExtension(const std::string& Path)
{
    size_t found = Path.find(".py");

    return (found != std::string::npos);
}

void Respond::buildGet(void)
{
    try
    {
        std::string FileContent;
        std::string relativePath;
        std::string Root = _location.getRoot();
        std::string Path = getEntryFromMap("Path");

        parsePath(Path);
        relativePath = parseRelativePath(Path, Root);
        modifyStatuscode(Path, relativePath);
        if (_location.getCgi() == true && _status_code == e_OK && isPyExtension(Path) == true)
        {
            Cgi cgi;
            FileContent = cgi.executeScript(*this);
        }
        else
            FileContent = getValidFile(relativePath);
        createResponse(FileContent);
			std::cout << CYAN_COLOR << "GET response: " << _requestData.find("Path")->second << RESET_COLOR << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Internal Server Error: " << e.what() << std::endl;
        _status_code = e_InternalServerError;
        createResponse(Generator::generateDefaulPage(*this));
    }
}
