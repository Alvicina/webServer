/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afidalgo <afidalgo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:56:49 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 19:08:43 by afidalgo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include "Socket.hpp"

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
		Socket						_masterSocket;

	public:
		Server();
		Server(Server const & copy);
		~Server();
		Server& operator=(Server const & other);

		void initMasterSocket();

		const	uint16_t getPort();
		int	setPort(std::string & param);
		int		checkParamToken(std::string & param);
		const Socket &getSocket();
};

#endif