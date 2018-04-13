#ifndef __MULTISELECTHPP__
#define __MULTISELECTHPP__

namespace NET
{
	class CMultiSelect : public CMultiBase
	{
		public:
			CMultiSelect();
			virtual ~CMultiSelect();

			int 	addFileEvent(int, int);
			void	delFileEvent(int, int);
			int 	eventLoop(void *);
	};
}
#endif
