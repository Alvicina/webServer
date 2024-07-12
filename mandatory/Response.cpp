#include "../includes/Response.hpp"
#include "../includes/Server.hpp"

static void ResponseContentRoutine(Request & request, Response & response)
{
	std::map<int, std::string> errorPages = request.getServer()->getErrorPages();
	std::map<int, std::string>::iterator it;

	for (it = errorPages.begin(); it != errorPages.end(); it++)
	{
		if (it->first == response.getStatusCode())
		{
			if (it->second.empty())
			{
				std::string content = Utils::codeStatus(it->first);
				response.setContent(content);
				response.setStatusCodeMessage(content);
				std::string path = "default";
				response.setFile(path);
				break ;
			}
			else
			{
				std::string path = (request.getServer()->getRoot() + it->second);
				if (access(path.c_str(), F_OK) == -1 && access(path.c_str(), R_OK) == -1)
				{
					std::string content = Utils::codeStatus(it->first);
					response.setContent(content);
					response.setStatusCodeMessage(content);
					path = "default";
					response.setFile(path);
					break ;
				}
				else
				{
					std::string content = Utils::codeStatus(it->first);
					response.setStatusCodeMessage(content);
					std::ifstream ErrorPageOpen(path.c_str());
					std::stringstream ErrorPageContent;
					ErrorPageContent << ErrorPageOpen.rdbuf();
					content = ErrorPageContent.str();
					response.setContent(content);
					response.setFile(path);
					break ;
				}
			}
		}
	}
}

static void ResponseContentType(Response & response)
{
	std::map<std::string, std::string> ContentType;
	std::string ext = response.getFile();
	size_t pos = ext.rfind('.', std::string::npos);
	if (pos == std::string::npos)
	{
		std::string extToFind = "default";
		response.getHeaders().insert(std::make_pair("Content-type: ",
		response.getFileExt(extToFind)));
	}
	else
	{
		std::string extToFind = ext.substr(pos);
		response.getHeaders().insert(std::make_pair("Content-type: ",
		response.getFileExt(extToFind)));
	}
}

static void ResponseContentLength(Response & response)
{
	std::string lengthTostring = Utils::intToString((int)response.getContent().size());
	response.getHeaders().insert(std::make_pair("Content-Length: ", lengthTostring));
}

static void ResponseConnectionType(Request & request, Response & response)
{
	std::map<std::string, std::string> headers = request.getHeaders();
	
	if (headers["Connection"] == "keep-alive\r")
		response.getHeaders().insert(std::make_pair("Connection: ", "keep-alive"));
	else
		response.getHeaders().insert(std::make_pair("Connection: ", "close"));
}

static void ResponseServer(Response & response)
{
	response.getHeaders().insert(std::make_pair("Server: ", "WebServer42"));
}

static void ResponseDate(Response & response)
{
	char date[1024];
	time_t actual = time(0);
	struct tm *GMTtime = gmtime(&actual);
	strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", GMTtime);
	response.getHeaders().insert(std::make_pair("Date: ", date));
}

static void ResponseLocationForError(Response & response)
{
	if (response.getFile() == "default")
		response.getHeaders().insert(std::make_pair("Location: ", "/"));
	else
		response.getHeaders().insert(std::make_pair("Location: ", response.getFile()));
}

static void ResponseLocation(Response & response, Request & request)
{
	if (response.getErrorResponse() == true)
		ResponseLocationForError(response);
	else
	{
		if (request.getLocation())
			response.getHeaders().insert(std::make_pair("Location: ", request.getLocation()->getLocationPath()));
	}
}

void Response::ResponseHeaderRoutine(Response & response, Request & request)
{
	ResponseContentType(response); //cuidado si metemos algun archivo con extension no contemplada
	ResponseContentLength(response);
	ResponseConnectionType(request, response);
	ResponseServer(response);
	ResponseLocation(response, request);
	ResponseDate(response);
}

void Response::ResponseRawRoutine()
{
	std::string raw;
	std::map<std::string, std::string>::iterator it;

	raw = getProtocol() + "/";
	raw.append((getProtocolVersion() + " "
	+ Utils::intToString(getStatusCode()) + " "
	+ getStatusCodeMessage() +  "\n"));
	for(it = getHeaders().begin(); it != getHeaders().end(); it++)
	{
		raw.append(it->first + it->second + "\n");
	}
	raw.append("\n" + getContent());
	setRaw(raw);
	std::cout << getRaw() << std::endl;
}

Response::Response(int errCode, Request *request) : _errorResponse(true)
{
	initFileExt();
	setStatusCode(errCode);
	ResponseContentRoutine(*request, *this);
	setProtocol(request->getProtocol());
	setProtocolVersion(request->getProtocolVersion());
	ResponseHeaderRoutine(*this, *request);
	ResponseRawRoutine();
}

Response::Response()
{
	_statusCode = 0;
	_errorResponse = false;
	_raw = "";
	_content = "";
	_file = "";
	_protocol = "";
	_protocolVersion = "";
	 initFileExt();
}

Response::Response(const Response &response)
{
	*this = response;
}

Response &Response::operator=(const Response &response)
{
	if (this != &response)
	{
		this->_statusCode = response._statusCode;
		this->_headers = response._headers;
		this->_raw = response._raw;
		this->_content = response._content;
		this->_protocol = response._protocol;
		this->_protocolVersion = response._protocolVersion;
	}
	return (*this);
}

Response::~Response() {}

int Response::getStatusCode()
{
	return (this->_statusCode);
}

void Response::setStatusCode(int statusCode)
{
	this->_statusCode = statusCode;
}

std::string & Response::getStatusCodeMessage()
{
	return (_statusCodeMessage);
}

void Response::setStatusCodeMessage(std::string & message)
{
	_statusCodeMessage = message;
}

std::map<std::string, std::string> &Response::getHeaders()
{
	return (this->_headers);
}

void Response::setHeaders(std::map<std::string, std::string> &headers)
{
	this->_headers = headers;
}

std::string &Response::getRaw()
{
	return (this->_raw);
}

void Response::setRaw(std::string &raw)
{
	this->_raw = raw;
}

std::string &Response::getContent()
{
	return (this->_content);
}

void Response::setContent(std::string &content)
{
	this->_content = content;
}

std::string &Response::getProtocol()
{
	return (this->_protocol);
}

void Response::setProtocol(std::string &protocol)
{
	this->_protocol = protocol;
}

std::string &Response::getProtocolVersion()
{
	return (this->_protocolVersion);
}

void Response::setProtocolVersion(std::string &protocolVersion)
{
	this->_protocolVersion = protocolVersion;
}

std::string &Response::getFile()
{
	return (this->_file);
}

void Response::setFile(std::string & file)
{
	_file = file;
}

std::string & Response::getFileExt(std::string & ext)
{
	if (_exts.count(ext))
		return (_exts[ext]);
	return (_exts["default"]);
}

void Response::setErrorResponse(bool isError)
{
	_errorResponse = isError;
}

bool & Response::getErrorResponse()
{
	return (_errorResponse);
}

void Response::initFileExt()
{
	_exts[".html"] = "text/html";
    _exts[".htm"] = "text/html";
    _exts[".css"] = "text/css";
    _exts[".ico"] = "image/x-icon";
    _exts[".avi"] = "video/x-msvideo";
    _exts[".bmp"] = "image/bmp";
    _exts[".doc"] = "application/msword";
    _exts[".gif"] = "image/gif";
    _exts[".gz"] = "application/x-gzip";
    _exts[".ico"] = "image/x-icon";
    _exts[".jpg"] = "image/jpeg";
    _exts[".jpeg"] = "image/jpeg";
    _exts[".png"] = "image/png";
    _exts[".txt"] = "text/plain";
    _exts[".mp3"] = "audio/mp3";
    _exts[".pdf"] = "application/pdf";
    _exts["default"] = "text/html";
}



