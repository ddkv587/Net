#ifndef __PROCESSORHPP__
#define __PROCESSORHPP__

#include <sys/types.h>
#include <sys/time.h>

#include "ThreadBase.hpp"
#include "IFileListener.hpp"

class CMultiplexManager;

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
			CMultiplexManager* m_pMultiplex;
	};
}
#endif
