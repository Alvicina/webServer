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

Response* RequestHandlerPost::doHandleRequest(void)
{
	return (new Response());
}