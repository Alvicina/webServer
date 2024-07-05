#include "../includes/ServerManager.hpp"

ServerManager::ServerManager() {}

ServerManager::ServerManager(const std::vector<Server> &servers):
	_servers(servers) {}

ServerManager::ServerManager(const ServerManager &serverManager)
{
	*this = serverManager;
}

ServerManager &ServerManager::operator=(const ServerManager &serverManager)
{
	if (this != &serverManager)
	{
		this->_servers = serverManager._servers;
		this->_clients = serverManager._clients;
		this->_epoll = serverManager._epoll;
	}
	return (*this);
}

ServerManager::~ServerManager()
{
	std::map<int, Socket *>::iterator it = this->_clients.begin();
	while (it != this->_clients.end())
	{
		delete it->second;
	}
}

void ServerManager::serve()
{
	this->initServerMasterSockets();
	this->initEpoll();
	this->epollLoop();
}

void ServerManager::initServerMasterSockets()
{
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		this->_servers[i].initMasterSocket();
	}
}

void ServerManager::initEpoll()
{
	this->_epoll.init();
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		_epoll.addMasterSocket(this->_servers[i].getSocket());
	}
}

void ServerManager::epollLoop()
{
	std::cout << "Server listening..." << std::endl;
	while (true)
	{
		this->handleEpollEvents(this->_epoll.waitForEvents());
	}
}

void ServerManager::handleEpollEvents(std::vector<EpollEvent> events)
{
	for (size_t i = 0; i < events.size(); i++)
	{
		this->handleEpollEvent(events[i]);
	}
}

void ServerManager::handleEpollEvent(EpollEvent &event)
{
	try
	{
		Server *server = this->findServerByFd(event.data.fd);
		if (server != NULL)
			this->handleNewConnection(*server);
		else
			this->handleClientRequest(event);
	}
	catch (IOException &e)
	{
		this->closeClientConnection(event.data.fd);
		std::cerr << e.what() << std::endl;
	}
	catch (Epoll::EpollInitializationFailedException &e)
	{
		this->closeClientConnection(event.data.fd);
		std::cerr << e.what() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

Server *ServerManager::findServerByFd(int fd)
{
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		if (fd == this->_servers[i].getSocket().getFd())
			return (&this->_servers[i]);
	}
	return (NULL);
}

void ServerManager::handleNewConnection(Server &server)
{
	Socket *socket = server.getSocket().acceptConnection();
	this->_clients[socket->getFd()] = socket;
	this->_epoll.addClientSocket(*socket);
	std::cout << "New client connection with fd " << socket->getFd() << std::endl;
}

void ServerManager::handleClientRequest(EpollEvent &event)
{
	if (event.events & EPOLLIN)
	{
		// TODO: Leer la solicitud entera
		char buffer[1024];
		ssize_t bytes = read(event.data.fd, buffer, sizeof(buffer) - 1);
		buffer[bytes] = 0;
		if (bytes > 0)
		{
			std::cout << "Client request received:" << std::endl;
			std::cout << buffer << std::endl;
			this->_epoll.setSocketOnWriteMode(*this->_clients[event.data.fd]);
		}
		else if (bytes == 0)
		{
			this->closeClientConnection(event.data.fd);
		}
		else
		{
			throw IOException();
		}
	}
	if (event.events & EPOLLOUT)
	{
		handlerRoutine();
		std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\nHello, Javi!";

		if (send(event.data.fd, response.c_str(), response.size(), 0) == -1)
			throw IOException();
		this->_epoll.setSocketOnReadMode(*this->_clients[event.data.fd]);
		std::cout << "Response sent!" << std::endl;
	}
}

void ServerManager::closeClientConnection(int fd)
{
	delete this->_clients[fd];
	this->_clients.erase(fd);
	std::cout
		<< "Closed connection with client "
		<< fd << std::endl;
}

void ServerManager::setServers(const std::vector<Server> &servers)
{
	this->_servers = servers;
}

Response* ServerManager::handlerRoutine()
{
	Request request;
	Methods value = HEAD;
	std::string protocol = "HTTP";
	std::string protocolversion = "1.1";
	std::string raw = "http://localhost:8002/index.html";
	std::string uri = "index.html";
	std::map<std::string, std::string> headers;
	headers["Connection"] = "keep-alive";

	request.setMethod((value));
	request.setProtocol(protocol);
	request.setProtocolVersion(protocolversion);
	request.setRaw(raw); 
	request.setUri(uri);
	request.setServer(_servers[0]);
	request.setLocation(_servers[0].getLocation()[0]);
	request.setHeaders(headers);

	RequestHandler *handler = RequestFactory::makeRequestHandler(request);
	std::cout << "Type of request: " << handler->getMethods() << std::endl;
	Response *response = handler->handleRequest();
	return (response);
}
