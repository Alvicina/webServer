/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:19:46 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/01 13:22:18 by alejandro        ###   ########.fr       */
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
		std::vector<int> const & getLocationMethods();
		bool const &			 getAutoIndexLocation();
		std::string const & 	 getIndexLocation();
		std::string const & 	 getReturnLocation();
		std::string const &		 getAliasLocation();
		void	setPath(std::string const & path);
		void	setRootLocation(std::string const & root);
		void	setLocationMethods(std::vector<std::string> & methods);
		void 	setLocationAutoIndex(std::string const & autoIndex);
		void 	setIndexLocation(std::string const & index);
		void 	setReturnLocation(std::string const & Return);
		void 	setAliasLocation(std::string const & alias);
				

		
};

#endif