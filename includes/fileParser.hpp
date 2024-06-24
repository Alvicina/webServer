/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:04:24 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/24 13:14:57 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#include <iostream>

class fileParser
{
	private:
		std::string _content;

	public:
		fileParser(std::string content);
		~fileParser();
		void removeComments(void);
		void removeWhitespace(void);
		std::string getContent();
};

#endif