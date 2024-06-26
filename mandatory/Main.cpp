/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:31:25 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/26 17:18:36 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

std::string fileCheckAndRead(std::string const & file)
{
	FileChecker configFile(file);
	std::string content;
	
	if (configFile.getTypeOfFile(configFile.getPath()))
		return (NULL);
	if (configFile.getAccess())
		return (NULL);
	content = configFile.readFile(file);
	if (content.empty())
		return (NULL);
	//std::cout << "Config file content: " << std::endl << content << std::endl;
	return (content);
}

int parse(std::string content)
{
	FileParser	serverConf(content);
	serverConf.removeComments();
	serverConf.removeWhitespace();
	//std::cout << "Config file content without comments: " << std::endl << serverConf.getContent() << std::endl;
	if (serverConf.splitServer() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	for (std::vector<std::string>::const_iterator it = serverConf.getConfig().begin(); it != serverConf.getConfig().end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout << serverConf.getNbServers() << std::endl;
	if (serverConf.buildServers() == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
	
int startRoutine(std::string const & file)
{
	std::string content;
	
	content = fileCheckAndRead(file);
	if (content.empty())
		return (EXIT_FAILURE);
	if (parse(content) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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
