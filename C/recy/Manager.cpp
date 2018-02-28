#include <sys/epoll.h>

#include "common.h"
#include "Manager.hpp"

std::mutex mt;
std::queue<int> manager_queue;


static int epollfd;
struct epoll_event ev, events[SYSTEM_LIMIT_MAX_EPOLL_EVENTS];

void initManager()
{
	epollfd = epoll_create(SYSTEM_LIMIT_MAX_EPOLL_EVENTS + 1);

	if ( epollfd == -1 ) {
		handle_error("epoll_create");
	}
}

void destroyManager()
{
	close(fd_epoll);	
}

void setNonBlock(int fd)
{
	assert(fd > 0);
	
	int flags = fcntl(fd, F_GETFL, 0);

	if ( flags == -1 ) 
		handle_error("fcntl get");

	flags |= O_NONBLOCK;

	if ( fcntl(fd, F_SETFL, flags) == -1 ) 
		handle_error("fcntl set");
}

void addCtrl(int fd)
{
	assert(fd > 0);

	setNonBlock(fd);	

	mt.lock();
	ev.events = EPOLLIN | EPOLLET;
	//manager_queue.push(fd);
	ev.data.fd = fd;
	if ( -1 == epoll_ctrl(epollfd, EPOLL_CTL_ADD, fd, &ev) )
		handle_error("e[poll ctrl");
	mt.unlock();
}

void* handover(void* arg)
{
	while(!s_iStop) {
		/*
		if ( manager_queue.size() != 0 ) {
			mt.lock();
			for ( int i=0; i < manager_queue.size(); ++i ) {
				int fd = manager_queue.pop_front();
			}
			mt.unlock();
		}
		*/
		int ret = epoll_wait(epollfd, events, SYSTEM_LIMIT_MAX_EPOLL_EVENTS, 100);	//100 ms timeout
		
		if ( 0 == ret ) {
			//timeout
			continue;
		} else if ( -1 == ret ) {
			handle_error("epoll wait");
		} else {
			for ( int index = 0; index < ret; ++index ) {
				if ( events[index].events & EPOLLERR ||
						events[index].events & EPOLLHUP ||
						events[index].events & EPOLLRDHUP ||
						!(events[index].events & EPOLLIN) ) {
					close(event[index].data.fd);
					continue;
				} else {
					//read data
					char buff[1024];
					while ( !s_iStop ) {
						ssize_t size = read(events[index].data.fd, buff, 1024);
						
						if ( size == -1 ) {
							if ( errno == EAGAIN ) {
								//read complete
								break;
							}
						}
					}
				}
			}
		}
	}
}
