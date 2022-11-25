//
// Created by Xander Voorvaart on 11/7/22.
//

#include "../../../inc/Respond.hpp"

std::string sendSuccesfulDelete(const std::string& relativePath)
{
    return ("Succesfully deleted " + relativePath + " on disk\n");
}

void Respond::buildDelete(void)
{
    try
    {
		std::string relativePath;
		std::string Root = _location.getRoot();
		std::string Path = getEntryFromMap("Path");

        parsePath(Path);
        relativePath = Root + Path;
        if (isDirectory(relativePath) == true)
        {
            _status_code = e_Forbidden;
            createResponse(Generator::generateDefaulPage(*this));
            return ;
        }

        modifyStatuscode(Path, relativePath);
        if (_status_code != e_OK)
        {
            createResponse(Generator::generateDefaulPage(*this));
            return;
        }
        deleteFile(relativePath);
        createResponse(sendSuccesfulDelete(relativePath));
		std::cout << RED_COLOR << "File deleted" << RESET_COLOR << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Internal Server Error: " << e.what() << std::endl;
        _status_code = e_InternalServerError;
        createResponse(Generator::generateDefaulPage(*this));
    }
	
}
