/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:10:52 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/31 16:03:50 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <iostream>

class Location {
	private:

		std::string					_name; 
		std::string					_root;
		std::string					_index; 
		std::vector<std::string>	_methods; 
		int							_autoindex;
		std::string					_cgi_name;
		std::string					_cgi_ext;

	public:
		Location();
		~Location();

		/* Getters */
		std::string					getName(void) const;
		std::string					getRoot(void) const;
		std::string					getIndex(void) const;
		int							getAutoIndex(void) const;
		std::vector<std::string>	getAllowMethods(void) const;
		std::string					getCgiFileExtension(void) const;
		std::string					getCgiName(void) const;

		/* Setters */
		void						setName(std::string& name);
		void 						setRoot(std::string& root);
		void						setIndex(std::string& index);
		void						setAutoIndex(int& autoindex);
		void						setCgiName(std::string& cgiName);
		void						setCgiExt(std::string& cgiExt);
		
		/* Adders */
		void						addToAllowMethod(std::string& method);
};

std::ostream& operator<<(std::ostream& stream, Location& location);
