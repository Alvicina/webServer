/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:12:35 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/27 17:11:15 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/FileParser.hpp"

FileParser::FileParser(std::string content) : _content(content), _nbServers(0)
{
	
}

FileParser::~FileParser()
{
	
}

FileParser::FileParser(FileParser const & copy) : _content(copy._content), _configs(copy._configs), _nbServers(copy._nbServers)
{
	
}

FileParser& FileParser::operator=(FileParser const & other)
{
	if (this != &other)
	{
		_content = other._content;
		_configs = other._configs;
		_nbServers = other._nbServers;
	}
	return (*this);
}

std::string FileParser::getContent(void)
{
	return (_content);
}

std::vector<std::string> const & FileParser::getConfig(void)
{
	return (_configs);
}

size_t FileParser::getNbServers(void)
{
	return (_nbServers);
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
			throw ParserErrorException("Error: character out of scope");
		i++;
	}
	/*if (!content[i])
		return (startServer);*/
	if (content.compare(i, 6, "server") != 0)
		throw ParserErrorException("Error: character out of scope");
	i = i + 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw ParserErrorException("Error: character out of scope");
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

void FileParser::splitServer(void)
{
	ssize_t startServer = 0;
	ssize_t endServer = 1;

	if (_content.find("server") == std::string::npos)
		throw ParserErrorException("Error: No server conf found");
	while (startServer != endServer && startServer < (ssize_t)_content.length())
	{
		startServer = getBeginServer(startServer, _content);
		endServer = getEndServer(startServer, _content);
		if (startServer == endServer)
			throw ParserErrorException("Error: scope problem in conf file");
		_configs.push_back(_content.substr(startServer, endServer - startServer + 1));
		_nbServers++;
		startServer = endServer + 1;
	}
}

static std::vector<std::string> getParams(std::string separators, std::string conf)
{
	std::vector<std::string>	params;
	std::string::size_type		start = 0;
	std::string::size_type		end = 0;
	
	while (1)
	{
		end = conf.find_first_of(separators, start);
		if (end == std::string::npos)
			break ;
		std::string temp = conf.substr(start, end - start);
		params.push_back(temp);
		std::cout << temp << std::endl;
		start = conf.find_first_not_of(separators, end);
	}	
	return (params);
}

static void portRoutine(std::string & params, Server & serv)
{
	if (serv.getPort())
		throw ServerErrorException("Error: Port duplicated in server conf");
	serv.setPort(params);
}

static void hostRoutine(std::string & params, Server & serv)
{
	if (serv.getHost())
		throw ServerErrorException("Error: Host duplicated");
		
}

static void extractionRoutine(std::vector<std::string> params, Server & serv, size_t pos, int *locationFlag)
{
	if (params[pos] == "listen" && (pos + 1) < params.size() && *locationFlag)
		portRoutine(params[++pos], serv);
	else if (params[pos] == "host" && (pos + 1) < params.size() && *locationFlag)
		hostRoutine(params[++pos], serv);
		
}

static void	setUpServer(Server & serv, std::string & config)
{
	std::vector<std::string> params;
	int	locationFlag = 1;
	
	params = getParams(std::string(" \n\t"), config += ' ');
	if (params.size() < 3)
		throw ParserErrorException("Error: not enough params in server conf");
	size_t i = 0;
	while (i < params.size())
	{
		extractionRoutine(params, serv, i, &locationFlag);
		i++;
	}
}

void FileParser::buildServers(void)
{
	size_t i = 0;

	while (i < _nbServers)
	{
		Server serv;
		setUpServer(serv, _configs[i]);
		_servers.push_back(serv);
		i++;
	}	
}

