/*
 * CT645Protocol.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef CT645PROTOCOL_H_
#define CT645PROTOCOL_H_

#include "pcol_dl645.h"
#include "GMProtocol.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// CT645Protocol
///////////////////////////////////////////////////////////////////////////////

typedef void (*PF_DL645_SAVE)(CGMPoint*, void*, DWORD, void*);

class CT645Protocol: public CGMProtocol {
public:
	CT645Protocol(BYTE nMPT, PF_DL645_SAVE pf);
	virtual ~CT645Protocol(void);

	virtual int HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
			int nBufSize);
	virtual int HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
			int nBufSize);

	virtual int GetDefaultComDcb(ComDcb *pDcb);
	virtual int GetCommands(void *parms, Command *&pCMDs, PointType type);

private:
	PF_DL645_SAVE m_pfSave;
};
} /* namespace pcols */

#endif /* CT645PROTOCOL_H_ */
