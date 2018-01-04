#include "common.h"
#include "Manager.hpp"
#include "Listener.hpp"

struct thread_info{
	pthread_t 	thread_id;
	int 		thread_num;
	char		*argv;
};

void init()
{
	initSocket();
}

void destroy()
{
	destroySocket();
}

int main(int argc, const char *argv[])
{
	int ret;
	struct thread_info thread_listener;
	struct thread_info thread_handover;

	init();	

	ret = pthread_create(&thread_listener.thread_id, NULL, &listen, &thread_listener);
	if ( ret != 0 ) {
		handle_error_errno(ret, "pthread_create listen");
	}

	ret = pthread_create(&thread_handover.thread_id, NULL, &handover, &thread_handover);
	if ( ret != 0 ) {
		handle_error_errno(ret, "pthread_create handover");
	}

	pthread_join(thread_listener.thread_id);
	pthread_join(thread_handover.thread_id);

	destroy();
	return 0;
}
