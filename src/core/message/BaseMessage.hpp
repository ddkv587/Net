#ifndef __BASEMESSAGEHPP__
#define __BASEMESSAGEHPP__

namespace NET
{
	class CBaseMessage
	{
		public:
			enum EMessageType
			{
				EMT_INVALID = -1,
				EMT_ACCEPT,
				EMT_SEND,
				EMT_RECV,
				EMT_UPDATE,
				EMT_RESET,
				EMT_OTHER,
				EMT_MAX
			};

		public:
			CBaseMessage()
				: m_type(EMT_INVALID)
			{
				;
			}

			virtual ~CBaseMessage() {}

			EMessageType			getMessageType() 		{ return m_type; } 
			virtual void 			handleMessage(void*) = 0;

		protected:
			CBaseMessage(CBaseMessage&) = delete;
			CBaseMessage(const CBaseMessage&) = delete;
			CBaseMessage& operator=(const CBaseMessage&) = delete;

		protected:
			EMessageType	m_type;
	};
}
#endif
