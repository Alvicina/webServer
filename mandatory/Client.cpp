#include "../includes/Client.hpp"

Client::Client(): _socket(NULL), _server(NULL) {}

Client::Client(Socket *socket, Server *server):
	_socket(socket), _server(server) {}

Client::Client(const Client &client)
{
	*this = client;
}

Client &Client::operator=(const Client &client)
{
	if (this != &client)
	{
		this->_socket = client._socket;
		this->_server = client._server;
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

Server &Client::getServer()
{
	return (*this->_server);
}
