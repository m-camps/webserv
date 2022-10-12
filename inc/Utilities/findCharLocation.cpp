/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   findCharLocation.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: xvoorvaa <xvoorvaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/09 14:57:32 by xvoorvaa      #+#    #+#                 */
/*   Updated: 2022/10/09 15:08:48 by xvoorvaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

std::vector<int> findCharLocation(std::string& line, int32_t c)
{
	std::vector<int> allCharLocations;

	for (uint32_t i = 0; i < line.length(); i++)
	{
		if (line[i] == c)
			allCharLocations.push_back(i);
	}
	return (allCharLocations);
}
