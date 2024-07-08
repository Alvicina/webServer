#include "../includes/Response.hpp"

Response::Response() {}

Response::Response(const Response &response)
{
	*this = response;
}

static void ResponseContentRoutine(Request & request, Response & response)
{
	std::map<int, std::string> errorPages = request.getServer()->getErrorPages();
	std::map<int, std::string>::iterator it;

	for (it = errorPages.begin(); it != errorPages.end(); it++)
	{
		if (it->first == response.getStatusCode())
		{
			if (it->second.empty())
			{
				std::string content = Utils::codeStatus(it->first);
				response.setContent(content);
				break ;
			}
			else
			{
				std::string path = (request.getServer()->getRoot() + it->second);
				if (access(path.c_str(), F_OK) == -1 && access(path.c_str(), R_OK) == -1)
				{
					std::string content = Utils::codeStatus(it->first);
					response.setContent(content);
					break ;
				}
				else
				{
					std::ifstream ErrorPageOpen(path.c_str());
					std::stringstream ErrorPageContent;
					ErrorPageContent << ErrorPageOpen.rdbuf();
					std::string content = ErrorPageContent.str();
					response.setContent(content);
					break ;
				}
			}
		}
	}
}

static void ResponseContentType(Response & response)
{
	std::map<std::string, std::string> ContentType;
	std::string ext = response.getFile(); //.rfind('.', std::string::npos)
	size_t pos = ext.rfind('.', std::string::npos);
	if (pos == std::string::npos)
	{
		std::string extToFind = "default";
		ContentType["Content-type: "] = response.getFileExt(extToFind);
		response.setHeaders(ContentType);
	}
	else
	{
		std::string extToFind = ext.substr(pos);
		ContentType["Content-type: "] = response.getFileExt(extToFind);
		response.setHeaders(ContentType);
	}
}

static void ResponseHeaderRoutine(Response & response, Request & request)
{
	(void) request;
	ResponseContentType(response);
}

void Response::initFileExt()
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

Response::Response(int errCode, Request *request)
{
	initFileExt();
	setStatusCode(errCode);
	ResponseContentRoutine(*request, *this);
	setFile(request->getUri());
	setProtocol(request->getProtocol());
	setProtocolVersion(request->getProtocolVersion());
	ResponseHeaderRoutine(*this, *request);

	std::cout << getStatusCode() << std::endl;
	std::cout << getContent() << std::endl;
	std::cout << getFile() << std::endl;
	std::cout << getProtocol() << std::endl;
	std::cout << getProtocolVersion() << std::endl;
	
	for (std::map<std::string, std::string>::iterator it = getHeaders().begin(); 
	it != getHeaders().end(); it++)
	{
		std::cout << it->first << it->second << std::endl;
	}
}

Response &Response::operator=(const Response &response)
{
	if (this != &response)
	{
		this->_statusCode = response._statusCode;
		this->_headers = response._headers;
		this->_raw = response._raw;
		this->_content = response._content;
		this->_protocol = response._protocol;
		this->_protocolVersion = response._protocolVersion;
	}
	return (*this);
}

Response::~Response() {}

int Response::getStatusCode()
{
	return (this->_statusCode);
}

void Response::setStatusCode(int statusCode)
{
	this->_statusCode = statusCode;
}

std::map<std::string, std::string> &Response::getHeaders()
{
	return (this->_headers);
}

void Response::setHeaders(std::map<std::string, std::string> &headers)
{
	this->_headers = headers;
}

std::string &Response::getRaw()
{
	return (this->_raw);
}

void Response::setRaw(std::string &raw)
{
	this->_raw = raw;
}

std::string &Response::getContent()
{
	return (this->_content);
}

void Response::setContent(std::string &content)
{
	this->_content = content;
}

std::string &Response::getProtocol()
{
	return (this->_protocol);
}

void Response::setProtocol(std::string &protocol)
{
	this->_protocol = protocol;
}

std::string &Response::getProtocolVersion()
{
	return (this->_protocolVersion);
}

void Response::setProtocolVersion(std::string &protocolVersion)
{
	this->_protocolVersion = protocolVersion;
}

std::string &Response::getFile()
{
	return (this->_file);
}

void Response::setFile(std::string & file)
{
	_file = file;
}

std::string & Response::getFileExt(std::string & ext)
{
	if (_exts.count(ext))
		return (_exts[ext]);
	return (_exts["default"]);
}



