#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.hpp"

class Response
{
	private:
		int _statusCode;
		std::map<std::string, std::string> _headers;
		std::string _raw;
		std::string _content;
		std::string _protocol;
		std::string _protocolVersion;

	public:
		Response();
		Response(const Response &response);
		Response &operator=(const Response &response);
		~Response();

		int getStatusCode();
		void setStatusCode(int statusCode);
		std::map<std::string, std::string> &getHeaders();
		void setHeaders(std::map<std::string, std::string> &headers);
		std::string &getRaw();
		void setRaw(std::string &raw);
		std::string &getContent();
		void setContent(std::string &content);
		std::string &getProtocol();
		void setProtocol(std::string &protocol);
		std::string &getProtocolVersion();
		void setProtocolVersion(std::string &protocolVersion);
};

#endif
