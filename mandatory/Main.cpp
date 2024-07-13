/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:31:25 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/03 17:19:35 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"
#include "../includes/FileChecker.hpp"
#include "../includes/FileParser.hpp"
#include "../includes/Location.hpp"
#include "../includes/Server.hpp"
#include "../includes/ServerManager.hpp"
#include "../includes/Logger.hpp"

int startRoutine(std::string const & file)
{
	try
	{	
		FileChecker configFile(file);
		std::string content;
		content = configFile.checkAndRead();
		FileParser serverConf(content);
		serverConf.parse();
		ServerManager serverManager(serverConf.getServers());
		serverManager.serve();
		return (EXIT_SUCCESS);
	}
	catch (std::exception &e)
	{
		Logger::logError(e.what());
		return (EXIT_FAILURE);
	}
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
		Logger::logError("Wrong number or arguments");
		return (EXIT_FAILURE);
	}
	return (startRoutine(file));
}
