/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerINVALID.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:28:34 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/08 12:46:10 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestHandlerINVALID.hpp"

RequestHandlerInvalid::RequestHandlerInvalid(Request & request) : 
RequestHandler(request)
{
	
}

RequestHandlerInvalid::RequestHandlerInvalid(Request & request, int errorCode) : 
RequestHandler(request)
{
	_errorCode = errorCode;
}

RequestHandlerInvalid::~RequestHandlerInvalid()
{

}

RequestHandlerInvalid::RequestHandlerInvalid(RequestHandlerInvalid & copy) :
RequestHandler(*copy._request)
{
	
}

RequestHandlerInvalid& RequestHandlerInvalid::operator=(RequestHandlerInvalid & other)
{
	if (this != & other)
	{
		_method = other._method;
		_request = other._request;
	}
	return (*this);
}

void RequestHandlerInvalid::ResponseContentFileRoutine(Response & response)
{
	std::map<int, std::string> errorPages = this->_request->getServer()->getErrorPages();
	std::map<int, std::string>::iterator it;

	for (it = errorPages.begin(); it != errorPages.end(); it++)
	{
		if (it->first ==  response.getStatusCode())
		{
			if (it->second.empty())
			{
				std::string content = Utils::codeStatus(it->first);
				response.setContent(content);
				std::string responseFile = "Default";
				response.setFile(responseFile);
				break ;
			}
			else
			{
				std::string path = (this->_request->getServer()->getRoot() + it->second);
				if (access(path.c_str(), F_OK) == -1 && access(path.c_str(), R_OK) == -1)
				{
					std::string content = Utils::codeStatus(it->first);
					response.setContent(content);
					std::string responseFile = "Default";
					response.setFile(responseFile);
					break ;
				}
				else
				{
					std::ifstream ErrorPageOpen(path.c_str());
					/*if (!ErrorPageOpen)
						throw EXCEPCION */
					std::stringstream ErrorPageContent;
					ErrorPageContent << ErrorPageOpen.rdbuf();
					std::string content = ErrorPageContent.str();
					response.setContent(content);
					std::string responseFile = path;
					response.setFile(responseFile);
					break ;
				}
			}
		}
	}
}

void RequestHandlerInvalid::ResponseContentType(Response & response)
{
	std::map<std::string, std::string> ContentType;
	std::string ext = response.getFile(); //.rfind('.', std::string::npos)
	size_t pos = ext.rfind('.', std::string::npos);
	if (pos == std::string::npos)
	{
		std::string extToFind = "default";
		ContentType["Content-type: "] = getExts(extToFind);
		response.setHeaders(ContentType);
	}
	else
	{
		std::string extToFind = ext.substr(pos);
		ContentType["Content-type: "] = getExts(extToFind);
		response.setHeaders(ContentType);
	}
}

void RequestHandlerInvalid::ResponseHeaderRoutine(Response & response)
{
	ResponseContentType(response);
}

Response* RequestHandlerInvalid::handleRequest(void)
{
	Response *invalidResponse = new Response();
	
	if (_errorCode == 0)
		invalidResponse->setStatusCode(405);
	else
		invalidResponse->setStatusCode(_errorCode);
	ResponseContentFileRoutine(*invalidResponse);
	invalidResponse->setProtocol(_request->getProtocol());
	invalidResponse->setProtocolVersion(_request->getProtocolVersion());
	ResponseHeaderRoutine(*invalidResponse);
	
	std::cout << invalidResponse->getStatusCode() << std::endl;
	std::cout << invalidResponse->getContent() << std::endl;
	std::cout << invalidResponse->getFile() << std::endl;
	std::cout << invalidResponse->getProtocol() << std::endl;
	std::cout << invalidResponse->getProtocolVersion() << std::endl;
	
	for (std::map<std::string, std::string>::iterator it = invalidResponse->getHeaders().begin(); 
	it != invalidResponse->getHeaders().end(); it++)
	{
		std::cout << it->first << it->second << std::endl;
	}
	
	
	
	return (invalidResponse);
}