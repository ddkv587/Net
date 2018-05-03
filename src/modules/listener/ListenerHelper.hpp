#ifndef __LISTENERHELPERHPP__
#define __LISTENERHELPERHPP__

using namespace NET;

extern "C" {
	CListener* create()
	{
		return new CListener();
	}
}

extern "C" {
	void destroy(CListener* p)
	{
		if ( NULL != p ) delete p;
	}
}

#endif
