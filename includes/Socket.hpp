#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SOCKET_MAX_CONN 1000

class Socket
{
	private:
		int 				_fd;
		struct sockaddr_in	_address;
		socklen_t			_addressLen;

		void createSocket();
		void setUpSocket();
		void bindSocket(in_addr_t host, uint16_t port);
		void listenForConnections();

	public:
		Socket();
		Socket(const Socket &socket);
		Socket &operator=(const Socket &socket);
		~Socket();

		void initAsMasterSocket(in_addr_t host, uint16_t port);
		Socket acceptConnection() const;

		int getFd() const;
		void setFd(int fd);
		struct sockaddr_in getAddress() const;
		socklen_t getAddressLen() const;

		class SocketInitializationFailedException : public std::exception
		{
			// TODO: Cerrar el _fd cuando se atrapa la excepción
			public:
				const char *what() const throw()
				{
					return ("ERROR: The docket could not be initialized due to an unexpected error.");
				}
		};
};

#endif
