/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileChecker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:18:38 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/25 17:25:49 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILECHECKER_HPP
#define FILECHECKER_HPP

#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "../includes/utils.hpp"

class fileChecker
{
	private:
		std::string const _pathToFile;
		//size_t		_size;
	public:
		fileChecker(std::string const & path);
		~fileChecker();
		int getTypeOfFile(std::string const & path) const;
		int	getAccess(void);
		std::string const & getPath();
		std::string readFile(std::string const & path) const;
};

#endif