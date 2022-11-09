//
// Created by Xander Voorvaart on 11/7/22.
//

#include "../Respond.hpp"

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
        std::string Root = "data/www"; // Needs to change to location root
        std::ofstream File(Root + "/" + getFilename(MetaData));

        File << Body;
    }
    catch (const std::exception& e)
    {
        throw (e);
    }
}

std::string Respond::ParseBody(void)
{
	std::string ContentFile;

    try
    {
        std::size_t found;
        std::string RequestBody = getEntryFromMap("Body");
        std::string Boundry = Generator::generateBoundry(*this);

        RequestBody.erase(0, Boundry.length() + 5);
        RequestBody.erase(RequestBody.length() - Boundry.length() - 6, Boundry.length() + 6);

        for (int32_t i = 0; i < 3; i++)
        {
            found = RequestBody.find(CRLF);
             _MetaData.append(RequestBody, 0, found);
             _MetaData += "\n";
            RequestBody.erase(0, found + 2);
        }
        ContentFile = RequestBody.substr(0, RequestBody.length());
    }
    catch (const std::exception& e)
    {
        throw (std::logic_error("POST could not be handled"));
    }

    return (ContentFile);
}

std::string Respond::sendSuccesfulUpload(std::string MetaData)
{
    std::string Root = "data/www"; // Needs to change to location root
    std::string FileName = getFilename(MetaData);
    std::string RelativePath = Root + "/" + FileName;

    return ("Saved upload to file <a href=\"" + FileName + "\">" + RelativePath + "</a> on disk");
}

void Respond::buildPost(void)
{
    std::cout << "POST" << std::endl;
    try
    {
        std::string MetaData;
        std::string Body = getEntryFromMap("Body");

        addToHeader(Generator::generateStatus(*this));
        Body = ParseBody();
        putBodyInFile(_MetaData, Body);

        std::string message = sendSuccesfulUpload(_MetaData);

        setBody(message);
        addToHeader(Generator::generateContentLength(message.length()));
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw (e);
    }
}
