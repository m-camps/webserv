//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

std::string Respond::ParseBody(void)
{
    try
    {
        std::size_t found;
        std::string ContentFile;
        std::string RequestBody = _Exchanger.getHashMapString("Body");
        std::string Boundry = Generator::generateBoundry(_Exchanger);

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

void Respond::BuildPost(void)
{
    try
    {
        std::string MetaData;
        std::string Body = Map("Body");

        std::cout << "POST" << std::endl;
        Generator::generateStatus(_Exchanger);
        Body = ParseBody();

        putBodyInFile(_MetaData, Body);
        _Exchanger.setBody("");
        Generator::generateContentLength(_Exchanger, 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}