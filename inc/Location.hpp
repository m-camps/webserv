/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:10:52 by mcamps        #+#    #+#                 */
/*   Updated: 2022/11/22 16:26:43 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <iostream>
#include <map>

class Location {
	private:

		std::string					_name; 
		std::string					_root;
		std::string					_index; 
		std::vector<std::string>	_methods; 
		int							_autoindex;
		int							_cgi;
		int							_return_status;
		std::string					_return_path;

		//if there is a cginame and cgi extension then its a cgi request
	public:
		Location();
		~Location();
		Location(bool);

		/* Getters */
		std::string					getName(void) const;
		std::string					getRoot(void) const;
		std::string					getIndex(void) const;
		int							getAutoIndex(void) const;
		std::vector<std::string>	getAllowMethods(void) const;
		int							getCgi(void) const;
		int							getReturnStatus(void) const;
		std::string					getReturnPath(void) const;

		/* Setters */
		void						setName(const std::string& name);
		void 						setRoot(const std::string& root);
		void						setIndex(const std::string& index);
		void						setAutoIndex(const int& autoindex);
		void						setCgi(const int& cgi);
		void						setReturnStatus(const int& status);
		void						setReturnPath(const std::string& path);
		
		/* Adders */
		void						addToAllowMethod(const std::string& method);
};

typedef std::map<std::string, Location> Locations;

std::ostream& operator<<(std::ostream& stream, Location& location);
