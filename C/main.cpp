#include "common.h"
#include "Processer.hpp"
#include "Listener.hpp"

void init()
{
}

void destroy()
{
}

int main(int argc, const char *argv[])
{
	int ret;
	CListener listen;

	init();	

	listen.run();

	while(!s_iStop);

	destroy();
	return 0;
}
