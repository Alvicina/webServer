/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerGET.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:39:56 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/14 13:29:26 by alvicina         ###   ########.fr       */
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
	std::string root = "/" + _request->getServer()->getRoot();
	std::cout << root << std::endl;
	std::cout << _request->getUri() << std::endl;
	
	size_t rootSize = root.size();
	std::string pathToResource;
	
	if ((strncmp(root.c_str(), _request->getUri().c_str(), rootSize)) == 0 || (strncmp(root.c_str(), _request->getUri().c_str(), rootSize -1)) == 0)
	{
		pathToResource = _request->getUri();
		if (pathToResource[0] == '/')
			pathToResource.erase(0, 1);
	}
	else
		pathToResource = this->_request->getServer()->getRoot() + this->_request->getUri();
	int typeOfResource = Utils::typeOfFile(pathToResource);
	std::cout << pathToResource << std::endl;
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
			if (_request->getLocation())
			{
				pathToResource = pathToResource + _request->getLocation()->getIndexLocation();
				if (_request->getLocation()->getAutoIndexLocation())
				{
					int status = response->buildHtmlIndex(*_request);
					if (status)
					{
						delete response;
						throw HandlerErrorException(status, *_request);
					}
					std::string file = "default";
					response->setFile(file);
				}
				else if (access(pathToResource.c_str(), R_OK) == -1)
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
			else
			{
				delete response;
				throw HandlerErrorException(403, *_request);
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
	return (response);
}