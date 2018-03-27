#include <string>
#include <cstdarg>
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

	std::string CLog::format(const char* fmt, ...) 
	{
			int size = 512;
			char* buff = new char[size];
			
			va_list v1;
			va_start(v1, fmt);
			int nsize = vsnprintf(buff, size, fmt, v1);
			if ( size <= nsize ) {
				delete[] buff;
				buff = new char[nsize + 1];
			
				nsize = vsnprintf(buff, size, fmt, v1);
			}
			std::string ret(buff);
			
			va_end(v1);
			delete[] buff;
			
			return ret;
	}
}
