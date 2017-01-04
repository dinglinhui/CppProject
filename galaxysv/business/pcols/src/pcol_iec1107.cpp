/*
 * pcol_iec1107h.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "pcol_iec1107.h"
#include <stdlib.h>
#include <string.h>
#include <ctypes.h>

namespace pcols {

int IEC_Start(IEC_FRAME *frm) {
	frm->hdr.code = IEC_START;
	frm->hdr.length = 0;
	return 0;
}

int IEC_ReadData(IEC_FRAME *frm, BYTE nBaud) {
	frm->hdr.code = IEC_READDATA;
	frm->data[0] = nBaud;
	frm->hdr.length = 1;
	return 0;
}

static int IEC_ConvertHost(char *str, BYTE host[6]) {
	int nPos = 0;
	char buff[32] = { 0 };
	for (int i = 5; i >= 0; --i) {
		buff[nPos++] = ((host[i] >> 4) + 0x30);
		buff[nPos++] = ((host[i] & 0x0f) + 0x30);
	}

	int nhtr = 0;
	for (int i = 0; i < nPos; ++i) {
		if (buff[i] != '0') {
			nhtr = i;
			break;
		}
	}

	int nLen = nPos - nhtr;
	memcpy(str, &buff[nhtr], nLen);
	return nLen;
}

int IEC_MakeFrame(IEC_FRAME *frm, BYTE *lpBuf, int /* nBufSize */) {
	int nSize = 0;

	switch (frm->hdr.code) {
	// exp: / ? 电表地址 ! CR LF
	case IEC_START: {
		lpBuf[nSize++] = '/';
		lpBuf[nSize++] = '?';

		char host[32] = { 0 };
		int nLen = IEC_ConvertHost(host, frm->hdr.host);
		memcpy(&lpBuf[nSize], host, nLen);
		nSize += nLen;

		lpBuf[nSize++] = '!';
		lpBuf[nSize++] = IEC_CR;
		lpBuf[nSize++] = IEC_LF;
	}
		break;

		// exp:	ACK 0 x 0 CR LF
	case IEC_READDATA: {
		lpBuf[nSize++] = IEC_ACK;
		lpBuf[nSize++] = 0x30;
		lpBuf[nSize++] = 0x30 + frm->data[0];
		lpBuf[nSize++] = 0x30;
		lpBuf[nSize++] = IEC_CR;
		lpBuf[nSize++] = IEC_LF;
	}
		break;

	default:
		break;
	}

	return nSize;
}

static int __IEC_Start(IEC_FRAME *frm, BYTE *lpRecv, int nSize) {
	int nPos = indexOf((char*) lpRecv, nSize, '/');
	if (nPos >= 0) {
		const char* buff = (const char*) &lpRecv[nPos];
		BYTE nZ = nPos + 4;
		char ext[] = { IEC_CR, IEC_LF, 0 };

		nPos = indexOf(buff, (const char*) ext);
		if (nPos >= 0) {
			frm->data[0] = lpRecv[nZ] - 0x30;
			return 0;
		}
	}

	return -1;
}

static int __IEC_ReadData(IEC_FRAME *frm, BYTE *lpBuf, int nSize) {
	int nStart = indexOf((char*) lpBuf, nSize, IEC_STX);
	if (nStart >= 0) {
		const char* buff = (const char*) &lpBuf[nStart];

		char ext[] = { '!', IEC_CR, IEC_LF, IEC_ETX, 0 };
		int nEnd = indexOf(buff, ext);
		if (nEnd >= 0) {
			int nLen = nEnd - nStart - 1;
			frm->data = &lpBuf[nStart + 1];
			frm->hdr.length = nLen;
			return 0;
		}
	}
	return -1;
}

int IEC_GetFrame(IEC_FRAME *frm, BYTE *lpRecv, int nSize) {
	int nRet = 0;

	switch (frm->hdr.code) {
	case IEC_START:
		nRet = __IEC_Start(frm, lpRecv, nSize);
		break;

	case IEC_ORIGREAD:
	case IEC_READDATA:
		nRet = __IEC_ReadData(frm, lpRecv, nSize);
		break;

	default:
		nRet = -1;
		break;
	}

	return nRet;
}

//////////////////////////////////////////////////////////////////////////////////////
//
//
typedef int (*pfIECCVT)(WORD di, BYTE *lpBuf, char *value, int nSize);

// exp: (061127)
//		(101012)
//		(0611271010)
static int __to_datetime(WORD di, BYTE *lpBuf, char *value, int nSize) {
	int n1 = indexOf(value, nSize, '(');
	if (n1 < 0)
		return 0;
	int n2 = indexOf(value, nSize, ')');
	if (n2 <= 0)
		return 0;

	char buf[32] = { 0 };
	int nBos = 0;
	int nPos = n1 + 1;
	int nInx = 0;

	memcpy(lpBuf, &di, sizeof(WORD));
	nInx += sizeof(WORD);

	while (nPos < n2) {
		if (value[nPos] == ' ' || value[nPos] == '-' || value[nPos] == ':' || value[nPos] == ',') {
			nPos++;
			continue;
		}
		if (value[nPos + 2] == ' ' || value[nPos + 2] == '-' || value[nPos + 2] == ':' || value[nPos + 2] == ',' || value[nPos + 2] == ')') {
			memcpy(&buf[nBos], (char*) &value[nPos], 2);
			nPos += 2;
			nBos += 2;
			continue;
		}
		if (value[nPos + 4] == ' ' || //年有时候是4位
				value[nPos + 4] == '-' || value[nPos + 4] == ':' || value[nPos + 4] == ',' || value[nPos + 4] == ')') {
			memcpy(&buf[nBos], (char*) &value[nPos + 2], 2);
			nPos += 4;
			nBos += 2;
		}
	}

	int nLen = strlen(buf);
	char tmp[6] = { 0 };
	nBos = 0;
	if ((nLen % 6) != 0) {
		nBos = 2;
	}
//	TRACE0("nLen = %d\r\n", nLen);
	for (int i = 0; i < nLen; i++) {
//		TRACE0("%c", buf[i]);
	}
//	TRACE("\r\n");
	while (nBos < nLen) {
		memcpy(tmp, &buf[nBos], 2);
		nBos += 2;
		lpBuf[nInx++] = atoi(tmp);
	}

	return nInx;
}

static int __to_datetimepram(WORD di, BYTE *lpBuf, char *value, int nSize) {
	int n1 = indexOf(value, nSize, '(');
	if (n1 < 0)
		return 0;
	int n2 = indexOf(value, nSize, ')');
	if (n2 <= 0)
		return 0;

	char buf[32] = { 0 };
	int nBos = 0;
	int nPos = n1 + 1;
	int nInx = 0;

	memcpy(lpBuf, &di, sizeof(WORD));
	nInx += sizeof(WORD);

	while (nPos < n2) {
		if (value[nPos] == ' ' || value[nPos] == '-' || value[nPos] == ':' || value[nPos] == ',') {
			nPos++;
			continue;
		}
		if (value[nPos + 2] == ' ' || value[nPos + 2] == '-' || value[nPos + 2] == ':' || value[nPos + 2] == ',' || value[nPos + 2] == ')') {
			memcpy(&buf[nBos], (char*) &value[nPos], 2);
			nPos += 2;
			nBos += 2;
			continue;
		}
		if (value[nPos + 4] == ' ' || //年有时候是4位
				value[nPos + 4] == '-' || value[nPos + 4] == ':' || value[nPos + 4] == ',' || value[nPos + 4] == ')') {
			memcpy(&buf[nBos], (char*) &value[nPos + 2], 2);
			nPos += 4;
			nBos += 2;
		}
	}

	int nLen = strlen(buf);
	char tmp[6] = { 0 };
	nBos = 2;

//	TRACE0("nLen = %d\r\n", nLen);
//	for (int i = 0; i < nLen; i++) {
//		TRACE0("%c", buf[i]);
//	}
//	TRACE("\r\n");
	while (nBos < nLen) {
		memcpy(tmp, &buf[nBos], 2);
		nBos += 2;
		lpBuf[nInx++] = atoi(tmp);
	}

	return nInx;
}
// exp: (0611271211)
static int __to_demand_datetime(WORD di, BYTE *lpBuf, char *value, int nSize) {
	int n1 = indexOf(value, nSize, '(');
	if (n1 < 0)
		return 0;
	int n2 = indexOf(value, nSize, ')');
	if (n2 <= 0)
		return 0;

	char buf[32] = { 0 };
	int nBos = 0;
	int nPos = n1 + 1;
	int nInx = 0;

	memcpy(lpBuf, &di, sizeof(WORD));
	nInx += sizeof(WORD);

	while (nPos < n2) {
		if (value[nPos] == ' ' || value[nPos] == '-' || value[nPos] == ':' || value[nPos] == ',') {
			nPos++;
			continue;
		}

		memcpy(&buf[nBos], (char*) &value[nPos], 2);
		nPos += 2;
		nBos += 2;
	}

	int nLen = strlen(buf);
	char tmp[6] = { 0 };
	nBos = 2;
	if ((nLen % 12) == 0) {
		nBos += 2;
	}

	while (nBos < nLen) {
		memcpy(tmp, &buf[nBos], 2);
		nBos += 2;
		lpBuf[nInx++] = atoi(tmp);
	}

	return nInx;
}

// exp: (123456.78)
//		(123456.78*kWh)
//		(123456.78*hvarh)
//		(236.0*V)
static int __to_float(WORD di, BYTE *lpBuf, char *value, int nSize) {
	int n0 = indexOf(value, nSize, '-');
	if (n0 >= 0)
		return 0;
	int n1 = indexOf(value, nSize, '(');
	if (n1 < 0)
		return 0;

	int n2 = indexOf(value, nSize, ')');
	if (n2 <= 0)
		return 0;

	int nInx = 0;
	memcpy(lpBuf, &di, sizeof(WORD));
	nInx += sizeof(WORD);

	int nLen = 0;
	int n3 = indexOf(value, nSize, '*');
	if (n3 > 0)
		nLen = n3 - n1 - 1;
	else
		nLen = n2 - n1 - 1;

	char buff[16] = { 0 };
	memcpy(buff, &value[n1 + 1], nLen);
	buff[nLen] = '\0';

	double dbVal = atof(buff);
	memcpy(&lpBuf[nInx], &dbVal, sizeof(double));
	nInx += sizeof(double);

	return nInx;
}
// exp:(14)(00.000*kW)
//
//
//
static int __to_float_gl(WORD di, BYTE *lpBuf, char *value, int nSize) {
	int n = indexOf(value, nSize, ')');
	if (n <= 0)
		return 0;
	value += n + 1;
	return __to_float(di, lpBuf, value, nSize);

}
// exp: (123456.78)
//		(123456.78*kWh)
//		(123456.78*hvarh)
//		(236.0*V)
static int __to_word(WORD di, BYTE *lpBuf, char *value, int nSize) {
	int n0 = indexOf(value, nSize, '-');
	if (n0 >= 0)
		return 0;
	int n1 = indexOf(value, nSize, '(');
	if (n1 < 0)
		return 0;

	int n2 = indexOf(value, nSize, ')');
	if (n2 <= 0)
		return 0;

	int nInx = 0;
	memcpy(lpBuf, &di, sizeof(WORD));
	nInx += sizeof(WORD);

	int nLen = 0;
	int n3 = indexOf(value, nSize, '*');
	if (n3 > 0)
		nLen = n3 - n1 - 1;
	else
		nLen = n2 - n1 - 1;

	char buff[16] = { 0 };
	memcpy(buff, &value[n1 + 1], nLen);
	buff[nLen] = '\0';

	WORD dbVal = (WORD) atof(buff);
	memcpy(&lpBuf[nInx], &dbVal, sizeof(WORD));
	nInx += sizeof(WORD);

	return nInx;
}

// exp:	(23.7*kW)(0611271010)
static int __to_demand(WORD di, BYTE *lpBuf, char *value, int nSize) {
	int n = indexOf(value, nSize, ')');
	if (n <= 0)
		return 0;

	int nLen1 = __to_float(di, lpBuf, value, n + 1);
	if (nLen1 > 0) {
		int nLen2 = __to_demand_datetime(di + 0x1000, &lpBuf[nLen1], &value[n + 1], nSize - n - 1);
		if (nLen2 > 0) {
			return (nLen1 + nLen2);
		}
	}

	return 0;
}

typedef struct tag_IEC_PARSE {
	const char* szFlag;
	WORD di;
	pfIECCVT pf;
} IEC_PARSE;

static const IEC_PARSE IEC1107_V00[] = /*-l&g表基本数据项，各种表定义没有冲突的-*/
{ { "0.9.1", 0xC011, __to_datetime },	// 时间
		{ "0.9.2", 0xC010, __to_datetime },  // 日期

		{ "L1", 0xB611, __to_word },  // A相电压
		{ "L2", 0xB612, __to_word },  // B相电压
		{ "L3", 0xB613, __to_word },  // C相电压

		{ "L.1", 0xB611, __to_word },  // A相电压
		{ "L.2", 0xB612, __to_word },  // B相电压
		{ "L.3", 0xB613, __to_word },  // C相电压

		{ "32.7.0", 0xB611, __to_word },  // A相电压
		{ "52.7.0", 0xB612, __to_word },  // B相电压
		{ "72.7.0", 0xB613, __to_word },  // C相电压

		{ "50.1", 0xB611, __to_word },  // A相电压
		{ "50.2", 0xB612, __to_word },  // B相电压
		{ "50.3", 0xB613, __to_word },  // C相电压

		{ "32.7", 0xB611, __to_word },  // A相电压
		{ "52.7", 0xB612, __to_word },  // B相电压
		{ "72.7", 0xB613, __to_word },  // C相电压

		{ "A", 0xB621, __to_float },	// A相电流
		{ "B", 0xB622, __to_float },	// B相电流
		{ "C", 0xB623, __to_float },	// C相电流

		{ "31.7.0", 0xB621, __to_float },	// A相电流
		{ "51.7.0", 0xB622, __to_float },	// B相电流
		{ "71.7.0", 0xB623, __to_float },	// C相电流

		{ "51.1", 0xB621, __to_float },	// A相电流
		{ "51.2", 0xB622, __to_float },	// B相电流
		{ "51.3", 0xB623, __to_float },	// C相电流

		{ "31.7", 0xB621, __to_float },	// A相电流
		{ "51.7", 0xB622, __to_float },	// B相电流
		{ "71.7", 0xB623, __to_float },	// C相电流

		//
		{ "81.7.00", 0xB660, __to_float },	// A相电压相角
		{ "81.7.10", 0xB661, __to_float },	// B相电压相角
		{ "81.7.20", 0xB662, __to_float },	// C相电压相角

		{ "81.7.0", 0xB660, __to_float },	// A相电压相角
		{ "81.7.1", 0xB661, __to_float },	// B相电压相角
		{ "81.7.2", 0xB662, __to_float },	// C相电压相角

		{ "31.1", 0xB660, __to_float },	// A相电压相角
		{ "31.2", 0xB661, __to_float },	// B相电压相角
		{ "31.3", 0xB662, __to_float },	// C相电压相角
		//
		{ "81.7.40", 0xB663, __to_float },	// A相电流相角
		{ "81.7.50", 0xB664, __to_float },	// B相电流相角
		{ "81.7.60", 0xB665, __to_float },	// C相电流相角

		{ "81.7.4", 0xB663, __to_float },	// A相电流相角
		{ "81.7.5", 0xB664, __to_float },	// B相电流相角
		{ "81.7.6", 0xB665, __to_float },	// C相电流相角

		{ "51.1", 0xB663, __to_float },	// A相电流相角
		{ "51.2", 0xB664, __to_float },	// B相电流相角
		{ "51.3", 0xB665, __to_float },	// C相电流相角

		//
		{ "13.7", 0xB650, __to_float },	// 总功率因素
		{ "13.7.0", 0xB650, __to_float },	// 总功率因素

		{ "33.7", 0xB651, __to_float },	// A相功率因素
		{ "53.7", 0xB652, __to_float },	// B相功率因素
		{ "73.7", 0xB653, __to_float },	// C相功率因素

		{ "33.7.0", 0xB651, __to_float },	// A相功率因素
		{ "53.7.0", 0xB652, __to_float },	// B相功率因素
		{ "73.7.0", 0xB653, __to_float },	// C相功率因素

		{ "53.1", 0xB651, __to_float },	// A相功率因素
		{ "53.2", 0xB652, __to_float },	// B相功率因素
		{ "53.3", 0xB653, __to_float },	// C相功率因素

		{ "61.1", 0xB651, __to_float },	// A相功率因素
		{ "61.2", 0xB652, __to_float },	// B相功率因素
		{ "61.3", 0xB653, __to_float },	// C相功率因素

		{ "5.1", 0xB651, __to_float },	// A相功率因素
		{ "5.2", 0xB652, __to_float },	// B相功率因素
		{ "5.3", 0xB653, __to_float },	// C相功率因素

		{ "2.0", 0xB210, __to_datetimepram },     //最后1次编程时间
		{ "C.2.1", 0xB210, __to_datetimepram },     //最后1次编程时间
		{ "95", 0xB210, __to_datetimepram },     //最后1次编程时间

		{ "C.2.0", 0xB212, __to_word },     //编程次数

		{ "0.0.2", 0xC030, __to_word },     //有功常数
		{ "0.0.3", 0xC031, __to_word },     //无功常数

		{ "0.3.0", 0xC030, __to_word },     //有功常数
		{ "0.3.1", 0xC031, __to_word },     //无功常数

		{ "1.7.0", 0xB630, __to_float_gl },  // 有功功率
		{ "96", 0xB630, __to_float_gl },  // 有功功率

		{ "97", 0xB640, __to_float_gl },  // 无功功率

		{ "4.a", 0xB631, __to_float_gl },  // A有功功率
		{ "4.b", 0xB632, __to_float_gl },  // B有功功率
		{ "4.c", 0xB633, __to_float_gl },  // C有功功率

		{ "4.A", 0xB631, __to_float_gl },  // A有功功率
		{ "4.B", 0xB632, __to_float_gl },  // B有功功率
		{ "4.C", 0xB633, __to_float_gl },  // C有功功率

		{ "21.7.0", 0xB631, __to_float_gl },  // A有功功率
		{ "41.7.0", 0xB632, __to_float_gl },  // B有功功率
		{ "61.7.0", 0xB633, __to_float_gl },  // C有功功率

		{ "C.7.0", 0xB310, __to_word },     //总断相次数
		{ "71", 0xB310, __to_word },     //总断相次数

		{ "32.32", 0xB311, __to_word },     //A相断相次数
		{ "52.32", 0xB312, __to_word },     //B相断相次数
		{ "72.32", 0xB313, __to_word },     //C相断相次数

		{ "71.1", 0xB311, __to_word },     //A相断相次数
		{ "71.2", 0xB312, __to_word },     //B相断相次数
		{ "71.3", 0xB313, __to_word },     //C相断相次数

		{ "C.7.1", 0xB311, __to_word },     //A相断相次数
		{ "C.7.2", 0xB312, __to_word },     //B相断相次数
		{ "C.7.3", 0xB313, __to_word },     //C相断相次数

		{ "70.1", 0xB311, __to_word },     //A相断相次数
		{ "70.2", 0xB312, __to_word },     //B相断相次数
		{ "70.3", 0xB313, __to_word },     //C相断相次数

		{ "70.a", 0xB321, __to_word },     //A相断相时间累计值
		{ "70.b", 0xB322, __to_word },     //B相断相时间累计值
		{ "70.c", 0xB323, __to_word },     //C相断相时间累计值

		{ "70.A", 0xB321, __to_word },     //A相断相时间累计值
		{ "70.B", 0xB322, __to_word },     //B相断相时间累计值
		{ "70.C", 0xB323, __to_word },     //C相断相时间累计值

		{ "20", 0x9010, __to_float },	// 正向有功总电量
		{ "1.8.0", 0x9010, __to_float },	// 正向有功总电量

		{ "8.4", 0x9011, __to_float },	// (第一费率)正向有功分时电量
		{ "8.1", 0x9012, __to_float },	// (第二费率)正向有功分时电量
		{ "8.2", 0x9013, __to_float },	// (第三费率)正向有功分时电量
		{ "8.3", 0x9014, __to_float },	// (第四费率)正向有功分时电量

		{ "1.8.4", 0x9011, __to_float },	// (第一费率)正向有功分时电量
		{ "1.8.1", 0x9012, __to_float },	// (第二费率)正向有功分时电量
		{ "1.8.2", 0x9013, __to_float },	// (第三费率)正向有功分时电量
		{ "1.8.3", 0x9014, __to_float },	// (第四费率)正向有功分时电量

		//
		{ "20*?", 0x9410, __to_float },	// (上月)正向有功总电量
		{ "1.8.0*?", 0x9010, __to_float },	// 正向有功总电量

		{ "8.4*?", 0x9411, __to_float },	// (上月)(第一费率)正向有功分时电量
		{ "8.1*?", 0x9412, __to_float },	// (上月)(第二费率)正向有功分时电量
		{ "8.2*?", 0x9413, __to_float },	// (上月)(第三费率)正向有功分时电量
		{ "8.3*?", 0x9414, __to_float },	// (上月)(第四费率)正向有功分时电量

		{ "1.8.4*?", 0x9411, __to_float },	// (上月)(第一费率)正向有功分时电量
		{ "1.8.1*?", 0x9412, __to_float },	// (上月)(第二费率)正向有功分时电量
		{ "1.8.2*?", 0x9413, __to_float },	// (上月)(第三费率)正向有功分时电量
		{ "1.8.3*?", 0x9414, __to_float },	// (上月)(第四费率)正向有功分时电量

		//
		{ "2.8.0", 0x9020, __to_float },	// 反向有功总电量
		{ "38.4", 0x9021, __to_float },	// (第一费率)反向有功分时电量
		{ "38.1", 0x9022, __to_float },	// (第二费率)反向有功分时电量
		{ "38.2", 0x9023, __to_float },	// (第三费率)反向有功分时电量
		{ "38.3", 0x9024, __to_float },	// (第四费率)反向有功分时电量

		{ "2.8.4", 0x9021, __to_float },	// (第一费率)反向有功分时电量
		{ "2.8.1", 0x9022, __to_float },	// (第二费率)反向有功分时电量
		{ "2.8.2", 0x9023, __to_float },	// (第三费率)反向有功分时电量
		{ "2.8.3", 0x9024, __to_float },	// (第四费率)反向有功分时电量

		//
		{ "2.8.0*?", 0x9420, __to_float },	// (上月)反向有功总电量
		{ "38.4*?", 0x9421, __to_float },	// (上月)(第一费率)反向有功分时电量
		{ "38.1*?", 0x9422, __to_float },	// (上月)(第二费率)反向有功分时电量
		{ "38.2*?", 0x9423, __to_float },	// (上月)(第三费率)反向有功分时电量
		{ "38.3*?", 0x9424, __to_float },	// (上月)(第四费率)反向有功分时电量

		{ "2.8.4*?", 0x9421, __to_float },	// (上月)(第一费率)反向有功分时电量
		{ "2.8.1*?", 0x9422, __to_float },	// (上月)(第二费率)反向有功分时电量
		{ "2.8.2*?", 0x9423, __to_float },	// (上月)(第三费率)反向有功分时电量
		{ "2.8.3*?", 0x9424, __to_float },	// (上月)(第四费率)反向有功分时电量

		//
		{ "3.8.0", 0x9110, __to_float },	// 正向无功总电量
		{ "9.4", 0x9111, __to_float },	// (第一费率)正向无功分时电量
		{ "9.1", 0x9112, __to_float },	// (第二费率)正向无功分时电量
		{ "9.2", 0x9113, __to_float },	// (第三费率)正向无功分时电量
		{ "9.3", 0x9114, __to_float },	// (第四费率)正向无功分时电量

		{ "3.8.4", 0x9111, __to_float },	// (第一费率)正向无功分时电量
		{ "3.8.1", 0x9112, __to_float },	// (第二费率)正向无功分时电量
		{ "3.8.2", 0x9113, __to_float },	// (第三费率)正向无功分时电量
		{ "3.8.3", 0x9114, __to_float },	// (第四费率)正向无功分时电量

		//
		{ "3.8.0*?", 0x9510, __to_float },	// (上月)正向无功总电量
		{ "9.4*?", 0x9511, __to_float },	// (上月)(第一费率)正向无功分时电量
		{ "9.1*?", 0x9512, __to_float },	// (上月)(第二费率)正向无功分时电量
		{ "9.2*?", 0x9513, __to_float },	// (上月)(第三费率)正向无功分时电量
		{ "9.3*?", 0x9514, __to_float },	// (上月)(第四费率)正向无功分时电量

		{ "3.8.4*?", 0x9511, __to_float },	// (上月)(第一费率)正向无功分时电量
		{ "3.8.1*?", 0x9512, __to_float },	// (上月)(第二费率)正向无功分时电量
		{ "3.8.2*?", 0x9513, __to_float },	// (上月)(第三费率)正向无功分时电量
		{ "3.8.3*?", 0x9514, __to_float },	// (上月)(第四费率)正向无功分时电量

		//
		{ "24", 0x9120, __to_float },	// 反向无功总电量
		{ "4.8.0", 0x9120, __to_float },	// 反向无功总电量

		{ "39.4", 0x9121, __to_float },	// (第一费率)反向无功分时电量
		{ "39.1", 0x9122, __to_float },	// (第二费率)反向无功分时电量
		{ "39.2", 0x9123, __to_float },	// (第三费率)反向无功分时电量
		{ "39.3", 0x9124, __to_float },	// (第四费率)反向无功分时电量

		{ "4.8.4", 0x9121, __to_float },	// (第一费率)反向无功分时电量
		{ "4.8.1", 0x9122, __to_float },	// (第二费率)反向无功分时电量
		{ "4.8.2", 0x9123, __to_float },	// (第三费率)反向无功分时电量
		{ "4.8.3", 0x9124, __to_float },	// (第四费率)反向无功分时电量

		//
		{ "24*?", 0x9520, __to_float },	// (上月)反向无功总电量
		{ "4.8.0*?", 0x9520, __to_float },	// (上月)反向无功总电量

		{ "39.4*?", 0x9521, __to_float },	// (上月)(第一费率)反向无功分时电量
		{ "39.1*?", 0x9522, __to_float },	// (上月)(第二费率)反向无功分时电量
		{ "39.2*?", 0x9523, __to_float },	// (上月)(第三费率)反向无功分时电量
		{ "39.3*?", 0x9524, __to_float },	// (上月)(第四费率)反向无功分时电量

		{ "4.8.4*?", 0x9521, __to_float },	// (上月)(第一费率)反向无功分时电量
		{ "4.8.1*?", 0x9522, __to_float },	// (上月)(第二费率)反向无功分时电量
		{ "4.8.2*?", 0x9523, __to_float },	// (上月)(第三费率)反向无功分时电量
		{ "4.8.3*?", 0x9524, __to_float },	// (上月)(第四费率)反向无功分时电量

		{ "5.8.0", 0x9130, __to_float },	// 1象限无功总电量

		{ "5.8.4", 0x9131, __to_float },	// (第一费率)1象限无功分时电量
		{ "5.8.1", 0x9132, __to_float },	// (第二费率)1象限无功分时电量
		{ "5.8.2", 0x9133, __to_float },	// (第三费率)1象限无功分时电量
		{ "5.8.3", 0x9134, __to_float },	// (第四费率)1象限无功分时电量
		//
		{ "5.8.0*?", 0x9530, __to_float },	// (上月)1象限无功总电量

		{ "5.8.4*?", 0x9531, __to_float },	// (上月)(第一费率)1象限无功分时电量
		{ "5.8.1*?", 0x9532, __to_float },	// (上月)(第二费率)1象限无功分时电量
		{ "5.8.2*?", 0x9533, __to_float },	// (上月)(第三费率)1象限无功分时电量
		{ "5.8.3*?", 0x9534, __to_float },	// (上月)(第四费率)1象限无功分时电量

		{ "6.8.0", 0x9140, __to_float },	// 2象限无功总电量

		{ "6.8.4", 0x9141, __to_float },	// (第一费率)2象限无功分时电量
		{ "6.8.1", 0x9142, __to_float },	// (第二费率)2象限无功分时电量
		{ "6.8.2", 0x9143, __to_float },	// (第三费率)2象限无功分时电量
		{ "6.8.3", 0x9144, __to_float },	// (第四费率)2象限无功分时电量
		//
		{ "6.8.0*?", 0x9540, __to_float },	// (上月)2象限无功总电量

		{ "6.8.4*?", 0x9541, __to_float },	// (上月)(第一费率)2象限无功分时电量
		{ "6.8.1*?", 0x9542, __to_float },	// (上月)(第二费率)2象限无功分时电量
		{ "6.8.2*?", 0x9543, __to_float },	// (上月)(第三费率)2象限无功分时电量
		{ "6.8.3*?", 0x9544, __to_float },	// (上月)(第四费率)2象限无功分时电量

		{ "7.8.0", 0x9120, __to_float },	// 3象限无功总电量

		{ "7.8.4", 0x9121, __to_float },	// (第一费率)3象限无功分时电量
		{ "7.8.1", 0x9122, __to_float },	// (第二费率)3象限无功分时电量
		{ "7.8.2", 0x9123, __to_float },	// (第三费率)3象限无功分时电量
		{ "7.8.3", 0x9124, __to_float },	// (第四费率)3象限无功分时电量
		//

		{ "7.8.0*?", 0x9520, __to_float },	// (上月)3象限无功总电量

		{ "7.8.4*?", 0x9521, __to_float },	// (上月)(第一费率)3象限无功分时电量
		{ "7.8.1*?", 0x9522, __to_float },	// (上月)(第二费率)3象限无功分时电量
		{ "7.8.2*?", 0x9523, __to_float },	// (上月)(第三费率)3象限无功分时电量
		{ "7.8.3*?", 0x9524, __to_float },	// (上月)(第四费率)3象限无功分时电量

		{ "8.8.0", 0x9160, __to_float },	// 4象限无功总电量

		{ "8.8.4", 0x9161, __to_float },	// (第一费率)4象限无功分时电量
		{ "8.8.1", 0x9162, __to_float },	// (第二费率)4象限无功分时电量
		{ "8.8.2", 0x9163, __to_float },	// (第三费率)4象限无功分时电量
		{ "8.8.3", 0x9164, __to_float },	// (第四费率)4象限无功分时电量
		//

		{ "8.8.0*?", 0x9560, __to_float },	// (上月)4象限无功总电量

		{ "8.8.4*?", 0x9561, __to_float },	// (上月)(第一费率)4象限无功分时电量
		{ "8.8.1*?", 0x9562, __to_float },	// (上月)(第二费率)4象限无功分时电量
		{ "8.8.2*?", 0x9563, __to_float },	// (上月)(第三费率)4象限无功分时电量
		{ "8.8.3*?", 0x9564, __to_float },	// (上月)(第四费率)4象限无功分时电量

		{ "6.4", 0xA010, __to_demand },	// 正向有功总最大需量
		{ "1.6.0", 0xA010, __to_demand },	// 正向有功总最大需量
		{ "15.6.0", 0xA010, __to_demand },	// 正向有功总最大需量

//	{ "6.1",		0xA011, __to_demand		},	// (第一费率)正向有功最大需量
		{ "6.1", 0xA012, __to_demand },	// (第二费率)正向有功最大需量
		{ "6.2", 0xA013, __to_demand },	// (第三费率)正向有功最大需量
		{ "6.3", 0xA014, __to_demand },	// (第四费率)正向有功最大需量

		{ "1.6.4", 0xA011, __to_demand },	// (第一费率)正向有功最大需量
		{ "1.6.1", 0xA012, __to_demand },	// (第二费率)正向有功最大需量
		{ "1.6.2", 0xA013, __to_demand },	// (第三费率)正向有功最大需量
		{ "1.6.3", 0xA014, __to_demand },	// (第四费率)正向有功最大需量

		{ "15.6.4", 0xA011, __to_demand },	// (第一费率)正向有功最大需量
		{ "15.6.1", 0xA012, __to_demand },	// (第二费率)正向有功最大需量
		{ "15.6.2", 0xA013, __to_demand },	// (第三费率)正向有功最大需量
		{ "15.6.3", 0xA014, __to_demand },	// (第四费率)正向有功最大需量
		//
		{ "6.4*?", 0xA410, __to_demand },	// (上月)正向有功总最大需量
		{ "1.6.0*?", 0xA410, __to_demand },	// (上月)正向有功总最大需量
		{ "15.6.0*?", 0xA410, __to_demand },	// (上月)正向有功总最大需量

//	{ "6.1*?",		0xA411, __to_demand		},	// (上月)(第一费率)正向有功最大需量
		{ "6.1*?", 0xA412, __to_demand },	// (上月)(第二费率)正向有功最大需量
		{ "6.2*?", 0xA413, __to_demand },	// (上月)(第三费率)正向有功最大需量
		{ "6.3*?", 0xA414, __to_demand },	// (上月)(第四费率)正向有功最大需量

		{ "1.6.4*?", 0xA411, __to_demand },	// (上月)(第一费率)正向有功最大需量
		{ "1.6.1*?", 0xA412, __to_demand },	// (上月)(第二费率)正向有功最大需量
		{ "1.6.2*?", 0xA413, __to_demand },	// (上月)(第三费率)正向有功最大需量
		{ "1.6.3*?", 0xA414, __to_demand },	// (上月)(第四费率)正向有功最大需量

		{ "15.6.4*?", 0xA411, __to_demand },	// (上月)(第一费率)正向有功最大需量
		{ "15.6.1*?", 0xA412, __to_demand },	// (上月)(第二费率)正向有功最大需量
		{ "15.6.2*?", 0xA413, __to_demand },	// (上月)(第三费率)正向有功最大需量
		{ "15.6.3*?", 0xA414, __to_demand },	// (上月)(第四费率)正向有功最大需量
		//
		{ "6.6", 0xA020, __to_demand },	// 反向有功总最大需量
		{ "2.6.0", 0xA020, __to_demand },	// 反向有功总最大需量
		{ "36.0", 0xA020, __to_demand },	// 反向有功总最大需量

		{ "6.6.4", 0xA021, __to_demand },	// (第一费率)反向有功最大需量
		{ "6.6.1", 0xA022, __to_demand },	// (第二费率)反向有功最大需量
		{ "6.6.2", 0xA023, __to_demand },	// (第三费率)反向有功最大需量
		{ "6.6.3", 0xA024, __to_demand },	// (第三费率)反向有功最大需量

		{ "2.6.4", 0xA021, __to_demand },	// (第一费率)反向有功最大需量
		{ "2.6.1", 0xA022, __to_demand },	// (第二费率)反向有功最大需量
		{ "2.6.2", 0xA023, __to_demand },	// (第三费率)反向有功最大需量
		{ "2.6.3", 0xA024, __to_demand },	// (第三费率)反向有功最大需量

		{ "36.4", 0xA021, __to_demand },	// (第一费率)反向有功最大需量
		{ "36.1", 0xA022, __to_demand },	// (第二费率)反向有功最大需量
		{ "36.2", 0xA023, __to_demand },	// (第三费率)反向有功最大需量
		{ "36.3", 0xA024, __to_demand },	// (第三费率)反向有功最大需量
		//
		{ "6.6*?", 0xA420, __to_demand },	// (上月)反向有功总最大需量
		{ "2.6.0*?", 0xA420, __to_demand },	// (上月)反向有功总最大需量

		{ "6.6.4*?", 0xA421, __to_demand },	// (上月)(第一费率)反向有功最大需量
		{ "6.6.1*?", 0xA422, __to_demand },	// (上月)(第二费率)反向有功最大需量
		{ "6.6.2*?", 0xA423, __to_demand },	// (上月)(第三费率)反向有功最大需量
		{ "6.6.3*?", 0xA424, __to_demand },	// (上月)(第三费率)反向有功最大需量

		{ "2.6.4*?", 0xA421, __to_demand },	// (上月)(第一费率)反向有功最大需量
		{ "2.6.1*?", 0xA422, __to_demand },	// (上月)(第二费率)反向有功最大需量
		{ "2.6.2*?", 0xA423, __to_demand },	// (上月)(第三费率)反向有功最大需量
		{ "2.6.3*?", 0xA424, __to_demand },	// (上月)(第三费率)反向有功最大需量

		//
		{ "6.7", 0xA110, __to_demand },	// 正向无功总最大需量
		{ "3.6.0", 0xA110, __to_demand },	// 正向无功总最大需量
		{ "5.6.0", 0xA110, __to_demand },	// 正向无功总最大需量

		{ "6.7.4", 0xA111, __to_demand },	// (第一费率)正向无功最大需量
		{ "6.7.1", 0xA112, __to_demand },	// (第二费率)正向无功最大需量
		{ "6.7.2", 0xA113, __to_demand },	// (第三费率)正向无功最大需量
		{ "6.7.3", 0xA114, __to_demand },	// (第三费率)正向无功最大需量

		{ "3.6.4", 0xA111, __to_demand },	// (第一费率)正向无功最大需量
		{ "3.6.1", 0xA112, __to_demand },	// (第二费率)正向无功最大需量
		{ "3.6.2", 0xA113, __to_demand },	// (第三费率)正向无功最大需量
		{ "3.6.3", 0xA114, __to_demand },	// (第三费率)正向无功最大需量

		{ "5.6.4", 0xA111, __to_demand },	// (第一费率)正向无功最大需量
		{ "5.6.1", 0xA112, __to_demand },	// (第二费率)正向无功最大需量
		{ "5.6.2", 0xA113, __to_demand },	// (第三费率)正向无功最大需量
		{ "5.6.3", 0xA114, __to_demand },	// (第三费率)正向无功最大需量

		//
		{ "6.7*?", 0xA510, __to_demand },	// (上月)正向无功总最大需量
		{ "3.6.0*?", 0xA510, __to_demand },	// (上月)正向无功总最大需量
		{ "5.6.0*?", 0xA510, __to_demand },	// (上月)正向无功总最大需量

		{ "6.7.4*?", 0xA511, __to_demand },	// (上月)(第一费率)正向无功最大需量
		{ "6.7.1*?", 0xA512, __to_demand },	// (上月)(第二费率)正向无功最大需量
		{ "6.7.2*?", 0xA513, __to_demand },	// (上月)(第三费率)正向无功最大需量
		{ "6.7.3*?", 0xA514, __to_demand },	// (上月)(第四费率)正向无功最大需量

		{ "3.6.4*?", 0xA511, __to_demand },	// (上月)(第一费率)正向无功最大需量
		{ "3.6.1*?", 0xA512, __to_demand },	// (上月)(第二费率)正向无功最大需量
		{ "3.6.2*?", 0xA513, __to_demand },	// (上月)(第三费率)正向无功最大需量
		{ "3.6.3*?", 0xA514, __to_demand },	// (上月)(第四费率)正向无功最大需量

		{ "5.6.4*?", 0xA511, __to_demand },	// (上月)(第一费率)正向无功最大需量
		{ "5.6.1*?", 0xA512, __to_demand },	// (上月)(第二费率)正向无功最大需量
		{ "5.6.2*?", 0xA513, __to_demand },	// (上月)(第三费率)正向无功最大需量
		{ "5.6.3*?", 0xA514, __to_demand },	// (上月)(第四费率)正向无功最大需量

		//
		{ "6.8", 0xA120, __to_demand },	// 反向无功总最大需量
		{ "4.6.0", 0xA120, __to_demand },	// 反向无功总最大需量

		{ "6.8.4", 0xA121, __to_demand },	// (第一费率)反向无功最大需量
		{ "6.8.1", 0xA122, __to_demand },	// (第二费率)反向无功最大需量
		{ "6.8.2", 0xA123, __to_demand },	// (第三费率)反向无功最大需量
		{ "6.8.3", 0xA124, __to_demand },	// (第三费率)反向无功最大需量

		{ "4.6.4", 0xA121, __to_demand },	// (第一费率)反向无功最大需量
		{ "4.6.1", 0xA122, __to_demand },	// (第二费率)反向无功最大需量
		{ "4.6.2", 0xA123, __to_demand },	// (第三费率)反向无功最大需量
		{ "4.6.3", 0xA124, __to_demand },	// (第三费率)反向无功最大需量

		//
		{ "6.8*?", 0xA520, __to_demand },	// (上月)反向无功总最大需量
		{ "4.6.0*?", 0xA520, __to_demand },	// (上月)反向无功总最大需量

		{ "6.8.4*?", 0xA521, __to_demand },	// (上月)(第一费率)反向无功最大需量
		{ "6.8.1*?", 0xA522, __to_demand },	// (上月)(第二费率)反向无功最大需量
		{ "6.8.2*?", 0xA523, __to_demand },	// (上月)(第三费率)反向无功最大需量
		{ "6.8.3*?", 0xA524, __to_demand },	// (上月)(第三费率)反向无功最大需量

		{ "4.6.4*?", 0xA521, __to_demand },	// (上月)(第一费率)反向无功最大需量
		{ "4.6.1*?", 0xA522, __to_demand },	// (上月)(第二费率)反向无功最大需量
		{ "4.6.2*?", 0xA523, __to_demand },	// (上月)(第三费率)反向无功最大需量
		{ "4.6.3*?", 0xA524, __to_demand },	// (上月)(第三费率)反向无功最大需量

		};

static const IEC_PARSE IEC1107_V01[] = /*-江苏-*/
{ { "21", 0x9020, __to_float },	// 反向有功总电量
		//
		{ "21*?", 0x9420, __to_float },	// (上月)反向有功总电量
		//
		{ "23", 0x9110, __to_float },	// 正向无功总电量
		//
		{ "23*?", 0x9510, __to_float },	// (上月)正向无功总电量

		{ "11", 0xC011, __to_datetime },	// 时间
		{ "12", 0xC010, __to_datetime },  // 日期

		{ "2", 0xB212, __to_word },     //编程次数
		{ "4", 0xB630, __to_float_gl },  // 有功功率
		{ "14", 0xB214, __to_word },	// 电池工作时间min
		{ "17", 0xC030, __to_word },     //有功常数
		{ "18", 0xC031, __to_word },     //无功常数
		};

static const IEC_PARSE IEC1107_V02[] = /*-锦州-*/
{ { "2", 0x9011, __to_float },	// (第一费率)正向有功分时电量
		{ "4", 0x9012, __to_float },	// (第二费率)正向有功分时电量
		{ "6", 0x9013, __to_float },	// (第三费率)正向有功分时电量
		{ "8", 0x9014, __to_float },	// (第四费率)正向有功分时电量
		//
		{ "2*?", 0x9411, __to_float },	// (上月)(第一费率)正向有功分时电量
		{ "4*?", 0x9412, __to_float },	// (上月)(第二费率)正向有功分时电量
		{ "6*?", 0x9413, __to_float },	// (上月)(第三费率)正向有功分时电量
		{ "8*?", 0x9414, __to_float },	// (上月)(第四费率)正向有功分时电量
		//
		{ "22", 0x9020, __to_float },	// 反向有功总电量
		{ "32", 0x9021, __to_float },	// (第一费率)反向有功分时电量
		{ "34", 0x9022, __to_float },	// (第二费率)反向有功分时电量
		{ "36", 0x9023, __to_float },	// (第三费率)反向有功分时电量
		{ "38", 0x9024, __to_float },	// (第四费率)反向有功分时电量
		//
		{ "22*?", 0x9420, __to_float },	// (上月)反向有功总电量
		{ "32*?", 0x9421, __to_float },	// (上月)(第一费率)反向有功分时电量
		{ "34*?", 0x9422, __to_float },	// (上月)(第二费率)反向有功分时电量
		{ "36*?", 0x9423, __to_float },	// (上月)(第三费率)反向有功分时电量
		{ "38*?", 0x9424, __to_float },	// (上月)(第四费率)反向有功分时电量
		//
		{ "21", 0x9110, __to_float },	// 正向无功总电量
		{ "3", 0x9111, __to_float },	// (第一费率)正向无功分时电量
		{ "5", 0x9112, __to_float },	// (第二费率)正向无功分时电量
		{ "7", 0x9113, __to_float },	// (第三费率)正向无功分时电量
		{ "9", 0x9114, __to_float },	// (第四费率)正向无功分时电量
		//
		{ "21*?", 0x9510, __to_float },	// (上月)正向无功总电量
		{ "3*?", 0x9511, __to_float },	// (上月)(第一费率)正向无功分时电量
		{ "5*?", 0x9512, __to_float },	// (上月)(第二费率)正向无功分时电量
		{ "7*?", 0x9513, __to_float },	// (上月)(第三费率)正向无功分时电量
		{ "9*?", 0x9514, __to_float },	// (上月)(第四费率)正向无功分时电量
		//
		{ "23", 0x9120, __to_float },	// 反向无功总电量
		{ "33", 0x9121, __to_float },	// (第一费率)反向无功分时电量
		{ "35", 0x9122, __to_float },	// (第二费率)反向无功分时电量
		{ "37", 0x9123, __to_float },	// (第三费率)反向无功分时电量
		{ "39", 0x9124, __to_float },	// (第四费率)反向无功分时电量
		//
		{ "23*?", 0x9520, __to_float },	// (上月)反向无功总电量
		{ "33*?", 0x9521, __to_float },	// (上月)(第一费率)反向无功分时电量
		{ "35*?", 0x9522, __to_float },	// (上月)(第二费率)反向无功分时电量
		{ "37*?", 0x9523, __to_float },	// (上月)(第三费率)反向无功分时电量
		{ "39*?", 0x9524, __to_float },	// (上月)(第四费率)反向无功分时电量
		//
		{ "24", 0x9130, __to_float },	// 1象限无功总电量
		//
		{ "24*?", 0x9530, __to_float },	// (上月)1象限无功总电量
		//
		{ "44.4", 0x9131, __to_float },	// (第一费率)1象限无功分时电量
		{ "44.1", 0x9132, __to_float },	// (第二费率)1象限无功分时电量
		{ "44.2", 0x9133, __to_float },	// (第三费率)1象限无功分时电量
		{ "44.3", 0x9134, __to_float },	// (第四费率)1象限无功分时电量
		//
		{ "44.4*?", 0x9531, __to_float },	// (上月)(第一费率)1象限无功分时电量
		{ "44.1*?", 0x9532, __to_float },	// (上月)(第二费率)1象限无功分时电量
		{ "44.2*?", 0x9533, __to_float },	// (上月)(第三费率)1象限无功分时电量
		{ "44.3*?", 0x9534, __to_float },	// (上月)(第四费率)1象限无功分时电量

		{ "25", 0x9140, __to_float },	// 2象限无功总电量
		//
		{ "25*?", 0x9540, __to_float },	// (上月)2象限无功总电量
		//
		{ "45.4", 0x9141, __to_float },	// (第一费率)2象限无功分时电量
		{ "45.1", 0x9142, __to_float },	// (第二费率)2象限无功分时电量
		{ "45.2", 0x9143, __to_float },	// (第三费率)2象限无功分时电量
		{ "45.3", 0x9144, __to_float },	// (第四费率)2象限无功分时电量		//

		{ "45.4*?", 0x9541, __to_float },	// (上月)(第一费率)2象限无功分时电量
		{ "45.1*?", 0x9542, __to_float },	// (上月)(第二费率)2象限无功分时电量
		{ "45.2*?", 0x9543, __to_float },	// (上月)(第三费率)2象限无功分时电量
		{ "45.3*?", 0x9544, __to_float },	// (上月)(第四费率)2象限无功分时电量

		{ "26", 0x9150, __to_float },	// 3象限无功总电量
		//
		{ "26*?", 0x9550, __to_float },	// (上月)3象限无功总电量
		//
		{ "27", 0x9160, __to_float },	// 4象限无功总电量
		//
		{ "27*?", 0x9560, __to_float },	// (上月)4象限无功总电量

		{ "11", 0xC011, __to_datetime },	// 时间
		{ "12", 0xC010, __to_datetime },  // 日期
		{ "95", 0xB210, __to_datetimepram },  //最后1次编程时间
		{ "14", 0xB214, __to_float },	// 电池工作时间（hour-min）
		{ "17", 0xC030, __to_word },     //有功常数
		{ "18", 0xC031, __to_word },     //无功常数
		};

static const IEC_PARSE IEC1107_V03[] = /*-泉州-*/
{ { "1", 0xC011, __to_datetime },	// 时间
		{ "2", 0xC010, __to_datetime },  // 日期

		{ "3", 0x9010, __to_float },	// 正向有功总电量
		{ "4", 0x9011, __to_float },	// (第一费率)正向有功分时电量
		{ "5", 0x9012, __to_float },	// (第二费率)正向有功分时电量
		{ "1.8.2", 0x9013, __to_float },	// (第三费率)正向有功分时电量
		{ "6", 0x9014, __to_float },	// (第四费率)正向有功分时电量
		//
		{ "3*?", 0x9410, __to_float },	// (上月)正向有功总电量
		{ "4*?", 0x9411, __to_float },	// (上月)(第一费率)正向有功分时电量
		{ "5*?", 0x9412, __to_float },	// (上月)(第二费率)正向有功分时电量
		{ "1.8.2*?", 0x9413, __to_float },	// (上月)(第三费率)正向有功分时电量
		{ "6*?", 0x9414, __to_float },	// (上月)(第四费率)正向有功分时电量
		//
		{ "7", 0x9110, __to_float },	// 正向无功总电量
		//
		{ "7*?", 0x9510, __to_float },	// (上月)正向无功总电量

		{ "9", 0x9130, __to_float },	// 1象限无功总电量
		//
		{ "9*?", 0x9530, __to_float },	// (上月)1象限无功总电量
		//
		{ "10", 0xA010, __to_demand },	// 正向有功总最大需量
		//
		{ "10*?", 0xA410, __to_demand },	// (上月)正向有功总最大需量
		};

static const IEC_PARSE IEC1107_V04[] = {/*-福建-*/
{ "2", 0xC011, __to_datetime },	// 时间
		{ "1", 0xC010, __to_datetime },  // 日期

		{ "3", 0x9010, __to_float },	// 正向有功总电量
		{ "4", 0x9011, __to_float },	// (第一费率)正向有功分时电量
		{ "5", 0x9012, __to_float },	// (第二费率)正向有功分时电量
		{ "5.5", 0x9013, __to_float },	// (第三费率)正向有功分时电量
		{ "6", 0x9014, __to_float },	// (第四费率)正向有功分时电量
		//
		{ "3*?", 0x9410, __to_float },	// (上月)正向有功总电量
		{ "4*?", 0x9411, __to_float },	// (上月)(第一费率)正向有功分时电量
		{ "5*?", 0x9412, __to_float },	// (上月)(第二费率)正向有功分时电量
		{ "5.5*?", 0x9413, __to_float },	// (上月)(第三费率)正向有功分时电量
		{ "6*?", 0x9414, __to_float },	// (上月)(第四费率)正向有功分时电量
		//
		{ "16", 0x9020, __to_float },	// 反向有功总电量
		{ "16*?", 0x9420, __to_float },	// (上月)反向有功总电量

		{ "7", 0x9110, __to_float },	// 正向无功总电量
		{ "7*?", 0x9510, __to_float },	// (上月)正向无功总电量

		{ "8", 0x9111, __to_float },	// (第一费率)正向无功分时电量
		{ "9", 0x9112, __to_float },	// (第二费率)正向无功分时电量
		{ "9.5", 0x9113, __to_float },	// (第三费率)正向无功分时电量
		{ "10", 0x9114, __to_float },	// (第四费率)正向无功分时电量

		{ "8*?", 0x9511, __to_float },	// (上月)(第一费率)正向无功分时电量
		{ "9*?", 0x9512, __to_float },	// (上月)(第二费率)正向无功分时电量
		{ "9.5*?", 0x9513, __to_float },	// (上月)(第三费率)正向无功分时电量
		{ "10*?", 0x9514, __to_float },	// (上月)(第四费率)正向无功分时电量

		{ "11", 0x9120, __to_float },	// 反向无功总电量
		{ "11*?", 0x9520, __to_float },	// (上月)反向无功总电量

		{ "12", 0x9121, __to_float },	// (第一费率)反向无功分时电量
		{ "13", 0x9122, __to_float },	// (第二费率)反向无功分时电量
		{ "13.5", 0x9123, __to_float },	// (第三费率)反向无功分时电量
		{ "14", 0x9124, __to_float },	// (第四费率)反向无功分时电量

		{ "12*?", 0x9521, __to_float },	// (上月)(第一费率)正向无功分时电量
		{ "13*?", 0x9522, __to_float },	// (上月)(第二费率)正向无功分时电量
		{ "13.5*?", 0x9523, __to_float },	// (上月)(第三费率)正向无功分时电量
		{ "14*?", 0x9524, __to_float },	// (上月)(第四费率)正向无功分时电量

		{ "15", 0xA010, __to_demand },	// 正向有功总最大需量

		{ "15.1", 0xA011, __to_demand },	// (第一费率)正向有功最大需量
		{ "15.2", 0xA012, __to_demand },	// (第二费率)正向有功最大需量
//	{ "6.2",		0xA013, __to_demand		},	// (第三费率)正向有功最大需量
		{ "15.3", 0xA014, __to_demand },	// (第四费率)正向有功最大需量
		//
		{ "15*?", 0xA410, __to_demand },	// (上月)正向有功总最大需量

		{ "15.1*?", 0xA411, __to_demand },	// (上月)(第一费率)正向有功最大需量
		{ "15.2*?", 0xA412, __to_demand },	// (上月)(第二费率)正向有功最大需量
//	{ "6.2*?",		0xA413, __to_demand		},	// (上月)(第三费率)正向有功最大需量
		{ "15.3*?", 0xA414, __to_demand },	// (上月)(第四费率)正向有功最大需量
		//
		{ "17", 0xA020, __to_demand },	// 正向无功总最大需量

		{ "17.1", 0xA021, __to_demand },	// (第一费率)正向无功最大需量
		{ "17.2", 0xA022, __to_demand },	// (第二费率)正向无功最大需量
//	{ "17.3",		0xA023, __to_demand		},	// (第三费率)正向无功最大需量
		{ "17.3", 0xA024, __to_demand },	// (第三费率)正向无功最大需量		//
		//
		{ "18", 0xA020, __to_demand },	// 反向无功总最大需量

		{ "18.1", 0xA021, __to_demand },	// (第一费率)反向无功最大需量
		{ "18.2", 0xA022, __to_demand },	// (第二费率)反向无功最大需量
//	{ "6.8.2",		0xA023, __to_demand		},	// (第三费率)反向无功最大需量
		{ "18.3", 0xA024, __to_demand },	// (第三费率)反向无功最大需量
		};

static IEC_PARSE* __find(BYTE type, const char* szFlag) {
	int nCount = sizeof(IEC1107_V00) / sizeof(IEC_PARSE);
	for (int i = 0; i < nCount; ++i) {
		if (strcmp(IEC1107_V00[i].szFlag, szFlag) == 0) {
			return (IEC_PARSE*) &IEC1107_V00[i];
		}
	}

	//判断是哪一种电表规约

	const IEC_PARSE *pIEC1107;
	switch (type) {
	case 4:
		pIEC1107 = IEC1107_V01;
		nCount = sizeof(IEC1107_V01) / sizeof(IEC_PARSE);
		break;
	case 5:
		pIEC1107 = IEC1107_V02;
		nCount = sizeof(IEC1107_V02) / sizeof(IEC_PARSE);
		break;
	case 6:
		pIEC1107 = IEC1107_V03;
		nCount = sizeof(IEC1107_V03) / sizeof(IEC_PARSE);
		break;
	case 7:
		pIEC1107 = IEC1107_V04;
		nCount = sizeof(IEC1107_V04) / sizeof(IEC_PARSE);
		break;
	default:
		pIEC1107 = IEC1107_V01;
		nCount = sizeof(IEC1107_V01) / sizeof(IEC_PARSE);
		break;
	}

	for (int i = 0; i < nCount; ++i) {
		if (strcmp(pIEC1107[i].szFlag, szFlag) == 0) {
			return (IEC_PARSE*) &pIEC1107[i];
		}
	}

	return nullptr;
}

int IEC_Decode(BYTE type, BYTE *lpBuf, char* data, int nSize, int *nLastMonth) {
	int n1 = indexOf(data, nSize, '(');
	if (n1 >= 0 && n1 < 16) {
		char flag[16] = { 0 };
		memcpy(flag, data, n1);
		flag[n1] = '\0';

		int n2 = indexOf(flag, n1, '*');
		if (n2 > 0 && n2 < 8) {
			char mon[8] = { 0 };
			n2++;
			memcpy(mon, &flag[n2], n1 - n2);

			int nMon = atoi(mon);

			if (*nLastMonth == 0)/*-刚开始解析，找出上月标志-*/
			{
				*nLastMonth = atoi(mon);
			}

			if (nMon != *nLastMonth) {
				return 0;
			}
			strcpy(&flag[n2], "?");
		}

		IEC_PARSE *pIEC = __find(type, flag);
		if ((pIEC != nullptr) && (pIEC->pf != nullptr)) {
			return pIEC->pf(pIEC->di, lpBuf, (char*) &data[n1], nSize - n1);
		}
	}
	return 0;
}

} /* namespace pcols */
