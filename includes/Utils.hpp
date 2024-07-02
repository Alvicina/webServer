/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:40:14 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/02 16:21:40 by alvicina         ###   ########.fr       */
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
#include <cstring>

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

#define MAX_CONTENT_LENGTH 30000000

enum methods {
	GET = 1,
	POST,
	DELETE,
	PUT,
	HEAD
};

class utils
{
	public:
		static void inputMessage(std::string const & message, bool error);
		static long int	stringToInt(std::string const & string);
		static int		checkFile(std::string const & path, int mode);
		static int		typeOfFile(std::string const & path);
		static int		fileExistsAndReadable(std::string const & path, std::string const & index);
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