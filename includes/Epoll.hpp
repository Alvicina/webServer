#ifndef EPOLL
#define EPOLL

#include <vector>
#include <sys/epoll.h>
#include "Socket.hpp"

#define MAX_EPOLL_EVENTS 1000

class Epoll
{
	private:
		int								_fd;
		std::vector<struct epoll_event>	_epollEvents;

	public:
		Epoll();
		Epoll(const Epoll &epoll);
		Epoll &operator=(const Epoll &epoll);
		~Epoll();

		void init();
		void addMasterSocket(const Socket &socket);
		void addClientSocket(const Socket &socket);
		std::vector<struct epoll_event> waitForEvents();

		int getFd() const;

		typedef struct epoll_event EpollEvent;

		class EpollInitializationFailedException : public std::exception
		{
			// TODO: Cerrar el _fd cuando se atrapa la excepción
			public:
				const char *what() const throw()
				{
					return ("ERROR: Epoll could not be initialized due to an unexpected error.");
				}
		};
};

#endif
