/*
 * CILFKPacketFactoryImpl.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "CILFKPacketFactoryImpl.h"
#include "CILFKPacket.h"
#include "CIL645Packet.h"

namespace syscil {

CILFKPacketFactoryImpl::CILFKPacketFactoryImpl() {
}

CILFKPacketFactoryImpl::~CILFKPacketFactoryImpl() {
}

CILPacket* CILFKPacketFactoryImpl::Create(PCOL_TYPE type) {
	CILPacket *pk = nullptr;

	switch (type) {
	case PCOL_FK: {
		pk = (CILPacket *)new CILFKPacket();
		break;
	}
	case PCOL_DL645: {
		pk = (CILPacket *)new CIL645Packet();
		break;
	}
	default: {
		pk = new CILPacket(type);
		break;
	}
	}

	return pk;
}

} /* namespace syscil */
