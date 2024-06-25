/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:12:35 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/25 18:16:27 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fileParser.hpp"

fileParser::fileParser(std::string content) : _content(content)
{
	
}

fileParser::~fileParser()
{
	
}

std::string fileParser::getContent(void)
{
	return (_content);
}

void fileParser::removeComments(void)
{
	size_t commentStart = _content.find("#", 0);
	while (commentStart != std::string::npos)
	{ 
		size_t commentEnd = _content.find("\n", commentStart);
		_content.erase(commentStart, commentEnd - commentStart);
		commentStart = _content.find("#");
	}
}

void fileParser::removeWhitespace(void)
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

static int getBeginServer(int startServer, std::string const & content)
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

static int getEndServer(int startServer, std::string const & content)
{
	int scope = 0;
	int i = startServer + 1;
	
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
}

int fileParser::splitServer(void)
{
	int startServer = 0;
	int endServer = 1;

	if (_content.find("server") == std::string::npos)
	{
		utils::inputMessage("Error: No server conf found", true);
		return (EXIT_FAILURE);
	}
	while (startServer != endServer && startServer < _content.length())
	{
		startServer = getBeginServer(startServer, _content);
		endServer = getEndServer(startServer, _content);
	}
	
}

