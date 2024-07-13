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
	std::map<int, Client *>::iterator it = this->_clients.begin();
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
	this->logServerListening();
	while (true)
	{
		this->handleEpollEvents(this->_epoll.waitForEvents());
	}
}

void ServerManager::logServerListening()
{
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		Logger::logInfo(
			(std::ostringstream &)(std::ostringstream().flush()
				<< "👂 Server listening on port " << this->_servers[i].getPort()
			), 1);
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
		Logger::logError(e.what());
	}
	catch (Epoll::EpollInitializationFailedException &e)
	{
		this->closeClientConnection(event.data.fd);
		Logger::logError(e.what());
	}
	catch (std::exception &e)
	{
		Logger::logError(e.what());
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
	// TODO: Probar si la siguiente linea está bien. Se llama al destructor de client?
	this->_clients[socket->getFd()] = new Client(socket);
	this->_epoll.addClientSocket(*socket);
	this->logNewConnection(socket->getFd());
}

void ServerManager::logNewConnection(int fd)
{
	Logger::logInfo(
		(std::ostringstream &)(std::ostringstream().flush()
			<< "✅ New client connection with fd " << fd
		), 1);
}

void ServerManager::handleClientRequest(EpollEvent &event)
{
	Client *client = this->_clients[event.data.fd];

	if (event.events & EPOLLIN)
	{
		std::string rawRequest = this->getRawRequestFromEpollEvent(event);
		if (rawRequest.size() == 0)
		{
			this->closeClientConnection(event.data.fd);
			return;
		}
		this->_epoll.setSocketOnWriteMode(client->getSocket());
		// TODO: Handle request parse errors (send a response with HTTP error code)
		RequestParser requestParser(rawRequest);
		Request &request = requestParser.parseRequest(this->_servers);
		this->logRequestReceived(request, event.data.fd);
		Response *response = request.getServer()->handleRequest(request);
		client->getResponseQueue().push_back(response);
	}
	if (event.events & EPOLLOUT)
	{
		for (size_t i = 0; i < client->getResponseQueue().size(); i++)
		{
			Response *response = client->getResponseQueue()[i];
			if (send(event.data.fd, response->getRaw().c_str(), response->getRaw().size(), 0) == -1)
				throw IOException();
			this->logResponseSent(*response, event.data.fd);
			delete response;
		}
		client->getResponseQueue().clear();
		this->_epoll.setSocketOnReadMode(client->getSocket());
	}
}

void ServerManager::logRequestReceived(Request &request, int fd) const
{
	Logger::logInfo((std::ostringstream &)(std::ostringstream().flush()
		<< "⬇️  Request received from client " << fd << ": "
		<< Request::getMethodName(request.getMethod()) << " "
		<< request.getUri()));
}

void ServerManager::logResponseSent(Response &response, int fd) const
{
	Logger::logInfo((std::ostringstream &)(std::ostringstream().flush()
		<< "⬆️  Response sent to client " << fd << ": "
		<< response.getStatusCode() << " " << response.getStatusCodeMessage()));
}

std::string ServerManager::getRawRequestFromEpollEvent(EpollEvent &event)
{
	std::string buffer;
	char bufferTmp[1024];
	ssize_t bytes;

	do
	{
		bytes = read(event.data.fd, bufferTmp, sizeof(bufferTmp) - 1);
		if (bytes < 0)
			throw IOException();
		bufferTmp[bytes] = 0;
		buffer += bufferTmp;
	}
	while (bytes == sizeof(bufferTmp) - 1);
	return (buffer);
}

void ServerManager::closeClientConnection(int fd)
{
	this->_epoll.deleteClientSocket(this->_clients[fd]->getSocket());
	delete this->_clients[fd];
	this->_clients.erase(fd);
	this->logClosedConnection(fd);
}

void ServerManager::logClosedConnection(int fd)
{
	Logger::logInfo(
		(std::ostringstream &)(std::ostringstream().flush()
			<< "❎ Closed connection with client " << fd
		), 1);
}

void ServerManager::setServers(const std::vector<Server> &servers)
{
	this->_servers = servers;
}
