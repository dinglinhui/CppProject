/*
 * pcol_edmi.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "pcol_edmi.h"
#include <string.h>

namespace pcols {

static unsigned short GetSequence() {
	static unsigned short i = 0x00;
	++i;
	return i;
}

static short gencrc_16(short i) {
	short j;
	short k;
	short crc;

	k = i << 8;
	crc = 0;

	for (j = 0; j < 8; j++) {
		if ((crc ^ k) & 0x8000)
			crc = (crc << 1) ^ 0x1021;
		else
			crc <<= 1;
		k <<= 1;
	}

	return (crc);
}

static unsigned short CalculateCharacterCRC16(unsigned short crc,
		unsigned char c) {
	return ((crc << 8) ^ gencrc_16((crc >> 8) ^ c));
}

static int Reverse(BYTE *lpBuf, BYTE *lpSrc, int nSize) {
	int nIndx = 0;
	for (int i = 0; i < nSize; ++i) {
		if (lpSrc[i] == MKX_STX || lpSrc[i] == MKX_ETX || lpSrc[i] == MKX_XON
				|| lpSrc[i] == MKX_XOFF || lpSrc[i] == MKX_DLE) {
			lpBuf[nIndx++] = MKX_DLE;
			lpBuf[nIndx++] = (lpSrc[i] + 0x40);
		} else {
			lpBuf[nIndx++] = lpSrc[i];
		}
	}
	return nIndx;
}

static DWORD MKX_ConvertHost(BYTE host[6]) {
	DWORD dwValue = host[0] & 0x0F;
	dwValue += 10 * (host[0] >> 4);
	dwValue += 100 * (host[1] & 0x0F);
	dwValue += 1000 * (host[1] >> 4);
	dwValue += 10000 * (host[2] & 0x0F);
	dwValue += 100000 * (host[2] >> 4);
	dwValue += 1000000 * (host[3] & 0x0F);
	dwValue += 10000000 * (host[3] >> 4);
	dwValue += 100000000 * (host[4] & 0x0F);
	dwValue += 1000000000 * (host[4] >> 4);

	return dwValue;
}

static int FindFlagPos(BYTE *lpBuf, int nSize, int &nS, int &nE) {
	int nStart = -1;
	int nEnd = -1;

	for (int i = 0; i < nSize; ++i) {
		if (lpBuf[i] == MKX_STX) {
			nStart = i;
			break;
		}
	}

	for (int j = nStart + 1; j < nSize; ++j) {
		if (lpBuf[j] == MKX_ETX) {
			nEnd = j;
			break;
		}
	}

	if (nStart >= 0 && nEnd >= 0) {
		nS = nStart;
		nE = nEnd;
		return 0;
	}

	return -1;
}

static int CheckBuffIsValid(BYTE *lpBuf, int nSize) {
	unsigned short crc = 0;

	for (int i = 0; i < nSize; ++i) {
		crc = CalculateCharacterCRC16(crc, lpBuf[i]);
	}

	if (crc == 0) {
		return 0;
	}
	return -1;
}

static int __Reverse(BYTE *lpBuf, BYTE *lpSrc, int nSize) {
	int nIndx = 0;
	for (int i = 0; i < nSize; ++i) {
		if (lpSrc[i] == MKX_STX || lpSrc[i] == MKX_ETX || lpSrc[i] == MKX_XON
				|| lpSrc[i] == MKX_XOFF || lpSrc[i] == MKX_DLE) {
			i++;
			lpBuf[nIndx++] = (lpSrc[i] - 0x40);
		} else {
			lpBuf[nIndx++] = lpSrc[i];
		}
	}
	return nIndx;
}

int MKX_MakeFrame(MKX_FRAME *frm, BYTE *lpBuf, int /* nBufSize */) {
	if (frm == NULL)
		return 0;
	if (frm->data == NULL)
		return 0;
	if (lpBuf == NULL)
		return 0;

	unsigned short crc = 0;
	unsigned short seq = GetSequence();

	int nIndx = 0;
	lpBuf[nIndx++] = 0xFF;
	lpBuf[nIndx++] = MKX_STX;

	int nPos = 0;
	BYTE buff[128] = { 0 };
	buff[nPos++] = MKX_STX;
	buff[nPos++] = 'E';

	DWORD dwHost = MKX_ConvertHost(frm->hdr.dest);
	buff[nPos++] = HIBYTE(HIWORD(dwHost));
	buff[nPos++] = LOBYTE(HIWORD(dwHost));
	buff[nPos++] = HIBYTE(LOWORD(dwHost));
	buff[nPos++] = LOBYTE(LOWORD(dwHost));

	buff[nPos++] = 0x00;
	buff[nPos++] = 0x00;
	buff[nPos++] = 0x00;
	buff[nPos++] = 0x01;

	buff[nPos++] = (BYTE) HIBYTE(seq);
	buff[nPos++] = (BYTE) LOBYTE(seq);

	memcpy(&buff[nPos], frm->data, frm->hdr.length);
	nPos += frm->hdr.length;

	for (int i = 0; i < nPos; ++i) {
		crc = CalculateCharacterCRC16(crc, buff[i]);
	}

	buff[nPos++] = (BYTE) HIBYTE(crc);
	buff[nPos++] = (BYTE) LOBYTE(crc);

	int nSize = Reverse(&lpBuf[nIndx], &buff[1], nPos - 1);
	nIndx += nSize;

	lpBuf[nIndx++] = MKX_ETX;

	return nIndx;
}

int MKX_Access(MKX_FRAME *frm) {
	frm->hdr.length = 0;
	return 0;
}

int MKX_Logon(MKX_FRAME *frm, const char* szId, const char* szPwd) {
	int nPos = 0;

	frm->data[nPos++] = 'L';
	memcpy(&frm->data[nPos], szId, strlen(szId));
	nPos += strlen(szId);
	frm->data[nPos++] = ',';
	memcpy(&frm->data[nPos], szPwd, strlen(szPwd));
	nPos += strlen(szPwd);
	frm->data[nPos++] = 0;

	frm->hdr.length = nPos;
	return 0;
}

int MKX_Logoff(MKX_FRAME *frm) {
	int nPos = 0;
	frm->data[nPos++] = 'X';
	frm->hdr.length = nPos;
	return 0;
}

int MKX_Read(MKX_FRAME *frm, DWORD reg_num) {
	int nPos = 0;

	WORD lo_reg = LOWORD(reg_num);

	frm->data[nPos++] = 'R';
	frm->data[nPos++] = (BYTE) HIBYTE(lo_reg);
	frm->data[nPos++] = (BYTE) LOBYTE(lo_reg);

	frm->hdr.length = nPos;
	return 0;
}

int MKX_GetFrame(MKX_FRAME *frm, BYTE *lpBuf, int nSize) {
	int nStart = 0;
	int nEnd = 0;
	int nRet = FindFlagPos(lpBuf, nSize, nStart, nEnd);
	if (nRet == 0) {
		BYTE buff[128] = { 0 };
		buff[0] = lpBuf[nStart];
		BYTE length = __Reverse(&buff[1], &lpBuf[nStart + 1],
				(nEnd - nStart - 1));
		length++;

		nRet = CheckBuffIsValid(buff, length);

		if (nRet == 0) {
			// 去掉起始符和'E'
			int nPos = 2;

			// 上位机地址
			memcpy(frm->hdr.srce, &buff[nPos], 4);
			nPos += 4;

			// 表计地址
			memcpy(frm->hdr.dest, &buff[nPos], 4);
			nPos += 4;

			// 帧序号
			nPos += 2;

			// 功能码
			frm->hdr.code = buff[nPos++];

			switch (frm->hdr.code) {
			case MKX_ACK:
			case MKX_NAK:
			case MKX_CAN:
				frm->hdr.length = 0;
				break;

			case 'R':
				frm->hdr.length = MKX_Convert(frm->data, &buff[nPos]);
				break;
			}
		}
	}

	return nRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Convert
//
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef int (*pfConvert)(BYTE *, BYTE *);

typedef struct tag_MKX_MAP {
	WORD reg;
	WORD di;
	pfConvert pf;
} MKX_MAP;

static int MKX_Convertfloat2WORD(BYTE *lpBuf, BYTE *lpSrc) {
	BYTE buff[4] = { 0 };
	buff[0] = lpSrc[3];
	buff[1] = lpSrc[2];
	buff[2] = lpSrc[1];
	buff[3] = lpSrc[0];

	float value = 0.0;
	memcpy(&value, buff, 4);

	WORD wd = (WORD) value;
	memcpy(lpBuf, &wd, sizeof(WORD));

	return sizeof(WORD);
}

static int MKX_Convertfloat2double(BYTE *lpBuf, BYTE *lpSrc) {
	BYTE buff[4] = { 0 };
	buff[0] = lpSrc[3];
	buff[1] = lpSrc[2];
	buff[2] = lpSrc[1];
	buff[3] = lpSrc[0];

	float value = 0.0;
	memcpy(&value, buff, 4);

	double dbVal = (double) value;
	memcpy(lpBuf, &dbVal, sizeof(double));

	return sizeof(double);
}

static int MKX_Convertfloat2kdouble(BYTE *lpBuf, BYTE *lpSrc) {
	BYTE buff[4] = { 0 };
	buff[0] = lpSrc[3];
	buff[1] = lpSrc[2];
	buff[2] = lpSrc[1];
	buff[3] = lpSrc[0];

	float value = 0.0;
	memcpy(&value, buff, 4);

	double dbVal = ((double) value) / 1000;
	memcpy(lpBuf, &dbVal, sizeof(double));

	return sizeof(double);
}

static int MKX_Convertdouble2kdouble(BYTE *lpBuf, BYTE *lpSrc) {
	BYTE buff[8] = { 0 };
	buff[0] = lpSrc[7];
	buff[1] = lpSrc[6];
	buff[2] = lpSrc[5];
	buff[3] = lpSrc[4];
	buff[4] = lpSrc[3];
	buff[5] = lpSrc[2];
	buff[6] = lpSrc[1];
	buff[7] = lpSrc[0];

	double value = 0.0;
	memcpy(&value, buff, sizeof(double));

	value /= 1000;
	memcpy(lpBuf, &value, sizeof(double));

	return sizeof(double);
}

static int MKX_ConvertToDate(BYTE *lpBuf, BYTE *lpSrc) {
	lpBuf[0] = lpSrc[2];
	lpBuf[1] = lpSrc[1];
	lpBuf[2] = lpSrc[0];
	lpBuf[3] = 0;
	return 4;
}

static int MKX_ConvertToTime(BYTE *lpBuf, BYTE *lpSrc) {
	lpBuf[0] = lpSrc[0];
	lpBuf[1] = lpSrc[1];
	lpBuf[2] = lpSrc[2];
	return 3;
}

static int MKX_ConvertToDateTime(BYTE *lpBuf, BYTE *lpSrc) {
	lpBuf[0] = lpSrc[1];	// month
	lpBuf[1] = lpSrc[0];	// day
	lpBuf[2] = lpSrc[3];	// hour
	lpBuf[3] = lpSrc[4];	// minute
	return 4;
}

static MKX_MAP ls_mapMKX[] = { { 0xE000, 0xB611, MKX_Convertfloat2WORD },// A相电压
		{ 0xE001, 0xB612, MKX_Convertfloat2WORD },	// B相电压
		{ 0xE002, 0xB613, MKX_Convertfloat2WORD },	// C相电压

		{ 0xE010, 0xB621, MKX_Convertfloat2double },	// A相电流
		{ 0xE011, 0xB622, MKX_Convertfloat2double },	// B相电流
		{ 0xE012, 0xB623, MKX_Convertfloat2double },	// C相电流

		{ 0xE027, 0xB663, MKX_Convertfloat2double },	// Ia相位角
		{ 0xE028, 0xB664, MKX_Convertfloat2double },	// Ib相位角
		{ 0xE029, 0xB665, MKX_Convertfloat2double },	// Ic相位角

		{ 0xE02A, 0xB660, MKX_Convertfloat2double },	// Uab/Ua相位角
		{ 0xE02B, 0xB661, MKX_Convertfloat2double },	// Ub相位角
		{ 0xE02C, 0xB662, MKX_Convertfloat2double },	// Ucb/Uc相位角

		{ 0xE030, 0xB631, MKX_Convertfloat2kdouble },	// A相有功功率
		{ 0xE031, 0xB632, MKX_Convertfloat2kdouble },	// B相有功功率
		{ 0xE032, 0xB633, MKX_Convertfloat2kdouble },	// C相有功功率
		{ 0xE033, 0xB630, MKX_Convertfloat2kdouble },	// 总瞬时有功功率

		{ 0xE040, 0xB641, MKX_Convertfloat2kdouble },	// A相无功功率
		{ 0xE041, 0xB642, MKX_Convertfloat2kdouble },	// B相无功功率
		{ 0xE042, 0xB643, MKX_Convertfloat2kdouble },	// C相无功功率
		{ 0xE043, 0xB640, MKX_Convertfloat2kdouble },	// 总瞬时无功功率

		{ 0xE026, 0xB650, MKX_Convertfloat2double },	// 总功率因素

		{ 0xF010, 0xC010, MKX_ConvertToDate },	// 日期
		{ 0xF011, 0xC011, MKX_ConvertToTime },	// 时间

		/////////////////////////////////////////////////////////////
		// 当前电量数据
		/////////////////////////////////////////////////////////////

		{ 0x0060, 0x9021, MKX_Convertfloat2kdouble },	// (当前)费率1反向有功总电能
		{ 0x0061, 0x9022, MKX_Convertfloat2kdouble },	// (当前)费率2反向有功总电能
		{ 0x0062, 0x9023, MKX_Convertfloat2kdouble },	// (当前)费率3反向有功总电能
		{ 0x0063, 0x9024, MKX_Convertfloat2kdouble },	// (当前)费率4反向有功总电能
		{ 0x0069, 0x9020, MKX_Convertfloat2kdouble },	// (当前)反向有功总电能

		{ 0x0160, 0x9011, MKX_Convertfloat2kdouble },	// (当前)费率1正向有功总电能
		{ 0x0161, 0x9012, MKX_Convertfloat2kdouble },	// (当前)费率2正向有功总电能
		{ 0x0162, 0x9013, MKX_Convertfloat2kdouble },	// (当前)费率3正向有功总电能
		{ 0x0163, 0x9014, MKX_Convertfloat2kdouble },	// (当前)费率4正向有功总电能
		{ 0x0169, 0x9010, MKX_Convertfloat2kdouble },	// (当前)正向有功总电能

		{ 0x0260, 0x9121, MKX_Convertfloat2kdouble },	// (当前)费率1反向无功总电能
		{ 0x0261, 0x9122, MKX_Convertfloat2kdouble },	// (当前)费率2反向无功总电能
		{ 0x0262, 0x9123, MKX_Convertfloat2kdouble },	// (当前)费率3反向无功总电能
		{ 0x0263, 0x9124, MKX_Convertfloat2kdouble },	// (当前)费率4反向无功总电能
		{ 0x0269, 0x9120, MKX_Convertfloat2kdouble },	// (当前)反向无功总电能

		{ 0x0360, 0x9111, MKX_Convertfloat2kdouble },	// (当前)费率1正向无功总电能
		{ 0x0361, 0x9112, MKX_Convertfloat2kdouble },	// (当前)费率2正向无功总电能
		{ 0x0362, 0x9113, MKX_Convertfloat2kdouble },	// (当前)费率3正向无功总电能
		{ 0x0363, 0x9114, MKX_Convertfloat2kdouble },	// (当前)费率4正向无功总电能
		{ 0x0369, 0x9110, MKX_Convertfloat2kdouble },	// (当前)正向无功总电能

		{ 0x0660, 0x9131, MKX_Convertfloat2kdouble },	// (当前)费率1一象限无功总电能
		{ 0x0661, 0x9132, MKX_Convertfloat2kdouble },	// (当前)费率2一象限无功总电能
		{ 0x0662, 0x9133, MKX_Convertfloat2kdouble },	// (当前)费率3一象限无功总电能
		{ 0x0663, 0x9134, MKX_Convertfloat2kdouble },	// (当前)费率4一象限无功总电能
		{ 0x0669, 0x9130, MKX_Convertfloat2kdouble },	// (当前)一象限无功总电能

		{ 0x0760, 0x9151, MKX_Convertfloat2kdouble },	// (当前)费率1二象限无功总电能
		{ 0x0761, 0x9152, MKX_Convertfloat2kdouble },	// (当前)费率2二象限无功总电能
		{ 0x0762, 0x9153, MKX_Convertfloat2kdouble },	// (当前)费率3二象限无功总电能
		{ 0x0763, 0x9154, MKX_Convertfloat2kdouble },	// (当前)费率4二象限无功总电能
		{ 0x0769, 0x9150, MKX_Convertfloat2kdouble },	// (当前)二象限无功总电能

		{ 0x0860, 0x9161, MKX_Convertfloat2kdouble },	// (当前)费率1三象限无功总电能
		{ 0x0861, 0x9162, MKX_Convertfloat2kdouble },	// (当前)费率2三象限无功总电能
		{ 0x0862, 0x9163, MKX_Convertfloat2kdouble },	// (当前)费率3三象限无功总电能
		{ 0x0863, 0x9164, MKX_Convertfloat2kdouble },	// (当前)费率4三象限无功总电能
		{ 0x0869, 0x9160, MKX_Convertfloat2kdouble },	// (当前)三象限无功总电能

		{ 0x0960, 0x9141, MKX_Convertfloat2kdouble },	// (当前)费率1四象限无功总电能
		{ 0x0961, 0x9142, MKX_Convertfloat2kdouble },	// (当前)费率2四象限无功总电能
		{ 0x0962, 0x9143, MKX_Convertfloat2kdouble },	// (当前)费率3四象限无功总电能
		{ 0x0963, 0x9144, MKX_Convertfloat2kdouble },	// (当前)费率4四象限无功总电能
		{ 0x0969, 0x9140, MKX_Convertfloat2kdouble },	// (当前)四象限无功总电能

		/////////////////////////////////////////////////////////////
		// 上月电量数据
		/////////////////////////////////////////////////////////////

		{ 0x0040, 0x9421, MKX_Convertfloat2kdouble },	// (上月)费率1反向有功总电能
		{ 0x0041, 0x9422, MKX_Convertfloat2kdouble },	// (上月)费率2反向有功总电能
		{ 0x0042, 0x9423, MKX_Convertfloat2kdouble },	// (上月)费率3反向有功总电能
		{ 0x0043, 0x9424, MKX_Convertfloat2kdouble },	// (上月)费率4反向有功总电能
		{ 0x0049, 0x9420, MKX_Convertfloat2kdouble },	// (上月)反向有功总电能

		{ 0x0140, 0x9411, MKX_Convertfloat2kdouble },	// (上月)费率1正向有功总电能
		{ 0x0141, 0x9412, MKX_Convertfloat2kdouble },	// (上月)费率2正向有功总电能
		{ 0x0142, 0x9413, MKX_Convertfloat2kdouble },	// (上月)费率3正向有功总电能
		{ 0x0143, 0x9414, MKX_Convertfloat2kdouble },	// (上月)费率4正向有功总电能
		{ 0x0149, 0x9410, MKX_Convertfloat2kdouble },	// (上月)正向有功总电能

		{ 0x0640, 0x9531, MKX_Convertfloat2kdouble },	// (上月)费率1一象限无功总电能
		{ 0x0641, 0x9532, MKX_Convertfloat2kdouble },	// (上月)费率2一象限无功总电能
		{ 0x0642, 0x9533, MKX_Convertfloat2kdouble },	// (上月)费率3一象限无功总电能
		{ 0x0643, 0x9534, MKX_Convertfloat2kdouble },	// (上月)费率4一象限无功总电能
		{ 0x0649, 0x9530, MKX_Convertfloat2kdouble },	// (上月)一象限无功总电能

		{ 0x0740, 0x9551, MKX_Convertfloat2kdouble },	// (上月)费率1二象限无功总电能
		{ 0x0741, 0x9552, MKX_Convertfloat2kdouble },	// (上月)费率2二象限无功总电能
		{ 0x0742, 0x9553, MKX_Convertfloat2kdouble },	// (上月)费率3二象限无功总电能
		{ 0x0743, 0x9554, MKX_Convertfloat2kdouble },	// (上月)费率4二象限无功总电能
		{ 0x0749, 0x9550, MKX_Convertfloat2kdouble },	// (上月)二象限无功总电能

		{ 0x0840, 0x9561, MKX_Convertfloat2kdouble },	// (上月)费率1三象限无功总电能
		{ 0x0841, 0x9562, MKX_Convertfloat2kdouble },	// (上月)费率2三象限无功总电能
		{ 0x0842, 0x9563, MKX_Convertfloat2kdouble },	// (上月)费率3三象限无功总电能
		{ 0x0843, 0x9564, MKX_Convertfloat2kdouble },	// (上月)费率4三象限无功总电能
		{ 0x0849, 0x9560, MKX_Convertfloat2kdouble },	// (上月)三象限无功总电能

		{ 0x0940, 0x9541, MKX_Convertfloat2kdouble },	// (上月)费率1四象限无功总电能
		{ 0x0941, 0x9542, MKX_Convertfloat2kdouble },	// (上月)费率2四象限无功总电能
		{ 0x0942, 0x9543, MKX_Convertfloat2kdouble },	// (上月)费率3四象限无功总电能
		{ 0x0943, 0x9544, MKX_Convertfloat2kdouble },	// (上月)费率4四象限无功总电能
		{ 0x0949, 0x9540, MKX_Convertfloat2kdouble },	// (上月)四象限无功总电能

		/////////////////////////////////////////////////////////////
		// 当前最大需量
		/////////////////////////////////////////////////////////////

		{ 0x1000, 0xA021, MKX_Convertfloat2kdouble },	// (当前)费率1反向有功总最大需量
		{ 0x1001, 0xA022, MKX_Convertfloat2kdouble },	// (当前)费率2反向有功总最大需量
		{ 0x1002, 0xA023, MKX_Convertfloat2kdouble },	// (当前)费率3反向有功总最大需量
		{ 0x1003, 0xA024, MKX_Convertfloat2kdouble },	// (当前)费率4反向有功总最大需量
		{ 0x1009, 0xA020, MKX_Convertfloat2kdouble },	// (当前)反向有功总最大需量

		{ 0x1100, 0xA011, MKX_Convertfloat2kdouble },	// (当前)费率1正向有功总最大需量
		{ 0x1101, 0xA012, MKX_Convertfloat2kdouble },	// (当前)费率2正向有功总最大需量
		{ 0x1102, 0xA013, MKX_Convertfloat2kdouble },	// (当前)费率3正向有功总最大需量
		{ 0x1103, 0xA014, MKX_Convertfloat2kdouble },	// (当前)费率4正向有功总最大需量
		{ 0x1109, 0xA010, MKX_Convertfloat2kdouble },	// (当前)正向有功总最大需量

		/////////////////////////////////////////////////////////////
		// 上月最大需量
		/////////////////////////////////////////////////////////////

		{ 0x1010, 0xA421, MKX_Convertfloat2kdouble },	// (上月)费率1反向有功总最大需量
		{ 0x1011, 0xA422, MKX_Convertfloat2kdouble },	// (上月)费率2反向有功总最大需量
		{ 0x1012, 0xA423, MKX_Convertfloat2kdouble },	// (上月)费率3反向有功总最大需量
		{ 0x1013, 0xA424, MKX_Convertfloat2kdouble },	// (上月)费率4反向有功总最大需量
		{ 0x1019, 0xA420, MKX_Convertfloat2kdouble },	// (上月)反向有功总最大需量

		{ 0x1110, 0xA411, MKX_Convertfloat2kdouble },	// (上月)费率1正向有功总最大需量
		{ 0x1111, 0xA412, MKX_Convertfloat2kdouble },	// (上月)费率2正向有功总最大需量
		{ 0x1112, 0xA413, MKX_Convertfloat2kdouble },	// (上月)费率3正向有功总最大需量
		{ 0x1113, 0xA414, MKX_Convertfloat2kdouble },	// (上月)费率4正向有功总最大需量
		{ 0x1119, 0xA410, MKX_Convertfloat2kdouble },	// (上月)正向有功总最大需量

		/////////////////////////////////////////////////////////////
		// 当前最大需量发生时间
		/////////////////////////////////////////////////////////////

		{ 0x8000, 0xB021, MKX_ConvertToDateTime },	// (当前)费率1反向有功最大需量发生时间
		{ 0x8001, 0xB022, MKX_ConvertToDateTime },	// (当前)费率2反向有功最大需量发生时间
		{ 0x8002, 0xB023, MKX_ConvertToDateTime },	// (当前)费率3反向有功最大需量发生时间
		{ 0x8003, 0xB024, MKX_ConvertToDateTime },	// (当前)费率4反向有功最大需量发生时间
		{ 0x8009, 0xB020, MKX_ConvertToDateTime },	// (当前)反向有功总最大需量发生时间

		{ 0x8100, 0xB011, MKX_ConvertToDateTime },	// (当前)费率1正向有功最大需量发生时间
		{ 0x8101, 0xB012, MKX_ConvertToDateTime },	// (当前)费率2正向有功最大需量发生时间
		{ 0x8102, 0xB013, MKX_ConvertToDateTime },	// (当前)费率3正向有功最大需量发生时间
		{ 0x8103, 0xB014, MKX_ConvertToDateTime },	// (当前)费率4正向有功最大需量发生时间
		{ 0x8109, 0xB010, MKX_ConvertToDateTime },	// (当前)正向有功总最大需量发生时间

		/////////////////////////////////////////////////////////////
		// 上月最大需量发生时间
		/////////////////////////////////////////////////////////////

		{ 0x8010, 0xB421, MKX_ConvertToDateTime },	// (上月)费率1反向有功最大需量发生时间
		{ 0x8011, 0xB422, MKX_ConvertToDateTime },	// (上月)费率2反向有功最大需量发生时间
		{ 0x8012, 0xB423, MKX_ConvertToDateTime },	// (上月)费率3反向有功最大需量发生时间
		{ 0x8013, 0xB424, MKX_ConvertToDateTime },	// (上月)费率4反向有功最大需量发生时间
		{ 0x8019, 0xB420, MKX_ConvertToDateTime },	// (上月)反向有功总最大需量发生时间

		{ 0x8110, 0xB411, MKX_ConvertToDateTime },	// (上月)费率1正向有功最大需量发生时间
		{ 0x8111, 0xB412, MKX_ConvertToDateTime },	// (上月)费率2正向有功最大需量发生时间
		{ 0x8112, 0xB413, MKX_ConvertToDateTime },	// (上月)费率3正向有功最大需量发生时间
		{ 0x8113, 0xB414, MKX_ConvertToDateTime },	// (上月)费率4正向有功最大需量发生时间
		{ 0x8119, 0xB410, MKX_ConvertToDateTime },	// (上月)正向有功总最大需量发生时间
		};

int MKX_Convert(BYTE *lpBuf, BYTE *lpSrc) {
	int nPos = 0;
	int nIndx = 0;

	WORD reg_num = lpSrc[nPos++];
	reg_num = (reg_num << 8) + lpSrc[nPos++];

	int nCount = sizeof(ls_mapMKX) / sizeof(MKX_MAP);
	for (int i = 0; i < nCount; ++i) {
		if (ls_mapMKX[i].reg == reg_num) {
			memcpy(&lpBuf[nIndx], &ls_mapMKX[i].di, sizeof(WORD));
			nIndx += sizeof(WORD);
			nIndx += ls_mapMKX[i].pf(&lpBuf[nIndx], &lpSrc[nPos]);
			break;
		}
	}

	return nIndx;
}

} /* namespace pcols */
