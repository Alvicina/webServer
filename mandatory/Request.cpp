#include "../includes/Request.hpp"

Request::Request() {}

Request::Request(const Request &request)
{
	*this = request;
}

Request &Request::operator=(const Request &request)
{
	if (this != &request)
	{
		this->_raw = request._raw;
		this->_headers = request._headers;
		this->_args = request._args;
		this->_method = request._method;
		this->_uri = request._uri;
		this->_protocol = request._protocol;
		this->_protocolVersion = request._protocolVersion;
		this->_content = request._content;
		this->_server = request._server;
		this->_location = request._location;
	}
	return (*this);
}

Request::~Request() {}

std::string &Request::getRaw()
{
	return (this->_raw);
}

void Request::setRaw(std::string &raw)
{
	this->_raw = raw;
}

std::map<std::string, std::string> &Request::getHeaders()
{
	return (this->_headers);
}

void Request::setHeaders(std::map<std::string, std::string> &headers)
{
	this->_headers = headers;
}

std::map<std::string, std::string> &Request::getArgs()
{
	return (this->_args);
}

void Request::setArgs(std::map<std::string, std::string> &args)
{
	this->_args = args;
}

Methods &Request::getMethod()
{
	return (this->_method);
}

void Request::setMethod(Methods &method)
{
	this->_method = method;
}

std::string &Request::getUri()
{
	return (this->_uri);
}

void Request::setUri(std::string &uri)
{
	this->_uri = uri;
}

std::string &Request::getProtocol()
{
	return (this->_protocol);
}

void Request::setProtocol(std::string &protocol)
{
	this->_protocol = protocol;
}

std::string &Request::getProtocolVersion()
{
	return (this->_protocolVersion);
}

void Request::setProtocolVersion(std::string &protocolVersion)
{
	this->_protocolVersion = protocolVersion;
}

std::string &Request::getContent()
{
	return (this->_content);
}

void Request::setContent(std::string &content)
{
	this->_content = content;
}

Server *Request::getServer()
{
	return (this->_server);
}

void Request::setServer(Server &server)
{
	this->_server = &server;
}

Location *Request::getLocation()
{
	return (this->_location);
}

void Request::setLocation(Location &location)
{
	this->_location = &location;
}
