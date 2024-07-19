/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:03:13 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/19 10:21:40 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestHandler.hpp"

RequestHandler::RequestHandler(Request & request) : _method(request.getMethod()),
_request(&request), _errorCode(0)
{
	_exts[".html"] = "text/html";
    _exts[".htm"] = "text/html";
    _exts[".css"] = "text/css";
    _exts[".ico"] = "image/x-icon";
    _exts[".avi"] = "video/x-msvideo";
    _exts[".bmp"] = "image/bmp";
    _exts[".doc"] = "application/msword";
    _exts[".gif"] = "image/gif";
    _exts[".gz"] = "application/x-gzip";
    _exts[".ico"] = "image/x-icon";
    _exts[".jpg"] = "image/jpeg";
    _exts[".jpeg"] = "image/jpeg";
    _exts[".png"] = "image/png";
    _exts[".txt"] = "text/plain";
    _exts[".mp3"] = "audio/mp3";
    _exts[".pdf"] = "application/pdf";
    _exts["default"] = "text/html";
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

Methods& RequestHandler::getMethods(void)
{
	return (_method);
}

Request* RequestHandler::getRequest(void)
{
	return (_request);
}

std::string & RequestHandler::getExts(std::string & ext)
{
	if (_exts.count(ext))
		return (_exts[ext]);
	return (_exts["default"]);
}

Response* RequestHandler::handleRequest()
{
    try
    {
        Response* response = doHandleRequest();
        return (response);
    }
    catch(HandlerErrorException &e)
    {
        Response* errorResponse = e.createResponse();
        return (errorResponse);
    }
}

bool RequestHandler::isRequestMethodAllow(Request & request)
{
	if (request.getLocation())
	{
		std::vector<int>::iterator it;
		std::vector<int> methods = request.getLocation()->getLocationMethods();
		for (it = methods.begin(); it != methods.end(); it++)
		{
			if (request.getMethod() == *it)
				return (true);
		}
		return (false);
	}
	return (true);
}

HandlerErrorException::HandlerErrorException(int errCode, Request & request) throw()
{
    _errCode = errCode;
    _request = &request;
}

Response* HandlerErrorException::createResponse() const throw()
{
    return (new Response(_errCode, _request));
}

HandlerErrorException::~HandlerErrorException(void) throw()
{
    
}

int & HandlerErrorException::getErrCode(void)
{
    return (_errCode);
}

