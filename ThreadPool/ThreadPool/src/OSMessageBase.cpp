#include "OSMessageBase.h"

///////////////////////////////////////////////////////////
//
// class MSG definition.
//
///////////////////////////////////////////////////////////

MSG::MSG(void) :
	m_pSource( NULL),
	m_nCmd(FM_NULL),
	m_wParam(0),
	m_lParam(0),
	m_bAsyn( FALSE),
	m_pACT( NULL)
{
}

//OSMem* MSG::GetMemPool(void)
//{
//	OS_ENTER_CRITICAL();
//	if (ms_MemPool == NULL)
//	{
//		ms_MemPool = new OSMem(Max_OSMSGSize, sizeof(MSG), TRUE);
//	}
//	OS_EXIT_CRITICAL();
//	return ms_MemPool;
//}

/*void* MSG::operator new(size_t nSize)
 {
 if (nSize != sizeof(MSG))
 {
 return NULL;
 }

 void *ptr = NULL;
 OSMem *pPool = GetMemPool();
 if (pPool != NULL)
 {
 pPool->Get(ptr);
 }
 return ptr;
 }*/

/*void MSG::operator delete(void *p, size_t nSize)
 {
 if (nSize != sizeof(MSG))
 {
 return;
 }

 OSMem *pPool = GetMemPool();
 if (pPool != NULL)
 {
 pPool->Put(p);
 }
 }*/

///////////////////////////////////////////////////////////
//
// class CMessageBase definition.
//
///////////////////////////////////////////////////////////
OSMessageBase::OSMessageBase(int nQueueSize) :
	m_bOk( FALSE)/*, m_Queue(nQueueSize)*/
{
}

OSMessageBase::~OSMessageBase(void)
{
}

int OSMessageBase::PostMessage(OSMessageBase *pTarget, MSG *msg)
{
	if (pTarget != NULL && msg != NULL)
	{
		msg->m_bAsyn = TRUE;
		return pTarget->ReceiveMessage(msg);
	}
	return -1;
}

int OSMessageBase::PostMessage(OSMessageBase *pTarget, DWORD nCmd, DWORD wParam, DWORD lParam, void *act)
{
	MSG *msg = new MSG;
	if (msg != NULL)
	{
		msg->m_pSource = this;
		msg->m_nCmd = nCmd;
		msg->m_wParam = wParam;
		msg->m_lParam = lParam;
		msg->m_pACT = act;
		int nRet = PostMessage(pTarget, msg);
		if (nRet != OS_NO_ERR)
		{
			delete msg;
		}
		return nRet;
	}
	return -1;
}

int OSMessageBase::SendMessage(OSMessageBase *pTarget, MSG *msg)
{
	if (pTarget != NULL && msg != NULL)
	{
		msg->m_bAsyn = FALSE;
		return pTarget->OnHandleMessage(msg);
	}
	return -1;
}

int OSMessageBase::SendMessage(OSMessageBase *pTarget, DWORD nCmd, DWORD wParam, DWORD lParam)
{
	MSG *msg = new MSG;
	if (msg != NULL)
	{
		msg->m_pSource = this;
		msg->m_nCmd = nCmd;
		msg->m_wParam = wParam;
		msg->m_lParam = lParam;
		int nRet = SendMessage(pTarget, msg);
		delete msg;
		return nRet;
	}
	return -1;
}

int OSMessageBase::Post(MSG *msg)
{
	return 0; //m_Queue.Post((void*) msg);
}

int OSMessageBase::Pend(MSG *&msg, DWORD dwWaitTime)
{
	return 0; //m_Queue.Pend((void*&) msg, dwWaitTime);
}

BOOL OSMessageBase::QueueInitialize(void)
{
	//if (!m_Queue.Create())
	//	return FALSE;

	m_bOk = TRUE;
	return TRUE;
}

int OSMessageBase::ReceiveMessage(MSG *msg)
{
	return Post(msg);
}

int OSMessageBase::OnHandleMessage(MSG *msg)
{
	//msg = msg;
	return -1;
}
