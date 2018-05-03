#ifndef __IPROTOCOLHPP__
#define __IPROTOCOLHPP__

namespace NET
{
	class IProtocol
	{
		public:
#define SYNC_FLAG 0x03302112
	
		typedef struct tagSocketHeader
		{
			int sync;
			int size;
			int protocol;
		} HEADER_MANAGER;

		const static int SIZE_HEADER_MANAGER = sizeof(HEADER_MANAGER);

		public:
			virtual int analyse(int, char*) = 0;
			virtual int package(int, const Object*, char*&) = 0;

			virtual int callSpecialFunc(int, int, const char*, Object*&) = 0;
	};
}
#endif
