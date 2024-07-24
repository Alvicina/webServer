/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerPOST.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:52:15 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/08 17:13:33 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestHandlerPOST.hpp"

RequestHandlerPost::RequestHandlerPost(Request & request) : 
RequestHandler(request)
{
	
}

RequestHandlerPost::~RequestHandlerPost()
{
	
}

RequestHandlerPost::RequestHandlerPost(RequestHandlerPost & copy) : 
RequestHandler(*copy._request)
{
	
}

RequestHandlerPost& RequestHandlerPost::operator=(RequestHandlerPost & other)
{
	if (this != &other)
	{
		_method = other._method;
		_request = other._request;
	}
	return (*this);
}

Response * RequestHandlerPost::doHandleRequest(void)
{
	Response	*response = new Response();
	bool		reddir = false;
	bool 		isCgi = false;

	std::cout << "REQUEST" << std::endl << *_request << std::endl;
	std::string content = "probando upload";
	_request->setContent(content);
	std::cout << "content: " << _request->getContent() << std::endl;
	bool isValid = isRequestMethodAllow();
	if (isValid == false)
		throw FactoryErrorException(405, *_request);
	reddir = checkAndSetReturn();
	if (reddir == false)
		checkAndSetAlias();
	isCgi = isCgiRequest(response);
	std::cout << "hola" << std::endl;
	if (isCgi == true)
		doCgi(response);
	response->setProtocol(_request->getProtocol());
	response->setProtocolVersion(_request->getProtocolVersion());
	response->ResponseHeaderRoutine(*response, _request);
	response->setStatusCode(200);
	if (reddir)
		response->setStatusCode(301);
	std::string statusCodeMessage = Utils::codeStatus(response->getStatusCode());
	response->setStatusCodeMessage(statusCodeMessage);
	response->ResponseRawRoutine();
	std::cout << "hola3" << std::endl;
	return (response);
}