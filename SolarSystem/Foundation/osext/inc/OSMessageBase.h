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

namespace OSExt {
/*
 * Msg
 */
#define FM_NULL                         0x0000
#define FM_ACT           				0x0001      // 异步完成消息
#define FM_GETSERVICE    				0x0002      // 获取系统服务
#define FM_CREATE                       0x0003
#define FM_DESTROY                      0x0004
#define FM_MOVE                         0x0005
#define FM_SIZE                         0x0006
#define FM_ACTIVATE                     0x0007
// 用户自定义消息起始码
#define FM_USER			 				0X00FF

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
	OSUtils::CCircleBuff m_Queue;
};

} /* namespace OSExt */

#endif /* OSMESSAGEBASE_H_ */
