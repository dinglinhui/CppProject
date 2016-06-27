/*
 * GMProtocol.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "GMProtocol.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// CGMProtocol
///////////////////////////////////////////////////////////////////////////////

CGMProtocol::CGMProtocol(BYTE nType, BYTE nChannel) :
		m_pNext( nullptr),
		m_nType(nType),
		m_nChannel(nChannel) {
}

CGMProtocol::~CGMProtocol(void) {
}

int CGMProtocol::HandleTx(CGMPoint*, Command*, BYTE*, int) {
	return 0;
}

int CGMProtocol::HandleRx(CGMPoint*, Command*, BYTE*, int) {
	return 0;
}

int CGMProtocol::GetCommands(void*, Command*&, PointType) {
	return 0;
}

int CGMProtocol::GetTranTimeOut(Command*) {
	return MP_TRANTIMEOUT;
}

int CGMProtocol::GetRecvTimeOut(Command*) {
	return MP_RECVTIMEOUT;
}

int CGMProtocol::GetDefaultComDcb(ComDcb*) {
	return -1;
}

int CGMProtocol::GetSendComDcb(Command*, ComDcb*) {
	return -1;
}

int CGMProtocol::GetRetryCount(void) {
	return 2;	// default 2 times
}

int CGMProtocol::GetRecvComDcb(Command*, ComDcb*) {
	return -1;
}

} /* namespace pcols */
