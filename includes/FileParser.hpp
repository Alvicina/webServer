/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:04:24 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/27 10:46:31 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FileParser_HPP
#define FileParser_HPP

#include "Utils.hpp"
#include "Server.hpp"

class FileParser
{
	private:
		std::string 				_content;
		std::vector<std::string>	_configs;
		std::vector<Server>			_servers;
		size_t						_nbServers;		
		
	public:
		FileParser(std::string content);
		FileParser(FileParser const & copy);
		~FileParser();
		FileParser& operator=(FileParser const & other);
		void	removeComments(void);
		void 	removeWhitespace(void);
		std::string getContent(void);
		std::vector<std::string> const & getConfig(void);
		size_t getNbServers(void);
		void 	splitServer(void);
		void	buildServers(void);
};

#endif