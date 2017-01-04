/*
 * GMDevice.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef GMDEVICE_H_
#define GMDEVICE_H_

#include "GMPoint.h"
#include "OSComPort.h"
#include <thread>

namespace pcols {
using namespace osext;

///////////////////////////////////////////////////////////////////////////////
// CGMDevice
///////////////////////////////////////////////////////////////////////////////

class CGMDevice {
public:
	CGMDevice(void);
	virtual ~CGMDevice(void);

	BOOL Open(OSComPort *pPort, WORD wCacheSize);
	BOOL IsOpen(void);

	BOOL SetComDcb(ComDcb *pDcb);
	void GetComDcb(ComDcb *pDcb);
	int SendAndWaitRespond(CGMPoint *pObj, Command *pCMD);

	void SaveComDcb(void);
	void RestoreComDcb(void);

	void ClearCounter(void);
	DWORD GetSendCount(void);
	DWORD GetRecvCount(void);

private:
	int SendFrame(CGMPoint *pObj, Command *pCMD);
	int WaitMeterData(CGMPoint *pObj, Command *pCMD);
	int GetTimesOfSent(int nSize, ComDcb *pDcb);

	DWORD m_dwSends;
	DWORD m_dwRecvs;
	ComDcb m_hDCB;
	WORD m_wCacheSize;
	BYTE* m_pCache;
	OSComPort* m_pSerialport;
};

} /* namespace pcols */

#endif /* GMDEVICE_H_ */
