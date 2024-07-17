/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 10:11:48 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/17 18:25:29 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/CGIHandler.hpp"

CgiHandler::CgiHandler(Request & request)
{
	_request = &request;
}

CgiHandler::~CgiHandler()
{

}

CgiHandler::CgiHandler(CgiHandler & copy)
{
	*this = copy;
}

CgiHandler& CgiHandler::operator=(CgiHandler & other)
{
	if (this != &other)
	{
		_request = other._request;
	}
	return (*this);
}

void CgiHandler::setRequest(Request & request)
{
	_request = &request;
}

Request* CgiHandler::getRequest(void)
{
	return (_request);
}

void CgiHandler::setContent(std::string & content)
{
	_content = content;
}
std::string& CgiHandler::getContent(void)
{
	return (_content);
}

std::string CgiHandler::createPathToResource(void)
{
	std::string root = "/" + _request->getServer()->getRoot();
	size_t rootSize = root.size();
	std::string pathToResource;
	
	if ((strncmp(root.c_str(), _request->getUri().c_str(), rootSize)) == 0
	 || (strncmp(root.c_str(), _request->getUri().c_str(), rootSize -1)) == 0)
	{
		pathToResource = _request->getUri();
		if (pathToResource[0] == '/')
			pathToResource.erase(0, 1);
	}
	else
		pathToResource = this->_request->getServer()->getRoot()
		 + this->_request->getUri();
	return (pathToResource);
}

void CgiHandler::exceptionRoutine(int statusCode, Response *response)
{
	delete response;
	throw HandlerErrorException(statusCode, *_request);
}

void CgiHandler::contentForFile(Response *response, std::string & pathToResource)
{
	if (access(pathToResource.c_str(), R_OK) == -1)
		exceptionRoutine(403, response);
	else
		cgiExecute(response,  pathToResource);
}

void CgiHandler::contentForDIR(Response *response, std::string & pathToResource)
{
	if (access(pathToResource.c_str(), R_OK) == -1)
		exceptionRoutine(403, response);
	else
	{
		if (_request->getLocation())
		{
			pathToResource = pathToResource + "/" + _request->getLocation()->getIndexLocation();
			if (access(pathToResource.c_str(), R_OK) == -1)
				exceptionRoutine(403, response);
			else
				cgiExecute(response, pathToResource);
		}
		else
			exceptionRoutine(403, response);
	}
}

std::string CgiHandler::methodToString(int number)
{
	switch (number)
	{
		case 1:
			return "GET";
		case 2:
			return "POST";
		case 3:
			return "DELETE";
		case 4:
			return "PUT";
		case 5:
			return "HEAD";
		default:
			return "UNKNOWN";
	}
}

std::string CgiHandler::getScriptName(void)
{
	std::string Uri = _request->getUri();
	size_t pos = Uri.find(".");
	if (pos == std::string::npos)
		return (_request->getLocation()->getIndexLocation());
	pos = Uri.find_last_of('/');
	std::string scriptName = Uri.substr(pos);
	return (scriptName);
}

void CgiHandler::initEnv(Response *response, std::string & pathToResource)
{
	(void) response;
	(void) pathToResource;

	_mapEnv["AUTH_TYPE"] = "Basic";
	_mapEnv["CONTENT_LENGTH"] = _request->getHeaders()["Content-length"];
	_mapEnv["CONTENT_TYPE"] = _request->getHeaders()["Content-type"];
	_mapEnv["GATEWAY_INTERFACE"] = "CGI/1.1";
	_mapEnv["SCRIPT_NAME"] = _request->getUri();
	_mapEnv["SCRIPT_FILENAME"] = getScriptName();
	_mapEnv[""];
	
	
	
	
	///

	//_mapEnv[""];
	_mapEnv["REQUEST_METHOD"] = methodToString((int)_request->getMethod());
	_mapEnv["HTTP_COOKIE"] = _request->getHeaders()["Cookie"];
	_mapEnv["DOCUMENT_ROOT"] = _request->getLocation()->getLocationRoot();
	_mapEnv["REQUEST_URI"] = _request->getUri();
	_mapEnv["SERVER_PROTOCOL"] = "HTTP/1.1";
	_mapEnv["REDIRECT_STATURS"] = "200";
	_mapEnv["SERVER_SOFTWARE"] = "WebServer42";
	
	
}

void CgiHandler::cgiExecute(Response *response, std::string & pathToResource)
{
	//initEnv(response, pathToResource);
}

void CgiHandler::handleCgiRequest(Response *response)
{
	std::string pathToResource = createPathToResource();
	int typeOfResource = Utils::typeOfFile(pathToResource);
	if (typeOfResource == 1)
		contentForFile(response, pathToResource);
	else if (typeOfResource == 2)
		contentForDIR(response, pathToResource);
	else if (typeOfResource == -1)
		exceptionRoutine(404, response);
}