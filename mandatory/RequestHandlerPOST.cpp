/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerPOST.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:52:15 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 13:03:13 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandlerPOST.hpp"

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

Response* RequestHandlerPost::handleRequest(void)
{
	return (new Response());
}