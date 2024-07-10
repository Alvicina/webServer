#include "../includes/Request.hpp"
#include "../includes/Server.hpp"

std::map<Methods, std::string> Request::_methodDict;

Request::Request(): _server(NULL), _location(NULL) {}

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

std::ostream &operator<<(std::ostream &os, const Request &request)
{
	os << Request::getMethodName(request._method) << " ";
	os << request._uri;
	if (request._args.size() > 0)
	{
		std::map<std::string, std::string>::const_iterator argIt = request._args.begin();
		os << "?" << argIt->first << "=" << argIt->second;
		argIt++;
		while (argIt != request._args.end())
		{
			os << "&" << argIt->first << "=" << argIt->second;
			argIt++;
		}
	}
    os
		<< " " << request._protocol << "/"
		<< request._protocolVersion << std::endl;
	std::map<std::string, std::string>::const_iterator headersIt = request._headers.begin();
	while (headersIt != request._headers.end())
	{
		os << headersIt->first << ": " << headersIt->second << std::endl;
		headersIt++;
	}
	os << std::endl << request._content;
    return os;
}

Methods Request::getMethodEnumValue(const std::string &methodName)
{
	if (Request::_methodDict.size() == 0)
		Request::initMethodDict();
	std::map<Methods, std::string>::iterator it = Request::_methodDict.begin();
	while (it != Request::_methodDict.end())
	{
		if (it->second.compare(methodName) == 0)
			return (it->first);
		it++;
	}
	return (UNKNOWN);
}

void Request::initMethodDict()
{
	Request::_methodDict[GET] = "GET";
	Request::_methodDict[POST] = "POST";
	Request::_methodDict[PUT] = "PUT";
	Request::_methodDict[DELETE] = "DELETE";
	Request::_methodDict[HEAD] = "HEAD";
}

std::string Request::getMethodName(const Methods &method)
{
	if (Request::_methodDict.size() == 0)
		Request::initMethodDict();
	return (Request::_methodDict[method]);
}

std::string &Request::getRaw()
{
	return (this->_raw);
}

void Request::setRaw(const std::string &raw)
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

void Request::setMethod(const Methods &method)
{
	this->_method = method;
}

std::string &Request::getUri()
{
	return (this->_uri);
}

void Request::setUri(const std::string &uri)
{
	this->_uri = uri;
}

std::string &Request::getProtocol()
{
	return (this->_protocol);
}

void Request::setProtocol(const std::string &protocol)
{
	this->_protocol = protocol;
}

std::string &Request::getProtocolVersion()
{
	return (this->_protocolVersion);
}

void Request::setProtocolVersion(const std::string &protocolVersion)
{
	this->_protocolVersion = protocolVersion;
}

std::string &Request::getContent()
{
	return (this->_content);
}

void Request::setContent(const std::string &content)
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
