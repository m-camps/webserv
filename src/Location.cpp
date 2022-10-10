/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:16:03 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/10 14:46:55 by mcamps        ########   odam.nl         */
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

/* ACCESSORS */
std::string&	Location::getLocationRoot(void)
{
	return _root;
}

std::string&	Location::getLocationIndex(void)
{
	return _index;
}

bool&		Location::getLocationAutoIndex(void)
{
	return _autoindex;
}

std::vector<std::string>&	Location::getLocationAllowMethods(void)
{
	return _methods;
}

std::string&	Location::getLocationCgiFileExtension(void)
{
	return _cgi_ext;
}

std::string&	Location::getLocationCgiName(void)
{
	return _cgi_name;
}


/* MUTATORS */
void	Location::setLocationIndex(std::string& LocationIndexToAdd)
{
	LocationIndexToAdd.erase(remove(LocationIndexToAdd.begin(), LocationIndexToAdd.end(), ';'), LocationIndexToAdd.end());
	_index = LocationIndexToAdd;
	return ;
}


void	Location::setCgiName(std::string& cgiName)
{
	cgiName.erase(remove(cgiName.begin(), cgiName.end(), ';'), cgiName.end());
	_cgi_name = cgiName;
	return ;
}

void	Location::setCgiFileExtension(std::string& cgiFileExtension)
{
	cgiFileExtension.erase(remove(cgiFileExtension.begin(), cgiFileExtension.end(), ';'), cgiFileExtension.end());
	_cgi_ext = cgiFileExtension;
	return ;
}

void	Location::setLocationRoot(std::string& locationRootToAdd)
{
	locationRootToAdd.erase(remove(locationRootToAdd.begin(), locationRootToAdd.end(), ';'), locationRootToAdd.end());
	_root = locationRootToAdd;
	return ;
}

void	Location::setLocationAllowMethod(std::string& locationAllowedMethods)
{
	char *remainingLine = const_cast<char *>(locationAllowedMethods.c_str());
	char *spaceSeparatedWord = strtok (remainingLine, " ");
	while (spaceSeparatedWord != NULL)
	{
		std::string methodToAdd(spaceSeparatedWord);
		methodToAdd.erase(remove(methodToAdd.begin(), methodToAdd.end(), ';'), methodToAdd.end());
		
		std::vector<std::string>& myvector = _methods;
		std::vector<std::string>::iterator it = myvector.begin();
		it = myvector.insert(it, methodToAdd); //does this work now?
		spaceSeparatedWord = strtok (NULL, " ");
	}
	return ;
}

void	Location::setLocationAutoindex(std::string& AutiondexToSet)
{
	AutiondexToSet.erase(remove(AutiondexToSet.begin(), AutiondexToSet.end(), ';'), AutiondexToSet.end());
	if (AutiondexToSet == "off")
	{
		_autoindex = false;
	}
	else
	{
		_autoindex = true;
	}
	return ;
}

