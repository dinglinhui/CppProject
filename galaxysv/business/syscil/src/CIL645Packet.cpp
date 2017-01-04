#include <string.h>
#include "CIL645Packet.h"

///////////////////////////////////////////////////////////////////////////////
// CIL645Packet
///////////////////////////////////////////////////////////////////////////////
namespace syscil {
CIL645Packet::CIL645Packet(void) :
		CILPacket(PCOL_DL645) {
}

CIL645Packet::CIL645Packet(int nSize) :
		CILPacket(PCOL_DL645, nSize) {
}

CIL645Packet::CIL645Packet(BYTE *lpBuf, int nSize) :
		CILPacket(PCOL_DL645, lpBuf, nSize) {
}

CIL645Packet::~CIL645Packet(void) {
}

bool CIL645Packet::GetHDR(DL645_HDR &hdr) {
	BYTE *lpBuf = GetFrame();
	if (lpBuf != nullptr) {
		int nLen = GetLength();
		if (nLen > 8) {
			memcpy(hdr.rtua, &lpBuf[1], 6);
			hdr.code.body = lpBuf[8];
			return true;
		}
	}
	return false;
}

void CIL645Packet::Reversal(void) {
	BYTE *pData = nullptr;
	int nSize = GetData(pData);

	for (int i = 0; i < nSize; ++i) {
		if (i > 3) {
			if (((pData[i] & 0x0f) > 0x0c) || (((pData[i] >> 4) & 0x0f) > 0x0c)) {
				for (int j = 0; j < nSize; ++j) {
					pData[j] = 0x00;
				}
				break;
			}
		}

		if (pData[i] >= 0x33) {
			pData[i] -= 0x33;
		} else {
			pData[i] = 0x100 + pData[i] - 0x33;
		}
	}
}

int CIL645Packet::Create(BYTE *lpBuf, int nSize) {
	int nPos = 0;
	int nRet = 0;
	while (nPos < nSize) {
		nRet = DL645_IsValid(&lpBuf[nPos], (nSize - nPos));
		if (nRet == -1) {
			nPos = nRet;
			break;
		}

		if (nRet == -2) {
			nPos++;
			continue;
		}

		Append(&lpBuf[nPos], nRet);
		nPos += nRet;
		break;
	}
	return nPos;
}

PCMD_TYPE CIL645Packet::GetPcmdType(void) {
	PCMD_TYPE type = PCMD_BROADCAST;
	DL645_HDR hdr;

	if (GetHDR(hdr)) {
		if (hdr.code.mask.DIR == 1) {
			type = PCMD_RESPOND;
		} else {
			if (hdr.code.mask.CMD != EM_CMD_WITHHOLD || hdr.code.mask.CMD != EM_CMD_CLEARXL || hdr.code.mask.CMD != EM_CMD_CLEARALL) {
				type = PCMD_REQUEST;
			}
		}
	}
	return type;
}

int CIL645Packet::IsValid(void) {
	BYTE *lpBuf = GetFrame();
	if (lpBuf != nullptr) {
		int nLen = GetLength();
		int nRet = DL645_IsValid(lpBuf, nLen);
		if (nRet > 0) {
			return 0;
		}
	}
	return -1;
}

int CIL645Packet::GetData(BYTE*& ptr) {
	BYTE *lpBuf = GetFrame();
	if (lpBuf != nullptr) {
		int nSize = GetLength();
		if (nSize > 10) {
			int nLen = lpBuf[9];
			ptr = &lpBuf[10];
			return nLen;
		}
	}
	return 0;
}

CILPacket* CIL645Packet::Clone(void) {
	int nSize = GetSize();
	CIL645Packet *pk645 = new CIL645Packet(nSize);
	if (pk645 != nullptr) {
		BYTE *ptr = GetFrame();
		int nLen = GetLength();
		pk645->Append(ptr, nLen);
	}
	return pk645;
}

CILPacket* CIL645Packet::Duplicate(void) {
	BYTE *ptr = GetFrame();
	int nSize = GetSize();
	CIL645Packet *pk645 = new CIL645Packet(ptr, nSize);
	if (pk645 != nullptr) {
		int nLen = GetLength();
		pk645->Offset(nLen);
	}
	return pk645;
}
}

///////////////////////////////////////////////////////////////////////////////
// End of file: CIL645Packet.cpp
///////////////////////////////////////////////////////////////////////////////
