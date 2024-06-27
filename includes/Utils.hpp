/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:40:14 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/27 17:45:28 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <climits>
#include <exception>

/*NETWORK*/
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*STL CONTAINERS*/
#include <vector>
#include <map>

/*System*/
#include <sys/stat.h>

class utils
{
	public:
		static void inputMessage(std::string const & message, bool error);
		static int	stringToInt(std::string const & string);
};

class ParserErrorException : public std::exception
{
	private:
		std::string _msg;
	public:
		ParserErrorException(std::string msg) throw();
		virtual const char* what() const throw();
		virtual ~ParserErrorException() throw();
};

class ServerErrorException : public std::exception
{
	private:
		std::string _msg;
	public:
		ServerErrorException(std::string msg) throw();
		virtual const char* what() const throw();
		virtual ~ServerErrorException() throw();
};

#endif