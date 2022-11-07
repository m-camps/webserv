//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

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

        Generator::generateStatus(*this);
        setBody(FileContent);
        Generator::generateContentLength(*this, getBody().length());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        std::exit(ERROR);
    }
}
