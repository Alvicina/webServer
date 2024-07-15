#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "webserv.hpp"
#include "Socket.hpp"
#include "Response.hpp"

class Client
{
	private:
		Socket 					*_socket;
		std::vector<Response *>	_responseQueue;

	public:
		Client();
		Client(Socket *socket);
		Client(const Client &client);
		Client &operator=(const Client &client);
		~Client();

		Socket &getSocket();
		void setSocket(Socket *socket);
		std::vector<Response *> &getResponseQueue();
};

#endif
