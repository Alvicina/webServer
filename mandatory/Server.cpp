/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:27:41 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/29 12:12:23 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server() 
{
	_port = 0;
	_host = 0;
	_serverName = "";
	_root = "";
	_clientMaxBodySize = MAX_CONTENT_LENGTH;
	_index = "";
	_masterSocket = 0;
	_autoIndex = false;
	initErrorPages();
}

Server::Server(Server const & copy)
{
	*this = copy;	
}

Server::~Server()
{
	
}

Server& Server::operator=(Server const & other)
{
	if (this != &other)
	{
		_port = other._port;
		_host = other._host;
		_serverName = other._serverName;
		_root = other._root;
		_clientMaxBodySize = other._clientMaxBodySize;
		_index = other._index;
		_autoIndex = other._autoIndex;
		_errorPages = other._errorPages;
		_locations = other._locations;
		_serverAddress = other._serverAddress;
		_masterSocket = other._masterSocket;
	}
	return (*this);
}

std::string const & Server::getRoot(void)
{
	return (_root);
}

in_addr_t const & Server::getHost(void)
{
	return (_host);
}

uint16_t const & Server::getPort(void)
{
	return (_port);
}

static in_addr_t isHostValid(std::string & param)
{
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	int result = getaddrinfo(param.c_str(), nullptr, &hints, &res);
	if (result)
		throw ServerErrorException("Error: Invalid syntax for host");
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
	in_addr_t addr = ipv4->sin_addr.s_addr;
	freeaddrinfo(res);
	return(addr);
}

void Server::setHost(std::string & param)
{
	checkParamToken(param);
	if (param == "localhost")
		param = "127.0.0.1";
	_host = isHostValid(param);
}

static bool isRootDirectory(std::string const & param)
{
	struct stat buffer;
	int			status;

	status = stat(param.c_str(), &buffer);
	if (status == 0)
	{
		if (buffer.st_mode & S_IFDIR)
			return (true);
		return (false);
	}
	throw ServerErrorException("Error: could not check root stats");
}

void Server::setRoot(std::string & param)
{
	checkParamToken(param);
	if (isRootDirectory(param) == true)
	{
		_root = param;
		return ;
	}
	char *cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		throw ServerErrorException("Error: could not get cwd");
	std::string newRoot = cwd + param;
	if (isRootDirectory(param) == false)
		throw ServerErrorException("Error: wrong sintax for root");
	_root = newRoot;
}

void Server::setPort(std::string & param)
{
	unsigned int	serverPort = 0;
	size_t			i = 0; 		
	
	checkParamToken(param);
	while (i < param.size())
	{
		if (!isdigit(param[i]))
			throw ServerErrorException("Error: Invalid format for port");
		i++;
	}
	serverPort = utils::stringToInt(param);
	if (serverPort < 1 || serverPort > 65636)
		throw ServerErrorException("Error: Invalid format for port");
	_port = (uint16_t) serverPort;
}

void Server::checkParamToken(std::string & param)
{
	size_t pos = param.rfind(';');
	if (pos != param.size() - 1)
		throw ServerErrorException("Error: Invalid param token");
	param.erase(pos);
}

void Server::initErrorPages(void)
{
	_errorPages[301] = "";
	_errorPages[302] = "";
	_errorPages[400] = "";
	_errorPages[401] = "";
	_errorPages[402] = "";
	_errorPages[403] = "";
	_errorPages[404] = "";
	_errorPages[405] = "";
	_errorPages[406] = "";
	_errorPages[500] = "";
	_errorPages[501] = "";
	_errorPages[502] = "";
	_errorPages[503] = "";
	_errorPages[504] = "";
	_errorPages[505] = "";
}