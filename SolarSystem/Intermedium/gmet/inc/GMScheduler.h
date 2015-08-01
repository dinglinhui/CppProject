/*
 * GMScheduler.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef GMSCHEDULER_H_
#define GMSCHEDULER_H_

#include "OSThreadEx.h"
//#include <OSSemaphore.h>
#include "GMDevice.h"
#include "GMTask.h"

namespace pcols {
using namespace osext;

///////////////////////////////////////////////////////////////////////////////
// CGMScheduler
///////////////////////////////////////////////////////////////////////////////
class CGMScheduler: public OSThreadEx {
	friend class CGMTask;

public:
	CGMScheduler(int nPrio, int nStackSize, int nQueueSize);
	virtual ~CGMScheduler(void);

//	virtual void Dump( CDumpContext *pContext );
	virtual void OnSuccess(CGMTask *pTask, WORD TN, GMRET ret);
	virtual void OnFinish(CGMTask *pTask, WORD TN, GMRET ret);

	int GetPort(void) const;
	int Create(int nPort, OSComPort *pPort, WORD wCacheSize);

	CGMPoint* FindFirst(void);
	CGMPoint* FindNext(CGMPoint *pObj);

	CGMPoint* FindFirstActive(void);
	CGMPoint* FindNextActive(CGMPoint *pObj);

	CGMPoint* Find(WORD nTN);

protected:
	virtual OSRet OSInitHook(void);
	virtual int OnHandleMessage(OSMessage *msg);

	virtual int GetRetryCount(CGMPoint* pMP);

	void AddGMPoint(CGMPoint *pMP);
	void DelGMPoint(CGMPoint *pMP);

	void MeterReset(void);
private:
	void ResetComDcb(CGMPoint *pObj);
	int RequestData(CGMPoint *pObj, Command *pSend);

	void RemoveHead(void);
	void AddToTail(CGMTask *pTask);

	int m_nPort;
	CGMPoint* m_plstGMP;
	CGMDevice* m_pGMDevice;
	CGMTask* m_plstTask;
//	OSSemaphore m_hLock;
};

} /* namespace pcols */

#endif /* GMSCHEDULER_H_ */
