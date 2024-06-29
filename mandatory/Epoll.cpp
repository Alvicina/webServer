#include "../includes/Epoll.hpp"

Epoll::Epoll() {}

Epoll::Epoll(const Epoll &epoll)
{
	*this = epoll;
}

Epoll &Epoll::operator=(const Epoll &epoll)
{
	if (this != &epoll)
	{
		this->_fd = epoll._fd;
		this->_epollEvents = epoll._epollEvents;
	}
	return (*this);
}

Epoll::~Epoll() {}

void Epoll::init()
{
	this->_fd = epoll_create1(0);
	if (this->_fd == -1)
		throw EpollInitializationFailedException();
}

void Epoll::addMasterSocket(const Socket &socket)
{
	EpollEvent event;
	event.events = EPOLLIN;
	event.data.fd = socket.getFd();
	this->_epollEvents.push_back(event);
	if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, event.data.fd, &this->_epollEvents.back()) == -1)
		throw EpollInitializationFailedException();
}

void Epoll::addClientSocket(const Socket &socket)
{
	EpollEvent event;
	event.events =  EPOLLIN | EPOLLET;
	event.data.fd = socket.getFd();
	this->_epollEvents.push_back(event);
	if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, socket.getFd(), &this->_epollEvents.back()) == -1)
		throw EpollInitializationFailedException();
}

std::vector<Epoll::EpollEvent> Epoll::waitForEvents()
{
	EpollEvent eventsArray[MAX_EPOLL_EVENTS];
	int numOfEvents = epoll_wait(this->_fd, eventsArray, MAX_EPOLL_EVENTS, -1);
	if (numOfEvents == -1)
		throw EpollInitializationFailedException();
	std::vector<EpollEvent> events(eventsArray, eventsArray + numOfEvents);
	return (events);
}

int Epoll::getFd() const
{
	return (this->_fd);
}
