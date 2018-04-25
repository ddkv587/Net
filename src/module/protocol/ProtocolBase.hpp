#ifndef __PROTOCOLBASEHPP__
#define __PROTOCOLBASEHPP__

namespace NET
{
	class CProtocolBase : public IProtocol 
	{
		public:
			enum EProcotol
			{
				EP_INVALID = 0,
				EP_ECHO,
				EP_PING,
				EP_TIME,
				EP_HEART,
				EP_DISMISS,
				EP_MAX
			};

			typedef struct tagSocketPing
			{
				int current;
				int deadLimit;
			} PING_MANAGER;

			typedef struct tagSocketTime
			{
				int second;
				int millsecond;
				int offet;
				int type;
			} TIME_MANAGER;

			typedef struct tagSocketHeart
			{
				int type;
				int time;
			} HEART_MANAGER;

			const static int SIZE_PING_MANAGER = sizeof(PING_MANAGER);
			const static int SIZE_TIME_MANAGER = sizeof(TIME_MANAGER);
			const static int SIZE_HEART_MANAGER = sizeof(HEART_MANAGER);

		public:
			CProtocolBase();
			virtual ~CProtocolBase();

			virtual int analyse(int, char*);
			virtual int package(int, const OBJECT*, char*&);

			virtual int callSpecialFunc(int, int, const char*, OBJECT*&);

		private:
			bool 	checkProtocol(int);
			int  	checkSize(int, int);
			int		innerPackageECHO(const OBJECT*, char*&);
			int 	innerPackagePING(const OBJECT*, char*&);
			void	innerPackageTIME();
			void 	innerPackageHEART();

			CProtocolBase(CProtocolBase&) = delete;
			CProtocolBase(const CProtocolBase&) = delete;
	};
}
#endif
