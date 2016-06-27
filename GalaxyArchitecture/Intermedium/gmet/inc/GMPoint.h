/*
 * GMPoint.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef GMPOINT_H_
#define GMPOINT_H_

#include <OSComPort.h>
#include "Command.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Class definied in other files
///////////////////////////////////////////////////////////////////////////////

class CGMProtocol;
class CGMScheduler;

typedef enum tag_PointType {
	Single_Point = 1,
	Mul_Point = 2
} PointType;

///////////////////////////////////////////////////////////////////////////////
// CGMPoint
///////////////////////////////////////////////////////////////////////////////

class CGMPoint {
public:
	CGMPoint(CGMScheduler *parent);
	virtual ~CGMPoint(void);

	virtual int RequestHook(Command *pCMD);

	int GetBaseComDcb(ComDcb *pDcb);
	int GetSendComDcb(Command *pCMD, ComDcb *pDcb);
	int GetRecvComDcb(Command *pCMD, ComDcb *pDcb);

	int GetTranTimeOut(Command *pCMD = nullptr);
	int GetRecvTimeOut(Command *pCMD = nullptr);

	int MakeCommands(void *parm, Command *&pCMDs);

	int HandleTx(Command *pCMD, BYTE *lpBuf, int nBufSize);
	int HandleRx(Command *pCMD, BYTE *lpBuf, int nBufSize);

	int GetRetryCount(void);

	// basic parameters of measure point
	int m_TN;					// measure point's id
	int m_nActive;				// active or not
	int m_FailTimes;			// 失败次数090523gs
	PointType m_nPointType;

	// rs-485 parameters
	int m_nPort;				// com-port
	int m_nBaud;				// com-baudrate
	int m_nDatabits;			// com-data bits
	int m_nStopbits;			// com-stop bits
	int m_nParity;				// com-partiy

	// protocol parameters
	BYTE m_host[6];				// meter address
	char m_user[8];				// entry-user name
	char m_password[8];			// entry-password

	CGMPoint* m_pNext;		// next measure point of list
	CGMProtocol* m_pProtocol;	// communication protocol
	CGMScheduler* m_pScheduler;	// gmet scheduler

public:
	// 增加表计时间判断标识
	BYTE m_nMPTimeEnable;
};

///////////////////////////////////////////////////////////////////////////////
// Definied in sub system
///////////////////////////////////////////////////////////////////////////////

//extern BOOL ConvertBaud(ComBaud &ret, int nSpeed);
//extern BOOL ConvertDatabit(ComDataBit &ret, int nBit);
//extern BOOL ConvertParity(ComParity &ret, int nParity);
//extern BOOL ConvertStopbit(ComStopbit &ret, int nBit);

} /* namespace pcols */

#endif /* GMPOINT_H_ */
