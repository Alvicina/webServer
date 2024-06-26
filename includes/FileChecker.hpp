/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:18:38 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 10:03:39 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FileChecker_HPP
#define FileChecker_HPP

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "../includes/Utils.hpp"

class FileChecker
{
	private:
		std::string const _pathToFile;
		//size_t		_size;
	public:
		FileChecker(std::string const & path);
		~FileChecker();
		int getTypeOfFile(std::string const & path) const;
		int	getAccess(void);
		std::string const & getPath();
		std::string readFile(std::string const & path) const;
};

#endif