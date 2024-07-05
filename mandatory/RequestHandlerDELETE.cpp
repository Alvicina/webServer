/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerDELETE.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:58:56 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 13:03:56 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandlerDELETE.hpp"

RequestHandlerDelete::RequestHandlerDelete(Request & request) : 
RequestHandler(request)
{
	
}

RequestHandlerDelete::~RequestHandlerDelete()
{
	
}

RequestHandlerDelete::RequestHandlerDelete(RequestHandlerDelete & copy) : 
RequestHandler(*copy._request)
{
	
}

RequestHandlerDelete& RequestHandlerDelete::operator=(RequestHandlerDelete & other)
{
	if (this != &other)
	{
		_method = other._method;
		_request = other._request;
	}
	return (*this);
}

Response* RequestHandlerDelete::handleRequest(void)
{
	return (new Response());
}