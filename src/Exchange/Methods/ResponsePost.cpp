//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

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
             _MetaData += RequestBody.substr(0, found) + "\n";
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

void 		Respond::buildPost(void)
{
    std::cout << "POST" << std::endl;
    try
    {
        std::string MetaData;
        std::string Body = getEntryFromMap("Body");

        addToHeader(Generator::generateStatus(*this));
        Body = ParseBody();

        putBodyInFile(MetaData, Body);
        setBody("");
        addToHeader(Generator::generateContentLength(0));
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
