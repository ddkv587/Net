#ifndef __IFILELISTENERHPP__
#define __IFILELISTENERHPP__

class IFileListener
{
	public:
		virtual int addFileEvent(int, int) = 0;
		virtual void delFileEvent(int, int) = 0;

		virtual unsigned int size() const = 0;
};
#endif
