/*
 * ABBProtocol.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef ABBPROTOCOL_H_
#define ABBPROTOCOL_H_

#include"GMProtocol.h"

namespace pcols {

typedef void (*PF_ABB_SAVE)(CGMPoint*, void*, DWORD, void*);

class CABBProtocol: public CGMProtocol {
public:
	CABBProtocol(BYTE nMPT, PF_ABB_SAVE);
	virtual ~CABBProtocol(void);
	virtual int HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf, int nBufSize);
	virtual int HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf, int nBufSize);
	virtual int GetDefaultComDcb(ComDcb *pDcb);
	virtual int GetCommands(void *parms, Command *&pCMDs, PointType);
	void ParseClass11(BYTE frmIndex, CGMPoint* pObj, Command* pCMD, BYTE *lpBuf, int nBufSize);
	void ParseClass0(BYTE *lpBuf, int nBufSize);
	void ParseClass1(CGMPoint* pObj, Command* pCMD, BYTE *lpBuf, int nBufSize);
	// 2009-3-30
	virtual int GetRetryCount(void);
	// end

private:
	PF_ABB_SAVE m_pfSave;
	char m_farpwd[9];
//	char m_keypwd[8];
	bool bHaveNextFrame; //有没有后续帧
	bool bFirstReadClass; //是否第一次读
	void DisposeDL(int *pValue, BYTE nType, BYTE *lpBuf, BYTE dotPos); //处理电量
	void DisposeXL(int *pValue, BYTE nType, BYTE *lpBuf, BYTE dotPos); //处理需量
	void DisposeXLSJ(int *pValue, BYTE nType, BYTE *lpBuf); //处理需量时间
	void DisposeDY(BYTE nDy, int *pValue, BYTE *lpBuf); //处理电压
	int GetDotVal(int idotPos);
	BYTE dl_DotPos;
	BYTE xl_DotPos;
	double zxyg_dl; //正向有功总电量
	double zxwg_dl; //正向无功总电量
	//double zwg_dl;//总无功===四象限无功
	WORD dy;
	double realdata;
	double dl;	//电量
	double xl;	//需量
	//int value[64/sizeof(int)];
};

} /* namespace pcols */

#endif /* ABBPROTOCOL_H_ */
