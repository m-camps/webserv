//
// Created by Xander Voorvaart on 11/7/22.
//

#include "Respond.hpp"

void Respond::buildDelete(void)
{
	std::cout << "DELETE" << std::endl;
    try
    {
		std::string relativePath;
		std::string Root = _server.getRoot();
		std::string Path = getEntryFromMap("Path");

        relativePath = Root + Path;
        _status_code = modifyStatusCode(Path, relativePath);

        deleteFile(relativePath);
        createResponse("");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        throw (e);
    }
}
