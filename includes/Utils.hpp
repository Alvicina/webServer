/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:40:14 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/03 10:54:09 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "webserv.hpp"

class Utils
{
	public:
		static void inputMessage(std::string const & message, bool error);
		static long int	stringToInt(std::string const & string);
		static int		checkFile(std::string const & path, int mode);
		static int		typeOfFile(std::string const & path);
		static int		fileExistsAndReadable(std::string const & path, std::string const & index);
		static std::string codeStatus(int code);
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