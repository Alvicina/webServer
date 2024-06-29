/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:56:49 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/29 18:23:17 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include "Location.hpp"

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
		uint16_t const & getPort();
		in_addr_t const & getHost();
		std::string const & getRoot();
		std::string const & getServerName();
		unsigned long const & getClientMaxBodySize(void);
		void 	setRoot(std::string & param);
		void	setPort(std::string & param);
		void 	setHost(std::string & param);
		void 	setServerName(std::string const & param);
		void 	setClientMaxSize(std::string const & param);
		void	checkParamToken(std::string & param);
		void	initErrorPages(void);
		
};

#endif