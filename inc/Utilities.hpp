/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Utilities.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: xvoorvaa <xvoorvaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/09 15:03:03 by xvoorvaa      #+#    #+#                 */
/*   Updated: 2022/10/09 15:07:34 by xvoorvaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>

std::vector<int> findCharLocation(std::string& line, int32_t c);

std::string readFile(const std::string&);

std::string defaultStatusPage(uint32_t StatusCode);

int64_t getLength(std::ifstream& File);

std::string readFile(const std::istringstream& Body);
