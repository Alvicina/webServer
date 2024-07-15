#include "../includes/Client.hpp"

Client::Client(): _socket(NULL) {}

Client::Client(Socket *socket): _socket(socket) {}

Client::Client(const Client &client)
{
	*this = client;
}

Client &Client::operator=(const Client &client)
{
	if (this != &client)
	{
		this->_socket = client._socket;
		this->_responseQueue = client._responseQueue;
	}
	return (*this);
}

Client::~Client()
{
	delete this->_socket;
	for (size_t i = 0; i < this->_responseQueue.size(); i++)
	{
		delete this->_responseQueue[i];
	}
}

Socket &Client::getSocket()
{
	return (*this->_socket);
}

void Client::setSocket(Socket *socket)
{
	this->_socket = socket;
}

std::vector<Response *> &Client::getResponseQueue()
{
	return (this->_responseQueue);
}

