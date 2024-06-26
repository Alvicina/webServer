/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:12:35 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 11:03:05 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/FileParser.hpp"

FileParser::FileParser(std::string content) : _content(content)
{
	
}

FileParser::~FileParser()
{
	
}

std::string FileParser::getContent(void)
{
	return (_content);
}

std::vector<std::string> const & FileParser::getConfig(void)
{
	return (_configs);
}

void FileParser::removeComments(void)
{
	size_t commentStart = _content.find("#", 0);
	while (commentStart != std::string::npos)
	{ 
		size_t commentEnd = _content.find("\n", commentStart);
		_content.erase(commentStart, commentEnd - commentStart);
		commentStart = _content.find("#");
	}
}

void FileParser::removeWhitespace(void)
{
	size_t i = 0;

	while (_content[i] && isspace(_content[i]))
		i++;
	_content = _content.substr(i);
	i = _content.length() - 1;
	while (i > 0 && isspace(_content[i]))
		i--;
	_content = _content.substr(0, i + 1);
}

static int getBeginServer(ssize_t startServer, std::string const & content)
{
	size_t i = startServer;
	
	while (content[i])
	{
		if (content[i] == 's')
			break ;
		else if (!isspace(content[i]))
		{
			utils::inputMessage("Error: character out of scope", true);
			return (-1);
		}
		i++;
	}
	/*if (!content[i])
		return (startServer);*/
	if (content.compare(i, 6, "server") != 0)
	{
		utils::inputMessage("Error: character out of scope", true);
		return (-1);
	}
	i = i + 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
	{
		utils::inputMessage("Error: character out of scope", true);
		return (-1);
	}
}

static int getEndServer(ssize_t startServer, std::string const & content)
{
	int 	scope = 0;
	size_t 	i = startServer + 1;
	
	while (content[i])
	{
		if (content[i] == '{')
			scope++;
		else if (content[i] == '}')
		{
			if (!scope)
				return (i);
			scope--;
		}
		i++;
	}
	return (startServer);
}

int FileParser::splitServer(void)
{
	ssize_t startServer = 0;
	ssize_t endServer = 1;

	if (_content.find("server") == std::string::npos)
	{
		utils::inputMessage("Error: No server conf found", true);
		return (EXIT_FAILURE);
	}
	while (startServer != endServer && startServer < (ssize_t)_content.length())
	{
		startServer = getBeginServer(startServer, _content);
		if (startServer == -1)
			return (EXIT_FAILURE);
		endServer = getEndServer(startServer, _content);
		if (startServer == endServer)
		{
			utils::inputMessage("Error: scope problem in conf file", true);
			return (EXIT_FAILURE);
		}
		_configs.push_back(_content.substr(startServer, endServer - startServer + 1));
		_nbServers++;
		startServer = endServer + 1;
	}
	return (EXIT_SUCCESS);
}

