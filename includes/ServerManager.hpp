#ifndef SERVER_MANAGER
#define SERVER_MANAGER

#include <vector>
#include <sys/epoll.h>
#include <string.h>
#include "Server.hpp"
#include "Epoll.hpp"

class ServerManager
{
	private:
		std::vector<Server> _servers;
		std::vector<Socket>	_clients;
		Epoll				_epoll;

		void initServerMasterSockets();
		void initEpoll();
		void epollLoop();
		void handleEpollEvents(std::vector<struct epoll_event> events);

	public:
		ServerManager();
		ServerManager(const std::vector<Server> &servers);
		ServerManager(const ServerManager &serverManager);
		ServerManager &operator=(const ServerManager &serverManager);
		~ServerManager();

		void serve();

		void setServers(const std::vector<Server> &servers);
};

#endif
