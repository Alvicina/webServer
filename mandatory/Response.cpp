#include "../includes/Response.hpp"

Response::Response() {}

Response::Response(const Response &response)
{
	*this = response;
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

