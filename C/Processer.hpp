#ifndef __PROCESSORHPP__
#define __PROCESSORHPP__

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#include "ThreadBase.hpp"

class CMultiplexManager;
class IFileListener;
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
