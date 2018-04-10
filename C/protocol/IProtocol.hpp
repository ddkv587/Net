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

		typedef struct tagObject
		{
			int size;
			void* data;

			tagObject(int s) 
			{
				size = s;
				data = (void*)new char(s);
			}

			~tagObject() 
			{
				if ( NULL != data ) {
					delete (char*)data;
					data =NULL;
				};
			}
		} OBJECT;

		const static int SIZE_HEADER_MANAGER = sizeof(HEADER_MANAGER);

		public:
			virtual int analyse(int, char*) = 0;
			virtual int package(int, OBJECT*, char*) = 0;

			virtual int callSpecialFunc(int, int, const char*, OBJECT*) = 0;
	};
}
#endif
