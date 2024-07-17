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
#include "../includes/webserv.hpp"

int startRoutine(std::string const & file)
{
	try
	{	
		FileChecker configFile(file);
		std::string content;
		content = configFile.checkAndRead();
		FileParser serverConf(content);
		serverConf.parse();
		ServerManager &serverManager = ServerManager::getInstance();
		serverManager.setServers(serverConf.getServers());
		serverManager.serve();
		return (EXIT_SUCCESS);
	}
	catch (std::exception &e)
	{
		Logger::logError(e.what());
		return (EXIT_FAILURE);
	}
}

void handleSIGINT(int signum)
{
	if (signum == SIGINT)
		ServerManager::getInstance().stop();
}

int setEventHandlers()
{
	struct sigaction action;
    action.sa_handler = handleSIGINT;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
	return (sigaction(SIGINT, &action, NULL));
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
	if (setEventHandlers() == -1)
	{
		Logger::logError("Could not setup signal handlers");
		return (EXIT_FAILURE);
	}
	return (startRoutine(file));
}
