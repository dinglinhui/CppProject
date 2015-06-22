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
class Message {
public:
	Message(void);

	OSMessageBase* m_pSource;   // 发出该消息的对象
	DWORD m_nCmd;      // 命令
	DWORD m_wParam;    // 参数1
	DWORD m_lParam;    // 参数2
	bool m_bAsyn;     // 异步标记
	void* m_pACT;      // 异步完成标识
};

class OSMessageBase {
public:
	OSMessageBase(int nQueueBuffSize = 256);
	virtual ~OSMessageBase(void);

	bool isOK() const {
		return m_bOk;
	}

	int PostMessage(OSMessageBase *pTarget, Message *msg);
	int PostMessage(OSMessageBase *pTarget, DWORD nCmd, DWORD wParam, DWORD lParam, void* act = NULL);

	int SendMessage(OSMessageBase *pTarget, Message *msg);
	int SendMessage(OSMessageBase *pTarget, DWORD nCmd, DWORD wParam, DWORD lParam);

protected:
	virtual int OnHandleMessage(Message *msg);
	virtual int ReceiveMessage(Message *msg);

	bool QueueInitialize(void);

	int Write(Message *msg);
	int Read(Message *msg);

private:
	bool m_bOk;
	//
	std::mutex m_mutex;
	utils::CCircleBuff m_Queue;
};

} /* namespace osext */

#endif /* OSMESSAGEBASE_H_ */
