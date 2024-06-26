/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 12:27:41 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 12:37:05 by alvicina         ###   ########.fr       */
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
		_serverAddress = other._serverAddress;
		_masterSocket = other._masterSocket;
	}
	return (*this);
}