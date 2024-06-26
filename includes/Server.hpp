/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:56:49 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 12:29:36 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"

class Location;

class Server
{
	private:
		uint16_t					_port;
		in_addr_t					_host;
		std::string					_serverName;
		std::string					_root;
		unsigned long 				_clientMaxBodySize;
		std::string					_index;
		bool						_autoIndex;
		std::map<int, std::string> 	_errorPages;
		std::vector<Location>		_locations;
		struct sockaddr_in			_serverAddress;
		int							_masterSocket;
	public:
		Server();
		Server(Server const & copy);
		~Server();
		Server& operator=(Server const & other);
		
};

#endif