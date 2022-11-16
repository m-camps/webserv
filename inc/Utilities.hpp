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

std::string readFile(const std::istringstream& Body);

void deleteFile(std::string& filePath);

bool isDirectory(const std::string& FileName);

size_t ft_strol(const std::string& line);

std::vector<std::string> 	splitLineWithStrtok(std::string& line, const char* delimit);

template <typename T>
std::string ToString(T Number)
{
    std::ostringstream ss;
    ss << Number;
    return (ss.str());
}
