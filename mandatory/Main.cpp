/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:31:25 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/03 16:42:03 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"
#include "../includes/FileChecker.hpp"
#include "../includes/FileParser.hpp"
#include "../includes/Location.hpp"
#include "../includes/Server.hpp"

int startRoutine(std::string const & file)
{
	FileChecker configFile(file);
	std::string content;
	content = configFile.checkAndRead();
	FileParser serverConf(content);
	serverConf.parse();
	return (EXIT_SUCCESS);
}

int main(int argc,  char **argv)
{
	std::string file;
	
	if (argc == 1)
		file = "config/default.conf";
	else if (argc == 2)
		file = argv[1];
	else
	{
		utils::inputMessage("Error: Wrong number or arguments", true);
		return (EXIT_FAILURE);
	}
	try
	{
		startRoutine(file);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
