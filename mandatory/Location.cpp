/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:37:22 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/27 09:22:45 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"

Location::Location() : _clientMaxBodySize(0)
{
	
}

Location::~Location()
{
	
}

Location::Location(Location const & copy)
{
	*this = copy;
}

Location& Location::operator=(Location const & other)
{
	if (this != &other)
	{
		_path = other._path;
		_root = other._root;
		_autoIndex = other._autoIndex;
		_index = other._index;
		_methods = other._methods;
		_return = other._return;
		_alias = other._alias;
		_cgiPath = other._cgiPath;
		_cgiExt = other._cgiExt;
		_clientMaxBodySize = other._clientMaxBodySize;
	}
	return (*this);
}