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

}
