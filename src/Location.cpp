/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:16:03 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/10 13:22:58 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Location.hpp"

// Constructor with default values
Location::Location()
{
	this->_root = "/";
	this->_index = "index.html";
	this->_autoindex = false;
}

Location::~Location(){}

// Basic getters
std::string 				Location::getRoot() const {return this->_root;}
std::string 				Location::getIndex() const {return this->_index;}
std::vector<std::string>	Location::getMethods() const {return this->_methods;}
bool						Location::getAutoIndex() const {return this->_autoindex;}
std::string 				Location::getCgiName() const {return this->_cgi_name;}
std::string 				Location::getCgiExt() const {return this->_cgi_ext;}
