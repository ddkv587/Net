#ifndef __MSGLISTENERHPP__
#define __MSGLISTENERHPP__

namespace NET
{
	class CMsgListener : CBaseMessage
	{
		public:
			CMsgListener(int, struct sockaddr_in);
			virtual ~CMsgListener();

			virtual void 	handleMessage(void*);

			int 					getClientID() const		{ return m_clientID; }
			struct sockaddr_in&		getClientAddr() const 	{ return m_clientAddr; }

		protected:
			CMsgListener(CMsgListener&) = delete;
			CMsgListener(const CMsgListener&) = delete;

		private:
			int					m_clientID; 
			struct sockaddr_in	m_clientAddr;
	};
}
#endif
