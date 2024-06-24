/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:31:25 by alvicina          #+#    #+#             */
/*   Updated: 2024/06/24 13:15:43 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"
#include "../includes/fileChecker.hpp"
#include "../includes/fileParser.hpp"

std::string fileCheckAndRead(std::string const & file)
{
	fileChecker configFile(file);
	std::string content;
	
	if (configFile.getTypeOfFile(configFile.getPath()))
		return (NULL);
	if (configFile.getAccess())
		return (NULL);
	content = configFile.readFile(file);
	if (content.empty())
		return (NULL);
	std::cout << "Config file content: " << std::endl << content << std::endl;
	return (content);
}

void parse(std::string content)
{
	fileParser	serverConf(content);
	serverConf.removeComments();
	serverConf.removeWhitespace();
	std::cout << "Config file content without comments: " << std::endl << serverConf.getContent() << std::endl;
}
	
int startRoutine(std::string const & file)
{
	std::string content;
	
	content = fileCheckAndRead(file);
	if (content.empty())
		return (EXIT_FAILURE);
	parse(content);
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
