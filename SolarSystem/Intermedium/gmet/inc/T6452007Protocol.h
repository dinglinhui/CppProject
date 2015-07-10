/*
 * CT6452007Protocol.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef CT6452007PROTOCOL_H_
#define CT6452007PROTOCOL_H_

#include "pcol_dl645.h"
#include "GMProtocol.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// CT645Protocol
///////////////////////////////////////////////////////////////////////////////

typedef void (*PF_DL645_2007_SAVE)(CGMPoint*, void*, DWORD, void*); ////定义函数指针类型

class CT645_2007Protocol: public CGMProtocol {
public:
	CT645_2007Protocol(BYTE nMPT, PF_DL645_2007_SAVE pf); /////构造函数参数：nMPT测量点类型，函数指针变量
	virtual ~CT645_2007Protocol(void);

	virtual int HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf, int nBufSize);
	virtual int HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf, int nBufSize);

	virtual int GetDefaultComDcb(ComDcb *pDcb);
	virtual int GetCommands(void *parms, Command *&pCMDs, PointType type);
	void GetRecord(BYTE val[4][4]);

private:
	PF_DL645_2007_SAVE m_pfSave;  ////定义函数指针

	WORD m_iTimes[4]; //断相次数
	int m_iMinutes[4]; //断相时间
	BYTE m_sBegin[4][4]; //断相开始时间
	BYTE m_sEnd[4][4]; //断相结束时间

};

} /* namespace pcols */

#endif /* CT6452007PROTOCOL_H_ */
