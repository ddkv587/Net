#ifndef __PROCESSORHPP__
#define __PROCESSORHPP__

namespace NET
{
	class CProcessor : public ThreadBase, public IFileListener
	{
		public:
			CProcessor();
			virtual ~CProcessor();
			
			int addFileEvent(int fd, int mask);
			void delFileEvent(int fd, int mask);

			CProcessor(CProcessor&) = delete;
			CProcessor(const CProcessor&) = delete;

		protected:
			void mainLoop(void* arg);

		private:
			CMultiBase* m_pMultiplex;
	};
}
#endif
