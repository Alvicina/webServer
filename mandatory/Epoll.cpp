#include "../includes/Epoll.hpp"
#include "../includes/Socket.hpp"

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
	}
	return (*this);
}

Epoll::~Epoll()
{
	close(this->_fd);
}

void Epoll::init()
{
	this->_fd = epoll_create1(0);
	if (this->_fd == -1)
		throw EpollInitializationFailedException();
}

void Epoll::addMasterSocket(Socket &socket)
{
	EpollEvent event;
	event.events = EPOLLIN;
	event.data.fd = socket.getFd();
	socket.setEpollEvent(event);
	if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, event.data.fd, &socket.getEpollEvent()) == -1)
		throw EpollInitializationFailedException();
}

void Epoll::addClientSocket(Socket &socket)
{
	EpollEvent event;
	event.events =  EPOLLIN | EPOLLET;
	event.data.fd = socket.getFd();
	socket.setEpollEvent(event);
	if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, socket.getFd(), &socket.getEpollEvent()) == -1)
		throw EpollInitializationFailedException();
}

std::vector<EpollEvent> Epoll::waitForEvents()
{
	EpollEvent eventsArray[MAX_EPOLL_EVENTS];
	int numOfEvents = epoll_wait(this->_fd, eventsArray, MAX_EPOLL_EVENTS, -1);
	if (numOfEvents == -1)
		throw EpollInitializationFailedException();
	std::vector<EpollEvent> events(eventsArray, eventsArray + numOfEvents);
	return (events);
}

void Epoll::setSocketOnReadMode(Socket &socket)
{
	EpollEvent &event = socket.getEpollEvent();
	event.events = EPOLLIN | EPOLLET;
	if (epoll_ctl(this->_fd, EPOLL_CTL_MOD, socket.getFd(), &event) == -1)
		throw EpollInitializationFailedException();
}

void Epoll::setSocketOnWriteMode(Socket &socket)
{
	EpollEvent &event = socket.getEpollEvent();
	event.events = EPOLLOUT | EPOLLET;
	if (epoll_ctl(this->_fd, EPOLL_CTL_MOD, socket.getFd(), &event) == -1)
		throw EpollInitializationFailedException();
}

int Epoll::getFd() const
{
	return (this->_fd);
}
