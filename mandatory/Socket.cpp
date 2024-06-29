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
		this->_addressLen = socket._addressLen;
	}
	return (*this);
}

Socket::~Socket() {}

void Socket::initAsMasterSocket(in_addr_t host, uint16_t port)
{
	this->createSocket();
	this->setUpSocket();
	this->bindSocket(host, port);
	this->listenForConnections();
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

void Socket::bindSocket(in_addr_t host, uint16_t port)
{
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = host;
	this->_address.sin_port = htons(port);
	this->_addressLen = sizeof(this->_address);
	int result = bind(
		this->_fd,
		(struct sockaddr *)&this->_address,
		this->_addressLen
	);
	if (result < 0)
		throw SocketInitializationFailedException();
}

void Socket::listenForConnections()
{
	if (listen(this->_fd, SOCKET_MAX_CONN) == -1)
		throw SocketInitializationFailedException();
}

Socket Socket::acceptConnection() const
{
	Socket socket;
	socket.setFd(accept(
		this->_fd,
		(struct sockaddr *) &this->_address,
		(socklen_t *) &this->_addressLen
	));
	if (socket.getFd() == -1)
		throw SocketInitializationFailedException();
	return (socket);
}

int Socket::getFd() const
{
	return (this->_fd);
}

void Socket::setFd(int fd)
{
	this->_fd = fd;
}

struct sockaddr_in Socket::getAddress() const
{
	return (this->_address);
}

socklen_t Socket::getAddressLen() const
{
	return (this->_addressLen);
}
