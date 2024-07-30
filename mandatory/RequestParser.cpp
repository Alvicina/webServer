#include "../includes/RequestParser.hpp"
#include "../includes/Server.hpp"

RequestParser::RequestParser(): _request(new Request()) {}

RequestParser::RequestParser(std::string &raw, Client *client)
{
	this->_clientSockHost = client->getSocket().getAddress().sin_addr.s_addr;
	this->_request = (client->getRequest()) ? client->getRequest() : new Request();
	this->_request->getRaw().append(raw);
}

RequestParser::RequestParser(const RequestParser &parser)
{
	*this = parser;
}

RequestParser &RequestParser::operator=(const RequestParser &parser)
{
	if (this != &parser)
	{
		this->_request = parser._request;
	}
	return (*this);
}

RequestParser::~RequestParser() {}

Request &RequestParser::parseRequest(std::vector<Server> &servers)
{
	try
	{
		if (this->_request->getRaw().find("\r\n\r\n") == std::string::npos)
			return (*this->_request);
		std::string rawRequest = this->_request->getRaw();
		if (!this->_request->getIsRequestLineProcessed())
			this->parseRequestLine(rawRequest);
		if (!this->_request->getAreHeadersProcessed())
		{
			this->parseHeaders(rawRequest);
			this->setRequestServer(servers);
			this->setRequestLocations();
			this->setPathInfo();
		}
		this->parseContent();
		return (*this->_request);
	}
	catch (std::exception &e)
	{
		throw RequestParseErrorException();
	}
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
	std::string protocolVersion = rawRequest.substr(0, separator);
	size_t removeSpace = protocolVersion.find("\r");
	if (removeSpace != std::string::npos)
		protocolVersion.erase(removeSpace);
	this->_request->setProtocolVersion(protocolVersion);
	rawRequest = rawRequest.substr(separator + 1);
	this->_request->setIsRequestLineProcessed(true);
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
		this->_request->setQueryString(uri.substr(separator + 1));
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
		std::string key = Utils::strToLower(line.substr(0, separator));
		std::string value = line.substr(separator + 2);
		if (value[value.size() - 1] == '\r')
			value.erase(value.size() - 1);
		headers[key] = value;
		std::getline(stream, line);
	}
	rawRequest = rawRequest.substr(headersLen + 2);
	this->_request->setAreHeadersProcessed(true);
}

void RequestParser::parseContent()
{
	std::map<std::string, std::string> &headers = this->_request->getHeaders();
	size_t separator = this->_request->getRaw().find("\r\n\r\n");
	if (separator == std::string::npos)
		throw RequestParseErrorException();
	std::string rawBody = this->_request->getRaw().substr(separator + 4);

	if (headers.find("transfer-encoding") != headers.end() &&
		headers["transfer-encoding"].compare("chunked") == 0)
		this->parseContentWithChunkedEncoding(rawBody);
	else if (headers.find("content-length") != headers.end())
		this->parseContentWithContentLength(rawBody, headers["content-length"]);
	else
		this->_request->setIsComplete(true);
}

void RequestParser::parseContentWithChunkedEncoding(std::string &rawBody)
{
	size_t expectedBytes;
	size_t totalBodyBytes = 0;
	std::string sanitizingStr;

	if (rawBody.find("0\r\n\r\n") == std::string::npos)
		return;

	do
	{
		size_t separator = rawBody.find("\r\n");
		if (separator == std::string::npos)
			throw RequestParseErrorException();
		expectedBytes = Utils::hexToDecimal(rawBody.substr(0, separator));
		totalBodyBytes += expectedBytes;
		if (totalBodyBytes > (size_t) this->_request->getServer()->getClientMaxBodySize())
			throw RequestParseErrorException();
		rawBody = rawBody.substr(separator + 2);
		separator = rawBody.find("\r\n");
		if (separator == std::string::npos)
			throw RequestParseErrorException();
		std::string chunk = rawBody.substr(0, separator);
		if (chunk.size() != expectedBytes)
			throw RequestParseErrorException();
		rawBody = rawBody.substr(separator + 2);
		this->_request->getContent().append(chunk);
	}
	while (expectedBytes != 0);
	this->_request->setIsComplete(true);
}

void RequestParser::parseContentWithContentLength(std::string &rawBody, std::string &contentLength)
{
	std::stringstream ss(contentLength);
	size_t expectedLength;

	ss >> expectedLength;
	if (ss.fail())
		throw RequestParseErrorException();
	if (expectedLength > (size_t) this->_request->getServer()->getClientMaxBodySize())
		throw RequestParseErrorException();
	if (rawBody.size() > expectedLength)
		throw RequestParseErrorException();
	if (rawBody.size() == expectedLength)
	{
		this->_request->setContent(rawBody);
		this->_request->setIsComplete(true);
	}
}

void RequestParser::setRequestServer(std::vector<Server> &servers)
{
	std::string requestHost;
	size_t hostPortSeparator;
	uint16_t requestPort;
	Server *defaultServer = NULL;

	requestHost = this->_request->getHeaders()["host"];
	hostPortSeparator = requestHost.find(":");
	if (hostPortSeparator == std::string::npos)
		requestPort = 80;
	else
		requestPort = std::atoi(requestHost.substr(hostPortSeparator + 1).c_str());
	requestHost = requestHost.substr(0, hostPortSeparator);
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].getPort() != requestPort)
			continue;

		if (servers[i].getHost() == this->_clientSockHost || servers[i].getHost() == INADDR_ANY)
		{
			if (servers[i].getServerName() == requestHost)
			{
				this->_request->setServer(servers[i]);
				break;
			}
			else if (servers[i].getIsDefault() || servers[i].getHost() == INADDR_ANY)
			{
				defaultServer = &servers[i];
			}
		}
	}
	if (this->_request->getServer() == NULL)
	{
		if (defaultServer)
			this->_request->setServer(*defaultServer);
		else
			throw RequestParseErrorException();
	}
}

void RequestParser::setRequestLocations()
{
	if (!this->_request->getServer())
		return;
	
	std::vector<Location> &locations = this->_request->getServer()->getLocation();
	for (size_t i = 0; i < locations.size(); i++)
	{
		std::string testDir;
		size_t uriSize;
		size_t locationPathSize;

		uriSize = this->_request->getUri().size();
		locationPathSize = locations[i].getLocationPath().size();
		if (uriSize < locationPathSize)
			continue;
		testDir = this->_request->getUri();
		testDir = testDir.substr(0, locationPathSize);
		if (testDir.compare(locations[i].getLocationPath()) != 0)
			continue;
		if (uriSize > locationPathSize &&
			this->_request->getUri()[locationPathSize] != '/' &&
			this->_request->getUri()[locationPathSize - 1] != '/')
		{
			continue;
		}
		Location *requestLocation = this->_request->getLocation();
		if (!requestLocation || requestLocation->getLocationPath().size() < locationPathSize)
			this->_request->setLocation(locations[i]);
	}
}

void RequestParser::setPathInfo()
{
	if (!this->_request->getLocation())
		return;
	std::string &uri = this->_request->getUri();
	std::vector<std::string> &cgiExtensions = this->_request->getLocation()->getCgiExtensionLocation();
	if (cgiExtensions.size() == 0)
		return;
	for (size_t i = 0; i < cgiExtensions.size(); i++)
	{
		std::string extension = cgiExtensions[i];
		if (extension[extension.size() - 1] != '/')
			extension.append("/");
		size_t separator = uri.find(extension);
		if (separator != std::string::npos)
		{
			separator += extension.size() - 1;
			this->_request->setPathInfo(uri.substr(separator));
			this->_request->setUri(uri.substr(0, separator));
			break;
		}
	}
}
