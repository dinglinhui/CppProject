/*
 * OSDispatcherEx.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef OSDISPATCHEREX_H_
#define OSDISPATCHEREX_H_

#include "OSDispatcher.h"
#include "OSMessageBase.h"
#include "OSThreadEx.h"
#include "MemPool.hpp"
#include "Events.hpp"

namespace osext {
using Delegate = std::function<void(int, int)>;
using Event = utils::Events<Delegate>;

class OSThreadEx;
class OSDispatcherEx: public OSDispatcher, public OSMessageBase {
public:
	OSDispatcherEx();
	virtual ~OSDispatcherEx();

	virtual OSRet Start(void) override;
	virtual OSRet Stop(void) override;
	//
	int Register(OSThreadEx *pService);
	int UnRegister(OSThreadEx *pService);
	//
	void SendMessageToDescendants(OSMessage *msg);
	void SendMessageToDescendants(MSGType nCmd, DWORD wParam, DWORD lParam);
	void PostMessageToDescendants(OSMessage *msg);
	void PostMessageToDescendants(MSGType nCmd, DWORD wParam, DWORD lParam);

	OSThreadEx* Find(std::thread::id nID) const;

	friend OSDispatcherEx* GetOSDispatcher(void);

public:
	// for timer scheduler, call in cpu tick process
	static void TimerScheduler(void);
	// for message get & put
	void *GetMessagePtr(void);
	void PutMessagePtr(void *p);

protected:
	virtual OSRet Run(void) override;
	virtual OSRet OSInitHook(void) override;

	virtual int OnHandleMessage(OSMessage *msg) override;
	virtual int ReceiveMessage(OSMessage *msg) override;

	void InitThreadList(void);

private:
	OSThreadEx* m_pThreadList;

	Event event;
	static utils::MemPool<OSMessage> mempool;
	static OSDispatcherEx* m_pDispatcher;
};

//global paramter for system
extern OSDispatcherEx* GetOSDispatcher(void);

//global message
int OSSendMessage(int nPrio, MSGType nCmd, DWORD wParam, DWORD lParam);
int OSPostMessage(int nPrio, MSGType nCmd, DWORD wParam, DWORD lParam);

//global message get & put
void *OSGetMessagePtr(void);
void OSPutMessagePtr(void *);

} /* namespace osext */

#endif /* OSDISPATCHEREX_H_ */
