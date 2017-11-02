#include "Core/Log.h"
#include "Core/Memory.h"

int main(int argc, const char *argv[])
{
	Net::CLog::logPanic("format:%s", "test");

	return 0;
}
