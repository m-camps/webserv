//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

void Respond::BuildGet(void)
{
    std::cout << "GET" << std::endl;
    try
    {
        std::string FileContent;
        std::string relativePath;
        std::string Root = _Exchanger.getServer().getRoot();
        std::string Path = _Exchanger.getHashMapString("Path");

        relativePath = Root + Path;
        uint32_t StatusCode = modifyStatusCode(Path, relativePath);
        _Exchanger.setStatusCode(StatusCode);

        FileContent = getValidFile(Root, relativePath, _Exchanger.getStatusCode());

        Generator::generateStatus(_Exchanger);
        _Exchanger.setBody(FileContent);
        Generator::generateContentLength(_Exchanger, _Exchanger.getBody().length());
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        std::exit(ERROR);
    }
}