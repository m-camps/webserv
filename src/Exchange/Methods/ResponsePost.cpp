//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

std::string getFilename(std::string& MetaData)
{
    std::string line;
    std::size_t found = MetaData.find("filename=");
    if (found == std::string::npos)
        throw (std::invalid_argument("File has no name"));

    std::istringstream issFile(MetaData.substr(found + 10, MetaData.length()));
    std::getline(issFile, line);

    line.erase(line.length() - 1, 1);
    return (line);
}

void Respond::putBodyInFile(std::string& MetaData, std::string& Body)
{
    try
    {
        std::string Root = _location.getRoot();
        std::ofstream File(Root + "/" + getFilename(MetaData));

        File << Body;
    }
    catch (const std::exception& e)
    {
        throw (e);
    }
}

std::string Respond::removeBoundry(void)
{
    try
    {
        std::string RequestBody = getEntryFromMap("Body");
        std::string Boundry = Generator::generateBoundry(*this);

        RequestBody.erase(0, Boundry.length() + 5);
        RequestBody.erase(RequestBody.length() - Boundry.length() - 6, Boundry.length() + 6);
        return (RequestBody);
    }
    catch (const std::exception& e)
    {
        throw (std::runtime_error("Could not find boundry"));
    }
}

std::string Respond::parseMetadata(std::string& RequestBody)
{
    std::size_t found;
    std::string MetaData;

    for (int32_t i = 0; i < 3; i++)
    {
        found = RequestBody.find(CRLF);
        MetaData.append(RequestBody, 0, found);
        MetaData += "\n";
        RequestBody.erase(0, found + 2);
    }
    return (MetaData);
}

std::string Respond::sendSuccesfulUpload(std::string MetaData)
{
    std::string Root = _location.getRoot();
    std::string FileName = getFilename(MetaData);
    std::string RelativePath = Root + "/" + FileName;

    return ("Saved upload to file <a href=\"" + FileName + "\">" + RelativePath + "</a> on disk");
}

void Respond::buildPost(void)
{
    std::cout << "POST" << std::endl;
    std::string BodySize = getEntryFromMap("Content-Length");

    try
    {
        if (ToString(_server.getClientBodySize() * 1000000) < BodySize)
        {
            _status_code = e_PayloadTooLarge;
            createResponse(Generator::generateDefaulPage(_status_code));
            return ;
        }
        std::string MetaData;
        std::string Body = getEntryFromMap("Body");

        Body = removeBoundry();
        MetaData = parseMetadata(Body);
        putBodyInFile(MetaData, Body);

        Body.clear();
        Body = sendSuccesfulUpload(MetaData);
        createResponse(Body);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Internal Server Error: " << e.what() << std::endl;
        _status_code = e_InternalServerError;
        createResponse(Generator::generateDefaulPage(_status_code));
    }
}
