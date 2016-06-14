#ifndef __CIL_PACKETFACTORY_H__
#define __CIL_PACKETFACTORY_H__

#include <iostream>
#include <cstdio>
#include <cstring>
#include <typeinfo>
#include "CILAbstractFactory.h"
#include "CILPacketFactoryImpl.h"

///////////////////////////////////////////////////////////////////////////////
// CILPacketFactory
///////////////////////////////////////////////////////////////////////////////
namespace syscil {

/* create object by class name */
void * Create(const char * class_name);

class CILPacketFactory: public CILAbstractFactory {
public:
	static void SetImplementation(CILPacketFactoryImpl *ptr);

	static CILPacket* Create(PCOL_TYPE type);

private:
	static CILPacketFactoryImpl *ms_pImpl;
};
}

#endif // __CIL_PACKETFACTORY_H__

///////////////////////////////////////////////////////////////////////////////
// End of file: CILPacketFactory.h
///////////////////////////////////////////////////////////////////////////////
