//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

void Respond::BuildRedir(void)
{
    const std::string NewLocation = getServer().getIndex();

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
        std::string Root = _server.getRoot();
        std::string Path = getEntryFromMap("Path");

        relativePath = Root + Path;
        uint32_t StatusCode = modifyStatusCode(Path, relativePath);
		_status_code = StatusCode;

        FileContent = getValidFile(Root, relativePath, _status_code);

        addToHeader(Generator::generateStatus(*this));
        setBody(CRLF + FileContent);
        addToHeader(Generator::generateContentLength(getBody().length() - 2));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        std::exit(ERROR);
    }
}
