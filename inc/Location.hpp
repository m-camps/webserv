/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:10:52 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/17 17:48:22 by mcamps        ########   odam.nl         */
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
		bool						_is_cgi_complete;

		//if there is a cginame and cgi extension then its a cgi request
	public:
		Location();
		~Location();

		std::string&				getLocationName(void);
		std::string&				getLocationRoot(void);
		std::string&				getLocationIndex(void);
		bool&						getLocationAutoIndex(void);
		std::vector<std::string>&	getLocationAllowMethods(void);
		std::string&				getLocationCgiFileExtension(void);
		std::string&				getLocationCgiName(void);
		bool						getCgiCompleteBool(void) const;
		
		void						setLocationName(std::string& locationNameToAdd);
		void						setLocationRoot(std::string& locationRootToAdd);
		void						setLocationIndex(std::string& LocationIndexToAdd);
		void						setLocationAllowMethod(std::string& locationAllowedMethods);
		void						setLocationAutoindex(std::string& AutiondexToSet);
		void						setCgiName(std::string& cgiName);
		void						setCgiFileExtension(std::string& cgiFileExtension);
		void						setCgiCompleteBool(bool value);

};

std::ostream& operator<<(std::ostream& stream, Location& location);

typedef struct selectParsing
{
	std::string		_name;
	void			(Location::*pointerToLocation)(std::string&);
}				t_selectLocation;
