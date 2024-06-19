/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileChecker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:26:46 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/19 13:37:10 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fileChecker.hpp"
#include "../includes/utils.hpp"

fileChecker::fileChecker(std::string const & path) : _pathToFile(path), _size(0)
{
	
}

fileChecker::~fileChecker()
{
	
}

static int checkStats(struct stat *buffer)
{
	if (!(buffer->st_mode & S_IFREG))
	{
		utils::inputMessage("Error: invalid type of file", true);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int getStats(char const *path, struct stat *buffer)
{
	if(stat(path, buffer) == -1)
	{
		utils::inputMessage("Error: could not get stats of file", true);
		return (EXIT_FAILURE);
	}
	if (checkStats(buffer) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int fileChecker::getTypeOfFile(std::string const & path) const
{
	struct stat buffer;

	if (getStats(path.c_str(), &buffer))
		return (EXIT_FAILURE);
	return (0);
}

std::string const & fileChecker::getPath(void)
{
	return (this->_pathToFile);
}

int fileChecker::getAccess(void)
{
	int accessOK = access(this->_pathToFile.c_str(), R_OK);
	if (accessOK == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}