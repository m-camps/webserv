/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:16:03 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/07 12:23:09 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Location.hpp"

// Constructor with default values
Location::Location()
{
	_autoindex = -1;
	_return_status = -1;
}

Location::~Location(){} 

/* Getters */
std::string					Location::getName(void) const{ return _name; }
std::string					Location::getRoot(void) const { return _root; }
std::string					Location::getIndex(void) const { return _index; }
int							Location::getAutoIndex(void) const { return _autoindex; }
std::vector<std::string>	Location::getAllowMethods(void) const { return _methods; }
std::string					Location::getCgiFileExtension(void) const { return _cgi_ext; }
std::string					Location::getCgiName(void) const { return _cgi_name; }
int							Location::getReturnStatus(void) const { return _return_status; }
std::string					Location::getReturnPath(void) const { return _return_path; }

/* Setters */
void						Location::setName(std::string& name) { _name = name; }
void						Location::setRoot(std::string& root) { _root = root; }
void						Location::setIndex(std::string& index) { _index = index; }
void						Location::setAutoIndex(int& autoindex) { _autoindex = autoindex; }
void						Location::setCgiName(std::string& cgiName) { _cgi_name = cgiName; }
void						Location::setCgiExt(std::string& cgiExt) { _cgi_ext = cgiExt; }
void						Location::setReturnStatus(int& status) { _return_status = status; }
void						Location::setReturnPath(std::string& path) { _return_path = path; }

/* Adders */
void 						Location::addToAllowMethod(std::string& method) { _methods.push_back(method); }

std::ostream& operator<<(std::ostream& stream, Location& location)
{
	stream << "LocationName: [" << location.getName() << "]\n";
	stream << "\tRoot: [" <<  location.getRoot() << "]\n";
	stream << "\tIndex: [" <<  location.getIndex() << "]\n";
	stream << "\tAutoIndex: [" <<  location.getAutoIndex() << "]\n";
	stream << "\tMethods: [";
	for (size_t i = 0; i < location.getAllowMethods().size(); i++)
		stream << location.getAllowMethods().at(i) << " ";
	stream << "]\n";
	stream << "\tCgiName: [" <<  location.getCgiName() << "]\n";
	stream << "\tCgiExtension: [" <<  location.getCgiFileExtension() << "]\n";
	return (stream);
}
