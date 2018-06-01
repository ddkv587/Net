#ifndef __ITIMERLISTENERHPP__
#define __ITIMERLISTENERHPP__

class ITimerListener
{
	public:
		virtual void onTimer(const POINT pTimer, UINT uiCurrentCount) = 0;
		virtual void onTimerComplete(const POINT pTimer) = 0;
};
#endif
