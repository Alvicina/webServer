#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "webserv.hpp"
#include "Request.hpp"

class RequestParser
{
	private:
		std::string _raw;
		Request 	*_request;

		RequestParser();
		RequestParser(const RequestParser &parser);

	public:
		RequestParser(std::string &raw);
		RequestParser &operator=(const RequestParser &parser);
		~RequestParser();

		Request &parseRequest();
};

#endif
