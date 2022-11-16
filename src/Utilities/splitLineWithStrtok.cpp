/* ************************************************************ */
/* Created by Xander Voorvaart on 11/16/22.                     */
/* ************************************************************ */

#include <vector>
#include <string>

std::vector<std::string> 	splitLineWithStrtok(std::string& line, const char* delimit)
{
    std::vector<std::string> 	ret;

    char *c_line = const_cast<char *>(line.c_str());
    char *word = strtok(c_line, delimit);
    while (word != NULL)
    {
        ret.push_back(word);
        word = strtok(NULL, delimit);
    }
    return (ret);
}
