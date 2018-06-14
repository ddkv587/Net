#ifndef __MULTISELECTHPP__
#define __MULTISELECTHPP__

namespace NET
{
	class CMultiSelect : public CMultiManager::IMultiBase
	{
		public:
			CMultiSelect();
			virtual ~CMultiSelect();

			void	init();
			void	destroy();

			INT		setSize(INT) override;
			INT		addFileEvent(INT, INT, EVENT_LOOP*) override;
			void	delFileEvent(INT, INT, EVENT_LOOP*) override;
			INT		eventLoop(void*, EVENT_LOOP*) override;
	};
}
#endif
