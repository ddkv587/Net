#ifndef __MULTIMANAGERHPP__
#define __MULTIMANAGERHPP__

namespace NET
{
	class CMultiManager
	{
		public:
			enum EMultiType
			{
				EMT_NONE = -1,
				EMT_EPOLL,
				EMT_KQUEUE,
				EMT_SELECT,
			};
		
		private:

		public:
			CMultiManager();
			virtual ~CMultiManager();

			void					init(EMultiType type = EMT_EPOLL);
			void 					destroy();
			INT						setSize(INT size);

			inline EMultiType		getMultiType()							{ return m_eType; }
			void					enableEdgeTrigger(BOOLEAN on = TRUE);

			INT						addFileEvent(INT, INT);
			void					delFileEvent(INT, INT);
			INT						eventLoop(void*);
			void					addTimer(const CTimer*);

		private:
			const CTimer*			getNearestTimer();

		private:
			EMultiType				m_eType;
			
			IMultiBase* 			m_pBase;
			EVENT_LOOP*				m_pEventLoop;
	};
}
#endif
