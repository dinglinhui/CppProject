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
#include "MemPool.h"

namespace galaxy {
	using namespace osext;
	using namespace syscil;
	using namespace utils;

	class SysApp: public OSDispatcherEx {
	public:
		virtual ~SysApp();
		static SysApp *GetAppInstance();

		CILService m_objCIL;
		CILSchedule m_objSch;
		CILDispatcher m_objDsp;
		static MemPool<char> mempool;

	protected:
		virtual OSRet OSInitHook(void) override final;
		virtual int OnHandleMessage(OSMessage *msg) override final;

	private:
		SysApp();
		int Initialize(int *ret = nullptr);

		static SysApp *theApp;

	};

} /* namespace galaxy */

#endif /* KDAPP_H_ */
