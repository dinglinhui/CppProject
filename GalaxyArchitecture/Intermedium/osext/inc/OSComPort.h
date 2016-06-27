/*
 * OSComPort.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef OSCOMPORT_H_
#define OSCOMPORT_H_

#include "OSDefs.h"
#include "CircleBuff.h"

namespace osext {

class OSComPort {
public:
	OSComPort(ComPort nPort, ComDcb *dcb = nullptr, DWORD dwRxQueueLen = 512, DWORD dwTxQueueLen = 512);
	virtual ~OSComPort();

	//Implementation
public:
	int Open(ComBaud nBaud, ComDataBit databit, ComParity parity, ComStopbit stopbit);
	int Open(void);
	int Close(void);
	bool IsOpen(void) const {
		return m_bOpened;
	}

	//If dwMilliseconds is zero, WaitCommEvent function tests the object's state
	// and returns immediately. If dwMilliseconds is INFINITE,
	// WaitCommEvent function's time-out interval never elapses.
	ComEvent WaitCommEvent(DWORD dwMilliseconds);

	int Read(BYTE *buff, int length);
	int Write(const BYTE *Buff, const int length);
	//
	int GetRxDataLen(void);
	int GetTxDataLen(void);

	//config comm setting
	BOOL SetCommDCB(ComDcb *dcb);
	ComDcb *GetCommDCB(void) {
		return &m_hComDcb;
	}
	virtual int ConfigComBaud(ComBaud nBaud);
	//
	void Flush();

	//Override
protected:
	virtual int ConfigDCB();
	//
	virtual int _open(void) = 0;
	virtual int _close(void) = 0;
	virtual void _StartToSend(void) = 0;

protected:
	bool m_bOpened;
	//
	ComPort m_nPort;

	DWORD m_dwRxQueueLen;
	utils::CCircleBuff* m_hRxQueue;

	DWORD m_dwTxQueueLen;
	utils::CCircleBuff* m_hTxQueue;
	//
	ComDcb m_hComDcb;
};

} /* namespace osext */

#endif /* OSCOMPORT_H_ */
