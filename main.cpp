#include "Log.h"

using namespace Net;

int main(int argc, const char *argv[])
{
	CLog::logCore("format:%s", "test");
	return 0;
}
