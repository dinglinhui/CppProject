/*
 * KDApp.h
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

class KDApp: public OSDispatcherEx {
public:
	virtual ~KDApp();
	KDApp *GetAppInstance();

	CILService m_objCIL;
	CILSchedule m_objSch;
	CILDispatcher m_objDsp;

protected:
	virtual OSRet OSInitHook(void) override final;
	virtual int OnHandleMessage(OSMessage *msg) override final;

private:
	KDApp();
	int Initialize(int *ret = nullptr);

	static KDApp *theApp;
};

} /* namespace KDING */

#endif /* KDAPP_H_ */
