#ifndef __MSGLISTENERHPP__
#define __MSGLISTENERHPP__

namespace NET
{
	class CMsgListener : CBaseMessage
	{
		public:
			virtual void 			addFileListener(IFileListener*);
			virtual void 			delFileListener(IFileListener*); 
			void 					printListener();

			virtual IFileListener* 	scheduling(::std::list<IFileListener*>&);

		private:
			::std::list<IFileListener*> m_lstListener;
	};
}
#endif
