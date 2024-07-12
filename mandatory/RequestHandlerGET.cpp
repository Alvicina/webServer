/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerGET.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:39:56 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/12 12:41:47 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestHandlerGET.hpp"
#include "../includes/Server.hpp"

RequestHandlerGet::RequestHandlerGet(Request & request) : 
RequestHandler(request)
{
	
}

RequestHandlerGet::~RequestHandlerGet()
{
	
}

RequestHandlerGet::RequestHandlerGet(RequestHandlerGet & copy) : 
RequestHandler(*copy._request)
{
	
}

RequestHandlerGet& RequestHandlerGet::operator=(RequestHandlerGet & other)
{
	if (this != &other)
	{
		_method = other._method;
		_request = other._request;
	}
	return (*this);
}

void RequestHandlerGet::ResponseContentRoutine(Response *response)
{
	std::string pathToResource;
	pathToResource = this->_request->getServer()->getRoot() + this->_request->getUri();
	int typeOfResource = Utils::typeOfFile(pathToResource);
	if (typeOfResource == 1)
	{
		if (access(pathToResource.c_str(), R_OK) == -1)
		{
			delete response;
			throw HandlerErrorException(403, *_request);
		}
		else
		{
			std::ifstream resourse(pathToResource.c_str());
			std::stringstream resourceContent;
			resourceContent << resourse.rdbuf();
			std::string content = resourceContent.str();
			response->setContent(content);
			response->setFile(pathToResource);
		}
	}
	else if (typeOfResource == 2)
	{
		if (access(pathToResource.c_str(), R_OK) == -1)
		{
			delete response;
			throw HandlerErrorException(403, *_request);
		}
		else
		{
			pathToResource = pathToResource + _request->getLocation()->getIndexLocation();
			if (access(pathToResource.c_str(), R_OK) == -1)
			{
				delete response;
				throw HandlerErrorException(403, *_request);
			}
			else
			{
				std::ifstream resourse(pathToResource.c_str());
				std::stringstream resourceContent;
				resourceContent << resourse.rdbuf();
				std::string content = resourceContent.str();
				response->setContent(content);
				response->setFile(pathToResource);
			}
		}
	}
	else if (typeOfResource == -1)
	{
		delete response;
		throw HandlerErrorException(404, *_request);
	}		
}

Response * RequestHandlerGet::doHandleRequest(void)
{
	Response *response = new Response();

	ResponseContentRoutine(response);
	response->setProtocol(_request->getProtocol());
	response->setProtocolVersion(_request->getProtocolVersion());
	response->ResponseHeaderRoutine(*response, *_request);
	response->setStatusCode(200);
	std::string statusCodeMessage = Utils::codeStatus(response->getStatusCode());
	response->setStatusCodeMessage(statusCodeMessage);
	response->ResponseRawRoutine();

	/*int count = 0;
	std::map<std::string, std::string>::iterator it;
	for(it = response->getHeaders().begin(); it != response->getHeaders().end(); it++)
	{
		
		count++;
		std::cout << count << std::endl;
		std::cout << it->first << " " << it->second << std::endl;
	}*/

	return (response);
}