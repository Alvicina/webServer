/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 11:56:49 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/30 19:17:01 by alvicina         ###   ########.fr       */
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
		std::string const & getIndex(void);
		bool const & getAutoindex(void);
		std::vector<Location> const & getLocation();
		void 	setLocation(std::string & locationPath, std::vector<std::string> & locationVars);
		void 	setAutoIndex(std::string const & param);
		void 	setIndex(std::string const & param);
		void 	setRoot(std::string & param);
		void	setPort(std::string & param);
		void 	setHost(std::string & param);
		void 	setServerName(std::string const & param);
		void 	setClientMaxSize(std::string const & param);
		void	checkParamToken(std::string & param);
		void	initErrorPages(void);
		void 	locationRootRoutine(std::string & locationVars, Location & location);
		void 	locationExtractionRoutine(std::vector<std::string> & locationVars, size_t pos, Location & location);
		
		
		
};

#endif