/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:18:38 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 12:08:52 by alvicina         ###   ########.fr       */
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
		int getTypeOfFile(std::string const & path) const;
		int	getAccess(void);
		std::string const & getPath();
		std::string readFile(std::string const & path) const;
};

#endif