#ifndef __MULTIMANAGERHPP__
#define __MULTIMANAGERHPP__

namespace NET
{
	class CMultiManager
	{
		public:
			class IMultiBase
			{
				public:
					virtual ~IMultiBase() {}

					virtual	void			init() = 0;
					virtual void			destroy() = 0;

					virtual UINT            setSize(UINT size) = 0;
					virtual INT             addFileEvent(INT fd, INT mask, EVENT_LOOP* eventLoop) = 0;
					virtual void            delFileEvent(INT fd, INT mask, EVENT_LOOP* eventLoop) = 0;
					virtual	INT				eventLoop(void* timeout, ::std::vector<FIRED_EVENT>& lstFired) = 0;
			};

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

			void							init(EMultiType type = EMT_EPOLL);
			void 							destroy();
			UINT							setSize(UINT size);

			inline EMultiType				getMultiType()						{ return m_eType; }
			
			INT								addFileEvent(INT, INT, fileProc, DATA*);
			void							delFileEvent(INT, INT);
			INT								eventLoop(void*);
			void							addTimer(const CTimer*);
			const ::std::vector<FIRED_EVENT>&		getFiredList()				{ return m_lstFired; }

		
			static UINT						setEventLoopSize(UINT size);	
			static const EVENT_LOOP*		getEventLoop()						{ return s_pEventLoop; }
		
			//for epoll
			void							enableEdgeTrigger(BOOLEAN on = TRUE);

		private:
			const CTimer*					getNearestTimer();		
			
		private:
			UINT							m_uiSize;
			EMultiType						m_eType;		
			IMultiBase* 					m_pBase;
			::std::vector<FIRED_EVENT>		m_lstFired;

			//static file map, ( sizeof(FILE_EVENT) + sizeof(FIRED_EVENT) ) * MAX_FILE_SIZE + ...
			static ::std::mutex				m_mutex;
			static EVENT_LOOP*				s_pEventLoop;		
	};
}
#endif
