/* ************************************************************ */
/* Created by Xander Voorvaart on 11/15/22.                     */
/* ************************************************************ */

#include <sstream>

size_t ft_strol(const std::string& line)
{
    size_t Number;
    std::istringstream(line) >> Number;
    return (Number);
}
