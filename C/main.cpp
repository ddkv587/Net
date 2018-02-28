#include "common.h"
#include "Processor.hpp"
#include "Listener.hpp"

void init()
{
}

void destroy()
{
}

int main(int argc, const char *argv[])
{
	CListener listen;
	CProcessor process1;
	CProcessor process2;
	CProcessor process3;
	CProcessor process4;

	init();	

	listen.addFileListener(&process1);
	listen.addFileListener(&process2);
	listen.addFileListener(&process3);
	listen.addFileListener(&process4);

	process1.run();
	process2.run();
	process3.run();
	process4.run();

	listen.run();

	while(!s_iStop);

	destroy();
	return 0;
}
