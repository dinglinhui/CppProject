/*
 * CILService.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef CILSERVICE_H_
#define CILSERVICE_H_

#include "CILDevice.h"
#include "OSThreadEx.h"
#include "OSThreadPool.h"
namespace CIL {
class CILService: public OSExt::OSThreadEx {
public:
	CILService();
	virtual ~CILService();

	void Install(CILDevice *pDevice);
	void UnInstall(CILDevice *pDevice);
	void SetSchedPool(OSExt::OSThreadPool *pPool);
	void SetPackEntry(OSExt::OSMessageBase *ptr);

	CILDevice* FindDevice(int nIdent);
	OSExt::OSThreadPool* GetSchedPool(void) const;
	OSExt::OSMessageBase* GetPackEntry(void) const;

protected:
	virtual int OSInitHook(void);
	virtual int ReceiveMessage(OSExt::Message *msg);
	virtual int OnHandleMessage(OSExt::Message *msg);
	virtual int Run() override final;

private:
	CILDevice* m_plstDevice;
	OSExt::OSMessageBase* m_pEntry;
	OSExt::OSThreadPool* m_pShedPool;
};

} /* namespace OSExt */

#endif /* CILSERVICE_H_ */
