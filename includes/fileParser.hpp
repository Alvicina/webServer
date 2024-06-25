/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:04:24 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/25 17:28:10 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#include <iostream>
#include <vector>
#include "../includes/utils.hpp"

class fileParser
{
	private:
		std::string 				_content;
		std::vector<std::string>	_configs;
		size_t						_nbServers;		
		
	public:
		fileParser(std::string content);
		~fileParser();
		void removeComments(void);
		void removeWhitespace(void);
		std::string getContent();
		int splitServer(void);
};

#endif