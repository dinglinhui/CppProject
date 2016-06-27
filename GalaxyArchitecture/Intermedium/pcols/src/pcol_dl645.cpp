/*
 * cildl645.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include <string.h>
#include <ctypes.h>
#include "pcol_dl645.h"

namespace pcols {

static BYTE FA_CheckSum(BYTE* pBuff, int nLen) {
	BYTE cs = 0x00;
	int nPos = 0;

	if (pBuff == nullptr)
		return cs;
	for (nPos = 0; nPos < nLen; ++nPos) {
		cs += pBuff[nPos];
	}
	return cs;
}

int DL645_ReadData(DL645_FRAME *pFrame, BYTE host[6], WORD di) {
	if (pFrame == nullptr)
		return -1;
	if (pFrame->data == nullptr)
		return -1;

	memcpy(pFrame->hdr.rtua, host, 6);
	pFrame->hdr.code.mask.ACD = 0;
	pFrame->hdr.code.mask.DIR = 0;
	pFrame->hdr.code.mask.SERIES = 0;
	pFrame->hdr.code.mask.CMD = EM_CMD_READDATA;

	memcpy(pFrame->data, &di, sizeof(WORD));
	pFrame->length = sizeof(WORD);

	return 0;
}

int DL645_07_ReadData(DL645_FRAME *pFrame, BYTE host[6], DWORD di) {
	if (pFrame == nullptr)
		return -1;
	if (pFrame->data == nullptr)
		return -1;

	memcpy(pFrame->hdr.rtua, host, 6);
	pFrame->hdr.code.mask.ACD = 0;
	pFrame->hdr.code.mask.DIR = 0;
	pFrame->hdr.code.mask.SERIES = 0;
	pFrame->hdr.code.mask.CMD = 0x11; //////不同于645规约

	memcpy(pFrame->data, &di, sizeof(DWORD));
	pFrame->length = sizeof(DWORD);

	return 0;
}

int DL645_CheckTime(DL645_FRAME *pFrame, time_t hTime) {
	if (pFrame == nullptr)
		return -1;
	if (pFrame->data == nullptr)
		return -1;

	for (int i = 0; i < 6; ++i) {
		pFrame->hdr.rtua[i] = 0x99;
	}

	pFrame->hdr.code.mask.ACD = 0;
	pFrame->hdr.code.mask.DIR = 0;
	pFrame->hdr.code.mask.SERIES = 0;
	pFrame->hdr.code.mask.CMD = EM_CMD_CHECKTIME;

	tm* tm_sys = localtime(&hTime);

	int nPos = 0;
	hex2bcd(tm_sys->tm_sec, &pFrame->data[nPos++]);
	hex2bcd(tm_sys->tm_min, &pFrame->data[nPos++]);
	hex2bcd(tm_sys->tm_hour, &pFrame->data[nPos++]);
	hex2bcd(tm_sys->tm_mday, &pFrame->data[nPos++]);
	hex2bcd(tm_sys->tm_mon, &pFrame->data[nPos++]);
	hex2bcd((BYTE) (tm_sys->tm_year % 100), &pFrame->data[nPos++]);
	pFrame->length = nPos;
	return 0;
}

DL645_HDR DL645_Respond(DL645_HDR hdr) {
	DL645_HDR dst;
	memcpy(&dst, &hdr, sizeof(DL645_HDR));
	dst.code.mask.DIR = (!hdr.code.mask.DIR);
	return dst;
}

DL645_HDR DL645_RespondError(DL645_HDR hdr) {
	DL645_HDR dst;
	memcpy(&dst, &hdr, sizeof(DL645_HDR));
	dst.code.mask.DIR = (!hdr.code.mask.DIR);
	dst.code.mask.ACD = 1;
	return dst;
}

int DL645_IsValid(BYTE *lpBuf, int nSize) {
	if (lpBuf == nullptr)
		return -1;

	// 缓冲区长度不足,无有效帧
	if (nSize < 12)
		return -1;

	int nPos = 0;

	// 无有效帧头
	if (lpBuf[nPos] != 0x68)
		return -2;
	nPos += 7;
	if (lpBuf[nPos] != 0x68)
		return -2;
	nPos++;

	// 命令字符
	nPos++;

	// 缓冲区数据长度不足,无有效帧
	int nLen = lpBuf[nPos];
	if (nSize < (nLen + 12))
		return -1;
	nPos++;

	// 数据域
	nPos += nLen;

	// 校验和
	BYTE sum = FA_CheckSum(lpBuf, nPos);
	if (sum != lpBuf[nPos])
		return -2;
	nPos++;

	// 帧尾
	if (lpBuf[nPos] != 0x16)
		return -1;
	nPos++;

	return nPos;
}

int DL645_MakeFrame(DL645_FRAME *pFrm, BYTE *lpBuf, int nSize) {
	if (pFrm == nullptr)
		return 0;
	if (lpBuf == nullptr)
		return 0;
	if ((pFrm->length + 12) > nSize)
		return 0;

	int nPos = 0;
	lpBuf[nPos++] = 0x68;
	memcpy(&lpBuf[nPos], pFrm->hdr.rtua, 6);
	nPos += 6;
	lpBuf[nPos++] = 0x68;
	lpBuf[nPos++] = pFrm->hdr.code.body;
	lpBuf[nPos++] = pFrm->length;
	memcpy(&lpBuf[nPos], pFrm->data, pFrm->length);

	for (int i = 0; i < pFrm->length; ++i) {
//		if( lpBuf[nPos+i] <= 0xCC )
		{
			lpBuf[nPos + i] += 0x33;
		}
	}

	nPos += pFrm->length;

	BYTE sum = FA_CheckSum(&lpBuf[0], nPos);
	lpBuf[nPos++] = sum;
	lpBuf[nPos++] = 0x16;

	return nPos;
}

int DL645_GetFrame(DL645_FRAME *pFrm, BYTE *lpBuf, int nSize) {
	if (pFrm == nullptr)
		return -1;
	if (pFrm->data == nullptr)
		return -1;

	int nPos = 0;
	int nRet = 0;
	while (nPos < nSize) {
		nRet = DL645_IsValid(&lpBuf[nPos], (nSize - nPos));
		if (nRet == -2) {
			nPos++;
			continue;
		}

		if (nRet == -1) {
			nPos = -1;
			break;
		}

		memcpy(&pFrm->hdr.rtua, &lpBuf[nPos + 1], 6);
		pFrm->hdr.code.body = lpBuf[nPos + 8];
		pFrm->length = lpBuf[nPos + 9];
		pFrm->data = &lpBuf[nPos + 10];

		for (int i = 0; i < pFrm->length; ++i) {
//			if( pFrm->data[i] >= 0x33 )
			{
				pFrm->data[i] -= 0x33;
			}
		}

		nPos += nRet;
		break;
	}

	return nPos;
}

/******************************************************************
 *
 *	数据转换
 *
 *******************************************************************/

#ifdef DL645_FIXED
#include "cil_cvt_fixed_dl645.inl"
#else
#include "cil_cvt_float_dl645.inl"
#endif

const DL645_DATA s_DL645[] = { { 0x9010, 14, 4, sizeof(EM_DI901X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x901F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI901F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9020, 14, 4, sizeof(EM_DI902X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x902F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI902F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9110, 14, 4, sizeof(EM_DI911X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x911F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI911F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9120, 14, 4, sizeof(EM_DI912X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x912F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI912F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9130, 14, 4, sizeof(EM_DI913X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x913F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI913F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9140, 14, 4, sizeof(EM_DI914X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x914F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI914F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9150, 14, 4, sizeof(EM_DI915X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x915F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI915F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9160, 14, 4, sizeof(EM_DI916X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x916F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI916F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9410, 14, 4, sizeof(EM_DI941X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x941F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI941F), DL645_Encode_9xxF,
		DL645_Decode_9xxF }, { 0x9420, 14, 4, sizeof(EM_DI942X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x942F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI942F), DL645_Encode_9xxF,
		DL645_Decode_9xxF },

{ 0x9510, 14, 4, sizeof(EM_DI951X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x951F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI951F), DL645_Encode_9xxF, DL645_Decode_9xxF }, { 0x9520, 14, 4,
		sizeof(EM_DI952X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x952F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI952F), DL645_Encode_9xxF, DL645_Decode_9xxF },

{ 0x9530, 14, 4, sizeof(EM_DI953X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x953F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI953F), DL645_Encode_9xxF, DL645_Decode_9xxF }, { 0x9540, 14, 4,
		sizeof(EM_DI954X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x954F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI954F), DL645_Encode_9xxF, DL645_Decode_9xxF },

{ 0x9550, 14, 4, sizeof(EM_DI955X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x955F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI955F), DL645_Encode_9xxF, DL645_Decode_9xxF }, { 0x9560, 14, 4,
		sizeof(EM_DI956X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x956F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI956F), DL645_Encode_9xxF, DL645_Decode_9xxF },

{ 0x9810, 14, 4, sizeof(EM_DI981X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x981F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI981F), DL645_Encode_9xxF, DL645_Decode_9xxF }, { 0x9820, 14, 4,
		sizeof(EM_DI982X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x982F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI982F), DL645_Encode_9xxF, DL645_Decode_9xxF },
//
		{ 0xA010, 14, 3, sizeof(EM_DIA01X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA01F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA01F), DL645_Encode_AxxF, DL645_Decode_AxxF }, {
				0xA020, 14, 3, sizeof(EM_DIA02X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA02F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA02F), DL645_Encode_AxxF, DL645_Decode_AxxF },

		{ 0xA110, 14, 3, sizeof(EM_DIA01X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA11F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA01F), DL645_Encode_AxxF, DL645_Decode_AxxF }, {
				0xA120, 14, 3, sizeof(EM_DIA02X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA12F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA02F), DL645_Encode_AxxF, DL645_Decode_AxxF },

		{ 0xA410, 14, 3, sizeof(EM_DIA41X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA41F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA41F), DL645_Encode_AxxF, DL645_Decode_AxxF }, {
				0xA420, 14, 3, sizeof(EM_DIA42X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA42F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA42F), DL645_Encode_AxxF, DL645_Decode_AxxF },

		{ 0xA510, 14, 3, sizeof(EM_DIA51X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA51F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA51F), DL645_Encode_AxxF, DL645_Decode_AxxF }, {
				0xA520, 14, 3, sizeof(EM_DIA52X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA52F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA52F), DL645_Encode_AxxF, DL645_Decode_AxxF },

		{ 0xA810, 14, 3, sizeof(EM_DIA81X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA81F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA81F), DL645_Encode_AxxF, DL645_Decode_AxxF }, {
				0xA820, 14, 3, sizeof(EM_DIA82X), DL645_Encode_Axxx, DL645_Decode_Axxx }, { 0xA82F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIA82F), DL645_Encode_AxxF, DL645_Decode_AxxF },
		//
		{ 0xB010, 14, 4, sizeof(EM_DIB01X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB01F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB01F), DL645_Encode_BxxF, DL645_Decode_BxxF }, {
				0xB020, 14, 4, sizeof(EM_DIB02X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB02F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB02F), DL645_Encode_BxxF, DL645_Decode_BxxF },

		{ 0xB110, 14, 4, sizeof(EM_DIB01X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB11F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB01F), DL645_Encode_BxxF, DL645_Decode_BxxF }, {
				0xB120, 14, 4, sizeof(EM_DIB02X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB12F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB02F), DL645_Encode_BxxF, DL645_Decode_BxxF },

		{ 0xB410, 14, 4, sizeof(EM_DIB41X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB41F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB41F), DL645_Encode_BxxF, DL645_Decode_BxxF }, {
				0xB420, 14, 4, sizeof(EM_DIB42X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB42F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB42F), DL645_Encode_BxxF, DL645_Decode_BxxF },

		{ 0xB510, 14, 4, sizeof(EM_DIB51X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB51F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB51F), DL645_Encode_BxxF, DL645_Decode_BxxF }, {
				0xB520, 14, 4, sizeof(EM_DIB52X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB52F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB52F), DL645_Encode_BxxF, DL645_Decode_BxxF },

		{ 0xB810, 14, 4, sizeof(EM_DIB81X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB81F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB81F), DL645_Encode_BxxF, DL645_Decode_BxxF }, {
				0xB820, 14, 4, sizeof(EM_DIB82X), DL645_Encode_Bxxx, DL645_Decode_Bxxx }, { 0xB82F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DIB82F), DL645_Encode_BxxF, DL645_Decode_BxxF },
		//
		{ 0xB210, 0, 4, sizeof(EM_DIB210), DL645_Encode_B210, DL645_Decode_B210 }, { 0xB211, 0, 4, sizeof(EM_DIB211), DL645_Encode_B211, DL645_Decode_B211 }, { 0xB212, 0, 2, sizeof(EM_DIB212),
				DL645_Encode_B212, DL645_Decode_B212 }, { 0xB213, 0, 2, sizeof(EM_DIB213), DL645_Encode_B213, DL645_Decode_B213 }, { 0xB214, 0, 3, sizeof(EM_DIB214), DL645_Encode_B214,
				DL645_Decode_B214 }, { 0xB21F, 0, 15, sizeof(EM_DIB21F), DL645_Encode_B21F, DL645_Decode_B21F }, { 0xB310, 3, 2, sizeof(EM_DIB310), DL645_Encode_B310, DL645_Decode_B310 }, { 0xB31F, 0,
				8, sizeof(EM_DIB31F), DL645_Encode_B31F, DL645_Decode_B31F }, { 0xB320, 3, 3, sizeof(EM_DIB320), DL645_Encode_B320, DL645_Decode_B320 }, { 0xB32F, 0, 12, sizeof(EM_DIB32F),
				DL645_Encode_B32F, DL645_Decode_B32F }, { 0xB330, 3, 4, sizeof(EM_DIB330), DL645_Encode_B330, DL645_Decode_B330 }, { 0xB33F, 0, 16, sizeof(EM_DIB33F), DL645_Encode_B33F,
				DL645_Decode_B33F }, { 0xB34F, 0, 16, sizeof(EM_DIB34F), DL645_Encode_B33F, DL645_Decode_B33F }, { 0xB611, 2, 2, sizeof(EM_DIB611), DL645_Encode_B611, DL645_Decode_B611 }, { 0xB61F, 0,
				6, sizeof(EM_DIB61F), DL645_Encode_B61F, DL645_Decode_B61F }, { 0xB621, 2, 2, sizeof(EM_DIB621), DL645_Encode_B621, DL645_Decode_B621 }, { 0xB62F, 0, 6, sizeof(EM_DIB62F),
				DL645_Encode_B62F, DL645_Decode_B62F }, { 0xB630, 3, 3, sizeof(EM_DIB630), DL645_Encode_B630, DL645_Decode_B630 }, { 0xB634, 1, 2, sizeof(EM_DIB634), DL645_Encode_B634,
				DL645_Decode_B634 }, { 0xB63F, 0, 12, sizeof(EM_DIB63F), DL645_Encode_B63F, DL645_Decode_B63F }, { 0xB640, 3, 2, sizeof(EM_DIB640), DL645_Encode_B640, DL645_Decode_B640 }, { 0xB64F, 0,
				8, sizeof(EM_DIB64F), DL645_Encode_B64F, DL645_Decode_B64F }, { 0xB650, 3, 2, sizeof(EM_DIB650), DL645_Encode_B650, DL645_Decode_B650 }, { 0xB65F, 0, 8, sizeof(EM_DIB65F),
				DL645_Encode_B65F, DL645_Decode_B65F }, { 0xB660, 5, 2, sizeof(EM_DIB660), DL645_Encode_B660, DL645_Decode_B660 }, { 0xB66F, 0, 12, sizeof(EM_DIB66F), DL645_Encode_B66F,
				DL645_Decode_B66F },
		//
		{ 0xC010, 0, 4, sizeof(EM_DIC010), DL645_Encode_C010, DL645_Decode_C010 }, { 0xC011, 0, 3, sizeof(EM_DIC011), DL645_Encode_C011, DL645_Decode_C011 }, { 0xC01F, 0, 7, sizeof(EM_DIC01F),
				DL645_Encode_C01F, DL645_Decode_C01F }, { 0xC020, 0, 1, sizeof(EM_DIC020), DL645_Encode_C020, DL645_Decode_C020 }, { 0xC021, 0, 1, sizeof(EM_DIC021), DL645_Encode_C021,
				DL645_Decode_C021 }, { 0xC022, 0, 1, sizeof(EM_DIC022), DL645_Encode_C022, DL645_Decode_C022 }, { 0xC02F, 0, 3, sizeof(EM_DIC02F), DL645_Encode_C02F, DL645_Decode_C02F }, { 0xC030, 0,
				3, sizeof(EM_DIC030), DL645_Encode_C030, DL645_Decode_C030 }, { 0xC031, 0, 3, sizeof(EM_DIC031), DL645_Encode_C031, DL645_Decode_C031 }, { 0xC032, 0, 6, sizeof(EM_DIC032),
				DL645_Encode_C032, DL645_Decode_C032 }, { 0xC033, 0, 6, sizeof(EM_DIC033), DL645_Encode_C033, DL645_Decode_C033 }, { 0xC034, 0, 6, sizeof(EM_DIC034), DL645_Encode_C034,
				DL645_Decode_C034 }, { 0xC03F, 0, 24, sizeof(EM_DIC03F), DL645_Encode_C03F, DL645_Decode_C03F }, { 0xC111, 0, 1, sizeof(EM_DIC111), DL645_Encode_C111, DL645_Decode_C111 }, { 0xC112, 0,
				1, sizeof(EM_DIC112), DL645_Encode_C112, DL645_Decode_C112 }, { 0xC113, 0, 1, sizeof(EM_DIC113), DL645_Encode_C113, DL645_Decode_C113 }, { 0xC114, 0, 1, sizeof(EM_DIC114),
				DL645_Encode_C114, DL645_Decode_C114 }, { 0xC115, 0, 1, sizeof(EM_DIC115), DL645_Encode_C115, DL645_Decode_C115 }, { 0xC116, 0, 1, sizeof(EM_DIC116), DL645_Encode_C116,
				DL645_Decode_C116 }, { 0xC117, 0, 2, sizeof(EM_DIC117), DL645_Encode_C117, DL645_Decode_C117 }, { 0xC118, 0, 1, sizeof(EM_DIC118), DL645_Encode_C118, DL645_Decode_C118 }, { 0xC119, 0,
				4, sizeof(EM_DIC119), DL645_Encode_C119, DL645_Decode_C119 }, { 0xC11A, 0, 4, sizeof(EM_DIC11A), DL645_Encode_C11A, DL645_Decode_C11A }, { 0xC211, 0, 2, sizeof(EM_DIC211),
				DL645_Encode_C211, DL645_Decode_C211 }, { 0xC212, 0, 4, sizeof(EM_DIC212), DL645_Encode_C212, DL645_Decode_C212 }, { 0xC310, 0, 1, sizeof(EM_DIC310), DL645_Encode_C310,
				DL645_Decode_C310 }, { 0xC311, 0, 1, sizeof(EM_DIC311), DL645_Encode_C311, DL645_Decode_C311 }, { 0xC312, 0, 1, sizeof(EM_DIC312), DL645_Encode_C312, DL645_Decode_C312 }, { 0xC313, 0,
				1, sizeof(EM_DIC313), DL645_Encode_C313, DL645_Decode_C313 }, { 0xC314, 0, 1, sizeof(EM_DIC314), DL645_Encode_C314, DL645_Decode_C314 }, { 0xC321, 13, 3, sizeof(EM_DIC32P),
				DL645_Encode_C32P, DL645_Decode_C32P }, { 0xC331, 13, 3, sizeof(EM_DIC3QM), DL645_Encode_C3QM, DL645_Decode_C3QM }, { 0xC33F, 0, 24, sizeof(EM_DIC3QF), DL645_Encode_C3QF,
				DL645_Decode_C3QF }, { 0xC341, 13, 3, sizeof(EM_DIC3QM), DL645_Encode_C3QM, DL645_Decode_C3QM }, { 0xC351, 13, 3, sizeof(EM_DIC3QM), DL645_Encode_C3QM, DL645_Decode_C3QM }, { 0xC361,
				13, 3, sizeof(EM_DIC3QM), DL645_Encode_C3QM, DL645_Decode_C3QM }, { 0xC371, 13, 3, sizeof(EM_DIC3QM), DL645_Encode_C3QM, DL645_Decode_C3QM }, { 0xC381, 13, 3, sizeof(EM_DIC3QM),
				DL645_Encode_C3QM, DL645_Decode_C3QM }, { 0xC391, 13, 3, sizeof(EM_DIC3QM), DL645_Encode_C3QM, DL645_Decode_C3QM }, { 0xC3A1, 13, 3, sizeof(EM_DIC3QM), DL645_Encode_C3QM,
				DL645_Decode_C3QM }, { 0xC411, 12, 3, sizeof(EM_DIC41N), DL645_Encode_C41N, DL645_Decode_C41N }, { 0xC41E, 0, 1, sizeof(EM_DIC41E), DL645_Encode_C41E, DL645_Decode_C41E }, { 0xC510, 0,
				4, sizeof(EM_DIC510), DL645_Encode_C510, DL645_Decode_C510 }, { 0xC511, 0, 2, sizeof(EM_DIC511), DL645_Encode_C511, DL645_Decode_C511 }, { 0xF010, 14, 4, sizeof(EM_DI901X), nullptr,
				DL645_Decode_Fxxx }, //载波扩充
		{ 0xF01F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI901F), nullptr, DL645_Decode_FxxF },	//载波扩充
		//鄂电表增加

		/*Start of muyue on 2012-2-28 18:34 V1.1*/
		{ 0xD120, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI914F), DL645_Encode_9xxF, DL645_Decode_9xxF }, { 0x610F, 0, (LINYANG_EM_MAX_TARIFFNUM + 1) * 4, sizeof(EM_DI914F),
				DL645_Encode_9xxF, DL645_Decode_9xxF },
		/*End of muyue on 2012-2-28 18:34 V1.1*/
		{ 0xE207, 0, 0, sizeof(EM_DIE207), DL645_Encode_E207, DL645_Decode_E207 },	//7号数据包
		{ 0xE209, 0, 0, sizeof(EM_DIE209), DL645_Encode_E209, DL645_Decode_E209 },	//9号数据包
		{ 0xE211, 0, 0, sizeof(EM_DIE211), DL645_Encode_E211, DL645_Decode_E211 },	//10号数据包
		{ 0xE221, 0, 0, sizeof(EM_DIE221), DL645_Encode_E221, DL645_Decode_E221 },	//11号数据包
		{ 0xE400, 0, 4, sizeof(EM_DI901X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0xE401, 0, 4, sizeof(EM_DI901X), DL645_Encode_9xxx, DL645_Decode_9xxx },	//分相
		{ 0xE402, 0, 4, sizeof(EM_DI901X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0xE403, 0, 4, sizeof(EM_DI901X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0xE420, 0, 4, sizeof(EM_DI901X),
				DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0xE421, 0, 4, sizeof(EM_DI901X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0xE422, 0, 4, sizeof(EM_DI901X), DL645_Encode_9xxx,
				DL645_Decode_9xxx }, { 0xE423, 0, 4, sizeof(EM_DI901X), DL645_Encode_9xxx, DL645_Decode_9xxx },

		{ 0xE410, 0, 4, sizeof(EM_DI911X), DL645_Encode_9xxx, DL645_Decode_9xxx },	//无功总
		{ 0xE411, 0, 4, sizeof(EM_DI911X), DL645_Encode_9xxx, DL645_Decode_9xxx },	//无功总
		{ 0xE412, 0, 4, sizeof(EM_DI911X), DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0xE430, 0, 4, sizeof(EM_DI911X), DL645_Encode_9xxx, DL645_Decode_9xxx },

		//安徽电表增加
		{ 0xD40B, 0, 4, sizeof(EM_DID40B), DL645_Encode_D40B, DL645_Decode_D40B }, //最近一次开盖、端钮时间
		{ 0xD40D, 0, 4, sizeof(EM_DID40D), DL645_Encode_D40D, DL645_Decode_D40D }, //累计开盖、端钮次数
		{ 0xD415, 0, 4, sizeof(EM_DID415), DL645_Encode_D415, DL645_Decode_D415 }, //时段表切换及标志
		//END
		};

static const DL645_DATA s_DL645_2007[] = {
// 需量
		{ 0x01010000, 0, 8, 0, nullptr, DL645_07_Decode_AB00 }, { 0x0101ff00, 0, 8 * 5, 0, nullptr, DL645_07_Decode_ABxxx }, { 0x01010001, 0, 8, 0, nullptr, DL645_07_Decode_AB00 }, { 0x0101ff01, 0, 8
				* 5, 0, nullptr, DL645_07_Decode_ABxxx }, { 0x01020000, 0, 8, 0, nullptr, DL645_07_Decode_AB00 }, { 0x0102ff00, 0, 8 * 5, 0, nullptr, DL645_07_Decode_ABxxx }, { 0x01020001, 0, 8, 0,
				nullptr, DL645_07_Decode_AB00 }, { 0x0102ff01, 0, 8 * 5, 0, nullptr, DL645_07_Decode_ABxxx }, { 0x01030000, 0, 8, 0, nullptr, DL645_07_Decode_AB00 }, { 0x0103ff00, 0, 8 * 5, 0,
				nullptr, DL645_07_Decode_ABxxx }, { 0x01030001, 0, 8, 0, nullptr, DL645_07_Decode_AB00 }, { 0x0103ff01, 0, 8 * 5, 0, nullptr, DL645_07_Decode_ABxxx }, { 0x01040000, 0, 8, 0, nullptr,
				DL645_07_Decode_AB00 }, { 0x0104ff00, 0, 8 * 5, 0, nullptr, DL645_07_Decode_ABxxx }, { 0x01040001, 0, 8, 0, nullptr, DL645_07_Decode_AB00 }, { 0x0104ff01, 0, 8 * 5, 0, nullptr,
				DL645_07_Decode_ABxxx },
		// 电压
		{ 0x02010100, 3, 2, sizeof(EM_DIB611), DL645_Encode_B611, DL645_Decode_B611 }, { 0x0201ff00, 0, 6, sizeof(EM_DIB61F), DL645_Encode_B61F, DL645_Decode_B61F },
		// 电流
		{ 0x02020100, 3, 3, sizeof(EM_DIB621), DL645_07_Encode_B621, DL645_07_Decode_B621 }, { 0x0202ff00, 0, 9, sizeof(EM_DIB62F), DL645_07_Encode_B62F, DL645_07_Decode_B62F }, { 0x02800001, 0, 3,
				sizeof(EM_DIB621), DL645_07_Encode_B621, DL645_07_Decode_B621 }, // 南网零序电流
		// 功率
		{ 0x02030000, 4, 3, sizeof(EM_DIB630), DL645_07_Encode_B630, DL645_07_Decode_B630 }, { 0x0203ff00, 0, 12, sizeof(EM_DIB63F), DL645_07_Encode_B63F, DL645_07_Decode_B63F }, { 0x02040000, 4, 3,
				sizeof(EM_DIB640), DL645_07_Encode_B630, DL645_07_Decode_B630 }, { 0x0204ff00, 0, 12, sizeof(EM_DIB64F), DL645_07_Encode_B63F, DL645_07_Decode_B63F }, { 0x02050000, 4, 3,
				sizeof(EM_DIB640), DL645_07_Encode_B630, DL645_07_Decode_B630 }, { 0x0205ff00, 0, 12, sizeof(EM_DIB64F), DL645_07_Encode_B63F, DL645_07_Decode_B63F },
		// 相角
		{ 0x0207ff00, 0, 6, sizeof(EM_DIB66F), DL645_07_Encode_B66F, DL645_07_Decode_B66F },
		// 清零次数, 时间
		{ 0x03300000, 0, 3, sizeof(EM_DIB212_07), DL645_07_Encode_B212, DL645_07_Decode_B212 }, { 0x03300001, 0, 6, sizeof(EM_DIB210_07), DL645_07_Encode_B210, DL645_07_Decode_B210 }, { 0x03300100, 0,
				3, sizeof(EM_DIB212_07), DL645_07_Encode_B212, DL645_07_Decode_B212 }, { 0x03300101, 0, 6, sizeof(EM_DIB210_07), DL645_07_Encode_B210, DL645_07_Decode_B210 }, { 0x03300200, 0, 3,
				sizeof(EM_DIB212_07), DL645_07_Encode_B212, DL645_07_Decode_B212 }, { 0x03300201, 0, 6, sizeof(EM_DIB210_07), DL645_07_Encode_B210, DL645_07_Decode_B210 }, { 0x03300300, 0, 3,
				sizeof(EM_DIB212_07), DL645_07_Encode_B212, DL645_07_Decode_B212 }, { 0x03300301, 0, 6, sizeof(EM_DIB210_07), DL645_07_Encode_B210, DL645_07_Decode_B210 }, { 0x03300400, 0, 3,
				sizeof(EM_DIB212_07), DL645_07_Encode_B212, DL645_07_Decode_B212 }, { 0x03300401, 0, 6, sizeof(EM_DIB210_07), DL645_07_Encode_0401, DL645_07_Decode_0401 }, { 0x03300500, 0, 3,
				sizeof(EM_DIB212_07), DL645_07_Encode_B212, DL645_07_Decode_B212 }, { 0x03300501, 0, 6, sizeof(EM_DIB212_07), DL645_07_Encode_B210, DL645_07_Decode_B210 },
		// 断相
		{ 0x03040000, 0, 18, 0, nullptr, DL645_07_Decode_03040000 }, { 0x03040101, 0, 18, 0, nullptr, DL645_07_Decode_03040101 }, { 0x03040201, 0, 18, 0, nullptr, DL645_07_Decode_03040101 }, {
				0x03040301, 0, 18, 0, nullptr, DL645_07_Decode_03040101 },
		//断相备案文件中
		{ 0x13010001, 0, 18, 0, nullptr, DL645_07_Decode_13010001 }, { 0x13010002, 0, 18, 0, nullptr, DL645_07_Decode_13010002 }, { 0x13020001, 0, 18, 0, nullptr, DL645_07_Decode_13010001 }, {
				0x13020002, 0, 18, 0, nullptr, DL645_07_Decode_13010002 }, { 0x13030001, 0, 18, 0, nullptr, DL645_07_Decode_13010001 }, { 0x13030002, 0, 18, 0, nullptr, DL645_07_Decode_13010002 },

		{ 0x13010101, 0, 18, 0, nullptr, DL645_07_Decode_13010101 }, { 0x13012501, 0, 18, 0, nullptr, DL645_07_Decode_13012501 }, { 0x13020101, 0, 18, 0, nullptr, DL645_07_Decode_13010101 }, {
				0x13022501, 0, 18, 0, nullptr, DL645_07_Decode_13012501 }, { 0x13030101, 0, 18, 0, nullptr, DL645_07_Decode_13010101 }, { 0x13032501, 0, 18, 0, nullptr, DL645_07_Decode_13012501 },

		// 费率
		{ 0xC331, 13, 3, sizeof(EM_DIC3QM), DL645_Encode_C3QM, DL645_Decode_C3QM }, { 0xC33F, 0, 24, sizeof(EM_DIC3QF), DL645_Encode_C3QF, DL645_Decode_C3QF },
		// 脉冲常数
//	{0x04000409, 0,  3, 0, nullptr, DL645_07_Decode_C119},
//	{0x0400040A, 0,  3, 0, nullptr, DL645_07_Decode_C119},

		{ 0x04000409, 0, 3, sizeof(EM_DIC030), DL645_Encode_C030, DL645_Decode_C030 }, { 0x0400040A, 0, 3, sizeof(EM_DIC031), DL645_Encode_C031, DL645_Decode_C031 },

		// 电表状态字
		{ 0x040005ff, 0, 7, 0, nullptr, DL645_07_Decode_C020 },

		// 日冻结数据
//	{0x05060001, 0, nullptr, DL645_07_Decode_04060001},
		{ 0x05060101, 0, 20, 0, nullptr, DL645_Decode_9xxF }, { 0x05060201, 0, 20, 0, nullptr, DL645_Decode_9xxF }, { 0x05060301, 0, 20, 0, nullptr, DL645_Decode_9xxF }, { 0x05060401, 0, 20, 0,
				nullptr, DL645_Decode_9xxF }, { 0x05060501, 0, 20, 0, nullptr, DL645_Decode_9xxF }, { 0x05060601, 0, 20, 0, nullptr, DL645_Decode_9xxF }, { 0x05060701, 0, 20, 0, nullptr,
				DL645_Decode_9xxF }, { 0x05060801, 0, 20, 0, nullptr, DL645_Decode_9xxF },

		// 分相电能
		{ 0x9070, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9080, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9170, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9180, 0, 4,
				0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9071, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9081, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9171, 0, 4, 0,
				DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9181, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9072, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9082, 0, 4, 0,
				DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9172, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9182, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx },
		//
		{ 0x9470, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9480, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9570, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9580, 0, 4,
				0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9471, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9481, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9571, 0, 4, 0,
				DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9581, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9472, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9482, 0, 4, 0,
				DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9572, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0x9582, 0, 4, 0, DL645_Encode_9xxx, DL645_Decode_9xxx }, { 0xB214, 0, 3,
				sizeof(EM_DIB214), DL645_Encode_B214, DL645_Decode_B214 }, };

int GetLenofs_DL645(void) {
	int nCount = sizeof(s_DL645) / sizeof(DL645_DATA);
	return nCount;
}

int DL645_GetPackLength(WORD di) {
	int nCount = sizeof(s_DL645) / sizeof(DL645_DATA);
	for (int i = 0; i < nCount; ++i) {
		if (di >= s_DL645[i].DI && di <= (s_DL645[i].Plus + s_DL645[i].DI)) {
			return s_DL645[i].nPackLen;
		}
	}
	return 0;
}

int DL645_07_GetPackLength(DWORD di) {
	int nCount = sizeof(s_DL645_2007) / sizeof(DL645_DATA);
	for (int i = 0; i < nCount; ++i) {
		if (di >= s_DL645_2007[i].DI && di <= (s_DL645_2007[i].Plus + s_DL645_2007[i].DI)) {
			return s_DL645_2007[i].nPackLen;
		}
	}
	return DL645_GetPackLength(di);
}

int DL645_GetDataLength(WORD di) {
	int nCount = sizeof(s_DL645) / sizeof(DL645_DATA);
	for (int i = 0; i < nCount; ++i) {
		if (di >= s_DL645[i].DI && di <= (s_DL645[i].Plus + s_DL645[i].DI)) {
			return s_DL645[i].nDataLen;
		}
	}
	return 0;
}

int DL645_Encode(WORD di, BYTE *pTran, void *pAppl) {
	int nCount = sizeof(s_DL645) / sizeof(DL645_DATA);
	for (int i = 0; i < nCount; ++i) {
		if (di >= s_DL645[i].DI && di <= (s_DL645[i].Plus + s_DL645[i].DI)) {
			return s_DL645[i].pfEncode(pTran, pAppl);
		}
	}
	return 0;
}

int DL645_07_Encode(DWORD di, BYTE *pTran, void *pAppl) {
	int nCount = sizeof(s_DL645_2007) / sizeof(DL645_DATA);
	for (int i = 0; i < nCount; ++i) {
		if (di == s_DL645_2007[i].DI) {
			if (s_DL645_2007[i].pfEncode != nullptr) {
				return s_DL645_2007[i].pfEncode(pTran, pAppl);
			} else {
				return 0;
			}
		}
	}

	return DL645_Encode(di, pTran, pAppl);
}

int DL645_Decode(WORD di, void *pAppl, BYTE *pTran) {
	int nCount = sizeof(s_DL645) / sizeof(DL645_DATA);
	for (int i = 0; i < nCount; ++i) {
		if ((di >= s_DL645[i].DI) && (di <= (s_DL645[i].Plus + s_DL645[i].DI))) {
			return s_DL645[i].pfDecode(pAppl, pTran);
		}
	}
	return 0;
}

int DL645_07_Decode(DWORD di, void *pAppl, BYTE *pTran) {
	int nCount = sizeof(s_DL645_2007) / sizeof(DL645_DATA);
	for (int i = 0; i < nCount; ++i) {
		if (di == s_DL645_2007[i].DI) {
			if (s_DL645_2007[i].pfDecode != nullptr) {
				return s_DL645_2007[i].pfDecode(pAppl, pTran);
			} else {
				return 0;
			}
		}
	}

	return DL645_Decode(di, pAppl, pTran);
}

} /* namespace pcols */
