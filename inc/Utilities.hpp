/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utilities.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: xvoorvaa <xvoorvaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/09 15:03:03 by xvoorvaa      #+#    #+#                 */
/*   Updated: 2022/10/19 15:33:18 by xvoorvaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <sstream>

std::vector<int> findCharLocation(std::string& line, int32_t c);

std::string readFile(const std::string&);

std::string defaultStatusPage(uint32_t StatusCode);

int64_t getLength(std::ifstream& File);

std::string readFile(const std::istringstream& Body);

void deleteFile(std::string& filePath);

template <typename T>
std::string ToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return (ss.str());
}
