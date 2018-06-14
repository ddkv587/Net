#ifndef __PROTOCOLHPP__
#define __PROTOCOLHPP__

#define SYNC_FLAG 0x03302112

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

typedef struct tagSocketHeader
{
	INT sync;
	INT size;
	INT protocol;
} HEADER_MANAGER;

typedef struct tagSocketPing
{
	INT current;
	INT deadLimit;
} PING_MANAGER;

typedef struct tagSocketTime
{
	INT second;
	INT millsecond;
	INT offet;
	INT type;
} TIME_MANAGER;

typedef struct tagSocketHeart
{
	INT type;
	INT time;
} HEART_MANAGER;

const static INT SIZE_HEADER_MANAGER = constexpr sizeof(HEADER_MANAGER);
char test[SIZE_HEADER_MANAGER];
const static INT SIZE_PING_MANAGER = constexpr sizeof(PING_MANAGER);
const static INT SIZE_TIME_MANAGER = constexpr sizeof(TIME_MANAGER);
const static INT SIZE_HEART_MANAGER = constexpr sizeof(HEART_MANAGER);
#endif
