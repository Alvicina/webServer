#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "webserv.hpp"
#include "Request.hpp"

class Response
{
	private:
		int _statusCode;
		std::map<std::string, std::string> _headers;
		std::string _raw;
		std::string _content;
		std::string _file;
		std::string _protocol;
		std::string _protocolVersion;
		std::map<std::string, std::string> _exts;

	public:
		Response();
		Response(int errCode, Request *request);
		Response(const Response &response);
		Response &operator=(const Response &response);
		~Response();

		void initFileExt();
		std::string & getFileExt(std::string & ext);
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
		std::string &getFile();
		void setFile(std::string & file);
};

#endif
