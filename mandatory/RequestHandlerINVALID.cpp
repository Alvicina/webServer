/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerINVALID.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:28:34 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 13:33:44 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandlerINVALID.hpp"

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

Response* RequestHandlerInvalid::handleRequest(void)
{
	return (new Response());
}