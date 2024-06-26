#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Socket
{
	private:
		int 				_fd;
		struct sockaddr_in	_address;

		void createSocket();
		void setUpSocket();
		void bindSocket(uint16_t port);

	public:
		Socket();
		Socket(const Socket &socket);
		Socket &operator=(const Socket &socket);
		~Socket();

		void initAsMasterSocket(uint16_t port);

		int getFd() const;
		void setFd(int fd);

		class SocketInitializationFailedException : public std::exception
		{
			// TODO: Cerrar el _fd cuando se atrapa la excepción
			public:
				const char *what() const throw()
				{
					return ("The socket could not be initialized due to an unexpected error.");
				}
		};
};

#endif
