#include "../includes/Socket.hpp"

Socket::Socket() {}

Socket::Socket(const Socket &socket)
{
	*this = socket;
}

Socket &Socket::operator=(const Socket &socket)
{
	if (this != &socket)
	{
		this->_fd = socket._fd;
		this->_address = socket._address;
	}
	return (*this);
}

Socket::~Socket() {}

void Socket::initAsMasterSocket(uint16_t port)
{
	this->createSocket();
	this->setUpSocket();
	this->bindSocket(port);
}

void Socket::createSocket()
{
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd == -1)
		throw SocketInitializationFailedException();
}

void Socket::setUpSocket()
{
	int option = 1;

	int result = setsockopt(
		this->_fd, SOL_SOCKET,
		SO_REUSEADDR | SO_REUSEPORT,
		&option, sizeof(option)
	);
	if (result)
		throw SocketInitializationFailedException();
}

void Socket::bindSocket(uint16_t port)
{
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(port);
	int result = bind(
		this->_fd,
		(struct sockaddr *)&this->_address,
		sizeof(this->_address)
	);
	if (result < 0)
		throw SocketInitializationFailedException();
}

int Socket::getFd() const
{
	return (this->_fd);
}

void Socket::setFd(int fd)
{
	this->_fd = fd;
}
