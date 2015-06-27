/*
 * SysApp.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef KDAPP_H_
#define KDAPP_H_

#include "OSDispatcherEx.h"
#include "CILService.h"
#include "CILSchedule.h"
#include "CILDispatcher.h"

namespace kding {
using namespace osext;
using namespace syscil;

class SysApp: public OSDispatcherEx {
public:
	virtual ~SysApp();
	SysApp *GetAppInstance();

	CILService m_objCIL;
	CILSchedule m_objSch;
	CILDispatcher m_objDsp;

protected:
	virtual OSRet OSInitHook(void) override final;
	virtual int OnHandleMessage(OSMessage *msg) override final;

private:
	SysApp();
	int Initialize(int *ret = nullptr);

	static SysApp *theApp;
};

} /* namespace KDING */

#endif /* KDAPP_H_ */
