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
		CHECK_R( NULL != buff, -1 );
		HEADER_MANAGER* pHeader = (HEADER_MANAGER*)buff;

		CHECK_R( pHeader->sync == SYNC_FLAG, -1 );
		CHECK_R( checkProtocol(pHeader->protocol), -1 );

		return pHeader->size - size;
	}

	int CProtocolBase::package(int type,const OBJECT* src, char* &des)
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

	int CProtocolBase::callSpecialFunc(int type, int size,const char* src, OBJECT* &target)
	{
		CHECK_R(NULL != src, -1);
		switch(type) {
		case EP_ECHO: {
			printf("recv echo msg: %s", src);

			if ( NULL != target ) delete target;
				 target = new OBJECT( size );

				 memcpy(target->data, src, size);
			} return 0;
		case EP_PING: {
			 CHECK_R(size == SIZE_PING_MANAGER, -1);

			 PING_MANAGER* pPing = (PING_MANAGER*)src;
			 printf("recv ping msg, current: %d, deadLimit: %d\n", \
					 pPing->current, \
					 pPing->deadLimit);

			 CHECK_R(pPing->deadLimit != pPing->current, 1);	

			 if ( NULL != target ) delete target;
			 target = new OBJECT( SIZE_PING_MANAGER );

			 LOG(INFO) 
				<< CLog::format( "[%s, %d] begin to copy ping" ,__FILE__, __LINE__);									

			 memcpy(target->data, src, SIZE_PING_MANAGER);
			 pPing = (PING_MANAGER*)target->data;
			 pPing->current += 1;
			 LOG(INFO) 
				<< CLog::format( "[%s, %d] ready to send ping, target: %p" ,__FILE__, __LINE__, target);	
			 } return 0;
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

	int CProtocolBase::innerPackageECHO(const OBJECT* src, char* &des)
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

	int CProtocolBase::innerPackagePING(const OBJECT* src, char* &des)
	{
		LOG(INFO) 
			<< CLog::format( "[%s, %d] begin to innerPackagePING,size: %d" ,__FILE__, __LINE__, src->size);									

		CHECK_R( NULL != src, -1 );

		if ( NULL != des ) delete des;

		des = (char*)malloc(src->size + SIZE_HEADER_MANAGER);
		HEADER_MANAGER* pHeader = (HEADER_MANAGER*)des;
		pHeader->sync = SYNC_FLAG;
		pHeader->size = src->size;
		pHeader->protocol = EP_PING;

		memcpy(des + SIZE_HEADER_MANAGER, src->data, src->size);

		LOG(INFO) 
				<< CLog::format( "[%s, %d] ready send: %d" ,__FILE__, __LINE__, pHeader->size);									

		return pHeader->size + SIZE_HEADER_MANAGER;
	}

	void CProtocolBase::innerPackageTIME()
	{

	}

	void CProtocolBase::innerPackageHEART()
	{

	}
}
