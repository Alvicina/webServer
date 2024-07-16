#ifndef REQUEST_PARSER_HPP
#define REQUEST_PARSER_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "Utils.hpp"

class RequestParser
{
	private:
		std::string _raw;
		Request 	*_request;

		RequestParser();
		RequestParser(const RequestParser &parser);

		void parseRequestLine(std::string &rawRequest);
		void parseUri(std::string uri);
		void parseArgs(std::string args);
		void parseArg(std::string arg);
		void parseHeaders(std::string &rawRequest);
		void setRequestServer(std::vector<Server> &servers);
		void setRequestLocations();

	public:
		RequestParser(std::string &raw);
		RequestParser &operator=(const RequestParser &parser);
		~RequestParser();

		Request &parseRequest(std::vector<Server> &servers);

		class RequestParseErrorException : public std::exception
		{
			public:
				const char *what() const throw()
				{
					return ("The request format is invalid.");
				}
		};
		
};

#endif
