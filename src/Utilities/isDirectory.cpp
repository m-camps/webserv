/* ************************************************************ */
/* Created by Xander Voorvaart on 11/14/22.                     */
/* ************************************************************ */

#include <string>
#include <sys/stat.h>

bool    isDirectory(const std::string& FileName)
{
    struct stat t_status;

    if (stat(FileName.c_str(), &t_status) == 0)
    {
        if (t_status.st_mode & S_IFDIR)
            return (true);
    }
    return (false);
}
