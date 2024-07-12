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
	Response *response;
	
	if (event.events & EPOLLIN)
	{
		std::string rawRequest = this->getRawRequestFromEpollEvent(event);
		if (rawRequest.size() == 0)
		{
			this->closeClientConnection(event.data.fd);
			return;
		}
		// this->_epoll.setSocketOnWriteMode(*this->_clients[event.data.fd]);
		// TODO: Handle request parse errors (send a response with HTTP error code)
		RequestParser requestParser(rawRequest);
		Request &request = requestParser.parseRequest(this->_servers);
		//std::cout << request << std::endl;
		response = request.getServer()->handleRequest(request);
		// std::cout << response->getRaw() << std::endl;


		// std::cout << response->getRaw() << std::endl;
		if (send(event.data.fd, response->getRaw().c_str(), response->getRaw().size(), 0) == -1)
			throw IOException();
		// this->_epoll.setSocketOnReadMode(*this->_clients[event.data.fd]);
		std::cout << "Response sent!" << std::endl;
	}
	// if (event.events & EPOLLOUT)
	// {
	// 	std::cout << response->getRaw() << std::endl;
	// 	if (send(event.data.fd, response->getRaw().c_str(), response->getRaw().size(), 0) == -1)
	// 		throw IOException();
	// 	this->_epoll.setSocketOnReadMode(*this->_clients[event.data.fd]);
	// 	std::cout << "Response sent!" << std::endl;
	delete response;
	// }
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
