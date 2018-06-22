#ifndef __PROCESSORHPP__
#define __PROCESSORHPP__

namespace NET
{
	class CWorker : public CThreadBase, public IClientListener
	{
		public:
			class CProcessor
			{
				public:
					static INT read(const EVENT_LOOP* loop, INT fd, DATA*& dataBuff, INT mask);
					static INT write(const EVENT_LOOP* loop, INT fd, DATA*& dataBuff, INT mask);
			};
		public:
			CWorker();
			virtual ~CWorker();
			
			virtual INT 	addClient(INT fd);
			virtual void 	delClient(INT fd);
			
			BOOLEAN         isEnable()              { return TRUE; }
			BOOLEAN     	isFull() 				{ return FALSE; }

			inline UINT 	size() const            { return m_uiSize;	}

			void			setMaxSize(UINT uiSize);
                      
			CWorker(CWorker&) = delete;
			CWorker(const CWorker&) = delete;

		protected:
			void 			mainLoop(void*) override;
			void 			process(void*&);

		private:
			UINT 	   	 		m_uiSize;
			CMultiManager* 	    m_pMultiManager;
        
            ::std::mutex        m_mutex;
	};
}
#endif
