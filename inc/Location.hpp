/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mcamps <mcamps@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 13:10:52 by mcamps        #+#    #+#                 */
/*   Updated: 2022/10/10 13:15:43 by mcamps        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include <iostream>

class Location{
	private:
		std::string					_root;
		std::string					_index;
		std::vector<std::string>	_methods;
		bool						_autoindex;
		std::string					_cgi_name;
		std::string					_cgi_ext;

	public:
		Location();
		~Location();

		std::string					getRoot() const;
		std::string					getIndex() const;
		std::vector<std::string>	getMethods() const;
		bool						getAutoIndex() const;
		std::string					getCgiName() const;
		std::string					getCgiExt() const;
};
