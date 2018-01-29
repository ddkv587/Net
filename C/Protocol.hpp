#ifndef __PROCOTOLHPP__
#define __PROCOTOLHPP__

#define SYNC_FLAG 0x03302112

enum EProcotol
{
	EP_INVALID = -1,
	EP_ECHO,
	EP_PING,
	EP_TIME,
	EP_HEART,
	EP_DISMISS,
	EP_MAX
};

typedef struct tagSocketHeader
{
	int sync;
	unsigned int length;
	EProcotol protocol;
} HEADER_MANAGER;

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

const static int SIZE_HEADER_MANAGER = sizeof(HEADER_MANAGER);
const static int SIZE_PING_MANAGER = sizeof(PING_MANAGER);
const static int SIZE_TIME_MANAGER = sizeof(TIME_MANAGER);
const static int SIZE_HEART_MANAGER = sizeof(HEART_MANAGER);
#endif
