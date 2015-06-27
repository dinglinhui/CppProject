/*
 * EDMIProtocol.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef INTERMEDIUM_GMET_INC_EDMIPROTOCOL_H_
#define INTERMEDIUM_GMET_INC_EDMIPROTOCOL_H_

#include <pcol_edmi.h>
#include "GMProtocol.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// CEDMIProtocol
///////////////////////////////////////////////////////////////////////////////

typedef void (*PF_EDMI_SAVE)(CGMPoint*, void*, DWORD, void*);

class CEDMIProtocol: public CGMProtocol {
public:
	CEDMIProtocol(BYTE nMPT, PF_EDMI_SAVE pSave);
	virtual ~CEDMIProtocol(void);

	virtual int HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
			int nBufSize);
	virtual int HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
			int nBufSize);

	virtual int GetCommands(void *parms, Command *&pCMDs, PointType type);

private:
	int GetCommandsEx(BYTE gtt, Command *&pCMDs);

	PF_EDMI_SAVE m_pfSave;
};

} /* namespace pcols */

#endif /* INTERMEDIUM_GMET_INC_EDMIPROTOCOL_H_ */
