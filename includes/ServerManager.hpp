#ifndef SERVER_MANAGER
#define SERVER_MANAGER

#include "Server.hpp"
#include "Epoll.hpp"
#include "RequestFactory.hpp"
#include "Response.hpp"
#include "RequestParser.hpp"
#include "Logger.hpp"
#include "Client.hpp"

class ServerManager
{
	private:
		std::vector<Server> 	_servers;
		std::map<int, Client *>	_clients;
		Epoll					_epoll;

		void initServerMasterSockets();
		void initEpoll();
		void epollLoop();
		void handleEpollEvents(std::vector<EpollEvent> events);
		void handleEpollEvent(EpollEvent &event);
		Server *findServerByFd(int fd);
		void handleNewConnection(Server &server);
		void handleClientEvent(EpollEvent &event);
		void handleClientRequest(EpollEvent &event, Client *client);
		void sendResponseToClient(EpollEvent &event, Client *client);
		void closeClientConnection(int fd);
		std::string getRawRequestFromEpollEvent(EpollEvent &event);
		void logServerListening();
		void logNewConnection(int fd);
		void logClosedConnection(int fd);
		void logRequestReceived(Request &request, int fd) const;
		void logResponseSent(Response &response, int fd) const;

		ServerManager(const ServerManager &serverManager);
		ServerManager &operator=(const ServerManager &serverManager);

	public:
		ServerManager();
		ServerManager(const std::vector<Server> &servers);
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
