/*
 * IEC1107Protocol.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "IEC1107Protocol.h"
#include "pcol_dl645.h"
#include <ctypes.h>
#include <cassert>

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

typedef struct tag_CMD_IEC1107 {
	BYTE gtt;
	BYTE cmd;
	BYTE baud;
} CMD_IEC1107;

///////////////////////////////////////////////////////////////////////////////
// static helper data
///////////////////////////////////////////////////////////////////////////////

static const CMD_IEC1107 l_commands[] = { { 0x01, IEC_START, 0 }, { 0x02, IEC_START, 0 }, /////Add 2008.11.21
		{ 0x03, IEC_START, 0 } /////Add 2008.11.21
};

///////////////////////////////////////////////////////////////////////////////
// CIEC1107Protocol
///////////////////////////////////////////////////////////////////////////////

CIEC1107Protocol::CIEC1107Protocol(BYTE nMPT, PF_IEC_SAVE pSave) :
		CGMProtocol(nMPT),
		m_pfSave(pSave) {
	m_1107type = nMPT;
}

CIEC1107Protocol::~CIEC1107Protocol(void) {
}

int CIEC1107Protocol::GetTranTimeOut(Command*) {
	return 300;
}

int CIEC1107Protocol::GetRecvTimeOut(Command*) {
	return 30;
}

int CIEC1107Protocol::GetCommands(void *parms, Command *&pCMDs, PointType) {
	Command **ppCMD = &pCMDs;
	BYTE gtt = (BYTE) *(int *) parms;
	int nNums = 0;
	int nSize = sizeof(l_commands) / sizeof(CMD_IEC1107);
	for (int i = 0; i < nSize; ++i) {
		if (gtt == l_commands[i].gtt) {
			*ppCMD = new Command;
			if (*ppCMD != nullptr) {
				(*ppCMD)->m_nAck = 1;
				((CMD_IEC1107*) (*ppCMD)->m_body)->gtt = l_commands[i].gtt;
				((CMD_IEC1107*) (*ppCMD)->m_body)->cmd = l_commands[i].cmd;
				((CMD_IEC1107*) (*ppCMD)->m_body)->baud = l_commands[i].baud;
				(*ppCMD)->m_pNext = nullptr;
				(*ppCMD)->m_pAck = nullptr;
				ppCMD = &(*ppCMD)->m_pNext;
				nNums++;
			}
		}
	}
	return nNums;
}

int CIEC1107Protocol::GetDefaultComDcb(ComDcb *pDcb) {
	assert(pDcb != nullptr);

	pDcb->nBaud = CB_300;
	pDcb->databit = DB_7;
	pDcb->parity = CP_EVEN;
	pDcb->stopbit = CS_ONE;

	return 0;
}

int CIEC1107Protocol::GetSendComDcb(Command *pCMD, ComDcb *pDcb) {
	assert(pCMD != nullptr);
	assert(pDcb != nullptr);

	CMD_IEC1107 *ptr = (CMD_IEC1107*) pCMD->m_body;
	if (ptr->cmd == IEC_START) {
		ComDcb hDcb = { CB_300, DB_7, CP_EVEN, CS_ONE };
		memcpy(pDcb, &hDcb, sizeof(ComDcb));
		return 0;
	}

	return -1;
}

int CIEC1107Protocol::GetRecvComDcb(Command *pCMD, ComDcb *pDcb) {
	static const ComBaud baud[] = { CB_300, CB_600, CB_1200, CB_2400, CB_4800, CB_9600 };

	CMD_IEC1107 *ptr = (CMD_IEC1107*) pCMD->m_body;
	if (ptr->cmd == IEC_READDATA) {
		if (ptr->baud < 6) {
			ComDcb hDcb = { baud[ptr->baud], DB_7, CP_EVEN, CS_ONE };
			memcpy(pDcb, &hDcb, sizeof(ComDcb));
			return 0;
		}
	}

	return -1;
}

int CIEC1107Protocol::HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf, int nBufSize) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);

	int nSize = 0;
	int nRet = 0;

	BYTE buff[16] = { 0 };
	IEC_FRAME frm;
	frm.data = buff;

	CMD_IEC1107 *ptr = (CMD_IEC1107*) pCMD->m_body;
	memcpy(frm.hdr.host, pObj->m_host, 6);

	switch (ptr->cmd) {
	case IEC_START:
		nRet = IEC_Start(&frm);
		break;

	case IEC_READDATA:
		nRet = IEC_ReadData(&frm, ptr->baud);
		break;

	case IEC_ORIGREAD:
		nRet = 0;
		break;

	default:
		nRet = -1;
		break;
	}

	if (nRet == 0) {
		nSize = IEC_MakeFrame(&frm, lpBuf, nBufSize);
//		DUMPMSG1(lpBuf, nSize);
	}
	return nSize;
}

int CIEC1107Protocol::HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf, int nBufSize) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);
	assert(lpBuf != nullptr);
	assert(m_pfSave != nullptr);

	IEC_FRAME frm;
	BYTE buff[32] = { 0 };

	CMD_IEC1107 *ptr = (CMD_IEC1107*) pCMD->m_body;
	frm.hdr.code = ptr->cmd;
	frm.data = buff;

	int nRet = IEC_GetFrame(&frm, lpBuf, nBufSize);
	if (nRet >= 0) {
		switch (ptr->cmd) {
		case IEC_START: {
			int nBaud = frm.data[0];

			Command *pRes = new Command;
			if (pRes != nullptr) {
				pRes->m_nAck = 1;
				pRes->m_pNext = nullptr;
				pRes->m_pAck = nullptr;
				((CMD_IEC1107*) pRes->m_body)->gtt = ptr->gtt;
				((CMD_IEC1107*) pRes->m_body)->cmd = IEC_READDATA;
				((CMD_IEC1107*) pRes->m_body)->baud = nBaud;
				pCMD->m_pAck = pRes;
			}
		}
			break;

		case IEC_ORIGREAD:
		case IEC_READDATA: {
			char *data = (char*) frm.data;
			char span[] = { IEC_CR, IEC_LF, '\0' };
			BYTE buff[64] = { 0 };
			BYTE srce[64] = { 0 };
			int value[64 / sizeof(int)] = { 0 };

			int nMon = 0;
			WORD di = 0x00;
			int nLen = 0;
			int nPos = 0;
			int nIndx = indexOf(data, span);
			int nDIS = 0;

//			time_t htm = time(nullptr);
//			tm tm_sys = *localtime(&htm);

			while (nIndx > 0) {
				memset(buff, 0, 64);
				memcpy(srce, data, nIndx);

				nPos = 0;
				nLen = IEC_Decode(m_1107type, buff, (char*) srce, nIndx, &nMon);
				while (nPos < nLen) {
					di = MAKEWORD(buff[nPos], buff[nPos + 1]);

					nPos += 2;
					nDIS = DL645_GetDataLength(di);
					assert(nDIS < 64);

					memset(value, 0, 64);
					memcpy(value, &buff[nPos], nDIS);
					m_pfSave(pObj, (void*) &ptr->gtt, di, (void*) value);
					/*-Q+存到Q1，Q-存到Q4-*/
					if (((di >= 0x9110) && (di <= 0x9114)) || ((di >= 0x9510) && (di <= 0x9514))) {
						m_pfSave(pObj, (void*) &ptr->gtt, (di + 0x20), (void*) value);
					}
					if (((di >= 0x9120) && (di <= 0x9124)) || ((di >= 0x9520) && (di <= 0x9524))) {
						m_pfSave(pObj, (void*) &ptr->gtt, (di + 0x20), (void*) value);
					}

					nPos += nDIS;
				}

				data += nIndx;
				data += 2;

				nIndx = indexOf(data, span);
			}
		}
			break;

		default:
			break;
		}
	}
	// if start error, read data use default comdcb.
	else if (ptr->cmd == IEC_ORIGREAD) {

		Command *pRes = new Command;
		if (pRes != nullptr) {
			pRes->m_nAck = 1;
			pRes->m_pNext = nullptr;
			pRes->m_pAck = nullptr;
			((CMD_IEC1107*) pRes->m_body)->gtt = ptr->gtt;
			((CMD_IEC1107*) pRes->m_body)->cmd = IEC_ORIGREAD;
			((CMD_IEC1107*) pRes->m_body)->baud = 0;
			pCMD->m_pAck = pRes;
		}
		nRet = 0;
	}
	// if read data error,don't retry send it.
	else if (ptr->cmd == IEC_READDATA || ptr->cmd == IEC_ORIGREAD) {
		nRet = 0;
	}

	return nRet;
}

} /* namespace pcols */
