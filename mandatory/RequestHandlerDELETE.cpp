/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerDELETE.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:58:56 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/08 17:13:23 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestHandlerDELETE.hpp"

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

Response* RequestHandlerDelete::doHandleRequest(void)
{
	Response *response = new Response();
/*	struct stat	attri;
	std::string relativeFilePath = "./files/";

	std::string	fileName = _request->getUri();
	if (fileName[0] == '/' || fileName[0] == '\\')
		fileName.erase(0, 1);
	std::string pathAndFile = relativeFilePath + fileName;

	for (size_t i = 0; i < fileName.size(); i++)
	{
		if (!std::isalpha(fileName[i]) && !std::isdigit(fileName[i]))
		{
			delete response;
			throw HandlerErrorException(403, *_request);
			//std::cerr << "Error 403: Only letters and numbers are allowed in file name: " << fileName << std::endl;
			//return (0);
		}
	}
	if (fileName.size() > MAX_FILENAME_LENGTH)
	{
		delete response;
		throw HandlerErrorException(414, *_request);
		//std::cerr << "Error 414: Filaname too long in: " << fileName << std::endl;
	}
	else if (fileName.find('/') != std::string::npos || fileName.find('\\' || fileName.find("..") != std::string::npos) != std::string::npos)
	{
		delete response;
		throw HandlerErrorException(403, *_request);
		//std::cerr << "Error 403: Path is not allowed in: " << fileName << std::endl;
	}
	else if (stat(pathAndFile.c_str(), &attri) != 0)
	{
		delete response;
		throw HandlerErrorException(404, *_request);
		//std::cerr << "Error 404: Unable to access the file " << pathAndFile << std::endl;
	}
	else if (attri.st_mode & S_IFDIR)
	{
		delete response;
		throw HandlerErrorException(403, *_request);
		//std::cerr << "Error 403: " << pathAndFile << " is a folder, and deleting folders is not allowed." << std::endl;
	}*/

	// ********** Deletes file if possible **********

//	std::string	pathAndFile = '.' + _request->getUri();

	std::string	pathAndFile = strPathAndFile();

	int err = std::remove(pathAndFile.c_str());

	if (err != 0)
	{
		delete response;
		int errnum = this->fileError(pathAndFile);
		throw HandlerErrorException(errnum, *_request);
		//std::cerr << "Error: Unable to delete the file " << pathAndFile << std::endl;
	}
	else
	{
		//std::cout << "El fichero " << pathAndFile << " fue borrado exitosamente.\n";
		std::string content = pathAndFile + " has been successfully deleted!!";  
		response->setContent(content);
		response->setProtocol(_request->getProtocol());
		response->setProtocolVersion(_request->getProtocolVersion());
		response->ResponseHeaderRoutine(*response, _request);
		response->setStatusCode(200);
		std::string statusCodeMessage = Utils::codeStatus(response->getStatusCode());
		response->setStatusCodeMessage(statusCodeMessage);
		response->ResponseRawRoutine();
	}
	return (response);
}

// Considering the fact that an error has happend, returns the type of the error
int RequestHandlerDelete::fileError(std::string pathAndFile)
{
	std::ifstream file(pathAndFile.c_str());
	if (!file.good())
		return (404); // If file/folder doesn't exist, 404 doesn't exist error
	return (403); // If it exist, 403 forbidden error
}

// Returns "path + file" used to delete considering configuration of location and server
std::string	RequestHandlerDelete::strPathAndFile()
{
	std::string path;
	std::string file;
	std::string pathAndFile = _request->getUri();

	if (_request->getLocation()) // if location exists
	{
		std::string::size_type pos = pathAndFile.find_last_of('/');
		if (pos != std::string::npos && pos != pathAndFile.length() - 1)
		{
    	    path = pathAndFile.substr(0, pos + 1);
			file = pathAndFile.substr(pos + 1);
    	}
		else
		{
			path = pathAndFile;
		}

		std::string substPath = _request->getLocation()->getReturnLocation();
		if (!substPath.empty()) // if _return exists in location
		{
			return ("301");
		}
		substPath = _request->getLocation()->getAliasLocation();
		if (!substPath.empty()) // if _alias exists in location
		{
			if (substPath[substPath.size() - 1] == '/' && file[0] == '/')
				file.erase(0, 1);
			if (substPath[0] != '/')
				substPath.insert(0, "/");
			return ('.' + substPath + file);
		}
		substPath = _request->getLocation()->getLocationRoot(); // root of location
		if (!substPath.empty()) // if _root exists in location
		{
			if (substPath[substPath.size() - 1] == '/' && pathAndFile[0] == '/')
				pathAndFile.erase(0, 1);
			if (substPath[0] != '/')
				substPath.insert(0, "/");
			return ('.' + substPath + pathAndFile);
		}
	}
	std::string root = _request->getServer()->getRoot();
	if (root[root.size() - 1] == '/' && pathAndFile[0] == '/')
		pathAndFile.erase(0, 1);
	if (root[0] != '/')
		root.insert(0, "/");
//	std::string prueba = '.' + root + pathAndFile;
	return ('.' + root + pathAndFile); //root of Server class
}
