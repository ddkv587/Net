#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
namespace Net
{
	class CLogBase
	{
		public:
			enum ELogLevel {
				ELogLevel_Core = 0,
				ELogLevel_Painc,
				ELogLevel_Error,
				ELogLevel_Debug,
				ELogLevel_Info,
			};

		public:
			static void logOut(ELogLevel level, char *pLogContent);

			static void switchLog(bool status);

		private:
			CLogBase() {}
			virtual ~CLogBase() {}

		private:
			static bool m_bLogEnable;
	};

	class CLog
	{
		public:
			static void logInfo(char *format, ...);
			static void logDebug(char *format, ...);
			static void logError(char *format, ...);
			static void logPanic(char *format, ...);
			static void logCore(char *format, ...);

		private:
			CLog() {}
			virtual ~CLog() {}
	};
}
#endif
