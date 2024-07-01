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

void ServerManager::handleEpollEvents(std::vector<struct epoll_event> events)
{
	for (size_t i = 0; i < events.size(); i++)
	{
		bool foundServer = false;
		for (size_t j = 0; j < this->_servers.size(); j++)
		{
			if (events[i].data.fd == this->_servers[j].getSocket().getFd())
			{
				Socket *socket = this->_servers[j].getSocket().acceptConnection();
				this->_clients[socket->getFd()] = socket;
				this->_epoll.addClientSocket(*socket);
				foundServer = true;
				std::cout << "New client with fd " << socket->getFd() << std::endl;
				break;
			}
		}
		if (!foundServer)
		{
			if (events[i].events & EPOLLIN)
			{
				std::cout << "Client request received!" << std::endl;
				char buffer[1024];
				ssize_t bytes = read(events[i].data.fd, buffer, sizeof(buffer) - 1);
				buffer[bytes] = 0;
				if (bytes > 0)
				{
					std::cout << buffer << std::endl;
					this->_epoll.setSocketOnWriteMode(*this->_clients[events[i].data.fd]);
				}
				else if (bytes == 0)
				{
					// TODO: Close connection request
				}
				else
				{
					// TODO: Handle error
				}
			}
			if (events[i].events & EPOLLOUT)
			{
				std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 12\n\nHello, Javi!";
				if (send(events[i].data.fd, response.c_str(), response.size(), 0) == -1)
				{
					// TODO: Handle error
				}
				this->_epoll.setSocketOnReadMode(*this->_clients[events[i].data.fd]);
				std::cout << "Response sent!" << std::endl;
			}
		}
	}
}

void ServerManager::setServers(const std::vector<Server> &servers)
{
	this->_servers = servers;
}
