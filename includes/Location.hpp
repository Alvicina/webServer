/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:19:46 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/30 19:20:29 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Utils.hpp"

class Location
{
	private:
		std::string					_path;
		std::string					_root;
		bool						_autoIndex;
		std::string					_index;
		std::vector<int> 			_methods;
		std::string					_return;
		std::string					_alias;
		std::vector<std::string>	_cgiPath;
		std::vector<std::string>	_cgiExt;
		unsigned long				_clientMaxBodySize;
	public:
		Location();
		Location(Location const & copy);
		~Location();
		Location& operator=(Location const & other);
		std::string const & getLocationPath();
		std::string const & getLocationRoot();
		void	setPath(std::string const & path);
		void	setRootLocation(std::string const & root);
		
};

#endif