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
		std::string Root = _location.getRoot();
		std::string Path = getEntryFromMap("Path");

        parsePath(Path);
        relativePath = Root + Path;
        modifyStatuscode(Path, relativePath);
        deleteFile(relativePath);
        createResponse("");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Internal Server Error: " << e.what() << std::endl;
        _status_code = e_InternalServerError;
        createResponse(Generator::generateDefaulPage(_status_code));
    }
}
