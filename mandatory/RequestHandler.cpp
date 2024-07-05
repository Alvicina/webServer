/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:03:13 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 12:16:52 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler(Request & request) : _method(request.getMethod()),
_request(&request)
{
	
}

RequestHandler::~RequestHandler()
{
	
}

RequestHandler::RequestHandler(RequestHandler & copy) : _method(copy._method),
_request(copy._request)
{
	
}

RequestHandler& RequestHandler::operator=(RequestHandler & other)
{
	if (this != &other)
	{
		_method = other._method;
		_request = other._request;
	}
	return (*this);
}