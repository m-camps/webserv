/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:10:52 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/12 14:19:09 by mcamps        ########   odam.nl         */
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

		std::string&				getLocationRoot(void);
		std::string&				getLocationIndex(void);
		bool&						getLocationAutoIndex(void);
		std::vector<std::string>&	getLocationAllowMethods(void);
		std::string&				getLocationCgiFileExtension(void);
		std::string&				getLocationCgiName(void);

		//void						setLocationName(std::string& locationRootToAdd); dont have just yet
		 void						setLocationRoot(std::string& locationRootToAdd);
		 void						setLocationIndex(std::string& locationIndexToAdd);
		 void						setLocationAllowMethod(std::string& locationAllowedMethods);
		 void						setLocationAutoindex(std::string& AutiondexToSet);
		 void						setCgiName(std::string& cgiName);
		 void						setCgiFileExtension(std::string& cgiFileExtension);

		/*
		std::vector<t_location>&	getMethods(void);
		std::string					getRoot() const;
		std::string					getIndex() const;
		std::vector<std::string>	getMethods() const;
		bool						getAutoIndex() const;
		std::string					getCgiName() const;
		std::string					getCgiExt() const;
		*/
};

std::ostream& operator<<(std::ostream& stream, Location& location);


typedef void 	(Location::*selectParsingFunctionLocation)(std::string&);
typedef struct dispatchTableLocation
{
	std::string								_name;
	const selectParsingFunctionLocation		&_pointerToLocationParserFunction; //const?
	//void	(Location::*pointerToLocation)(std::string&);
}				t_dispatchTableLocation;


