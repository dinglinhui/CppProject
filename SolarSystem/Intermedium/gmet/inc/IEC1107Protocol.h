/*
 * IEC1107Protocol.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef INTERMEDIUM_GMET_INC_IEC1107PROTOCOL_H_
#define INTERMEDIUM_GMET_INC_IEC1107PROTOCOL_H_

#include "pcol_iec1107.h"
#include "GMProtocol.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// CIEC1107Protocol
///////////////////////////////////////////////////////////////////////////////
typedef void (*PF_IEC_SAVE)(CGMPoint*, void*, DWORD, void*);

class CIEC1107Protocol: public CGMProtocol {
public:
	CIEC1107Protocol(BYTE nMPT, PF_IEC_SAVE pSave);
	virtual ~CIEC1107Protocol(void);

	virtual int HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
			int nBufSize);
	virtual int HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
			int nBufSize);

	virtual int GetDefaultComDcb(ComDcb *pDcb);
	virtual int GetSendComDcb(Command *pCMD, ComDcb *pDcb);
	virtual int GetRecvComDcb(Command *pCMD, ComDcb *pDcb);

	virtual int GetTranTimeOut(Command *pCMD = nullptr);
	virtual int GetRecvTimeOut(Command *pCMD = nullptr);

	virtual int GetCommands(void *parms, Command *&pCMDs, PointType type);

private:
	PF_IEC_SAVE m_pfSave;
	BYTE m_1107type;/*-具体哪一种1107-*/
};

} /* namespace pcols */

#endif /* INTERMEDIUM_GMET_INC_IEC1107PROTOCOL_H_ */
