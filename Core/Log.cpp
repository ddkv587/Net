#include <assert.h>
#include <stdarg.h>
#include "config.h"
#include "Log.h"

namespace Net
{
#ifdef CONFIG_LOG_ENABLE
	bool CLogBase::m_bLogEnable = CONFIG_LOG_ENABLE;
#else
	bool CLogBase::m_bLogEnable = false;
#endif

	void CLogBase::logOut(ELogLevel level, char *pLogContent)
	{
		assert(NULL != pLogContent);
		if ( m_bLogEnable ) {
#ifdef CONFIG_LOG_LEVEL
			if ( level <=  CONFIG_LOG_LEVEL ) {
				fprintf(stdout, "%s\n", pLogContent);
#ifdef CONFIG_LOG_OUT_FILE
				char prefix[][10] = {
					"core",
					"panic",
					"error",
					"debug",
					"info",
				};
				char filePath[1024];
				snprintf(filePath, 1024, "%s/%s", CONFIG_LOG_OUT_FILE_PREFIX, prefix[level]);

				FILE *fp = fopen(filePath, "w+");
				assert(NULL != fp);

				fprintf(fp, "%s\n", pLogContent);
				fclose(fp);
#endif
			}
#endif
		}
	}

	void CLogBase::switchLog(bool status)
	{
		m_bLogEnable = status;
	}

	void CLog::logInfo(char *format, ...)
	{
		char logContent[1024];
		va_list ap;

		va_start(ap, format);
		vsnprintf(logContent, 1024, format, ap);
		va_end(ap);
	
		CLogBase::logOut(CLogBase::ELogLevel_Info, logContent);
	}

	void CLog::logDebug(char *format, ...)
	{
		char logContent[1024];
		va_list ap;

		va_start(ap, format);
		vsnprintf(logContent, 1024, format, ap);
		va_end(ap);
	
		CLogBase::logOut(CLogBase::ELogLevel_Debug, logContent);
	}

	void CLog::logError(char *format, ...)
	{
		char logContent[1024];
		va_list ap;

		va_start(ap, format);
		vsnprintf(logContent, 1024, format, ap);
		va_end(ap);
	
		CLogBase::logOut(CLogBase::ELogLevel_Error, logContent);
	}

	void CLog::logPanic(char *format, ...)
	{
		char logContent[1024];
		va_list ap;

		va_start(ap, format);
		vsnprintf(logContent, 1024, format, ap);
		va_end(ap);
	
		CLogBase::logOut(CLogBase::ELogLevel_Painc, logContent);
	}

	void CLog::logCore(char *format, ...)
	{
		char logContent[1024];
		va_list ap;

		va_start(ap, format);
		vsnprintf(logContent, 1024, format, ap);
		va_end(ap);
	
		CLogBase::logOut(CLogBase::ELogLevel_Core, logContent);
	}
}
