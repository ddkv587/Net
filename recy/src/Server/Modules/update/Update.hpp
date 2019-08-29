#ifndef __UPDATEHPP__
#define __UPDATEHPP__

namespace NET
{
	class CUpdate : public CThread
	{
		public:
			CUpdate();
			virtual ~CUpdate();

		protected:
			void 	mainLoop(void* arg);
	};
}
#endif
