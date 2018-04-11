#include "NET_Log.hpp"
#include "NET_Socket.hpp"
#include "NET_Listener.hpp"
#include "NET_Processor.hpp"

int main(int argc, const char *argv[])
{
	NET::CLog::initLog(argv[0]);

	NET::CListener listen;
	NET::CProcessor process1;
	NET::CProcessor process2;
	NET::CProcessor process3;
	NET::CProcessor process4;
	NET::CProcessor process5;

	listen.addFileListener(&process1);
	//listen.addFileListener(&process2);
	//listen.addFileListener(&process3);
	//listen.addFileListener(&process4);
	//listen.addFileListener(&process5);

	process1.run();
	//process2.run();
	//process3.run();
	//process4.run();
	//process5.run();

	listen.run();

	while(true);

	return 0;
}
