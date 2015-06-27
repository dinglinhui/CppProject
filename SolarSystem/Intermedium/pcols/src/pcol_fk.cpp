/*
 * cil_fk.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include <pcol_fk.h>
#include <stdio.h>
#include <string.h>
#include <ctypes.h>
#include <time.h>

namespace pcols {

////////////////////////////////////////////////////////////////////////
// FN、PN到DA、DT的转换
////////////////////////////////////////////////////////////////////////

WORD plms_cvt_da(WORD pn) {
	if (pn == 0)
		return 0x0000;
	if (pn == 0xFFFF)
		return 0xFFFF;

	BYTE DA1 = 0;
	BYTE DA2 = 0;
	BYTE nPos = (pn - 1) / 8 + 1;
	BYTE nSpa = (pn - 1) % 8;

	DA1 = (0x01 << nSpa);
	DA2 = nPos;

	return MAKEWORD(DA1, DA2);
}

WORD plms_cvt_dt(BYTE fn) {
	BYTE DT1 = 0;
	BYTE DT2 = 0;
	BYTE nPos = (fn - 1) / 8;
	BYTE nSpa = (fn - 1) % 8;

	DT1 |= (0x01 << nSpa);
	DT2 |= nPos;

	return MAKEWORD(DT1, DT2);
}

BYTE plms_cvt_fn(WORD dt) {
	static const BYTE s_dt[] = { 0, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
			0x80 };

	if (dt == 0x0000)
		return 0;

	BYTE lo = LOBYTE(dt);
	BYTE hi = HIBYTE(dt);

	int nCount = sizeof(s_dt) / sizeof(BYTE);
	for (int i = 0; i < nCount; ++i) {
		if (s_dt[i] == lo) {
			return hi * 8 + i;
		}
	}

	return 0;
}

WORD plms_cvt_pn(WORD da) {
	static const BYTE s_da[] = { 0, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
			0x80 };

	if (da == 0x0000)
		return 0;

	BYTE lo = LOBYTE(da);
	BYTE hi = HIBYTE(da);

	int nCount = sizeof(s_da) / sizeof(BYTE);
	for (int i = 0; i < nCount; ++i) {
		if (s_da[i] == lo) {
			for (int j = 0; j < nCount; ++j) {
				if (s_da[j] == hi) {
					return (j - 1) * 8 + i;
				}
			}
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////

static BYTE ms_nSequence = 0;

static BYTE FK_GetSequence(void) {
	BYTE nSequence = (ms_nSequence % 0x0F);
	++ms_nSequence;
	return nSequence;
}

static BYTE FK_CaclCheckSum(BYTE *lpBuf, int nBufSize) {
	BYTE sums = 0;
	for (int i = 0; i < nBufSize; ++i) {
		sums += lpBuf[i];
	}
	return sums;
}

static BYTE FK_GetRespondAFN(BYTE afn) {
	BYTE rep = afn;
	switch (afn) {
	case FK_CMD_RESET:
	case FK_CMD_SETPARAM:
	case FK_CMD_CONTROL:
		rep = FK_CMD_ACK;
		break;

	default:
		break;
	}
	return rep;
}

void FK_RequestLink(FK_UDATA *udt) {
	// C
	udt->code.body_mask.cmd = 9;
	udt->code.body_mask.PRM = 1;
	udt->code.body_mask.DIR = 1;
	udt->code.body_mask.FCV = 0;
	udt->code.body_mask.FCB_ACD = 0;

	// AFN
	udt->lnkData.afn = FK_CMD_LINKSTATE;

	// SEQ
	udt->lnkData.seq.body_mask.TpV = 0;
	udt->lnkData.seq.body_mask.FIR = 1;
	udt->lnkData.seq.body_mask.FIN = 1;
	udt->lnkData.seq.body_mask.CON = 1;
	udt->lnkData.seq.body_mask.PRSEQ = FK_GetSequence();
}

void FK_ReportData(BYTE afn, FK_UDATA *udt) {
	// C
	udt->code.body_mask.cmd = 0;

	if (afn == FK_CMD_CLASS1) {
		udt->code.body_mask.cmd = 10;
	} else if (afn == FK_CMD_CLASS2) {
		udt->code.body_mask.cmd = 11;
	}
	udt->code.body_mask.PRM = 1;
	udt->code.body_mask.DIR = 1;
	udt->code.body_mask.FCV = 0;
	udt->code.body_mask.FCB_ACD = 0;

	// AFN
	udt->lnkData.afn = afn;

	// SEQ
	udt->lnkData.seq.body_mask.TpV = 0;
	udt->lnkData.seq.body_mask.FIR = 1;
	udt->lnkData.seq.body_mask.FIN = 1;
	udt->lnkData.seq.body_mask.CON = 0;
	udt->lnkData.seq.body_mask.PRSEQ = FK_GetSequence();
}

BOOL FK_RespondEx(FK_UDATA *dst, FK_UDATA *src, BOOL bAck) {
	// C
	BYTE afn = FK_GetRespondAFN(src->lnkData.afn);
	if (afn == 0x01) {
		dst->code.body_mask.cmd = 0;
	} else if (afn == 0x02) {
		dst->code.body_mask.cmd = 11;
	} else {
		if (!bAck) {
			dst->code.body_mask.cmd = 9;
		} else {
			dst->code.body_mask.cmd = 8;
		}
	}

	dst->code.body_mask.DIR = 1;
	dst->code.body_mask.PRM = 0;
	dst->code.body_mask.FCB_ACD = 0;
	dst->code.body_mask.FCV = 0;

	// RTUA
	memcpy(&dst->rtua, &src->rtua, sizeof(FK_ADDRESS));

	// AFN
	dst->lnkData.afn = afn;

	// SEQ
	dst->lnkData.seq.body_mask.TpV = src->lnkData.seq.body_mask.TpV;
	dst->lnkData.seq.body_mask.FIR = 1;
	dst->lnkData.seq.body_mask.FIN = 1;
	dst->lnkData.seq.body_mask.CON = 0;
	dst->lnkData.seq.body_mask.PRSEQ = src->lnkData.seq.body_mask.PRSEQ;

	// AUX
	dst->lnkData.aux.EC.EC1 = 0;
	dst->lnkData.aux.EC.EC2 = 0;
	dst->lnkData.aux.Tp.PFC = src->lnkData.aux.Tp.PFC;
	dst->lnkData.aux.Tp.time = src->lnkData.aux.Tp.time;
	dst->lnkData.aux.Tp.delay = src->lnkData.aux.Tp.delay;

	return true;
}

int FK_HasAUX(BYTE afn) {
	int nRet = -1;
	switch (afn) {
	case FK_CMD_RESET:
	case FK_CMD_SETPARAM:
	case FK_CMD_CONTROL:
	case FK_CMD_TRANSMIT:
		nRet = 0;
		break;

	default:
		break;
	}
	return nRet;
}

int FK_IsValid(BYTE *lpBuf, int nSize) {
	if (lpBuf == nullptr)
		return -1;
	if (nSize < 8)
		return -1;

	int nPos = 0;
	if (lpBuf[nPos] != 0x68)
		return -2;
	nPos++;

	WORD flen = MAKEWORD(lpBuf[nPos], lpBuf[nPos + 1]);
	nPos += 2;
	WORD slen = MAKEWORD(lpBuf[nPos], lpBuf[nPos + 1]);
	nPos += 2;

	if (lpBuf[nPos] != 0x68)
		return -2;
	nPos++;

	int nData = nPos;

	if ((flen != slen))
//		((flen&0x0003) != 0x0002)
//		||((flen&0x0003) != 0x0001) )
		return -2;

	// 数据长度域低2位禁用
	flen = (flen >> 2);
	if (nSize < (flen + 8))
		return -1;
	nPos += flen;

	BYTE sum = FK_CaclCheckSum(&lpBuf[nData], flen);
	if (sum != lpBuf[nPos])
		return -2;
	nPos++;

	if (lpBuf[nPos] != 0x16)
		return -2;
	nPos++;

	return nPos;
}

int FK_MakeFrame(FK_UDATA *udt, BYTE *lpBuf, int nSize) {
	if (lpBuf == nullptr)
		return -1;
	if (nSize <= 0)
		return -1;

	int nPos = 0;
	int nSte = 0;
	int flen = udt->lnkData.cell.nLen + 8;
	int slen = 0;

	// get frame length
	if (udt->code.body_mask.FCB_ACD == 1)
		flen += 2;
	if (udt->lnkData.seq.body_mask.TpV == 1)
		flen += 6;

	slen = flen;
	flen = (flen << 2) | 0x02;

	// write fixed part
	lpBuf[nPos++] = 0x68;
	lpBuf[nPos++] = LOBYTE(flen);
	lpBuf[nPos++] = HIBYTE(flen);
	lpBuf[nPos++] = LOBYTE(flen);
	lpBuf[nPos++] = HIBYTE(flen);
	lpBuf[nPos++] = 0x68;

	nSte = nPos;

	// write C
	lpBuf[nPos++] = udt->code.body;

	// write rtua
	word2bcd(udt->rtua.A1, &lpBuf[nPos]);
	nPos += 2;
	word2bcd(udt->rtua.A2, &lpBuf[nPos]);
	nPos += 2;
//	lpBuf[nPos++] = LOBYTE(udt->rtua.A2);
//	lpBuf[nPos++] = HIBYTE(udt->rtua.A2);
	lpBuf[nPos++] = udt->rtua.A3;

	// write AFX & SEQ
	lpBuf[nPos++] = udt->lnkData.afn;
	lpBuf[nPos++] = udt->lnkData.seq.body;

	// write user data
	memcpy(&lpBuf[nPos], udt->lnkData.cell.lpBuf, udt->lnkData.cell.nLen);
	nPos += udt->lnkData.cell.nLen;

	// write aux
	if (udt->code.body_mask.FCB_ACD == 1) {
		lpBuf[nPos++] = udt->lnkData.aux.EC.EC1;
		lpBuf[nPos++] = udt->lnkData.aux.EC.EC2;
	}

	if (udt->lnkData.seq.body_mask.TpV == 1) {
		lpBuf[nPos++] = udt->lnkData.aux.Tp.PFC;
		nPos += plms_fmt_tran(16, &lpBuf[nPos], &udt->lnkData.aux.Tp.time);
		lpBuf[nPos++] = udt->lnkData.aux.Tp.delay;
	}

	// cacl check sum
	BYTE sum = FK_CaclCheckSum(&lpBuf[nSte], slen);
	lpBuf[nPos++] = sum;
	lpBuf[nPos++] = 0x16;

	return nPos;
}

int FK_GetFrame(FK_UDATA *udt, BYTE *lpBuf, int nSize) {
	int nPos = 0;
	int nRet = 0;
	while (nPos < nSize) {
		nRet = FK_IsValid(&lpBuf[nPos], (nSize - nPos));
		if (nRet == -2) {
			nPos++;
			continue;
		}

		if (nRet == -1) {
			nPos = -1;
			break;
		}

		nPos += nRet;

		int nUdp = 6;

		udt->code.body = lpBuf[nUdp++];
		udt->rtua.A1 = bcd2word(&lpBuf[nUdp]);
		nUdp += 2;

		// FFFF广播地址 特殊处理
		if ((lpBuf[nUdp] == 0xFF) && (lpBuf[nUdp + 1] == 0xFF)) {
			udt->rtua.A2 = 0xFFFF;
		} else {
			udt->rtua.A2 = bcd2word(&lpBuf[nUdp]);
		}
//		udt->rtua.A2            = MAKEWORD(lpBuf[nUdp], lpBuf[nUdp+1]);
		nUdp += 2;
		udt->rtua.A3 = lpBuf[nUdp++];
		udt->lnkData.afn = lpBuf[nUdp++];
		udt->lnkData.seq.body = lpBuf[nUdp++];
		udt->lnkData.cell.lpBuf = &lpBuf[nUdp];

		WORD flen = MAKEWORD(lpBuf[1], lpBuf[2]);
		flen = (flen >> 2);
		flen -= 8;

		// 主站发出的下行帧
		if (udt->code.body_mask.DIR == 0) {
			// 时间标签
			if (udt->lnkData.seq.body_mask.TpV == 1) {
				if (flen >= 6)
					flen -= 6;
				int nPos = nUdp + flen;
				udt->lnkData.aux.Tp.PFC = lpBuf[nPos++];
				nPos += plms_fmt_appl(16, &lpBuf[nPos],
						&udt->lnkData.aux.Tp.time);
				udt->lnkData.aux.Tp.delay = lpBuf[nPos++];
			}

			// 消息验证码
			int nAUX = FK_HasAUX(udt->lnkData.afn);
			if (nAUX == 0) {
//			#ifdef V2005
				const BYTE PW_LEN = 16;
//			#else
//				const BYTE PW_LEN = 2;
//			#endif
				if (flen >= PW_LEN) {
					flen -= PW_LEN;
				} else //报文异常
				{
					return -1;
				}
				nUdp += flen;
				memcpy(&udt->lnkData.aux.PW, &lpBuf[nUdp], PW_LEN);
				nUdp += PW_LEN;
			}
		}
		// 终端发出的上行帧
		else {
			// 带附加信息域
			if (udt->lnkData.seq.body_mask.TpV == 1) {
				if (flen >= 8)
					flen -= 8;
				nUdp += flen;

				udt->lnkData.aux.EC.EC1 = lpBuf[nUdp++];
				udt->lnkData.aux.EC.EC1 = lpBuf[nUdp++];
				udt->lnkData.aux.Tp.PFC = lpBuf[nUdp++];
				nUdp += plms_fmt_appl(16, &lpBuf[nUdp],
						&udt->lnkData.aux.Tp.time);
				udt->lnkData.aux.Tp.delay = lpBuf[nUdp++];
			}
		}

		udt->lnkData.cell.nLen = flen;
		break;
	}

	return nPos;
}

////////////////////////////////////////////////////////////////////////
// 数据格式转换
////////////////////////////////////////////////////////////////////////

typedef int (*fmt_appl)(BYTE *pTran, void *pAppl);
typedef int (*fmt_tran)(BYTE *pTran, void *pAppl);

typedef struct tag_FMT1 {
	int nClass;
	fmt_appl pfAppl;
	fmt_tran pfTran;
} FMT1;

static int plms_valid(void *ptr, int nSize) {
	for (int i = 0; i < nSize; ++i) {
		if ((((BYTE*) ptr)[i] != 0xEE) && (((BYTE*) ptr)[i] != 0xFF)
				&& (((BYTE*) ptr)[i] != 0xEF)) {
			return 0;
		}
	}
	//如果全为FF，无效数据应该回EE,GS
	int numFF = 0;
	for (int i = 0; i < nSize; ++i) {
		if (((BYTE*) ptr)[i] == 0xFF) {
			numFF++;
		}
	}
	if (numFF >= nSize) {
		memset(ptr, 0xEE, nSize);
	}
	return -1;
}

// 数据类别转换
// 数据格式01
static int plms_fmt_appl_01(BYTE *pTran, void *pAppl) {
	FK_DATAMASK01 *ptr = (FK_DATAMASK01*) pAppl;
	if (plms_valid(pTran, 6) == 0) {
		ptr->nSec = bcd2hex(pTran[0]);
		ptr->nMin = bcd2hex(pTran[1]);
		ptr->nHour = bcd2hex(pTran[2]);
		ptr->nDay = bcd2hex(pTran[3]);
		ptr->nMonWeek.mask.nMonth = bcd2hex(pTran[4] & 0x1F);
		ptr->nMonWeek.mask.nWeek = bcd2hex((pTran[4] & 0xE0) >> 5);
		ptr->nYear = bcd2hex(pTran[5]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK01));
	}
	return 6;
}

static int plms_fmt_tran_01(BYTE *pTran, void *pAppl) {
	FK_DATAMASK01 *ptr = (FK_DATAMASK01*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK01)) == 0) {
		int nPos = 0;
		hex2bcd(ptr->nSec, &pTran[nPos++]);
		hex2bcd(ptr->nMin, &pTran[nPos++]);
		hex2bcd(ptr->nHour, &pTran[nPos++]);
		hex2bcd(ptr->nDay, &pTran[nPos++]);

		BYTE b1 = 0, b2 = 0;
		hex2bcd(ptr->nMonWeek.mask.nMonth, &b1);
		hex2bcd(ptr->nMonWeek.mask.nWeek, &b2);

		pTran[nPos++] = b1 | (b2 << 5);
		hex2bcd(ptr->nYear, &pTran[nPos++]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK01));
	}
	return 6;
}

// 数据格式02
static int plms_fmt_appl_02(BYTE *pTran, void *pAppl) {
	FK_DATAMASK02 *ptr = (FK_DATAMASK02*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		WORD wVal = 0;
		bcd2word(pTran[0], pTran[1] & 0x0F, &wVal);
		BYTE G = (pTran[1] & 0xE0) >> 5;
		if (G == 0)
			ptr->dbVal = wVal * 10000;
		else if (G == 1)
			ptr->dbVal = wVal * 1000;
		else if (G == 2)
			ptr->dbVal = wVal * 100;
		else if (G == 3)
			ptr->dbVal = wVal * 10;
		else if (G == 4)
			ptr->dbVal = wVal;
		else if (G == 5)
			ptr->dbVal = (double) wVal / 10;
		else if (G == 6)
			ptr->dbVal = (double) wVal / 100;
		else if (G == 7)
			ptr->dbVal = (double) wVal / 1000;
		if (((pTran[1] & 0x10) >> 4) == 1) {
			ptr->dbVal *= -1;
		}
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK02));
	}
	return 2;
}

static int plms_fmt_tran_02(BYTE *pTran, void *pAppl) {
	FK_DATAMASK02 *ptr = (FK_DATAMASK02*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK02)) == 0) {
		BYTE S = 0;
		double db = ptr->dbVal;
		int temp = (int) db;
		if (temp < 0) {
			db *= -1;
			S = 1;
		}

		BYTE G = 0;
		WORD wVal = (WORD) db;

		if (db >= 10000000.0) {
			wVal = 999;
			G = 0;
		} else if (db >= 1000000.0) {
			wVal = (db + 5000) / 10000.0;
			G = 0;
		} else if (db >= 100000.0) {
			wVal = (db + 500) / 1000.0;
			G = 1;
		} else if (db >= 10000.0) {
			wVal = (db + 50) / 100.0;
			G = 2;
		} else if (db >= 1000.0) {
			wVal = (db + 5) / 10.0;
			G = 3;
		} else if (db >= 100.0) {
			wVal = (db + 0.5);
			G = 4;
		} else if (db >= 10.0) {
			wVal = (db + 0.05) * 10.0;
			G = 5;
		} else if (db >= 1.0) {
			wVal = (db + 0.005) * 100.0;
			G = 6;
		} else {
			wVal = (db + 0.0005) * 1000.0;
			G = 7;
		}

		word2bcd(wVal, &pTran[0], &pTran[1]);
		pTran[1] &= 0x0F;
		pTran[1] |= (S << 4);
		pTran[1] |= (G & 0x07) << 5;
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK02));
	}
	return 2;
}

// 数据格式03
static int plms_fmt_appl_03(BYTE *pTran, void *pAppl) {
	FK_DATAMASK03 *ptr = (FK_DATAMASK03*) pAppl;
	if (plms_valid(pTran, 4) == 0) {
		DWORD dwVal = 0;
		bcd2long(pTran[0], pTran[1], pTran[2], pTran[3] & 0x0F, &dwVal);

		BYTE S = (pTran[3] & 0x10) >> 4;
		BYTE G = (pTran[3] & 0x40) >> 6;

		if (G == 1) {
			ptr->dbVal = dwVal * 1000;
		} else {
			ptr->dbVal = dwVal;
		}

		if (S == 1) {
			ptr->dbVal *= -1;
		}
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK03));
	}
	return 4;
}

static int plms_fmt_tran_03(BYTE *pTran, void *pAppl) {
	FK_DATAMASK03 *ptr = (FK_DATAMASK03*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK03)) == 0) {
		BYTE S = 0;
		BYTE G = 0;

		double db = ptr->dbVal;
		int temp = (int) db;
		if (temp < 0) {
			db *= -1;
			S = 1;
		}

		if (db > 9999999.0) {
			G = 1;
			db /= 1000;
		}

		DWORD val = (DWORD) db;
		long2bcd(val, &pTran[0], &pTran[1], &pTran[2], &pTran[3]);
		pTran[3] &= 0x0F;
		pTran[3] |= (S << 4);
		pTran[3] |= (G << 6);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK03));
	}
	return 4;
}

// 数据格式04
static int plms_fmt_appl_04(BYTE *pTran, void *pAppl) {
	FK_DATAMASK04 *ptr = (FK_DATAMASK04*) pAppl;
	if (plms_valid(pTran, 1) == 0) {
		ptr->S0 = (pTran[0] & 0x80) >> 7;
		BYTE val = bcd2hex(pTran[0] & 0x7F);
		ptr->nVal = val;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK04));
	}
	return 1;
}

static int plms_fmt_tran_04(BYTE *pTran, void *pAppl) {
	FK_DATAMASK04 *ptr = (FK_DATAMASK04*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK04)) == 0) {
		hex2bcd(ptr->nVal, &pTran[0]);
		pTran[0] &= 0x7F;
		pTran[0] |= (ptr->S0 << 7);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK04));
	}
	return 1;
}

// 数据格式05
static int plms_fmt_appl_05(BYTE *pTran, void *pAppl) {
	FK_DATAMASK05 *ptr = (FK_DATAMASK05*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		WORD wVal = 0;
		bcd2word(pTran[0], pTran[1] & 0x7F, &wVal);
		ptr->sVal =
				(((pTran[1] & 0x80) >> 7) == 0) ?
						(short) wVal : -1 * (short) wVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK05));
	}
	return 2;
}

static int plms_fmt_tran_05(BYTE *pTran, void *pAppl) {
	FK_DATAMASK05 *ptr = (FK_DATAMASK05*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK05)) == 0) {
		BYTE S = (ptr->sVal >= 0.0) ? 0 : 1;
		WORD val = (ptr->sVal >= 0.0) ? ptr->sVal : -1 * ptr->sVal;
		word2bcd(val, &pTran[0], &pTran[1]);
		pTran[1] &= 0x7F;
		pTran[1] |= (S << 7);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK05));
	}
	return 2;
}

// 数据格式06
static int plms_fmt_appl_06(BYTE *pTran, void *pAppl) {
	FK_DATAMASK06 *ptr = (FK_DATAMASK06*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		WORD wVal = 0;
		bcd2word(pTran[0], pTran[1] & 0x7F, &wVal);
		ptr->sVal =
				(((pTran[1] & 0x80) >> 7) == 0) ?
						(short) wVal : -1 * (short) wVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK06));
	}
	return 2;
}

static int plms_fmt_tran_06(BYTE *pTran, void *pAppl) {
	FK_DATAMASK06 *ptr = (FK_DATAMASK06*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK06)) == 0) {
		BYTE S = (ptr->sVal >= 0.0) ? 0 : 1;
		WORD val = (ptr->sVal >= 0.0) ? ptr->sVal : -1 * ptr->sVal;
		word2bcd(val, &pTran[0], &pTran[1]);
		pTran[1] &= 0x7F;
		pTran[1] |= (S << 7);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK06));
	}
	return 2;
}

// 数据格式07
static int plms_fmt_appl_07(BYTE *pTran, void *pAppl) {
	FK_DATAMASK07 *ptr = (FK_DATAMASK07*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		WORD wVal = 0;
		bcd2word(pTran[0], pTran[1], &wVal);
		ptr->wVal = wVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK07));
	}
	return 2;
}

static int plms_fmt_tran_07(BYTE *pTran, void *pAppl) {
	FK_DATAMASK07 *ptr = (FK_DATAMASK07*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK07)) == 0) {
		word2bcd(ptr->wVal, &pTran[0], &pTran[1]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK07));
	}
	return 2;
}

// 数据格式08
static int plms_fmt_appl_08(BYTE *pTran, void *pAppl) {
	FK_DATAMASK08 *ptr = (FK_DATAMASK08*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		WORD wVal = 0;
		bcd2word(pTran[0], pTran[1], &wVal);
		ptr->wVal = wVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK08));
	}
	return 2;
}

static int plms_fmt_tran_08(BYTE *pTran, void *pAppl) {
	FK_DATAMASK08 *ptr = (FK_DATAMASK08*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK08)) == 0) {
		word2bcd(ptr->wVal, &pTran[0], &pTran[1]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK08));
	}
	return 2;
}

// 数据格式09
static int plms_fmt_appl_09(BYTE *pTran, void *pAppl) {
	FK_DATAMASK09 *ptr = (FK_DATAMASK09*) pAppl;
	if (plms_valid(pTran, 3) == 0) {
		DWORD dwVal = 0;
		bcd2long(pTran[0], pTran[1], pTran[2] & 0x7F, 0, &dwVal);
		ptr->nVal =
				(((pTran[2] & 0x80) >> 7) == 0) ?
						(int) dwVal : -1 * (int) dwVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK09));
	}
	return 3;
}

static int plms_fmt_tran_09(BYTE *pTran, void *pAppl) {
	FK_DATAMASK09 *ptr = (FK_DATAMASK09*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK09)) == 0) {
		BYTE S = (ptr->nVal >= 0.0) ? 0 : 1;
		DWORD val = (ptr->nVal >= 0.0) ? ptr->nVal : -1 * ptr->nVal;
		BYTE temp = 0;
		long2bcd(val, &pTran[0], &pTran[1], &pTran[2], &temp);
		pTran[2] &= 0x7F;
		pTran[2] |= (S << 7);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK09));
	}
	return 3;
}

// 数据格式10
static int plms_fmt_appl_10(BYTE *pTran, void *pAppl) {
	FK_DATAMASK10 *ptr = (FK_DATAMASK10*) pAppl;
	if (plms_valid(pTran, 3) == 0) {
		DWORD dwVal = 0;
		bcd2long(pTran[0], pTran[1], pTran[2], 0, &dwVal);
		ptr->dwVal = dwVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK10));
	}
	return 3;
}

static int plms_fmt_tran_10(BYTE *pTran, void *pAppl) {
	FK_DATAMASK10 *ptr = (FK_DATAMASK10*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK10)) == 0) {
		BYTE temp = 0;
		long2bcd(ptr->dwVal, &pTran[0], &pTran[1], &pTran[2], &temp);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK10));
	}
	return 3;
}

// 数据格式11
static int plms_fmt_appl_11(BYTE *pTran, void *pAppl) {
	FK_DATAMASK11 *ptr = (FK_DATAMASK11*) pAppl;
	if (plms_valid(pTran, 4) == 0) {
		DWORD dwVal = 0;
		bcd2long(pTran[0], pTran[1], pTran[2], pTran[3], &dwVal);
		ptr->dwVal = dwVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK11));
	}
	return 4;
}

static int plms_fmt_tran_11(BYTE *pTran, void *pAppl) {
	FK_DATAMASK11 *ptr = (FK_DATAMASK11*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK11)) == 0) {
		long2bcd(ptr->dwVal, &pTran[0], &pTran[1], &pTran[2], &pTran[3]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK11));
	}
	return 4;
}

// 数据格式12
static int plms_fmt_appl_12(BYTE *pTran, void *pAppl) {
	FK_DATAMASK12 *ptr = (FK_DATAMASK12*) pAppl;
	if (plms_valid(pTran, 6) == 0) {
		DWORD lo = 0;
		DWORD hi = 0;
		bcd2long(pTran[0], pTran[1], pTran[2], pTran[3], &lo);
		bcd2long(pTran[4], pTran[5], 0, 0, &hi);
		ptr->llVal = (long long) hi * 100000000 + (long long) lo;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK12));
	}
	return 6;
}

static int plms_fmt_tran_12(BYTE *pTran, void *pAppl) {
	FK_DATAMASK12 *ptr = (FK_DATAMASK12*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK12)) == 0) {
		BYTE temp1 = 0;
		BYTE temp2 = 0;
		DWORD lo = (DWORD) (ptr->llVal % 100000000);
		DWORD hi = (DWORD) (ptr->llVal / 100000000);
		long2bcd(lo, &pTran[0], &pTran[1], &pTran[2], &pTran[3]);
		long2bcd(hi, &pTran[4], &pTran[5], &temp1, &temp2);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK12));
	}
	return 6;
}

// 数据格式13
static int plms_fmt_appl_13(BYTE *pTran, void *pAppl) {
	FK_DATAMASK13 *ptr = (FK_DATAMASK13*) pAppl;
	if (plms_valid(pTran, 4) == 0) {
		DWORD dwVal = 0;
		bcd2long(pTran[0], pTran[1], pTran[2], pTran[3], &dwVal);
		ptr->dwVal = dwVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK13));
	}
	return 4;
}

static int plms_fmt_tran_13(BYTE *pTran, void *pAppl) {
	FK_DATAMASK13 *ptr = (FK_DATAMASK13*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK13)) == 0) {
		long2bcd(ptr->dwVal, &pTran[0], &pTran[1], &pTran[2], &pTran[3]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK13));
	}
	return 4;
}

// 数据格式14
static int plms_fmt_appl_14(BYTE *pTran, void *pAppl) {
	FK_DATAMASK14 *ptr = (FK_DATAMASK14*) pAppl;
	if (plms_valid(pTran, 5) == 0) {
		DWORD lo = 0;
		DWORD hi = 0;
		bcd2long(pTran[0], pTran[1], pTran[2], pTran[3], &lo);
		bcd2long(pTran[4], 0, 0, 0, &hi);
		ptr->llVal = (long long) hi * 100000000 + (long long) lo;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK14));
	}
	return 5;
}

static int plms_fmt_tran_14(BYTE *pTran, void *pAppl) {
	FK_DATAMASK14 *ptr = (FK_DATAMASK14*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK14)) == 0) {
		BYTE temp1 = 0;
		BYTE temp2 = 0;
		BYTE temp3 = 0;
		DWORD lo = (DWORD) (ptr->llVal % 100000000);
		DWORD hi = (DWORD) (ptr->llVal / 100000000);
		long2bcd(lo, &pTran[0], &pTran[1], &pTran[2], &pTran[3]);
		long2bcd(hi, &pTran[4], &temp1, &temp2, &temp3);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK14));
	}
	return 5;
}

// 数据格式15
static int plms_fmt_appl_15(BYTE *pTran, void *pAppl) {
	FK_DATAMASK15 *ptr = (FK_DATAMASK15*) pAppl;
	if (plms_valid(pTran, 5) == 0) {
		BYTE nPos = 0;
		ptr->nMin = bcd2hex(pTran[nPos++]);
		ptr->nHour = bcd2hex(pTran[nPos++]);
		ptr->nDay = bcd2hex(pTran[nPos++]);
		ptr->nMonth = bcd2hex(pTran[nPos++]);
		ptr->nYear = bcd2hex(pTran[nPos++]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK15));
	}
	return 5;
}

static int plms_fmt_tran_15(BYTE *pTran, void *pAppl) {
	FK_DATAMASK15 *ptr = (FK_DATAMASK15*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK15)) == 0) {
		BYTE nPos = 0;
		hex2bcd(ptr->nMin, &pTran[nPos++]);
		hex2bcd(ptr->nHour, &pTran[nPos++]);
		hex2bcd(ptr->nDay, &pTran[nPos++]);
		hex2bcd(ptr->nMonth, &pTran[nPos++]);
		hex2bcd(ptr->nYear, &pTran[nPos++]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK15));
	}
	return 5;
}

// 数据格式16
static int plms_fmt_appl_16(BYTE *pTran, void *pAppl) {
	FK_DATAMASK16 *ptr = (FK_DATAMASK16*) pAppl;
	if (plms_valid(pTran, 4) == 0) {
		BYTE nPos = 0;
		ptr->nSec = bcd2hex(pTran[nPos++]);
		ptr->nMin = bcd2hex(pTran[nPos++]);
		ptr->nHour = bcd2hex(pTran[nPos++]);
		ptr->nDay = bcd2hex(pTran[nPos++]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK16));
	}
	return 4;
}

static int plms_fmt_tran_16(BYTE *pTran, void *pAppl) {
	FK_DATAMASK16 *ptr = (FK_DATAMASK16*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK16)) == 0) {
		BYTE nPos = 0;
		hex2bcd(ptr->nSec, &pTran[nPos++]);
		hex2bcd(ptr->nMin, &pTran[nPos++]);
		hex2bcd(ptr->nHour, &pTran[nPos++]);
		hex2bcd(ptr->nDay, &pTran[nPos++]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK16));
	}
	return 4;
}

// 数据格式17
static int plms_fmt_appl_17(BYTE *pTran, void *pAppl) {
	FK_DATAMASK17 *ptr = (FK_DATAMASK17*) pAppl;
	if (plms_valid(pTran, 4) == 0) {
		BYTE nPos = 0;
		ptr->nMin = bcd2hex(pTran[nPos++]);
		ptr->nHour = bcd2hex(pTran[nPos++]);
		ptr->nDay = bcd2hex(pTran[nPos++]);
		ptr->nMonth = bcd2hex(pTran[nPos++]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK17));
	}
	return 4;
}

static int plms_fmt_tran_17(BYTE *pTran, void *pAppl) {
	FK_DATAMASK17 *ptr = (FK_DATAMASK17*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK17)) == 0) {
		BYTE nPos = 0;
		hex2bcd(ptr->nMin, &pTran[nPos++]);
		hex2bcd(ptr->nHour, &pTran[nPos++]);
		hex2bcd(ptr->nDay, &pTran[nPos++]);
		hex2bcd(ptr->nMonth, &pTran[nPos++]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK17));
	}
	return 4;
}

// 数据格式18
static int plms_fmt_appl_18(BYTE *pTran, void *pAppl) {
	FK_DATAMASK18 *ptr = (FK_DATAMASK18*) pAppl;
	if (plms_valid(pTran, 3) == 0) {
		BYTE nPos = 0;
		ptr->nMin = bcd2hex(pTran[nPos++]);
		ptr->nHour = bcd2hex(pTran[nPos++]);
		ptr->nDay = bcd2hex(pTran[nPos++]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK18));
	}
	return 3;
}

static int plms_fmt_tran_18(BYTE *pTran, void *pAppl) {
	FK_DATAMASK18 *ptr = (FK_DATAMASK18*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK18)) == 0) {
		BYTE nPos = 0;
		hex2bcd(ptr->nMin, &pTran[nPos++]);
		hex2bcd(ptr->nHour, &pTran[nPos++]);
		hex2bcd(ptr->nDay, &pTran[nPos++]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK18));
	}
	return 3;
}

// 数据格式19
static int plms_fmt_appl_19(BYTE *pTran, void *pAppl) {
	FK_DATAMASK19 *ptr = (FK_DATAMASK19*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		BYTE nPos = 0;
		ptr->nMin = bcd2hex(pTran[nPos++]);
		ptr->nHour = bcd2hex(pTran[nPos++]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK19));
	}
	return 2;
}

static int plms_fmt_tran_19(BYTE *pTran, void *pAppl) {
	FK_DATAMASK19 *ptr = (FK_DATAMASK19*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK19)) == 0) {
		BYTE nPos = 0;
		hex2bcd(ptr->nMin, &pTran[nPos++]);
		hex2bcd(ptr->nHour, &pTran[nPos++]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK19));
	}
	return 2;
}

// 数据格式20
static int plms_fmt_appl_20(BYTE *pTran, void *pAppl) {
	FK_DATAMASK20 *ptr = (FK_DATAMASK20*) pAppl;
	if (plms_valid(pTran, 3) == 0) {
		BYTE nPos = 0;
		ptr->nDay = bcd2hex(pTran[nPos++]);
		ptr->nMonth = bcd2hex(pTran[nPos++]);
		ptr->nYear = bcd2hex(pTran[nPos++]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK20));
	}
	return 3;
}

static int plms_fmt_tran_20(BYTE *pTran, void *pAppl) {
	FK_DATAMASK20 *ptr = (FK_DATAMASK20*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK20)) == 0) {
		BYTE nPos = 0;
		hex2bcd(ptr->nDay, &pTran[nPos++]);
		hex2bcd(ptr->nMonth, &pTran[nPos++]);
		hex2bcd(ptr->nYear, &pTran[nPos++]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK20));
	}
	return 3;
}

// 数据格式21
static int plms_fmt_appl_21(BYTE *pTran, void *pAppl) {
	FK_DATAMASK21 *ptr = (FK_DATAMASK21*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		BYTE nPos = 0;
		ptr->nMonth = bcd2hex(pTran[nPos++]);
		ptr->nYear = bcd2hex(pTran[nPos++]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK21));
	}
	return 2;
}

static int plms_fmt_tran_21(BYTE *pTran, void *pAppl) {
	FK_DATAMASK21 *ptr = (FK_DATAMASK21*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK21)) == 0) {
		BYTE nPos = 0;
		hex2bcd(ptr->nMonth, &pTran[nPos++]);
		hex2bcd(ptr->nYear, &pTran[nPos++]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK21));
	}
	return 2;
}

// 数据格式22
static int plms_fmt_appl_22(BYTE *pTran, void *pAppl) {
	FK_DATAMASK22 *ptr = (FK_DATAMASK22*) pAppl;
	if (plms_valid(pTran, 1) == 0) {
		ptr->nVal = bcd2hex(pTran[0]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK22));
	}
	return 1;
}

static int plms_fmt_tran_22(BYTE *pTran, void *pAppl) {
	FK_DATAMASK22 *ptr = (FK_DATAMASK22*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK22)) == 0) {
		hex2bcd(ptr->nVal, &pTran[0]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK22));
	}
	return 1;
}

// 数据格式23
static int plms_fmt_appl_23(BYTE *pTran, void *pAppl) {
	FK_DATAMASK23 *ptr = (FK_DATAMASK23*) pAppl;
	if (plms_valid(pTran, 3) == 0) {
		DWORD dwVal = 0;
		bcd2long(pTran[0], pTran[1], pTran[2], 0, &dwVal);
		ptr->dwVal = dwVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK23));
	}
	return 3;
}

static int plms_fmt_tran_23(BYTE *pTran, void *pAppl) {
	FK_DATAMASK23 *ptr = (FK_DATAMASK23*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK23)) == 0) {
		BYTE temp = 0;
		long2bcd(ptr->dwVal, &pTran[0], &pTran[1], &pTran[2], &temp);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK23));
	}
	return 3;
}

// 698增补

// 数据格式24
static int plms_fmt_appl_24(BYTE *pTran, void *pAppl) {
	FK_DATAMASK24 *ptr = (FK_DATAMASK24*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		ptr->hour = bcd2hex(pTran[0]);
		ptr->day = bcd2hex(pTran[1]);
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK24));
	}
	return 2;
}

static int plms_fmt_tran_24(BYTE *pTran, void *pAppl) {
	FK_DATAMASK24 *ptr = (FK_DATAMASK24*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK24)) == 0) {
		hex2bcd(ptr->hour, &pTran[0]);
		hex2bcd(ptr->day, &pTran[1]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK24));
	}
	return 2;
}

// 数据格式25
static int plms_fmt_appl_25(BYTE *pTran, void *pAppl) {
	FK_DATAMASK25 *ptr = (FK_DATAMASK25*) pAppl;
	if (plms_valid(pTran, 3) == 0) {
		DWORD dwVal = 0;
		bcd2long(pTran[0], pTran[1], pTran[2] & 0x7F, 0, &dwVal);
		ptr->dbVal =
				(((pTran[2] & 0x80) >> 7) == 0) ?
						(int) dwVal : -1 * (int) dwVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK25));
	}
	return 3;
}

static int plms_fmt_tran_25(BYTE *pTran, void *pAppl) {
	FK_DATAMASK25 *ptr = (FK_DATAMASK25*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK25)) == 0) {
		BYTE S = (ptr->dbVal >= 0.0) ? 0 : 1;
		long val = (ptr->dbVal >= 0.0) ? ptr->dbVal : -1 * ptr->dbVal;
		BYTE temp = 0;

		long2bcd(val, &pTran[0], &pTran[1], &pTran[2], &temp);
		pTran[2] &= 0x7F;
		pTran[2] |= (S << 7);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK25));
	}
	return 3;
}

// 数据格式26
static int plms_fmt_appl_26(BYTE *pTran, void *pAppl) {
	FK_DATAMASK26 *ptr = (FK_DATAMASK26*) pAppl;
	if (plms_valid(pTran, 2) == 0) {
		WORD wVal = 0;
		bcd2word(pTran[0], pTran[1], &wVal);
		ptr->wVal = wVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK26));
	}
	return 2;
}

static int plms_fmt_tran_26(BYTE *pTran, void *pAppl) {
	FK_DATAMASK26 *ptr = (FK_DATAMASK26*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK26)) == 0) {
		word2bcd(ptr->wVal, &pTran[0], &pTran[1]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK26));
	}
	return 2;
}

// 数据格式27
static int plms_fmt_appl_27(BYTE *pTran, void *pAppl) {
	FK_DATAMASK27 *ptr = (FK_DATAMASK27*) pAppl;
	if (plms_valid(pTran, 4) == 0) {
		DWORD dwVal = 0;
		bcd2long(pTran[0], pTran[1], pTran[2], pTran[3], &dwVal);
		ptr->dbVal = dwVal;
	} else {
		memcpy(ptr, pTran, sizeof(FK_DATAMASK27));
	}
	return 4;
}

static int plms_fmt_tran_27(BYTE *pTran, void *pAppl) {
	FK_DATAMASK27 *ptr = (FK_DATAMASK27*) pAppl;
	if (plms_valid(ptr, sizeof(FK_DATAMASK27)) == 0) {
		DWORD val = (DWORD) ptr->dbVal;
		long2bcd(val, &pTran[0], &pTran[1], &pTran[2], &pTran[3]);
	} else {
		memcpy(pTran, ptr, sizeof(FK_DATAMASK27));
	}
	return 4;
}
//
static const FMT1 fmt1[] = { { 1, plms_fmt_appl_01, plms_fmt_tran_01 }, { 2,
		plms_fmt_appl_02, plms_fmt_tran_02 }, { 3, plms_fmt_appl_03,
		plms_fmt_tran_03 }, { 4, plms_fmt_appl_04, plms_fmt_tran_04 }, { 5,
		plms_fmt_appl_05, plms_fmt_tran_05 }, { 6, plms_fmt_appl_06,
		plms_fmt_tran_06 }, { 7, plms_fmt_appl_07, plms_fmt_tran_07 }, { 8,
		plms_fmt_appl_08, plms_fmt_tran_08 }, { 9, plms_fmt_appl_09,
		plms_fmt_tran_09 }, { 10, plms_fmt_appl_10, plms_fmt_tran_10 }, { 11,
		plms_fmt_appl_11, plms_fmt_tran_11 }, { 12, plms_fmt_appl_12,
		plms_fmt_tran_12 }, { 13, plms_fmt_appl_13, plms_fmt_tran_13 }, { 14,
		plms_fmt_appl_14, plms_fmt_tran_14 }, { 15, plms_fmt_appl_15,
		plms_fmt_tran_15 }, { 16, plms_fmt_appl_16, plms_fmt_tran_16 }, { 17,
		plms_fmt_appl_17, plms_fmt_tran_17 }, { 18, plms_fmt_appl_18,
		plms_fmt_tran_18 }, { 19, plms_fmt_appl_19, plms_fmt_tran_19 }, { 20,
		plms_fmt_appl_20, plms_fmt_tran_20 }, { 21, plms_fmt_appl_21,
		plms_fmt_tran_21 }, { 22, plms_fmt_appl_22, plms_fmt_tran_22 }, { 23,
		plms_fmt_appl_23, plms_fmt_tran_23 }, { 24, plms_fmt_appl_24,
		plms_fmt_tran_24 }, { 25, plms_fmt_appl_25, plms_fmt_tran_25 }, { 26,
		plms_fmt_appl_26, plms_fmt_tran_26 }, { 27, plms_fmt_appl_27,
		plms_fmt_tran_27 }, };

static const FMT1* Find(int nClass) {
	int nCount = sizeof(fmt1) / sizeof(FMT1);
	for (int i = 0; i < nCount; ++i) {
		if (fmt1[i].nClass == nClass) {
			return &fmt1[i];
		}
	}
	return nullptr;
}

int plms_fmt_appl(int nClass, BYTE *pTran, void *pAppl) {
	const FMT1* ptr = Find(nClass);
	if (ptr != nullptr) {
		return ptr->pfAppl(pTran, pAppl);
	}
	return 0;
}

int plms_fmt_tran(int nClass, BYTE *pTran, void *pAppl) {
	const FMT1 *ptr = Find(nClass);
	if (ptr != nullptr) {
		return ptr->pfTran(pTran, pAppl);
	}
	return 0;
}

// 时标转换
// 小时冻结时标
/*int plms_tdh_tran( BYTE *pTran, FK_TDH *hTd )
 {
 int nPos = 0;
 pTran[nPos++] = ((hTd->nDensity&0x03)<<6)|((hTd->nHour/10)<<4)|(hTd->nHour%10);
 return nPos;
 }

 int plms_tdh_appl( BYTE *pTran, FK_TDH *hTd )
 {
 int nPos = 0;
 hTd->nDensity = (pTran[nPos]>>6);
 hTd->nHour    = (((pTran[nPos]&0x1F)>>4)*10)+(pTran[nPos]&0x0F);
 nPos++;
 return nPos;
 }*/
int plms_tdh_tran(BYTE *pTran, FK_TDH *hTd) {
	int nPos = 0;
	pTran[nPos++] = ((hTd->nHour / 10) << 4) | (hTd->nHour % 10);
	pTran[nPos++] = hTd->nDensity;
	return nPos;
}

int plms_tdh_appl(BYTE *pTran, FK_TDH *hTd) {
	int nPos = 0;
	hTd->nHour = (((pTran[nPos] & 0x3F) >> 4) * 10) + (pTran[nPos] & 0x0F);
	nPos++;
	hTd->nDensity = pTran[nPos++];

	return nPos;
}

// 日冻结时标
int plms_tdd_tran(BYTE *pTran, FK_TDD *hTd) {
	int nPos = 0;
	nPos += plms_fmt_tran_20(&pTran[nPos], &hTd->date);
	return nPos;
}

int plms_tdd_appl(BYTE *pTran, FK_TDD *hTd) {
	int nPos = 0;
	nPos += plms_fmt_appl_20(&pTran[nPos], &hTd->date);
	return nPos;
}

// 曲线冻结时标
int plms_tdc_tran(BYTE *pTran, FK_TDC *hTd) {
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &hTd->date);
	pTran[nPos++] = hTd->nDensity;
	pTran[nPos++] = hTd->nCount;
	return nPos;
}

int plms_tdc_appl(BYTE *pTran, FK_TDC *hTd) {
	int nPos = 0;
	nPos += plms_fmt_appl_15(&pTran[nPos], &hTd->date);
	hTd->nDensity = pTran[nPos++];
	hTd->nCount = pTran[nPos++];
	return nPos;
}

// 月冻结时标
int plms_tdm_tran(BYTE *pTran, FK_TDM *hTd) {
	int nPos = 0;
	nPos += plms_fmt_tran_21(&pTran[nPos], &hTd->date);
	return nPos;
}

int plms_tdm_appl(BYTE *pTran, FK_TDM *hTd) {
	int nPos = 0;
	nPos += plms_fmt_appl_21(&pTran[nPos], &hTd->date);
	return nPos;
}

// 数据项转换

typedef int (*fmt)(BYTE *pTran, void *pAppl);
typedef struct tag_FMT2 {
	BYTE afn;
	BYTE fn;
	fmt pf;
} FMT2;

///////////////////////////////////////////////////////////////////////////////
// AFN = 04(设置参数)
///////////////////////////////////////////////////////////////////////////////
// 组1
// F1终端通信参数设置
static int plms_04F01(BYTE *pTran, void *pAppl) {
	FK_DT04F01 *ptr = (FK_DT04F01*) pAppl;
	int nPos = 0;
	ptr->nRTStimes = pTran[nPos++];
	ptr->nStartMins = pTran[nPos++];
	ptr->wOverTimes = MAKEWORD(pTran[nPos], pTran[nPos + 1] & 0x0F);
	nPos++;
	ptr->nRetryCount = (pTran[nPos++] >> 4) & 0x03;
	ptr->nAckFlag = pTran[nPos++];
	ptr->nHearBeat = pTran[nPos++];
	return nPos;
}

// F2终端中继转发设置
static int plms_04F02(BYTE *pTran, void *pAppl) {
	FK_DT04F02 *ptr = (FK_DT04F02*) pAppl;
	int nPos = 0;
	ptr->nAgree = pTran[nPos] >> 7;
	ptr->nNum = pTran[nPos++] & 0x7F;

	int nNums = min(ptr->nNum, NL_FK_MAX_RETRANS);
	for (int i = 0; i < nNums; ++i) {
		ptr->wAddr[i] = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
		nPos += 2;
	}
	return nPos;
}

// F3主站IP地址和端口
static int plms_04F03(BYTE *pTran, void *pAppl) {
	FK_DT04F03 *ptr = (FK_DT04F03*) pAppl;
	int nPos = 0;
	ptr->IPHost.nAddr1 = pTran[nPos++];
	ptr->IPHost.nAddr2 = pTran[nPos++];
	ptr->IPHost.nAddr3 = pTran[nPos++];
	ptr->IPHost.nAddr4 = pTran[nPos++];
	ptr->IPHost.nPort = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	ptr->IPAux.nAddr1 = pTran[nPos++];
	ptr->IPAux.nAddr2 = pTran[nPos++];
	ptr->IPAux.nAddr3 = pTran[nPos++];
	ptr->IPAux.nAddr4 = pTran[nPos++];
	ptr->IPAux.nPort = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;

	memcpy(ptr->APN, &pTran[nPos], 16);
	nPos += 16;
	return nPos;
}

// F4主站电话号码
static int plms_04F04(BYTE *pTran, void *pAppl) {
	FK_DT04F04 *ptr = (FK_DT04F04*) pAppl;
	int nPos = 0;
	int nInx = 0;
	for (int i = 0; i < 8; ++i) {
		ptr->hHost.Code[nInx++] = pTran[nPos++];
	}
	nInx = 0;
	for (int j = 0; j < 8; ++j) {
		ptr->hSmsCenter.Code[nInx++] = pTran[nPos++];
	}
	return nPos;
}

// F5终端消息认证参数设置
static int plms_04F05(BYTE *pTran, void *pAppl) {
	FK_DT04F05 *ptr = (FK_DT04F05*) pAppl;
	int nPos = 0;
	ptr->nID = pTran[nPos++];
	ptr->nParam[0] = pTran[nPos++];
	ptr->nParam[1] = pTran[nPos++];
	return nPos;
}

// F6终端组地址设置
static int plms_04F06(BYTE *pTran, void *pAppl) {
	FK_DT04F06 *ptr = (FK_DT04F06*) pAppl;
	int nPos = 0;
	for (int i = 0; i < 8; ++i) {
		ptr->nGroupAdr[i] = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
		nPos += 2;
	}
	return nPos;
}

// F7终端IP地址和端口
static int plms_04F07(BYTE *pTran, void *pAppl) {
	FK_DT04F07 *ptr = (FK_DT04F07*) pAppl;
	int nPos = 0;
	BYTE len;
	ptr->IPDevice.nAddr1 = pTran[nPos++];
	ptr->IPDevice.nAddr2 = pTran[nPos++];
	ptr->IPDevice.nAddr3 = pTran[nPos++];
	ptr->IPDevice.nAddr4 = pTran[nPos++];

	ptr->IPMask.nAddr1 = pTran[nPos++];
	ptr->IPMask.nAddr2 = pTran[nPos++];
	ptr->IPMask.nAddr3 = pTran[nPos++];
	ptr->IPMask.nAddr4 = pTran[nPos++];

	ptr->IPGetway.nAddr1 = pTran[nPos++];
	ptr->IPGetway.nAddr2 = pTran[nPos++];
	ptr->IPGetway.nAddr3 = pTran[nPos++];
	ptr->IPGetway.nAddr4 = pTran[nPos++];

	ptr->AgentType = pTran[nPos++];
	ptr->IPAgent.nAddr1 = pTran[nPos++];
	ptr->IPAgent.nAddr2 = pTran[nPos++];
	ptr->IPAgent.nAddr3 = pTran[nPos++];
	ptr->IPAgent.nAddr4 = pTran[nPos++];
	ptr->IPAgent.nPort = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	ptr->AgentConnectType = pTran[nPos++];

	len = pTran[nPos++];
	if (len > 20) {
		len = 20;
	}
	ptr->UserNameLength = len;
	memcpy(ptr->UserName, &pTran[nPos], len);
	nPos += len;

	len = pTran[nPos++];
	if (len > 20) {
		len = 20;
	}
	ptr->PasswordLength = len;

	memcpy(ptr->Password, &pTran[nPos], len);
	nPos += len;

	ptr->IPDevice.nPort = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	return nPos;
}

// F8终端上行通信工作方式
static int plms_04F08(BYTE *pTran, void *pAppl) {
	FK_DT04F08 *ptr = (FK_DT04F08*) pAppl;
	int nPos = 0;
	ptr->nGPRS.body = pTran[nPos++];
	ptr->wSecs = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	ptr->nCount = pTran[nPos++];
	ptr->nMins = pTran[nPos++];
	memcpy(ptr->nFlag, &pTran[nPos], 3);
	nPos += 3;
	return nPos;
}

// 组2
// F9终端配置数量表
static int plms_04F09(BYTE *pTran, void *pAppl) {
	FK_DT04F09 *ptr = (FK_DT04F09*) pAppl;
	int nPos = 0;
	memcpy(ptr->nRecordFlag, &pTran[nPos], 8);
	nPos += 8;
	memcpy(ptr->nImportFlag, &pTran[nPos], 8);
	nPos += 8;
	return nPos;
}

// F10终端电能表/交流采样装置配置参数
static int plms_04F10(BYTE *pTran, void *pAppl) {

	int nPos = 0;
	FK_DT04F10_LS *ptr = (FK_DT04F10_LS*) pAppl;
	ptr->nCount = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	int nCount = min(ptr->nCount, NL_JZ_MAX_04F10LSBUF_POINT);

	for (int i = 0; i < nCount; ++i) {
		ptr->hMeter[i].nIndex = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
		nPos += 2;
		ptr->hMeter[i].nTN = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
		nPos += 2;
		ptr->hMeter[i].nStoreIndex = 0xffff;

		ptr->hMeter[i].nPort = (pTran[nPos] & 0x1F);
		ptr->hMeter[i].nBaud = (pTran[nPos++] >> 5);

		ptr->hMeter[i].nProtocol = pTran[nPos++];

		memcpy(ptr->hMeter[i].addr, &pTran[nPos], 6);
		nPos += 6;
		memcpy(ptr->hMeter[i].pwd, &pTran[nPos], 6);
		nPos += 6;
		ptr->hMeter[i].nTariffs = (pTran[nPos++] & 0x3F);

		ptr->hMeter[i].nDecimal = ((pTran[nPos] & 0x03) + 1);
		ptr->hMeter[i].nInter = ((pTran[nPos++] & 0x0C) >> 2) + 4;

		memcpy(ptr->hMeter[i].collAddr, &pTran[nPos], 6);
		nPos += 6;
		ptr->hMeter[i].nUserBigKind = (pTran[nPos] & 0xf0) >> 4; 	//用户大类号
		ptr->hMeter[i].nUserSmallKind = pTran[nPos++] & 0x0f;
	}
	return nPos;
}
// F11终端脉冲配置参数
static int plms_04F11(BYTE *pTran, void *pAppl) {
	int nPos = 0;
	FK_DT04F11_LS *ptr = (FK_DT04F11_LS*) pAppl;
	ptr->nCount = pTran[nPos++];
	int nCount = min(ptr->nCount, NL_FK_MAX_MC);

	for (int i = 0; i < nCount; ++i) {
		ptr->hPulse[i].nPort = pTran[nPos++];
		ptr->hPulse[i].nTN = pTran[nPos++];
		ptr->hPulse[i].nKind = pTran[nPos++];
		ptr->hPulse[i].nMK = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
		nPos += 2;
	}
	return nPos;
}

// F12终端状态量输入参数
static int plms_04F12(BYTE *pTran, void *pAppl) {
	FK_DT04F12 *ptr = (FK_DT04F12*) pAppl;
	int nPos = 0;
	ptr->nFlag = pTran[nPos++];
	ptr->nKind = pTran[nPos++];
//	ptr->nWarn = pTran[nPos++];
	return nPos;
}

// F13终端电压/电流模拟量配置参数
static int plms_04F13(BYTE *pTran, void *pAppl) {
	int nPos = 0;
	FK_DT04F13_LS *ptr = (FK_DT04F13_LS*) pAppl;
	ptr->nCount = pTran[nPos++];
	int nCount = min(ptr->nCount, NL_FK_MAX_ML);

	for (int i = 0; i < nCount; ++i) {
		ptr->hDMS[i].nPort = pTran[nPos++];
		ptr->hDMS[i].nTN = pTran[nPos++];
		ptr->hDMS[i].nKind = pTran[nPos++];
	}
	return nPos;
}

// F14终端总加组配置参数
static int plms_04F14(BYTE *pTran, void *pAppl) {
	int nPos = 0;
	FK_DT04F14_LS *ptr = (FK_DT04F14_LS*) pAppl;
	ptr->nCount = pTran[nPos++];
	int nCount = min(ptr->nCount, NL_FK_MAX_SUIT);

	for (int i = 0; i < nCount; ++i) {
		ptr->hSuit[i].nIndex = pTran[nPos++];
		ptr->hSuit[i].nMPs = pTran[nPos++];

		int nSize = min(ptr->hSuit[i].nMPs, 64);
		for (int j = 0; j < nSize; ++j) {
			ptr->hSuit[i].flag[j].body = pTran[nPos++];
			ptr->hSuit[i].flag[j].mask.TN++;
		}
	}
	return nPos;
}

// F15有功总电能差动越限事件参数配置
static int plms_04F15(BYTE *pTran, void *pAppl) {
	int nPos = 0;
	FK_DT04F15_LS *ptr = (FK_DT04F15_LS*) pAppl;
	ptr->nCount = pTran[nPos++];
	int nCount = min(ptr->nCount, NL_FK_MAX_DIFFER);

	for (int i = 0; i < nCount; ++i) {
		ptr->hDiffer[i].nIndex = pTran[nPos++];
		ptr->hDiffer[i].nDstSN = pTran[nPos++];
		ptr->hDiffer[i].nSrcSN = pTran[nPos++];
		ptr->hDiffer[i].nFlag = pTran[nPos++];
		ptr->hDiffer[i].nRelativeVal = pTran[nPos++];
		nPos += plms_fmt_appl_03(&pTran[nPos], &ptr->hDiffer[i].dmUtterly);
	}
	return nPos;
}

// F16虚拟专网用户名、密码
static int plms_04F16(BYTE *pTran, void *pAppl) {
	FK_DT04F16 *ptr = (FK_DT04F16*) pAppl;
	int nPos = 0;
	memcpy(ptr->szVNetUser, &pTran[nPos], 32);
	nPos += 32;
	memcpy(ptr->szVNetPwd, &pTran[nPos], 32);
	nPos += 32;
	return nPos;
}

// 组3
// F17终端保安定值
static int plms_04F17(BYTE *pTran, void *pAppl) {
	FK_DT04F17 *ptr = (FK_DT04F17*) pAppl;
	return plms_fmt_appl_02(&pTran[0], &ptr->dmValue);
}

// F18终端工控时段
static int plms_04F18(BYTE *pTran, void *pAppl) {
	FK_DT04F18 *ptr = (FK_DT04F18*) pAppl;
	int nPos = 0;
	for (int i = 0; i < 12; ++i) {
		ptr->period[i].body = pTran[nPos++];
	}
	return nPos;
}

// F19终端时段控定值浮动系数
static int plms_04F19(BYTE *pTran, void *pAppl) {
	FK_DT04F19 *ptr = (FK_DT04F19*) pAppl;
	return plms_fmt_appl_04(&pTran[0], &ptr->dmValue);
}

// F20终端月电能量控定值浮动系数
static int plms_04F20(BYTE *pTran, void *pAppl) {
	FK_DT04F20 *ptr = (FK_DT04F20*) pAppl;
	return plms_fmt_appl_04(&pTran[0], &ptr->dmValue);
}

// F21终端电能量费率时段和费率数
static int plms_04F21(BYTE *pTran, void *pAppl) {
	FK_DT04F21 *ptr = (FK_DT04F21*) pAppl;
	int nPos = 0;
	memcpy(ptr->period, &pTran[nPos], 48);
	nPos += 48;
	ptr->nTOUs = pTran[nPos++];
	return nPos;
}

// F22终端电能量费率
static int plms_04F22(BYTE *pTran, void *pAppl) {
	FK_DT04F22 *ptr = (FK_DT04F22*) pAppl;
	int nPos = 0;

	ptr->nTOUs = pTran[nPos++];

	int num = min(ptr->nTOUs, 14);
	for (int i = 0; i < num; ++i) {
		nPos += plms_fmt_appl_03(&pTran[nPos], &ptr->dmTOU[i]);
	}

	if (ptr->nTOUs > 14) {
		nPos += 4 * (ptr->nTOUs - 14);
	}
	return nPos;
}

// F23终端催费告警参数
static int plms_04F23(BYTE *pTran, void *pAppl) {
	FK_DT04F23 *ptr = (FK_DT04F23*) pAppl;
	int nPos = 0;
	memcpy(ptr->nWarnFlag, &pTran[nPos], 3);
	nPos += 3;
	return nPos;
}

// 组4
// F25测量但基本参数
static int plms_04F25(BYTE *pTran, void *pAppl) {
	FK_DT04F25 *ptr = (FK_DT04F25*) pAppl;
	int nPos = 0;
	ptr->nPT = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	ptr->nCT = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->dmVoltage);
	nPos += plms_fmt_appl_22(&pTran[nPos], &ptr->dmCurrent);
	nPos += plms_fmt_appl_23(&pTran[nPos], &ptr->dmLoad);
	ptr->nConnect = pTran[nPos++];
	return nPos;
}

// F26测量点限值参数
static int plms_04F26(BYTE *pTran, void *pAppl) {
	FK_DT04F26 *ptr = (FK_DT04F26*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->ulVolt);
	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->llVolt);
	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->blVolt);

	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->uulVolt);
	ptr->uulVoltLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->uulVoltLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->lllVolt);
	ptr->lllVoltLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->lllVoltLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_appl_25(&pTran[nPos], &ptr->uulICurr);
	ptr->uulICurrLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->uulICurrLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_appl_25(&pTran[nPos], &ptr->ulICurr);
	ptr->ulICurrLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->ulICurrLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_appl_25(&pTran[nPos], &ptr->ul0Curr);
	ptr->ul0CurrLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->ul0CurrLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_appl_23(&pTran[nPos], &ptr->uulPower);
	ptr->uulPowerLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->uulPowerLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_appl_23(&pTran[nPos], &ptr->ulPower);
	ptr->ulPowerLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->ulPowerLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lVoltImb);
	ptr->lVoltImbLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->lVoltImbLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lCurrImb);
	ptr->lCurrImbLimitValue.OverLimitTime = pTran[nPos++];
	nPos += plms_fmt_appl_05(&pTran[nPos],
			&ptr->lCurrImbLimitValue.OverLimitComeBackCoefficient);

	ptr->nTimes = pTran[nPos++];
	return nPos;
}

// F27测量点数据冻结参数
static int plms_04F27(BYTE *pTran, void *pAppl) {
	FK_DT04F27 *ptr = (FK_DT04F27*) pAppl;
	int nPos = 0;
	for (int i = 0; i < 3; i++) {
		nPos += plms_fmt_appl_26(&pTran[nPos], &ptr->PhaseRA[i]);
		nPos += plms_fmt_appl_26(&pTran[nPos], &ptr->PhaseXA[i]);
		nPos += plms_fmt_appl_26(&pTran[nPos], &ptr->PhaseGA[i]);
		nPos += plms_fmt_appl_26(&pTran[nPos], &ptr->PhaseBA[i]);
	}
	return nPos;
}

// F28测量点功率因素分段限值
static int plms_04F28(BYTE *pTran, void *pAppl) {
	FK_DT04F28 *ptr = (FK_DT04F28*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->limit1);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->limit2);
	return nPos;
}

// F29电能表显示号
static int plms_04F29(BYTE *pTran, void *pAppl) {
	FK_DT04F29 *ptr = (FK_DT04F29*) pAppl;
	int nPos = 0;

	memcpy(ptr->meternum, &pTran[nPos], 12);

	nPos += 12;
	return nPos;
}

// F30停抄/投抄设置
static int plms_04F30(BYTE *pTran, void *pAppl) {
	FK_DT04F30 *ptr = (FK_DT04F30*) pAppl;
	int nPos = 0;

	ptr->nCollectSwitch = pTran[nPos++];
	return nPos;
}
// F31停抄/投抄设置
static int plms_04F31(BYTE *pTran, void *pAppl) {
	FK_DT04F31 *ptr = (FK_DT04F31*) pAppl;
	int nPos = 0;

	int tol = pTran[nPos++];
	if (tol > 20) {
		tol = 20;
	}

	ptr->AppertainCount = tol;
	for (int i = 0; i < tol; i++) {
		memcpy(&ptr->AppertainAddress[i][0], &pTran[nPos], 6);
		nPos += 6;
	}
	return nPos;
}

// 组5
// F33终端抄表运行参数设置
static int plms_04F33(BYTE *pTran, void *pAppl) {
	int nPos = 0;
	FK_DT04F33_LS *ptr = (FK_DT04F33_LS*) pAppl;
	ptr->nCount = pTran[nPos++];

	int nCount = min(ptr->nCount, NL_FK_PARA_MAX_COM);

	for (int i = 0; i < nCount; ++i) {
		ptr->hPortMode[i].nComPort = pTran[nPos++];

		ptr->hPortMode[i].wCtrlByte = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
		nPos += 2;

		memcpy(ptr->hPortMode[i].day, &pTran[nPos], 4);
		nPos += 4;

		nPos += plms_fmt_appl_19(&pTran[nPos], &ptr->hPortMode[i].dmTime);

		ptr->hPortMode[i].GMMins = pTran[nPos++];

		nPos += plms_fmt_appl_18(&pTran[nPos], &ptr->hPortMode[i].CheckTime);

		int nNum = pTran[nPos];

		nNum = min(nNum, 25);

		ptr->hPortMode[i].primeNum = nNum;

		nPos++;

		for (int j = 0; j < nNum; j++) {
			nPos += plms_fmt_appl_19(&pTran[nPos],
					&ptr->hPortMode[i].GMPeriod[j][0]);
			nPos += plms_fmt_appl_19(&pTran[nPos],
					&ptr->hPortMode[i].GMPeriod[j][1]);
		}
	}
	return nPos;
}
// F34：集中器下行通信模块的参数设置
static int plms_04F34(BYTE *pTran, void *pAppl) {
	int nPos = 0;
	FK_DT04F34_LS *ptr = (FK_DT04F34_LS*) pAppl;
	ptr->nCount = pTran[nPos++];

	int nCount = min(ptr->nCount, NL_FK_PARA_MAX_COM);

	for (int i = 0; i < nCount; i++) {
		ptr->hDevicePara[i].Port = pTran[nPos++];
		ptr->hDevicePara[i].PortDCB = pTran[nPos++];
		memcpy((BYTE *) &ptr->hDevicePara[i].PortBPS, &pTran[nPos], 4);
		nPos += 4;
	}
	return nPos;
}

// F35重点用户
static int plms_04F35(BYTE *pTran, void *pAppl) {
	FK_DT04F35 *ptr = (FK_DT04F35*) pAppl;
	int nPos = 0;

	ptr->ImCount = pTran[nPos++];

	memset((BYTE *) &ptr->ImporIndex, 0x0, NL_FK_PARA_MAX_IMPORTANT * 2);

	int nCount = min(ptr->ImCount, NL_FK_PARA_MAX_IMPORTANT);
	for (int i = 0; i < nCount; i++) {

		WORD index = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
		nPos += 2;
		memcpy((BYTE *) &ptr->ImporIndex[i], &index, sizeof(WORD));
	}
	//memcpy((BYTE *)&ptr->ImporIndex, &pTran[nPos], 2*nCount);
	//nPos += 2*nCount;
	return nPos;
}

// F36网络流量门限
static int plms_04F36(BYTE *pTran, void *pAppl) {
	FK_DT04F36 *ptr = (FK_DT04F36*) pAppl;
	memcpy(&ptr->dwMaxStream, pTran, 4);
	return 4;
}
// F37级联参数
static int plms_04F37(BYTE *pTran, void *pAppl) {
	FK_DT04F37 *ptr = (FK_DT04F37*) pAppl;
	int nPos = 0;

	ptr->bCom = pTran[nPos++];
	ptr->bCtrlbyte = pTran[nPos++];
	ptr->bRecvFrameOverTime = pTran[nPos++];
	ptr->bRecvByteOverTime = pTran[nPos++];
	ptr->bReSendTimes = pTran[nPos++];
	ptr->bAskPeriod = pTran[nPos++];

	int num = pTran[nPos++];
	ptr->bCascadedNum = min(num, CASCADED_MAX_TMN);

	memcpy(&ptr->bCascadedAddr[0], &pTran[nPos], 4 * (ptr->bCascadedNum));

	nPos += 4 * num;
	return nPos;
}
// 1 类数据配置设置
static int plms_04F38(BYTE *pTran, void *pAppl) {
	FK_ITEM_CONFIG *ptr = (FK_ITEM_CONFIG*) pAppl;
	WORD wPos = 0;
	BYTE nUserBigKind = pTran[wPos++];	//本次设置的用户大类号
	if ((nUserBigKind <= GM_PUBLIC_TRANSFORM) && (nUserBigKind != 0)) {
		ptr->nUserBigKind = nUserBigKind;

		// 增加赋值0位置为无效值, 防止写入数据库
		// 若0位置有效 后面会根据报文再次赋值
		ptr->hItemMeta[0].nUserSmallKind = 0xEE;
		// end

		BYTE nTmp = pTran[wPos++];		//本次设置的小类号组数
		ptr->nSuitCount = min(NL_FK_MAX_SMALL_KIND, nTmp);

		BYTE nUserSmallKind;
		for (int i = 0; i < ptr->nSuitCount; i++) {
			nUserSmallKind = pTran[wPos++];		//取用户小类号

			nTmp = pTran[wPos++];		//小类号的组成组数
			BYTE nCount = min(NL_FK_MAX_DT_ITEM, nTmp);

			ptr->hItemMeta[nUserSmallKind].nUserSmallKind = nUserSmallKind;
			ptr->hItemMeta[nUserSmallKind].nDTCount = nCount;
			if (nUserSmallKind < NL_FK_MAX_SMALL_KIND) {
				memcpy(ptr->hItemMeta[nUserSmallKind].DTFlag, &pTran[wPos],
						nCount);
			}
			wPos += nCount;
		}
	}
	return wPos;
}

static int plms_04F39(BYTE *pTran, void *pAppl) {
	return plms_04F38(pTran, pAppl);
}
// 组6
// F41时段工控定值
static int plms_04F41(BYTE *pTran, void *pAppl) {
	FK_DT04F41 *ptr = (FK_DT04F41*) pAppl;
	int nPos = 0;
	ptr->nFlag = pTran[nPos++];
	for (int i = 0; i < 3; ++i) {
		if ((ptr->nFlag & (0x01 << i)) != 0) {
			ptr->scheme[i].nIndex = pTran[nPos++];
			for (int j = 0; j < 8; ++j) {
				if (((ptr->scheme[i].nIndex >> j) & 0x01) != 0) {
					nPos += plms_fmt_appl_02(&pTran[nPos],
							&ptr->scheme[i].param[j]);
				}
			}
		}
	}
	return nPos;
}

// F42厂休工控参数
static int plms_04F42(BYTE *pTran, void *pAppl) {
	FK_DT04F42 *ptr = (FK_DT04F42*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_02(&pTran[nPos], &ptr->value);
	nPos += plms_fmt_appl_19(&pTran[nPos], &ptr->time);
	ptr->delay = pTran[nPos++];
	ptr->day.body = pTran[nPos++];
	return nPos;
}

// F43功率控制的功率滑差时间
static int plms_04F43(BYTE *pTran, void *pAppl) {
	FK_DT04F43 *ptr = (FK_DT04F43*) pAppl;
	int nPos = 0;
	ptr->nMins = pTran[nPos++];
	return nPos;
}

// F44营业报停控参数
static int plms_04F44(BYTE *pTran, void *pAppl) {
	FK_DT04F44 *ptr = (FK_DT04F44*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_20(&pTran[nPos], &ptr->tmStart);
	nPos += plms_fmt_appl_20(&pTran[nPos], &ptr->tmEnd);
	nPos += plms_fmt_appl_02(&pTran[nPos], &ptr->value);
	return nPos;
}

// F45功控轮次设定
static int plms_04F45(BYTE *pTran, void *pAppl) {
	FK_DT04F45 *ptr = (FK_DT04F45*) pAppl;
	int nPos = 0;
	ptr->nFlag = pTran[nPos++];

	if (NL_FK_MAX_YK == 2) {
		ptr->nFlag &= 0x03;
	} else if (NL_FK_MAX_YK == 4) {
		ptr->nFlag &= 0x0f;
	}

	return nPos;
}

// F46月电量控定值
static int plms_04F46(BYTE *pTran, void *pAppl) {
	FK_DT04F46 *ptr = (FK_DT04F46*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_03(&pTran[nPos], &ptr->value);
	return nPos;
}

// F47购电量(费)控参数
static int plms_04F47(BYTE *pTran, void *pAppl) {
	FK_DT04F47 *ptr = (FK_DT04F47*) pAppl;
	int nPos = 0;
	memcpy(&ptr->dwSNO, &pTran[nPos], sizeof(DWORD));
	nPos += sizeof(DWORD);
	ptr->nFlag = pTran[nPos++];
	nPos += plms_fmt_appl_03(&pTran[nPos], &ptr->value);
	nPos += plms_fmt_appl_03(&pTran[nPos], &ptr->lWarn);
	nPos += plms_fmt_appl_03(&pTran[nPos], &ptr->lControl);
	return nPos;
}

// F48电控轮次设定
static int plms_04F48(BYTE *pTran, void *pAppl) {
	FK_DT04F48 *ptr = (FK_DT04F48*) pAppl;
	int nPos = 0;
	ptr->nFlag = pTran[nPos++];

	if (NL_FK_MAX_YK == 2) {
		ptr->nFlag &= 0x03;
	} else if (NL_FK_MAX_YK == 4) {
		ptr->nFlag &= 0x0f;
	}
	return nPos;
}

// 组7
// F49功控告警时间
static int plms_04F49(BYTE *pTran, void *pAppl) {
	FK_DT04F49 *ptr = (FK_DT04F49*) pAppl;
	int nPos = 0;
	ptr->nMins = pTran[nPos++];
	return nPos;
}

// 组8
// F57终端声音告警/禁止设置
static int plms_04F57(BYTE *pTran, void *pAppl) {
	FK_DT04F57 *ptr = (FK_DT04F57*) pAppl;
	int nPos = 0;
	memcpy(ptr->nWarnFlag, &pTran[nPos], 3);
	nPos += 3;
	return nPos;
}

// F58终端自动保电参数
static int plms_04F58(BYTE *pTran, void *pAppl) {
	FK_DT04F58 *ptr = (FK_DT04F58*) pAppl;
	int nPos = 0;
	ptr->nHours = pTran[nPos++];
	return nPos;
}

// F59电能表异常判别阀值设定
static int plms_04F59(BYTE *pTran, void *pAppl) {
	FK_DT04F59 *ptr = (FK_DT04F59*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_22(&pTran[nPos], &ptr->nMTCC);
	nPos += plms_fmt_appl_22(&pTran[nPos], &ptr->nMTFly);
	ptr->nMTStop = pTran[nPos++];
	ptr->nMTChkTime = pTran[nPos++];
	return nPos;
}

// F60谐波限值
static int plms_04F60(BYTE *pTran, void *pAppl) {
	FK_DT04F60 *ptr = (FK_DT04F60*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lvSumVolt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lvOddVolt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lvEvenVolt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv2Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv4Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv6Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv8Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv10Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv12Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv14Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv16Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv18Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv3Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv5Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv7Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv9Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv11Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv13Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv15Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv17Volt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->lv19Volt);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lvSumCurr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv2Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv4Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv6Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv8Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv10Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv12Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv14Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv16Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv18Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv3Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv5Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv7Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv9Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv11Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv13Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv15Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv17Curr);
	nPos += plms_fmt_appl_06(&pTran[nPos], &ptr->lv19Curr);
	return nPos;
}

// F61直流模拟量接入参数
static int plms_04F61(BYTE *pTran, void *pAppl) {
	FK_DT04F61 *ptr = (FK_DT04F61*) pAppl;
	int nPos = 0;
	ptr->nFlag = pTran[nPos++];
	return nPos;
}

// 组9
// F65定时发送1类数据任务设置
static int plms_04F65(BYTE *pTran, void *pAppl) {
	FK_DT04F65 *ptr = (FK_DT04F65*) pAppl;
	int nPos = 0;
	ptr->nCycle = pTran[nPos++];
	nPos += plms_fmt_appl_01(&pTran[nPos], &ptr->tmBasic);
	ptr->nRS = pTran[nPos++];
	ptr->nCount = pTran[nPos++];
	int nCount = min(ptr->nCount, NL_FK_MAX_TASKS);

	for (int i = 0; i < nCount; ++i) {
		memcpy(&ptr->nDT[i], &pTran[nPos], 4);
		nPos += 4;
	}

	if (ptr->nCount > nCount) {
		nPos += (ptr->nCount - nCount) * 4;
	}
	return nPos;
}

// F66定时发送2类数据任务设置
static int plms_04F66(BYTE *pTran, void *pAppl) {
	FK_DT04F66 *ptr = (FK_DT04F66*) pAppl;
	int nPos = 0;
	ptr->nCycle = pTran[nPos++];
	nPos += plms_fmt_appl_01(&pTran[nPos], &ptr->tmBasic);
	ptr->nRS = pTran[nPos++];
	ptr->nCount = pTran[nPos++];
	int nCount = min(ptr->nCount, NL_FK_MAX_TASKS);

	for (int i = 0; i < nCount; ++i) {
		memcpy(&ptr->nDT[i], &pTran[nPos], 4);
		nPos += 4;
	}

	if (ptr->nCount > nCount) {
		nPos += (ptr->nCount - nCount) * 4;
	}
	return nPos;
}

// F67定时发送1类数据任务启动/停止设置
static int plms_04F67(BYTE *pTran, void *pAppl) {
	FK_DT04F67 *ptr = (FK_DT04F67*) pAppl;
	int nPos = 0;
	ptr->nFlag = pTran[nPos++];
	return nPos;
}

// F68定时发送2类数据任务启动/停止设置
static int plms_04F68(BYTE *pTran, void *pAppl) {
	FK_DT04F68 *ptr = (FK_DT04F68*) pAppl;
	int nPos = 0;
	ptr->nFlag = pTran[nPos++];
	return nPos;
}

// 组10
// F73电容器参数
static int plms_04F73(BYTE *pTran, void *pAppl) {
	FK_DT04F73 *ptr = (FK_DT04F73*) pAppl;
	int nPos = 0;
	for (int i = 0; i < 9; ++i) {
		ptr->capacitor[i].nMode = pTran[nPos++];
		nPos += plms_fmt_appl_02(&pTran[nPos], &ptr->capacitor[i].capacity);
	}
	return nPos;
}

// F74电容器投切运行参数
static int plms_04F74(BYTE *pTran, void *pAppl) {
	FK_DT04F74 *ptr = (FK_DT04F74*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->nFunction);
	nPos += plms_fmt_appl_23(&pTran[nPos], &ptr->lvAddPower);
	nPos += plms_fmt_appl_23(&pTran[nPos], &ptr->lvRemovePower);
	ptr->ntmDelay = pTran[nPos++];
	ptr->ntmSpan = pTran[nPos++];
	return nPos;
}

// F75电容器保护参数
static int plms_04F75(BYTE *pTran, void *pAppl) {
	FK_DT04F75 *ptr = (FK_DT04F75*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->ulVolt);
	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->ublVolt);
	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->llVolt);
	nPos += plms_fmt_appl_07(&pTran[nPos], &ptr->lblVolt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->ulJCurr);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->ublJCurr);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->ulJVolt);
	nPos += plms_fmt_appl_05(&pTran[nPos], &ptr->ubljVolt);
	return nPos;
}

// F76电容器投切控制方式
static int plms_04F76(BYTE *pTran, void *pAppl) {
	FK_DT04F76 *ptr = (FK_DT04F76*) pAppl;
	int nPos = 0;
	ptr->nControl = pTran[nPos++];
	return nPos;
}

// 组11
// F81直流模拟量变比
static int plms_04F81(BYTE *pTran, void *pAppl) {
	FK_DT04F81 *ptr = (FK_DT04F81*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_02(&pTran[nPos], &ptr->nDMSStart);
	nPos += plms_fmt_appl_02(&pTran[nPos], &ptr->nDMSEnd);
	return nPos;
}

// F82直流模拟量限值
static int plms_04F82(BYTE *pTran, void *pAppl) {
	FK_DT04F82 *ptr = (FK_DT04F82*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_02(&pTran[nPos], &ptr->ulDMS);
	nPos += plms_fmt_appl_02(&pTran[nPos], &ptr->llDMS);
	return nPos;
}

// F83直流模拟量冻结参数
static int plms_04F83(BYTE *pTran, void *pAppl) {
	FK_DT04F83 *ptr = (FK_DT04F83*) pAppl;
	int nPos = 0;
	ptr->nDensity = pTran[nPos++];
	return nPos;
}
/*
 // F91当前功率下浮控的告警时间和控制时间
 static int plms_04F91( BYTE *pTran, void *pAppl )
 {
 FK_DT04F91 *ptr = (FK_DT04F91*)pAppl;
 int nPos = 0;
 ptr->ntm1Warn = pTran[nPos++];
 ptr->ntm2Warn = pTran[nPos++];
 ptr->ntm3Warn = pTran[nPos++];
 ptr->ntm4Warn = pTran[nPos++];
 ptr->ntmCtrl  = pTran[nPos++];
 return nPos;
 }*/

// F92电能表局编号
static int plms_04F92(BYTE *pTran, void *pAppl) {
	FK_DT04F92 *ptr = (FK_DT04F92*) pAppl;
	int nPos = 0;
	memcpy(&ptr->strOffice[nPos], pTran, 12);
	nPos += 12;
	ptr->nFeeType = pTran[nPos++];
	return nPos;
}
/*
 // F93各次谐波电压含有率上限
 static int plms_04F93( BYTE *pTran, void *pAppl )
 {
 FK_DT04F93 *ptr = (FK_DT04F93*)pAppl;
 int nPos = 0;
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul2Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul4Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul6Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul8Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul10Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul12Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul14Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul16Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul18Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul3Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul5Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul7Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul9Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul11Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul13Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul15Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul17Harm);
 nPos += plms_fmt_appl(5, &pTran[nPos], &ptr->ul19Harm);
 return nPos;
 }*/

// F85终端地址
static int plms_04F85(BYTE *pTran, void *pAppl) {
	FK_DT04F85 *ptr = (FK_DT04F85*) pAppl;
	int nPos = 0;
	ptr->A1 = bcd2word(&pTran[nPos]);
	nPos += 2;
	ptr->A2 = bcd2word(&pTran[nPos]);
	nPos += 2;
	return nPos;
}

static int plms_04F101(BYTE *pTran, void *pAppl) {
	FK_DT04F101 *ptr = (FK_DT04F101*) pAppl;
	int nPos = 0;
	ptr->RouterInfo = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	return nPos;
}

static int plms_04F169(BYTE *pTran, void *pAppl) {
	FK_DT04F169 *ptr = (FK_DT04F169*) pAppl;
	int nPos = 0;
	ptr->nChannel = pTran[nPos];
	nPos++;
	return nPos;
}

static int plms_04F170(BYTE *pTran, void *pAppl) {
	FK_DT04F170 *ptr = (FK_DT04F170*) pAppl;
	int nPos = 0;
	ptr->wPLCPcol = MAKEWORD(pTran[nPos], pTran[nPos + 1]);
	nPos += 2;
	return nPos;
}

static int plms_04F171(BYTE *pTran, void *pAppl) {
	FK_DT04F171 *ptr = (FK_DT04F171*) pAppl;
	int nPos = 0;
	ptr->IsGetDhcpIp = pTran[nPos];
	nPos++;

	ptr->DhcpParam.IPDhcp.nAddr1 = pTran[nPos++];
	ptr->DhcpParam.IPDhcp.nAddr2 = pTran[nPos++];
	ptr->DhcpParam.IPDhcp.nAddr3 = pTran[nPos++];
	ptr->DhcpParam.IPDhcp.nAddr4 = pTran[nPos++];

	ptr->DhcpParam.IPDhcpMask.nAddr1 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpMask.nAddr2 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpMask.nAddr3 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpMask.nAddr4 = pTran[nPos++];

	ptr->DhcpParam.IPDhcpGateway.nAddr1 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpGateway.nAddr2 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpGateway.nAddr3 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpGateway.nAddr4 = pTran[nPos++];

	ptr->DhcpParam.IPDhcpDns.nAddr1 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpDns.nAddr2 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpDns.nAddr3 = pTran[nPos++];
	ptr->DhcpParam.IPDhcpDns.nAddr4 = pTran[nPos++];

	return nPos;
}
/*
 static int plms_04F179(BYTE* pTran, void* pAppl)
 {
 FK_DT04F179* ptr = (FK_DT04F179*)pAppl;
 int nPos = 0;
 ptr->nHour        = pTran[nPos++];
 ptr->nMin         = pTran[nPos++];
 ptr->nWaitMins   = pTran[nPos++];
 ptr->nWaitReport  = pTran[nPos++];
 ptr->nReportTimes = pTran[nPos++];
 return nPos;
 }
 */
static int plms_04F111(BYTE* pTran, void* pAppl) {
	FK_DT04F111* ptr = (FK_DT04F111*) pAppl;
	int nPos = 0;
	ptr->nPort = pTran[nPos++];
	ptr->nStart = pTran[nPos++];
	nPos += plms_fmt_appl_18(&pTran[nPos], &ptr->dmStartTime);
	ptr->nHour = pTran[nPos++];

	return nPos;
}
///////////////////////////////////////////////////////////////////////////////
// AFN = 05
///////////////////////////////////////////////////////////////////////////////
// 组1
// F1遥控跳闸
static int plms_05F01(BYTE *pTran, void *pAppl) {
	FK_DT05F01 *ptr = (FK_DT05F01*) pAppl;
	int nPos = 0;
	ptr->ntmLimit = pTran[nPos] & 0x0F;
	ptr->ntmDelay = (pTran[nPos] >> 4) & 0x0F;
	nPos++;
	return nPos;
}

// 组2
// F9时段功控投入
static int plms_05F09(BYTE *pTran, void *pAppl) {
	FK_DT05F09 *ptr = (FK_DT05F09*) pAppl;
	int nPos = 0;
	ptr->nFlag = pTran[nPos++];
	ptr->nScheme = pTran[nPos++];
	return nPos;
}

// F12当前功率下浮控投入
static int plms_05F12(BYTE *pTran, void *pAppl) {
	FK_DT05F12 *ptr = (FK_DT05F12*) pAppl;
	int nPos = 0;
	ptr->ntmWindow = pTran[nPos++];
	nPos += plms_fmt_appl_04(&pTran[nPos], &ptr->nQuotiety);
	ptr->ntmDelay = pTran[nPos++];
	ptr->nCtrlTime = pTran[nPos++];

	for (BYTE i = 0; i < 4; i++) {
		ptr->nWarnTime[i] = pTran[nPos++];
	}

	return nPos;
}

// 组4
// F25终端保电投入
static int plms_05F25(BYTE *pTran, void *pAppl) {
	FK_DT05F25 *ptr = (FK_DT05F25*) pAppl;
	int nPos = 0;
	ptr->nTime = pTran[nPos++];
	return nPos;
}

// F31对时命令
static int plms_05F31(BYTE *pTran, void *pAppl) {
	FK_DT05F31 *ptr = (FK_DT05F31*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_appl_01(&pTran[nPos], &ptr->time);
	return nPos;
}

// F32中文信息
static int plms_05F32(BYTE *pTran, void *pAppl) {
	FK_DT05F32 *ptr = (FK_DT05F32*) pAppl;
	int nPos = 0;
	ptr->nIndex = pTran[nPos] & 0x0F;
	ptr->nType = (pTran[nPos++] >> 4) & 0x0F;
	ptr->nLength = pTran[nPos++];
	memcpy(ptr->str, &pTran[nPos], ptr->nLength);
	nPos += ptr->nLength;
	ptr->str[ptr->nLength] = '\0';
	return nPos;
}

// 组6
// F41电容器控制投入
static int plms_05F41(BYTE *pTran, void *pAppl) {
	FK_DT05F41 *ptr = (FK_DT05F41*) pAppl;
	int nPos = 0;
	memcpy(ptr->nFlag, &pTran[nPos], 2);
	nPos += 2;
	return nPos;
}

// F42电容器控制切除
static int plms_05F42(BYTE *pTran, void *pAppl) {
	FK_DT05F42 *ptr = (FK_DT05F42*) pAppl;
	int nPos = 0;
	memcpy(ptr->nFlag, &pTran[nPos], 2);
	nPos += 2;
	return nPos;
}

static int plms_05F49(BYTE *pTran, void *pAppl) {
	FK_DT05F49 *ptr = (FK_DT05F49*) pAppl;
	ptr->nPort = pTran[0];
	return 1;
}

static int plms_05F50(BYTE *pTran, void *pAppl) {
	FK_DT05F50 *ptr = (FK_DT05F50*) pAppl;
	ptr->nPort = pTran[0];
	return 1;
}

static int plms_05F51(BYTE *pTran, void *pAppl) {
	FK_DT05F51 *ptr = (FK_DT05F51*) pAppl;
	ptr->nPort = pTran[0];
	return 1;
}

static int plms_05F52(BYTE *pTran, void *pAppl) {
	FK_DT05F52 *ptr = (FK_DT05F52*) pAppl;
	ptr->nPort = pTran[0];
	return 1;
}

static int plms_05F53(BYTE *pTran, void *pAppl) {
	FK_DT05F53 *ptr = (FK_DT05F53*) pAppl;
	ptr->nPort = pTran[0];
	return 1;
}
/*
 static int plms_05F149(BYTE* pTran, void* pAppl)
 {
 FK_DT05F149* ptr = (FK_DT05F149*)pAppl;
 ptr->nStart = pTran[0];
 return 1;
 }*/

static int plms_05F63(BYTE* pTran, void* pAppl) {
	FK_DT05F63* ptr = (FK_DT05F63*) pAppl;
	int nPos = 0;
	ptr->nStart = pTran[nPos++];
	ptr->nPort = pTran[nPos++];

	return nPos;
}

///////////////////////////////////////////////////////////////////////////////
// AFN = 0A(查询参数)
///////////////////////////////////////////////////////////////////////////////
// 组1
// F1终端通信参数设置
static int plms_0AF01(BYTE *pTran, void *pAppl) {
	FK_DT04F01 *ptr = (FK_DT04F01*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nRTStimes;
	pTran[nPos++] = ptr->nStartMins;
	pTran[nPos++] = LOBYTE(ptr->wOverTimes);
	pTran[nPos++] = HIBYTE(ptr->wOverTimes) | (ptr->nRetryCount << 4);
	pTran[nPos++] = ptr->nAckFlag;
	pTran[nPos++] = ptr->nHearBeat;
	return nPos;
}

// F2终端中继转发设置
static int plms_0AF02(BYTE *pTran, void *pAppl) {
	FK_DT04F02 *ptr = (FK_DT04F02*) pAppl;
	int nPos = 0;

	int nNums = min(ptr->nNum, NL_FK_MAX_RETRANS);

	pTran[nPos++] = (nNums) | (ptr->nAgree << 7);
	for (int i = 0; i < nNums; ++i) {
		pTran[nPos++] = LOBYTE(ptr->wAddr[i]);
		pTran[nPos++] = HIBYTE(ptr->wAddr[i]);
	}
	return nPos;
}

// F3主站IP地址和端口
static int plms_0AF03(BYTE *pTran, void *pAppl) {
	FK_DT04F03 *ptr = (FK_DT04F03*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->IPHost.nAddr1;
	pTran[nPos++] = ptr->IPHost.nAddr2;
	pTran[nPos++] = ptr->IPHost.nAddr3;
	pTran[nPos++] = ptr->IPHost.nAddr4;
	pTran[nPos++] = LOBYTE(ptr->IPHost.nPort);
	pTran[nPos++] = HIBYTE(ptr->IPHost.nPort);
	pTran[nPos++] = ptr->IPAux.nAddr1;
	pTran[nPos++] = ptr->IPAux.nAddr2;
	pTran[nPos++] = ptr->IPAux.nAddr3;
	pTran[nPos++] = ptr->IPAux.nAddr4;
	pTran[nPos++] = LOBYTE(ptr->IPAux.nPort);
	pTran[nPos++] = HIBYTE(ptr->IPAux.nPort);
	/*
	 pTran[nPos++] = ptr->IPGetway.nAddr1;
	 pTran[nPos++] = ptr->IPGetway.nAddr2;
	 pTran[nPos++] = ptr->IPGetway.nAddr3;
	 pTran[nPos++] = ptr->IPGetway.nAddr4;
	 pTran[nPos++] = LOBYTE(ptr->IPGetway.nPort);
	 pTran[nPos++] = HIBYTE(ptr->IPGetway.nPort);
	 pTran[nPos++] = ptr->IPAgent.nAddr1;
	 pTran[nPos++] = ptr->IPAgent.nAddr2;
	 pTran[nPos++] = ptr->IPAgent.nAddr3;
	 pTran[nPos++] = ptr->IPAgent.nAddr4;
	 pTran[nPos++] = LOBYTE(ptr->IPAgent.nPort);
	 pTran[nPos++] = HIBYTE(ptr->IPAgent.nPort);
	 */
	memcpy(&pTran[nPos], ptr->APN, 16);
	nPos += 16;
	return nPos;
}

// F4主站电话号码
static int plms_0AF04(BYTE *pTran, void *pAppl) {
	FK_DT04F04 *ptr = (FK_DT04F04*) pAppl;
	int nPos = 0;
	int nInx = 0;
	for (int i = 0; i < 8; ++i) {
		pTran[nPos++] = ptr->hHost.Code[nInx++];
	}
	nInx = 0;
	for (int j = 0; j < 8; ++j) {
		pTran[nPos++] = ptr->hSmsCenter.Code[nInx++];
	}
	return nPos;
}

// F5终端消息认证参数设置
static int plms_0AF05(BYTE *pTran, void *pAppl) {
	FK_DT04F05 *ptr = (FK_DT04F05*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nID;
	pTran[nPos++] = ptr->nParam[0];
	pTran[nPos++] = ptr->nParam[1];
	return nPos;
}

// F6终端组地址设置
static int plms_0AF06(BYTE *pTran, void *pAppl) {
	FK_DT04F06 *ptr = (FK_DT04F06*) pAppl;
	int nPos = 0;
	for (int i = 0; i < 8; ++i) {
		pTran[nPos++] = LOBYTE(ptr->nGroupAdr[i]);
		pTran[nPos++] = HIBYTE(ptr->nGroupAdr[i]);
	}
	return nPos;
}

// F7终端IP地址和端口
static int plms_0AF07(BYTE *pTran, void *pAppl) {
	FK_DT04F07 *ptr = (FK_DT04F07*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->IPDevice.nAddr1;
	pTran[nPos++] = ptr->IPDevice.nAddr2;
	pTran[nPos++] = ptr->IPDevice.nAddr3;
	pTran[nPos++] = ptr->IPDevice.nAddr4;

	pTran[nPos++] = ptr->IPMask.nAddr1;
	pTran[nPos++] = ptr->IPMask.nAddr2;
	pTran[nPos++] = ptr->IPMask.nAddr3;
	pTran[nPos++] = ptr->IPMask.nAddr4;

	pTran[nPos++] = ptr->IPGetway.nAddr1;
	pTran[nPos++] = ptr->IPGetway.nAddr2;
	pTran[nPos++] = ptr->IPGetway.nAddr3;
	pTran[nPos++] = ptr->IPGetway.nAddr4;

	pTran[nPos++] = ptr->AgentType;

	pTran[nPos++] = ptr->IPAgent.nAddr1;
	pTran[nPos++] = ptr->IPAgent.nAddr2;
	pTran[nPos++] = ptr->IPAgent.nAddr3;
	pTran[nPos++] = ptr->IPAgent.nAddr4;

	pTran[nPos++] = LOBYTE(ptr->IPAgent.nPort);
	pTran[nPos++] = HIBYTE(ptr->IPAgent.nPort);

	pTran[nPos++] = ptr->AgentConnectType;

	pTran[nPos++] = ptr->UserNameLength;
	BYTE len = min(ptr->UserNameLength, 20);
	memcpy(&pTran[nPos], ptr->UserName, len);
	nPos += len;

	pTran[nPos++] = ptr->PasswordLength;

	BYTE passlen = min(ptr->PasswordLength, 20);
	memcpy(&pTran[nPos], ptr->Password, passlen);
	nPos += passlen;

	pTran[nPos++] = LOBYTE(ptr->IPDevice.nPort);
	pTran[nPos++] = HIBYTE(ptr->IPDevice.nPort);
	return nPos;
}

// F8终端事件记录配置设置
static int plms_0AF08(BYTE *pTran, void *pAppl) {
	FK_DT04F08 *ptr = (FK_DT04F08*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nGPRS.body;
	pTran[nPos++] = LOBYTE(ptr->wSecs);
	pTran[nPos++] = HIBYTE(ptr->wSecs);
	pTran[nPos++] = ptr->nCount;
	pTran[nPos++] = ptr->nMins;
	memcpy(&pTran[nPos++], ptr->nFlag, 3);
	nPos += 3;
	return nPos;
}

// 组2
// F9终端配置数量表
static int plms_0AF09(BYTE *pTran, void *pAppl) {
	FK_DT04F09 *ptr = (FK_DT04F09*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->nRecordFlag, 8);
	nPos += 8;
	memcpy(&pTran[nPos], ptr->nImportFlag, 8);
	nPos += 8;
	return nPos;
}

// F10终端电能表/交流采样装置配置参数
static int plms_0AF10(BYTE *pTran, void *pAppl) {
	FK_DT04F10_LS *ptr = (FK_DT04F10_LS*) pAppl;

	int nPos = 2;
	int nCnt = 0;
	WORD nRdCnt = 0;

	nCnt = min(ptr->nCount, NL_JZ_MAX_04F10LSBUF_POINT);

	for (int i = 0; i < nCnt; ++i) {
		if ((ptr->hMeter[i].nIndex == 0)
				|| (ptr->hMeter[i].nIndex > NL_JZ_MAX_MEASURED_POINT)
				|| (ptr->hMeter[i].nTN > NL_JZ_MAX_MEASURED_POINT)) {
			continue;
		}
		nRdCnt++;
		pTran[nPos++] = LOBYTE(ptr->hMeter[i].nIndex);
		pTran[nPos++] = HIBYTE(ptr->hMeter[i].nIndex);
		pTran[nPos++] = LOBYTE(ptr->hMeter[i].nTN);
		pTran[nPos++] = HIBYTE(ptr->hMeter[i].nTN);
		pTran[nPos++] = (ptr->hMeter[i].nBaud << 5)
				| (ptr->hMeter[i].nPort & 0x1F);
		pTran[nPos++] = ptr->hMeter[i].nProtocol;
		memcpy(&pTran[nPos], ptr->hMeter[i].addr, 6);
		nPos += 6;
		memcpy(&pTran[nPos], ptr->hMeter[i].pwd, 6);
		nPos += 6;
		pTran[nPos++] = ptr->hMeter[i].nTariffs;
		pTran[nPos++] = (ptr->hMeter[i].nDecimal - 1)
				| ((ptr->hMeter[i].nInter - 4) << 2);
		memcpy(&pTran[nPos], ptr->hMeter[i].collAddr, 6);
		nPos += 6;
		pTran[nPos++] = (ptr->hMeter[i].nUserBigKind << 4)
				| (ptr->hMeter[i].nUserSmallKind);
	}

	pTran[0] = LOBYTE(nRdCnt);
	pTran[1] = HIBYTE(nRdCnt);

	return nPos;
}

// F11终端脉冲配置参数
static int plms_0AF11(BYTE *pTran, void *pAppl) {
	FK_DT04F11_LS *ptr = (FK_DT04F11_LS*) pAppl;

	int nPos = 1;
	int nCnt = 0;
	WORD nRdCnt = 0;

	nCnt = min(ptr->nCount, NL_FK_MAX_MC);

	for (int i = 0; i < nCnt; ++i) {
		if ((ptr->hPulse[i].nPort == 0) || (ptr->hPulse[i].nPort > NL_FK_MAX_MC)
				|| (ptr->hPulse[i].nTN > NL_JZ_MAX_MEASURED_POINT)) {
			continue;
		}
		nRdCnt++;
		pTran[nPos++] = ptr->hPulse[i].nPort;
		pTran[nPos++] = ptr->hPulse[i].nTN;
		pTran[nPos++] = ptr->hPulse[i].nKind;
		pTran[nPos++] = LOBYTE(ptr->hPulse[i].nMK);
		pTran[nPos++] = HIBYTE(ptr->hPulse[i].nMK);
	}

	pTran[0] = nRdCnt;

	return nPos;
}

// F12终端状态量输入
static int plms_0AF12(BYTE *pTran, void *pAppl) {
	FK_DT04F12 *ptr = (FK_DT04F12*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nFlag;
	pTran[nPos++] = ptr->nKind;
//	pTran[nPos++] = ptr->nWarn;
	return nPos;
}

// F13终端电压/电流模拟量配置参数
static int plms_0AF13(BYTE *pTran, void *pAppl) {
	FK_DT04F13_LS *ptr = (FK_DT04F13_LS*) pAppl;

	int nPos = 1;
	int nCnt = 0;
	WORD nRdCnt = 0;

	nCnt = min(ptr->nCount, NL_FK_MAX_ML);

	for (int i = 0; i < nCnt; ++i) {
		if ((ptr->hDMS[i].nPort == 0) || (ptr->hDMS[i].nPort > NL_FK_MAX_MC)
				|| (ptr->hDMS[i].nTN > NL_JZ_MAX_MEASURED_POINT)) {
			continue;
		}
		nRdCnt++;
		pTran[nPos++] = ptr->hDMS[i].nPort;
		pTran[nPos++] = ptr->hDMS[i].nTN;
		pTran[nPos++] = ptr->hDMS[i].nKind;
	}

	pTran[0] = nRdCnt;

	return nPos;
}
// F14终端总加组配置参数
static int plms_0AF14(BYTE *pTran, void *pAppl) {
	FK_DT04F14_LS *ptr = (FK_DT04F14_LS*) pAppl;

	int nPos = 1;
	int nCnt = 0;
	WORD nRdCnt = 0;

	nCnt = min(ptr->nCount, NL_FK_MAX_SUIT);

	for (int i = 0; i < nCnt; ++i) {
		if ((ptr->hSuit[i].nIndex == 0)
				|| (ptr->hSuit[i].nIndex > NL_FK_MAX_SUIT)) {
			continue;
		}
		nRdCnt++;
		pTran[nPos++] = min(ptr->hSuit[i].nIndex, NL_FK_MAX_SUIT);
		int nSize = min(ptr->hSuit[i].nMPs, 63);
		pTran[nPos++] = nSize;
		for (int j = 0; j < nSize; ++j) {
			BYTE body = 0;
			body = (ptr->hSuit[i].flag[j].body & 0xC0)
					| ((ptr->hSuit[i].flag[j].body & 0x3F) - 1);
			pTran[nPos++] = body;
		}
	}
	pTran[0] = nRdCnt;
	return nPos;
}

// F15有功总电能差动越限事件参数配置
static int plms_0AF15(BYTE *pTran, void *pAppl) {
	FK_DT04F15_LS *ptr = (FK_DT04F15_LS*) pAppl;

	int nPos = 1;
	int nCnt = 0;
	WORD nRdCnt = 0;

	nCnt = min(ptr->nCount, NL_FK_MAX_DIFFER);

	for (int i = 0; i < nCnt; ++i) {
		if ((ptr->hDiffer[i].nIndex == 0)
				|| (ptr->hDiffer[i].nIndex > NL_FK_MAX_DIFFER)) {
			continue;
		}
		nRdCnt++;
		pTran[nPos++] = ptr->hDiffer[i].nIndex;
		pTran[nPos++] = ptr->hDiffer[i].nDstSN;
		pTran[nPos++] = ptr->hDiffer[i].nSrcSN;
		pTran[nPos++] = ptr->hDiffer[i].nFlag;
		pTran[nPos++] = ptr->hDiffer[i].nRelativeVal;
		nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->hDiffer[i].dmUtterly);
	}

	pTran[0] = nRdCnt;

	return nPos;
}

// F16虚拟专网用户名、密码
static int plms_0AF16(BYTE *pTran, void *pAppl) {
	FK_DT04F16 *ptr = (FK_DT04F16*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->szVNetUser, 32);
	nPos += 32;
	memcpy(&pTran[nPos], ptr->szVNetPwd, 32);
	nPos += 32;
	return nPos;
}

// 组3
// F17终端保安定值
static int plms_0AF17(BYTE *pTran, void *pAppl) {
	FK_DT04F17 *ptr = (FK_DT04F17*) pAppl;
	return plms_fmt_tran_02(&pTran[0], &ptr->dmValue);
}

// F18终端工控时段
static int plms_0AF18(BYTE *pTran, void *pAppl) {
	FK_DT04F18 *ptr = (FK_DT04F18*) pAppl;
	int nPos = 0;
	for (int i = 0; i < 12; ++i) {
		pTran[nPos++] = ptr->period[i].body;
	}
	return nPos;
}

// F19终端时段控定值浮动系数
static int plms_0AF19(BYTE *pTran, void *pAppl) {
	FK_DT04F19 *ptr = (FK_DT04F19*) pAppl;
	return plms_fmt_tran_04(&pTran[0], &ptr->dmValue);
}

// F20终端月电能量控定值浮动系数
static int plms_0AF20(BYTE *pTran, void *pAppl) {
	FK_DT04F20 *ptr = (FK_DT04F20*) pAppl;
	return plms_fmt_tran_04(&pTran[0], &ptr->dmValue);
}

// F21终端电能量费率时段和费率数
static int plms_0AF21(BYTE *pTran, void *pAppl) {
	FK_DT04F21 *ptr = (FK_DT04F21*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->period, 48);
	nPos += 48;
	pTran[nPos++] = ptr->nTOUs;
	return nPos;
}

// F22终端电能量费率
static int plms_0AF22(BYTE *pTran, void *pAppl) {
	FK_DT04F22 *ptr = (FK_DT04F22*) pAppl;
	int nPos = 0;

	pTran[nPos++] = ptr->nTOUs;

	int num = min(ptr->nTOUs, 14);
	for (int i = 0; i < num; ++i) {
		nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmTOU[i]);
	}
	return nPos;
}

// F23终端催费告警参数
static int plms_0AF23(BYTE *pTran, void *pAppl) {
	FK_DT04F23 *ptr = (FK_DT04F23*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->nWarnFlag, 3);
	nPos += 3;
	return nPos;
}

// 组4
// F25测量但基本参数
static int plms_0AF25(BYTE *pTran, void *pAppl) {
	FK_DT04F25 *ptr = (FK_DT04F25*) pAppl;
	int nPos = 0;
	pTran[nPos++] = LOBYTE(ptr->nPT);
	pTran[nPos++] = HIBYTE(ptr->nPT);
	pTran[nPos++] = LOBYTE(ptr->nCT);
	pTran[nPos++] = HIBYTE(ptr->nCT);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVoltage);
	nPos += plms_fmt_tran_22(&pTran[nPos], &ptr->dmCurrent);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmLoad);
	pTran[nPos++] = ptr->nConnect;
	return nPos;
}

// F26测量点限值参数
static int plms_0AF26(BYTE *pTran, void *pAppl) {
	FK_DT04F26 *ptr = (FK_DT04F26*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->ulVolt);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->llVolt);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->blVolt);

	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->uulVolt);
	pTran[nPos++] = ptr->uulVoltLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->uulVoltLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->lllVolt);
	pTran[nPos++] = ptr->lllVoltLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->lllVoltLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->uulICurr);
	pTran[nPos++] = ptr->uulICurrLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->uulICurrLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->ulICurr);
	pTran[nPos++] = ptr->ulICurrLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->ulICurrLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->ul0Curr);
	pTran[nPos++] = ptr->ul0CurrLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->ul0CurrLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->uulPower);
	pTran[nPos++] = ptr->uulPowerLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->uulPowerLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->ulPower);
	pTran[nPos++] = ptr->ulPowerLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->ulPowerLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->lVoltImb);
	pTran[nPos++] = ptr->lVoltImbLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->lVoltImbLimitValue.OverLimitComeBackCoefficient);

	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->lCurrImb);
	pTran[nPos++] = ptr->lCurrImbLimitValue.OverLimitTime;
	nPos += plms_fmt_tran_05(&pTran[nPos],
			&ptr->lCurrImbLimitValue.OverLimitComeBackCoefficient);

	pTran[nPos++] = ptr->nTimes;
	return nPos;
}

// F27测量点数据冻结参数
static int plms_0AF27(BYTE *pTran, void *pAppl) {
	FK_DT04F27 *ptr = (FK_DT04F27*) pAppl;
	int nPos = 0;
	for (int i = 0; i < 3; i++) {
		nPos += plms_fmt_tran_26(&pTran[nPos], &ptr->PhaseRA[i]);
		nPos += plms_fmt_tran_26(&pTran[nPos], &ptr->PhaseXA[i]);
		nPos += plms_fmt_tran_26(&pTran[nPos], &ptr->PhaseGA[i]);
		nPos += plms_fmt_tran_26(&pTran[nPos], &ptr->PhaseBA[i]);
	}
	return nPos;
}

// F28测量点功率因素分段限值
static int plms_0AF28(BYTE *pTran, void *pAppl) {
	FK_DT04F28 *ptr = (FK_DT04F28*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->limit1);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->limit2);
	return nPos;
}

// F29终端当地电能表显示号
static int plms_0AF29(BYTE *pTran, void *pAppl) {
	FK_DT04F29 *ptr = (FK_DT04F29*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->meternum, 12);

	nPos = 12;
	return nPos;
}

// F30台区集中抄表停抄/投抄设置
static int plms_0AF30(BYTE *pTran, void *pAppl) {
	FK_DT04F30 *ptr = (FK_DT04F30*) pAppl;
	int nPos = 0;

	pTran[nPos++] = ptr->nCollectSwitch;
	return nPos;
}

// F31载波从节点附属节点地址
static int plms_0AF31(BYTE *pTran, void *pAppl) {
	FK_DT04F31 *ptr = (FK_DT04F31*) pAppl;
	int nPos = 0;

	int tol = ptr->AppertainCount;
	if (tol > 20) {
		tol = 20;
	}

	pTran[nPos++] = tol;
	for (int i = 0; i < tol; i++) {
		memcpy(&pTran[nPos], &ptr->AppertainAddress[i][0], 6);
		nPos += 6;
	}
	return nPos;
}

// 组5
// 终端抄表运行参数设置
static int plms_0AF33(BYTE *pTran, void *pAppl) {

	FK_DT04F33_LS *ptr = (FK_DT04F33_LS*) pAppl;

	int nPos = 1;
	int nCnt = 0;
	WORD nRdCnt = 0;
	BYTE nSeq[NL_FK_PARA_MAX_COM];

	nCnt = min(ptr->nCount, NL_FK_PARA_MAX_COM);
	memset(nSeq, 0, sizeof(nSeq));

	for (int i = 0; i < nCnt; ++i) {
		if ((ptr->hPortMode[i].nComPort == 0)
				|| (ptr->hPortMode[i].nComPort > NL_FK_PARA_MAX_COM)) {
			continue;
		}

		nRdCnt++;
		pTran[nPos++] = ptr->hPortMode[i].nComPort;

		memcpy(&pTran[nPos], (BYTE*) &ptr->hPortMode[i].wCtrlByte, 2);
		nPos += 2;

		memcpy(&pTran[nPos], (BYTE*) &ptr->hPortMode[i].day[0], 4);
		nPos += 4;

		nPos += plms_fmt_tran_19(&pTran[nPos], &ptr->hPortMode[i].dmTime);

		pTran[nPos++] = ptr->hPortMode[i].GMMins;

		nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->hPortMode[i].CheckTime);

		int nNum = min(ptr->hPortMode[i].primeNum, 25);

		pTran[nPos++] = nNum;

		for (int j = 0; j < nNum; j++) {
			nPos += plms_fmt_tran_19(&pTran[nPos],
					&ptr->hPortMode[i].GMPeriod[j][0]);
			nPos += plms_fmt_tran_19(&pTran[nPos],
					&ptr->hPortMode[i].GMPeriod[j][1]);
		}

	}

	pTran[0] = nRdCnt;

	return nPos;
}
// 组5
// F34：集中器下行通信模块的参数设置
static int plms_0AF34(BYTE *pTran, void *pAppl) {
	FK_DT04F34_LS *ptr = (FK_DT04F34_LS*) pAppl;

	int nPos = 1;
	int nCnt = 0;
	WORD nRdCnt = 0;

	nCnt = min(ptr->nCount, NL_FK_PARA_MAX_COM);

	for (int i = 0; i < nCnt; i++) {
		if ((ptr->hDevicePara[i].Port == 0)
				|| (ptr->hDevicePara[i].Port > NL_FK_PARA_MAX_COM)) {
			continue;
		}
		nRdCnt++;
		pTran[nPos++] = ptr->hDevicePara[i].Port;
		pTran[nPos++] = ptr->hDevicePara[i].PortDCB;

		memcpy(&pTran[nPos], (BYTE *) &ptr->hDevicePara[i].PortBPS, 4);
		nPos += 4;
	}

	pTran[0] = nRdCnt;
	return nPos;
}

// F35重点用户
static int plms_0AF35(BYTE *pTran, void *pAppl) {
	FK_DT04F35 *ptr = (FK_DT04F35*) pAppl;
	int nPos = 0;

	pTran[nPos++] = ptr->ImCount;

	int num = min(ptr->ImCount, NL_FK_PARA_MAX_IMPORTANT);

	memcpy(&pTran[nPos], (BYTE *) &ptr->ImporIndex, num * 2);

	nPos += (num * 2);
	return nPos;
}
// F36网络流量门限
static int plms_0AF36(BYTE *pTran, void *pAppl) {
	FK_DT04F36 *ptr = (FK_DT04F36*) pAppl;
	memcpy(pTran, &ptr->dwMaxStream, 4);
	return 4;
}

// F37级联参数
static int plms_0AF37(BYTE *pTran, void *pAppl) {
	FK_DT04F37 *ptr = (FK_DT04F37*) pAppl;
	int nPos = 0;

	pTran[nPos++] = ptr->bCom;
	pTran[nPos++] = ptr->bCtrlbyte;
	pTran[nPos++] = ptr->bRecvFrameOverTime;
	pTran[nPos++] = ptr->bRecvByteOverTime;
	pTran[nPos++] = ptr->bReSendTimes;
	pTran[nPos++] = ptr->bAskPeriod;
//	pTran[nPos++] = ptr->bCascadedNum;
//	memcpy( &pTran[nPos], &ptr->bCascadedAddr[0], 4*(ptr->bCascadedNum));

	int num = min(ptr->bCascadedNum, CASCADED_MAX_TMN);

	pTran[nPos++] = num;

	memcpy(&pTran[nPos], &ptr->bCascadedAddr[0], 4 * num);

	nPos += 4 * num;
	return nPos;
}
//
static int plms_0AF38(BYTE *pTran, void *pAppl) {
	FK_ITEM_CONFIG *hItem = (FK_ITEM_CONFIG*) pAppl;
	WORD wPos = 0;

	pTran[wPos++] = hItem->nUserBigKind;		//本次查询的用户大类号
	pTran[wPos++] = hItem->nSuitCount;		//本次查询数量

	BYTE nCount = min(hItem->nSuitCount, NL_FK_MAX_SMALL_KIND);

	for (int i = 0; i < nCount; i++) {
		pTran[wPos++] = hItem->hItemMeta[i].nUserSmallKind;		//查询的用户小类号
		BYTE nDTCount = min(NL_FK_MAX_DT_ITEM, hItem->hItemMeta[i].nDTCount);
		//BYTE nDTCount = NL_FK_MAX_DT_ITEM;
		pTran[wPos++] = nDTCount;
		memcpy(&pTran[wPos], hItem->hItemMeta[i].DTFlag, nDTCount);
		wPos += nDTCount;
	}
	return wPos;
}

static int plms_0AF39(BYTE *pTran, void *pAppl) {
	return plms_0AF38(pTran, pAppl);
}
// 组6
// F41时段工控定值
static int plms_0AF41(BYTE *pTran, void *pAppl) {
	FK_DT04F41 *ptr = (FK_DT04F41*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nFlag;
	for (int i = 0; i < 3; ++i) {
		if ((ptr->nFlag & (0x01 << i)) != 0) {
			pTran[nPos++] = ptr->scheme[i].nIndex;
			for (int j = 0; j < 8; ++j) {
				if (((ptr->scheme[i].nIndex >> j) & 0x01) != 0) {
					nPos += plms_fmt_tran_02(&pTran[nPos],
							&ptr->scheme[i].param[j]);
				}
			}
		}
	}
	return nPos;
}

// F42厂休工控参数
static int plms_0AF42(BYTE *pTran, void *pAppl) {
	FK_DT04F42 *ptr = (FK_DT04F42*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->value);
	nPos += plms_fmt_tran_19(&pTran[nPos], &ptr->time);
	pTran[nPos++] = ptr->delay;
	pTran[nPos++] = ptr->day.body;
	return nPos;
}

// F43功率控制的功率滑差时间
static int plms_0AF43(BYTE *pTran, void *pAppl) {
	FK_DT04F43 *ptr = (FK_DT04F43*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nMins;
	return nPos;
}

// F44营业报停控参数
static int plms_0AF44(BYTE *pTran, void *pAppl) {
	FK_DT04F44 *ptr = (FK_DT04F44*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_20(&pTran[nPos], &ptr->tmStart);
	nPos += plms_fmt_tran_20(&pTran[nPos], &ptr->tmEnd);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->value);
	return nPos;
}

// F45功控轮次设定
static int plms_0AF45(BYTE *pTran, void *pAppl) {
	FK_DT04F45 *ptr = (FK_DT04F45*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nFlag;
	return nPos;
}

// F46月电量控定值
static int plms_0AF46(BYTE *pTran, void *pAppl) {
	FK_DT04F46 *ptr = (FK_DT04F46*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->value);
	return nPos;
}

// F47购电量(费)控参数
static int plms_0AF47(BYTE *pTran, void *pAppl) {
	FK_DT04F47 *ptr = (FK_DT04F47*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->dwSNO, sizeof(DWORD));
	nPos += sizeof(DWORD);
	pTran[nPos++] = ptr->nFlag;
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->value);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->lWarn);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->lControl);
	return nPos;
}

// F48电控轮次设定
static int plms_0AF48(BYTE *pTran, void *pAppl) {
	FK_DT04F48 *ptr = (FK_DT04F48*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nFlag;
	return nPos;
}

// 组7
// F49功控告警时间
static int plms_0AF49(BYTE *pTran, void *pAppl) {
	FK_DT04F49 *ptr = (FK_DT04F49*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nMins;
	return nPos;
}

// 组8
// F57终端声音告警/禁止设置
static int plms_0AF57(BYTE *pTran, void *pAppl) {
	FK_DT04F57 *ptr = (FK_DT04F57*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->nWarnFlag, 3);
	nPos += 3;
	return nPos;
}

// F58终端自动保电参数
static int plms_0AF58(BYTE *pTran, void *pAppl) {
	FK_DT04F58 *ptr = (FK_DT04F58*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nHours;
	return nPos;
}

// F59电能表异常判别阀值设定
static int plms_0AF59(BYTE *pTran, void *pAppl) {
	FK_DT04F59 *ptr = (FK_DT04F59*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_22(&pTran[nPos], &ptr->nMTCC);
	nPos += plms_fmt_tran_22(&pTran[nPos], &ptr->nMTFly);
	pTran[nPos++] = ptr->nMTStop;
	pTran[nPos++] = ptr->nMTChkTime;
	return nPos;
}

// F60谐波限值
static int plms_0AF60(BYTE *pTran, void *pAppl) {
	FK_DT04F60 *ptr = (FK_DT04F60*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->lvSumVolt);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->lvOddVolt);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->lvEvenVolt);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lvSumCurr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv2Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv4Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv6Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv8Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv10Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv12Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv14Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv16Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv18Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv3Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv5Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv7Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv9Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv11Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv13Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv15Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv17Curr);
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->lv19Curr);
	return nPos;
}

// F61直流模拟量接入参数
static int plms_0AF61(BYTE *pTran, void *pAppl) {
	FK_DT04F61 *ptr = (FK_DT04F61*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nFlag;
	return nPos;
}

// 组9
// F65定时发送1类数据任务设置
static int plms_0AF65(BYTE *pTran, void *pAppl) {
	FK_DT04F65 *ptr = (FK_DT04F65*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nCycle;
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->tmBasic);
	pTran[nPos++] = ptr->nRS;

	int nCount = min(ptr->nCount, NL_FK_MAX_TASKS);

	pTran[nPos++] = nCount;

	for (int i = 0; i < nCount; ++i) {
		memcpy(&pTran[nPos], &ptr->nDT[i], 4);
		nPos += 4;
	}
	return nPos;
}

// F66定时发送2类数据任务设置
static int plms_0AF66(BYTE *pTran, void *pAppl) {
	FK_DT04F66 *ptr = (FK_DT04F66*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nCycle;
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->tmBasic);
	pTran[nPos++] = ptr->nRS;

	int nCount = min(ptr->nCount, NL_FK_MAX_TASKS);

	pTran[nPos++] = nCount;
	for (int i = 0; i < nCount; ++i) {
		memcpy(&pTran[nPos], &ptr->nDT[i], 4);
		nPos += 4;
	}
	return nPos;
}

// F67定时发送1类数据任务启动/停止设置
static int plms_0AF67(BYTE *pTran, void *pAppl) {
	FK_DT04F67 *ptr = (FK_DT04F67*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nFlag;
	return nPos;
}

// F68定时发送2类数据任务启动/停止设置
static int plms_0AF68(BYTE *pTran, void *pAppl) {
	FK_DT04F68 *ptr = (FK_DT04F68*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nFlag;
	return nPos;
}

// 组10
// F73电容器参数
static int plms_0AF73(BYTE *pTran, void *pAppl) {
	FK_DT04F73 *ptr = (FK_DT04F73*) pAppl;
	int nPos = 0;
	for (int i = 0; i < 9; ++i) {
		pTran[nPos++] = ptr->capacitor[i].nMode;
		nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->capacitor[i].capacity);
	}
	return nPos;
}

// F74电容器投切运行参数
static int plms_0AF74(BYTE *pTran, void *pAppl) {
	FK_DT04F74 *ptr = (FK_DT04F74*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nFunction);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->lvAddPower);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->lvRemovePower);
	pTran[nPos++] = ptr->ntmDelay;
	pTran[nPos++] = ptr->ntmSpan;
	return nPos;
}

// F75电容器保护参数
static int plms_0AF75(BYTE *pTran, void *pAppl) {
	FK_DT04F75 *ptr = (FK_DT04F75*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->ulVolt);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->ublVolt);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->llVolt);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->lblVolt);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->ulJCurr);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->ublJCurr);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->ulJVolt);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->ubljVolt);
	return nPos;
}

// F76电容器投切控制方式
static int plms_0AF76(BYTE *pTran, void *pAppl) {
	FK_DT04F76 *ptr = (FK_DT04F76*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nControl;
	return nPos;
}

// 组11
// F81直流模拟量变比
static int plms_0AF81(BYTE *pTran, void *pAppl) {
	FK_DT04F81 *ptr = (FK_DT04F81*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->nDMSStart);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->nDMSEnd);
	return nPos;
}

// F82直流模拟量限值
static int plms_0AF82(BYTE *pTran, void *pAppl) {
	FK_DT04F82 *ptr = (FK_DT04F82*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->ulDMS);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->llDMS);
	return nPos;
}

// F83直流模拟量冻结参数
static int plms_0AF83(BYTE *pTran, void *pAppl) {
	FK_DT04F83 *ptr = (FK_DT04F83*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nDensity;
	return nPos;
}
/*
 // F91当前功率下浮控的告警时间和控制时间
 static int plms_0AF91( BYTE *pTran, void *pAppl )
 {
 FK_DT04F91 *ptr = (FK_DT04F91*)pAppl;
 int nPos = 0;
 pTran[nPos++] = ptr->ntm1Warn;
 pTran[nPos++] = ptr->ntm2Warn;
 pTran[nPos++] = ptr->ntm3Warn;
 pTran[nPos++] = ptr->ntm4Warn;
 pTran[nPos++] = ptr->ntmCtrl;
 return nPos;
 }*/

// F92电能表局编号
static int plms_0AF92(BYTE *pTran, void *pAppl) {
	FK_DT04F92 *ptr = (FK_DT04F92*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->strOffice, 12);
	nPos += 12;
	pTran[nPos++] = ptr->nFeeType;
	return nPos;
}
/*
 // F93各次谐波电压含有率上限
 static int plms_0AF93( BYTE *pTran, void *pAppl )
 {
 FK_DT04F93 *ptr = (FK_DT04F93*)pAppl;
 int nPos = 0;
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul2Harm  );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul4Harm  );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul6Harm  );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul8Harm  );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul10Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul12Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul14Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul16Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul18Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul3Harm  );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul5Harm  );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul7Harm  );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul9Harm  );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul11Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul13Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul15Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul17Harm );
 nPos += plms_fmt_tran(5, &pTran[nPos], &ptr->ul19Harm );
 return nPos;
 }*/

// F85终端地址
static int plms_0AF85(BYTE *pTran, void *pAppl) {
	FK_DT04F85 *ptr = (FK_DT04F85*) pAppl;
	int nPos = 0;
	word2bcd(ptr->A1, &pTran[nPos]);
	nPos += 2;
	word2bcd(ptr->A2, &pTran[nPos]);
	nPos += 2;
	return nPos;
}

static int plms_0AF169(BYTE *pTran, void *pAppl) {
	FK_DT04F169 *ptr = (FK_DT04F169*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nChannel;
	return nPos;
}

static int plms_0AF170(BYTE *pTran, void *pAppl) {
	FK_DT04F170 *ptr = (FK_DT04F170*) pAppl;
	int nPos = 0;
	pTran[nPos++] = LOBYTE(ptr->wPLCPcol);
	pTran[nPos++] = HIBYTE(ptr->wPLCPcol);
	return nPos;
}
/*
 static int plms_0AF179(BYTE* pTran, void* pAppl)
 {
 FK_DT04F179* ptr = (FK_DT04F179*)pAppl;
 int nPos = 0;
 pTran[nPos++] = ptr->nHour;
 pTran[nPos++] = ptr->nMin;
 pTran[nPos++] = ptr->nWaitMins;
 pTran[nPos++] = ptr->nWaitReport;
 pTran[nPos++] = ptr->nReportTimes;
 return nPos;
 }*/
static int plms_0AF111(BYTE* pTran, void* pAppl) {
	FK_DT04F111* ptr = (FK_DT04F111*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nPort;
	pTran[nPos++] = ptr->nStart;
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmStartTime);
	pTran[nPos++] = ptr->nHour;

	return nPos;
}

static int plms_0AF112(BYTE* pTran, void* pAppl) {
	REPORT_METER* ptr = (REPORT_METER*) pAppl;
	int nPos = 0;
	pTran[nPos++] = LOBYTE(ptr->nIndex);
	pTran[nPos++] = HIBYTE(ptr->nIndex);
	//pTran[nPos++] = ptr->nProtocol;

	memcpy(&pTran[nPos], ptr->addr, 6);
	nPos += 6;
	memcpy(&pTran[nPos], ptr->collAddr, 6);
	nPos += 6;

	pTran[nPos++] = ptr->nState;
	return nPos;
}
///////////////////////////////////////////////////////////////////////////////
// AFN = 0C
///////////////////////////////////////////////////////////////////////////////
// 组1
// F1终端版本信息
/*static int plms_0CF01( BYTE *pTran, void *pAppl )
 {
 FK_DT0CF01 *ptr = (FK_DT0CF01*)pAppl;
 int nPos = 0;
 strcpy((char*)&pTran[nPos], ptr->szFactory);
 nPos += 4;
 strcpy((char*)&pTran[nPos], ptr->szDevice);
 nPos += 8;
 strcpy((char*)&pTran[nPos], ptr->szVersion);
 nPos += 4;
 nPos += plms_fmt_tran_20(&pTran[nPos], &ptr->dmDate);
 strcpy((char*)&pTran[nPos], ptr->szConfig);
 nPos += 11;
 return 0;
 }
 */
// F2终端日历时钟
static int plms_0CF02(BYTE *pTran, void *pAppl) {
	FK_DT0CF02 *ptr = (FK_DT0CF02*) pAppl;

	time_t hTime = time(nullptr);
	tm* tm_sys = localtime(&hTime);

	ptr->dmTime.nYear = tm_sys->tm_year % 100;
	ptr->dmTime.nMonWeek.mask.nMonth = tm_sys->tm_mon + 1;

	//国网规约规定周日为7，2009.8.6gs
	if (tm_sys->tm_wday == 0) {
		tm_sys->tm_wday = 7;
	}
	//end
	ptr->dmTime.nMonWeek.mask.nWeek = tm_sys->tm_wday;
	ptr->dmTime.nDay = tm_sys->tm_mday;
	ptr->dmTime.nHour = tm_sys->tm_hour;
	ptr->dmTime.nMin = tm_sys->tm_min;
	ptr->dmTime.nSec = tm_sys->tm_sec;
	int nPos = 0;
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->dmTime);
	return nPos;
}

// F3终端参数状态
static int plms_0CF03(BYTE *pTran, void *pAppl) {
	FK_DT0CF03 *ptr = (FK_DT0CF03*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->nFlag, 31);
	nPos += 31;
	return nPos;
}

// F4终端通信状态
static int plms_0CF04(BYTE *pTran, void *pAppl) {
	FK_DT0CF04 *ptr = (FK_DT0CF04*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ((ptr->nReport & 0x03) | ((ptr->nTalk & 0x03) << 2)) & 0x0F;
	return nPos;
}

// F5终端控制设置状态
static int plms_0CF05(BYTE *pTran, void *pAppl) {
	FK_DT0CF05 *ptr = (FK_DT0CF05*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nState;
	pTran[nPos++] = ptr->nSuitFlag;

	for (int i = 0; i < NL_FK_MAX_SUIT; ++i) {
		if (((ptr->nSuitFlag >> i) & 0x01) != 0) {
			pTran[nPos++] = ptr->hSuit[i].nScheme;
			pTran[nPos++] = ptr->hSuit[i].tmFlag;
			pTran[nPos++] = ptr->hSuit[i].stGK;
			pTran[nPos++] = ptr->hSuit[i].stDK;
			pTran[nPos++] = ptr->hSuit[i].stGKLC;
			pTran[nPos++] = ptr->hSuit[i].stDKLC;
		}
	}

	return nPos;
}

// F6终端当前控制状态
static int plms_0CF06(BYTE *pTran, void *pAppl) {
	FK_DT0CF06 *ptr = (FK_DT0CF06*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nstOut;
	pTran[nPos++] = ptr->nstFeeWarn;
	pTran[nPos++] = ptr->nSuitFlag;

	for (int i = 0; i < NL_FK_MAX_SUIT; ++i) {
		if (((ptr->nSuitFlag >> i) & 0x01) != 0) {
			nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->hSuit[i].dmRating);
			nPos += plms_fmt_tran_04(&pTran[nPos], &ptr->hSuit[i].dmQuotiety);
			pTran[nPos++] = ptr->hSuit[i].nstGKOn;
			pTran[nPos++] = ptr->hSuit[i].nstYDKOn;
			pTran[nPos++] = ptr->hSuit[i].nstGDKOn;
			pTran[nPos++] = ptr->hSuit[i].nstGKWarn;
			pTran[nPos++] = ptr->hSuit[i].nstDKWarn;
		}
	}
	return nPos;
}

// F7终端事件计数器当前值
static int plms_0CF07(BYTE *pTran, void *pAppl) {
	FK_DT0CF07 *ptr = (FK_DT0CF07*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nEC1;
	pTran[nPos++] = ptr->nEC2;
	return nPos;
}

// F8终端事件标志状态
static int plms_0CF08(BYTE *pTran, void *pAppl) {
	FK_DT0CF08 *ptr = (FK_DT0CF08*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->nFlag, 8);
	nPos += 8;
	return nPos;
}

// 组2
// F9终端状态量及变位标志
static int plms_0CF09(BYTE *pTran, void *pAppl) {
	FK_DT0CF09 *ptr = (FK_DT0CF09*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nST;
	pTran[nPos++] = ptr->nCD;
	return nPos;
}

// F10查询当日网络流量
static int plms_0CF10(BYTE *pTran, void *pAppl) {
	FK_DT0CF10 *ptr = (FK_DT0CF10*) pAppl;

	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->dwDayStream, 4);
	nPos += 4;
	memcpy(&pTran[nPos], &ptr->dwMonStream, 4);
	nPos += 4;

	return nPos;
}

// F11终端集中抄表状态信息
static int plms_0CF11(BYTE *pTran, void *pAppl) {
	int nPos = 0;
	FK_DT0CF11 *ptr = (FK_DT0CF11*) pAppl;
	ptr->nCount = 1;
	pTran[nPos++] = ptr->nCount;
	for (int i = 0; i < ptr->nCount; i++) {
		pTran[nPos++] = ptr->hMetState[i].port;
		pTran[nPos++] = LOBYTE(ptr->hMetState[i].num);
		pTran[nPos++] = HIBYTE(ptr->hMetState[i].num);
		pTran[nPos++] = ptr->hMetState[i].state;
		pTran[nPos++] = LOBYTE(ptr->hMetState[i].suc_count);
		pTran[nPos++] = HIBYTE(ptr->hMetState[i].suc_count);
		pTran[nPos++] = ptr->hMetState[i].suc_zd_count;
		nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->hMetState[i].dmBeginTime);
		nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->hMetState[i].dmEndTime);
	}
	return nPos;
}

// F12终端网络信号品质
static int plms_0CF12(BYTE *pTran, void *pAppl) {
	int nPos = 0;
	FK_DT0CF12 *ptr = (FK_DT0CF12*) pAppl;

	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dBm);

	return nPos;
}

// F16江苏扩充查询中文信息
static int plms_0CF16(BYTE *pTran, void *pAppl) {
	FK_DT0CF16 *ptr = (FK_DT0CF16*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ((ptr->nType << 4) | (ptr->nIndex & 0x0F));
	pTran[nPos++] = ptr->nLen;
	memcpy(&pTran[nPos], ptr->strInfo, ptr->nLen);
	nPos += ptr->nLen;
	return nPos;
}

// 组3
// F17当前总加有功功率
static int plms_0CF17(BYTE *pTran, void *pAppl) {
	FK_DT0CF17 *ptr = (FK_DT0CF17*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPower);
	return nPos;
}

// F18当前总加无功功率
static int plms_0CF18(BYTE *pTran, void *pAppl) {
	FK_DT0CF18 *ptr = (FK_DT0CF18*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPower);
	return nPos;
}

// F19当日总加有功电能量
static int plms_0CF19(BYTE *pTran, void *pAppl) {
	FK_DT0CF19 *ptr = (FK_DT0CF19*) pAppl;
	int nPos = 0;
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmPower[i]);
	}
	return nPos;
}

// F20当日总加无功电能量
static int plms_0CF20(BYTE *pTran, void *pAppl) {
	return plms_0CF19(pTran, pAppl);
}

// F21当月总加有功电能量
static int plms_0CF21(BYTE *pTran, void *pAppl) {
	return plms_0CF19(pTran, pAppl);
}

// F22当月总加无功电能量
static int plms_0CF22(BYTE *pTran, void *pAppl) {
	return plms_0CF19(pTran, pAppl);
}

// F23终端当前剩余电量
static int plms_0CF23(BYTE *pTran, void *pAppl) {
	FK_DT0CF23 *ptr = (FK_DT0CF23*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmEnergy);
	return nPos;
}

// F24当前功率下浮控控后总加有功功率冻结值
static int plms_0CF24(BYTE *pTran, void *pAppl) {
	FK_DT0CF24 *ptr = (FK_DT0CF24*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPower);
	return nPos;
}

// 组4
// F25当前三相及总有/无功功率、功率因素、三相电压、电流、零序电流
static int plms_0CF25(BYTE *pTran, void *pAppl) {
	FK_DT0CF25 *ptr = (FK_DT0CF25*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPs);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPa);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPb);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPc);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmQs);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmQa);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmQb);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmQc);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFs);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFa);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFb);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFc);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmUa);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmUb);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmUc);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIa);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIb);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIc);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmI0);
	//
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmAPs);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmAPa);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmAPb);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmAPc);
	return nPos;
}

// F26U、V、W三相断相统计数据及最近一次断相记录
static int plms_0CF26(BYTE *pTran, void *pAppl) {
	FK_DT0CF26 *ptr = (FK_DT0CF26*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->dmDsRefs);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->dmDaRefs);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->dmDbRefs);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->dmDcRefs);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmDsTimes);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmDaTimes);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmDbTimes);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmDcTimes);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmDsBeginTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmDaBeginTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmDbBeginTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmDcBeginTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmDsEndTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmDaEndTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmDbEndTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmDcEndTime);
	return nPos;
}

// F27电能表日历时钟及电能表状态信息
static int plms_0CF27(BYTE *pTran, void *pAppl) {
	FK_DT0CF27 *ptr = (FK_DT0CF27*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->dmMeterTime);

	nPos += plms_fmt_tran_27(&pTran[nPos], &ptr->dmBatteryRun);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmProgramCount);
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->dmLastProgram);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmClrAllCount);
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->dmLastClrAll);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmClrDemandCount);
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->dmLastClrDemand);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmClrEventCount);
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->dmLastClrEvent);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmCheckTimeCount);
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->dmLastCheckTime);
	return nPos;
}

// F28电能表状态字1-7
static int plms_0CF28(BYTE *pTran, void *pAppl) {
	FK_DT0CF28 *ptr = (FK_DT0CF28*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	memcpy(&pTran[nPos], &ptr->dmXchge[0], 14);
	nPos += 14;

	memcpy(&pTran[nPos], &ptr->dmState[0], 14);
	nPos += 14;

	return nPos;
}

static int plms_0CF31(BYTE *pTran, void *pAppl) {
	FK_DT0CF31 *ptr = (FK_DT0CF31*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmZPEnergy_A);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmFPEnergy_A);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ1Energy_A);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ2Energy_A);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmZPEnergy_B);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmFPEnergy_B);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ1Energy_B);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ2Energy_B);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmZPEnergy_C);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmFPEnergy_C);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ1Energy_C);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ2Energy_C);

	return nPos;
}

static int plms_0CF32(BYTE *pTran, void *pAppl) {
	FK_DT0CF32 *ptr = (FK_DT0CF32*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmZPEnergy_A);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmFPEnergy_A);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ1Energy_A);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ2Energy_A);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmZPEnergy_B);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmFPEnergy_B);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ1Energy_B);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ2Energy_B);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmZPEnergy_C);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmFPEnergy_C);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ1Energy_C);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQ2Energy_C);

	return nPos;
}

// 组5
// F33当前正向有/无功电能示值、一/四象限无功电能示值(总、费率1~M)
static int plms_0CF33(BYTE *pTran, void *pAppl) {
	FK_DT0CF33 *ptr = (FK_DT0CF33*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm1QEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm4QEnergy[i]);
	}
	return nPos;
}

// F34当前反向有/无功电能示值、二/三象限无功电能示值(总、费率1~M)
static int plms_0CF34(BYTE *pTran, void *pAppl) {
	return plms_0CF33(pTran, pAppl);
}

// F35当月正向有/无功最大需量及发生时间
static int plms_0CF35(BYTE *pTran, void *pAppl) {
	FK_DT0CF35 *ptr = (FK_DT0CF35*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxP[i].dmValue);
	}
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxP[i].dmTime);
	}
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxQ[i].dmValue);
	}
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxQ[i].dmTime);
	}

	return nPos;
}

// F36当月反向有/无功最大需量及发生时间
static int plms_0CF36(BYTE *pTran, void *pAppl) {
	return plms_0CF35(pTran, pAppl);
}

// F37上月正向有/无功电能示值、一/四象限无功电能示值(总、费率1~M)
static int plms_0CF37(BYTE *pTran, void *pAppl) {
	return plms_0CF33(pTran, pAppl);
}

// F38上月反向有/无功电能示值、二/三象限无功电能示值(总、费率1~M)
static int plms_0CF38(BYTE *pTran, void *pAppl) {
	return plms_0CF33(pTran, pAppl);
}

// F39上月正向有/无功最大需量及发生时间
static int plms_0CF39(BYTE *pTran, void *pAppl) {
	return plms_0CF35(pTran, pAppl);
}

// F40上月反向有/无功最大需量及发生时间
static int plms_0CF40(BYTE *pTran, void *pAppl) {
	return plms_0CF35(pTran, pAppl);
}

// 组6
// F41当日正向有功电能量
static int plms_0CF41(BYTE *pTran, void *pAppl) {
	FK_DT0CF41 *ptr = (FK_DT0CF41*) pAppl;
	int nPos = 0;
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_13(&pTran[nPos], &ptr->dmEnergy[i]);
	}
	return nPos;
}

// F42当日正向无功电能量
static int plms_0CF42(BYTE *pTran, void *pAppl) {
	return plms_0CF41(pTran, pAppl);
}

// F43当日反向有功电能量
static int plms_0CF43(BYTE *pTran, void *pAppl) {
	return plms_0CF41(pTran, pAppl);
}

// F44当日反向无功电能量
static int plms_0CF44(BYTE *pTran, void *pAppl) {
	return plms_0CF41(pTran, pAppl);
}

// F45当月正向有功电能量
static int plms_0CF45(BYTE *pTran, void *pAppl) {
	return plms_0CF41(pTran, pAppl);
}

// F46当月正向无功电能量
static int plms_0CF46(BYTE *pTran, void *pAppl) {
	return plms_0CF41(pTran, pAppl);
}

// F47当月反向有功电能量
static int plms_0CF47(BYTE *pTran, void *pAppl) {
	return plms_0CF41(pTran, pAppl);
}

// F48当月反向无功电能量
static int plms_0CF48(BYTE *pTran, void *pAppl) {
	return plms_0CF41(pTran, pAppl);
}

// 组7
// F49当前电压、电流相位角
static int plms_0CF49(BYTE *pTran, void *pAppl) {
	FK_DT0CF49 *ptr = (FK_DT0CF49*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleUa);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleUb);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleUc);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleIa);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleIb);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleIc);
	return nPos;
}

// 组8
// F57当前U、V、W三相电压、电流2~N次谐波有效值
static int plms_0CF57(BYTE *pTran, void *pAppl) {
	FK_DT0CF57 *ptr = (FK_DT0CF57*) pAppl;
	int nPos = 0;

	ptr->nHarmonics = 19;
	pTran[nPos++] = ptr->nHarmonics;

	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmHUa[i - 2]);
	}
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmHUb[i - 2]);
	}
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmHUc[i - 2]);
	}
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->dmHIa[i - 2]);
	}
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->dmHIb[i - 2]);
	}
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->dmHIc[i - 2]);
	}
	return nPos;
}

// F58当前U、V、W三相电压、电流2~N次谐波含有率
static int plms_0CF58(BYTE *pTran, void *pAppl) {
	FK_DT0CF58 *ptr = (FK_DT0CF58*) pAppl;
	int nPos = 0;

	ptr->nHarmonics = 19;
	pTran[nPos++] = ptr->nHarmonics;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nUaSHRatio);
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nUaNHRatio[i - 2]);
	}
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nUbSHRatio);
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nUbNHRatio[i - 2]);
	}
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nUcSHRatio);
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nUcNHRatio[i - 2]);
	}
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nIaSHRatio);
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nIaNHRatio[i - 2]);
	}
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nIbSHRatio);
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nIbNHRatio[i - 2]);
	}
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nIcSHRatio);
	for (int i = 2; i <= ptr->nHarmonics; ++i) {
		nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->nIcNHRatio[i - 2]);
	}
	return nPos;
}

// 组9
// F65当前电容器投切状态
static int plms_0CF65(BYTE *pTran, void *pAppl) {
	FK_DT0CF65 *ptr = (FK_DT0CF65*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nType;
	memcpy(&pTran[nPos], &ptr->nFlag, 2);
	nPos += 2;
	return nPos;
}

// F66当前电容器累计补偿投入时间和次数
static int plms_0CF66(BYTE *pTran, void *pAppl) {
	FK_DT0CF66 *ptr = (FK_DT0CF66*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->dwTimes, 36);
	nPos += 36;
	memcpy(&pTran[nPos], ptr->dwCount, 36);
	nPos += 36;
	return nPos;
}

// F67当日、当月电容器累计补偿的无功电能量
static int plms_0CF67(BYTE *pTran, void *pAppl) {
	FK_DT0CF67 *ptr = (FK_DT0CF67*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_13(&pTran[nPos], &ptr->dmDayQEnergy);
	nPos += plms_fmt_tran_13(&pTran[nPos], &ptr->dmMonQEnergy);
	return nPos;
}

// 组10
// F73直流模拟量实时数据
static int plms_0CF73(BYTE *pTran, void *pAppl) {
	FK_DT0CF73 *ptr = (FK_DT0CF73*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmDMS);
	return nPos;
}

// 组11
// F81小时冻结总加有功功率
static int plms_0CF81(BYTE *pTran, void *pAppl) {
	FK_DT0CF81 *ptr = (FK_DT0CF81*) pAppl;
	int nPos = 0;

	//if( ptr->hTd.nDensity > 3 )
	{
		ptr->hTd.nDensity = 1;
	}

	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPower[i]);
		}
	}

	return nPos;
}

// F82小时冻结总加无功功率
static int plms_0CF82(BYTE *pTran, void *pAppl) {
	return plms_0CF81(pTran, pAppl);
}

// F83小时冻结总加有功总电能量
static int plms_0CF83(BYTE *pTran, void *pAppl) {
	FK_DT0CF83 *ptr = (FK_DT0CF83*) pAppl;
	int nPos = 0;

	//if( ptr->hTd.nDensity > 3 )
	{
		ptr->hTd.nDensity = 1;
	}

	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmEnergy[i]);
		}
	}
	return nPos;
}

// F84小时冻结总加无功总电能量
static int plms_0CF84(BYTE *pTran, void *pAppl) {
	return plms_0CF83(pTran, pAppl);
}

// 组12
// F89小时冻结有功功率
static int plms_0CF89(BYTE *pTran, void *pAppl) {
	FK_DT0CF89 *ptr = (FK_DT0CF89*) pAppl;
	int nPos = 0;

	//if( ptr->hTd.nDensity > 3 )
	{
		ptr->hTd.nDensity = 1;
	}

	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPower[i]);
		}
	}
	return nPos;
}

// F90小时冻结U相有功功率
static int plms_0CF90(BYTE *pTran, void *pAppl) {
	return plms_0CF89(pTran, pAppl);
}

// F91小时冻结V相有功功率
static int plms_0CF91(BYTE *pTran, void *pAppl) {
	return plms_0CF89(pTran, pAppl);
}

// F92小时冻结W相有功功率
static int plms_0CF92(BYTE *pTran, void *pAppl) {
	return plms_0CF89(pTran, pAppl);
}

// F93小时冻结无功功率
static int plms_0CF93(BYTE *pTran, void *pAppl) {
	return plms_0CF89(pTran, pAppl);
}

// F94小时冻结U相无功功率
static int plms_0CF94(BYTE *pTran, void *pAppl) {
	return plms_0CF89(pTran, pAppl);
}

// F95小时冻结V相无功功率
static int plms_0CF95(BYTE *pTran, void *pAppl) {
	return plms_0CF89(pTran, pAppl);
}

// F96小时冻结W相无功功率
static int plms_0CF96(BYTE *pTran, void *pAppl) {
	return plms_0CF89(pTran, pAppl);
}

// 组13
// F97小时冻结U相电压
static int plms_0CF97(BYTE *pTran, void *pAppl) {
	FK_DT0CF97 *ptr = (FK_DT0CF97*) pAppl;
	int nPos = 0;

	//if(( ptr->hTd.nDensity > 3 )||(ptr->hTd.nDensity < 1))
	{
		ptr->hTd.nDensity = 1;
	}

	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVoltage[i]);
		}
	}
	return nPos;
}

// F98小时冻结V相电压
static int plms_0CF98(BYTE *pTran, void *pAppl) {
	return plms_0CF97(pTran, pAppl);
}

// F99小时冻结W相电压
static int plms_0CF99(BYTE *pTran, void *pAppl) {
	return plms_0CF97(pTran, pAppl);
}

// F100小时冻结U相电流
static int plms_0CF100(BYTE *pTran, void *pAppl) {
	FK_DT0CF100 *ptr = (FK_DT0CF100*) pAppl;
	int nPos = 0;

	//if( ptr->hTd.nDensity > 3 )
	{
		ptr->hTd.nDensity = 1;
	}
//	TRACE0("ptr->hTd.nDensity = %d\r\n",ptr->hTd.nDensity);
	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmCurrent[i]);
		}
	}
	return nPos;
}

// F101小时冻结V相电流
static int plms_0CF101(BYTE *pTran, void *pAppl) {
	return plms_0CF100(pTran, pAppl);
}

// F102小时冻结W相电流
static int plms_0CF102(BYTE *pTran, void *pAppl) {
	return plms_0CF100(pTran, pAppl);
}

// F103小时冻结0相电流
static int plms_0CF103(BYTE *pTran, void *pAppl) {
	return plms_0CF100(pTran, pAppl);
}

// 组14
// F105小时冻结正向有功总电能量
static int plms_0CF105(BYTE *pTran, void *pAppl) {
	FK_DT0CF105 *ptr = (FK_DT0CF105*) pAppl;
	int nPos = 0;

	if (ptr->hTd.nDensity > 3) {
		ptr->hTd.nDensity = 0;
	}

	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_13(&pTran[nPos], &ptr->dmEnergy[i]);
		}
	}
	return nPos;
}

// F106小时冻结正向无功总电能量
static int plms_0CF106(BYTE *pTran, void *pAppl) {
	return plms_0CF105(pTran, pAppl);
}

// F107小时冻结反向有功总电能量
static int plms_0CF107(BYTE *pTran, void *pAppl) {
	return plms_0CF105(pTran, pAppl);
}

// F108小时冻结反向无功总电能量
static int plms_0CF108(BYTE *pTran, void *pAppl) {
	return plms_0CF105(pTran, pAppl);
}

// F109小时冻结正向有功总电能示值
static int plms_0CF109(BYTE *pTran, void *pAppl) {
	FK_DT0CF109 *ptr = (FK_DT0CF109*) pAppl;
	int nPos = 0;

	if (ptr->hTd.nDensity > 3) {
		ptr->hTd.nDensity = 0;
	}

	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmEnergy[i]);
		}
	}
	return nPos;
}

// F110小时冻结正向无功总电能示值
static int plms_0CF110(BYTE *pTran, void *pAppl) {
	return plms_0CF109(pTran, pAppl);
}

// F111小时冻结反向有功总电能示值
static int plms_0CF111(BYTE *pTran, void *pAppl) {
	return plms_0CF109(pTran, pAppl);
}

// F112小时冻结反向无功总电能示值
static int plms_0CF112(BYTE *pTran, void *pAppl) {
	return plms_0CF109(pTran, pAppl);
}

// 组15
// F113小时冻结总功率因素
static int plms_0CF113(BYTE *pTran, void *pAppl) {
	FK_DT0CF113 *ptr = (FK_DT0CF113*) pAppl;
	int nPos = 0;

	if (ptr->hTd.nDensity > 3) {
		ptr->hTd.nDensity = 0;
	}

	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFunctor[i]);
		}
	}
	return nPos;
}

// F114小时冻结U相功率因素
static int plms_0CF114(BYTE *pTran, void *pAppl) {
	return plms_0CF113(pTran, pAppl);
}

// F115小时冻结V相功率因素
static int plms_0CF115(BYTE *pTran, void *pAppl) {
	return plms_0CF113(pTran, pAppl);
}

// F116小时冻结W相功率因素
static int plms_0CF116(BYTE *pTran, void *pAppl) {
	return plms_0CF113(pTran, pAppl);
}

// 组16
// F121小时冻结直流模拟量
static int plms_0CF121(BYTE *pTran, void *pAppl) {
	FK_DT0CF121 *ptr = (FK_DT0CF121*) pAppl;
	int nPos = 0;

	if (ptr->hTd.nDensity > 3) {
		ptr->hTd.nDensity = 0;
	}

	nPos += plms_tdh_tran(&pTran[nPos], &ptr->hTd);

	if (ptr->hTd.nDensity > 0) {
		int nStep = 0x01 << (ptr->hTd.nDensity - 1);
		int nStar = nStep - 1;
		for (int i = nStar; i < 4; i += nStep) {
			nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmDMS[i]);
		}
	}
	return nPos;
}

// Add 2009-11-10 for 698
// F129当前正向有功电能示值(总、费率1~M)
static int plms_0CF129(BYTE *pTran, void *pAppl) {
	FK_DT0CF33 *ptr = (FK_DT0CF33*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy[i]);
	}
	return nPos;
}

// F130当前正向无功电能示值(总、费率1~M)
static int plms_0CF130(BYTE *pTran, void *pAppl) {
	FK_DT0CF33 *ptr = (FK_DT0CF33*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergy[i]);
	}
	return nPos;
}
// F131当前反向有功电能示值(总、费率1~M)
static int plms_0CF131(BYTE *pTran, void *pAppl) {
	return plms_0CF129(pTran, pAppl);
}
// F132当前反向无功电能示值(总、费率1~M)
static int plms_0CF132(BYTE *pTran, void *pAppl) {
	return plms_0CF130(pTran, pAppl);
}

// F133当前一象限无功电能示值(总、费率1~M)
static int plms_0CF133(BYTE *pTran, void *pAppl) {
	FK_DT0CF33 *ptr = (FK_DT0CF33*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm1QEnergy[i]);
	}
	return nPos;
}

// F134当前二象限无功电能示值(总、费率1~M)
static int plms_0CF134(BYTE *pTran, void *pAppl) {
	return plms_0CF133(pTran, pAppl);
}

// F135当前三象限无功电能示值(总、费率1~M)
static int plms_0CF135(BYTE *pTran, void *pAppl) {
	FK_DT0CF33 *ptr = (FK_DT0CF33*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm4QEnergy[i]);
	}
	return nPos;
}

// F136当前四象限无功电能示值(总、费率1~M)
static int plms_0CF136(BYTE *pTran, void *pAppl) {
	return plms_0CF135(pTran, pAppl);
}

// F137上月正向有功电能示值(总、费率1~M)
static int plms_0CF137(BYTE *pTran, void *pAppl) {
	return plms_0CF129(pTran, pAppl);
}

// F138上月正向无功电能示值(总、费率1~M)
static int plms_0CF138(BYTE *pTran, void *pAppl) {
	return plms_0CF130(pTran, pAppl);
}
// F139上月反向有功电能示值(总、费率1~M)
static int plms_0CF139(BYTE *pTran, void *pAppl) {
	return plms_0CF129(pTran, pAppl);
}
// F140上月反向无功电能示值(总、费率1~M)
static int plms_0CF140(BYTE *pTran, void *pAppl) {
	return plms_0CF130(pTran, pAppl);
}

// F141上月一象限无功电能示值(总、费率1~M)
static int plms_0CF141(BYTE *pTran, void *pAppl) {
	return plms_0CF133(pTran, pAppl);
}

// F142上月二象限无功电能示值(总、费率1~M)
static int plms_0CF142(BYTE *pTran, void *pAppl) {
	return plms_0CF133(pTran, pAppl);
}

// F143上月三象限无功电能示值(总、费率1~M)
static int plms_0CF143(BYTE *pTran, void *pAppl) {
	return plms_0CF135(pTran, pAppl);
}

// F144上月四象限无功电能示值(总、费率1~M)
static int plms_0CF144(BYTE *pTran, void *pAppl) {
	return plms_0CF135(pTran, pAppl);
}

// F145当月正向有功最大需量及发生时间
static int plms_0CF145(BYTE *pTran, void *pAppl) {
	FK_DT0CF35 *ptr = (FK_DT0CF35*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxP[i].dmValue);
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxP[i].dmTime);
	}

	return nPos;
}
// F146当月正向无功最大需量及发生时间
static int plms_0CF146(BYTE *pTran, void *pAppl) {
	FK_DT0CF35 *ptr = (FK_DT0CF35*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxQ[i].dmValue);
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxQ[i].dmTime);
	}

	return nPos;
}

// F147当月反向有功最大需量及发生时间
static int plms_0CF147(BYTE *pTran, void *pAppl) {
	return plms_0CF145(pTran, pAppl);
}

// F148当月反向无功最大需量及发生时间
static int plms_0CF148(BYTE *pTran, void *pAppl) {
	return plms_0CF146(pTran, pAppl);
}

// F149上月正向有功最大需量及发生时间
static int plms_0CF149(BYTE *pTran, void *pAppl) {
	return plms_0CF145(pTran, pAppl);
}

// F150上月正向无功最大需量及发生时间
static int plms_0CF150(BYTE *pTran, void *pAppl) {
	return plms_0CF146(pTran, pAppl);
}

// F151上月反向有功最大需量及发生时间
static int plms_0CF151(BYTE *pTran, void *pAppl) {
	return plms_0CF145(pTran, pAppl);
}

// F152上月反向无功最大需量及发生时间
static int plms_0CF152(BYTE *pTran, void *pAppl) {
	return plms_0CF146(pTran, pAppl);
}

// F153 第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0CF153(BYTE *pTran, void *pAppl) {
	FK_DT0CF153 *ptr = (FK_DT0CF153*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy[i].llVal);
	}

	return nPos;
}

// F154 第二时区冻结正向有功电能示值（总、费率1~M）
static int plms_0CF154(BYTE *pTran, void *pAppl) {
	return plms_0CF153(pTran, pAppl);
}

// F155 第三时区冻结正向有功电能示值（总、费率1~M）
static int plms_0CF155(BYTE *pTran, void *pAppl) {
	return plms_0CF153(pTran, pAppl);
}

// F156 第四时区冻结正向有功电能示值（总、费率1~M）
static int plms_0CF156(BYTE *pTran, void *pAppl) {
	return plms_0CF153(pTran, pAppl);
}

// F157 第五时区冻结正向有功电能示值（总、费率1~M）
static int plms_0CF157(BYTE *pTran, void *pAppl) {
	return plms_0CF153(pTran, pAppl);
}

// F158 第六时区冻结正向有功电能示值（总、费率1~M）
static int plms_0CF158(BYTE *pTran, void *pAppl) {
	return plms_0CF153(pTran, pAppl);
}

// F159 第七时区冻结正向有功电能示值（总、费率1~M）
static int plms_0CF159(BYTE *pTran, void *pAppl) {
	return plms_0CF153(pTran, pAppl);
}

// F160 第八时区冻结正向有功电能示值（总、费率1~M）
static int plms_0CF160(BYTE *pTran, void *pAppl) {
	return plms_0CF153(pTran, pAppl);
}

// F161 电能表远程控制通断电状态及记录
static int plms_0CF161(BYTE *pTran, void *pAppl) {
	FK_DT0CF161 *ptr = (FK_DT0CF161*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->mStause;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->mStartTime);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->mStopTime);
	return nPos;
}

// F165 电能表开关操作次数及时间
static int plms_0CF165(BYTE *pTran, void *pAppl) {
	FK_DT0CF165 *ptr = (FK_DT0CF165*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mProgramKeyTimes);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->ProgramKeyTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mTrailCoverTimes);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->TrailCoverTime);
	return nPos;
}
// F166 电能表参数修改次数及时间
static int plms_0CF166(BYTE *pTran, void *pAppl) {
	FK_DT0CF166 *ptr = (FK_DT0CF166*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mCheckClockTimes);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->CheckClockTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mPeriodChangeTimes);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->PeriodChangeTime);
	return nPos;
}
// F167 电能表购、用电信息
static int plms_0CF167(BYTE *pTran, void *pAppl) {
	FK_DT0CF167 *ptr = (FK_DT0CF167*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mBuyTimes);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->RemainRMB);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->TotalBuyRMB);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->RemainEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->OverEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->TotalBuyEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->OverdraftStopEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->AlarmEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->FailureEnergy);
	return nPos;
}
// F168 电能表结算信息
static int plms_0CF168(BYTE *pTran, void *pAppl) {
	FK_DT0CF168 *ptr = (FK_DT0CF168*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->StrikeEnergy[i].llVal);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->UnStrikeEnergy[i].llVal);
	}
	return nPos;
}

// F169集中抄表中继路由信息
static int plms_0CF169(BYTE *pTran, void *pAppl) {
	FK_DT0CF169 *ptr = (FK_DT0CF169*) pAppl;
	int nPos = 0;

	pTran[nPos++] = ptr->nPort;

	BYTE size = ptr->nRouter;

	if (size >= 0xEE) // EE EF FF
			{
		pTran[nPos++] = 0xEE;
	} else {
		if (size > NL_FK_MAX_ROUTE)
			size = NL_FK_MAX_ROUTE;
		BYTE size2 = 0;

		pTran[nPos++] = size;

		for (int i = 0; i < size; i++) {
			size2 = ptr->Router[i].nRelaySeries;
			if (size2 > NL_FK_MAX_ROUTE_SERIES)
				size2 = 0;
			pTran[nPos++] = size2;
			for (int j = 0; j < size2; j++) {
				nPos += plms_fmt_tran_12(&pTran[nPos],
						&ptr->Router[i].RelayAddr[j]);
			}
		}
	}

	return nPos;
}

// F170集中抄表电表抄读信息
static int plms_0CF170(BYTE *pTran, void *pAppl) {
	FK_DT0CF170 *ptr = (FK_DT0CF170*) pAppl;
	int nPos = 0;

	pTran[nPos++] = ptr->nPort;
	pTran[nPos++] = ptr->nRelaySeries;
	pTran[nPos++] = ptr->commPhase;
	pTran[nPos++] = ptr->commQuanlity;
	pTran[nPos++] = ptr->commFlag;

	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->commSuccTime);
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->commFailTime);

	pTran[nPos++] = ptr->commFailTimes;

	return nPos;
}

static int plms_0CF180(BYTE* pTran, void* pAppl) {
	FK_DT0CF180* ptr = (FK_DT0CF180*) pAppl;
	int nPos = 0;

	pTran[nPos++] = LOBYTE(ptr->nMeter.nIndex);
	pTran[nPos++] = HIBYTE(ptr->nMeter.nIndex);

	pTran[nPos++] = LOBYTE(ptr->nMeter.nTN);
	pTran[nPos++] = HIBYTE(ptr->nMeter.nTN);

	pTran[nPos++] = ptr->nMeter.nProtocol;

	memcpy(&pTran[nPos], ptr->nMeter.addr, 6);
	nPos += 6;
	memcpy(&pTran[nPos], ptr->nMeter.collAddr, 6);
	nPos += 6;

	return nPos;
}
/*static int plms_0CF182(BYTE* pTran, void* pAppl)
 {
 FK_DT0CF182* ptr = (FK_DT0CF182*)pAppl;
 int nPos = 0;

 pTran[nPos++] = LOBYTE(ptr->wAllCount);
 pTran[nPos++] = HIBYTE(ptr->wAllCount);
 pTran[nPos++] = LOBYTE(ptr->wInNetCount);
 pTran[nPos++] = HIBYTE(ptr->wInNetCount);
 pTran[nPos++] = LOBYTE(ptr->wOutNetCount);
 pTran[nPos++] = HIBYTE(ptr->wOutNetCount);

 if(ptr->wOutNetCount > 20)
 ptr->wOutNetCount = 20;


 for(int i = 0;i < ptr->wOutNetCount;i++ )
 {
 memcpy(&pTran[nPos], ptr->m_sOutNetInfo[i], 6);

 nPos += 6;
 }

 return nPos;
 }
 */
static int plms_0CF186(BYTE* pTran, void* pAppl) {
	FK_DT0CF186* ptr = (FK_DT0CF186*) pAppl;
	int nPos = 0;
	strcpy((char*) &pTran[nPos], ptr->szFactoryCode);
	nPos += 2;
	strcpy((char*) &pTran[nPos], ptr->szChipCode);
	nPos += 2;

	pTran[nPos++] = ptr->nDay;
	pTran[nPos++] = ptr->nMonth;
	pTran[nPos++] = ptr->nYear;
	pTran[nPos++] = LOBYTE(ptr->nVersion);
	pTran[nPos++] = HIBYTE(ptr->nVersion);

	return nPos;
}

static int plms_0CF240(BYTE* pTran, void* pAppl) {
	FK_DT0CF240* ptr = (FK_DT0CF240*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_01(&pTran[nPos], &ptr->tmStart);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->wBuildNum);

	return nPos;
}

static int plms_0CF241(BYTE* pTran, void* pAppl) {
	FK_DT0CF241* ptr = (FK_DT0CF241*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], ptr->gMetSucFlag, 132);
	nPos += 132;

	return nPos;
}

///////////////////////////////////////////////////////////////////////////////
// AFN = 0D
///////////////////////////////////////////////////////////////////////////////
// 组1 (日冻结)
// F1正向有/无功电能示值 一/四象限无功电能示值
static int plms_0DF01(BYTE *pTran, void *pAppl) {
	FK_DT0DF01 *ptr = (FK_DT0DF01*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm1QEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm4QEnergy[i]);
	}
	return nPos;
}

// F2反向有/无功电能示值 一/四象限无功电能示值
static int plms_0DF02(BYTE *pTran, void *pAppl) {
	return plms_0DF01(pTran, pAppl);
}
// F3正向有/无功最大需量及发生时间
static int plms_0DF03(BYTE *pTran, void *pAppl) {
	FK_DT0DF03 *ptr = (FK_DT0DF03*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxP[i].dmValue);
	}
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxP[i].dmTime);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxQ[i].dmValue);
	}
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxQ[i].dmTime);
	}
	return nPos;
}
// F4反向有/无功最大需量及发生时间
static int plms_0DF04(BYTE *pTran, void *pAppl) {
	return plms_0DF03(pTran, pAppl);
}

// F5正向有功电能量
static int plms_0DF05(BYTE *pTran, void *pAppl) {
	FK_DT0DF05 *ptr = (FK_DT0DF05*) pAppl;
	int nPos = 0;
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_13(&pTran[nPos], &ptr->dmEnergy[i]);
	}
	return nPos;
}

// F6正向无功电能量
static int plms_0DF06(BYTE *pTran, void *pAppl) {
	return plms_0DF05(pTran, pAppl);
}

// F7反向有功电能量
static int plms_0DF07(BYTE *pTran, void *pAppl) {
	return plms_0DF05(pTran, pAppl);
}

// F8反向无功电能量
static int plms_0DF08(BYTE *pTran, void *pAppl) {
	return plms_0DF05(pTran, pAppl);
}

// 组2 (抄表日冻结)
// F9正向有/无功电能示值 一/四象限无功电能示值
static int plms_0DF09(BYTE *pTran, void *pAppl) {
	return plms_0DF01(pTran, pAppl);
}

// F10反向有/无功电能示值 一/四象限无功电能示值
static int plms_0DF10(BYTE *pTran, void *pAppl) {
	return plms_0DF02(pTran, pAppl);
}

// F11正向有/无功最大需量及发生时间
static int plms_0DF11(BYTE *pTran, void *pAppl) {
	return plms_0DF03(pTran, pAppl);
}

// F12反向有/无功最大需量及发生时间
static int plms_0DF12(BYTE *pTran, void *pAppl) {
	return plms_0DF04(pTran, pAppl);
}

// 组3 (月冻结)
// F17正向有/无功电能示值 一/四象限无功电能示值
static int plms_0DF17(BYTE *pTran, void *pAppl) {
	FK_DT0DF17 *ptr = (FK_DT0DF17*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm1QEnergy[i]);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm4QEnergy[i]);
	}
	return nPos;
}

// F18反向有/无功电能示值 一/四象限无功电能示值
static int plms_0DF18(BYTE *pTran, void *pAppl) {
	return plms_0DF17(pTran, pAppl);
}

// F19正向有/无功最大需量及发生时间
static int plms_0DF19(BYTE *pTran, void *pAppl) {
	FK_DT0DF19 *ptr = (FK_DT0DF19*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxP[i].dmValue);
	}
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxP[i].dmTime);
	}
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxQ[i].dmValue);
	}
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxQ[i].dmTime);
	}
	//
	return nPos;
}

// F20反向有/无功最大需量及发生时间
static int plms_0DF20(BYTE *pTran, void *pAppl) {
	return plms_0DF19(pTran, pAppl);
}
// F21正向有功电能量
static int plms_0DF21(BYTE *pTran, void *pAppl) {
	FK_DT0DF21 *ptr = (FK_DT0DF21*) pAppl;
	int nPos = 0;
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_13(&pTran[nPos], &ptr->dmEnergy[i]);
	}
	return nPos;
}

// F22正向无功电能量
static int plms_0DF22(BYTE *pTran, void *pAppl) {
	return plms_0DF21(pTran, pAppl);
}

// F23反向有功电能量
static int plms_0DF23(BYTE *pTran, void *pAppl) {
	return plms_0DF21(pTran, pAppl);
}

// F24反向无功电能量
static int plms_0DF24(BYTE *pTran, void *pAppl) {
	return plms_0DF21(pTran, pAppl);
}

// 组4 (日冻结)
// F25日总及分相最大有功功率级发生时间、有功功率为零时间
static int plms_0DF25(BYTE *pTran, void *pAppl) {
	FK_DT0DF25 *ptr = (FK_DT0DF25*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxPs);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPsTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxPa);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPaTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxPb);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPbTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxPc);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPcTime);
	pTran[nPos++] = LOBYTE(ptr->nPsTimes);
	pTran[nPos++] = HIBYTE(ptr->nPsTimes);
	pTran[nPos++] = LOBYTE(ptr->nPaTimes);
	pTran[nPos++] = HIBYTE(ptr->nPaTimes);
	pTran[nPos++] = LOBYTE(ptr->nPbTimes);
	pTran[nPos++] = HIBYTE(ptr->nPbTimes);
	pTran[nPos++] = LOBYTE(ptr->nPcTimes);
	pTran[nPos++] = HIBYTE(ptr->nPcTimes);
	return nPos;
}

// F26日总及分相最大需量及发生时间
static int plms_0DF26(BYTE *pTran, void *pAppl) {
	FK_DT0DF26 *ptr = (FK_DT0DF26*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxSDemand);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxSDemandTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxADemand);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxADemandTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxBDemand);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxBDemandTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxCDemand);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxCDemandTime);
	return nPos;
}

// F27日电压统计数据
static int plms_0DF27(BYTE *pTran, void *pAppl) {
	FK_DT0DF27 *ptr = (FK_DT0DF27*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->nuulVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nddlVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->ndlVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nokVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nuulVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nddlVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->ndlVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nokVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nuulVcTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nddlVcTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulVcTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->ndlVcTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nokVcTimes, 2);
	nPos += 2;
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMaxVa);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxVaTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMinVa);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinVaTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMaxVb);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxVbTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMinVb);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinVbTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMaxVc);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxVcTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMinVc);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinVcTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmAvgVa);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmAvgVb);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmAvgVc);
	return nPos;
}

// F28日不平衡度越限累计时间
static int plms_0DF28(BYTE *pTran, void *pAppl) {
	FK_DT0DF28 *ptr = (FK_DT0DF28*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->nITimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nVTimes, 2);
	nPos += 2;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMaxIValue);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxITime);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMaxVValue);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxVTime);
	return nPos;
}

// F29日电流越限统计
static int plms_0DF29(BYTE *pTran, void *pAppl) {
	FK_DT0DF29 *ptr = (FK_DT0DF29*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->nuulIa, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulIa, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nuulIb, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulIb, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nuulIc, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulIc, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulI0, 2);
	nPos += 2;
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmMaxIa);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxIaTime);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmMaxIb);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxIbTime);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmMaxIc);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxIcTime);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmMaxI0);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxI0Time);
	return nPos;
}

// F30日视在功率越限累计时间
static int plms_0DF30(BYTE *pTran, void *pAppl) {
	FK_DT0DF30 *ptr = (FK_DT0DF30*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->nuulPTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulPTimes, 2);
	nPos += 2;
	return nPos;
}

// F31日负载率统计
static int plms_0DF31(BYTE *pTran, void *pAppl) {
	FK_DT0DF31 *ptr = (FK_DT0DF31*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMaxPs);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPsTime);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMinPs);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinPsTime);
	return nPos;
}

// F32日电能表断相数据
static int plms_0DF32(BYTE *pTran, void *pAppl) {
	FK_DT0DF32 *ptr = (FK_DT0DF32*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->dmPBsRefs);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->dmPBaRefs);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->dmPBbRefs);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->dmPBcRefs);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmPBsTimes);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmPBaTimes);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmPBbTimes);
	nPos += plms_fmt_tran_10(&pTran[nPos], &ptr->dmPBcTimes);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmPBBeginTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmPBaBeginTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmPBbBeginTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmPBcBeginTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmPBEndTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmPBaEndTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmPBbEndTime);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmPBcEndTime);
	return nPos;
}
// 组5 (月冻结)
// F33月总及分相最大有功功率级发生时间、有功功率为零时间
static int plms_0DF33(BYTE *pTran, void *pAppl) {
	FK_DT0DF33 *ptr = (FK_DT0DF33*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxPs);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPsTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxPa);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPaTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxPb);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPbTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxPc);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPcTime);
//  gs4/13/2009 1:11PM
//	pTran[nPos++] = ptr->nPsTimes;
//	pTran[nPos++] = ptr->nPaTimes;
//	pTran[nPos++] = ptr->nPbTimes;
//	pTran[nPos++] = ptr->nPcTimes;
	pTran[nPos++] = LOBYTE(ptr->nPsTimes);
	pTran[nPos++] = HIBYTE(ptr->nPsTimes);
	pTran[nPos++] = LOBYTE(ptr->nPaTimes);
	pTran[nPos++] = HIBYTE(ptr->nPaTimes);
	pTran[nPos++] = LOBYTE(ptr->nPbTimes);
	pTran[nPos++] = HIBYTE(ptr->nPbTimes);
	pTran[nPos++] = LOBYTE(ptr->nPcTimes);
	pTran[nPos++] = HIBYTE(ptr->nPcTimes);
	return nPos;
}

// F34月总及分相最大需量及发生时间
static int plms_0DF34(BYTE *pTran, void *pAppl) {
	FK_DT0DF34 *ptr = (FK_DT0DF34*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxSDemand);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxSDemandTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxADemand);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxADemandTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxBDemand);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxBDemandTime);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxCDemand);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxCDemandTime);
	return nPos;
}

// F35月电压统计数据
static int plms_0DF35(BYTE *pTran, void *pAppl) {
	FK_DT0DF35 *ptr = (FK_DT0DF35*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->nuulVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nddlVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->ndlVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nokVaTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nuulVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nddlVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->ndlVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nokVbTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nuulVcTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nddlVcTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulVcTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->ndlVcTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nokVcTimes, 2);
	nPos += 2;
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMaxVa);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxVaTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMinVa);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinVaTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMaxVb);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxVbTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMinVb);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinVbTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMaxVc);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxVcTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmMinVc);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinVcTime);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmAvgVa);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmAvgVb);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmAvgVc);
	return nPos;
}

// F36月不平衡度越限累计时间
static int plms_0DF36(BYTE *pTran, void *pAppl) {
	FK_DT0DF36 *ptr = (FK_DT0DF36*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->nITimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nVTimes, 2);
	nPos += 2;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMaxIValue);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxITime);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMaxVValue);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxVTime);
	return nPos;
}

// F37月电流越限统计
static int plms_0DF37(BYTE *pTran, void *pAppl) {
	FK_DT0DF37 *ptr = (FK_DT0DF37*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->nuulIa, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulIa, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nuulIb, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulIb, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nuulIc, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulIc, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulI0, 2);
	nPos += 2;
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmMaxIa);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxIaTime);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmMaxIb);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxIbTime);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmMaxIc);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxIcTime);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmMaxI0);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxI0Time);
	return nPos;
}

// F38月视在功率越限累计时间
static int plms_0DF38(BYTE *pTran, void *pAppl) {
	FK_DT0DF38 *ptr = (FK_DT0DF38*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->nuulPTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->nulPTimes, 2);
	nPos += 2;
	return nPos;
}

static int plms_0DF39(BYTE *pTran, void *pAppl) {
	FK_DT0DF39 *ptr = (FK_DT0DF39*) pAppl;
	int nPos = 0;

	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMaxPs);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxPsTime);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMinPs);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMinPsTime);

	return nPos;
}
// 组6
// F41电容器投入累计时间和次数
static int plms_0DF41(BYTE *pTran, void *pAppl) {
	FK_DT0DF41 *ptr = (FK_DT0DF41*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->dwTimes, 36);
	nPos += 36;
	memcpy(&pTran[nPos], &ptr->dwCount, 36);
	nPos += 36;
	return nPos;
}

// F42日、月电容器累计补偿的无功电能量
static int plms_0DF42(BYTE *pTran, void *pAppl) {
	FK_DT0DF42 *ptr = (FK_DT0DF42*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_13(&pTran[nPos], &ptr->dmDayEnergy);
	nPos += plms_fmt_tran_13(&pTran[nPos], &ptr->dmMonEnergy);
	return nPos;
}

// F43日功率因素区段累计时间
static int plms_0DF43(BYTE *pTran, void *pAppl) {
	FK_DT0DF43 *ptr = (FK_DT0DF43*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->wT1Times, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->wT2Times, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->wT3Times, 2);
	nPos += 2;
	return nPos;
}

// F44月功率因素区段累计时间
static int plms_0DF44(BYTE *pTran, void *pAppl) {
	FK_DT0DF44 *ptr = (FK_DT0DF44*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->wT1Times, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->wT2Times, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->wT3Times, 2);
	nPos += 2;
	return nPos;
}

// 组7
// F49终端日供电时间、日复位累计次数
static int plms_0DF49(BYTE *pTran, void *pAppl) {
	FK_DT0DF49 *ptr = (FK_DT0DF49*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->wRunTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->wResetRefs, 2);
	nPos += 2;
	return nPos;
}

// F50终端日控制统计数据
static int plms_0DF50(BYTE *pTran, void *pAppl) {
	FK_DT0DF50 *ptr = (FK_DT0DF50*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nMECtrlRefs;
	pTran[nPos++] = ptr->nGECtrlRefs;
	pTran[nPos++] = ptr->nPCtrlRefs;
	pTran[nPos++] = ptr->nRCtrlRefs;
	return nPos;
}

// F51终端月供电时间、月复位累计次数
static int plms_0DF51(BYTE *pTran, void *pAppl) {
	FK_DT0DF51 *ptr = (FK_DT0DF51*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->wRunTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->wResetRefs, 2);
	nPos += 2;
	return nPos;
}

// F52终端月控制统计数据
static int plms_0DF52(BYTE *pTran, void *pAppl) {
	FK_DT0DF52 *ptr = (FK_DT0DF52*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nMECtrlRefs;
	pTran[nPos++] = ptr->nGECtrlRefs;
	pTran[nPos++] = ptr->nPCtrlRefs;
	pTran[nPos++] = ptr->nRCtrlRefs;
	return nPos;
}

// F53终端日通讯流量
static int plms_0DF53(BYTE *pTran, void *pAppl) {
	FK_DT0DF53 *ptr = (FK_DT0DF53*) pAppl;
	int nPos = 0;

	memcpy(&pTran[nPos], &ptr->dwDayCommFlow, 4);
	nPos += 4;
	return nPos;
}

// F54终端月通讯流量
static int plms_0DF54(BYTE *pTran, void *pAppl) {
	FK_DT0DF54 *ptr = (FK_DT0DF54*) pAppl;
	int nPos = 0;

	memcpy(&pTran[nPos], &ptr->dwMonCommFlow, 4);
	nPos += 4;
	return nPos;
}

// 组8
// F57总加组日最大、最小有功功率及其发生时间 有功功率为零日累计时间
static int plms_0DF57(BYTE *pTran, void *pAppl) {
	FK_DT0DF57 *ptr = (FK_DT0DF57*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmMaxP);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPTime);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmMinP);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinPTime);
	memcpy(&pTran[nPos], &ptr->wP0Times, 2);
	nPos += 2;
	return nPos;
}

// F58总加组日累计有功电能量
static int plms_0DF58(BYTE *pTran, void *pAppl) {
	FK_DT0DF58 *ptr = (FK_DT0DF58*) pAppl;
	int nPos = 0;
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmEnergy[i]);
	}
	return nPos;
}

// F59总加组日累计无功电能量
static int plms_0DF59(BYTE *pTran, void *pAppl) {
	FK_DT0DF59 *ptr = (FK_DT0DF59*) pAppl;
	int nPos = 0;
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmEnergy[i]);
	}
	return nPos;
}

// F60总加组月最大、最小有功功率及其发生时间 有功功率为零日累计时间
static int plms_0DF60(BYTE *pTran, void *pAppl) {
	FK_DT0DF60 *ptr = (FK_DT0DF60*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmMaxP);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxPTime);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmMinP);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinPTime);
	memcpy(&pTran[nPos], &ptr->wP0Times, 2);
	nPos += 2;
	return nPos;
}

// F61总加组月累计有功电能量
static int plms_0DF61(BYTE *pTran, void *pAppl) {
	FK_DT0DF61 *ptr = (FK_DT0DF61*) pAppl;
	int nPos = 0;
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmEnergy[i]);
	}
	return nPos;
}

// F62总加组月累计无功电能量
static int plms_0DF62(BYTE *pTran, void *pAppl) {
	FK_DT0DF62 *ptr = (FK_DT0DF62*) pAppl;
	int nPos = 0;
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmEnergy[i]);
	}
	return nPos;
}

// 组9
// F65总加组超功率定值的月累计时间、月累计电能量
static int plms_0DF65(BYTE *pTran, void *pAppl) {
	FK_DT0DF65 *ptr = (FK_DT0DF65*) pAppl;
	int nPos = 0;
	pTran[nPos++] = LOBYTE(ptr->wTimes);
	pTran[nPos++] = HIBYTE(ptr->wTimes);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmEnergy);
	return nPos;
}

// F66总加组超月电能量定值得月累计时间、累计电能量
static int plms_0DF66(BYTE *pTran, void *pAppl) {
	FK_DT0DF66 *ptr = (FK_DT0DF66*) pAppl;
	int nPos = 0;
	pTran[nPos++] = LOBYTE(ptr->wTimes);
	pTran[nPos++] = HIBYTE(ptr->wTimes);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmEnergy);
	return nPos;
}

// 组10
// F73总加组有功功率
static int plms_0DF73(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(2, pTran, pAppl);
}

// F74总加组无功功率
static int plms_0DF74(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(2, pTran, pAppl);
}

// F75总加组有功电量
static int plms_0DF75(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(3, pTran, pAppl);
}

// F76总加组无功电量
static int plms_0DF76(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(3, pTran, pAppl);
}

// 组11
// F81测量点有功功率
static int plms_0DF81(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(9, pTran, pAppl);
}

// F82测量点U相有功功率
static int plms_0DF82(BYTE *pTran, void *pAppl) {
	return plms_0DF81(pTran, pAppl);
}

// F83测量点V相有功功率
static int plms_0DF83(BYTE *pTran, void *pAppl) {
	return plms_0DF81(pTran, pAppl);
}

// F84测量点W相有功功率
static int plms_0DF84(BYTE *pTran, void *pAppl) {
	return plms_0DF81(pTran, pAppl);
}

// F85测量点无功功率
static int plms_0DF85(BYTE *pTran, void *pAppl) {
	return plms_0DF81(pTran, pAppl);
}

// F86测量点U相无功功率
static int plms_0DF86(BYTE *pTran, void *pAppl) {
	return plms_0DF81(pTran, pAppl);
}

// F87测量点V相无功功率
static int plms_0DF87(BYTE *pTran, void *pAppl) {
	return plms_0DF81(pTran, pAppl);
}

// F88测量点W相无功功率
static int plms_0DF88(BYTE *pTran, void *pAppl) {
	return plms_0DF81(pTran, pAppl);
}

// 组12
// F89U相电压
static int plms_0DF89(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(7, pTran, pAppl);
}

// F90V相电压
static int plms_0DF90(BYTE *pTran, void *pAppl) {
	return plms_0DF89(pTran, pAppl);
}

// F91W相电压
static int plms_0DF91(BYTE *pTran, void *pAppl) {
	return plms_0DF89(pTran, pAppl);
}

// F92U相电流
static int plms_0DF92(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(25, pTran, pAppl);
}

// F93V相电流
static int plms_0DF93(BYTE *pTran, void *pAppl) {
	return plms_0DF92(pTran, pAppl);
}

// F94W相电流
static int plms_0DF94(BYTE *pTran, void *pAppl) {
	return plms_0DF92(pTran, pAppl);
}

// F95零序电流
static int plms_0DF95(BYTE *pTran, void *pAppl) {
	return plms_0DF92(pTran, pAppl);
}

// 组13
// F97测量点正向有功总电能
static int plms_0DF97(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(13, pTran, pAppl);
}

// F98测量点正向无功总电能
static int plms_0DF98(BYTE *pTran, void *pAppl) {
	return plms_0DF97(pTran, pAppl);
}

// F99测量点反向有功总电能
static int plms_0DF99(BYTE *pTran, void *pAppl) {
	return plms_0DF97(pTran, pAppl);
}

// F100测量点反向无功总电能
static int plms_0DF100(BYTE *pTran, void *pAppl) {
	return plms_0DF97(pTran, pAppl);
}

// F101测量点正向有功总电能示值
static int plms_0DF101(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(11, pTran, pAppl);
}

// F102测量点正向无功总电能示值
static int plms_0DF102(BYTE *pTran, void *pAppl) {
	return plms_0DF101(pTran, pAppl);
}

// F103测量点反向有功总电能示值
static int plms_0DF103(BYTE *pTran, void *pAppl) {
	return plms_0DF101(pTran, pAppl);
}

// F104测量点反向无功总电能示值
static int plms_0DF104(BYTE *pTran, void *pAppl) {
	return plms_0DF101(pTran, pAppl);
}

// 组14
// F105总功率因素
static int plms_0DF105(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(5, pTran, pAppl);
}

// F106U相功率因素
static int plms_0DF106(BYTE *pTran, void *pAppl) {
	return plms_0DF105(pTran, pAppl);
}

// F107V相功率因素
static int plms_0DF107(BYTE *pTran, void *pAppl) {
	return plms_0DF105(pTran, pAppl);
}

// F108W相功率因素
static int plms_0DF108(BYTE *pTran, void *pAppl) {
	return plms_0DF105(pTran, pAppl);
}

// F109电压相位角曲线
static int plms_0DF109(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(5, pTran, pAppl);
}

// F110电流相位角曲线
static int plms_0DF110(BYTE *pTran, void *pAppl) {
	return plms_0DF109(pTran, pAppl);
}

// 组15
// F113U相2次~19次谐波电流日最大值及发生时间
static int plms_0DF113(BYTE *pTran, void *pAppl) {
	FK_DT0DF113 *ptr = (FK_DT0DF113*) pAppl;
	int nPos = 0;
	for (int i = 2; i <= 19; ++i) {
		nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->hHarm[i - 2].value);
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->hHarm[i - 2].time);
	}
	nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->dmMaxI);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxITime);
	return nPos;
}

// F114V相2次~19次谐波电流日最大值及发生时间
static int plms_0DF114(BYTE *pTran, void *pAppl) {
	return plms_0DF113(pTran, pAppl);
}

// F115W相2次~19次谐波电流日最大值及发生时间
static int plms_0DF115(BYTE *pTran, void *pAppl) {
	return plms_0DF113(pTran, pAppl);
}

// F116U相2次~19次谐波电压含有率及总畸变率日最大值及发生时间
static int plms_0DF116(BYTE *pTran, void *pAppl) {
	FK_DT0DF116 *ptr = (FK_DT0DF116*) pAppl;
	int nPos = 0;
	for (int i = 2; i <= 19; ++i) {
		nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->hHarm[i - 2].value);
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->hHarm[i - 2].time);
	}
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmMaxV);
	nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxVTime);
	return nPos;
}

// F117V相2次~19次谐波电压含有率及总畸变率日最大值及发生时间
static int plms_0DF117(BYTE *pTran, void *pAppl) {
	return plms_0DF116(pTran, pAppl);
}

// F118W相2次~19次谐波电压含有率及总畸变率日最大值及发生时间
static int plms_0DF118(BYTE *pTran, void *pAppl) {
	return plms_0DF116(pTran, pAppl);
}

// 组16
// F121U相谐波越限日统计数据
static int plms_0DF121(BYTE *pTran, void *pAppl) {
	FK_DT0DF121 *ptr = (FK_DT0DF121*) pAppl;
	int nPos = 0;
	pTran[nPos++] = ptr->nHarms;
	memcpy(&pTran[nPos], &ptr->wVsTimes, 2);
	nPos += 2;
	for (int i = 0; i < ptr->nHarms; ++i) {
		memcpy(&pTran[nPos], &ptr->wVpTimes[18], 2);
		nPos += 2;
	}
	memcpy(&pTran[nPos], &ptr->wIsTimes, 2);
	nPos += 2;
	for (int i = 0; i < ptr->nHarms; ++i) {
		memcpy(&pTran[nPos], &ptr->wIpTimes[18], 2);
		nPos += 2;
	}
	return nPos;
}

// F122V相谐波越限日统计数据
static int plms_0DF122(BYTE *pTran, void *pAppl) {
	return plms_0DF121(pTran, pAppl);
}

// F123W相谐波越限日统计数据
static int plms_0DF123(BYTE *pTran, void *pAppl) {
	return plms_0DF121(pTran, pAppl);
}

// 组17
// F129直流模拟量越限日累计时间、最大/最小值及发生时间
static int plms_0DF129(BYTE *pTran, void *pAppl) {
	FK_DT0DF129 *ptr = (FK_DT0DF129*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->wulTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->wllTimes, 2);
	nPos += 2;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmMax);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxTime);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmMin);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinTime);
	return nPos;
}

// F130直流模拟量越限月累计时间、最大/最小值及发生时间
static int plms_0DF130(BYTE *pTran, void *pAppl) {
	FK_DT0DF130 *ptr = (FK_DT0DF130*) pAppl;
	int nPos = 0;
	memcpy(&pTran[nPos], &ptr->wulTimes, 2);
	nPos += 2;
	memcpy(&pTran[nPos], &ptr->wllTimes, 2);
	nPos += 2;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmMax);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMaxTime);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmMin);
	nPos += plms_fmt_tran_18(&pTran[nPos], &ptr->dmMinTime);
	return nPos;
}

// 组18
// F138直流模拟量数据
static int plms_0DF138(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(2, pTran, pAppl);
}

// Add 2009-11-10 for 698
//
// 组19
// 一象限无功总电能示值曲线
static int plms_0DF145(BYTE *pTran, void *pAppl) {
	return plms_fmt_tran(11, pTran, pAppl);
}
// 四象限无功总电能示值曲线
static int plms_0DF146(BYTE *pTran, void *pAppl) {
	return plms_0DF145(pTran, pAppl);
}
// 三象限无功总电能示值曲线
static int plms_0DF147(BYTE *pTran, void *pAppl) {
	return plms_0DF145(pTran, pAppl);
}
// 二象限无功总电能示值曲线
static int plms_0DF148(BYTE *pTran, void *pAppl) {
	return plms_0DF145(pTran, pAppl);
}

// F153日冻结分相正向有功电能示值
static int plms_0DF153(BYTE *pTran, void *pAppl) {
	FK_DT0DF153 *ptr = (FK_DT0DF153*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergyA);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergyB);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergyC);
	return nPos;
}

// F154日冻结分相正向无功电能示值
static int plms_0DF154(BYTE *pTran, void *pAppl) {
	FK_DT0DF154 *ptr = (FK_DT0DF154*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergyA);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergyB);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergyC);
	return nPos;
}

// F155日冻结分相反向有功电能示值
static int plms_0DF155(BYTE *pTran, void *pAppl) {
	return plms_0DF153(pTran, pAppl);
}

// F156日冻结分相反向无功电能示值
static int plms_0DF156(BYTE *pTran, void *pAppl) {
	return plms_0DF154(pTran, pAppl);
}

// F157月冻结分相正向有功电能示值
static int plms_0DF157(BYTE *pTran, void *pAppl) {
	return plms_0DF153(pTran, pAppl);
}

// F158月冻结分相正向无功电能示值
static int plms_0DF158(BYTE *pTran, void *pAppl) {
	return plms_0DF154(pTran, pAppl);
}

// F159月冻结分相反向有功电能示值
static int plms_0DF159(BYTE *pTran, void *pAppl) {
	return plms_0DF153(pTran, pAppl);
}

// F160月冻结分相反向无功电能示值
static int plms_0DF160(BYTE *pTran, void *pAppl) {
	return plms_0DF154(pTran, pAppl);
}

//
// 组21 (日冻结)
// F161正向有功电能示值
static int plms_0DF161(BYTE *pTran, void *pAppl) {
	FK_DT0DF01 *ptr = (FK_DT0DF01*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy[i]);
	}
	return nPos;
}
// F162正向无功电能示值
static int plms_0DF162(BYTE *pTran, void *pAppl) {
	FK_DT0DF01 *ptr = (FK_DT0DF01*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergy[i]);
	}
	return nPos;
}

// F163反向有功电能示值
static int plms_0DF163(BYTE *pTran, void *pAppl) {
	return plms_0DF161(pTran, pAppl);
}

// F164反向无功电能示值
static int plms_0DF164(BYTE *pTran, void *pAppl) {
	return plms_0DF162(pTran, pAppl);
}

// F165一象限无功电能示值
static int plms_0DF165(BYTE *pTran, void *pAppl) {
	FK_DT0DF01 *ptr = (FK_DT0DF01*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm1QEnergy[i]);
	}
	return nPos;
}
// F166二象限无功电能示值
static int plms_0DF166(BYTE *pTran, void *pAppl) {
	return plms_0DF165(pTran, pAppl);
}

// F167三象限无功电能示值
static int plms_0DF167(BYTE *pTran, void *pAppl) {
	FK_DT0DF01 *ptr = (FK_DT0DF01*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm4QEnergy[i]);
	}
	return nPos;
}

// F168四象限无功电能示值
static int plms_0DF168(BYTE *pTran, void *pAppl) {
	return plms_0DF167(pTran, pAppl);
}

// 组22 (抄表日冻结)
// F169正向有功电能示值
static int plms_0DF169(BYTE *pTran, void *pAppl) {
	return plms_0DF161(pTran, pAppl);
}

// F170正向无功电能示值
static int plms_0DF170(BYTE *pTran, void *pAppl) {
	return plms_0DF162(pTran, pAppl);
}

// F171反向有功电能示值
static int plms_0DF171(BYTE *pTran, void *pAppl) {
	return plms_0DF161(pTran, pAppl);
}

// F172反向无功电能示值
static int plms_0DF172(BYTE *pTran, void *pAppl) {
	return plms_0DF162(pTran, pAppl);
}

// F173一象限无功电能示值
static int plms_0DF173(BYTE *pTran, void *pAppl) {
	return plms_0DF165(pTran, pAppl);
}

// F174二象限无功电能示值
static int plms_0DF174(BYTE *pTran, void *pAppl) {
	return plms_0DF165(pTran, pAppl);
}

// F175三象限无功电能示值
static int plms_0DF175(BYTE *pTran, void *pAppl) {
	return plms_0DF167(pTran, pAppl);
}

// F176四象限无功电能示值
static int plms_0DF176(BYTE *pTran, void *pAppl) {
	return plms_0DF167(pTran, pAppl);
}

// 组23 (月冻结)
// F177正向有功电能示值
static int plms_0DF177(BYTE *pTran, void *pAppl) {
	FK_DT0DF17 *ptr = (FK_DT0DF17*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy[i]);
	}
	return nPos;
}

// F178正向无功电能示值
static int plms_0DF178(BYTE *pTran, void *pAppl) {
	FK_DT0DF17 *ptr = (FK_DT0DF17*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQEnergy[i]);
	}
	return nPos;
}

// F179反向有功电能示值
static int plms_0DF179(BYTE *pTran, void *pAppl) {
	return plms_0DF177(pTran, pAppl);
}

// F180反向无功电能示值
static int plms_0DF180(BYTE *pTran, void *pAppl) {
	return plms_0DF178(pTran, pAppl);
}

// F181一象限无功电能示值
static int plms_0DF181(BYTE *pTran, void *pAppl) {
	FK_DT0DF17 *ptr = (FK_DT0DF17*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm1QEnergy[i]);
	}
	return nPos;
}

// F182二象限无功电能示值
static int plms_0DF182(BYTE *pTran, void *pAppl) {
	return plms_0DF181(pTran, pAppl);
}

// F183三象限无功电能示值
static int plms_0DF183(BYTE *pTran, void *pAppl) {
	FK_DT0DF17 *ptr = (FK_DT0DF17*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;

	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm4QEnergy[i]);
	}
	return nPos;
}

// F184四象限无功电能示值
static int plms_0DF184(BYTE *pTran, void *pAppl) {
	return plms_0DF183(pTran, pAppl);
}

// 组24
// (日冻结)
// F185正向有功最大需量及发生时间
static int plms_0DF185(BYTE *pTran, void *pAppl) {
	FK_DT0DF03 *ptr = (FK_DT0DF03*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxP[i].dmValue);
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxP[i].dmTime);
	}

	return nPos;
}

// F186正向无功最大需量及发生时间
static int plms_0DF186(BYTE *pTran, void *pAppl) {
	FK_DT0DF03 *ptr = (FK_DT0DF03*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxQ[i].dmValue);
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxQ[i].dmTime);
	}
	//

	return nPos;
}

// F187反向有功最大需量及发生时间
static int plms_0DF187(BYTE *pTran, void *pAppl) {
	return plms_0DF185(pTran, pAppl);
}

// F188反向无功最大需量及发生时间
static int plms_0DF188(BYTE *pTran, void *pAppl) {
	return plms_0DF186(pTran, pAppl);
}

// (抄表日冻结)
// F189正向有功最大需量及发生时间
static int plms_0DF189(BYTE *pTran, void *pAppl) {
	return plms_0DF185(pTran, pAppl);
}

// F190正向无功最大需量及发生时间
static int plms_0DF190(BYTE *pTran, void *pAppl) {
	return plms_0DF186(pTran, pAppl);
}

// F191反向有功最大需量及发生时间
static int plms_0DF191(BYTE *pTran, void *pAppl) {
	return plms_0DF185(pTran, pAppl);
}

// F192反向无功最大需量及发生时间
static int plms_0DF192(BYTE *pTran, void *pAppl) {
	return plms_0DF186(pTran, pAppl);
}

// 组25
// (月冻结)
// F193正向有功最大需量及发生时间
static int plms_0DF193(BYTE *pTran, void *pAppl) {
	FK_DT0DF19 *ptr = (FK_DT0DF19*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxP[i].dmValue);
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxP[i].dmTime);
	}

	return nPos;
}

// F194正向无功最大需量及发生时间
static int plms_0DF194(BYTE *pTran, void *pAppl) {
	FK_DT0DF19 *ptr = (FK_DT0DF19*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmMaxQ[i].dmValue);
		nPos += plms_fmt_tran_17(&pTran[nPos], &ptr->dmMaxQ[i].dmTime);
	}

	return nPos;
}

// F195反向有功最大需量及发生时间
static int plms_0DF195(BYTE *pTran, void *pAppl) {
	return plms_0DF193(pTran, pAppl);
}

// F196反向无功最大需量及发生时间
static int plms_0DF196(BYTE *pTran, void *pAppl) {
	return plms_0DF194(pTran, pAppl);
}

// F201月冻结第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0DF201(BYTE *pTran, void *pAppl) {
	FK_DT0DF201 *ptr = (FK_DT0DF201*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy[i].llVal);
	}

	return nPos;
}
// F202月冻结第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0DF202(BYTE *pTran, void *pAppl) {
	return plms_0DF201(pTran, pAppl);
}
// F203月冻结第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0DF203(BYTE *pTran, void *pAppl) {
	return plms_0DF201(pTran, pAppl);
}
// F204月冻结第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0DF204(BYTE *pTran, void *pAppl) {
	return plms_0DF201(pTran, pAppl);
}
// F205月冻结第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0DF205(BYTE *pTran, void *pAppl) {
	return plms_0DF201(pTran, pAppl);
}
// F206月冻结第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0DF206(BYTE *pTran, void *pAppl) {
	return plms_0DF201(pTran, pAppl);
}
// F207月冻结第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0DF207(BYTE *pTran, void *pAppl) {
	return plms_0DF201(pTran, pAppl);
}
// F208月冻结第一时区冻结正向有功电能示值（总、费率1~M）
static int plms_0DF208(BYTE *pTran, void *pAppl) {
	return plms_0DF201(pTran, pAppl);
}
// F209 电能表远程控制通断电状态及记录
static int plms_0DF209(BYTE *pTran, void *pAppl) {
	FK_DT0DF209 *ptr = (FK_DT0DF209*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->mStause;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->mStartTime);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->mStopTime);
	return nPos;
}

// F213 电能表开关操作次数及时间
static int plms_0DF213(BYTE *pTran, void *pAppl) {
	FK_DT0DF213 *ptr = (FK_DT0DF213*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mProgramKeyTimes);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->ProgramKeyTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mTrailCoverTimes);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->TrailCoverTime);
	return nPos;
}
// F214 电能表参数修改次数及时间
static int plms_0DF214(BYTE *pTran, void *pAppl) {
	FK_DT0DF214 *ptr = (FK_DT0DF214*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mCheckClockTimes);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->CheckClockTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mPeriodChangeTimes);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->PeriodChangeTime);
	return nPos;
}
// F215 电能表购、用电信息
static int plms_0DF215(BYTE *pTran, void *pAppl) {
	FK_DT0DF215 *ptr = (FK_DT0DF215*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	nPos += plms_fmt_tran_08(&pTran[nPos], &ptr->mBuyTimes);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->RemainRMB);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->TotalBuyRMB);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->RemainEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->OverEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->TotalBuyEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->OverdraftStopEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->AlarmEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->FailureEnergy);
	return nPos;
}
// F216 电能表结算信息
static int plms_0DF216(BYTE *pTran, void *pAppl) {
	FK_DT0DF216 *ptr = (FK_DT0DF216*) pAppl;
	int nPos = 0;
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	pTran[nPos++] = NL_FK_MAX_TARIFF;
	//
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->StrikeEnergy[i].llVal);
	}
	for (int i = 0; i <= NL_FK_MAX_TARIFF; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->UnStrikeEnergy[i].llVal);
	}

	//增加故障电量数据
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->ErrorEnergy);

	return nPos;
}
///////////////////////////////////////////////////////////////////////////////
// AFN = 0E(请求三类数据)
///////////////////////////////////////////////////////////////////////////////

//ERC1
static int plms_ERC1(BYTE *pTran, void *pAppl) {
	FK_ERC1* ptr = (FK_ERC1*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->flag.body;
	memcpy(&pTran[nPos], &ptr->sVerOld, 4);
	nPos += 4;
	memcpy(&pTran[nPos], &ptr->sVerNew, 4);
	nPos += 4;
	//
	pTran[0] = 1;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC2
static int plms_ERC2(BYTE *pTran, void *pAppl) {
	FK_ERC2 *ptr = (FK_ERC2*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->flag.body;
	//
	pTran[0] = 2;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC3
static int plms_ERC3(BYTE *pTran, void *pAppl) {
	FK_ERC3 *ptr = (FK_ERC3*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->Master;
	for (int i = 0; i < NL_FK_MAX_DTCHG; ++i) {
		if (ptr->DT[i] != 0x00000000 && ptr->DT[i] != 0xFFFFFFFF) {
			memcpy(&pTran[nPos], &ptr->DT[i], 4);
			nPos += 4;
		}
	}
	//
	pTran[0] = 3;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC4
static int plms_ERC4(BYTE *pTran, void *pAppl) {
	FK_ERC4 *ptr = (FK_ERC4*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->nState;
	pTran[nPos++] = ptr->nRevState;
	//
	pTran[0] = 4;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC5
static int plms_ERC5(BYTE *pTran, void *pAppl) {
	FK_ERC5 *ptr = (FK_ERC5*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->nWheel;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPCur);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPAft);
	//
	pTran[0] = 5;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC6
static int plms_ERC6(BYTE *pTran, void *pAppl) {
	FK_ERC6 *ptr = (FK_ERC6*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->nPN;
	pTran[nPos++] = ptr->nWheel;
	pTran[nPos++] = ptr->type.body;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPCur);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPAft);
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmPSet);
	//
	pTran[0] = 6;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC7
static int plms_ERC7(BYTE *pTran, void *pAppl) {
	FK_ERC7 *ptr = (FK_ERC7*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->nPN;
	pTran[nPos++] = ptr->nWheel;
	pTran[nPos++] = ptr->type.body;
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmECur);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmESet);
	//
	pTran[0] = 7;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC8
static int plms_ERC8(BYTE *pTran, void *pAppl) {
	FK_ERC8 *ptr = (FK_ERC8*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//pTran[nPos++] = (ptr->nPN&0x3F);
	WORD tmp = ptr->nPN & 0x0fff;
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;

	pTran[nPos++] = ptr->flag.body;
	//
	pTran[0] = 8;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC9
static int plms_ERC9(BYTE *pTran, void *pAppl) {
	FK_ERC9 *ptr = (FK_ERC9*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	//WORD tmp = (ptr->nBE<<15)|(ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;

	pTran[nPos++] = ptr->flag.body;
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVa);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVb);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVc);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIa);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIb);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIc);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmEnergy);
	//
	pTran[0] = 9;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC10
static int plms_ERC10(BYTE *pTran, void *pAppl) {
	FK_ERC10 *ptr = (FK_ERC10*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//WORD tmp = (ptr->nBE<<15)|(ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	pTran[nPos++] = ptr->flag.body;
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVa);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVb);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVc);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIa);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIb);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIc);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmEnergy);
	//
	pTran[0] = 10;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC11
static int plms_ERC11(BYTE *pTran, void *pAppl) {
	FK_ERC11 *ptr = (FK_ERC11*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//WORD tmp = (ptr->nBE<<15)|(ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleUa);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleUb);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleUc);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleIa);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleIb);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmAngleIc);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmEnergy);
	//
	pTran[0] = 11;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC12
static int plms_ERC12(BYTE *pTran, void *pAppl) {
	FK_ERC12 *ptr = (FK_ERC12*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//WORD tmp = (ptr->nBE<<15)|(ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	//
	pTran[0] = 12;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC13
static int plms_ERC13(BYTE *pTran, void *pAppl) {
	FK_ERC13 *ptr = (FK_ERC13*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//WORD tmp = (ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	pTran[nPos++] = ptr->flag.body;
	//
	pTran[0] = 13;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC14
static int plms_ERC14(BYTE *pTran, void *pAppl) {
	FK_ERC14 *ptr = (FK_ERC14*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmPowerOff);
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmPowerOn);
	//
	pTran[0] = 14;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC15
static int plms_ERC15(BYTE *pTran, void *pAppl) {
	FK_ERC15 *ptr = (FK_ERC15*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	WORD tmp = (ptr->nBE << 15) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	pTran[nPos++] = ptr->nFlag;
	memcpy(&pTran[nPos], &ptr->nHarmFlag, 3);
	nPos += 3;

	if (ptr->nFlag & 0x80) {
		nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->harm.I.ivSum);
		for (int i = 2; i <= 19; ++i) {
			nPos += plms_fmt_tran_06(&pTran[nPos], &ptr->harm.I.ivCur[i - 2]);
		}
	} else {
		nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->harm.U.hcSum);
		for (int i = 2; i <= 19; ++i) {
			nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->harm.U.hcVal[i - 2]);
		}
	}
	//
	pTran[0] = 15;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC16
static int plms_ERC16(BYTE *pTran, void *pAppl) {
	FK_ERC16 *ptr = (FK_ERC16*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ((ptr->nBE << 7) & 0x80) | (ptr->nDNO & 0x3F);
	pTran[nPos++] = ptr->nFlag;
	nPos += plms_fmt_tran_02(&pTran[nPos], &ptr->dmDMS);
	//
	pTran[0] = 16;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC17
static int plms_ERC17(BYTE *pTran, void *pAppl) {
	FK_ERC17 *ptr = (FK_ERC17*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	pTran[nPos++] = ptr->nFlag;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmImbU);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmImbI);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVa);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVb);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVc);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIa);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIb);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIc);
	//
	pTran[0] = 17;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC18
static int plms_ERC18(BYTE *pTran, void *pAppl) {
	FK_ERC18 *ptr = (FK_ERC18*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	pTran[nPos++] = ptr->nFlag;
	memcpy(&pTran[nPos], ptr->nSuit, 2);
	nPos += 2;
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFunctor);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmPower);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVolt);
	//
	pTran[0] = 18;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC19
static int plms_ERC19(BYTE *pTran, void *pAppl) {
	FK_ERC19 *ptr = (FK_ERC19*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = (ptr->nPN & 0x3F);
	memcpy(&pTran[nPos], &ptr->nIndex, 4);
	nPos += 4;
	pTran[nPos++] = ptr->nFlag;
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmBuy);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmWarn);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmCtrl);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmPreFree);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmAftFree);
	//
	pTran[0] = 19;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC20
static int plms_ERC20(BYTE *pTran, void *pAppl) {
	FK_ERC20 *ptr = (FK_ERC20*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	memcpy(&pTran[nPos], &ptr->nPW, 16);
	nPos += 16;
	pTran[nPos++] = ptr->nMSA;
	//
	pTran[0] = 20;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC21
static int plms_ERC21(BYTE *pTran, void *pAppl) {
	FK_ERC21 *ptr = (FK_ERC21*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->nError;
	//
	pTran[0] = 21;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC22
static int plms_ERC22(BYTE *pTran, void *pAppl) {
	FK_ERC22 *ptr = (FK_ERC22*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ((ptr->nBE << 7) & 0x80) | (ptr->nPN & 0x3F);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmDstEnergy);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmSrcEnergy);
	pTran[nPos++] = ptr->nRelate;
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmUtterly);
	pTran[nPos++] = ptr->nDstTN;
	for (int i = 0; i < ptr->nDstTN; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmDstTnEnergy[i]);
	}
	pTran[nPos++] = ptr->nSrcTN;
	for (int i = 0; i < ptr->nSrcTN; ++i) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmSrcTnEnergy[i]);
	}
	//
	pTran[0] = 22;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC23
static int plms_ERC23(BYTE *pTran, void *pAppl) {
	FK_ERC23 *ptr = (FK_ERC23*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	pTran[nPos++] = ptr->nPN;
	pTran[nPos++] = ptr->nWheel;
	pTran[nPos++] = ptr->type.body;
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmECur);
	nPos += plms_fmt_tran_03(&pTran[nPos], &ptr->dmESet);
	//
	pTran[0] = 23;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC24
static int plms_ERC24(BYTE *pTran, void *pAppl) {
	FK_ERC24 *ptr = (FK_ERC24*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	pTran[nPos++] = (ptr->nType) << 6 | (ptr->nPhases);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVa);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVb);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmVc);
	//
	pTran[0] = 24;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC25
static int plms_ERC25(BYTE *pTran, void *pAppl) {
	FK_ERC25 *ptr = (FK_ERC25*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	pTran[nPos++] = (ptr->nType) << 6 | (ptr->nPhases);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIa);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIb);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIc);
	//
	pTran[0] = 25;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC26
static int plms_ERC26(BYTE *pTran, void *pAppl) {
	FK_ERC26 *ptr = (FK_ERC26*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	pTran[nPos++] = (ptr->nType) << 6;
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmPCur);
	nPos += plms_fmt_tran_23(&pTran[nPos], &ptr->dmPSet);
	//
	pTran[0] = 26;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC27
static int plms_ERC27(BYTE *pTran, void *pAppl) {
	FK_ERC27 *ptr = (FK_ERC27*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//WORD tmp = (ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPreEnergy);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmAftEnergy);
	//
	pTran[0] = 27;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC28
static int plms_ERC28(BYTE *pTran, void *pAppl) {
	FK_ERC28 *ptr = (FK_ERC28*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//WORD tmp = (ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmEnergy1);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmEnergy2);
	nPos += plms_fmt_tran_22(&pTran[nPos], &ptr->valSet);
	//
	pTran[0] = 28;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC29
static int plms_ERC29(BYTE *pTran, void *pAppl) {
	FK_ERC29 *ptr = (FK_ERC29*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//WORD tmp = (ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPreEnergy);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmAftEnergy);
	nPos += plms_fmt_tran_22(&pTran[nPos], &ptr->valSet);
	//
	pTran[0] = 29;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC30
static int plms_ERC30(BYTE *pTran, void *pAppl) {
	FK_ERC30 *ptr = (FK_ERC30*) pAppl;
	int nPos = 2;
	//
	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);
	//WORD tmp = (ptr->nPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->nPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmEnergy);
	pTran[nPos++] = ptr->valSet;
	//
	pTran[0] = 30;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC31
static int plms_ERC31(BYTE *pTran, void *pAppl) {
	FK_ERC31 *ptr = (FK_ERC31*) pAppl;
	int nPos = 2;

	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	//WORD tmp = (ptr->wPN&0x0fff);
	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->wPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;

	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmLastCbTime);
	if (ptr->dmPEnergy.llVal != 0xeeeeeeeel) {
		nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPEnergy);
	} else {
		memset(&pTran[nPos], 0xee, 5);
		nPos += 5;
	}

	if (ptr->dmQnergy.dwVal != 0xeeeeeeee) {
		nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dmQnergy);
	} else {
		memset(&pTran[nPos], 0xee, 4);
		nPos += 4;
	}

	pTran[0] = 31;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC32
static int plms_ERC32(BYTE *pTran, void *pAppl) {
	FK_ERC32 *ptr = (FK_ERC32*) pAppl;
	int nPos = 2;

	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	memcpy(&pTran[nPos], &ptr->dwCommFlow, 4);
	nPos += 4;

	memcpy(&pTran[nPos], &ptr->dwFlowSet, 4);
	nPos += 4;

	pTran[0] = 32;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC33
static int plms_ERC33(BYTE *pTran, void *pAppl) {
	FK_ERC33 *ptr = (FK_ERC33*) pAppl;
	int nPos = 2;

	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	WORD tmp = (ptr->wPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;

	memcpy(&pTran[nPos], &ptr->wMetRunStateChge[0], 14);
	nPos += 14;

	memcpy(&pTran[nPos], &ptr->wMetRunState[0], 14);
	nPos += 14;

	pTran[0] = 33;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC34
static int plms_ERC34(BYTE *pTran, void *pAppl) {
	FK_ERC34 *ptr = (FK_ERC34*) pAppl;
	int nPos = 2;

	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	WORD tmp = ((ptr->nBE << 15) & 0x8000) | (ptr->wPN & 0x0fff);
	memcpy(&pTran[nPos], (BYTE *) &tmp, 2);
	nPos += 2;

	pTran[nPos] = ((ptr->nType << 6) & 0xc0) | (ptr->nPhases & 0x07);
	nPos += 1;

	pTran[0] = 34;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC35
static int plms_ERC35(BYTE *pTran, void *pAppl) {
	FK_ERC35 *ptr = (FK_ERC35*) pAppl;
	int nPos = 2;

	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmTime);

	pTran[nPos++] = ptr->nPort;

	if (ptr->nNum > sizeof(ptr->dMeter) / sizeof(FK_NEWMETER)) {
		ptr->nNum = sizeof(ptr->dMeter) / sizeof(FK_NEWMETER);
	}

	pTran[nPos++] = ptr->nNum;

	for (BYTE i = 0; i < ptr->nNum; i++) {
		nPos += plms_fmt_tran_12(&pTran[nPos], &ptr->dMeter[i].dmAddr);

		pTran[nPos] = ((ptr->dMeter[i].nPhase << 5) & 0xe0)
				| (ptr->dMeter[i].nQuality & 0x0f);
		nPos += 1;

		pTran[nPos++] = ptr->dMeter[i].nProtocol;
	}

	pTran[0] = 35;
	pTran[1] = nPos - 2;
	return nPos;
}

//ERC36
static int plms_ERC36(BYTE *pTran, void *pAppl) {
	FK_ERC36 *ptr = (FK_ERC36*) pAppl;
	int nPos = 2;

	nPos += plms_fmt_tran_15(&pTran[nPos], &ptr->dmPowerOff);

	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmPosPEnergy);
	nPos += plms_fmt_tran_14(&pTran[nPos], &ptr->dmNegPEnergy);

	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm1QEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm2QEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm3QEnergy);
	nPos += plms_fmt_tran_11(&pTran[nPos], &ptr->dm4QEnergy);

	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmUa);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmUb);
	nPos += plms_fmt_tran_07(&pTran[nPos], &ptr->dmUc);

	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIa);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIb);
	nPos += plms_fmt_tran_25(&pTran[nPos], &ptr->dmIc);

	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPs);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPa);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPb);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmPc);

	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmQs);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmQa);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmQb);
	nPos += plms_fmt_tran_09(&pTran[nPos], &ptr->dmQc);

	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFs);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFa);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFb);
	nPos += plms_fmt_tran_05(&pTran[nPos], &ptr->dmFc);

	memcpy(&pTran[nPos], &ptr->dmState[0], 14);
	nPos += 14;

	pTran[nPos++] = ptr->dmTmnState;

	pTran[0] = 36;
	pTran[1] = nPos - 2;
	return nPos;
}

static const FMT2 fmt2[] = { { FK_CMD_SETPARAM, 1, plms_04F01 }, {
		FK_CMD_SETPARAM, 2, plms_04F02 }, { FK_CMD_SETPARAM, 3, plms_04F03 }, {
		FK_CMD_SETPARAM, 4, plms_04F04 }, { FK_CMD_SETPARAM, 5, plms_04F05 }, {
		FK_CMD_SETPARAM, 6, plms_04F06 }, { FK_CMD_SETPARAM, 7, plms_04F07 }, {
		FK_CMD_SETPARAM, 8, plms_04F08 }, { FK_CMD_SETPARAM, 9, plms_04F09 }, {
		FK_CMD_SETPARAM, 10, plms_04F10 }, { FK_CMD_SETPARAM, 11, plms_04F11 },
		{ FK_CMD_SETPARAM, 12, plms_04F12 },
		{ FK_CMD_SETPARAM, 13, plms_04F13 },
		{ FK_CMD_SETPARAM, 14, plms_04F14 },
		{ FK_CMD_SETPARAM, 15, plms_04F15 },
		{ FK_CMD_SETPARAM, 16, plms_04F16 },
		{ FK_CMD_SETPARAM, 17, plms_04F17 },
		{ FK_CMD_SETPARAM, 18, plms_04F18 },
		{ FK_CMD_SETPARAM, 19, plms_04F19 },
		{ FK_CMD_SETPARAM, 20, plms_04F20 },
		{ FK_CMD_SETPARAM, 21, plms_04F21 },
		{ FK_CMD_SETPARAM, 22, plms_04F22 },
		{ FK_CMD_SETPARAM, 23, plms_04F23 },
//	{ FK_CMD_SETPARAM,	24,	plms_04F24  },
		{ FK_CMD_SETPARAM, 25, plms_04F25 },
		{ FK_CMD_SETPARAM, 26, plms_04F26 },
		{ FK_CMD_SETPARAM, 27, plms_04F27 },
		{ FK_CMD_SETPARAM, 28, plms_04F28 },
		{ FK_CMD_SETPARAM, 29, plms_04F29 },
		{ FK_CMD_SETPARAM, 30, plms_04F30 },
		{ FK_CMD_SETPARAM, 31, plms_04F31 },
//	{ FK_CMD_SETPARAM,	32,	plms_04F32  },
		{ FK_CMD_SETPARAM, 33, plms_04F33 },
		{ FK_CMD_SETPARAM, 34, plms_04F34 },
		{ FK_CMD_SETPARAM, 35, plms_04F35 },
		{ FK_CMD_SETPARAM, 36, plms_04F36 },
		{ FK_CMD_SETPARAM, 37, plms_04F37 },
		{ FK_CMD_SETPARAM, 38, plms_04F38 },
		{ FK_CMD_SETPARAM, 39, plms_04F39 },
		{ FK_CMD_SETPARAM, 41, plms_04F41 },
		{ FK_CMD_SETPARAM, 42, plms_04F42 },
		{ FK_CMD_SETPARAM, 43, plms_04F43 },
		{ FK_CMD_SETPARAM, 44, plms_04F44 },
		{ FK_CMD_SETPARAM, 45, plms_04F45 },
		{ FK_CMD_SETPARAM, 46, plms_04F46 },
		{ FK_CMD_SETPARAM, 47, plms_04F47 },
		{ FK_CMD_SETPARAM, 48, plms_04F48 },
		{ FK_CMD_SETPARAM, 49, plms_04F49 },
		{ FK_CMD_SETPARAM, 57, plms_04F57 },
		{ FK_CMD_SETPARAM, 58, plms_04F58 },
		{ FK_CMD_SETPARAM, 59, plms_04F59 },
		{ FK_CMD_SETPARAM, 60, plms_04F60 },
		{ FK_CMD_SETPARAM, 61, plms_04F61 },
//	{ FK_CMD_SETPARAM,	62,	plms_04F62  },
//	{ FK_CMD_SETPARAM,  63, plms_04F63  },	// 江苏增补
		{ FK_CMD_SETPARAM, 65, plms_04F65 },
		{ FK_CMD_SETPARAM, 66, plms_04F66 },
		{ FK_CMD_SETPARAM, 67, plms_04F67 },
		{ FK_CMD_SETPARAM, 68, plms_04F68 },
		{ FK_CMD_SETPARAM, 73, plms_04F73 },
		{ FK_CMD_SETPARAM, 74, plms_04F74 },
		{ FK_CMD_SETPARAM, 75, plms_04F75 },
		{ FK_CMD_SETPARAM, 76, plms_04F76 },
		{ FK_CMD_SETPARAM, 81, plms_04F81 },
		{ FK_CMD_SETPARAM, 82, plms_04F82 },
		{ FK_CMD_SETPARAM, 83, plms_04F83 },
		{ FK_CMD_SETPARAM, 85, plms_04F85 },	// 江苏增补
//	{ FK_CMD_SETPARAM,  91, plms_04F91  },	// 江苏增补
		{ FK_CMD_SETPARAM, 92, plms_04F92 },	// 江苏增补
//	{ FK_CMD_SETPARAM,  93, plms_04F93  },	// 江苏增补
		{ FK_CMD_SETPARAM, 101, plms_04F101 },  // plc
		{ FK_CMD_SETPARAM, 169, plms_04F169 },  // 自定义 通道类型
		{ FK_CMD_SETPARAM, 170, plms_04F170 },  // 自定义 载波方案
		{ FK_CMD_SETPARAM, 171, plms_04F171 },  // 自定义 自动获取DHCP
//	{ FK_CMD_SETPARAM,  179, plms_04F179  },  // 自定义 自动搜表时间
		{ FK_CMD_SETPARAM, 111, plms_04F111 },  // 规约增补 集中器自维护定时任务参数
		//
		{ FK_CMD_CONTROL, 1, plms_05F01 }, { FK_CMD_CONTROL, 9, plms_05F09 }, {
				FK_CMD_CONTROL, 12, plms_05F12 }, { FK_CMD_CONTROL, 25,
				plms_05F25 }, { FK_CMD_CONTROL, 31, plms_05F31 }, {
				FK_CMD_CONTROL, 32, plms_05F32 }, { FK_CMD_CONTROL, 41,
				plms_05F41 }, { FK_CMD_CONTROL, 42, plms_05F42 }, {
				FK_CMD_CONTROL, 49, plms_05F49 }, { FK_CMD_CONTROL, 50,
				plms_05F50 }, { FK_CMD_CONTROL, 51, plms_05F51 }, {
				FK_CMD_CONTROL, 52, plms_05F52 }, { FK_CMD_CONTROL, 53,
				plms_05F53 },
//	{ FK_CMD_CONTROL,	149,plms_05F149 },
		{ FK_CMD_CONTROL, 63, plms_05F63 },  //规约增补 立即启动集中器自维护
		//
		{ FK_CMD_ASKPARAM, 1, plms_0AF01 }, { FK_CMD_ASKPARAM, 2, plms_0AF02 },
		{ FK_CMD_ASKPARAM, 3, plms_0AF03 }, { FK_CMD_ASKPARAM, 4, plms_0AF04 },
		{ FK_CMD_ASKPARAM, 5, plms_0AF05 }, { FK_CMD_ASKPARAM, 6, plms_0AF06 },
		{ FK_CMD_ASKPARAM, 7, plms_0AF07 }, { FK_CMD_ASKPARAM, 8, plms_0AF08 },
		{ FK_CMD_ASKPARAM, 9, plms_0AF09 }, { FK_CMD_ASKPARAM, 10, plms_0AF10 },
		{ FK_CMD_ASKPARAM, 11, plms_0AF11 },
		{ FK_CMD_ASKPARAM, 12, plms_0AF12 },
		{ FK_CMD_ASKPARAM, 13, plms_0AF13 },
		{ FK_CMD_ASKPARAM, 14, plms_0AF14 },
		{ FK_CMD_ASKPARAM, 15, plms_0AF15 },
		{ FK_CMD_ASKPARAM, 16, plms_0AF16 },
		{ FK_CMD_ASKPARAM, 17, plms_0AF17 },
		{ FK_CMD_ASKPARAM, 18, plms_0AF18 },
		{ FK_CMD_ASKPARAM, 19, plms_0AF19 },
		{ FK_CMD_ASKPARAM, 20, plms_0AF20 },
		{ FK_CMD_ASKPARAM, 21, plms_0AF21 },
		{ FK_CMD_ASKPARAM, 22, plms_0AF22 },
		{ FK_CMD_ASKPARAM, 23, plms_0AF23 },
//	{ FK_CMD_ASKPARAM,	24,	plms_0AF24  },
		{ FK_CMD_ASKPARAM, 25, plms_0AF25 },
		{ FK_CMD_ASKPARAM, 26, plms_0AF26 },
		{ FK_CMD_ASKPARAM, 27, plms_0AF27 },
		{ FK_CMD_ASKPARAM, 28, plms_0AF28 },
		{ FK_CMD_ASKPARAM, 29, plms_0AF29 },
		{ FK_CMD_ASKPARAM, 30, plms_0AF30 },
		{ FK_CMD_ASKPARAM, 31, plms_0AF31 },
		{ FK_CMD_ASKPARAM, 33, plms_0AF33 },
		{ FK_CMD_ASKPARAM, 34, plms_0AF34 },
		{ FK_CMD_ASKPARAM, 35, plms_0AF35 },
		{ FK_CMD_ASKPARAM, 36, plms_0AF36 },
		{ FK_CMD_ASKPARAM, 37, plms_0AF37 },
		{ FK_CMD_ASKPARAM, 38, plms_0AF38 },
		{ FK_CMD_ASKPARAM, 39, plms_0AF39 },
		{ FK_CMD_ASKPARAM, 41, plms_0AF41 },
		{ FK_CMD_ASKPARAM, 42, plms_0AF42 },
		{ FK_CMD_ASKPARAM, 43, plms_0AF43 },
		{ FK_CMD_ASKPARAM, 44, plms_0AF44 },
		{ FK_CMD_ASKPARAM, 45, plms_0AF45 },
		{ FK_CMD_ASKPARAM, 46, plms_0AF46 },
		{ FK_CMD_ASKPARAM, 47, plms_0AF47 },
		{ FK_CMD_ASKPARAM, 48, plms_0AF48 },
		{ FK_CMD_ASKPARAM, 49, plms_0AF49 },
		{ FK_CMD_ASKPARAM, 57, plms_0AF57 },
		{ FK_CMD_ASKPARAM, 58, plms_0AF58 },
		{ FK_CMD_ASKPARAM, 59, plms_0AF59 },
		{ FK_CMD_ASKPARAM, 60, plms_0AF60 },
		{ FK_CMD_ASKPARAM, 61, plms_0AF61 },
//	{ FK_CMD_ASKPARAM,	62,	plms_0AF62  },
//	{ FK_CMD_ASKPARAM,  63, plms_0AF63  },	// 江苏增补
		{ FK_CMD_ASKPARAM, 65, plms_0AF65 },
		{ FK_CMD_ASKPARAM, 66, plms_0AF66 },
		{ FK_CMD_ASKPARAM, 67, plms_0AF67 },
		{ FK_CMD_ASKPARAM, 68, plms_0AF68 },
		{ FK_CMD_ASKPARAM, 73, plms_0AF73 },
		{ FK_CMD_ASKPARAM, 74, plms_0AF74 },
		{ FK_CMD_ASKPARAM, 75, plms_0AF75 },
		{ FK_CMD_ASKPARAM, 76, plms_0AF76 },
		{ FK_CMD_ASKPARAM, 81, plms_0AF81 },
		{ FK_CMD_ASKPARAM, 82, plms_0AF82 },
		{ FK_CMD_ASKPARAM, 83, plms_0AF83 },
		{ FK_CMD_ASKPARAM, 85, plms_0AF85 },	// 江苏增补
//	{ FK_CMD_ASKPARAM,  91, plms_0AF91  },	// 江苏增补
		{ FK_CMD_ASKPARAM, 92, plms_0AF92 },	// 江苏增补
//	{ FK_CMD_ASKPARAM,  93, plms_0AF93  },	// 江苏增补
		{ FK_CMD_ASKPARAM, 169, plms_0AF169 }, // 自定义 通道类型
		{ FK_CMD_ASKPARAM, 170, plms_0AF170 }, // 自定义 载波方案
//	{ FK_CMD_ASKPARAM,  179, plms_0AF179  }, // 自定义 自动搜表时间
		{ FK_CMD_ASKPARAM, 111, plms_0AF111 }, // // 规约增补 集中器自维护定时任务参数
		{ FK_CMD_ASKPARAM, 112, plms_0AF112 }, // 规约增补 集器与电能表参数对应关系

//	{ FK_CMD_CLASS1,	 1,	plms_0CF01  },
		{ FK_CMD_CLASS1, 2, plms_0CF02 }, { FK_CMD_CLASS1, 3, plms_0CF03 }, {
				FK_CMD_CLASS1, 4, plms_0CF04 },
		{ FK_CMD_CLASS1, 5, plms_0CF05 }, { FK_CMD_CLASS1, 6, plms_0CF06 }, {
				FK_CMD_CLASS1, 7, plms_0CF07 },
		{ FK_CMD_CLASS1, 8, plms_0CF08 }, { FK_CMD_CLASS1, 9, plms_0CF09 },
		{ FK_CMD_CLASS1, 10, plms_0CF10 },	// 江苏增补（查询网络流量）
		{ FK_CMD_CLASS1, 11, plms_0CF11 },	// 江苏增补（查询网络流量）
		{ FK_CMD_CLASS1, 12, plms_0CF12 },	// 江苏增补（查询网络信号品质）
		{ FK_CMD_CLASS1, 16, plms_0CF16 },  // 江苏增补（查询中文信息）
		{ FK_CMD_CLASS1, 17, plms_0CF17 }, { FK_CMD_CLASS1, 18, plms_0CF18 }, {
				FK_CMD_CLASS1, 19, plms_0CF19 },
		{ FK_CMD_CLASS1, 20, plms_0CF20 }, { FK_CMD_CLASS1, 21, plms_0CF21 }, {
				FK_CMD_CLASS1, 22, plms_0CF22 },
		{ FK_CMD_CLASS1, 23, plms_0CF23 }, { FK_CMD_CLASS1, 24, plms_0CF24 }, {
				FK_CMD_CLASS1, 25, plms_0CF25 },
		{ FK_CMD_CLASS1, 26, plms_0CF26 }, { FK_CMD_CLASS1, 27, plms_0CF27 }, {
				FK_CMD_CLASS1, 28, plms_0CF28 },
		{ FK_CMD_CLASS1, 31, plms_0CF31 }, { FK_CMD_CLASS1, 32, plms_0CF32 }, {
				FK_CMD_CLASS1, 33, plms_0CF33 },
		{ FK_CMD_CLASS1, 34, plms_0CF34 }, { FK_CMD_CLASS1, 35, plms_0CF35 }, {
				FK_CMD_CLASS1, 36, plms_0CF36 },
		{ FK_CMD_CLASS1, 37, plms_0CF37 }, { FK_CMD_CLASS1, 38, plms_0CF38 }, {
				FK_CMD_CLASS1, 39, plms_0CF39 },
		{ FK_CMD_CLASS1, 40, plms_0CF40 }, { FK_CMD_CLASS1, 41, plms_0CF41 }, {
				FK_CMD_CLASS1, 42, plms_0CF42 },
		{ FK_CMD_CLASS1, 43, plms_0CF43 }, { FK_CMD_CLASS1, 44, plms_0CF44 }, {
				FK_CMD_CLASS1, 45, plms_0CF45 },
		{ FK_CMD_CLASS1, 46, plms_0CF46 }, { FK_CMD_CLASS1, 47, plms_0CF47 }, {
				FK_CMD_CLASS1, 48, plms_0CF48 },
		{ FK_CMD_CLASS1, 49, plms_0CF49 }, { FK_CMD_CLASS1, 57, plms_0CF57 }, {
				FK_CMD_CLASS1, 58, plms_0CF58 },
		{ FK_CMD_CLASS1, 65, plms_0CF65 }, { FK_CMD_CLASS1, 66, plms_0CF66 }, {
				FK_CMD_CLASS1, 67, plms_0CF67 },
		{ FK_CMD_CLASS1, 73, plms_0CF73 }, { FK_CMD_CLASS1, 81, plms_0CF81 }, {
				FK_CMD_CLASS1, 82, plms_0CF82 },
		{ FK_CMD_CLASS1, 83, plms_0CF83 }, { FK_CMD_CLASS1, 84, plms_0CF84 }, {
				FK_CMD_CLASS1, 89, plms_0CF89 },
		{ FK_CMD_CLASS1, 90, plms_0CF90 }, { FK_CMD_CLASS1, 91, plms_0CF91 }, {
				FK_CMD_CLASS1, 92, plms_0CF92 },
		{ FK_CMD_CLASS1, 93, plms_0CF93 }, { FK_CMD_CLASS1, 94, plms_0CF94 }, {
				FK_CMD_CLASS1, 95, plms_0CF95 },
		{ FK_CMD_CLASS1, 96, plms_0CF96 }, { FK_CMD_CLASS1, 97, plms_0CF97 }, {
				FK_CMD_CLASS1, 98, plms_0CF98 },
		{ FK_CMD_CLASS1, 99, plms_0CF99 }, { FK_CMD_CLASS1, 100, plms_0CF100 },
		{ FK_CMD_CLASS1, 101, plms_0CF101 },
		{ FK_CMD_CLASS1, 102, plms_0CF102 },
		{ FK_CMD_CLASS1, 103, plms_0CF103 },
		{ FK_CMD_CLASS1, 105, plms_0CF105 },
		{ FK_CMD_CLASS1, 106, plms_0CF106 },
		{ FK_CMD_CLASS1, 107, plms_0CF107 },
		{ FK_CMD_CLASS1, 108, plms_0CF108 },
		{ FK_CMD_CLASS1, 109, plms_0CF109 },
		{ FK_CMD_CLASS1, 110, plms_0CF110 },
		{ FK_CMD_CLASS1, 111, plms_0CF111 },
		{ FK_CMD_CLASS1, 112, plms_0CF112 },
		{ FK_CMD_CLASS1, 113, plms_0CF113 },
		{ FK_CMD_CLASS1, 114, plms_0CF114 },
		{ FK_CMD_CLASS1, 115, plms_0CF115 },
		{ FK_CMD_CLASS1, 116, plms_0CF116 },
		{ FK_CMD_CLASS1, 121, plms_0CF121 },

		/*zzd*/
		{ FK_CMD_CLASS1, 129, plms_0CF129 },
		{ FK_CMD_CLASS1, 130, plms_0CF130 },
		{ FK_CMD_CLASS1, 131, plms_0CF131 },
		{ FK_CMD_CLASS1, 132, plms_0CF132 },
		{ FK_CMD_CLASS1, 133, plms_0CF133 },
		{ FK_CMD_CLASS1, 134, plms_0CF134 },
		{ FK_CMD_CLASS1, 135, plms_0CF135 },
		{ FK_CMD_CLASS1, 136, plms_0CF136 },
		{ FK_CMD_CLASS1, 137, plms_0CF137 },
		{ FK_CMD_CLASS1, 138, plms_0CF138 },
		{ FK_CMD_CLASS1, 139, plms_0CF139 },
		{ FK_CMD_CLASS1, 140, plms_0CF140 },
		{ FK_CMD_CLASS1, 141, plms_0CF141 },
		{ FK_CMD_CLASS1, 142, plms_0CF142 },
		{ FK_CMD_CLASS1, 143, plms_0CF143 },
		{ FK_CMD_CLASS1, 144, plms_0CF144 },
		{ FK_CMD_CLASS1, 145, plms_0CF145 },
		{ FK_CMD_CLASS1, 146, plms_0CF146 },
		{ FK_CMD_CLASS1, 147, plms_0CF147 },
		{ FK_CMD_CLASS1, 148, plms_0CF148 },
		{ FK_CMD_CLASS1, 149, plms_0CF149 },
		{ FK_CMD_CLASS1, 150, plms_0CF150 },
		{ FK_CMD_CLASS1, 151, plms_0CF151 },
		{ FK_CMD_CLASS1, 152, plms_0CF152 },

		{ FK_CMD_CLASS1, 153, plms_0CF153 },
		{ FK_CMD_CLASS1, 154, plms_0CF154 },
		{ FK_CMD_CLASS1, 155, plms_0CF155 },
		{ FK_CMD_CLASS1, 156, plms_0CF156 },
		{ FK_CMD_CLASS1, 157, plms_0CF157 },
		{ FK_CMD_CLASS1, 158, plms_0CF158 },
		{ FK_CMD_CLASS1, 159, plms_0CF159 },
		{ FK_CMD_CLASS1, 160, plms_0CF160 },

		{ FK_CMD_CLASS1, 161, plms_0CF161 },

		{ FK_CMD_CLASS1, 165, plms_0CF165 },
		{ FK_CMD_CLASS1, 166, plms_0CF166 },
		{ FK_CMD_CLASS1, 167, plms_0CF167 },
		{ FK_CMD_CLASS1, 168, plms_0CF168 },
		{ FK_CMD_CLASS1, 169, plms_0CF169 },
		{ FK_CMD_CLASS1, 170, plms_0CF170 },
		{ FK_CMD_CLASS1, 180, plms_0CF180 },
//	{ FK_CMD_CLASS1,   181,	plms_0CF181 },
//    { FK_CMD_CLASS1,   182,	plms_0CF182 },
		{ FK_CMD_CLASS1, 186, plms_0CF186 }, ////规约增补 载波模块程序版本号
		{ FK_CMD_CLASS1, 240, plms_0CF240 }, // 终端启动时间
		{ FK_CMD_CLASS1, 241, plms_0CF241 }, // 终端抄表成功标识
		{ FK_CMD_CLASS1, 242, plms_0CF241 }, // 终端冻结成功标识
		//
		{ FK_CMD_CLASS2, 1, plms_0DF01 }, { FK_CMD_CLASS2, 2, plms_0DF02 }, {
				FK_CMD_CLASS2, 3, plms_0DF03 },
		{ FK_CMD_CLASS2, 4, plms_0DF04 }, { FK_CMD_CLASS2, 5, plms_0DF05 }, {
				FK_CMD_CLASS2, 6, plms_0DF06 },
		{ FK_CMD_CLASS2, 7, plms_0DF07 }, { FK_CMD_CLASS2, 8, plms_0DF08 }, {
				FK_CMD_CLASS2, 9, plms_0DF09 },
		{ FK_CMD_CLASS2, 10, plms_0DF10 }, { FK_CMD_CLASS2, 11, plms_0DF11 }, {
				FK_CMD_CLASS2, 12, plms_0DF12 },
		{ FK_CMD_CLASS2, 17, plms_0DF17 }, { FK_CMD_CLASS2, 18, plms_0DF18 }, {
				FK_CMD_CLASS2, 19, plms_0DF19 },
		{ FK_CMD_CLASS2, 20, plms_0DF20 }, { FK_CMD_CLASS2, 21, plms_0DF21 }, {
				FK_CMD_CLASS2, 22, plms_0DF22 },
		{ FK_CMD_CLASS2, 23, plms_0DF23 }, { FK_CMD_CLASS2, 24, plms_0DF24 }, {
				FK_CMD_CLASS2, 25, plms_0DF25 },
		{ FK_CMD_CLASS2, 26, plms_0DF26 }, { FK_CMD_CLASS2, 27, plms_0DF27 }, {
				FK_CMD_CLASS2, 28, plms_0DF28 },
		{ FK_CMD_CLASS2, 29, plms_0DF29 }, { FK_CMD_CLASS2, 30, plms_0DF30 }, {
				FK_CMD_CLASS2, 31, plms_0DF31 },
		{ FK_CMD_CLASS2, 32, plms_0DF32 }, { FK_CMD_CLASS2, 33, plms_0DF33 }, {
				FK_CMD_CLASS2, 34, plms_0DF34 },
		{ FK_CMD_CLASS2, 35, plms_0DF35 }, { FK_CMD_CLASS2, 36, plms_0DF36 }, {
				FK_CMD_CLASS2, 37, plms_0DF37 },
		{ FK_CMD_CLASS2, 38, plms_0DF38 }, { FK_CMD_CLASS2, 39, plms_0DF39 }, {
				FK_CMD_CLASS2, 41, plms_0DF41 },
		{ FK_CMD_CLASS2, 42, plms_0DF42 }, { FK_CMD_CLASS2, 43, plms_0DF43 }, {
				FK_CMD_CLASS2, 44, plms_0DF44 },
		{ FK_CMD_CLASS2, 49, plms_0DF49 }, { FK_CMD_CLASS2, 50, plms_0DF50 }, {
				FK_CMD_CLASS2, 51, plms_0DF51 },
		{ FK_CMD_CLASS2, 52, plms_0DF52 }, { FK_CMD_CLASS2, 53, plms_0DF53 }, {
				FK_CMD_CLASS2, 54, plms_0DF54 },
		{ FK_CMD_CLASS2, 57, plms_0DF57 }, { FK_CMD_CLASS2, 58, plms_0DF58 }, {
				FK_CMD_CLASS2, 59, plms_0DF59 },
		{ FK_CMD_CLASS2, 60, plms_0DF60 }, { FK_CMD_CLASS2, 61, plms_0DF61 }, {
				FK_CMD_CLASS2, 62, plms_0DF62 },
		{ FK_CMD_CLASS2, 65, plms_0DF65 }, { FK_CMD_CLASS2, 66, plms_0DF66 }, {
				FK_CMD_CLASS2, 73, plms_0DF73 },
		{ FK_CMD_CLASS2, 74, plms_0DF74 }, { FK_CMD_CLASS2, 75, plms_0DF75 }, {
				FK_CMD_CLASS2, 76, plms_0DF76 },
		{ FK_CMD_CLASS2, 81, plms_0DF81 }, { FK_CMD_CLASS2, 82, plms_0DF82 }, {
				FK_CMD_CLASS2, 83, plms_0DF83 },
		{ FK_CMD_CLASS2, 84, plms_0DF84 }, { FK_CMD_CLASS2, 85, plms_0DF85 }, {
				FK_CMD_CLASS2, 86, plms_0DF86 },
		{ FK_CMD_CLASS2, 87, plms_0DF87 }, { FK_CMD_CLASS2, 88, plms_0DF88 }, {
				FK_CMD_CLASS2, 89, plms_0DF89 },
		{ FK_CMD_CLASS2, 90, plms_0DF90 }, { FK_CMD_CLASS2, 91, plms_0DF91 }, {
				FK_CMD_CLASS2, 92, plms_0DF92 },
		{ FK_CMD_CLASS2, 93, plms_0DF93 }, { FK_CMD_CLASS2, 94, plms_0DF94 }, {
				FK_CMD_CLASS2, 95, plms_0DF95 },
		{ FK_CMD_CLASS2, 97, plms_0DF97 }, { FK_CMD_CLASS2, 98, plms_0DF98 }, {
				FK_CMD_CLASS2, 99, plms_0DF99 }, { FK_CMD_CLASS2, 100,
				plms_0DF100 }, { FK_CMD_CLASS2, 101, plms_0DF101 }, {
				FK_CMD_CLASS2, 102, plms_0DF102 }, { FK_CMD_CLASS2, 103,
				plms_0DF103 }, { FK_CMD_CLASS2, 104, plms_0DF104 }, {
				FK_CMD_CLASS2, 105, plms_0DF105 }, { FK_CMD_CLASS2, 106,
				plms_0DF106 }, { FK_CMD_CLASS2, 107, plms_0DF107 }, {
				FK_CMD_CLASS2, 108, plms_0DF108 }, { FK_CMD_CLASS2, 109,
				plms_0DF109 }, { FK_CMD_CLASS2, 110, plms_0DF110 }, {
				FK_CMD_CLASS2, 113, plms_0DF113 }, { FK_CMD_CLASS2, 114,
				plms_0DF114 }, { FK_CMD_CLASS2, 115, plms_0DF115 }, {
				FK_CMD_CLASS2, 116, plms_0DF116 }, { FK_CMD_CLASS2, 117,
				plms_0DF117 }, { FK_CMD_CLASS2, 118, plms_0DF118 }, {
				FK_CMD_CLASS2, 121, plms_0DF121 }, { FK_CMD_CLASS2, 122,
				plms_0DF122 }, { FK_CMD_CLASS2, 123, plms_0DF123 }, {
				FK_CMD_CLASS2, 129, plms_0DF129 }, { FK_CMD_CLASS2, 130,
				plms_0DF130 }, { FK_CMD_CLASS2, 138, plms_0DF138 },

		/*-zzd-*/
		{ FK_CMD_CLASS2, 145, plms_0DF145 },
		{ FK_CMD_CLASS2, 146, plms_0DF146 },
		{ FK_CMD_CLASS2, 147, plms_0DF147 },
		{ FK_CMD_CLASS2, 148, plms_0DF148 },

		{ FK_CMD_CLASS2, 153, plms_0DF153 },
		{ FK_CMD_CLASS2, 154, plms_0DF154 },
		{ FK_CMD_CLASS2, 155, plms_0DF155 },
		{ FK_CMD_CLASS2, 156, plms_0DF156 },
		{ FK_CMD_CLASS2, 157, plms_0DF157 },
		{ FK_CMD_CLASS2, 158, plms_0DF158 },
		{ FK_CMD_CLASS2, 159, plms_0DF159 },
		{ FK_CMD_CLASS2, 160, plms_0DF160 },

		{ FK_CMD_CLASS2, 161, plms_0DF161 },
		{ FK_CMD_CLASS2, 162, plms_0DF162 },
		{ FK_CMD_CLASS2, 163, plms_0DF163 },
		{ FK_CMD_CLASS2, 164, plms_0DF164 },
		{ FK_CMD_CLASS2, 165, plms_0DF165 },
		{ FK_CMD_CLASS2, 166, plms_0DF166 },
		{ FK_CMD_CLASS2, 167, plms_0DF167 },
		{ FK_CMD_CLASS2, 168, plms_0DF168 },

		{ FK_CMD_CLASS2, 169, plms_0DF169 },
		{ FK_CMD_CLASS2, 170, plms_0DF170 },
		{ FK_CMD_CLASS2, 171, plms_0DF171 },
		{ FK_CMD_CLASS2, 172, plms_0DF172 },
		{ FK_CMD_CLASS2, 173, plms_0DF173 },
		{ FK_CMD_CLASS2, 174, plms_0DF174 },
		{ FK_CMD_CLASS2, 175, plms_0DF175 },
		{ FK_CMD_CLASS2, 176, plms_0DF176 },

		{ FK_CMD_CLASS2, 177, plms_0DF177 },
		{ FK_CMD_CLASS2, 178, plms_0DF178 },
		{ FK_CMD_CLASS2, 179, plms_0DF179 },
		{ FK_CMD_CLASS2, 180, plms_0DF180 },
		{ FK_CMD_CLASS2, 181, plms_0DF181 },
		{ FK_CMD_CLASS2, 182, plms_0DF182 },
		{ FK_CMD_CLASS2, 183, plms_0DF183 },
		{ FK_CMD_CLASS2, 184, plms_0DF184 },

		{ FK_CMD_CLASS2, 185, plms_0DF185 },
		{ FK_CMD_CLASS2, 186, plms_0DF186 },
		{ FK_CMD_CLASS2, 187, plms_0DF187 },
		{ FK_CMD_CLASS2, 188, plms_0DF188 },
		{ FK_CMD_CLASS2, 189, plms_0DF189 },
		{ FK_CMD_CLASS2, 190, plms_0DF190 },
		{ FK_CMD_CLASS2, 191, plms_0DF191 },
		{ FK_CMD_CLASS2, 192, plms_0DF192 },

		{ FK_CMD_CLASS2, 193, plms_0DF193 },
		{ FK_CMD_CLASS2, 194, plms_0DF194 },
		{ FK_CMD_CLASS2, 195, plms_0DF195 },
		{ FK_CMD_CLASS2, 196, plms_0DF196 },

		{ FK_CMD_CLASS2, 201, plms_0DF201 },
		{ FK_CMD_CLASS2, 202, plms_0DF202 },
		{ FK_CMD_CLASS2, 203, plms_0DF203 },
		{ FK_CMD_CLASS2, 204, plms_0DF204 },
		{ FK_CMD_CLASS2, 205, plms_0DF205 },
		{ FK_CMD_CLASS2, 206, plms_0DF206 },
		{ FK_CMD_CLASS2, 207, plms_0DF207 },
		{ FK_CMD_CLASS2, 208, plms_0DF208 },

		{ FK_CMD_CLASS2, 209, plms_0DF209 },

		{ FK_CMD_CLASS2, 213, plms_0DF213 },
		{ FK_CMD_CLASS2, 214, plms_0DF214 },
		{ FK_CMD_CLASS2, 215, plms_0DF215 },
		{ FK_CMD_CLASS2, 216, plms_0DF216 },
//	{ FK_CMD_CLASS2,   217,	plms_0DF217 },
//	{ FK_CMD_CLASS2,   218,	plms_0DF218 },
		//
		{ FK_CMD_CLASS3, 1, plms_ERC1 }, { FK_CMD_CLASS3, 2, plms_ERC2 }, {
				FK_CMD_CLASS3, 3, plms_ERC3 }, { FK_CMD_CLASS3, 4, plms_ERC4 },
		{ FK_CMD_CLASS3, 5, plms_ERC5 }, { FK_CMD_CLASS3, 6, plms_ERC6 }, {
				FK_CMD_CLASS3, 7, plms_ERC7 }, { FK_CMD_CLASS3, 8, plms_ERC8 },
		{ FK_CMD_CLASS3, 9, plms_ERC9 }, { FK_CMD_CLASS3, 10, plms_ERC10 }, {
				FK_CMD_CLASS3, 11, plms_ERC11 },
		{ FK_CMD_CLASS3, 12, plms_ERC12 }, { FK_CMD_CLASS3, 13, plms_ERC13 }, {
				FK_CMD_CLASS3, 14, plms_ERC14 },
		{ FK_CMD_CLASS3, 15, plms_ERC15 }, { FK_CMD_CLASS3, 16, plms_ERC16 }, {
				FK_CMD_CLASS3, 17, plms_ERC17 },
		{ FK_CMD_CLASS3, 18, plms_ERC18 }, { FK_CMD_CLASS3, 19, plms_ERC19 }, {
				FK_CMD_CLASS3, 20, plms_ERC20 },
		{ FK_CMD_CLASS3, 21, plms_ERC21 }, { FK_CMD_CLASS3, 22, plms_ERC22 }, {
				FK_CMD_CLASS3, 23, plms_ERC23 },
		{ FK_CMD_CLASS3, 24, plms_ERC24 }, { FK_CMD_CLASS3, 25, plms_ERC25 }, {
				FK_CMD_CLASS3, 26, plms_ERC26 },
		{ FK_CMD_CLASS3, 27, plms_ERC27 }, { FK_CMD_CLASS3, 28, plms_ERC28 }, {
				FK_CMD_CLASS3, 29, plms_ERC29 },
		{ FK_CMD_CLASS3, 30, plms_ERC30 }, { FK_CMD_CLASS3, 31, plms_ERC31 }, {
				FK_CMD_CLASS3, 32, plms_ERC32 },
		{ FK_CMD_CLASS3, 33, plms_ERC33 }, { FK_CMD_CLASS3, 34, plms_ERC34 }, {
				FK_CMD_CLASS3, 35, plms_ERC35 },
		{ FK_CMD_CLASS3, 36, plms_ERC36 }, };

static const FMT2* FindEx(BYTE afn, BYTE fn) {
	int nCount = sizeof(fmt2) / sizeof(FMT2);
	for (int i = 0; i < nCount; ++i) {
		if (fmt2[i].afn == afn && fmt2[i].fn == fn) {
			return &fmt2[i];
		}
	}
	return nullptr;
}

int plms_fmt_dt(BYTE afn, WORD dt, BYTE* pTran, void* pAppl) {
	BYTE fn = plms_cvt_fn(dt);
	return plms_fmt_fn(afn, fn, pTran, pAppl);
}

int plms_fmt_fn(BYTE afn, BYTE fn, BYTE* pTran, void* pAppl) {
	const FMT2* ptr = FindEx(afn, fn);
	if (ptr != nullptr) {
		return ptr->pf(pTran, pAppl);
	}
	return 0;
}

} /* namespace pcols */
