/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:40:14 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 12:35:06 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <stdint.h>

/*NETWORK*/
#include <netinet/in.h>

/*STL CONTAINERS*/
#include <vector>
#include <map>

/*System*/
#include <sys/stat.h>

#include "Server.hpp"
#include "FileParser.hpp"
#include "FileChecker.hpp"
#include "Location.hpp"


class utils
{
	public:
		static void inputMessage(std::string const & message, bool error);
};

#endif