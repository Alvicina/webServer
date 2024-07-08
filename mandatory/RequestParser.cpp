#include "../includes/RequestParser.hpp"

RequestParser::RequestParser(): _request(new Request()) {}

RequestParser::RequestParser(std::string &raw):
	_raw(raw), _request(new Request()) {}

RequestParser::RequestParser(const RequestParser &parser)
{
	*this = parser;
}

RequestParser &RequestParser::operator=(const RequestParser &parser)
{
	if (this != &parser)
	{
		this->_raw = parser._raw;
		*this->_request = *parser._request;
	}
	return (*this);
}

RequestParser::~RequestParser()
{
	delete this->_request;
}

Request &RequestParser::parseRequest(std::vector<Server> &servers)
{
	std::string rawRequest = this->_raw;
	this->parseRequestLine(rawRequest);
	this->parseHeaders(rawRequest);
	this->_request->setContent(rawRequest);
	this->setRequestServer(servers);
	return (*this->_request);
}

void RequestParser::parseRequestLine(std::string &rawRequest)
{
	size_t separator;

	separator = rawRequest.find(" ");
	if (separator == std::string::npos)
		throw RequestParseErrorException();
	this->_request->setMethod(Request::getMethodEnumValue(rawRequest.substr(0, separator)));
	rawRequest = rawRequest.substr(separator + 1);
	separator = rawRequest.find(" ");
	if (separator == std::string::npos)
		throw RequestParseErrorException();
	this->parseUri(rawRequest.substr(0, separator));
	rawRequest = rawRequest.substr(separator + 1);
	separator = rawRequest.find("/");
	if (separator == std::string::npos)
		throw RequestParseErrorException();
	this->_request->setProtocol(rawRequest.substr(0, separator));
	rawRequest = rawRequest.substr(separator + 1);
	separator = rawRequest.find("\n");
	if (separator == std::string::npos)
		throw RequestParseErrorException();
	this->_request->setProtocolVersion(rawRequest.substr(0, separator));
	rawRequest = rawRequest.substr(separator + 1);
}

void RequestParser::parseUri(std::string uri)
{
	size_t separator;

	separator = uri.find("?");
	if (separator == std::string::npos)
	{
		this->_request->setUri(uri);
	}
	else
	{
		this->_request->setUri(uri.substr(0, separator));
		this->parseArgs(uri.substr(separator + 1));
	}
}

void RequestParser::parseArgs(std::string args)
{
	size_t argSeparator;

	argSeparator = args.find("&");
	while (argSeparator != std::string::npos)
	{
		this->parseArg(args.substr(0, argSeparator));
		args = args.substr(argSeparator + 1);
		argSeparator = args.find("&");
	}
	this->parseArg(args);
}

void RequestParser::parseArg(std::string arg)
{
	std::string key;
	std::string value;
	size_t keyValueSeparator;

	keyValueSeparator = arg.find("=");
	if (keyValueSeparator == std::string::npos)
		throw RequestParseErrorException();
	key = arg.substr(0, keyValueSeparator);
	value = arg.substr(keyValueSeparator + 1);
	this->_request->getArgs()[key] = value;
}

void RequestParser::parseHeaders(std::string &rawRequest)
{
	std::istringstream stream(rawRequest);
	std::string line;
	size_t headersLen = 0;
	std::map<std::string, std::string> &headers = this->_request->getHeaders();

	std::getline(stream, line);
	while (line.size() != 1 || !std::isspace(line[0]))
	{
		headersLen += line.size() + 1;
		size_t separator = line.find(": ");
		if (separator == std::string::npos)
			throw RequestParseErrorException();
		headers[line.substr(0, separator)] = line.substr(separator + 2);
		std::getline(stream, line);
	}
	rawRequest = rawRequest.substr(headersLen + 2);
}

void RequestParser::setRequestServer(std::vector<Server> &servers)
{
	std::string requestHost;
	size_t hostPortSeparator;
	int requestPort;

	requestHost = this->_request->getHeaders()["Host"];
	hostPortSeparator = requestHost.find(":");
	if (hostPortSeparator == std::string::npos)
		requestPort = 80;
	else
		requestPort = std::atoi(requestHost.substr(hostPortSeparator + 1).c_str());
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].getPort() == requestPort)
			this->_request->setServer(servers[i]);
	}
}
