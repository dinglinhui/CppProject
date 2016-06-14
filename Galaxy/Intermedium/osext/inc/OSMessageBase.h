/*
 * OSMessageBase.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef OSMESSAGEBASE_H_
#define OSMESSAGEBASE_H_

#include <mutex>
#include "CircleBuff.h"
#include "OSDefs.h"

namespace osext {
class OSMessageBase;
class OSMessage {
public:
	OSMessage(void);

	OSMessageBase* m_pSource;   // 发出该消息的对象
	MSGType m_nCmd;      // 命令
	DWORD m_wParam;    // 参数1
	DWORD m_lParam;    // 参数2
	bool m_bAsyn;     // 异步标记
	void* m_pACT;      // 异步完成标识

	static void* operator new(size_t nSize);
	static void operator delete(void *p, size_t nSize);
};

class OSMessageBase {
public:
	OSMessageBase(int nQueueBuffSize = 256);
	virtual ~OSMessageBase(void);

	bool isOK() const {
		return m_bOk;
	}

	int PostMessage(OSMessageBase *pTarget, OSMessage *msg);
	int PostMessage(OSMessageBase *pTarget, MSGType nCmd, DWORD wParam, DWORD lParam, void* act = nullptr);

	int SendMessage(OSMessageBase *pTarget, OSMessage *msg);
	int SendMessage(OSMessageBase *pTarget, MSGType nCmd, DWORD wParam, DWORD lParam);

protected:
	virtual int OnHandleMessage(OSMessage *msg);
	virtual int ReceiveMessage(OSMessage *msg);

	bool QueueInitialize(void);
	int Post(OSMessage *msg);
	int Pend(OSMessage *&msg, DWORD dwWaitTime = 0);

	int Write(OSMessage *msg);
	int Read(OSMessage *msg);

private:
	bool m_bOk;
	//
	std::mutex m_mutex;
	utils::CCircleBuff m_Queue;
};

} /* namespace osext */

#endif /* OSMESSAGEBASE_H_ */
