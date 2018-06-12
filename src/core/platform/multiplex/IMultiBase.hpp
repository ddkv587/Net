#ifndef __IMULTIBASEHPP__
#define __IMULTIBASEHPP__

namespace NET
{
	class IMultiBase
	{
		public:
			virtual	void			init() = 0;
			virtual void			destroy() = 0;

			virtual INT             setSize(INT size) = 0;
			virtual INT             addFileEvent(INT fd, INT mask) = 0;
			virtual void            delFileEvent(INT fd, INT mask) = 0;
			virtual	INT				eventLoop(void* timeout) = 0;
	};
}
#endif
