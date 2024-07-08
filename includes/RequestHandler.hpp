/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:54:34 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/08 12:04:10 by alvicina         ###   ########.fr       */
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
		int			_errorCode; 
		std::map<std::string, std::string> _exts;

	public:
		RequestHandler(Request & request);
		RequestHandler(RequestHandler & copy);
		virtual ~RequestHandler();
		RequestHandler& operator=(RequestHandler & other);
		
		Methods& getMethods(void);
		Request* getRequest(void);
		virtual Response* handleRequest(void) = 0;
		std::string & getExts(std::string & ext);
		
};

#endif