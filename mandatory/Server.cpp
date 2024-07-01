/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:27:41 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/01 11:57:39 by alejandro        ###   ########.fr       */
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

std::string const & Server::getServerName(void)
{
	return (_serverName);
}

unsigned long const & Server::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
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

std::string const & Server::getIndex(void)
{
	return (_index);
}

bool const & Server::getAutoindex(void)
{
	return (_autoIndex);
}

std::vector<Location> const & Server::getLocation(void)
{
	return (_locations);
}

static in_addr_t isHostValid(std::string const & param)
{
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	int result = getaddrinfo(param.c_str(), NULL, &hints, &res);
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

void Server::setServerName(std::string const & params)
{
	_serverName = params;
}

void Server::setIndex(std::string const & params)
{
	_index = params;
}

void Server::setAutoIndex(std::string const & params)
{
	if (params != "on" && params != "off")
		throw ParserErrorException("Error: invalid syntax for autoindex");
	if (params == "on")
		_autoIndex = true;
}

void Server::setClientMaxSize(std::string const & params)
{	
	unsigned long number = 0;
	number = utils::stringToInt(params);
	if (number > MAX_CONTENT_LENGTH)
		throw ParserErrorException("Error: client body size out of bounds");
	_clientMaxBodySize = number;
}

void Server::locationRootRoutine(std::string & locationVars, Location & location)
{
	if (!location.getLocationRoot().empty())
			throw ServerErrorException("Error: location root duplicated");
	this->checkParamToken(locationVars);
	if (isRootDirectory(locationVars) == true)
		location.setRootLocation(locationVars);
	else
		location.setRootLocation(_root + locationVars);
	//std::cout << location.getLocationRoot() << std::endl;
}

void Server::locationMethodsRoutine(std::vector<std::string> & locationVars, size_t & pos, bool & methodsFlag, Location & location)
{
	std::vector<std::string> methods;
	
	if (methodsFlag)
		throw ParserErrorException("Error: methods of location is duplicated");
	while (++pos < locationVars.size())
	{
		if (locationVars[pos].find(";") != std::string::npos)
		{
			checkParamToken(locationVars[pos]);
			methods.push_back(locationVars[pos]);
			break ;
		}
		else
		{
			if ((pos + 1) >= locationVars.size())
				throw ParserErrorException("Error: location method is invalid");
			methods.push_back(locationVars[pos]);
		}
	}
	location.setLocationMethods(methods);
	methodsFlag = true;
}

void Server::locationAutoIndexRoutine(std::string & autoIndex, bool & autoIndexFlag, Location & location)
{
	if (location.getLocationPath() == "/cgi-bin")
		throw ServerErrorException("Error: AutoIndex not allowed for CGI");
	if (autoIndexFlag)
		throw ServerErrorException("Error: Autoindex duplicated in location");
	checkParamToken(autoIndex);
	location.setLocationAutoIndex(autoIndex);
	autoIndexFlag = true;
	//std::cout << location.getAutoIndex() << std::endl;
}

void Server::locationIndexRoutine(std::string & index, Location & location)
{
	if (!location.getIndexLocation().empty())
		throw ServerErrorException("Error: location index duplicated");
	checkParamToken(index);
	location.setIndexLocation(index);
	//std::cout << location.getIndexLocation() << std::endl;
}

void Server::locationExtractionRoutine(std::vector<std::string> & locationVars, size_t & pos, Location & location, bool & methodsFlag, bool & autoIndexFlag)
{
	if (locationVars[pos] == "root" && (pos + 1) < locationVars.size())
		locationRootRoutine(locationVars[++pos], location);
	else if ((locationVars[pos] == "allow_methods" || locationVars[pos] == "methods" ) && (pos + 1) < locationVars.size())
		locationMethodsRoutine(locationVars, pos, methodsFlag, location);
	else if (locationVars[pos] == "autoindex" && (pos + 1) < locationVars.size())
		locationAutoIndexRoutine(locationVars[++pos], autoIndexFlag, location);
	else if (locationVars[pos] == "index" && (pos + 1) < locationVars.size())
		locationIndexRoutine(locationVars[++pos], location);
	
}

void Server::setLocation(std::string & locationPath, std::vector<std::string> & locationVars)
{
	Location location;
	bool 	methodsFlag = false;
	bool	autoIndexFlag = false;
	
	
	location.setPath(locationPath);
	size_t pos = 0;
	while (pos < locationVars.size())
	{
		locationExtractionRoutine(locationVars, pos, location, methodsFlag, autoIndexFlag);
		pos++;
	}
	
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