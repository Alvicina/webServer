#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserv.hpp"
#include "Server.hpp"
#include "Location.hpp"

class Request
{
	private:
		std::string _raw; 
		std::map<std::string, std::string> _headers;
		std::map<std::string, std::string> _args;
		Methods _method;
		std::string _uri;
		std::string _protocol;
		std::string _protocolVersion;
		Server *_server;
		Location *_location;

	public:
		Request();
		Request(const Request &request);
		Request &operator=(const Request &request);
		~Request();

		std::string &getRaw();
		void setRaw(std::string &raw);
		std::map<std::string, std::string> &getHeaders();
		void setHeaders(std::map<std::string, std::string> &headers);
		std::map<std::string, std::string> &getArgs();
		void setArgs(std::map<std::string, std::string> &args);
		Methods &getMethod();
		void setMethod(Methods &method);
		std::string &getUri();
		void setUri(std::string &uri);
		std::string &getProtocol();
		void setProtocol(std::string &protocol);
		std::string &getProtocolVersion();
		void setProtocolVersion(std::string &protocolVersion);
		Server *getServer();
		void setServer(Server &server);
		Location *getLocation();
		void setLocation(Location &location);
};

#endif
