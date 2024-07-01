#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>

/*NETWORK*/
#include <netinet/in.h>

/*STL CONTAINERS*/
#include <vector>
#include <map>

/*System*/
#include <sys/stat.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define SOCKET_MAX_CONN 1000
#define MAX_EPOLL_EVENTS 1000

typedef struct epoll_event EpollEvent;

#endif
