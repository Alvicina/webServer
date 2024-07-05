/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:54:34 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 12:36:52 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"

class RequestHandler
{
	protected:
		Methods		_method;
		Request*	_request; 

	public:
		RequestHandler(Request & request);
		virtual ~RequestHandler();
		RequestHandler(RequestHandler & copy);
		RequestHandler& operator=(RequestHandler & other);
		virtual Response* handleRequest(void) = 0;
};

#endif