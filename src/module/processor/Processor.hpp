#ifndef __PROCESSORHPP__
#define __PROCESSORHPP__

namespace NET
{
	class CProcessor : public CThreadBase, public IFileListener
	{
		public:
			CProcessor();
			virtual ~CProcessor();
			
			virtual int 	addFileEvent(int fd, int mask);
			virtual void 	delFileEvent(int fd, int mask);
			
			unsigned int 	size() const { return m_uiSize;	}

		protected:
			CProcessor(CProcessor&) = delete;
			CProcessor(const CProcessor&) = delete;
			
			void mainLoop(void* arg);

		private:
			unsigned int 	m_uiSize;
			CMultiBase* 	m_pMultiplex;
	};
}
#endif
