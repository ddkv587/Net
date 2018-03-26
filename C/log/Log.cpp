#include "NET_Log.hpp"

namespace NET
{
	void CLog::initLog(const char* name)
	{
		google::InitGoogleLogging(name);
		FLAGS_log_dir = "/tmp/log";
		FLAGS_alsologtostderr = true;
		FLAGS_colorlogtostderr = true;
		FLAGS_minloglevel = LL_INFO;
	}

	void CLog::setDirection(char* strPath)
	{
		FLAGS_log_dir = strPath;
	}

	void CLog::setDirection(LOG_LEVEL level, char* strPath)
	{
		google::SetLogDestination(level, strPath);
	}

	void CLog::setFilter(LOG_LEVEL level)
	{
		FLAGS_minloglevel = level;
	}

	void CLog::enableToStderr(bool bEnable)
	{
		FLAGS_alsologtostderr = bEnable;
	}

	void CLog::enableColor(bool bEnable)
	{
		FLAGS_colorlogtostderr = bEnable;
	}
}
