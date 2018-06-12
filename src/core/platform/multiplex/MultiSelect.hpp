#ifndef __MULTISELECTHPP__
#define __MULTISELECTHPP__

namespace NET
{
	class CMultiSelect : public CMultiBase
	{
		public:
			CMultiSelect();
			virtual ~CMultiSelect();

			void	init();
			void	destroy();

			INT		setSize(INT);
			INT		addFileEvent(int, int);
			void	delFileEvent(int, int);
			INT		eventLoop(void *);
	};
}
#endif
