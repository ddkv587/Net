#ifndef __PROCESSORHPP__
#define __PROCESSORHPP__

namespace NET
{
	class CProcessor : public CThreadBase, public IClientListener
	{
		public:
			CProcessor();
			virtual ~CProcessor();
			
			virtual INT 	addCLient(int fd);
			virtual void 	delClient(int fd);
			
			inline UINT 	size() const            { return m_uiSize;	}
            BOOLEAN         isEnable()              { return TRUE; }
            BOOLEAN         isFull()                { return m_uiSize < 1024; }

			CProcessor(CProcessor&) = delete;
			CProcessor(const CProcessor&) = delete;

		protected:
			void mainLoop(void* arg);

		private:
			unsigned int 	    m_uiSize;
			CMultiBase* 	    m_pMultiplex;
        
            ::std::mutex        m_mutex;
	};
}
#endif
