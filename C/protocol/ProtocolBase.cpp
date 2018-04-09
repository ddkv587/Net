#include "NET_Protocol.hpp"

CProtocolBase::CProtocolBase()
{
	;
}

CProtocolBase::~CProtocolBase()
{
	;
}

int CProtocolBase::analyse(char* buff)
{
	if ( NULL == buff ) return -1;

	
	return 0;
}

int CProtocolBase::package(int type, OBJECT* src, char* des)
{

}

int CProtocolBase::callSpecialFunc(int type, OBJECT* target)
{

}

void CProtocolBase::innerPackageECHO(OBJECT* src, char* des)
{

}

void CProtocolBase::innerPackagePING()
{

}

void CProtocolBase::innerPackageTIME()
{

}

void CProtocolBase::innerPackageHEART()
{

}
