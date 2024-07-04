#ifndef SERVER_MANAGER
#define SERVER_MANAGER

#include "Server.hpp"
#include "Epoll.hpp"

class ServerManager
{
	private:
		std::vector<Server> 	_servers;
		std::map<int, Socket *>	_clients;
		Epoll					_epoll;

		void initServerMasterSockets();
		void initEpoll();
		void epollLoop();
		void handleEpollEvents(std::vector<EpollEvent> events);
		void handleEpollEvent(EpollEvent &event);
		Server *findServerByFd(int fd);
		void handleNewConnection(Server &server);
		void handleClientRequest(EpollEvent &event);
		void closeClientConnection(int fd);
		std::string getRawRequestFromEpollEvent(EpollEvent &event);

	public:
		ServerManager();
		ServerManager(const std::vector<Server> &servers);
		ServerManager(const ServerManager &serverManager);
		ServerManager &operator=(const ServerManager &serverManager);
		~ServerManager();

		void serve();

		void setServers(const std::vector<Server> &servers);

		class IOException : public std::exception
		{
			public:
				const char *what() const throw()
				{
					return ("ERROR: An unexpected IO error occurred.");
				}
		};
	
};

#endif
