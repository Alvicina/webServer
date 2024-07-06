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

Request &RequestParser::parseRequest()
{
	std::string rawRequest = this->_raw;
	this->parseRequestLine(rawRequest);
	this->parseHeaders(rawRequest);
	this->_request->setContent(rawRequest);
	return (*this->_request);
}

void RequestParser::parseRequestLine(std::string &rawRequest)
{
	size_t separator;

	// TODO: Handle errors when separator is std::string::npos (send HTTP 400 response)
	separator = rawRequest.find(" ");
	this->_request->setMethod(Request::getMethodEnumValue(rawRequest.substr(0, separator)));
	rawRequest = rawRequest.substr(separator + 1);
	separator = rawRequest.find(" ");
	// TODO: Separate URI from URL parameters (?arg1=val1&arg2=val2)
	this->_request->setUri(rawRequest.substr(0, separator));
	rawRequest = rawRequest.substr(separator + 1);
	separator = rawRequest.find("/");
	this->_request->setProtocol(rawRequest.substr(0, separator));
	rawRequest = rawRequest.substr(separator + 1);
	separator = rawRequest.find("\n");
	this->_request->setProtocolVersion(rawRequest.substr(0, separator));
	rawRequest = rawRequest.substr(separator + 1);
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
		headers[line.substr(0, separator)] = line.substr(separator + 2);
		std::getline(stream, line);
	}
	rawRequest = rawRequest.substr(headersLen + 2);
}
