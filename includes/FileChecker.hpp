/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:18:38 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/02 10:44:22 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FileChecker_HPP
#define FileChecker_HPP

#include "Utils.hpp"

class FileChecker
{
	private:
		std::string _pathToFile;
		//size_t		_size;
	public:
		FileChecker(std::string const & path);
		FileChecker(FileChecker const & copy);
		~FileChecker();
		FileChecker& operator=(FileChecker const & other);
		void getTypeOfFile(std::string const & path) const;
		void	getAccess(void);
		std::string const & getPath();
		std::string readFile(std::string const & path) const;
};

#endif