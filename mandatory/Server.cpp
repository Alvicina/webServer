/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afidalgo <afidalgo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:27:41 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 18:54:15 by afidalgo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

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
		_masterSocket = other._masterSocket;
	}
	return (*this);
}

const uint16_t Server::getPort(void)
{
	return (_port);
}

int Server::setPort(std::string & param)
{
	unsigned int	serverPort = 0;
	size_t			i = 0; 		
	
	if (checkParamToken(param) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (i < param.size())
	{
		if (!isdigit(param[i]))
		{
			utils::inputMessage("Error: Invalid format for port", true);
		}
	}
	
}

int Server::checkParamToken(std::string & param)
{
	size_t pos = param.rfind(';');
	if (pos != param.size() - 1)
	{
		utils::inputMessage("Error: Invalid param token", true);
		return (EXIT_FAILURE);
	}
	param.erase(pos);
	return (EXIT_FAILURE);
}