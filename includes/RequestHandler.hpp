/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:54:34 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/19 10:20:01 by alvicina         ###   ########.fr       */
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
		bool isRequestMethodAllow(Request & request);
		RequestHandler(Request & request);
		RequestHandler(RequestHandler & copy);
		virtual ~RequestHandler();
		RequestHandler& operator=(RequestHandler & other);
		
		Methods& getMethods(void);
		Request* getRequest(void);
		virtual Response* doHandleRequest(void) = 0;
		Response* handleRequest(void);
		std::string & getExts(std::string & ext);
		
};

class HandlerErrorException : public std::exception
{
	private:
		int	_errCode;
		Request *_request;
	public:
		HandlerErrorException(int errCode, Request & request) throw();
		Response* createResponse() const throw();
		virtual ~HandlerErrorException() throw();
		int &getErrCode();
};

#endif