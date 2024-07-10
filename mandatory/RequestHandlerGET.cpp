/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerGET.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:39:56 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/10 12:40:44 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestHandlerGET.hpp"

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
	std::string pathToResource = this->_request->getLocation()->getLocationRoot() +
	this->_request->getLocation()->getLocationPath() + this->_request->getUri();

	int typeOfResource = Utils::typeOfFile(pathToResource);
	(void) typeOfResource;
	(void) response;
	std::cout << pathToResource << std::endl;	
	std::cout << typeOfResource << std::endl;			
}

Response * RequestHandlerGet::doHandleRequest(void)
{
	Response *response = new Response();

	ResponseContentRoutine(response);
	return (response);
}