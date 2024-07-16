/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerGET.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:39:56 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/16 17:59:32 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestHandlerGET.hpp"
#include "../includes/Server.hpp"

RequestHandlerGet::RequestHandlerGet(Request & request) : 
RequestHandler(request)
{
	
}

RequestHandlerGet::~RequestHandlerGet()
{
	
}

RequestHandlerGet::RequestHandlerGet(RequestHandlerGet & copy) : 
RequestHandler(*copy._request)
{
	
}

RequestHandlerGet& RequestHandlerGet::operator=(RequestHandlerGet & other)
{
	if (this != &other)
	{
		_method = other._method;
		_request = other._request;
	}
	return (*this);
}

std::string RequestHandlerGet::createPathToResource()
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
		pathToResource = this->_request->getServer()->getRoot() + this->_request->getUri();
	return (pathToResource);
}

void RequestHandlerGet::exceptionRoutine(int statusCode, Response *response)
{
		delete response;
		throw HandlerErrorException(statusCode, *_request);
}

void RequestHandlerGet::openReadCopyFile(Response *response, std::string & pathToResource)
{
	std::ifstream resourse(pathToResource.c_str());
	std::stringstream resourceContent;
	resourceContent << resourse.rdbuf();
	std::string content = resourceContent.str();
	response->setContent(content);
	response->setFile(pathToResource);
}

void RequestHandlerGet::htmlIndexBuilder(Response * response)
{
	int status = response->buildHtmlIndex(*_request);
	if (status)
		exceptionRoutine(status, response);
	std::string file = "default";
	response->setFile(file);
}

void RequestHandlerGet::contentForDIR(Response * response, std::string & pathToResource)
{
	if (access(pathToResource.c_str(), R_OK) == -1)
		exceptionRoutine(403, response);
	else
	{
		if (_request->getLocation())
		{
			pathToResource = pathToResource + "/" + _request->getLocation()->getIndexLocation();
			if (_request->getLocation()->getAutoIndexLocation())
				htmlIndexBuilder(response);
			else if (access(pathToResource.c_str(), R_OK) == -1)
				exceptionRoutine(403, response);
			else
				openReadCopyFile(response, pathToResource);
		}
		else
			exceptionRoutine(403, response);
	}
}

void RequestHandlerGet::contentForFile(Response *response, std::string & pathToResource)
{
	if (access(pathToResource.c_str(), R_OK) == -1)
		exceptionRoutine(403, response);
	else
		openReadCopyFile(response,  pathToResource);
}

void RequestHandlerGet::ResponseContentRoutine(Response *response)
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

void RequestHandlerGet::setNewLocation(Request & request)
{
	std::vector<Location> &locations = request.getServer()->getLocation();

	for (size_t i = 0; i < locations.size(); i++)
	{
		std::string testDir;
		size_t uriSize;
		size_t locationPathSize;

		uriSize = request.getUri().size();
		locationPathSize = locations[i].getLocationPath().size();
		testDir = request.getUri();
		testDir = testDir.substr(0, locationPathSize);
		if (testDir.compare(locations[i].getLocationPath()) != 0)
			continue;
		if (uriSize > locationPathSize && request.getUri()[locationPathSize] != '/')
			continue;
		Location *requestLocation = request.getLocation();
		if (!requestLocation || requestLocation->getLocationPath().size() < locationPathSize)
			request.setLocation(locations[i]);
	}
}

void RequestHandlerGet::checkAndSetReturn(bool & reddir)
{
	if (_request->getLocation())
	{
		if(!_request->getLocation()->getReturnLocation().empty())
		{
			reddir = true;
		}
	}
}

std::string RequestHandlerGet::createNewUriForAlias(std::string & alias)
{
	std::string root = _request->getServer()->getRoot();
	size_t rootSize = root.size();
	std::string insertToOldUri = alias.substr(rootSize);
	if (insertToOldUri[0] != '/')
		insertToOldUri = "/" + insertToOldUri;
	std::string locationPath = _request->getLocation()->getLocationPath();
	size_t locationPathSize = locationPath.size();
	std::string oldUri = _request->getUri();
	std::string newUri = insertToOldUri + oldUri.erase(0, locationPathSize);
	return (newUri);
}

void RequestHandlerGet::checkAndSetAlias()
{
	if (_request->getLocation())
	{
		if(!_request->getLocation()->getAliasLocation().empty())
		{
			std::string newUri = createNewUriForAlias(_request->getLocation()->getAliasLocation());
			_request->setUri(newUri);
			setNewLocation(*_request);
		}
	}
}

Response * RequestHandlerGet::doHandleRequest(void)
{
	Response	*response = new Response();
	bool		reddir = false;

	checkAndSetAlias();
	checkAndSetReturn(reddir);
	if (reddir == false)
		ResponseContentRoutine(response);
	response->setProtocol(_request->getProtocol());
	response->setProtocolVersion(_request->getProtocolVersion());
	response->ResponseHeaderRoutine(*response, _request);
	response->setStatusCode(200);
	if (reddir)
		response->setStatusCode(301);
	std::string statusCodeMessage = Utils::codeStatus(response->getStatusCode());
	response->setStatusCodeMessage(statusCodeMessage);
	response->ResponseRawRoutine();
	return (response);
}
