/*
 * CILFKPacketFactoryImpl.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef CILFKPACKETFACTORYIMPL_H_
#define CILFKPACKETFACTORYIMPL_H_

#include "CILPacketFactoryImpl.h"

namespace syscil {

class CILFKPacketFactoryImpl: public CILPacketFactoryImpl {
public:
	CILFKPacketFactoryImpl();
	virtual ~CILFKPacketFactoryImpl();

	virtual CILPacket* Create(PCOL_TYPE type) override;
};

} /* namespace syscil */

#endif /* CILFKPACKETFACTORYIMPL_H_ */
