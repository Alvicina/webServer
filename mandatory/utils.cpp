/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:41:57 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/19 11:53:17 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

void utils::inputMessage(std::string const & message, bool error)
{
	if (error == true)
		std::cerr << message << std::endl;
	else	
		std::cout << message << std::endl;
}

