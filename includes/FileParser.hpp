/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:04:24 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 11:02:48 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FileParser_HPP
#define FileParser_HPP

#include <iostream>
#include <vector>
#include "../includes/Utils.hpp"

class FileParser
{
	private:
		std::string 				_content;
		std::vector<std::string>	_configs;
		size_t						_nbServers;		
		
	public:
		FileParser(std::string content);
		~FileParser();
		void removeComments(void);
		void removeWhitespace(void);
		std::string getContent(void);
		std::vector<std::string> const & getConfig(void);
		int splitServer(void);
};

#endif