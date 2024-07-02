/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:41:57 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/02 10:52:14 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

ParserErrorException::ParserErrorException(std::string msg) throw()
{
	_msg = "FileParser " + msg;
}

const char* ParserErrorException::what(void) const throw()
{
	return (_msg.c_str());
}

ParserErrorException::~ParserErrorException(void) throw()
{
	
}

ServerErrorException::ServerErrorException(std::string msg) throw()
{
	_msg = "Server conf " + msg;
}

const char* ServerErrorException::what(void) const throw()
{
	return (_msg.c_str());
}

ServerErrorException::~ServerErrorException(void) throw()
{
	
}

long int	utils::stringToInt(std::string const & string)
{
	long int	number = 0;
	size_t		i = 0;

	while (i < string.length())
	{
		if (!isdigit(string[i]))
			throw std::exception();
		i++;
	}
	std::stringstream ss(string);
	ss >> number;
	/*if (number > INT_MAX)
		throw std::exception();*/
	return (number);
}

void utils::inputMessage(std::string const & message, bool error)
{
	if (error == true)
		std::cerr << message << std::endl;
	else	
		std::cout << message << std::endl;
}

int utils::checkFile(std::string const & path, int mode)
{
	return (access(path.c_str(), mode));
}

int utils::typeOfFile(std::string const & path)
{
	struct stat buff;
	int			type;

	type = stat(path.c_str(), &buff);
	if (type == 0)
	{
		if (buff.st_mode & S_IFREG)
			return (1);
		else if (buff.st_mode & S_IFDIR)
			return (2);
		else
			return (3);
	}
	return (-1);
}

