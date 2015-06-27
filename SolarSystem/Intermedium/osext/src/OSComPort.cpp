/*
 * OSComPort.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include <OSComPort.h>

namespace osext {

//
// class OSComPort
//
OSComPort::OSComPort(ComPort nPort, ComDcb *dcb, DWORD dwRxQueueLen, DWORD dwTxQueueLen) :
		m_bOpened(false), m_nPort(nPort), m_dwRxQueueLen(dwRxQueueLen), m_hRxQueue(nullptr), m_dwTxQueueLen(dwTxQueueLen), m_hTxQueue(nullptr) {
	memcpy(&m_hComDcb, dcb, sizeof(tagCOMDCB));
}

OSComPort::~OSComPort(void) {
}

int OSComPort::Open(void) {
	if (m_bOpened)
		return 1;
	//
//	m_hMailBox = new OSMailbox(nullptr, true);
//	if (m_hMailBox == nullptr)
//		return 2;

	m_hRxQueue = new utils::CCircleBuff(m_dwRxQueueLen);
	if (m_hRxQueue == nullptr)
		return 3;
	if (!m_hRxQueue->isOK())
		return 3;

	m_hTxQueue = new utils::CCircleBuff(m_dwTxQueueLen);
	if (m_hTxQueue == nullptr)
		return 4;
	if (!m_hTxQueue->isOK())
		return 4;

	if (_open())
		return 5;

	m_bOpened = true;
	return 0;

}

int OSComPort::Open(ComBaud nBaud, ComDataBit databit, ComParity parity, ComStopbit stopbit) {
	m_hComDcb.nBaud = nBaud;
	m_hComDcb.databit = databit;
	m_hComDcb.parity = parity;
	m_hComDcb.stopbit = stopbit;

	return Open();
}

int OSComPort::Close(void) {
	if (!m_bOpened)
		return 1;

	if (!_close())
		return 2;

//	delete m_hMailBox;
	delete m_hRxQueue;
	delete m_hTxQueue;
	m_nPort = COM_NONE;
	m_bOpened = false;

	return 0;
}

BOOL OSComPort::SetCommDCB(ComDcb *dcb) {
	if (dcb == nullptr)
		return false;
	memcpy(&m_hComDcb, dcb, sizeof(tagCOMDCB));

//	if (m_bOpened)
	if (ConfigDCB() != 0)
		return false;
	return true;
}

int OSComPort::ConfigDCB(void) {
	return 0;
}

int OSComPort::ConfigComBaud(ComBaud nBaud) {
//	nBaud = nBaud;
	return 0;
}

ComEvent OSComPort::WaitCommEvent(DWORD dwMilliseconds) {
	void *msg = nullptr;
	int err;

	if (!m_bOpened)
		return COM_NOOPEN;

//	if (dwMilliseconds == 0)
//		err = m_hMailBox->Accept(msg);
//	else if (dwMilliseconds == INFINITE)
//		err = m_hMailBox->Pend(msg, 0);
//	else
//		err = m_hMailBox->Pend(msg, (dwMilliseconds / (1000 / OS_TICKS_PER_SEC)));
//
//	if (err == OS_NO_ERR)
//		return (ComEvent) (int) msg;
//	if (err == OS_TIMEOUT)
//		return COM_TIMEOUT;
	return COM_UNKNOW;
}

int OSComPort::Read(BYTE *buff, int length) {
	int len;

	if (!m_bOpened)
		return 0;
	if (buff != nullptr)
		len = m_hRxQueue->Read(buff, length);
	else
		len = m_hRxQueue->GetCount();
	return len;
}

int OSComPort::Write(const BYTE *Buff, const int length) {
	int nLen;

	if (!m_bOpened)
		return 0;

	nLen = m_hTxQueue->Write(Buff, length);
	_StartToSend();

	return nLen;
}

int OSComPort::GetRxDataLen(void) {
	int len;

	len = m_hRxQueue->GetCount();

	return len;
}

int OSComPort::GetTxDataLen(void) {
	int len;

	len = m_hTxQueue->GetCount();

	return len;
}

void OSComPort::Flush() {
	void *msg = nullptr;

	if (!m_bOpened)
		return;
//	m_hMailBox->Accept(msg);
	m_hRxQueue->Flush();
}

} /* namespace osext */
