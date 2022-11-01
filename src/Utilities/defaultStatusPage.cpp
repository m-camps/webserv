//
// Created by Xander Voorvaart on 17/10/2022.
//

#include <string>
#include "Utilities.hpp"

/*
 * If no page is given for the status-code, it will automatically redirect
 * to this page.
 */
std::string defaultStatusPage(uint32_t StatusCode)
{
	return (
			"<!DOCTYPE html>\n"
			"<body>\n"
			"<h1>ERROR " + ToString(StatusCode) +
			"</h1>\n"
			"</body>\n"
			"</html>"
			);
}

