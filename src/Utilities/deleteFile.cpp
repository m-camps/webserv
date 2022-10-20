//
// Created by Xander Voorvaart on 10/20/22.
//

#include <string>
#include <cstdio>

void deleteFile(std::string& filePath)
{
    int32_t result = std::remove(filePath.c_str());
    if (result != 0)
    {
        std::perror("In remove: ");
        return ; // Throw?
    }
}
