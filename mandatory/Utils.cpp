/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:41:57 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/01 17:56:39 by alvicina         ###   ########.fr       */
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

