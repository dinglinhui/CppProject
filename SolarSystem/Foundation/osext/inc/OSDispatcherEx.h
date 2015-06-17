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

namespace OSExt {

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
	void SendMessageToDescendants(Message *Message);
	void SendMessageToDescendants(DWORD nCmd, DWORD wParam, DWORD lParam);
	void PostMessageToDescendants(Message *Message);
	void PostMessageToDescendants(DWORD nCmd, DWORD wParam, DWORD lParam);

	OSThreadEx* Find(int nID) const;

	friend OSDispatcherEx* GetOSDispatcher(void);

public:
	// for message get & put
	void *GetMessagePtr(void);
	void PutMessagePtr(void *p);

protected:
	virtual int Run(void) override;
	virtual int OSInitHook(void);

	virtual int OnHandleMessage(Message *Message);
	virtual int ReceiveMessage(Message *Message);

private:
	OSThreadEx* m_pThreadList;

	static OSDispatcherEx* m_pDispatcher;
};

} /* namespace OSExt */

#endif /* OSDISPATCHEREX_H_ */
