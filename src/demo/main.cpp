#include <dlfcn.h>
#include "NET_Core.hpp"
#include "NET_Listener.hpp"
#include "NET_Processor.hpp"
#include "NET_Native.hpp"

int main(int argc, const char *argv[])
{
	NET::CLog::initLog(argv[0]);

	CMain::getInstance()->start();
	/*
	void* listener_handle = NULL; 

	LOG_IF( ERROR, NULL == ( listener_handle = dlopen("./libListener.so", RTLD_NOW) ) ) 
			<< NET::CLog::format( "dlopen error: %s", dlerror());

	typedef NET::CListener* (*listener_create_t)();
	typedef void (*listener_destroy_t)(NET::CListener*);

	listener_create_t listener_create;
	listener_destroy_t listener_destroy;

	LOG_IF( ERROR, NULL == ( listener_create = (listener_create_t)dlsym( listener_handle, "create" ) ) ) 
			<< NET::CLog::format( "dlopen error: %s", dlerror());

	LOG_IF( ERROR, NULL == ( listener_destroy = (listener_destroy_t)dlsym( listener_handle, "destroy" ) ) ) 
			<< NET::CLog::format( "dlopen error: %s", dlerror());

	dlclose(listener_handle);

	CListener* listen = listener_create();

	NET::CProcessor process1;
	NET::CProcessor process2;
	NET::CProcessor process3;
	NET::CProcessor process4;
	NET::CProcessor process5;

	listen->addFileListener(&process1);
	listen->addFileListener(&process2);
	listen->addFileListener(&process3);
	listen->addFileListener(&process4);
	listen->addFileListener(&process5);

	process1.run();
	process2.run();
	process3.run();
	process4.run();
	process5.run();

	listen->run();
	
	while(true);

	listener_destroy(listen);
	*/

	return 0;
}
