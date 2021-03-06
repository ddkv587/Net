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
			INT 		sync;
			INT 		protocol;
			UINT 		uiSize;
		} HEADER_MANAGER;

		typedef struct tagObject
		{
			int 		size;
			char* 		data;

			tagObject(int s) 
			{
				size = s;
				data = new char(s);
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
			virtual int analyse(CHAR*, UINT) = 0;
			virtual int package(int, const OBJECT*, char*&) = 0;

			virtual int callSpecialFunc(int, int, const char*, OBJECT*&) = 0;
	};
}
#endif
