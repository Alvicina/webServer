/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerINVALID.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:28:34 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 18:49:15 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestHandlerINVALID.hpp"

RequestHandlerInvalid::RequestHandlerInvalid(Request & request) : 
RequestHandler(request)
{
	
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

void RequestHandlerInvalid::ResponseContentRoutine(Response & response)
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
				break ;
			}
			else
			{
				std::ifstream ErrorPageOpen((this->_request->getServer()->getRoot() + it->second).c_str());
				/*if (!ErrorPageOpen)
					throw EXCEPCION */
				std::stringstream ErrorPageContent;
				ErrorPageContent << ErrorPageOpen.rdbuf();
				std::string content = ErrorPageContent.str();
				response.setContent(content);
				break ;
			}
		}
	}
}

Response* RequestHandlerInvalid::handleRequest(void)
{
	Response *invalidResponse = new Response();

	invalidResponse->setStatusCode(405);
	ResponseContentRoutine(*invalidResponse);
	invalidResponse->setProtocol(_request->getProtocol());
	invalidResponse->setProtocolVersion(_request->getProtocolVersion());
	
	
	//std::cout << invalidResponse->getContent() << std::endl;
	
	
	
	return (invalidResponse);
}