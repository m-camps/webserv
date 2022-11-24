/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:16:03 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/23 15:40:07 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Location.hpp"
#include "../inc/Constants.hpp"

// Constructor with default values
Location::Location()
{
	_autoindex = -1;
	_cgi = -1;
}

Location::Location(bool isEmpty)
{
	(void) isEmpty;

	_autoindex = 0;
	_cgi = 0;
	_index = DEFAULT_INDEX;
	_root = DEFAULT_ROOT;
	_name = "/";
	_methods.push_back("POST");
	_methods.push_back("DELETE");
	_methods.push_back("GET");
	_default_file = DEFAULT_FILE;
}

Location::~Location(){} 

/* Getters */
std::string					Location::getName(void) const{ return _name; }
std::string					Location::getRoot(void) const { return _root; }
std::string					Location::getIndex(void) const { return _index; }
int							Location::getAutoIndex(void) const { return _autoindex; }
std::vector<std::string>	Location::getAllowMethods(void) const { return _methods; }
int							Location::getCgi(void) const { return _cgi; }
std::string					Location::getReturnPath(void) const { return _return_path; }
std::string					Location::getDefaultFile(void) const { return _default_file; }

/* Setters */
void						Location::setName(const std::string& name) { _name = name; }
void						Location::setRoot(const std::string& root) { _root = root; }
void						Location::setIndex(const std::string& index) { _index = index; }
void						Location::setAutoIndex(const int& autoindex) { _autoindex = autoindex; }
void						Location::setCgi(const int& cgi) {_cgi = cgi; }
void						Location::setReturnPath(const std::string& path) { _return_path = path; }
void						Location::setDefaultFile(const std::string& default_file) { _default_file = default_file; }

/* Adders */
void 						Location::addToAllowMethod(const std::string& method) { _methods.push_back(method); }

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
	stream << "\tCgi: [" <<  location.getCgi() << "]\n";
	stream << "\tReturnRedirUrl: [" <<  location.getReturnPath() << "]\n";
	stream << "\tDefaultFile: [" <<  location.getDefaultFile() << "]\n";
	return (stream);
}
