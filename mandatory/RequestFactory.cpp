/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:04:21 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 13:43:59 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestFactory.hpp"

RequestFactory::RequestFactory()
{
	
}

RequestFactory::~RequestFactory()
{

}

RequestFactory::RequestFactory(RequestFactory & copy)
{
	*this = copy;
}

RequestFactory& RequestFactory::operator=(RequestFactory & other)
{
	return (*this);	
}

RequestHandler* RequestFactory::makeRequestHandler(Request & request)
{
	switch(request.getMethod())
	{
		case GET:
			return (new RequestHandlerGet(request));
		case POST:
			return (new RequestHandlerPost(request));
		case DELETE:
			return (new RequestHandlerDelete(request));
		default:
			return (new RequestHandlerInvalid(request));
	}
}