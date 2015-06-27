#include <string.h>
#include "CILFKPacket.h"

///////////////////////////////////////////////////////////////////////////////
// CILFKPacket
///////////////////////////////////////////////////////////////////////////////
namespace syscil {
CILFKPacket::CILFKPacket(void) :
		CILPacket(PCOL_FK) {
}

CILFKPacket::CILFKPacket(int nSize) :
		CILPacket(PCOL_FK, nSize) {
}

CILFKPacket::CILFKPacket(BYTE *lpBuf, int nSize) :
		CILPacket(PCOL_FK, lpBuf, nSize) {
}

CILFKPacket::~CILFKPacket(void) {
}

bool CILFKPacket::GetUData(FK_UDATA &udt) {
	BYTE *lpBuf = GetFrame();
	if (lpBuf != NULL) {
		int nSize = GetLength();
		if (nSize > 0) {
			int nRet = FK_GetFrame(&udt, lpBuf, nSize);
			return (nRet >= 0);
		}
	}
	return false;
}

int CILFKPacket::Create(BYTE *lpBuf, int nSize) {
	int nPos = 0;
	int nRet = 0;
	while (nPos < nSize) {
		nRet = FK_IsValid(&lpBuf[nPos], (nSize - nPos));
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

PCMD_TYPE CILFKPacket::GetPcmdType(void) {
	PCMD_TYPE type = PCMD_BROADCAST;
	FK_UDATA udt;
	if (GetUData(udt)) {
		if (udt.code.body_mask.DIR == 0) {
			if (udt.code.body_mask.PRM == 1) {
				if (udt.code.body_mask.cmd != 4) {
					type = PCMD_REQUEST;
				}
			} else {
				type = PCMD_RESPOND;
			}
		} else {
			if (udt.code.body_mask.PRM == 1) {
				if (udt.lnkData.seq.body_mask.CON == 1) {
					type = PCMD_REQUEST;
				}
			} else {
				type = PCMD_RESPOND;
			}
		}
	}
	return type;
}

int CILFKPacket::IsValid(void) {
	BYTE *lpBuf = GetFrame();
	int nSize = GetLength();
	int nRet = FK_IsValid(lpBuf, nSize);
	if (nRet > 0)
		return 0;
	return -1;
}

int CILFKPacket::GetData(BYTE*& ptr) {
	FK_UDATA udt;
	if (GetUData(udt)) {
		ptr = udt.lnkData.cell.lpBuf;
		return udt.lnkData.cell.nLen;
	}
	return 0;
}

CILPacket* CILFKPacket::Clone(void) {
	int nSize = GetSize();
	CILFKPacket *pkFK = new CILFKPacket(nSize);
	if (pkFK != NULL) {
		BYTE *ptr = GetFrame();
		int nLen = GetLength();
		pkFK->Append(ptr, nLen);
	}
	return pkFK;
}

CILPacket* CILFKPacket::Duplicate(void) {
	BYTE *ptr = GetFrame();
	int nSize = GetSize();
	CILFKPacket *pkFK = new CILFKPacket(ptr, nSize);
	if (pkFK != NULL) {
		int nLen = GetLength();
		pkFK->Offset(nLen);
	}
	return pkFK;
}
}
///////////////////////////////////////////////////////////////////////////////
// End of file: CILFKPacket.cpp
///////////////////////////////////////////////////////////////////////////////
