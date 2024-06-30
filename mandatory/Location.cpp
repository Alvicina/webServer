/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:37:22 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/30 19:27:02 by alvicina         ###   ########.fr       */
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

std::string const & Location::getLocationPath()
{
	return (_path);
}

		
std::string const & Location::getLocationRoot()
{
	return (_root);
}

void Location::setPath(std::string const & path)
{
	_path = path;
}

static bool isRootDirectory(std::string const & param)
{
	struct stat buffer;
	int			status;

	status = stat(param.c_str(), &buffer);
	if (status == 0)
	{
		if (buffer.st_mode & S_IFDIR)
			return (true);
		return (false);
	}
	throw ServerErrorException("Error: could not check root stats");
}

void Location::setRootLocation(std::string const & root)
{
	if (isRootDirectory(root) == true)
		_root = root;
}



