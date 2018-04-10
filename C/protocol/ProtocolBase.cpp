#include "NET_Protocol.hpp"

namespace NET 
{
	CProtocolBase::CProtocolBase()
	{
		;
	}

	CProtocolBase::~CProtocolBase()
	{
		;
	}

	int CProtocolBase::analyse(int size, char* buff)
	{
		CHECK_R( NULL != buff && size >= SIZE_HEADER_MANAGER, -1 );
		HEADER_MANAGER* pHeader = (HEADER_MANAGER*)buff;

		CHECK_R( pHeader->sync == SYNC_FLAG, -1 );
		CHECK_R( checkProtocol(pHeader->protocol), -1 );

		return pHeader->size - size;
	}

	int CProtocolBase::package(int type, OBJECT* src, char* des)
	{
		switch(type) {
		case EP_ECHO:
			return innerPackageECHO(src, des);
		case EP_PING:
			return innerPackagePING(src, des);
		default:
			return -1;
		}
	}

	int CProtocolBase::callSpecialFunc(int type, int size,const char* src, OBJECT* target)
	{
		CHECK_R(NULL != src, -1);
		switch(type) {
		case EP_ECHO: {
			printf("recv echo msg: %s", src);

			if ( NULL != target ) delete target;
				 target = new OBJECT( size );

				 memcpy(target->data, src, size);
			} break;
		case EP_PING: {
			 CHECK_R(size == SIZE_PING_MANAGER, -1);

			 PING_MANAGER* pPing = (PING_MANAGER*)src;

			 printf("recv ping msg, current: %d, deadLimit: %d\n", \
					 pPing->current, \
					 pPing->deadLimit);

			 CHECK_R(pPing->deadLimit != pPing->current, 1);	

			 if ( NULL != target ) delete target;
				 target = new OBJECT( size );

				 memcpy(pPing, src, SIZE_PING_MANAGER);
				 pPing = (PING_MANAGER*)target->data;
				 pPing->current += 1;
			 } break;
		default:
			 return -1;
		}
		return -1;
	}

	bool CProtocolBase::checkProtocol(int protocol)
	{
		if ( protocol > EP_INVALID && protocol < EP_MAX ) return true;

		return false;
	}

	int CProtocolBase::checkSize(int protocol, int size)
	{
		return 0;
	}

	int CProtocolBase::innerPackageECHO(OBJECT* src, char* des)
	{
		CHECK_R( NULL != src, -1 );

		if ( NULL != des ) delete des;

		des = new char(src->size + SIZE_HEADER_MANAGER);
		HEADER_MANAGER* pHeader = (HEADER_MANAGER*)des;
		pHeader->sync = SYNC_FLAG;
		pHeader->size = src->size + SIZE_HEADER_MANAGER;
		pHeader->protocol = EP_ECHO;

		memcpy(des + SIZE_HEADER_MANAGER, src, src->size);

		return pHeader->size;
	}

	int CProtocolBase::innerPackagePING(OBJECT* src, char* des)
	{
		CHECK_R( NULL != src, -1 );

		if ( NULL != des ) delete des;

		des = new char(src->size + SIZE_HEADER_MANAGER);
		HEADER_MANAGER* pHeader = (HEADER_MANAGER*)des;
		pHeader->sync = SYNC_FLAG;
		pHeader->size = src->size + SIZE_HEADER_MANAGER;
		pHeader->protocol = EP_PING;

		memcpy(des + SIZE_HEADER_MANAGER, src, src->size);

		return pHeader->size;
	}

	void CProtocolBase::innerPackageTIME()
	{

	}

	void CProtocolBase::innerPackageHEART()
	{

	}
}
