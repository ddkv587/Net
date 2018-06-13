#ifndef __PROCESSORHPP__
#define __PROCESSORHPP__

namespace NET
{
	class CProcessor : public CThreadBase, public IClientListener
	{
		public:
			CProcessor();
			virtual ~CProcessor();
			
			virtual INT 	addClient(INT fd);
			virtual void 	delClient(INT fd);
			
			inline UINT 	size() const            { return m_uiSize;	}
            BOOLEAN         isEnable()              { return TRUE; }
            BOOLEAN         isFull()                { return m_uiSize < 1024; }

			CProcessor(CProcessor&) = delete;
			CProcessor(const CProcessor&) = delete;

		protected:
			void mainLoop(void* arg);

		private:
			UINT 	   	 		m_uiSize;
			CMultiManager* 	    m_pMultiManager;
        
            ::std::mutex        m_mutex;
	};
}
#endif
