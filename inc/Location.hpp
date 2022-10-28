/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:10:52 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/28 17:41:12 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <iostream>



class Location {
	private:

		std::string					_name; //I DONT HAVE YET EITHER IN THE PARSER location /asdas, MAP STRING, LCOATION INSTANCE
		std::string					_root; //GET
		std::string					_index; //GET
		std::vector<std::string>	_methods; //GET
		bool						_autoindex; //GET
		std::string					_cgi_name; //GET
		std::string					_cgi_ext; //GET

	public:
		Location();
		~Location();

		/* Getters */
		std::string					getName(void) const;
		std::string					getRoot(void) const;
		std::string					getIndex(void) const;
		bool						getAutoIndex(void) const;
		std::vector<std::string>	getAllowMethods(void) const;
		std::string					getCgiFileExtension(void) const;
		std::string					getCgiName(void) const;

		/* Setters */
		void						setName(std::string& name);
		void 						setRoot(std::string& root);
		void						setIndex(std::string& index);
		void						setAutoIndex(bool& autoindex);
		void						setCgiName(std::string& cgiName);
		void						setCgiExt(std::string& cgiExt);
		
		/* Adders */
		void						addToAllowedMethod(std::string& method);
};

std::ostream& operator<<(std::ostream& stream, Location& location);
