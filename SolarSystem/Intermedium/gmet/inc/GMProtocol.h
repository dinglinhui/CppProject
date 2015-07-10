/*
 * GMProtocol.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef GMPROTOCOL_H_
#define GMPROTOCOL_H_

#include "GMPoint.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Constant definitions
///////////////////////////////////////////////////////////////////////////////

#define MP_RECVTIMEOUT	2	    // unit: 10ms
#define MP_TRANTIMEOUT	300	    // unit: 10ms

///////////////////////////////////////////////////////////////////////////////
// CGMProtocol
///////////////////////////////////////////////////////////////////////////////

class CGMProtocol {
public:
	CGMProtocol(BYTE nType, BYTE nChannel = 0);
	virtual ~CGMProtocol(void);

	BYTE GetType(void) const {
		return m_nType;
	}
	BYTE GetChannel(void) const {
		return m_nChannel;
	}

	virtual int HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf, int nBufSize);
	virtual int HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf, int nBufSize);

	virtual int GetCommands(void *parms, Command *&pCMDs, PointType type);

	virtual int GetTranTimeOut(Command *pCMD = NULL);
	virtual int GetRecvTimeOut(Command *pCMD = NULL);

	virtual int GetDefaultComDcb(ComDcb *pDcb);
	virtual int GetSendComDcb(Command *pCMD, ComDcb *pDcb);
	virtual int GetRecvComDcb(Command *pCMD, ComDcb *pDcb);

	virtual int GetRetryCount(void);

	CGMProtocol *m_pNext;

protected:
	BYTE m_nType;
	BYTE m_nChannel;
};

} /* namespace pcols */

#endif /* GMPROTOCOL_H_ */
