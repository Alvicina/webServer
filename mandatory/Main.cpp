/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:31:25 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/03 14:54:58 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"
#include "../includes/FileChecker.hpp"
#include "../includes/FileParser.hpp"
#include "../includes/Location.hpp"
#include "../includes/Server.hpp"

std::string fileCheckAndRead(std::string const & file)
{
	FileChecker configFile(file);
	std::string content;
	
	configFile.getTypeOfFile(configFile.getPath());
	configFile.getAccess();
	content = configFile.readFile(file);
	if (content.empty())
		throw ParserErrorException("Config file is empty");
	//std::cout << "Config file content: " << std::endl << content << std::endl;
	return (content);
}

void parse(std::string content)
{
	FileParser	serverConf(content);
	serverConf.removeComments();
	serverConf.removeWhitespace();
	//std::cout << "Config file content without comments: " << std::endl << serverConf.getContent() << std::endl;
	serverConf.splitServer();
	/*for (std::vector<std::string>::const_iterator it = serverConf.getConfig().begin(); it != serverConf.getConfig().end(); it++)
	{
		std::cout << *it << std::endl;
	}
	std::cout << serverConf.getNbServers() << std::endl;*/
	serverConf.buildServers();
}
	
int startRoutine(std::string const & file)
{
	std::string content;
	//declarar aqui el server manager y pasarle la lista de servers del fileparser desde aqui
	
	content = fileCheckAndRead(file);
	parse(content);
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
