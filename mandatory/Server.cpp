/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:27:41 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/03 13:09:03 by alejandro        ###   ########.fr       */
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

std::string & Server::getServerName(void)
{
	return (_serverName);
}

long int & Server::getClientMaxBodySize(void)
{
	return (_clientMaxBodySize);
}

std::string & Server::getRoot(void)
{
	return (_root);
}

in_addr_t & Server::getHost(void)
{
	return (_host);
}

uint16_t & Server::getPort(void)
{
	return (_port);
}

std::string & Server::getIndex(void)
{
	return (_index);
}

bool & Server::getAutoindex(void)
{
	return (_autoIndex);
}

std::vector<Location> & Server::getLocation(void)
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

void Server::setHost(std::string param)
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

void Server::setRoot(std::string param)
{
	checkParamToken(param);
	if (isRootDirectory(param) == true)
	{
		_root = param;
		//std::cout << _root << std::endl;
		return ;
	}
	char *cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		throw ServerErrorException("Error: could not get cwd");
	std::string newRoot = cwd + param;
	free(cwd);
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
	long int number = 0;
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

void Server::locationReturnRoutine(std::string & Return, Location & location)
{
	if (location.getLocationPath() == "/cgi-bin")
		throw ServerErrorException("Error: return directive not allowed for CGI");
	if (!location.getReturnLocation().empty())
		throw ServerErrorException("Error: return duplicated in location");
	checkParamToken(Return);
	location.setReturnLocation(Return);
	//std::cout << location.getReturnLocation() << std::endl;
}

void Server::locationAliasRoutine(std::string & alias, Location & location)
{
	if (location.getLocationPath() == "/cgi-bin")
		throw ServerErrorException("Error: alias directive not allowed for CGI");
	if (!location.getAliasLocation().empty())
		throw ServerErrorException("Error: return duplicated in location");
	checkParamToken(alias);
	location.setAliasLocation(alias);
	//std::cout << location.getAliasLocation() << std::endl;
}

void Server::locationCgiExtRoutine(std::vector<std::string> & locationVars, size_t & pos, Location & location)
{
	std::vector<std::string> cgiExt;

	while (++pos < locationVars.size())
	{
		if (locationVars[pos].find(";") != std::string::npos)
		{
			checkParamToken(locationVars[pos]);
			cgiExt.push_back(locationVars[pos]);
			break ;
		}
		else
		{
			if (pos + 1 >= locationVars.size())
				throw ServerErrorException("Error: CgiExt in location is invalid");
			cgiExt.push_back(locationVars[pos]);
		}
	}
	location.setCgiExtensionLocation(cgiExt);
	/*for (std::vector<std::string>::const_iterator it = location.getCgiExtensionLocation().begin(); it != location.getCgiExtensionLocation().end(); it++)
		std::cout << *it << std::endl;*/
}

void Server::locationCgiPathRoutine(std::vector<std::string> & locationVars, size_t & pos, Location & location)
{
	std::vector<std::string> cgiPath;

	while (++pos < locationVars.size())
	{
		if (locationVars[pos].find(";") != std::string::npos)
		{
			checkParamToken(locationVars[pos]);
			cgiPath.push_back(locationVars[pos]);
			break ;
		}
		else
		{
			if ((pos + 1) >= locationVars.size())
				throw ServerErrorException("Error: CgiPath in location is invalid");
			cgiPath.push_back(locationVars[pos]);
		}
	}
	location.setCgiPathLocation(cgiPath);
	/*for (std::vector<std::string>::const_iterator it = location.getCgiPathLocation().begin(); it != location.getCgiPathLocation().end(); it++)
		std::cout << *it << std::endl;*/
}

void Server::locationMaxSizeRoutine(std::string & maxSize, bool & maxSizeFlag, Location & location)
{
	if (maxSizeFlag)
		throw ServerErrorException("Error: Max body of location is duplicated");
	checkParamToken(maxSize);
	location.setMaxBodySizeLocation(maxSize);
	maxSizeFlag = true;
	//std::cout << location.getMaxBodySizeLocation() << std::endl;
}

void Server::locationExtractionRoutine(std::vector<std::string> & locationVars, size_t & pos, Location & location,
bool & methodsFlag, bool & autoIndexFlag, bool & maxSizeFlag)
{
	if (locationVars[pos] == "root" && (pos + 1) < locationVars.size())
		locationRootRoutine(locationVars[++pos], location);
	else if ((locationVars[pos] == "allow_methods" || locationVars[pos] == "methods" ) && (pos + 1) < locationVars.size())
		locationMethodsRoutine(locationVars, pos, methodsFlag, location);
	else if (locationVars[pos] == "autoindex" && (pos + 1) < locationVars.size())
		locationAutoIndexRoutine(locationVars[++pos], autoIndexFlag, location);
	else if (locationVars[pos] == "index" && (pos + 1) < locationVars.size())
		locationIndexRoutine(locationVars[++pos], location);
	else if (locationVars[pos] == "return" && (pos + 1) < locationVars.size())
		locationReturnRoutine(locationVars[++pos], location);
	else if (locationVars[pos] == "alias" && (pos + 1) < locationVars.size())
		locationAliasRoutine(locationVars[++pos], location);
	else if (locationVars[pos] == "cgi_ext" && (pos + 1) < locationVars.size())
		locationCgiExtRoutine(locationVars, pos, location);
	else if (locationVars[pos] == "cgi_path" && (pos + 1) < locationVars.size())
		locationCgiPathRoutine(locationVars, pos, location);
	else if (locationVars[pos] == "client_max_body_size" && (pos + 1) < locationVars.size())
		locationMaxSizeRoutine(locationVars[++pos], maxSizeFlag, location);
	else if (pos < locationVars.size())
		throw ServerErrorException("Error: directive in location is invalid");
}

void Server::checkLocationCgiIndex(Location & location)
{
	std::string path = location.getLocationRoot() + location.getLocationPath() + "/" + location.getIndexLocation();
	if (utils::typeOfFile(path) != 1)
	{
		char *cwd = getcwd(NULL, 0);
		std::string root(cwd);
		free(cwd); 
		location.setRootLocation(root);
		path = root + location.getLocationPath() + "/" + location.getIndexLocation();
	}
	if (path.empty() || utils::typeOfFile(path) != 1 || utils::checkFile(path, R_OK) < 0)
		throw ServerErrorException("Error: CGI not valid");
}

void Server::checkLocationCgiPath(Location & location)
{
	std::vector<std::string>::const_iterator it;
	for (it = location.getCgiPathLocation().begin(); it != location.getCgiPathLocation().end(); it++)
	{
		if (utils::typeOfFile(*it) < 0)
			throw ServerErrorException("Error: Error: CGI not valid");
	}
}

void Server::checkLocationCgiExtension(Location & location)
{
	std::vector<std::string>::const_iterator it;
	std::vector<std::string>::const_iterator itPath;
	for (it = location.getCgiExtensionLocation().begin(); it != location.getCgiExtensionLocation().end(); it++)
	{
		if (*it != ".py" && *it != ".sh" && *it != "*.py" && *it != "*.sh")
			throw ServerErrorException("Error: CGI not valid");
		for (itPath = location.getCgiPathLocation().begin(); itPath != location.getCgiPathLocation().end(); it++)
		{
			
			if (*it == ".py" || *it == "*.py")
			{
				if ((*itPath).find("python") != std::string::npos)
					location._extPath[".py"] = *itPath;
			}
			else if (*it == ".sh" || *it == "*.sh")
			{
				if ((*itPath).find("bash") != std::string::npos)
					location._extPath["sh"] = *itPath;
			}
		}
	}
}

void Server::checkLocationForCGI(Location & location)
{
	if (location.getCgiPathLocation().empty() || location.getCgiExtensionLocation().empty() || location.getIndexLocation().empty())
		throw ServerErrorException("Error: CGI not valid");
	if (utils::checkFile(location.getIndexLocation(), R_OK) < 0)
		checkLocationCgiIndex(location);
	if(location.getCgiPathLocation().size() != location.getCgiExtensionLocation().size())
		throw ServerErrorException("Error: CGI not valid");
	checkLocationCgiPath(location);
	checkLocationCgiExtension(location);
	if (location.getCgiPathLocation().size() != location.getExtPathMap().size())
		throw ServerErrorException("Error: CGI not valid");
}

void Server::isLocationValid(Location & location)
{
	if (location.getLocationPath() == "/cgi-bin")
		checkLocationForCGI(location);
	else
	{
		if (location.getLocationPath()[0] != '/')
			throw ServerErrorException("Error: invalid path in location");
		if (location.getLocationRoot().empty())
			location.setRootLocation(this->_root);
		if (utils::fileExistsAndReadable(location.getLocationRoot() + location.getLocationPath() + '/', location.getIndexLocation()))
		{
			std::cout << "rootloc: " << location.getLocationRoot() << "pathloc: " << location.getLocationPath() << std::endl;
			throw ServerErrorException("Error: Index for location invalid");
		}
		if (!location.getReturnLocation().empty())
		{
			if (utils::fileExistsAndReadable(location.getLocationRoot(), location.getReturnLocation()))
				throw ServerErrorException("Error: Return for location invalid");	
		}
		if (!location.getAliasLocation().empty())
		{
			if (utils::fileExistsAndReadable(location.getLocationRoot(), location.getAliasLocation()))
				throw ServerErrorException("Error: Alias for location invalid");
		}
	}
}

void Server::setLocation(std::string & locationPath, std::vector<std::string> & locationVars)
{
	Location location;
	bool 	methodsFlag = false;
	bool	autoIndexFlag = false;
	bool	maxSizeFlag = false;
	
	
	location.setPath(locationPath);
	size_t pos = 0;
	while (pos < locationVars.size())
	{
		locationExtractionRoutine(locationVars, pos, location, methodsFlag, autoIndexFlag, maxSizeFlag);
		pos++;
	}
	if (location.getLocationPath() != "/cgi-bin" && location.getIndexLocation().empty())
		location.setIndexLocation(this->_index);
	if (!maxSizeFlag)
		location.setMaxBodySizeLocation(this->_clientMaxBodySize);
	isLocationValid(location);
	_locations.push_back(location);
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

bool Server::checkForDuplicateLocation(void)
{
	if (_locations.size() < 2)
		return (false);
		
	std::vector<Location>::iterator itA;
	std::vector<Location>::iterator itB;
	for (itA = _locations.begin(); itA != _locations.end(); itA++)
	{
		for (itB = itA + 1; itB != _locations.end(); itB++)
		{
			if (itA->getLocationPath() == itB->getLocationPath())
				return (true);
		}
	}
	return (false);
}

static void checkErrorPagesForModule(std::vector<std::string> & errorCodes)
{
	if (errorCodes.size() % 2 != 0)
		throw ServerErrorException("Error: invalid Error page directive (module)");
}

static void checkErrorIsAllDigits(std::string const & error)
{
	size_t i = 0;
	while (i < error.size())
	{
		if (!isdigit(error[i]))
			throw ServerErrorException("Error: Error code invalid");
		i++;
	}
}

static void checkErrorNumberDigits(std::string const & error)
{
	if (error.size() != 3)
		throw ServerErrorException("Error: Error code is invalid");
}

static void checkErrorCodeStatus(int & errorToNumber)
{
	if (utils::codeStatus(errorToNumber) == "Undefined" || errorToNumber < 400 || errorToNumber > 599)
		throw ServerErrorException("Error: invalid code for error");
}

void Server::checkErrorPageForFile(std::string & errorPath)
{
	if (utils::typeOfFile(_root + errorPath) != 1)
		throw ServerErrorException("Error: incorrect path for error page file");
	if (utils::checkFile(_root + errorPath, 0) == -1 || utils::checkFile(_root + errorPath, 4) == -1)
		throw ServerErrorException("Error: Error page file not accesible");
}

void Server::setErrorPages(std::vector<std::string> & errorCodes)
{
	if (errorCodes.empty())
		return ;
	checkErrorPagesForModule(errorCodes);
	size_t	i = 0;
	while (i < errorCodes.size() - 1)
	{
		checkErrorIsAllDigits(errorCodes[i]);
		checkErrorNumberDigits(errorCodes[i]);
		int errorToNumber = utils::stringToInt(errorCodes[i]);
		checkErrorCodeStatus(errorToNumber);
		std::string errorPath = errorCodes[++i];
		checkParamToken(errorPath);
		if (utils::typeOfFile(errorPath) == 2)
			return ;
		else
			checkErrorPageForFile(errorPath);
		_errorPages[errorToNumber] = errorPath;
		i++;
	}
}
