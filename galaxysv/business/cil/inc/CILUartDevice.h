/*
 * CILUartDevice.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef CILUARTDEVICE_H_
#define CILUARTDEVICE_H_

#include "../../cil/inc/CILDevice.h"
#include "OSComPort.h"

namespace syscil {

class CILUartDevice: public CILDevice {
public:
	CILUartDevice(BYTE nIdent, int nCacheSize, DWORD tmTran, DWORD tmRecv);
	virtual ~CILUartDevice(void);

	void Init(OSComPort *ptr);

	DWORD GetTranTimeout(void) const;
	DWORD GetRecvTimeout(void) const;
	DWORD GetTimesOfSent(int nSize);

protected:
	virtual int Write(BYTE *lpBuf, int nSize);
	virtual int Read(BYTE *lpBuf, int nSize, DWORD wTimeout);

private:
	OSComPort* m_pPort;
	DWORD m_tmTran;
	DWORD m_tmRecv;
};

} /* namespace syscil */

#endif /* CILUARTDEVICE_H_ */
