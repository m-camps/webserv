//
// Created by Xander Voorvaart on 10/20/22.
//

#include <string>

void deleteFile(std::string& filePath)
{
    int32_t result = std::remove(filePath.c_str());
    if (result != 0)
    {
        throw (std::out_of_range("filePath cannot be removed"));
    }
}
