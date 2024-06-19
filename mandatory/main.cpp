/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:31:25 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/19 13:31:04 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"
#include "../includes/fileChecker.hpp"

int startRoutine(std::string const & file)
{
	fileChecker configFile(file);
	if (configFile.getTypeOfFile(configFile.getPath()))
		return (EXIT_FAILURE);
	if (configFile.getAccess())
		return (EXIT_FAILURE);
	return (0);
}

int main(int argc,  char **argv)
{
	std::string file;
	
	if (argc == 1)
		file = "config/nginx.conf";
	else if (argc == 2)
		file = argv[1];
	else
	{
		utils::inputMessage("Error: Wrong number or arguments", true);
		return (EXIT_FAILURE);
	}
	if (startRoutine(file) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
