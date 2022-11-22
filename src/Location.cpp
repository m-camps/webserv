/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:16:03 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/22 16:27:50 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"
#include "Constants.hpp"

// Constructor with default values
Location::Location()
{
	_autoindex = -1;
	_return_status = -1;
	_cgi = -1;
}

Location::Location(bool isEmpty)
{
	//isEmpty = true;
	(void)isEmpty;
	_autoindex = 0;
	_cgi = 0;
	_return_status = -1;
	_index = DEFAULT_INDEX;
	_root = DEFAULT_ROOT;
	_name = "/";
	_methods.push_back("POST");
	_methods.push_back("DELETE");
	_methods.push_back("GET");
}

Location::~Location(){} 

/* Getters */
std::string					Location::getName(void) const{ return _name; }
std::string					Location::getRoot(void) const { return _root; }
std::string					Location::getIndex(void) const { return _index; }
int							Location::getAutoIndex(void) const { return _autoindex; }
std::vector<std::string>	Location::getAllowMethods(void) const { return _methods; }
int							Location::getCgi(void) const {return _cgi; }
int							Location::getReturnStatus(void) const { return _return_status; }
std::string					Location::getReturnPath(void) const { return _return_path; }

/* Setters */
void						Location::setName(const std::string& name) { _name = name; }
void						Location::setRoot(const std::string& root) { _root = root; }
void						Location::setIndex(const std::string& index) { _index = index; }
void						Location::setAutoIndex(const int& autoindex) { _autoindex = autoindex; }
void						Location::setCgi(const int& cgi) {_cgi = cgi; }
void						Location::setReturnStatus(const int& status) { _return_status = status; }
void						Location::setReturnPath(const std::string& path) { _return_path = path; }

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
	stream << "\tReturnStatus: [" <<  location.getReturnStatus() << "]\n";
	stream << "\tReturnRedirUrl: [" <<  location.getReturnPath() << "]\n";
	return (stream);
}
