/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:27:41 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/27 17:52:42 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server() : _port(0), _host(0), _clientMaxBodySize(0)
{

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

in_addr_t Server::getHost(void)
{
	return (_host);
}

uint16_t Server::getPort(void)
{
	return (_port);
}

static bool isHostValid(std::string & param)
{
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	int result = getaddrinfo(param.c_str(), nullptr, &hints, &res);
	freeaddrinfo(res);
	if (!result)
		return (false);
	else
		return (true);
}

void Server::setHost(std::string & param)
{
	checkParamToken(param);
	if (param == "localhost")
		param = "127.0.0.1";
	if (isHostValid(param) == false)
		throw ServerErrorException("Error: Invalid syntax for host");
	
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