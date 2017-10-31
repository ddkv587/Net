#include "Core/Log.h"
#include "Core/Memory.h"

using namespace Net;

int main(int argc, const char *argv[])
{
	CLog::logPanic("format:%s", "test");

	return 0;
}
