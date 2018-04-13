#ifndef __PROCESSORHPP__
#define __PROCESSORHPP__

namespace NET
{
	class CProcessor : public ThreadBase, public IFileListener
	{
		public:
			CProcessor();
			virtual ~CProcessor();
			
			virtual int 	addFileEvent(int fd, int mask);
			virtual void 	delFileEvent(int fd, int mask);
			
			unsigned int 	size() const { return m_uiSize;	}

			CProcessor(CProcessor&) = delete;
			CProcessor(const CProcessor&) = delete;

		protected:
			void mainLoop(void* arg);

		private:
			unsigned int 	m_uiSize;
			CMultiBase* 	m_pMultiplex;
	};
}
#endif
