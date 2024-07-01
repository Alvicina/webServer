/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:37:22 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/01 13:23:20 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"

Location::Location()
{
	_path = "";
	_root = "";
	_autoIndex = false;
	_index = "";
	_return = "";
	_alias = "";
	_clientMaxBodySize = MAX_CONTENT_LENGTH;
	_methods.reserve(5);
	_methods.push_back(1);
	_methods.push_back(0);
	_methods.push_back(0);
	_methods.push_back(0);
	_methods.push_back(0);
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

std::vector<int> const & Location::getLocationMethods()
{
	return (_methods);
}

bool const & Location::getAutoIndexLocation()
{
	return (_autoIndex);
}

std::string const & Location::getIndexLocation()
{
	return (_index);
}

std::string const & Location::getReturnLocation()
{
	return (_return);
}

std::string const & Location::getAliasLocation()
{
	return (_alias);
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

void Location::setLocationMethods(std::vector<std::string> & methods)
{
	for (std::vector<int>::iterator it = _methods.begin(); it != _methods.end(); it++)
		*it = 0;
	for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); it++)
	{
		if (*it == "GET")
			_methods[0] = GET;
		else if (*it == "POST")
			_methods[1] = POST;
		else if (*it == "DELETE")
			_methods[2] = DELETE;
		else if (*it == "PUT")
			_methods[3] = PUT;
		else if (*it == "HEAD")
			_methods[4] = HEAD;
		else
			throw ServerErrorException("Error: method not supported");
	}
	/*std::cout << "---------" << std::endl;
	for (std::vector<int>::iterator it = _methods.begin(); it != _methods.end(); it++)
		std::cout << *it << std::endl;*/
}

void 	Location::setLocationAutoIndex(std::string const & autoIndex)
{
	if (autoIndex != "on" && autoIndex != "off")
		throw ParserErrorException("Error: invalid syntax for autoindex");
	if (autoIndex == "on")
		_autoIndex = true;
}

void Location::setIndexLocation(std::string const & index)
{
	_index = index;
}

void Location::setReturnLocation(std::string const & Return)
{
	_return = Return;
}

void Location::setAliasLocation(std::string const & alias)
{
	_alias = alias;
}




