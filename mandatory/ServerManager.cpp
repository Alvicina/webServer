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

ServerManager::~ServerManager() {}

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

void ServerManager::handleEpollEvents(std::vector<struct epoll_event> events)
{
	for (size_t i = 0; i < events.size(); i++)
	{
		bool foundServer = false;
		std::cout << "Request received!" << std::endl;
		for (size_t j = 0; j < this->_servers.size(); j++)
		{
			if (events[i].data.fd == this->_servers[j].getSocket().getFd())
			{
				this->_clients.push_back(this->_servers[j].getSocket().acceptConnection());
				this->_epoll.addClientSocket(this->_clients.back());
				foundServer = true;
				break;
			}
		}
		if (!foundServer)
		{
			char buffer[1024];
			ssize_t bytes = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
			buffer[bytes] = 0;
			// TODO: No hay solicitud
			// if (bytes <= 0)
			std::cout << buffer << std::endl;
			std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\nHello, Javi!";
			send(events[i].data.fd, response.c_str(), response.size(), 0);
			std::cout << "Response sent!" << std::endl;
		}
	}
}

void ServerManager::setServers(const std::vector<Server> &servers)
{
	this->_servers = servers;
}
