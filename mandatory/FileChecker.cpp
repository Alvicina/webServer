/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileChecker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 12:26:46 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 12:06:03 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

FileChecker::FileChecker(std::string const & path) : _pathToFile(path) //_size(0)
{
	
}

FileChecker::~FileChecker()
{
	
}

FileChecker::FileChecker(FileChecker const & copy) : _pathToFile(copy._pathToFile)
{
	
}

FileChecker& FileChecker::operator=(FileChecker const & other)
{
	if (this != &other)
	{
		this->_pathToFile = other._pathToFile;
	}
	return (*this);
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

int FileChecker::getTypeOfFile(std::string const & path) const
{
	struct stat buffer;

	if (getStats(path.c_str(), &buffer))
		return (EXIT_FAILURE);
	return (0);
}

std::string const & FileChecker::getPath(void)
{
	return (this->_pathToFile);
}

int FileChecker::getAccess(void)
{
	int accessOK = access(this->_pathToFile.c_str(), R_OK);
	if (accessOK == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

std::string FileChecker::readFile(std::string const & path) const
{
	if (path.empty() || path.length() == 0)
	{
		utils::inputMessage("Error: path to config File is empty", true);
		return (NULL);
	}
	std::ifstream configFileOpen(path.c_str());
	if (!configFileOpen)
	{
		utils::inputMessage("Error: Could not open config file", true);
		return (NULL);
	}
	std::stringstream configFileContent;
	configFileContent << configFileOpen.rdbuf();
	return (configFileContent.str());
}