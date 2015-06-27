/*
 * pcol_fa.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include <pcol_fa.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <ctypes.h>
#include <time.h>
#include "EM_Protocol.h"
#include "FA_Protocol.h"

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
int FA_IsValid(BYTE *lpBuf, int nSize) {
	// 缓冲区长度不足,无有效帧
	if (nSize < FA_FRAME_MIN_LENGTH)
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
	int nLen = MAKEWORD(lpBuf[nPos], lpBuf[nPos + 1]);
	if (nSize < (nLen + FA_FRAME_MIN_LENGTH))
		return -1;
	nPos += 2;

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
/////////////////////////////////////////////////////////////////////////////////////
//
// 数据格式转换
//
/////////////////////////////////////////////////////////////////////////////////////

typedef int (*FA_ENCODE_PF)(BYTE *lpBuf, void *pSrc);
typedef int (*FA_DECODE_PF)(void *pBody, BYTE *lpBuf);
typedef int (*FA_OUTPUT_PF)(EM_DATA *data, char *szOut);

typedef struct tag_FA_CODEC {
	DWORD diStart;       // 数据类型起始位
	DWORD diEnd; 		 // 数据类型结束位
	WORD wStep;         // 步长
	FA_ENCODE_PF pfEncode;   	 // 编码至协议层表示
	FA_DECODE_PF pfDecode;      // 解码至应用层表示
	FA_OUTPUT_PF pfOutput;		 // 格式化输出
} FA_CODEC;

/********************************************************************************
 *																				*
 *                                 数据转换定义									*
 *																				*
 *     		命名规则:																*
 *         		转换到协议层表示:  CFP_XXX...XXX									*
 *	      		返回转换后规约数据长度												*
 *         		转换到应用层表示:  CFA_XXX...XXX									*
 *         		返回转换后数据长度													*
 *																				*
 *********************************************************************************/

// NN
static int CFP_BYTE(BYTE *lpBuf, void *pSrc) {
	hex2bcd(((BYTE*) pSrc)[0], &lpBuf[0]);
	return 1;
}

static int CFA_BYTE(void *pBody, BYTE *lpBuf) {
	((BYTE*) pBody)[0] = bcd2hex(lpBuf[0]);
	return 1;
}

static int CPrint_BYTE(EM_DATA* pData, char* szOut) {
	sprintf(szOut, "%d", *(BYTE*) (pData->DataPtr));
	return 0;
}

// NNNN
static int CFP_WORD(BYTE *lpBuf, void *pSrc) {
	WORD wValue = 0;
	memcpy(&wValue, pSrc, sizeof(WORD));
	word2bcd(wValue, lpBuf);
	return 2;
}

static int CFA_WORD(void *pBody, BYTE *lpBuf) {
	WORD wValue = bcd2word(lpBuf);
	memcpy(pBody, &wValue, sizeof(WORD));
	return 2;
}

static int CPrint_WORD(EM_DATA* pData, char* szOut) {
	sprintf(szOut, "%d", *(WORD*) (pData->DataPtr));
	return 0;
}

// NNNNNNNN
static int CFP_DWORD(BYTE *lpBuf, void *pSrc) {
	DWORD dwValue = 0;
	memcpy(&dwValue, pSrc, sizeof(DWORD));
	long2bcd(dwValue, lpBuf);
	return 4;
}

static int CFA_DWORD(void *pBody, BYTE *lpBuf) {
	DWORD dwValue = 0;
	bcd2long(lpBuf, &dwValue);
	memcpy(pBody, &dwValue, sizeof(DWORD));
	return 4;
}

static int CPrint_DWORD(EM_DATA* pData, char* szOut) {
	sprintf(szOut, "%ld", *(DWORD*) (pData->DataPtr));
	return 0;
}
/*
 // NNNNNN.NN 有功电能量
 static int CFP_LONGLONG_62( BYTE *lpBuf, void *pSrc )
 {
 double dbValue = 0;

 dbValue = (double)(*(unsigned long long *)pSrc) / 10000;
 double2bcd( dbValue, lpBuf, 6, 2 );
 return 4;
 }

 static int CFA_LONGLONG_62( void *pBody, BYTE *lpBuf )
 {
 double dbValue = bcd2double( lpBuf, 4, 2 );
 unsigned long long tagetValue;

 tagetValue = dbValue * 10000;
 memcpy( pBody, &tagetValue, sizeof(unsigned long long) );
 return 4;
 }
 */
// NNNNNN.NN
static int CFP_DOUBLE_62(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 6, 2);
	return 4;
}

static int CFA_DOUBLE_62(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 4, 2);
	memcpy(pBody, &dbValue, sizeof(double));
	return 4;
}
/*
 static int CPrint_DOUBLE_62(EM_DATA* pData, char* szOut)
 {
 double dbVal = *(double*)(pData->DataPtr);
 if( (dbVal <= 0.001) && (dbVal >= -0.001) )
 dbVal = 0.0;
 sprintf(szOut, "%.2f", dbVal);
 return 0;
 }*/

// NNNN.NNNN
static int CFP_DOUBLE_44(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 4, 4);
	return 4;
}

static int CFA_DOUBLE_44(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 4, 4);
	memcpy(pBody, &dbValue, sizeof(double));
	return 4;
}

// NNNNNN.NN
static int CFP_ENERGY(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 6, 2);
	return 4;
}

static int CFA_ENERGY(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 4, 2);
	memcpy(pBody, &dbValue, sizeof(double));
	return 4;
}

static int CPrint_ENERGY(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.001) && (dbVal >= -0.001))
		dbVal = 0.0;
	sprintf(szOut, "%.2f", dbVal);
	return 0;
}

static int CFP_DI000XFF0X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI000XFF00 *pFA = (MT_DI000XFF00*) pSrc;

	for (BYTE i = 0; i < 5; i++) {
		nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00000X00[i]);
	}

	return nPos;
}

static int CFA_DI000XFF0X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI000XFF00 *pFA = (MT_DI000XFF00*) pBody;

	for (BYTE i = 0; i < 5; i++) {
		nPos += CFA_ENERGY(&pFA->DT00000X00[i], &lpBuf[nPos]);
	}

	return nPos;
}

// NN.NNNN
static int CFP_DEMAND(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 2, 4);
	return 3;
}

static int CFA_DEMAND(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 8, 4);
	memcpy(pBody, &dbValue, sizeof(double));
	return 3;
}

static int CPrint_DEMAND(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.00001) && (dbVal >= -0.00001))
		dbVal = 0.0;
	sprintf(szOut, "%.4f", dbVal);
	return 3;
}

static int CFP_DI_TIME(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI_TIME *pFA = (MT_DI_TIME*) pSrc;
	//
	hex2bcd(pFA->minute, &lpBuf[nPos++]);
	hex2bcd(pFA->hour, &lpBuf[nPos++]);
	hex2bcd(pFA->day, &lpBuf[nPos++]);
	hex2bcd(pFA->month, &lpBuf[nPos++]);
	hex2bcd(pFA->year, &lpBuf[nPos++]);
	//
	return nPos;
}

static int CFA_DI_TIME(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI_TIME *pFA = (MT_DI_TIME*) pBody;
	//
	pFA->minute = bcd2hex(lpBuf[nPos++]);
	pFA->hour = bcd2hex(lpBuf[nPos++]);
	pFA->day = bcd2hex(lpBuf[nPos++]);
	pFA->month = bcd2hex(lpBuf[nPos++]);
	pFA->year = bcd2hex(lpBuf[nPos++]);
	//
	return nPos;
}

// YYMMDDHHMM
static int CPrint_DI_TIME(EM_DATA* pData, char* szOut) {
	MT_DI_TIME* pFA = (MT_DI_TIME*) (pData->DataPtr);
	sprintf(szOut, "%02d-%02d-%02d %02d:%02d", pFA->year, pFA->month, pFA->day,
			pFA->hour, pFA->minute);
	return 0;
}

static int CFP_DI_TIMEYS(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI_TIMEYS *pFA = (MT_DI_TIMEYS*) pSrc;
	//
	hex2bcd(pFA->second, &lpBuf[nPos++]);
	hex2bcd(pFA->minute, &lpBuf[nPos++]);
	hex2bcd(pFA->hour, &lpBuf[nPos++]);
	hex2bcd(pFA->day, &lpBuf[nPos++]);
	hex2bcd(pFA->month, &lpBuf[nPos++]);
	hex2bcd(pFA->year, &lpBuf[nPos++]);
	//
	return nPos;
}

static int CFA_DI_TIMEYS(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI_TIMEYS *pFA = (MT_DI_TIMEYS*) pBody;
	//
	pFA->second = bcd2hex(lpBuf[nPos++]);
	pFA->minute = bcd2hex(lpBuf[nPos++]);
	pFA->hour = bcd2hex(lpBuf[nPos++]);
	pFA->day = bcd2hex(lpBuf[nPos++]);
	pFA->month = bcd2hex(lpBuf[nPos++]);
	pFA->year = bcd2hex(lpBuf[nPos++]);
	//
	return nPos;
}

static int CFP_XLVAL_TIME(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI010X0X00 *pFA = (MT_DI010X0X00*) pSrc;

	nPos += CFP_DEMAND(&lpBuf[nPos], &pFA->value);
	nPos += CFP_DI_TIME(&lpBuf[nPos], &pFA->demandTime);
	return nPos;
}

static int CFA_XLVAL_TIME(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI010X0X00 *pFA = (MT_DI010X0X00*) pBody;

	nPos += CFA_DEMAND(&pFA->value, &lpBuf[nPos]);
	nPos += CFA_DI_TIME(&pFA->demandTime, &lpBuf[nPos]);
	return nPos;
}

static int CPrint_XLVAL_TIME(EM_DATA* pData, char* szOut) {
	int nPos = 0;

	nPos += CPrint_DEMAND(pData, szOut);

	BYTE *pOffset = (BYTE *) pData->DataPtr;

	pOffset += nPos;

	pData->DataPtr = pOffset;
	CPrint_DI_TIME(pData, szOut);
	return 0;
}

static int CFP_LONG3BYTE(BYTE *lpBuf, void *pSrc) {
	MT_DI10000001 *pFA = (MT_DI10000001 *) pSrc;

	long2bcd(pFA->times, &lpBuf[0], &lpBuf[1], &lpBuf[2], &lpBuf[3]);
	return 3;
}

static int CFA_LONG3BYTE(void *pBody, BYTE *lpBuf) {
	MT_DI10000001 *pFA = (MT_DI10000001*) pBody;
	bcd2long(lpBuf[0], lpBuf[1], lpBuf[2], 0, &pFA->times);
	return 3;
}

// NNNNNN
static int CPrint_LONG3BYTE(EM_DATA* pData, char* szOut) {
	MT_DI10000001* pFA = (MT_DI10000001*) (pData->DataPtr);
	sprintf(szOut, "%ld", pFA->times);
	return 0;
}

static int CFP_DI010XFF0X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI010XFF00 *pFA = (MT_DI010XFF00*) pSrc;

	for (BYTE i = 0; i < 5; i++) {
		nPos += CFP_XLVAL_TIME(&lpBuf[nPos], &pFA->DT01010X00[i]);
	}
	return nPos;
}

static int CFA_DI010XFF0X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI010XFF00 *pFA = (MT_DI010XFF00*) pBody;

	for (BYTE i = 0; i < 5; i++) {
		nPos += CFA_XLVAL_TIME(&pFA->DT01010X00[i], &lpBuf[nPos]);
	}

	return nPos;
}

static int CFP_DI0201FF00(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0201FF00 *pFA = (MT_DI0201FF00*) pSrc;

	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_WORD(&lpBuf[nPos], &pFA->DT02010X00[i]);
	}
	return nPos;
}

static int CFA_DI0201FF00(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI0201FF00 *pFA = (MT_DI0201FF00*) pBody;

	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_WORD(&pFA->DT02010X00[i], &lpBuf[nPos]);
	}

	return nPos;
}

static int CFP_VOLTAGE(BYTE *lpBuf, void *pSrc) {
	return CFP_WORD(lpBuf, pSrc);
}

static int CFA_VOLTAGE(void *pBody, BYTE *lpBuf) {
	return CFA_WORD(pBody, lpBuf);
}

static int CPrint_VOLTAGE(EM_DATA* pData, char* szOut) {
	CPrint_WORD(pData, szOut);
	return 0;
}

static int CFP_CURRENT(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 3, 3);
	return 3;
}

static int CFA_CURRENT(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 3, 3);
	memcpy(pBody, &dbValue, sizeof(double));
	return 3;
}

static int CPrint_CURRENT(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.001) && (dbVal >= -0.001))
		dbVal = 0.0;
	sprintf(szOut, "%.3f", dbVal);
	return 0;
}

static int CFP_DI0202FF00(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0202FF00 *pFA = (MT_DI0202FF00*) pSrc;

	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_CURRENT(&lpBuf[nPos], &pFA->DT02020X00[i]);
	}
	return nPos;
}

static int CFA_DI0202FF00(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI0202FF00 *pFA = (MT_DI0202FF00*) pBody;

	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_CURRENT(&pFA->DT02020X00[i], &lpBuf[nPos]);
	}

	return nPos;
}

static int CFP_POWER(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 2, 4);
	return 3;
}

static int CFA_POWER(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 3, 4);
	memcpy(pBody, &dbValue, sizeof(double));
	return 3;
}

static int CPrint_POWER(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.001) && (dbVal >= -0.001))
		dbVal = 0.0;
	sprintf(szOut, "%.4f", dbVal);
	return 0;
}

static int CFP_DI0203FF00(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0203FF00 *pFA = (MT_DI0203FF00*) pSrc;

	for (BYTE i = 0; i < 4; i++) {
		nPos += CFP_POWER(&lpBuf[nPos], &pFA->DT02030X00[i]);
	}
	return nPos;
}

static int CFA_DI0203FF00(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI0203FF00 *pFA = (MT_DI0203FF00*) pBody;

	for (BYTE i = 0; i < 4; i++) {
		nPos += CFA_POWER(&pFA->DT02030X00[i], &lpBuf[nPos]);
	}

	return nPos;
}

static int CFP_PF(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 1, 3);
	return 2;
}

static int CFA_PF(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 2, 3);
	memcpy(pBody, &dbValue, sizeof(double));
	return 2;
}

static int CPrint_PF(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.001) && (dbVal >= -0.001))
		dbVal = 0.0;
	sprintf(szOut, "%.4f", dbVal);
	return 0;
}

static int CFP_DI0206FF00(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0206FF00 *pFA = (MT_DI0206FF00*) pSrc;

	for (BYTE i = 0; i < 4; i++) {
		nPos += CFP_PF(&lpBuf[nPos], &pFA->DT02060X00[i]);
	}
	return nPos;
}

static int CFA_DI0206FF00(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI0206FF00 *pFA = (MT_DI0206FF00*) pBody;

	for (BYTE i = 0; i < 4; i++) {
		nPos += CFA_PF(&pFA->DT02060X00[i], &lpBuf[nPos]);
	}

	return nPos;
}

static int CFP_PANGLE(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 3, 1);
	return 2;
}

static int CFA_PANGLE(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 2, 1);
	memcpy(pBody, &dbValue, sizeof(double));
	return 2;
}

static int CPrint_PANGLE(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.001) && (dbVal >= -0.001))
		dbVal = 0.0;
	sprintf(szOut, "%.1f", dbVal);
	return 0;
}

static int CFP_DI0207FF00(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0207FF00 *pFA = (MT_DI0207FF00*) pSrc;

	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_POWER(&lpBuf[nPos], &pFA->DT02070X00[i]);
	}
	return nPos;
}

static int CFA_DI0207FF00(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI0207FF00 *pFA = (MT_DI0207FF00*) pBody;

	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_POWER(&pFA->DT02070X00[i], &lpBuf[nPos]);
	}

	return nPos;
}

static int CFP_HRRATE(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));

	dbValue *= 100;
	double2bcd(dbValue, lpBuf, 3, 1);
	return 2;
}

static int CFA_HRRATE(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 2, 1);

	dbValue /= 100;
	memcpy(pBody, &dbValue, sizeof(double));
	return 2;
}

static int CPrint_HRRATE(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.001) && (dbVal >= -0.001))
		dbVal = 0.0;
	sprintf(szOut, "%.1f", dbVal);
	return 0;
}

static int CFP_DI020A0XFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI020A0XFF *pFA = (MT_DI020A0XFF*) pSrc;

	for (BYTE i = 0; i < 15; i++) {
		nPos += CFP_POWER(&lpBuf[nPos], &pFA->DT020A0X0X[i]);
	}
	return nPos;
}

static int CFA_DI020A0XFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI020A0XFF *pFA = (MT_DI020A0XFF*) pBody;

	for (BYTE i = 0; i < 15; i++) {
		nPos += CFA_POWER(&pFA->DT020A0X0X[i], &lpBuf[nPos]);
	}

	return nPos;
}

/*
 // NNNNNNN.N
 static int CFP_DOUBLE_71( BYTE *lpBuf, void *pSrc )
 {
 double dbValue = 0;
 memcpy( &dbValue, pSrc, sizeof(double) );
 double2bcd( dbValue, lpBuf, 7, 1 );
 return 4;
 }

 static int CFA_DOUBLE_71( void *pBody, BYTE *lpBuf )
 {
 double dbValue = bcd2double( lpBuf, 4, 1 );
 memcpy( pBody, &dbValue, sizeof(double) );
 return 4;
 }

 static int CPrint_DOUBLE_71(EM_DATA* pData, char* szOut)
 {
 double dbVal = *(double*)(pData->DataPtr);
 if( (dbVal <= 0.01) && (dbVal >= -0.01) )
 dbVal = 0.0;
 sprintf(szOut, "%.1f", dbVal);
 return 0;
 }*/

// NN.NNNN
static int CFP_DOUBLE_24(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 2, 4);
	return 3;
}

static int CFA_DOUBLE_24(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 3, 4);
	memcpy(pBody, &dbValue, sizeof(double));
	return 3;
}
/*
 static int CPrint_DOUBLE_24(EM_DATA* pData, char* szOut)
 {
 double dbVal = *(double*)(pData->DataPtr);
 if( (dbVal <= 0.00001) && (dbVal >= -0.00001) )
 dbVal = 0.0;
 sprintf(szOut, "%.4f", dbVal);
 return 0;
 }*/

// N.NNN
static int CFP_DOUBLE_13(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 1, 3);
	return 2;
}

static int CFA_DOUBLE_13(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 2, 3);
	memcpy(pBody, &dbValue, sizeof(double));
	return 3;
}
/*
 static int CPrint_DOUBLE_13(EM_DATA* pData, char* szOut)
 {
 double dbVal = *(double*)(pData->DataPtr);
 if( (dbVal <= 0.0001) && (dbVal >= -0.0001) )
 dbVal = 0.0;
 sprintf(szOut, "%.3f", dbVal);
 return 0;
 }*/

// NN.NN
static int CFP_DOUBLE_22(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 2, 2);
	return 2;
}

static int CFA_DOUBLE_22(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 2, 2);
	memcpy(pBody, &dbValue, sizeof(double));
	return 2;
}

static int CPrint_DOUBLE_22(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.001) && (dbVal >= -0.001))
		dbVal = 0.0;
	sprintf(szOut, "%.2f", dbVal);
	return 0;
}

// NNN.N
static int CFP_DOUBLE_31(BYTE *lpBuf, void *pSrc) {
	double dbValue = 0;
	memcpy(&dbValue, pSrc, sizeof(double));
	double2bcd(dbValue, lpBuf, 3, 1);
	return 2;
}

static int CFA_DOUBLE_31(void *pBody, BYTE *lpBuf) {
	double dbValue = bcd2double(lpBuf, 2, 1);
	memcpy(pBody, &dbValue, sizeof(double));
	return 2;
}

static int CPrint_DOUBLE_31(EM_DATA* pData, char* szOut) {
	double dbVal = *(double*) (pData->DataPtr);
	if ((dbVal <= 0.01) && (dbVal >= -0.01))
		dbVal = 0.0;
	sprintf(szOut, "%.1f", dbVal);
	return 0;
}
/*
 // 0.NN
 static int CFP_DOUBLE_02( BYTE *lpBuf, void *pSrc )
 {
 double dbValue = 0;
 memcpy( &dbValue, pSrc, sizeof(double) );
 double2bcd( dbValue, lpBuf, 0, 2 );
 return 1;
 }

 static int CFA_DOUBLE_02( void *pBody, BYTE *lpBuf )
 {
 double dbValue = bcd2double( lpBuf, 1, 2 );
 memcpy( pBody, &dbValue, sizeof(double) );
 return 1;
 }

 static int CPrint_DOUBLE_02(EM_DATA* pData, char* szOut)
 {
 double dbVal = *(double*)(pData->DataPtr);
 if( (dbVal <= 0.001) && (dbVal >= -0.001) )
 dbVal = 0.0;
 sprintf(szOut, "%.2f", dbVal);
 return 0;
 }*/

// 不可轮显项
static int CPrint_NONE(EM_DATA*, char* szOut) {
	sprintf(szOut, "nullptr");
	return 0;
}

//////////////////////////////////////////////////////////
// 日期
static int CFP_DI04000101(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000101 *pFA = (EM_DI04000101*) pSrc;
	//
	if (pFA == nullptr) {
		time_t hTime = time(nullptr);
		tm tm_sys = *localtime(&hTime);

		hex2bcd((BYTE) tm_sys.tm_wday, &lpBuf[nPos++]);
		hex2bcd((BYTE) tm_sys.tm_mday, &lpBuf[nPos++]);
		hex2bcd((BYTE) tm_sys.tm_mon + 1, &lpBuf[nPos++]);
		hex2bcd((BYTE) (tm_sys.tm_year % 100), &lpBuf[nPos++]);
	} else {
		hex2bcd(pFA->week, &lpBuf[nPos++]);
		hex2bcd(pFA->day, &lpBuf[nPos++]);
		hex2bcd(pFA->month, &lpBuf[nPos++]);
		hex2bcd(pFA->year, &lpBuf[nPos++]);
	}
	//
	return nPos;
}

static int CFA_DI04000101(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI04000101 *pFA = (EM_DI04000101*) pBody;
	//
	pFA->week = bcd2hex(lpBuf[nPos++]);
	pFA->day = bcd2hex(lpBuf[nPos++]);
	pFA->month = bcd2hex(lpBuf[nPos++]);
	pFA->year = bcd2hex(lpBuf[nPos++]);
	//
	return nPos;
}

// YYMMDD WW
static int CPrint_DI04000101(EM_DATA* pData, char* szOut) {
	EM_DI04000101* pFA = (EM_DI04000101*) (pData->DataPtr);
	sprintf(szOut, "%02d-%02d-%02d %02d", pFA->year, pFA->month, pFA->day,
			pFA->week);
	return 0;
}

//////////////////////////////////////////////////////////
// 时间
static int CFP_DI04000102(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000102 *pFA = (EM_DI04000102*) pSrc;
	//
	if (pFA == nullptr) {
		time_t hTime = time(nullptr);
		tm tm_sys = *localtime(&hTime);

		hex2bcd((BYTE) tm_sys.tm_sec, &lpBuf[nPos++]);
		hex2bcd((BYTE) tm_sys.tm_min, &lpBuf[nPos++]);
		hex2bcd((BYTE) tm_sys.tm_hour, &lpBuf[nPos++]);
	} else {
		hex2bcd(pFA->second, &lpBuf[nPos++]);
		hex2bcd(pFA->minute, &lpBuf[nPos++]);
		hex2bcd(pFA->hour, &lpBuf[nPos++]);
	}
	//
	return nPos;
}

static int CFA_DI04000102(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI04000102 *pFA = (EM_DI04000102*) pBody;
	//
	pFA->second = bcd2hex(lpBuf[nPos++]);
	pFA->minute = bcd2hex(lpBuf[nPos++]);
	pFA->hour = bcd2hex(lpBuf[nPos++]);
	//
	return nPos;
}

// hhmmss
static int CPrint_DI04000102(EM_DATA* pData, char* szOut) {
	EM_DI04000102* pFA = (EM_DI04000102*) (pData->DataPtr);
	sprintf(szOut, "%02d:%02d:%02d", pFA->hour, pFA->minute, pFA->second);
	return 0;
}

// 040001FF
static int CFP_DI040001FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040001FF *pFA = (EM_DI040001FF*) pSrc;

	nPos += CFP_DI04000101(&lpBuf[nPos], &pFA->DI04000101);
	nPos += CFP_DI04000102(&lpBuf[nPos], &pFA->DI04000102);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000103);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000104);
	nPos += CFP_WORD(&lpBuf[nPos], &pFA->DI04000105);
	nPos += CFP_DI_TIME(&lpBuf[nPos], &pFA->DI04000106);
	nPos += CFP_DI_TIME(&lpBuf[nPos], &pFA->DI04000107);
	nPos += CFP_DI_TIME(&lpBuf[nPos], &pFA->DI04000108);
	nPos += CFP_DI_TIME(&lpBuf[nPos], &pFA->DI04000109);
	return nPos;
}

static int CFA_DI040001FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040001FF *pFA = (EM_DI040001FF*) pBody;

	nPos += CFA_DI04000101(&pFA->DI04000101, &lpBuf[nPos]);
	nPos += CFA_DI04000102(&pFA->DI04000102, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000103, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000104, &lpBuf[nPos]);
	nPos += CFA_WORD(&pFA->DI04000105, &lpBuf[nPos]);
	nPos += CFA_DI_TIME(&pFA->DI04000106, &lpBuf[nPos]);
	nPos += CFA_DI_TIME(&pFA->DI04000107, &lpBuf[nPos]);
	nPos += CFA_DI_TIME(&pFA->DI04000108, &lpBuf[nPos]);
	nPos += CFA_DI_TIME(&pFA->DI04000109, &lpBuf[nPos]);

	return nPos;
}

// 040002FF
static int CFP_DI040002FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040002FF *pFA = (EM_DI040002FF*) pSrc;

	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000201);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000202);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000203);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000204);
	nPos += CFP_WORD(&lpBuf[nPos], &pFA->DI04000205);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000206);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000207);
	;
	return nPos;
}

static int CFA_DI040002FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040002FF *pFA = (EM_DI040002FF*) pBody;

	nPos += CFA_BYTE(&pFA->DI04000201, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000202, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000203, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000204, &lpBuf[nPos]);
	nPos += CFA_WORD(&pFA->DI04000205, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000206, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000207, &lpBuf[nPos]);
	return nPos;
}

// 040003FF
static int CFP_DI040003FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040003FF *pFA = (EM_DI040003FF*) pSrc;

	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000301);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000302);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000303);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000304);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000305);
	nPos += CFP_DWORD(&lpBuf[nPos], &pFA->DI04000306);
	nPos += CFP_DWORD(&lpBuf[nPos], &pFA->DI04000307);

	return nPos;
}

static int CFA_DI040003FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040003FF *pFA = (EM_DI040003FF*) pBody;

	nPos += CFA_BYTE(&pFA->DI04000301, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000302, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000303, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000304, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000305, &lpBuf[nPos]);
	nPos += CFA_DWORD(&pFA->DI04000306, &lpBuf[nPos]);
	nPos += CFA_DWORD(&pFA->DI04000307, &lpBuf[nPos]);
	return nPos;
}

static int CFP_DI04000401(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000401 *pFA = (EM_DI04000401*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->address, 6);
	nPos += 6;
	return nPos;
}

static int CFA_DI04000401(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000401 *pFA = (EM_DI04000401*) pBody;

	memcpy(pFA->address, (BYTE*) &lpBuf[nPos], 6);
	nPos += 6;

	return nPos;
}

static int CFP_DI04000402(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000402 *pFA = (EM_DI04000402*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->host, 6);
	nPos += 6;
	return nPos;
}

static int CFA_DI04000402(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000402 *pFA = (EM_DI04000402*) pBody;

	memcpy(pFA->host, (BYTE*) &lpBuf[nPos], 6);
	nPos += 6;

	return nPos;
}

static int CFP_DI04000403(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000403 *pFA = (EM_DI04000403*) pSrc;

	BYTE size = sizeof(pFA->propertyNum);

	memcpy((BYTE*) &lpBuf[nPos], pFA->propertyNum, size);
	nPos += size;
	return nPos;
}

static int CFA_DI04000403(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000403 *pFA = (EM_DI04000403*) pBody;

	BYTE size = sizeof(pFA->propertyNum);
	memcpy(pFA->propertyNum, (BYTE*) &lpBuf[nPos], size);
	nPos += size;

	return nPos;
}

static int CFP_DI04000404(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000404 *pFA = (EM_DI04000404*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->ratedVolchar, 6);
	nPos += 6;
	return nPos;
}

static int CFA_DI04000404(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	DWORD interger = 0;
	DWORD decimal = 0;
	DWORD curr = 0;
	BYTE point = 0;		 //是否有小数位

	EM_DI04000404 *pFA = (EM_DI04000404*) pBody;

	for (nPos = 5; nPos >= 0; nPos--) {
		if ((lpBuf[nPos] >= 0x30) && (lpBuf[nPos] <= 0x39)) {
			curr = curr * 10 + (lpBuf[nPos] - 0x30);
		} else if (lpBuf[nPos] == 0x2E)		 //小数点
				{
			interger = curr;
			curr = 0;
			point = 1;
			continue;
		} else if (curr == 0) {
			continue;
		} else
			break;
	}
	if (point == 1) {
		decimal = curr;
	} else {
		interger = curr;		 //整数位
	}
	pFA->ratedVol = interger;

	if (decimal < 10) {
		pFA->ratedVol += (double) decimal / 10;
	} else if (decimal < 100) {
		pFA->ratedVol += (double) decimal / 100;
	} else if (decimal < 1000) {
		pFA->ratedVol += (double) decimal / 1000;
	} else if (decimal < 10000) {
		pFA->ratedVol += (double) decimal / 10000;
	} else if (decimal < 100000) {
		pFA->ratedVol += (double) decimal / 100000;
	}

	pFA->ratedVol *= 10;

	nPos = 0;

	memcpy(pFA->ratedVolchar, (BYTE*) &lpBuf[nPos], 6);
	nPos += 6;

	return nPos;
}

static int CFP_DI04000405(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000405 *pFA = (EM_DI04000405*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->ratedCurrchar, 6);
	nPos += 6;
	return nPos;
}

static int CFA_DI04000405(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	DWORD interger = 0;
	DWORD decimal = 0;
	DWORD curr = 0;
	BYTE point = 0;		 //是否有小数位

	EM_DI04000405 *pFA = (EM_DI04000405*) pBody;

	for (nPos = 5; nPos >= 0; nPos--) {
		if ((lpBuf[nPos] >= 0x30) && (lpBuf[nPos] <= 0x39)) {
			curr = curr * 10 + (lpBuf[nPos] - 0x30);
		} else if (lpBuf[nPos] == 0x2E)		 //小数点
				{
			interger = curr;
			curr = 0;
			point = 1;
			continue;
		} else if (curr == 0) {
			continue;
		} else
			break;
	}

	if (point == 1) {
		decimal = curr;
	} else {
		interger = curr;		 //整数位
	}

	pFA->ratedCurr = interger;

	if (decimal < 10) {
		pFA->ratedCurr += (double) decimal / 10;
	} else if (decimal < 100) {
		pFA->ratedCurr += (double) decimal / 100;
	} else if (decimal < 1000) {
		pFA->ratedCurr += (double) decimal / 1000;
	} else if (decimal < 10000) {
		pFA->ratedCurr += (double) decimal / 10000;
	} else if (decimal < 100000) {
		pFA->ratedCurr += (double) decimal / 100000;
	}

	nPos = 0;

	memcpy(pFA->ratedCurrchar, (BYTE*) &lpBuf[nPos], 6);
	nPos += 6;

	return nPos;
}

static int CFP_DI04000406(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000406 *pFA = (EM_DI04000406*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->maxCurrchar, 6);
	nPos += 6;
	return nPos;
}

static int CFA_DI04000406(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	DWORD interger = 0;
	DWORD decimal = 0;
	DWORD curr = 0;
	BYTE point = 0;		 //是否有小数位

	EM_DI04000406 *pFA = (EM_DI04000406*) pBody;

	for (nPos = 5; nPos >= 0; nPos--) {
		if ((lpBuf[nPos] >= 0x30) && (lpBuf[nPos] <= 0x39)) {
			curr = curr * 10 + (lpBuf[nPos] - 0x30);
		} else if (lpBuf[nPos] == 0x2E)		 //小数点
				{
			interger = curr;
			curr = 0;
			point = 1;
			continue;
		} else if (curr == 0) {
			continue;
		} else
			break;
	}
	if (point == 1) {
		decimal = curr;
	} else {
		interger = curr;		 //整数位
	}

	pFA->maxCurr = interger;

	if (decimal < 10) {
		pFA->maxCurr += (double) decimal / 10;
	} else if (decimal < 100) {
		pFA->maxCurr += (double) decimal / 100;
	} else if (decimal < 1000) {
		pFA->maxCurr += (double) decimal / 1000;
	} else if (decimal < 10000) {
		pFA->maxCurr += (double) decimal / 10000;
	} else if (decimal < 100000) {
		pFA->maxCurr += (double) decimal / 100000;
	}

	nPos = 0;
	memcpy(pFA->maxCurrchar, (BYTE*) &lpBuf[nPos], 6);
	nPos += 6;

	return nPos;
}

static int CFP_DI04000407(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000407 *pFA = (EM_DI04000407*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->actpowerGrade, 4);
	nPos += 4;
	return nPos;
}

static int CFA_DI04000407(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000407 *pFA = (EM_DI04000407*) pBody;

	memcpy(pFA->actpowerGrade, (BYTE*) &lpBuf[nPos], 4);
	nPos += 4;

	return nPos;
}

static int CFP_DI04000408(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000408 *pFA = (EM_DI04000408*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->inactpowerGrade, 4);
	nPos += 4;
	return nPos;
}

static int CFA_DI04000408(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000408 *pFA = (EM_DI04000408*) pBody;

	memcpy(pFA->inactpowerGrade, (BYTE*) &lpBuf[nPos], 4);
	nPos += 4;

	return nPos;
}

static int CFP_DI04000409(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000409 *pFA = (EM_DI04000409*) pSrc;

	long2bcd(pFA->value, &lpBuf[0], &lpBuf[1], &lpBuf[2], &lpBuf[3]);
	nPos += 4;

	return nPos;
}

static int CFA_DI04000409(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI04000409 *pFA = (EM_DI04000409*) pBody;

	bcd2long(lpBuf[0], lpBuf[1], lpBuf[2], lpBuf[3], &pFA->value);
	nPos += 4;

	return nPos;
}

static int CFP_DI0400040A(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI0400040A *pFA = (EM_DI0400040A*) pSrc;

	long2bcd(pFA->value, &lpBuf[0], &lpBuf[1], &lpBuf[2], &lpBuf[3]);
	nPos += 4;

	return nPos;
}

static int CFA_DI0400040A(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI0400040A *pFA = (EM_DI0400040A*) pBody;

	bcd2long(lpBuf[0], lpBuf[1], lpBuf[2], lpBuf[3], &pFA->value);
	nPos += 4;

	return nPos;
}

static int CFP_DI0400040B(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI0400040B *pFA = (EM_DI0400040B*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->meterType, 4);
	nPos += 4;

	return nPos;
}

static int CFA_DI0400040B(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI0400040B *pFA = (EM_DI0400040B*) pBody;

	memcpy(pFA->meterType, (BYTE*) &lpBuf[nPos], 4);
	nPos += 4;

	return nPos;
}

static int CFP_DI0400040C(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI0400040C *pFA = (EM_DI0400040C*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->manudate, 10);
	nPos += 10;

	return nPos;
}

static int CFA_DI0400040C(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI0400040C *pFA = (EM_DI0400040C*) pBody;

	memcpy(pFA->manudate, (BYTE*) &lpBuf[nPos], 10);
	nPos += 10;

	return nPos;
}

static int CFP_DI0400040D(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI0400040D *pFA = (EM_DI0400040D*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->prtlversion, 16);
	nPos += 16;

	return nPos;
}

static int CFA_DI0400040D(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI0400040D *pFA = (EM_DI0400040D*) pBody;

	memcpy(pFA->prtlversion, (BYTE*) &lpBuf[nPos], 16);
	nPos += 16;

	return nPos;
}

static int CFP_DI0400040E(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI0400040E *pFA = (EM_DI0400040E*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->clientnum, 6);
	nPos += 6;

	return nPos;
}

static int CFA_DI0400040E(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI0400040E *pFA = (EM_DI0400040E*) pBody;

	memcpy(pFA->clientnum, (BYTE*) &lpBuf[nPos], 6);
	nPos += 6;

	return nPos;
}

// 040004FF
static int CFP_DI040004FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040004FF *pFA = (EM_DI040004FF*) pSrc;

	nPos += CFP_DI04000401(&lpBuf[nPos], &pFA->DI04000401);
	nPos += CFP_DI04000402(&lpBuf[nPos], &pFA->DI04000402);
	nPos += CFP_DI04000403(&lpBuf[nPos], &pFA->DI04000403);
	nPos += CFP_DI04000404(&lpBuf[nPos], &pFA->DI04000404);
	nPos += CFP_DI04000405(&lpBuf[nPos], &pFA->DI04000405);
	nPos += CFP_DI04000406(&lpBuf[nPos], &pFA->DI04000406);
	nPos += CFP_DI04000407(&lpBuf[nPos], &pFA->DI04000407);
	nPos += CFP_DI04000408(&lpBuf[nPos], &pFA->DI04000408);
	nPos += CFP_DI04000409(&lpBuf[nPos], &pFA->DI04000409);
	nPos += CFP_DI0400040A(&lpBuf[nPos], &pFA->DI0400040A);
	nPos += CFP_DI0400040B(&lpBuf[nPos], &pFA->DI0400040B);
	nPos += CFP_DI0400040C(&lpBuf[nPos], &pFA->DI0400040C);
	nPos += CFP_DI0400040D(&lpBuf[nPos], &pFA->DI0400040D);
	nPos += CFP_DI0400040E(&lpBuf[nPos], &pFA->DI0400040E);
	return nPos;
}

static int CFA_DI040004FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040004FF *pFA = (EM_DI040004FF*) pBody;

	nPos += CFA_DI04000401(&pFA->DI04000401, &lpBuf[nPos]);
	nPos += CFA_DI04000402(&pFA->DI04000402, &lpBuf[nPos]);
	nPos += CFA_DI04000403(&pFA->DI04000403, &lpBuf[nPos]);
	nPos += CFA_DI04000404(&pFA->DI04000404, &lpBuf[nPos]);
	nPos += CFA_DI04000405(&pFA->DI04000405, &lpBuf[nPos]);
	nPos += CFA_DI04000406(&pFA->DI04000406, &lpBuf[nPos]);
	nPos += CFA_DI04000407(&pFA->DI04000407, &lpBuf[nPos]);
	nPos += CFA_DI04000408(&pFA->DI04000408, &lpBuf[nPos]);
	nPos += CFA_DI04000409(&pFA->DI04000409, &lpBuf[nPos]);
	nPos += CFA_DI0400040A(&pFA->DI0400040A, &lpBuf[nPos]);
	nPos += CFA_DI0400040B(&pFA->DI0400040B, &lpBuf[nPos]);
	nPos += CFA_DI0400040C(&pFA->DI0400040C, &lpBuf[nPos]);
	nPos += CFA_DI0400040D(&pFA->DI0400040D, &lpBuf[nPos]);
	nPos += CFA_DI0400040E(&pFA->DI0400040E, &lpBuf[nPos]);
	return nPos;
}
/*

 static int CFP_DI040006FF( BYTE *lpBuf, void *pSrc )
 {
 int nPos = 0;
 EM_DI040006FF *pFA = (EM_DI040006FF*)pSrc;

 memcpy(pFA, lpBuf, 3);
 nPos += 3;

 return nPos;
 }

 static int CFA_DI040006FF( void *pBody, BYTE *lpBuf )
 {
 int nPos = 0;

 EM_DI040006FF *pFA = (EM_DI040006FF*)pBody;

 memcpy(lpBuf, pFA, 3);
 nPos += 3;

 return nPos;
 }

 static int CFP_DI040007FF( BYTE *lpBuf, void *pSrc )
 {
 int nPos = 0;
 EM_DI040007FF *pFA = (EM_DI040007FF*)pSrc;

 nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000701);
 nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000702);
 nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000703);
 nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000704);
 nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000705);

 return nPos;
 }

 static int CFA_DI040007FF( void *pBody, BYTE *lpBuf )
 {
 int nPos = 0;

 EM_DI040007FF *pFA = (EM_DI040007FF*)pBody;

 nPos += CFA_BYTE( &pFA->DI04000701,&lpBuf[nPos]);
 nPos += CFA_BYTE( &pFA->DI04000702,&lpBuf[nPos]);
 nPos += CFA_BYTE( &pFA->DI04000703,&lpBuf[nPos]);
 nPos += CFA_BYTE( &pFA->DI04000704,&lpBuf[nPos]);
 nPos += CFA_BYTE( &pFA->DI04000705,&lpBuf[nPos]);

 return nPos;
 }
 */
static int CFA_DI04000801(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI04000801 *pFA = (EM_DI04000801*) pBody;

	memcpy(&pFA->status, (BYTE*) &lpBuf[nPos], 1);
	nPos += 1;

	return nPos;
}

static int CFP_DI04000801(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000801 *pFA = (EM_DI04000801*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], &pFA->status, 1);
	nPos += 1;

	return nPos;
}

static int CFP_DI040008FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040008FF *pFA = (EM_DI040008FF*) pSrc;

	nPos += CFP_DI04000801(&lpBuf[nPos], &pFA->DI04000801);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04000802);

	return nPos;
}

static int CFA_DI040008FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040008FF *pFA = (EM_DI040008FF*) pBody;

	nPos += CFA_DI04000801(&pFA->DI04000801, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04000802, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04000AFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000AFF *pFA = (EM_DI04000AFF*) pSrc;

	nPos += CFP_DI_TIME(&lpBuf[nPos], &pFA->DI04000A01);
	for (BYTE i = 0; i < 6; i++) {
		nPos += CFP_WORD(&lpBuf[nPos], &pFA->DI04000A02[i]);
	}

	return nPos;
}

static int CFA_DI04000AFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000AFF *pFA = (EM_DI04000AFF*) pBody;

	nPos += CFA_DI_TIME(&pFA->DI04000A01, &lpBuf[nPos]);
	for (BYTE i = 0; i < 6; i++) {
		nPos += CFA_WORD(&pFA->DI04000A02[i], &lpBuf[nPos]);
	}

	return nPos;
}

static int CFP_DI04000B0X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000B0X *pFA = (EM_DI04000B0X*) pSrc;

	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->hour);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->day);

	return nPos;
}

static int CFA_DI04000B0X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000B0X *pFA = (EM_DI04000B0X*) pBody;

	nPos += CFA_BYTE(&pFA->hour, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->day, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04000BFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000BFF *pFA = (EM_DI04000BFF*) pSrc;

	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_DI04000B0X(&lpBuf[nPos], &pFA->DI04000B0X[i]);
	}

	return nPos;
}

static int CFA_DI04000BFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000BFF *pFA = (EM_DI04000BFF*) pBody;

	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_DI04000B0X(&pFA->DI04000B0X[i], &lpBuf[nPos]);
	}
	return nPos;
}

static int CFP_DI04000C0X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000C0X *pFA = (EM_DI04000C0X*) pSrc;

	memcpy((BYTE*) &lpBuf[nPos], pFA->pass, 4);
	nPos += 4;

	return nPos;
}

static int CFA_DI04000C0X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000C0X *pFA = (EM_DI04000C0X*) pBody;

	memcpy(pFA->pass, (BYTE*) &lpBuf[nPos], 4);
	nPos += 4;

	return nPos;
}

static int CFP_DI04000CFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000CFF *pFA = (EM_DI04000CFF*) pSrc;

	for (BYTE i = 0; i < 10; i++) {
		nPos += CFP_DI04000C0X(&lpBuf[nPos], &pFA->DI04000C0X[i]);
	}

	return nPos;
}

static int CFA_DI04000CFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000CFF *pFA = (EM_DI04000CFF*) pBody;

	for (BYTE i = 0; i < 10; i++) {
		nPos += CFA_DI04000C0X(&pFA->DI04000C0X[i], &lpBuf[nPos]);
	}
	return nPos;
}
/*
 static int CFP_DI04000E01( BYTE *lpBuf, void *pSrc )
 {
 int nPos = 0;
 EM_DI04000E01 *pFA = (EM_DI04000E01*)pSrc;

 double2bcd(pFA->powerUpLimit, lpBuf, 2, 4);
 nPos += 3;

 return nPos;
 }
 */
static int CFP_DI04000EFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000EFF *pFA = (EM_DI04000EFF*) pSrc;

	nPos += CFP_DOUBLE_24(&lpBuf[nPos], (void *) &pFA->DI04000E01);
	nPos += CFP_DOUBLE_24(&lpBuf[nPos], (void *) &pFA->DI04000E02);
	nPos += CFP_DOUBLE_31(&lpBuf[nPos], (void *) &pFA->DI04000E03);
	nPos += CFP_DOUBLE_31(&lpBuf[nPos], (void *) &pFA->DI04000E04);

	return nPos;
}

static int CFA_DI04000EFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000EFF *pFA = (EM_DI04000EFF*) pBody;

	nPos += CFA_DOUBLE_24((void *) &pFA->DI04000E01, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_24((void *) &pFA->DI04000E02, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_31((void *) &pFA->DI04000E03, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_31((void *) &pFA->DI04000E04, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04000FFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04000FFF *pFA = (EM_DI04000FFF*) pSrc;

	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04000F01);
	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04000F02);
	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04000F03);
	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04000F04);

	return nPos;
}

static int CFA_DI04000FFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04000FFF *pFA = (EM_DI04000FFF*) pBody;

	nPos += CFA_DOUBLE_62((void *) &pFA->DI04000F01, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_62((void *) &pFA->DI04000F02, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_62((void *) &pFA->DI04000F03, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_62((void *) &pFA->DI04000F04, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040010FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040010FF *pFA = (EM_DI040010FF*) pSrc;

	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04001001);
	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04001002);
	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04001003);
	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04001004);
	nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI04001005);
	return nPos;
}

static int CFA_DI040010FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040010FF *pFA = (EM_DI040010FF*) pBody;

	nPos += CFA_DOUBLE_62((void *) &pFA->DI04001001, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_62((void *) &pFA->DI04001002, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_62((void *) &pFA->DI04001003, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_62((void *) &pFA->DI04001004, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_62((void *) &pFA->DI04001005, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040011FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040011FF *pFA = (EM_DI040011FF*) pSrc;

	nPos += CFP_DI04000801(&lpBuf[nPos], (void *) &pFA->DI04001101);
	return nPos;
}

static int CFA_DI040011FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040011FF *pFA = (EM_DI040011FF*) pBody;

	nPos += CFA_DI04000801((void *) &pFA->DI04001101, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04001203(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04001203 *pFA = (EM_DI04001203*) pSrc;

	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->hour);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->minute);
	return nPos;
}

static int CFA_DI04001203(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04001203 *pFA = (EM_DI04001203*) pBody;

	nPos += CFA_BYTE((void *) &pFA->hour, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->minute, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040012FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040012FF *pFA = (EM_DI040012FF*) pSrc;

	nPos += CFP_DI_TIME(&lpBuf[nPos], (void *) &pFA->DI04001201);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->DI04001202);
	nPos += CFP_DI04001203(&lpBuf[nPos], (void *) &pFA->DI04001203);
	return nPos;
}

static int CFA_DI040012FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040012FF *pFA = (EM_DI040012FF*) pBody;

	nPos += CFA_DI_TIME((void *) &pFA->DI04001201, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->DI04001202, &lpBuf[nPos]);
	nPos += CFA_DI04001203((void *) &pFA->DI04001203, &lpBuf[nPos]);
	return nPos;
}
/*
 static int CFP_DI040013FF( BYTE *lpBuf, void *pSrc )
 {
 int nPos = 0;
 EM_DI040013FF *pFA = (EM_DI040013FF*)pSrc;

 nPos += CFP_BYTE(&lpBuf[nPos], (void *)&pFA->DI04001301);

 return nPos;
 }

 static int CFA_DI040013FF( void *pBody, BYTE *lpBuf )
 {
 int nPos = 0;

 EM_DI040013FF *pFA = (EM_DI040013FF*)pBody;

 nPos += CFA_BYTE((void *)&pFA->DI04001301,&lpBuf[nPos]);
 return nPos;
 }

 static int CFP_DI040014FF( BYTE *lpBuf, void *pSrc )
 {
 int nPos = 0;
 EM_DI040014FF *pFA = (EM_DI040014FF*)pSrc;

 nPos += CFP_WORD(&lpBuf[nPos], (void *)&pFA->DI04001401);

 return nPos;
 }

 static int CFA_DI040014FF( void *pBody, BYTE *lpBuf )
 {
 int nPos = 0;

 EM_DI040014FF *pFA = (EM_DI040014FF*)pBody;

 nPos += CFA_WORD((void *)&pFA->DI04001401,&lpBuf[nPos]);
 return nPos;
 }
 */
static int CFP_DI040600FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040600FF *pFA = (EM_DI040600FF*) pSrc;

	for (BYTE i = 0; i < 14; i++) {
		nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI040600XX[i]);
	}
	return nPos;
}

static int CFA_DI040600FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040600FF *pFA = (EM_DI040600FF*) pBody;

	for (BYTE i = 0; i < 14; i++) {
		nPos += CFA_DOUBLE_62((void *) &pFA->DI040600XX[i], &lpBuf[nPos]);
	}
	return nPos;
}

static int CFP_DI040601FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040601FF *pFA = (EM_DI040601FF*) pSrc;

	for (BYTE i = 0; i < 14; i++) {
		nPos += CFP_DOUBLE_44(&lpBuf[nPos], (void *) &pFA->DI040601XX[i]);
	}
	return nPos;
}

static int CFA_DI040601FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040601FF *pFA = (EM_DI040601FF*) pBody;

	for (BYTE i = 0; i < 14; i++) {
		nPos += CFA_DOUBLE_44((void *) &pFA->DI040601XX[i], &lpBuf[nPos]);
	}
	return nPos;
}

static int CFP_DI040602FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040602FF *pFA = (EM_DI040602FF*) pSrc;

	for (BYTE i = 0; i < 14; i++) {
		nPos += CFP_DOUBLE_62(&lpBuf[nPos], (void *) &pFA->DI040600XX[i]);
	}
	return nPos;
}

static int CFA_DI040602FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040602FF *pFA = (EM_DI040602FF*) pBody;

	for (BYTE i = 0; i < 14; i++) {
		nPos += CFA_DOUBLE_62((void *) &pFA->DI040600XX[i], &lpBuf[nPos]);
	}
	return nPos;
}

static int CFP_DI040603FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040603FF *pFA = (EM_DI040603FF*) pSrc;

	for (BYTE i = 0; i < 14; i++) {
		nPos += CFP_DOUBLE_44(&lpBuf[nPos], (void *) &pFA->DI040601XX[i]);
	}
	return nPos;
}

static int CFA_DI040603FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040603FF *pFA = (EM_DI040603FF*) pBody;

	for (BYTE i = 0; i < 14; i++) {
		nPos += CFA_DOUBLE_44((void *) &pFA->DI040601XX[i], &lpBuf[nPos]);
	}
	return nPos;
}

static int CFP_DI040901FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040901FF *pFA = (EM_DI040901FF*) pSrc;

	nPos += CFP_WORD(&lpBuf[nPos], (void *) &pFA->DI04090101);
	nPos += CFP_WORD(&lpBuf[nPos], (void *) &pFA->DI04090102);
	nPos += CFP_DOUBLE_24(&lpBuf[nPos], (void *) &pFA->DI04090103);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->DI04090104);

	return nPos;
}

static int CFA_DI040901FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040901FF *pFA = (EM_DI040901FF*) pBody;

	nPos += CFA_WORD((void *) &pFA->DI04090101, &lpBuf[nPos]);
	nPos += CFA_WORD((void *) &pFA->DI04090102, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_24((void *) &pFA->DI04090103, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->DI04090104, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040902FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040902FF *pFA = (EM_DI040902FF*) pSrc;

	nPos += CFP_WORD(&lpBuf[nPos], (void *) &pFA->DI04090201);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->DI04090202);

	return nPos;
}

static int CFA_DI040902FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040902FF *pFA = (EM_DI040902FF*) pBody;

	nPos += CFA_WORD((void *) &pFA->DI04090201, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->DI04090202, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040903FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040903FF *pFA = (EM_DI040903FF*) pSrc;

	nPos += CFP_WORD(&lpBuf[nPos], (void *) &pFA->DI04090301);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->DI04090302);

	return nPos;
}

static int CFA_DI040903FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040903FF *pFA = (EM_DI040903FF*) pBody;

	nPos += CFA_WORD((void *) &pFA->DI04090301, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->DI04090302, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040904FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040904FF *pFA = (EM_DI040904FF*) pSrc;

	nPos += CFP_WORD(&lpBuf[nPos], (void *) &pFA->DI04090401);
	nPos += CFP_DOUBLE_24(&lpBuf[nPos], (void *) &pFA->DI04090402);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->DI04090403);

	return nPos;
}

static int CFA_DI040904FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040904FF *pFA = (EM_DI040904FF*) pBody;

	nPos += CFA_WORD((void *) &pFA->DI04090401, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_24((void *) &pFA->DI04090402, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->DI04090403, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040905FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040905FF *pFA = (EM_DI040905FF*) pSrc;

	nPos += CFP_DOUBLE_22(&lpBuf[nPos], (void *) &pFA->DI04090501);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->DI04090502);

	return nPos;
}

static int CFA_DI040905FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040905FF *pFA = (EM_DI040905FF*) pBody;

	nPos += CFA_DOUBLE_22((void *) &pFA->DI04090501, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->DI04090502, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040906FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040906FF *pFA = (EM_DI040906FF*) pSrc;

	nPos += CFP_DOUBLE_22(&lpBuf[nPos], (void *) &pFA->DI04090601);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->DI04090602);

	return nPos;
}

static int CFA_DI040906FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040906FF *pFA = (EM_DI040906FF*) pBody;

	nPos += CFA_DOUBLE_22((void *) &pFA->DI04090601, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->DI04090602, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040907FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040907FF *pFA = (EM_DI040907FF*) pSrc;

	nPos += CFP_WORD(&lpBuf[nPos], (void *) &pFA->DI04090701);
	nPos += CFP_DOUBLE_24(&lpBuf[nPos], (void *) &pFA->DI04090702);
	nPos += CFP_DOUBLE_24(&lpBuf[nPos], (void *) &pFA->DI04090703);
	nPos += CFP_BYTE(&lpBuf[nPos], (void *) &pFA->DI04090704);

	return nPos;
}

static int CFA_DI040907FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040907FF *pFA = (EM_DI040907FF*) pBody;

	nPos += CFA_WORD((void *) &pFA->DI04090701, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_24((void *) &pFA->DI04090702, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_24((void *) &pFA->DI04090703, &lpBuf[nPos]);
	nPos += CFA_BYTE((void *) &pFA->DI04090704, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040908FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040908FF *pFA = (EM_DI040908FF*) pSrc;

	nPos += CFP_DOUBLE_31(&lpBuf[nPos], &pFA->DI04090801);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04090802);
	return nPos;
}

static int CFA_DI040908FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040908FF *pFA = (EM_DI040908FF*) pBody;

	nPos += CFA_DOUBLE_31(&pFA->DI04090801, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04090802, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI040909FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI040909FF *pFA = (EM_DI040909FF*) pSrc;

	nPos += CFP_DOUBLE_31(&lpBuf[nPos], &pFA->DI04090901);
	nPos += CFP_DOUBLE_24(&lpBuf[nPos], &pFA->DI04090902);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04090903);
	return nPos;
}

static int CFA_DI040909FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI040909FF *pFA = (EM_DI040909FF*) pBody;

	nPos += CFA_WORD(&pFA->DI04090901, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_24(&pFA->DI04090902, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04090903, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04090AFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04090AFF *pFA = (EM_DI04090AFF*) pSrc;

	nPos += CFP_DOUBLE_24(&lpBuf[nPos], &pFA->DI04090A01);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04090A02);
	return nPos;
}

static int CFA_DI04090AFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04090AFF *pFA = (EM_DI04090AFF*) pBody;

	nPos += CFA_DOUBLE_24(&pFA->DI04090A01, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04090A02, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04090BFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04090BFF *pFA = (EM_DI04090BFF*) pSrc;

	nPos += CFP_DOUBLE_24(&lpBuf[nPos], &pFA->DI04090B01);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04090B02);
	return nPos;
}

static int CFA_DI04090BFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04090BFF *pFA = (EM_DI04090BFF*) pBody;

	nPos += CFA_DOUBLE_24(&pFA->DI04090B01, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04090B02, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04090CFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04090CFF *pFA = (EM_DI04090CFF*) pSrc;

	nPos += CFP_WORD(&lpBuf[nPos], &pFA->DI04090C01);
	nPos += CFP_WORD(&lpBuf[nPos], &pFA->DI04090C02);
	return nPos;
}

static int CFA_DI04090CFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04090CFF *pFA = (EM_DI04090CFF*) pBody;

	nPos += CFA_WORD(&pFA->DI04090C01, &lpBuf[nPos]);
	nPos += CFA_WORD(&pFA->DI04090C02, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04090DFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04090DFF *pFA = (EM_DI04090DFF*) pSrc;

	nPos += CFP_DOUBLE_24(&lpBuf[nPos], &pFA->DI04090D01);
	nPos += CFP_DOUBLE_24(&lpBuf[nPos], &pFA->DI04090D02);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04090D03);
	return nPos;
}

static int CFA_DI04090DFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04090DFF *pFA = (EM_DI04090DFF*) pBody;

	nPos += CFA_DOUBLE_24(&pFA->DI04090D01, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_24(&pFA->DI04090D02, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04090D03, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI04090EFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04090EFF *pFA = (EM_DI04090EFF*) pSrc;

	nPos += CFP_DOUBLE_13(&lpBuf[nPos], &pFA->DI04090E01);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04090E02);
	return nPos;
}

static int CFA_DI04090EFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04090EFF *pFA = (EM_DI04090EFF*) pBody;

	nPos += CFA_DOUBLE_13(&pFA->DI04090E01, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04090E02, &lpBuf[nPos]);

	return 2;
}

static int CFP_DI04090FFF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04090FFF *pFA = (EM_DI04090FFF*) pSrc;

	nPos += CFP_DOUBLE_22(&lpBuf[nPos], &pFA->DI04090F01);
	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DI04090F02);
	return nPos;
}

static int CFA_DI04090FFF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04090FFF *pFA = (EM_DI04090FFF*) pBody;

	nPos += CFA_DOUBLE_22(&pFA->DI04090F01, &lpBuf[nPos]);
	nPos += CFA_BYTE(&pFA->DI04090F02, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI0480000X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI0480000X *pFA = (EM_DI0480000X*) pSrc;

	memcpy(pFA->version, (BYTE*) &lpBuf[nPos], 32);
	nPos += 32;

	return nPos;
}

static int CFA_DI0480000X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI0480000X *pFA = (EM_DI0480000X*) pBody;

	memcpy((BYTE*) &lpBuf[nPos], pFA->version, 32);
	nPos += 32;

	return nPos;
}

static int CFP_DI048000FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI048000FF *pFA = (EM_DI048000FF*) pSrc;

	nPos += CFP_DI0480000X(&lpBuf[nPos], (void *) &pFA->DI04800001);
	nPos += CFP_DI0480000X(&lpBuf[nPos], (void *) &pFA->DI04800002);
	nPos += CFP_DI0480000X(&lpBuf[nPos], (void *) &pFA->DI04800003);

	return nPos;
}

static int CFA_DI048000FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI048000FF *pFA = (EM_DI048000FF*) pBody;

	nPos += CFA_DI0480000X((void *) &pFA->DI04800001, &lpBuf[nPos]);
	nPos += CFA_DI0480000X((void *) &pFA->DI04800002, &lpBuf[nPos]);
	nPos += CFA_DI0480000X((void *) &pFA->DI04800003, &lpBuf[nPos]);

	return nPos;
}

// 冻结时间
static int CFP_DI04820000(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04820000 *pFA = (EM_DI04820000*) pSrc;

	hex2bcd(pFA->minute, &lpBuf[nPos++]);
	hex2bcd(pFA->hour, &lpBuf[nPos++]);
	hex2bcd(pFA->day, &lpBuf[nPos++]);
	hex2bcd(pFA->month, &lpBuf[nPos++]);

	return nPos;
}

static int CFA_DI04820000(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	EM_DI04820000 *pFA = (EM_DI04820000*) pBody;
	//
	pFA->minute = bcd2hex(lpBuf[nPos++]);
	pFA->hour = bcd2hex(lpBuf[nPos++]);
	pFA->day = bcd2hex(lpBuf[nPos++]);
	pFA->month = bcd2hex(lpBuf[nPos++]);
	//
	return nPos;
}

//全失压次数和累积时间
static int CFP_DI03050000(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI03050000 *pFA = (MT_DI03050000*) pSrc;

	long2bcd(pFA->timesAll, &lpBuf[0], &lpBuf[1], &lpBuf[2], &lpBuf[3]);
	nPos += 3;

	long2bcd(pFA->minsAll, &lpBuf[3], &lpBuf[4], &lpBuf[5], &lpBuf[6]);
	nPos += 3;

	return nPos;
}

static int CFA_DI03050000(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI03050000 *pFA = (MT_DI03050000*) pBody;

	bcd2long(lpBuf[0], lpBuf[1], lpBuf[2], 0, &pFA->timesAll);
	bcd2long(lpBuf[3], lpBuf[4], lpBuf[5], 0, &pFA->minsAll);

	nPos += 6;
	return nPos;
}

//全失压
static int CFP_DI03050001(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0305000X *pFA = (MT_DI0305000X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->startTime);
	nPos += CFP_CURRENT(&lpBuf[nPos], &pFA->nCurrent);
	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->endTime);
	return nPos;
}

static int CFA_DI03050001(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0305000X *pFA = (MT_DI0305000X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->startTime, &lpBuf[nPos]);
	nPos += CFA_CURRENT(&pFA->nCurrent, &lpBuf[nPos]);
	nPos += CFA_DI_TIMEYS(&pFA->endTime, &lpBuf[nPos]);

	return nPos;
}

//辅助电源失电
static int CFP_DI03060001(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0306000X *pFA = (MT_DI0306000X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->startTime);
	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->endTime);
	return nPos;
}

static int CFA_DI03060001(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0306000X *pFA = (MT_DI0306000X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->startTime, &lpBuf[nPos]);
	nPos += CFA_DI_TIMEYS(&pFA->endTime, &lpBuf[nPos]);

	return nPos;
}

//停电记录
static int CFP_DI03110001(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0311000X *pFA = (MT_DI0311000X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->startTime);
	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->endTime);
	return nPos;
}

static int CFA_DI03110001(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0311000X *pFA = (MT_DI0311000X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->startTime, &lpBuf[nPos]);
	nPos += CFA_DI_TIMEYS(&pFA->endTime, &lpBuf[nPos]);

	return nPos;
}

//编程记录
static int CFP_DI03300001(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330000X *pFA = (MT_DI0330000X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	memcpy(&lpBuf[nPos], &pFA->programDI, 40);
	nPos += 40;

	return nPos;
}

static int CFA_DI03300001(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330000X *pFA = (MT_DI0330000X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	memcpy(&pFA->programDI, &lpBuf[nPos], 40);
	nPos += 40;
	return nPos;
}

//电表清0记录
static int CFP_DI03300101(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330010X *pFA = (MT_DI0330010X*) pSrc;
	BYTE *pData = (BYTE *) &(pFA->nPosPEnergy);
	BYTE size = sizeof(pFA->nPosPEnergy);
	BYTE len;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	for (BYTE i = 0; i < 24; i++) {
		len = CFP_ENERGY(&lpBuf[nPos], pData);
		pData += size;
		nPos += len;
	}

	return nPos;
}

static int CFA_DI03300101(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330010X *pFA = (MT_DI0330010X*) pBody;
	BYTE *pData = (BYTE *) &(pFA->nPosPEnergy);
	BYTE size = sizeof(pFA->nPosPEnergy);
	BYTE len;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	for (BYTE i = 0; i < 24; i++) {
		len = CFA_ENERGY(pData, &lpBuf[nPos]);
		pData += size;
		nPos += len;
	}

	return nPos;
}

//需量清0记录
static int CFP_DI03300201(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330020X *pFA = (MT_DI0330020X*) pSrc;
	BYTE *pData = (BYTE *) &(pFA->nPosPDemand);
	BYTE len;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	for (BYTE i = 0; i < 24; i++) {
		len = CFP_XLVAL_TIME(&lpBuf[nPos], pData);
		pData += len;
		nPos += len;
	}

	return nPos;
}

static int CFA_DI03300201(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330020X *pFA = (MT_DI0330020X*) pBody;
	BYTE *pData = (BYTE *) &(pFA->nPosPDemand);
	BYTE len;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	for (BYTE i = 0; i < 24; i++) {
		len = CFA_XLVAL_TIME(pData, &lpBuf[nPos]);
		pData += len;
		nPos += len;
	}
	return nPos;
}

//事件清0记录
static int CFP_DI03300301(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330030X *pFA = (MT_DI0330030X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;
	memcpy(&lpBuf[nPos], &pFA->ercDI, sizeof(DWORD));
	nPos += 4;

	return nPos;
}

static int CFA_DI03300301(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330030X *pFA = (MT_DI0330030X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;
	memcpy(&pFA->ercDI, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	return nPos;
}

//校时记录
static int CFP_DI03300401(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330040X *pFA = (MT_DI0330040X*) pSrc;

	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->timeBefore);
	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->timeAfter);

	return nPos;
}

static int CFA_DI03300401(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330040X *pFA = (MT_DI0330040X*) pBody;

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	nPos += CFA_DI_TIMEYS(&pFA->timeBefore, &lpBuf[nPos]);
	nPos += CFA_DI_TIMEYS(&pFA->timeAfter, &lpBuf[nPos]);

	return nPos;
}

static int CFP_MMDDNN(BYTE *lpBuf, void *pSrc) {
	BYTE *tmpBody = (BYTE *) pSrc;
	/*for(BYTE i =0; i < 3; i++)
	 {
	 CFP_BYTE(&lpBuf[i], (void *)(&tmpBody[i]));
	 }	*/
	CFP_BYTE(&lpBuf[0], (void *) (&tmpBody[2]));
	CFP_BYTE(&lpBuf[1], (void *) (&tmpBody[1]));
	CFP_BYTE(&lpBuf[2], (void *) (&tmpBody[0]));
	return 3;
}

static int CFA_MMDDNN(void *pBody, BYTE *lpBuf) {
	BYTE *tmpBody = (BYTE *) pBody;

	/*for(BYTE i =0; i < 3; i++)
	 {
	 CFA_BYTE((void *)(&tmpBody[i]), &lpBuf[i]);
	 }	*/
	CFA_BYTE((void *) (&tmpBody[0]), &lpBuf[2]);
	CFA_BYTE((void *) (&tmpBody[1]), &lpBuf[1]);
	CFA_BYTE((void *) (&tmpBody[2]), &lpBuf[0]);
	return 3;
}

//时区表
static int CFP_DI04010000(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI04010000 *pFA = (EM_DI04010000*) pSrc;

	for (BYTE i = 0; i < LINYANG_EM_MAX_TIMEMASK; i++) {
		nPos += CFP_MMDDNN(&lpBuf[nPos], &pFA->DI_TIMEZONE[i]);
	}
	return nPos;
}

static int CFA_DI04010000(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI04010000 *pFA = (EM_DI04010000*) pBody;

	for (BYTE i = 0; i < LINYANG_EM_MAX_TIMEMASK; i++) {
		nPos += CFA_MMDDNN(&pFA->DI_TIMEZONE[i], &lpBuf[nPos]);
	}
	return nPos;
}
//时段表
static int CFP_DI04010001(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	EM_DI0401000X *pFA = (EM_DI0401000X*) pSrc;

	for (BYTE i = 0; i < LINYANG_EM_MAX_TIMEMASK; i++) {
		nPos += CFP_MMDDNN(&lpBuf[nPos], &pFA->DI_PERIOD[i]);
	}
	return nPos;
}

static int CFA_DI04010001(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	EM_DI0401000X *pFA = (EM_DI0401000X*) pBody;

	for (BYTE i = 0; i < LINYANG_EM_MAX_TIMEMASK; i++) {
		nPos += CFA_MMDDNN((void *) &pFA->DI_PERIOD[i], &lpBuf[nPos]);
	}
	return nPos;
}

//时段表编程记录
static int CFP_DI03300501(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330050X *pFA = (MT_DI0330050X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	for (BYTE i = 0; i < LINYANG_EM_MAX_TIMETABLE; i++) {
		nPos += CFP_DI04010001(&lpBuf[nPos], &pFA->DT040100FF.DI0401000X[i]);
	}

	for (BYTE i = 0; i < LINYANG_EM_MAX_TIMETABLE; i++) {
		nPos += CFP_DI04010001(&lpBuf[nPos], &pFA->DT040200FF.DI0401000X[i]);
	}
	return nPos;
}

static int CFA_DI03300501(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330050X *pFA = (MT_DI0330050X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	for (BYTE i = 0; i < LINYANG_EM_MAX_TIMETABLE; i++) {
		nPos += CFA_DI04010001(&lpBuf[nPos],
				(BYTE *) &pFA->DT040100FF.DI0401000X[i]);
	}

	for (BYTE i = 0; i < LINYANG_EM_MAX_TIMETABLE; i++) {
		nPos += CFA_DI04010001(&lpBuf[nPos],
				(BYTE *) &pFA->DT040200FF.DI0401000X[i]);
	}

	return nPos;
}

//时区表编程记录
static int CFP_DI03300601(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330060X *pFA = (MT_DI0330060X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	nPos += CFP_DI04010000(&lpBuf[nPos], &pFA->DT04010000);
	nPos += CFP_DI04010000(&lpBuf[nPos], &pFA->DT04020000);

	return nPos;
}

static int CFA_DI03300601(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330060X *pFA = (MT_DI0330060X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	nPos += CFA_DI04010000(&pFA->DT04010000, &lpBuf[nPos]);
	nPos += CFA_DI04010000(&pFA->DT04020000, &lpBuf[nPos]);

	return nPos;
}

//周休日编程记录
static int CFP_DI03300701(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330070X *pFA = (MT_DI0330070X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DT04000802);
	return nPos;
}

static int CFA_DI03300701(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330070X *pFA = (MT_DI0330070X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	nPos += CFA_BYTE(&pFA->DT04000802, &lpBuf[nPos]);

	return nPos;
}

//节假日编程记录
static int CFP_DI03300801(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330080X *pFA = (MT_DI0330080X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

//	nPos += CFP_DI040300FF(&lpBuf[nPos], &pFA->DT040300FF);
	return nPos;
}

static int CFA_DI03300801(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330080X *pFA = (MT_DI0330080X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

//	nPos += CFA_DI040300FF(&lpBuf[nPos], &pFA->DT040300FF);

	return nPos;
}

//有功组合方式编程记录
static int CFP_DI03300901(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0330090X *pFA = (MT_DI0330090X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	nPos += CFP_BYTE(&lpBuf[nPos], &pFA->DT040000601);
	return nPos;
}

static int CFA_DI03300901(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI0330090X *pFA = (MT_DI0330090X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	nPos += CFA_BYTE(&pFA->DT040000601, &lpBuf[nPos]);

	return nPos;
}

//结算日编程记录
static int CFP_DI03300C01(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI03300C0X *pFA = (MT_DI03300C0X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenTime);
	memcpy(&lpBuf[nPos], &pFA->handler, sizeof(DWORD));
	nPos += 4;

	nPos += CFP_DI04000BFF(&lpBuf[nPos], &pFA->DT04000BFF);
	return nPos;
}

static int CFA_DI03300C01(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI03300C0X *pFA = (MT_DI03300C0X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenTime, &lpBuf[nPos]);

	memcpy(&pFA->handler, &lpBuf[nPos], sizeof(DWORD));
	nPos += 4;

	nPos += CFA_DI04000BFF(&pFA->DT04000BFF, &lpBuf[nPos]);

	return nPos;
}

//结算日编程记录
static int CFP_DI03300D01(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI03300D0X *pFA = (MT_DI03300D0X*) pSrc;

	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenStartTime);
	nPos += CFP_DI_TIMEYS(&lpBuf[nPos], &pFA->happenEndTime);

	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00010000A);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00020000A);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00050000A);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00060000A);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00070000A);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00080000A);

	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00010000B);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00020000B);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00050000B);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00060000B);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00070000B);
	nPos += CFP_ENERGY(&lpBuf[nPos], &pFA->DT00080000B);
	return nPos;
}

static int CFA_DI03300D01(void *pBody, BYTE *lpBuf) {
	int nPos = 0;

	MT_DI03300D0X *pFA = (MT_DI03300D0X*) pBody;

	nPos += CFA_DI_TIMEYS(&pFA->happenStartTime, &lpBuf[nPos]);
	nPos += CFA_DI_TIMEYS(&pFA->happenEndTime, &lpBuf[nPos]);

	nPos += CFA_ENERGY(&pFA->DT00010000A, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00020000A, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00050000A, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00060000A, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00070000A, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00080000A, &lpBuf[nPos]);

	nPos += CFA_ENERGY(&pFA->DT00010000B, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00020000B, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00050000B, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00060000B, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00070000B, &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT00080000B, &lpBuf[nPos]);

	return nPos;
}

static int CFP_DI0500100X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0500100X *pFA = (MT_DI0500100X*) pSrc;

	nPos += CFP_DI0203FF00(&lpBuf[nPos], &pFA->power);
	nPos += CFP_DI0203FF00(&lpBuf[nPos], &pFA->inpower);

	return nPos;
}

static int CFA_DI0500100X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI0500100X *pFA = (MT_DI0500100X*) pBody;

	nPos += CFA_DI0203FF00(&pFA->power, &lpBuf[nPos]);
	nPos += CFA_DI0203FF00(&pFA->inpower, &lpBuf[nPos]);

	return nPos;
}

//失压记录
static int CFP_DI100000FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI100000FF *pFA = (MT_DI100000FF*) pSrc;
	//
	nPos += CFP_DWORD((BYTE*) &lpBuf[nPos], &pFA->DT10000001);
	nPos += CFP_DWORD((BYTE*) &lpBuf[nPos], &pFA->DT10000002);
	//
	return nPos;
}

static int CFA_DI100000FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI100000FF *pFA = (MT_DI100000FF*) pBody;
	//
	nPos += CFA_DWORD(&pFA->DT10000001, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DWORD(&pFA->DT10000002, (BYTE*) &lpBuf[nPos]);
	//
	return nPos;
}

static int CFP_DI1000FF01(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI1000FF01 *pFA = (MT_DI1000FF01*) pSrc;
	//
	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT10000101);
	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT10000201);
	//
	return nPos;
}

static int CFA_DI1000FF01(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI1000FF01 *pFA = (MT_DI1000FF01*) pBody;
	//
	nPos += CFA_DI_TIMEYS(&pFA->DT10000101, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_TIMEYS(&pFA->DT10000201, (BYTE*) &lpBuf[nPos]);
	//
	return nPos;
}

static int CFP_DI_LOSTV_TOTAL(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI_LOSTV_TOTAL *pFA = (MT_DI_LOSTV_TOTAL*) pSrc;
	//
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010201);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010301);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010401);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010501);
	//
	return nPos;
}

static int CFP_DI_START_LOSTV_PHASE(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI_START_LOSTV_PHASE *pFA = (MT_DI_START_LOSTV_PHASE*) pSrc;
	//
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010601);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010701);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010801);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010901);

	nPos += CFP_WORD((BYTE*) &lpBuf[nPos], &pFA->DT10010A01);
	nPos += CFP_CURRENT((BYTE*) &lpBuf[nPos], &pFA->DT10010B01);
	nPos += CFP_POWER((BYTE*) &lpBuf[nPos], &pFA->DT10010C01);
	nPos += CFP_POWER((BYTE*) &lpBuf[nPos], &pFA->DT10010D01);
	nPos += CFP_PF((BYTE*) &lpBuf[nPos], &pFA->DT10010E01);

	//
	return nPos;
}

static int CFP_DI_LOSTV_AH(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI_LOSTV_AH *pFA = (MT_DI_LOSTV_AH*) pSrc;
	//
	for (BYTE i = 0; i < 4; i++) {
		nPos += CFP_DOUBLE_62((BYTE*) &lpBuf[nPos], &pFA->Ah[i]);
	}

	//
	return nPos;
}

static int CFP_DI_END_LOSTV_PHASE(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI_END_LOSTV_PHASE *pFA = (MT_DI_END_LOSTV_PHASE*) pSrc;
	//
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010601);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010701);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010801);
	nPos += CFP_ENERGY((BYTE*) &lpBuf[nPos], &pFA->DT10010901);
	//
	return nPos;
}

static int CFA_DI_LOSTV_TOTAL(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI_LOSTV_TOTAL *pFA = (MT_DI_LOSTV_TOTAL*) pBody;
	//
	nPos += CFA_ENERGY(&pFA->DT10010201, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010301, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010401, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010501, (BYTE*) &lpBuf[nPos]);
	//
	return nPos;
}

static int CFA_DI_START_LOSTV_PHASE(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI_START_LOSTV_PHASE *pFA = (MT_DI_START_LOSTV_PHASE*) pBody;
	//
	nPos += CFA_ENERGY(&pFA->DT10010601, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010701, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010801, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010901, (BYTE*) &lpBuf[nPos]);

	nPos += CFA_WORD(&pFA->DT10010A01, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_CURRENT(&pFA->DT10010B01, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_POWER(&pFA->DT10010C01, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_POWER(&pFA->DT10010D01, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_PF(&pFA->DT10010E01, (BYTE*) &lpBuf[nPos]);

	//
	return nPos;
}

static int CFA_DI_LOSTV_AH(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI_LOSTV_AH *pFA = (MT_DI_LOSTV_AH*) pBody;
	//
	for (BYTE i = 0; i < 4; i++) {
		nPos += CFA_DOUBLE_62(&pFA->Ah[i], (BYTE*) &lpBuf[nPos]);
	}

	//
	return nPos;
}

static int CFA_DI_END_LOSTV_PHASE(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI_END_LOSTV_PHASE *pFA = (MT_DI_END_LOSTV_PHASE*) pBody;
	//
	nPos += CFA_ENERGY(&pFA->DT10010601, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010701, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010801, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_ENERGY(&pFA->DT10010901, (BYTE*) &lpBuf[nPos]);
	//
	return nPos;
}

static int CFP_DI100XFF0X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI100XFF0X *pFA = (MT_DI100XFF0X*) pSrc;
	//
	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT_START_LV_TIME);
	nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos], &pFA->DT_START_LV_TOTAL);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_DI_START_LOSTV_PHASE((BYTE*) &lpBuf[nPos],
				&pFA->DT_START_LV_PHASE[i]);
	}
	nPos += CFP_DI_LOSTV_AH((BYTE*) &lpBuf[nPos], &pFA->DT_LV_AH);
	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT_END_LV_TIME);
	nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos], &pFA->DT_END_LV_TOTAL);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_DI_END_LOSTV_PHASE((BYTE*) &lpBuf[nPos],
				&pFA->DT_START_LV_PHASE[i]);
	}

	//
	return nPos;
}

static int CFA_DI100XFF0X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI100XFF0X *pFA = (MT_DI100XFF0X*) pBody;
	//
	nPos += CFA_DI_TIMEYS(&pFA->DT_START_LV_TIME, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_START_LV_TOTAL, (BYTE*) &lpBuf[nPos]);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_DI_START_LOSTV_PHASE(&pFA->DT_START_LV_PHASE[i],
				(BYTE*) &lpBuf[nPos]);
	}
	nPos += CFA_DI_LOSTV_AH(&pFA->DT_LV_AH, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_TIMEYS(&pFA->DT_END_LV_TIME, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_END_LV_TOTAL, (BYTE*) &lpBuf[nPos]);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_DI_END_LOSTV_PHASE(&pFA->DT_START_LV_PHASE[i],
				(BYTE*) &lpBuf[nPos]);
	}
	//
	return nPos;
}

//电压逆相序
static int CFP_DI1400FF0X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI1400FF0X *pFA = (MT_DI1400FF0X*) pSrc;
	//
	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT_STR_PS_ERR_TIME);
	nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos], &pFA->DT_STR_PS_ERR_TOTAL);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos],
				&pFA->DT_STR_PS_ERR_PHASE[i]);
	}

	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT_END_PS_ERR_TIME);
	nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos], &pFA->DT_END_PS_ERR_TOTAL);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos],
				&pFA->DT_END_PS_ERR_PHASE[i]);
	}

	//
	return nPos;
}

static int CFA_DI1400FF0X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI1400FF0X *pFA = (MT_DI1400FF0X*) pBody;
	//
	nPos += CFA_DI_TIMEYS(&pFA->DT_STR_PS_ERR_TIME, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_STR_PS_ERR_TOTAL, (BYTE*) &lpBuf[nPos]);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_STR_PS_ERR_PHASE[i],
				(BYTE*) &lpBuf[nPos]);
	}
	nPos += CFA_DI_TIMEYS(&pFA->DT_END_PS_ERR_TIME, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_END_PS_ERR_TOTAL, (BYTE*) &lpBuf[nPos]);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_END_PS_ERR_PHASE[i],
				(BYTE*) &lpBuf[nPos]);
	}
	//
	return nPos;
}

//失流
static int CFP_DI180XFF0X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI180XFF0X *pFA = (MT_DI180XFF0X*) pSrc;
	//
	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT_START_LI_TIME);
	nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos], &pFA->DT_START_LI_TOTAL);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_DI_START_LOSTV_PHASE((BYTE*) &lpBuf[nPos],
				&pFA->DT_START_LI_PHASE[i]);
	}

	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT_END_LI_TIME);
	nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos], &pFA->DT_END_LI_TOTAL);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFP_DI_END_LOSTV_PHASE((BYTE*) &lpBuf[nPos],
				&pFA->DT_START_LI_PHASE[i]);
	}

	//
	return nPos;
}

static int CFA_DI180XFF0X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI180XFF0X *pFA = (MT_DI180XFF0X*) pBody;
	//
	nPos += CFA_DI_TIMEYS(&pFA->DT_START_LI_TIME, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_START_LI_TOTAL, (BYTE*) &lpBuf[nPos]);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_DI_START_LOSTV_PHASE(&pFA->DT_START_LI_PHASE[i],
				(BYTE*) &lpBuf[nPos]);
	}

	nPos += CFA_DI_TIMEYS(&pFA->DT_END_LI_TIME, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_END_LI_TOTAL, (BYTE*) &lpBuf[nPos]);
	for (BYTE i = 0; i < 3; i++) {
		nPos += CFA_DI_END_LOSTV_PHASE(&pFA->DT_START_LI_PHASE[i],
				(BYTE*) &lpBuf[nPos]);
	}
	//
	return nPos;
}

//功率因数超下限
static int CFP_DI1F00FF0X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI1F00FF0X *pFA = (MT_DI1F00FF0X*) pSrc;
	//
	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT_START_PF_DOWN_TIME);
	nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos],
			&pFA->DT_START_PF_DOWN_TOTAL);

	nPos += CFP_DI_TIMEYS((BYTE*) &lpBuf[nPos], &pFA->DT_END_PF_DOWN_TIME);
	nPos += CFP_DI_LOSTV_TOTAL((BYTE*) &lpBuf[nPos],
			&pFA->DT_END_PF_DOWN_TOTAL);

	//
	return nPos;
}

static int CFA_DI1F00FF0X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI1F00FF0X *pFA = (MT_DI1F00FF0X*) pBody;
	//
	nPos += CFA_DI_TIMEYS(&pFA->DT_START_PF_DOWN_TIME, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_START_PF_DOWN_TOTAL,
			(BYTE*) &lpBuf[nPos]);

	nPos += CFA_DI_TIMEYS(&pFA->DT_END_PF_DOWN_TIME, (BYTE*) &lpBuf[nPos]);
	nPos += CFA_DI_LOSTV_TOTAL(&pFA->DT_END_PF_DOWN_TOTAL,
			(BYTE*) &lpBuf[nPos]);

	//
	return nPos;
}

//end 记录

static int CFP_DI028000FF(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI028000FF* pFA = (MT_DI028000FF*) pSrc;

	nPos += CFP_CURRENT(&lpBuf[nPos], &pFA->DT02800001);
	nPos += CFP_DOUBLE_22(&lpBuf[nPos], &pFA->DT02800002);
	nPos += CFP_POWER(&lpBuf[nPos], &pFA->DT02800003);
	nPos += CFP_POWER(&lpBuf[nPos], &pFA->DT02800004);
	nPos += CFP_POWER(&lpBuf[nPos], &pFA->DT02800005);
	nPos += CFP_POWER(&lpBuf[nPos], &pFA->DT02800006);
	nPos += CFP_DOUBLE_31(&lpBuf[nPos], &pFA->DT02800007);
	nPos += CFP_WORD(&lpBuf[nPos], &pFA->DT02800008);
	nPos += CFP_VOLTAGE(&lpBuf[nPos], &pFA->DT02800009);
	nPos += CFP_DWORD(&lpBuf[nPos], &pFA->DT0280000A);
	nPos += CFP_DOUBLE_62(&lpBuf[nPos], &pFA->DT0280000B);

	return nPos;
}

static int CFA_DI028000FF(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI028000FF* pFA = (MT_DI028000FF*) pBody;

	nPos += CFA_CURRENT(&pFA->DT02800001, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_22(&pFA->DT02800002, &lpBuf[nPos]);
	nPos += CFA_POWER(&pFA->DT02800003, &lpBuf[nPos]);
	nPos += CFA_POWER(&pFA->DT02800004, &lpBuf[nPos]);
	nPos += CFA_POWER(&pFA->DT02800005, &lpBuf[nPos]);
	nPos += CFA_POWER(&pFA->DT02800006, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_31(&pFA->DT02800007, &lpBuf[nPos]);
	nPos += CFA_VOLTAGE(&pFA->DT02800008, &lpBuf[nPos]);
	nPos += CFA_VOLTAGE(&pFA->DT02800009, &lpBuf[nPos]);
	nPos += CFA_DWORD(&pFA->DT0280000A, &lpBuf[nPos]);
	nPos += CFA_DOUBLE_62(&pFA->DT0280000B, &lpBuf[nPos]);

	return nPos;
}

// 冻结时间
static int CFP_DI050X000X(BYTE *lpBuf, void *pSrc) {
	int nPos = 0;
	MT_DI0500000X *pFA = (MT_DI0500000X*) pSrc;

	hex2bcd(pFA->minute, &lpBuf[nPos++]);
	hex2bcd(pFA->hour, &lpBuf[nPos++]);
	hex2bcd(pFA->day, &lpBuf[nPos++]);
	hex2bcd(pFA->month, &lpBuf[nPos++]);
	hex2bcd(pFA->year, &lpBuf[nPos++]);

	return nPos;
}

static int CFA_DI050X000X(void *pBody, BYTE *lpBuf) {
	int nPos = 0;
	MT_DI0500000X *pFA = (MT_DI0500000X*) pBody;

	pFA->minute = bcd2hex(lpBuf[nPos++]);
	pFA->hour = bcd2hex(lpBuf[nPos++]);
	pFA->day = bcd2hex(lpBuf[nPos++]);
	pFA->month = bcd2hex(lpBuf[nPos++]);
	pFA->year = bcd2hex(lpBuf[nPos++]);

	return nPos;
}

/********************************************************************
 *																	*
 *                  特殊数据类型与转换函数的映射表						*
 *																	*
 * 定义与缺省转换(hex--->bcd、bcd--->hex)不同的转换。如果未定义			*
 * 特殊转换方式,则不进行转换，直接拷贝。对于连续的数据类型范围,				*
 * 如转换方式相同,通过定义起始DI、结束DI、DI步进值实现一批转换				*
 * 方式的定义。														*
 *																	*
 *********************************************************************/

static const FA_CODEC g_hFAConvertAction[] =
		{
				//电量
				{ 0x00000000, 0x00003F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0000FF00, 0x0000FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00010000, 0x00013F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0001FF00, 0x0001FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00020000, 0x00023F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0002FF00, 0x0002FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00030000, 0x00033F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0003FF00, 0x0003FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00040000, 0x00043F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0004FF00, 0x0004FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00050000, 0x00053F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0005FF00, 0x0005FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00060000, 0x00063F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0006FF00, 0x0006FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00070000, 0x00073F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0007FF00, 0x0007FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00080000, 0x00083F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0008FF00, 0x0008FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x00090000, 0x00093F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x0009FF00, 0x0009FF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x000A0000, 0x000A3F0C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x000AFF00, 0x000AFF0C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE },

				{ 0x000B0000, 0x000B000C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x00150000, 0x001E000C, 0x01,
						&CFP_ENERGY, &CFA_ENERGY, &CPrint_ENERGY }, {
						0x00290000, 0x0032000C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x003D0000, 0x0046000C, 0x01,
						&CFP_ENERGY, &CFA_ENERGY, &CPrint_ENERGY },

				{ 0x00800000, 0x0086000C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x00940000, 0x009A000C, 0x01,
						&CFP_ENERGY, &CFA_ENERGY, &CPrint_ENERGY }, {
						0x00A80000, 0x00AE000C, 0x01, &CFP_ENERGY, &CFA_ENERGY,
						&CPrint_ENERGY }, { 0x00BC0000, 0x00C2000C, 0x01,
						&CFP_ENERGY, &CFA_ENERGY, &CPrint_ENERGY },

				//需量
				{ 0x01000000, 0x01003F0C, 0x01, &CFP_XLVAL_TIME,
						&CFA_XLVAL_TIME, &CPrint_XLVAL_TIME }, { 0x0100FF00,
						0x0100FF0C, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x01010000, 0x01013F0C, 0x01,
						&CFP_XLVAL_TIME, &CFA_XLVAL_TIME, &CPrint_XLVAL_TIME },
				{ 0x0101FF00, 0x0101FF0C, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x01020000,
						0x01023F0C, 0x01, &CFP_XLVAL_TIME, &CFA_XLVAL_TIME,
						&CPrint_XLVAL_TIME }, { 0x0102FF00, 0x0102FF0C, 0x01,
						&CFP_DI010XFF0X, &CFA_DI010XFF0X, &CPrint_NONE }, {
						0x01030000, 0x01033F0C, 0x01, &CFP_XLVAL_TIME,
						&CFA_XLVAL_TIME, &CPrint_XLVAL_TIME }, { 0x0103FF00,
						0x0103FF0C, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x01040000, 0x01043F0C, 0x01,
						&CFP_XLVAL_TIME, &CFA_XLVAL_TIME, &CPrint_XLVAL_TIME },
				{ 0x0104FF00, 0x0104FF0C, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x01050000,
						0x01053F0C, 0x01, &CFP_XLVAL_TIME, &CFA_XLVAL_TIME,
						&CPrint_XLVAL_TIME }, { 0x0105FF00, 0x0105FF0C, 0x01,
						&CFP_DI010XFF0X, &CFA_DI010XFF0X, &CPrint_NONE }, {
						0x01060000, 0x01063F0C, 0x01, &CFP_XLVAL_TIME,
						&CFA_XLVAL_TIME, &CPrint_XLVAL_TIME }, { 0x0106FF00,
						0x0106FF0C, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x01070000, 0x01073F0C, 0x01,
						&CFP_XLVAL_TIME, &CFA_XLVAL_TIME, &CPrint_XLVAL_TIME },
				{ 0x0107FF00, 0x0107FF0C, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x01080000,
						0x01083F0C, 0x01, &CFP_XLVAL_TIME, &CFA_XLVAL_TIME,
						&CPrint_XLVAL_TIME }, { 0x0108FF00, 0x0108FF0C, 0x01,
						&CFP_DI010XFF0X, &CFA_DI010XFF0X, &CPrint_NONE }, {
						0x01090000, 0x01093F0C, 0x01, &CFP_XLVAL_TIME,
						&CFA_XLVAL_TIME, &CPrint_XLVAL_TIME }, { 0x0109FF00,
						0x0109FF0C, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x010A0000, 0x010A3F0C, 0x01,
						&CFP_XLVAL_TIME, &CFA_XLVAL_TIME, &CPrint_XLVAL_TIME },
				{ 0x010AFF00, 0x010AFF0C, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x01150000,
						0x011E000C, 0x01, &CFP_XLVAL_TIME, &CFA_XLVAL_TIME,
						&CPrint_XLVAL_TIME }, { 0x01290000, 0x0132000C, 0x01,
						&CFP_XLVAL_TIME, &CFA_XLVAL_TIME, &CPrint_XLVAL_TIME },
				{ 0x013D0000, 0x0146000C, 0x01, &CFP_XLVAL_TIME,
						&CFA_XLVAL_TIME, &CPrint_XLVAL_TIME }, { 0x01940000,
						0x019A000C, 0x01, &CFP_XLVAL_TIME, &CFA_XLVAL_TIME,
						&CPrint_XLVAL_TIME }, { 0x01A80000, 0x01AE000C, 0x01,
						&CFP_XLVAL_TIME, &CFA_XLVAL_TIME, &CPrint_XLVAL_TIME },
				{ 0x01BC0000, 0x01C2000C, 0x01, &CFP_XLVAL_TIME,
						&CFA_XLVAL_TIME, &CPrint_XLVAL_TIME },
				//变量
				{ 0x02010100, 0x02010300, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_WORD }, { 0x0201FF00, 0x0201FF00, 0x01,
						&CFP_DI0201FF00, &CFA_DI0201FF00, &CPrint_NONE }, {
						0x02020100, 0x02020300, 0x01, &CFP_CURRENT,
						&CFA_CURRENT, &CPrint_CURRENT }, { 0x0202FF00,
						0x0202FF00, 0x01, &CFP_DI0202FF00, &CFA_DI0202FF00,
						&CPrint_NONE }, { 0x02030000, 0x02030300, 0x01,
						&CFP_POWER, &CFA_POWER, &CPrint_POWER }, { 0x0203FF00,
						0x0203FF00, 0x01, &CFP_DI0203FF00, &CFA_DI0203FF00,
						&CPrint_NONE }, { 0x02040000, 0x02040300, 0x01,
						&CFP_POWER, &CFA_POWER, &CPrint_POWER }, { 0x0204FF00,
						0x0204FF00, 0x01, &CFP_DI0203FF00, &CFA_DI0203FF00,
						&CPrint_NONE }, { 0x02050000, 0x02050300, 0x01,
						&CFP_POWER, &CFA_POWER, &CPrint_POWER }, { 0x0205FF00,
						0x0205FF00, 0x01, &CFP_DI0203FF00, &CFA_DI0203FF00,
						&CPrint_NONE }, { 0x02060000, 0x02060300, 0x01, &CFP_PF,
						&CFA_PF, &CPrint_PF }, { 0x0206FF00, 0x0206FF00, 0x01,
						&CFP_DI0206FF00, &CFA_DI0206FF00, &CPrint_NONE }, {
						0x02070100, 0x02070300, 0x01, &CFP_PANGLE, &CFA_PANGLE,
						&CPrint_PANGLE }, { 0x0207FF00, 0x0207FF00, 0x01,
						&CFP_DI0207FF00, &CFA_DI0207FF00, &CPrint_NONE },

				//谐波含量
				{ 0x020A0101, 0x020A0115, 0x01, &CFP_HRRATE, &CFA_HRRATE,
						&CPrint_HRRATE }, { 0x020A01FF, 0x020A01FF, 0x01,
						&CFP_DI020A0XFF, &CFA_DI020A0XFF, &CPrint_NONE }, {
						0x020A0201, 0x020A0215, 0x01, &CFP_HRRATE, &CFA_HRRATE,
						&CPrint_HRRATE }, { 0x020A02FF, 0x020A02FF, 0x01,
						&CFP_DI020A0XFF, &CFA_DI020A0XFF, &CPrint_NONE }, {
						0x020A0301, 0x020A0315, 0x01, &CFP_HRRATE, &CFA_HRRATE,
						&CPrint_HRRATE }, { 0x020A03FF, 0x020A03FF, 0x01,
						&CFP_DI020A0XFF, &CFA_DI020A0XFF, &CPrint_NONE },

				{ 0x020B0101, 0x020B0115, 0x01, &CFP_HRRATE, &CFA_HRRATE,
						&CPrint_HRRATE }, { 0x020B01FF, 0x020B01FF, 0x01,
						&CFP_DI020A0XFF, &CFA_DI020A0XFF, &CPrint_NONE }, {
						0x020B0201, 0x020B0215, 0x01, &CFP_HRRATE, &CFA_HRRATE,
						&CPrint_HRRATE }, { 0x020B02FF, 0x020B02FF, 0x01,
						&CFP_DI020A0XFF, &CFA_DI020A0XFF, &CPrint_NONE }, {
						0x020B0301, 0x020B0315, 0x01, &CFP_HRRATE, &CFA_HRRATE,
						&CPrint_HRRATE }, { 0x020B03FF, 0x020B03FF, 0x01,
						&CFP_DI020A0XFF, &CFA_DI020A0XFF, &CPrint_NONE },
				//
				//0280000X
				{ 0x02800001, 0x02800001, 0x01, &CFP_CURRENT, &CFA_CURRENT,
						&CPrint_CURRENT }, { 0x02800002, 0x02800002, 0x01,
						&CFP_DOUBLE_22, &CFA_DOUBLE_22, &CPrint_DOUBLE_22 }, {
						0x02800003, 0x02800006, 0x01, &CFP_POWER, &CFA_POWER,
						&CPrint_POWER }, { 0x02800007, 0x02800007, 0x01,
						&CFP_DOUBLE_31, &CFA_DOUBLE_31, &CPrint_DOUBLE_31 }, {
						0x02800008, 0x02800009, 0x01, &CFP_VOLTAGE,
						&CFA_VOLTAGE, &CPrint_VOLTAGE },
				{ 0x0280000A, 0x0280000A, 0x01, &CFP_DWORD, &CFA_DWORD,
						&CPrint_DWORD }, { 0x0280000B, 0x0280000B, 0x01,
						&CFP_DOUBLE_62, &CFA_DOUBLE_62, &CPrint_DOUBLE_31 },

				{ 0x028000FF, 0x028000FF, 0x01, &CFP_DI028000FF,
						&CFA_DI028000FF, &CPrint_NONE },

				//全失压记录
				{ 0x03050000, 0x03050000, 0x01, &CFP_DI03050000,
						&CFA_DI03050000, &CPrint_LONG3BYTE }, { 0x03050001,
						0x0305000A, 0x01, &CFP_DI03050001, &CFA_DI03050001,
						&CPrint_NONE },

				//辅助电源失电记录
				{ 0x03060000, 0x03060000, 0x01, &CFP_DI03050000,
						&CFA_DI03050000, &CPrint_LONG3BYTE }, { 0x03060001,
						0x0306000A, 0x01, &CFP_DI03060001, &CFA_DI03060001,
						&CPrint_NONE },

				//掉电记录
				{ 0x03110000, 0x03110000, 0x01, &CFP_LONG3BYTE, &CFA_DI03050000,
						&CPrint_LONG3BYTE }, { 0x03110001, 0x0311000A, 0x01,
						&CFP_DI03110001, &CFA_DI03110001, &CPrint_NONE },

				//编程记录
				{ 0x03300000, 0x03300000, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300001, 0x0330000A, 0x01,
						&CFP_DI03300001, &CFA_DI03300001, &CPrint_NONE },

				//电表清0记录
				{ 0x03300100, 0x03300100, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300101, 0x0330010A, 0x01,
						&CFP_DI03300101, &CFA_DI03300101, &CPrint_NONE },

				//需量清0记录
				{ 0x03300200, 0x03300200, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300201, 0x0330020A, 0x01,
						&CFP_DI03300201, &CFA_DI03300201, &CPrint_NONE },

				//事件清0记录
				{ 0x03300300, 0x03300300, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300301, 0x0330030A, 0x01,
						&CFP_DI03300301, &CFA_DI03300301, &CPrint_NONE },

				//校时记录
				{ 0x03300400, 0x03300400, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300401, 0x0330040A, 0x01,
						&CFP_DI03300401, &CFA_DI03300401, &CPrint_NONE },

				//时段表编程记录
				{ 0x03300500, 0x03300500, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300501, 0x0330050A, 0x01,
						&CFP_DI03300501, &CFA_DI03300501, &CPrint_NONE },

				//时区表编程记录
				{ 0x03300600, 0x03300600, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300601, 0x0330060A, 0x01,
						&CFP_DI03300601, &CFA_DI03300601, &CPrint_NONE },

				//周休日编程记录
				{ 0x03300700, 0x03300700, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300701, 0x0330070A, 0x01,
						&CFP_DI03300701, &CFA_DI03300701, &CPrint_NONE },

				//节假日编程记录
				{ 0x03300800, 0x03300800, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300801, 0x0330080A, 0x01,
						&CFP_DI03300801, &CFA_DI03300801, &CPrint_NONE },

				//有功组合方式编程记录
				{ 0x03300900, 0x03300900, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300901, 0x0330090A, 0x01,
						&CFP_DI03300901, &CFA_DI03300901, &CPrint_NONE },

				//无功组合方式1编程记录
				{ 0x03300A00, 0x03300A00, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300A01, 0x03300A0A, 0x01,
						&CFP_DI03300901, &CFA_DI03300901, &CPrint_NONE },

				//无功组合方式2编程记录
				{ 0x03300B00, 0x03300B00, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300B01, 0x03300B0A, 0x01,
						&CFP_DI03300901, &CFA_DI03300901, &CPrint_NONE },

				//结算日编程记录
				{ 0x03300C00, 0x03300C00, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300C01, 0x03300C0A, 0x01,
						&CFP_DI03300C01, &CFA_DI03300C01, &CPrint_NONE },

				//开表盖记录
				{ 0x03300D00, 0x03300D00, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300D01, 0x03300D0A, 0x01,
						&CFP_DI03300D01, &CFA_DI03300D01, &CPrint_NONE },

				//开端钮盒记录
				{ 0x03300E00, 0x03300E00, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x03300E01, 0x03300E0A, 0x01,
						&CFP_DI03300D01, &CFA_DI03300D01, &CPrint_NONE },

//参数

				{ 0x04000101, 0x04000101, 0x01, &CFP_DI04000101,
						&CFA_DI04000101, &CPrint_DI04000101 }, { 0x04000102,
						0x04000102, 0x01, &CFP_DI04000102, &CFA_DI04000102,
						&CPrint_DI04000102 }, { 0x04000103, 0x04000103, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_BYTE }, { 0x04000104,
						0x04000104, 0x01, &CFP_BYTE, &CFA_BYTE, &CPrint_BYTE },
				{ 0x04000105, 0x04000105, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_WORD }, { 0x04000106, 0x04000109, 0x01,
						&CFP_DI_TIME, &CFA_DI_TIME, &CPrint_DI_TIME }, {
						0x040001FF, 0x040001FF, 0x01, &CFP_DI040001FF,
						&CFA_DI040001FF, &CPrint_NONE },

				{ 0x04000201, 0x04000204, 0x01, &CFP_BYTE, &CFA_BYTE,
						&CPrint_BYTE }, { 0x04000205, 0x04000205, 0x01,
						&CFP_WORD, &CFA_WORD, &CPrint_WORD }, { 0x04000206,
						0x04000207, 0x01, &CFP_BYTE, &CFA_BYTE, &CPrint_BYTE },
				{ 0x040002FF, 0x040002FF, 0x01, &CFP_DI040002FF,
						&CFA_DI040002FF, &CPrint_NONE },

				{ 0x04000301, 0x04000305, 0x01, &CFP_BYTE, &CFA_BYTE,
						&CPrint_BYTE }, { 0x04000306, 0x04000307, 0x01,
						&CFP_DWORD, &CFA_DWORD, &CPrint_DWORD }, { 0x040003FF,
						0x040003FF, 0x01, &CFP_DI040003FF, &CFA_DI040003FF,
						&CPrint_NONE },

				{ 0x04000401, 0x04000401, 0x01, &CFP_DI04000401,
						&CFA_DI04000401, &CPrint_NONE }, { 0x04000402,
						0x04000402, 0x01, &CFP_DI04000402, &CFA_DI04000402,
						&CPrint_NONE }, { 0x04000403, 0x04000403, 0x01,
						&CFP_DI04000403, &CFA_DI04000403, &CPrint_NONE }, {
						0x04000404, 0x04000404, 0x01, &CFP_DI04000404,
						&CFA_DI04000404, &CPrint_NONE }, { 0x04000405,
						0x04000405, 0x01, &CFP_DI04000405, &CFA_DI04000405,
						&CPrint_NONE }, { 0x04000406, 0x04000406, 0x01,
						&CFP_DI04000406, &CFA_DI04000406, &CPrint_NONE }, {
						0x04000407, 0x04000407, 0x01, &CFP_DI04000407,
						&CFA_DI04000407, &CPrint_NONE }, { 0x04000408,
						0x04000408, 0x01, &CFP_DI04000408, &CFA_DI04000408,
						&CPrint_NONE }, { 0x04000409, 0x04000409, 0x01,
						&CFP_DI04000409, &CFA_DI04000409, &CPrint_NONE }, {
						0x0400040A, 0x0400040A, 0x01, &CFP_DI0400040A,
						&CFA_DI0400040A, &CPrint_NONE }, { 0x0400040B,
						0x0400040B, 0x01, &CFP_DI0400040B, &CFA_DI0400040B,
						&CPrint_NONE }, { 0x0400040C, 0x0400040C, 0x01,
						&CFP_DI0400040C, &CFA_DI0400040C, &CPrint_NONE }, {
						0x0400040D, 0x0400040D, 0x01, &CFP_DI0400040D,
						&CFA_DI0400040D, &CPrint_NONE }, { 0x0400040E,
						0x0400040E, 0x01, &CFP_DI0400040E, &CFA_DI0400040E,
						&CPrint_NONE }, { 0x040004FF, 0x040004FF, 0x01,
						&CFP_DI040004FF, &CFA_DI040004FF, &CPrint_NONE },

				//电表状态字
				{ 0x04000501, 0x04000507, 0x01, nullptr, nullptr, &CPrint_WORD },
				{ 0x040005FF, 0x040005FF, 0x01, nullptr, nullptr, &CPrint_NONE },

				//有无功组合方式
				{ 0x04000601, 0x04000603, 0x01, nullptr, nullptr, &CPrint_NONE },
				{ 0x040006FF, 0x040006FF, 0x01, nullptr, nullptr, &CPrint_NONE },

				//串口通讯特征字
				{ 0x04000701, 0x04000705, 0x01, nullptr, nullptr, &CPrint_NONE },
				{ 0x040007FF, 0x040007FF, 0x01, nullptr, nullptr, &CPrint_NONE },

				//周休日
				{ 0x04000801, 0x04000801, 0x01, &CFP_DI04000801,
						&CFA_DI04000801, &CPrint_NONE }, { 0x04000802,
						0x04000802, 0x01, &CFP_BYTE, &CFA_BYTE, &CPrint_NONE },
				{ 0x040008FF, 0x040008FF, 0x01, &CFP_DI040008FF,
						&CFA_DI040008FF, &CPrint_NONE },

				//冻结模式字
				{ 0x04000901, 0x04000906, 0x01, nullptr, nullptr, &CPrint_NONE },
				{ 0x040009FF, 0x040009FF, 0x01, nullptr, nullptr, &CPrint_NONE },

				{ 0x04000A01, 0x04000A01, 0x01, &CFP_DI_TIME, &CFA_DI_TIME,
						&CPrint_NONE }, { 0x04000A02, 0x04000A07, 0x01,
						&CFP_WORD, &CFA_WORD, &CPrint_NONE }, { 0x04000AFF,
						0x04000AFF, 0x01, &CFP_DI04000AFF, &CFA_DI04000AFF,
						&CPrint_NONE },

				{ 0x04000B01, 0x04000B03, 0x01, &CFP_DI04000B0X,
						&CFA_DI04000B0X, &CPrint_NONE }, { 0x04000BFF,
						0x04000BFF, 0x01, &CFP_DI04000BFF, &CFA_DI04000BFF,
						&CPrint_NONE },

				{ 0x04000C01, 0x04000C0A, 0x01, &CFP_DI04000C0X,
						&CFA_DI04000C0X, &CPrint_NONE }, { 0x04000CFF,
						0x04000CFF, 0x01, &CFP_DI04000CFF, &CFA_DI04000CFF,
						&CPrint_NONE },

				//功率及电压限值
				{ 0x04000E01, 0x04000E02, 0x01, &CFP_DOUBLE_24, &CFA_DOUBLE_24,
						&CPrint_NONE }, { 0x04000E03, 0x04000E04, 0x01,
						&CFP_WORD, &CFA_WORD, &CPrint_NONE }, { 0x04000EFF,
						0x04000EFF, 0x01, &CFP_DI04000EFF, &CFA_DI04000EFF,
						&CPrint_NONE },

				//电量限值
				{ 0x04000F01, 0x04000F04, 0x01, &CFP_DOUBLE_62, &CFA_DOUBLE_62,
						&CPrint_NONE }, { 0x04000FFF, 0x04000FFF, 0x01,
						&CFP_DI04000FFF, &CFA_DI04000FFF, &CPrint_NONE },

				//
				{ 0x04001001, 0x04001001, 0x01, &CFP_DOUBLE_62, &CFA_DOUBLE_62,
						&CPrint_NONE }, { 0x04001002, 0x04001002, 0x01,
						&CFP_DOUBLE_62, &CFA_DOUBLE_62, &CPrint_NONE }, {
						0x04001003, 0x04001003, 0x01, &CFP_DOUBLE_62,
						&CFA_DOUBLE_62, &CPrint_NONE }, { 0x04001004,
						0x04001004, 0x01, &CFP_DOUBLE_62, &CFA_DOUBLE_62,
						&CPrint_NONE }, { 0x04001004, 0x04001005, 0x01,
						&CFP_DOUBLE_62, &CFA_DOUBLE_62, &CPrint_NONE }, {
						0x040010FF, 0x040010FF, 0x01, &CFP_DI040010FF,
						&CFA_DI040010FF, &CPrint_NONE },

				{ 0x04001101, 0x04001101, 0x01, &CFP_DI04000801,
						&CFA_DI04000801, &CPrint_NONE }, { 0x040011FF,
						0x040011FF, 0x01, &CFP_DI040011FF, &CFA_DI040011FF,
						&CPrint_NONE },

				//整点冻结参数
				{ 0x04001201, 0x04001201, 0x01, &CFP_DI_TIME, &CFA_DI_TIME,
						&CPrint_NONE }, { 0x04001202, 0x04001202, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x04001203,
						0x04001203, 0x01, &CFP_DI04001203, &CFA_DI04001203,
						&CPrint_NONE }, { 0x040012FF, 0x040012FF, 0x01,
						&CFP_DI040012FF, &CFA_DI040012FF, &CPrint_NONE },

				//无线通信参数
				{ 0x04001301, 0x04001301, 0x01, &CFP_BYTE, &CFA_BYTE,
						&CPrint_NONE }, { 0x040013FF, 0x040013FF, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE },

				//跳闸延时时间
				{ 0x04001401, 0x04001401, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_NONE }, { 0x040014FF, 0x040014FF, 0x01,
						&CFP_WORD, &CFA_WORD, &CPrint_NONE },

				{ 0x04010000, 0x04010000, 0x01, &CFP_DI04010000,
						&CFA_DI04010000, &CPrint_NONE }, { 0x04010001,
						0x04010008, 0x01, &CFP_DI04010001, &CFA_DI04010001,
						&CPrint_NONE }, { 0x04020000, 0x04020000, 0x01,
						&CFP_DI04010000, &CFA_DI04010000, &CPrint_NONE }, {
						0x04020001, 0x04020008, 0x01, &CFP_DI04010001,
						&CFA_DI04010001, &CPrint_NONE },

				//第一套梯值
				{ 0x04060001, 0x0406000E, 0x01, &CFP_DOUBLE_62, &CFA_DOUBLE_62,
						&CPrint_NONE }, { 0x040600FF, 0x040600FF, 0x01,
						&CFP_DI040600FF, &CFA_DI040600FF, &CPrint_NONE },

				//第一套电价
				{ 0x04060101, 0x0406010E, 0x01, &CFP_DOUBLE_44, &CFA_DOUBLE_44,
						&CPrint_NONE }, { 0x040601FF, 0x040601FF, 0x01,
						&CFP_DI040601FF, &CFA_DI040601FF, &CPrint_NONE },

				//第二套梯值
				{ 0x04060201, 0x0406020E, 0x01, &CFP_DOUBLE_62, &CFA_DOUBLE_62,
						&CPrint_NONE }, { 0x040602FF, 0x040602FF, 0x01,
						&CFP_DI040602FF, &CFA_DI040602FF, &CPrint_NONE },

				//第二套电价
				{ 0x04060301, 0x0406030E, 0x01, &CFP_DOUBLE_44, &CFA_DOUBLE_44,
						&CPrint_NONE }, { 0x040603FF, 0x040603FF, 0x01,
						&CFP_DI040603FF, &CFA_DI040603FF, &CPrint_NONE },

				//
				{ 0x04090101, 0x04090101, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_NONE }, { 0x04090102, 0x04090102, 0x01,
						&CFP_WORD, &CFA_WORD, &CPrint_NONE }, { 0x04090103,
						0x04090103, 0x01, &CFP_DOUBLE_24, &CFA_DOUBLE_24,
						&CPrint_NONE }, { 0x04090104, 0x04090104, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x040901FF,
						0x040901FF, 0x01, &CFP_DI040901FF, &CFA_DI040901FF,
						&CPrint_NONE },

				{ 0x04090201, 0x04090201, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_NONE }, { 0x04090202, 0x04090202, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x040902FF,
						0x040902FF, 0x01, &CFP_DI040902FF, &CFA_DI040902FF,
						&CPrint_NONE },

				{ 0x04090301, 0x04090301, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_NONE }, { 0x04090302, 0x04090302, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x040903FF,
						0x040903FF, 0x01, &CFP_DI040903FF, &CFA_DI040903FF,
						&CPrint_NONE },

				{ 0x04090401, 0x04090401, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_NONE }, { 0x04090402, 0x04090402, 0x01,
						&CFP_DOUBLE_24, &CFA_DOUBLE_24, &CPrint_NONE }, {
						0x04090403, 0x04090403, 0x01, &CFP_BYTE, &CFA_BYTE,
						&CPrint_NONE }, { 0x040904FF, 0x040904FF, 0x01,
						&CFP_DI040904FF, &CFA_DI040904FF, &CPrint_NONE },

				{ 0x04090501, 0x04090501, 0x01, &CFP_DOUBLE_22, &CFA_DOUBLE_22,
						&CPrint_NONE }, { 0x04090502, 0x04090502, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x040905FF,
						0x040905FF, 0x01, &CFP_DI040905FF, &CFA_DI040905FF,
						&CPrint_NONE },

				{ 0x04090601, 0x04090601, 0x01, &CFP_DOUBLE_22, &CFA_DOUBLE_22,
						&CPrint_NONE }, { 0x04090602, 0x04090602, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x040906FF,
						0x040906FF, 0x01, &CFP_DI040906FF, &CFA_DI040906FF,
						&CPrint_NONE },

				{ 0x04090701, 0x04090701, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_NONE }, { 0x04090702, 0x04090702, 0x01,
						&CFP_DOUBLE_24, &CFA_DOUBLE_24, &CPrint_NONE }, {
						0x04090703, 0x04090703, 0x01, &CFP_DOUBLE_24,
						&CFA_DOUBLE_24, &CPrint_NONE }, { 0x04090704,
						0x04090704, 0x01, &CFP_BYTE, &CFA_BYTE, &CPrint_NONE },
				{ 0x040907FF, 0x040907FF, 0x01, &CFP_DI040907FF,
						&CFA_DI040907FF, &CPrint_NONE },

				{ 0x04090801, 0x04090801, 0x01, &CFP_DOUBLE_31, &CFA_DOUBLE_31,
						&CPrint_NONE }, { 0x04090802, 0x04090802, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x040908FF,
						0x040908FF, 0x01, &CFP_DI040908FF, &CFA_DI040908FF,
						&CPrint_NONE },

				{ 0x04090901, 0x04090901, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_NONE }, { 0x04090902, 0x04090902, 0x01,
						&CFP_DOUBLE_24, &CFA_DOUBLE_24, &CPrint_NONE }, {
						0x04090903, 0x04090903, 0x01, &CFP_BYTE, &CFA_BYTE,
						&CPrint_NONE }, { 0x040909FF, 0x040909FF, 0x01,
						&CFP_DI040909FF, &CFA_DI040909FF, &CPrint_NONE },

				{ 0x04090A01, 0x04090A01, 0x01, &CFP_DOUBLE_24, &CFA_DOUBLE_24,
						&CPrint_NONE }, { 0x04090A02, 0x04090A02, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x04090AFF,
						0x04090AFF, 0x01, &CFP_DI04090AFF, &CFA_DI04090AFF,
						&CPrint_NONE },

				{ 0x04090B01, 0x04090B01, 0x01, &CFP_DOUBLE_24, &CFA_DOUBLE_24,
						&CPrint_NONE }, { 0x04090B02, 0x04090B02, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x04090BFF,
						0x04090BFF, 0x01, &CFP_DI04090BFF, &CFA_DI04090BFF,
						&CPrint_NONE },

				{ 0x04090C01, 0x04090C01, 0x01, &CFP_WORD, &CFA_WORD,
						&CPrint_NONE }, { 0x04090C02, 0x04090C02, 0x01,
						&CFP_WORD, &CFA_WORD, &CPrint_NONE }, { 0x04090CFF,
						0x04090CFF, 0x01, &CFP_DI04090CFF, &CFA_DI04090CFF,
						&CPrint_NONE },

				{ 0x04090D01, 0x04090D01, 0x01, &CFP_DOUBLE_24, &CFA_DOUBLE_24,
						&CPrint_NONE }, { 0x04090D02, 0x04090D02, 0x01,
						&CFP_DOUBLE_24, &CFA_DOUBLE_24, &CPrint_NONE }, {
						0x04090D03, 0x04090D03, 0x01, &CFP_BYTE, &CFA_BYTE,
						&CPrint_NONE }, { 0x04090DFF, 0x04090DFF, 0x01,
						&CFP_DI04090DFF, &CFA_DI04090DFF, &CPrint_NONE },

				{ 0x04090E01, 0x04090E01, 0x01, &CFP_DOUBLE_13, &CFA_DOUBLE_13,
						&CPrint_NONE }, { 0x04090E02, 0x04090E02, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x04090EFF,
						0x04090EFF, 0x01, &CFP_DI04090EFF, &CFA_DI04090EFF,
						&CPrint_NONE },

				{ 0x04090F01, 0x04090F01, 0x01, &CFP_DOUBLE_22, &CFA_DOUBLE_22,
						&CPrint_NONE }, { 0x04090F02, 0x04090F02, 0x01,
						&CFP_BYTE, &CFA_BYTE, &CPrint_NONE }, { 0x04090FFF,
						0x04090FFF, 0x01, &CFP_DI04090FFF, &CFA_DI04090FFF,
						&CPrint_NONE },

				//版本号
				{ 0x04800001, 0x04800003, 0x01, &CFP_DI0480000X,
						&CFA_DI0480000X, &CPrint_NONE }, { 0x048000FF,
						0x048000FF, 0x01, &CFP_DI048000FF, &CFA_DI048000FF,
						&CPrint_NONE },

				{ 0x04820000, 0x04820000, 0x01, &CFP_DI04820000,
						&CFA_DI04820000, &CPrint_NONE },

				//冻结数据
				{ 0x05000001, 0x0500000C, 0x01, &CFP_DI050X000X,
						&CFA_DI050X000X, &CPrint_NONE }, { 0x05000101,
						0x0500010C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05000201, 0x0500020C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05000301, 0x0500030C, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05000401,
						0x0500040C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05000501, 0x0500050C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05000601, 0x0500060C, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05000701,
						0x0500070C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05000801, 0x0500080C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05000901, 0x0500090C, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x05000A01,
						0x05000A0C, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x05001001, 0x0500100C, 0x01,
						&CFP_DI0500100X, &CFA_DI0500100X, &CPrint_NONE },

				{ 0x05010001, 0x0501000C, 0x01, &CFP_DI050X000X,
						&CFA_DI050X000X, &CPrint_NONE }, { 0x05010101,
						0x0501010C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05010201, 0x0501020C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05010301, 0x0501030C, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05010401,
						0x0501040C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05010501, 0x0501050C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05010601, 0x0501060C, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05010701,
						0x0501070C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05010801, 0x0501080C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05010901, 0x0501090C, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x05010A01,
						0x05010A0C, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x05011001, 0x0501100C, 0x01,
						&CFP_DI0500100X, &CFA_DI0500100X, &CPrint_NONE },

				{ 0x05020001, 0x0502000C, 0x01, &CFP_DI050X000X,
						&CFA_DI050X000X, &CPrint_NONE }, { 0x05020101,
						0x0502010C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05020201, 0x0502020C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05020301, 0x0502030C, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05020401,
						0x0502040C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05020501, 0x0502050C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05020601, 0x0502060C, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05020701,
						0x0502070C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05020801, 0x0502080C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05020901, 0x0502090C, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x05020A01,
						0x05020A0C, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x05021001, 0x0502100C, 0x01,
						&CFP_DI0500100X, &CFA_DI0500100X, &CPrint_NONE },

				{ 0x05030001, 0x0503000C, 0x01, &CFP_DI050X000X,
						&CFA_DI050X000X, &CPrint_NONE }, { 0x05030101,
						0x0503010C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05030201, 0x0503020C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05030301, 0x0503030C, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05030401,
						0x0503040C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05030501, 0x0503050C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05030601, 0x0503060C, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05030701,
						0x0503070C, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05030801, 0x0503080C, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05030901, 0x0503090C, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x05030A01,
						0x05030A0C, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x05031001, 0x0503100C, 0x01,
						&CFP_DI0500100X, &CFA_DI0500100X, &CPrint_NONE },

				{ 0x05060001, 0x0506001F, 0x01, &CFP_DI050X000X,
						&CFA_DI050X000X, &CPrint_NONE }, { 0x05060101,
						0x0506011F, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05060201, 0x0506021F, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05060301, 0x0506031F, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05060401,
						0x0506041F, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05060501, 0x0506051F, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05060601, 0x0506061F, 0x01, &CFP_DI000XFF0X,
						&CFA_DI000XFF0X, &CPrint_NONE }, { 0x05060701,
						0x0506071F, 0x01, &CFP_DI000XFF0X, &CFA_DI000XFF0X,
						&CPrint_NONE }, { 0x05060801, 0x0506081F, 0x01,
						&CFP_DI000XFF0X, &CFA_DI000XFF0X, &CPrint_NONE }, {
						0x05060901, 0x0506091F, 0x01, &CFP_DI010XFF0X,
						&CFA_DI010XFF0X, &CPrint_NONE }, { 0x05060A01,
						0x05060A1F, 0x01, &CFP_DI010XFF0X, &CFA_DI010XFF0X,
						&CPrint_NONE }, { 0x05061001, 0x0506101F, 0x01,
						&CFP_DI0500100X, &CFA_DI0500100X, &CPrint_NONE },
				//事件

				//失压
				{ 0x10000001, 0x10000002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x100000FF, 0x100000FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE },

				{ 0x10000101, 0x10000201, 0x01, &CFP_DI_TIMEYS, &CFA_DI_TIMEYS,
						&CPrint_NONE }, { 0x1000FF01, 0x1000FF01, 0x01,
						&CFP_DI1000FF01, &CFA_DI1000FF01, &CPrint_NONE },

				{ 0x10010001, 0x10010002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x100100FF, 0x100100FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1001FF01, 0x1001FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				{ 0x10020001, 0x10020002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x100200FF, 0x100200FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1002FF01, 0x1002FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				{ 0x10030001, 0x10030002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x100300FF, 0x100300FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1003FF01, 0x1003FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				//欠压
				{ 0x11010001, 0x11010002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x110100FF, 0x110100FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1101FF01, 0x1101FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				{ 0x11020001, 0x11020002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x110200FF, 0x110200FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1102FF01, 0x1102FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				{ 0x11030001, 0x11030002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x110300FF, 0x110300FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1103FF01, 0x1103FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				//过压
				{ 0x12010001, 0x12010002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x120100FF, 0x120100FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1201FF01, 0x1201FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				{ 0x12020001, 0x12020002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x120200FF, 0x120200FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1202FF01, 0x1202FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				{ 0x12030001, 0x12030002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x120300FF, 0x120300FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1203FF01, 0x1203FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				//断相
				{ 0x13010001, 0x13010002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x130100FF, 0x130100FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1301FF01, 0x1301FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				{ 0x13020001, 0x13020002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x130200FF, 0x130200FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1302FF01, 0x1302FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				{ 0x13030001, 0x13030002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x130300FF, 0x130300FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1303FF01, 0x1303FF0A, 0x01, &CFP_DI100XFF0X,
						&CFA_DI100XFF0X, &CPrint_NONE },

				//电压逆相序
				{ 0x14000001, 0x14000002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x140000FF, 0x140000FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1400FF01, 0x1400FF0A, 0x01, &CFP_DI1400FF0X,
						&CFA_DI1400FF0X, &CPrint_NONE },

				//电流逆相序
				{ 0x15000001, 0x15000002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x150000FF, 0x150000FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1500FF01, 0x1500FF0A, 0x01, &CFP_DI1400FF0X,
						&CFA_DI1400FF0X, &CPrint_NONE },

				//失流
				{ 0x18010001, 0x18010002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x180100FF, 0x180100FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1801FF01, 0x1801FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				{ 0x18020001, 0x18020002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x180200FF, 0x180200FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1802FF01, 0x1802FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				{ 0x18030001, 0x18030002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x180300FF, 0x180300FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1803FF01, 0x1803FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				//过流
				{ 0x19010001, 0x19010002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x190100FF, 0x190100FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1901FF01, 0x1901FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				{ 0x19020001, 0x19020002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x190200FF, 0x190200FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1902FF01, 0x1902FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				{ 0x19030001, 0x19030002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x190300FF, 0x190300FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1903FF01, 0x1903FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				//断流
				{ 0x1A010001, 0x1A010002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x1A0100FF, 0x1A0100FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1A01FF01, 0x1A01FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				{ 0x1A020001, 0x1A020002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x1A0200FF, 0x1A0200FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1A02FF01, 0x1A02FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				{ 0x1A030001, 0x1A030002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x1A0300FF, 0x1A0300FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1A03FF01, 0x1A03FF0A, 0x01, &CFP_DI180XFF0X,
						&CFA_DI180XFF0X, &CPrint_NONE },

				//功率因数超下限
				{ 0x1F000001, 0x1F000002, 0x01, &CFP_LONG3BYTE, &CFA_LONG3BYTE,
						&CPrint_LONG3BYTE }, { 0x1F0000FF, 0x1F0000FF, 0x01,
						&CFP_DI100000FF, &CFA_DI100000FF, &CPrint_NONE }, {
						0x1F00FF01, 0x1F00FF01, 0x01, &CFP_DI1F00FF0X,
						&CFA_DI1F00FF0X, &CPrint_NONE },

		/*
		 //DLT645-97
		 //当前电量
		 { 0x9010,	0x9014,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x901F,	0x901F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9020,	0x9024,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x902F,	0x902F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9110,	0x9114,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x911F,	0x911F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9120,	0x9124,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x912F,	0x912F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9130,	0x9134,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x913F,	0x913F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9140,	0x9144,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x914F,	0x914F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9150,	0x9154,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x915F,	0x915F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9160,	0x9164,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x916F,	0x916F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},

		 //上月电量
		 { 0x9410,	0x9414,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x941F,	0x941F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9420,	0x9424,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x942F,	0x942F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9510,	0x9514,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x951F,	0x951F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9520,	0x9524,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x952F,	0x952F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9530,	0x9534,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x953F,	0x953F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9540,	0x9544,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x954F,	0x954F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9550,	0x9554,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x955F,	0x955F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9560,	0x9564,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x956F,	0x956F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},

		 { 0x9470,	0x9472,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x947F,	0x947F,		0x01,	&CFP_DI91XF,		&CFA_DI91XF,	&CPrint_NONE		},
		 { 0x9480,	0x9482,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x948F,	0x948F,		0x01,	&CFP_DI91XF,		&CFA_DI91XF,	&CPrint_NONE		},

		 //上上月电量
		 { 0x9810,	0x9814,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x981F,	0x981F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9820,	0x9824,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x982F,	0x982F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9910,	0x9914,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x991F,	0x991F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9920,	0x9924,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x992F,	0x992F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9930,	0x9934,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x993F,	0x993F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9940,	0x9944,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x994F,	0x994F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9950,	0x9954,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x995F,	0x995F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},
		 { 0x9960,	0x9964,		0x01,	&CFP_DOUBLE_62,		&CFA_DOUBLE_62,	&CPrint_DOUBLE_62	},
		 { 0x996F,	0x996F,		0x01,	&CFP_DI9XXF,		&CFA_DI9XXF,	&CPrint_NONE		},

		 //当前需量
		 { 0xA010,	0xA014,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA01F,	0xA01F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA020,	0xA024,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA02F,	0xA02F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA110,	0xA114,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA11F,	0xA11F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA120,	0xA124,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA12F,	0xA12F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA130,	0xA134,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA13F,	0xA13F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA140,	0xA144,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA14F,	0xA14F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA150,	0xA154,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA15F,	0xA15F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA160,	0xA164,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA16F,	0xA16F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},

		 //上月需量
		 { 0xA410,	0xA414,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA41F,	0xA41F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA420,	0xA424,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA42F,	0xA42F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA510,	0xA514,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA51F,	0xA51F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA520,	0xA524,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA52F,	0xA52F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA530,	0xA534,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA53F,	0xA53F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA540,	0xA544,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA54F,	0xA54F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA550,	0xA554,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA55F,	0xA55F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA560,	0xA564,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA56F,	0xA56F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},

		 //上上月需量
		 { 0xA810,	0xA814,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA81F,	0xA81F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA820,	0xA824,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA82F,	0xA82F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA910,	0xA914,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA91F,	0xA91F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA920,	0xA924,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA92F,	0xA92F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA930,	0xA934,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA93F,	0xA93F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA940,	0xA944,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA94F,	0xA94F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA950,	0xA954,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA95F,	0xA95F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},
		 { 0xA960,	0xA964,		0x01,	&CFP_DOUBLE_24,		&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xA96F,	0xA96F,		0x01,	&CFP_DIAXXF,		&CFA_DIAXXF,	&CPrint_NONE		},

		 //当前需量发生时间
		 { 0xB010,	0xB014,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB01F,	0xB01F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB020,	0xB024,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB02F,	0xB02F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB110,	0xB114,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB11F,	0xB11F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB120,	0xB124,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB12F,	0xB12F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB130,	0xB134,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB13F,	0xB13F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB140,	0xB144,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB14F,	0xB14F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB150,	0xB154,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB15F,	0xB15F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB160,	0xB164,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB16F,	0xB16F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},

		 //上月需量发生时间
		 { 0xB410,	0xB414,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB41F,	0xB41F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB420,	0xB424,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB42F,	0xB42F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB510,	0xB514,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB51F,	0xB51F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB520,	0xB524,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB52F,	0xB52F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB530,	0xB534,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB53F,	0xB53F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB540,	0xB544,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB54F,	0xB54F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB550,	0xB554,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB55F,	0xB55F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB560,	0xB564,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB56F,	0xB56F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},

		 //上上月需量发生时间
		 { 0xB810,	0xB814,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB81F,	0xB81F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB820,	0xB824,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB82F,	0xB82F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB910,	0xB914,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB91F,	0xB91F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB920,	0xB924,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB92F,	0xB92F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB930,	0xB934,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB93F,	0xB93F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB940,	0xB944,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB94F,	0xB94F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB950,	0xB954,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB95F,	0xB95F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},
		 { 0xB960,	0xB964,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB96F,	0xB96F,		0x01,	&CFP_DIBXXF,		&CFA_DIBXXF,	&CPrint_NONE		},

		 //变量
		 { 0xB210, 0xB211,		0x01,	&CFP_DIB0XX,		&CFA_DIB0XX,	&CPrint_DIB0XX		},
		 { 0xB212, 0xB213,		0x01,	&CFP_WORD,			&CFA_WORD,		&CPrint_WORD		},
		 { 0xB214, 0xB214,		0x01,	&CFP_DIB32X,		&CFA_DIB32X,	&CPrint_DIB32X		},
		 { 0xB21F, 0xB21F,		0x01,	&CFP_DIB21F,		&CFA_DIB21F,	&CPrint_NONE		},
		 { 0xB310, 0xB313,		0x01,	&CFP_WORD,			&CFA_WORD,		&CPrint_WORD		},
		 { 0xB31F, 0xB31F,		0x01,	&CFP_DIB31F,		&CFA_DIB31F,	&CPrint_NONE		},
		 { 0xB320, 0xB323,		0x01,	&CFP_DIB32X,		&CFA_DIB32X,	&CPrint_DIB32X		},
		 { 0xB32F, 0xB32F,		0x01,	&CFP_DIB32F,		&CFA_DIB32F,	&CPrint_NONE		},
		 { 0xB330, 0xB333,		0x01,	&CFP_DIB33X,		&CFA_DIB33X,	&CPrint_DIB32X		},
		 { 0xB33F, 0xB33F,   	0x01, 	&CFP_DIB33F,    	&CFA_DIB33F,  	&CPrint_NONE     	},
		 { 0xB340, 0xB343,		0x01,	&CFP_DIB33X,		&CFA_DIB33X,	&CPrint_DIB32X		},
		 { 0xB34F, 0xB34F,		0x01,	&CFP_DIB34F,		&CFA_DIB34F,	&CPrint_NONE		},

		 { 0xB611,	0xB613,		0x01,	&CFP_DIB61X,		&CFA_DIB61X,		&CPrint_DIB61X		},
		 { 0xB61F,	0xB61F,		0x01,	&CFP_DIB61F,		&CFA_DIB61F,		&CPrint_NONE		},
		 { 0xB621,	0xB623,		0x01,	&CFP_DOUBLE_22,	&CFA_DOUBLE_22,	&CPrint_DOUBLE_22	},
		 { 0xB62F,	0xB62F,		0x01,	&CFP_DIB62F,		&CFA_DIB62F,		&CPrint_NONE		},
		 { 0xB630,	0xB633,		0x01,	&CFP_DOUBLE_24,	&CFA_DOUBLE_24,	&CPrint_DOUBLE_24	},
		 { 0xB634,	0xB635,		0x01,	&CFP_DOUBLE_22,	&CFA_DOUBLE_22,	&CPrint_DOUBLE_22	},
		 { 0xB63F,	0xB63F,		0x01,	&CFP_DIB63F,		&CFA_DIB63F,		&CPrint_NONE		},
		 { 0xB640,	0xB643,		0x01,	&CFP_DOUBLE_22,	&CFA_DOUBLE_22,	&CPrint_DOUBLE_22	},
		 { 0xB64F,	0xB64F,		0x01,	&CFP_DIB64F,		&CFA_DIB64F,		&CPrint_NONE		},
		 { 0xB650,	0xB653,		0x01,	&CFP_DOUBLE_13,	&CFA_DOUBLE_13,	&CPrint_DOUBLE_13	},
		 { 0xB65F,	0xB65F,		0x01,	&CFP_DIB65F,		&CFA_DIB65F,		&CPrint_NONE		},

		 //参数
		 { 0xC010, 	0xC010,   0x01, &CFP_DIC010,    &CFA_DIC010,  &CPrint_DIC010     },
		 { 0xC011, 	0xC011,   0x01, &CFP_DIC011,    &CFA_DIC011,  &CPrint_DIC011     },
		 { 0xC01F, 	0xC01F,   0x01, &CFP_DIC01F,    &CFA_DIC01F,  &CPrint_NONE       },
		 { 0xC020, 	0xC022,   0x01, &CFP_DIC020,    &CFA_DIC020,  &CPrint_DIC020     },
		 { 0xC02F, 	0xC02F,   0x01, &CFP_DIC02F,    &CFA_DIC02F,  &CPrint_DIC02F     },
		 { 0xC030,	0xC031,	  0x01, &CFP_DIC030,	&CFA_DIC030,	&CPrint_DIC030	   },
		 { 0xC032,	0xC034,	  0x01, &CFP_DIC032,	&CFA_DIC032,	&CPrint_DIC032	   },
		 { 0xC03F,	0xC03F,	  0x01, &CFP_DIC03F,	&CFA_DIC03F,	&CPrint_NONE	   },
		 { 0xC111,	0xC116,	  0x01,	&CFP_BYTE,		&CFA_BYTE,		&CPrint_NONE       }, //
		 { 0xC117,	0xC117,	  0x01,	&CFP_DIC117,	&CFA_DIC117,	&CPrint_DIC117     }, //结算日
		 { 0xC118,	0xC118,	  0x01,	&CFP_BYTE,		&CFA_BYTE,		&CPrint_NONE       }, //
		 { 0xC119,	0xC11A,	  0x01,	&CFP_DOUBLE_62,	&CFA_DOUBLE_62,	&CPrint_DOUBLE_62  },
		 */
		/*	{ 0xC310,   0xC314,     0x01,   &CFP_DIC31X,        &CFA_DIC31X,    &CPrint_DIC31X     },
		 { 0xC31F,   0xC31F,     0x01,   &CFP_DIC31F,        &CFA_DIC31F,    &CPrint_NONE       },
		 { 0xC321,   0xC32E,     0x01,   &CFP_DIC32X,        &CFA_DIC32X,    &CPrint_DIC32X     },
		 { 0xC32F,   0xC32F,     0x01,   &CFP_DIC32F,        &CFA_DIC32F,    &CPrint_NONE       },
		 { 0xC331,   0xC338,     0x01,   &CFP_DIC33X,        &CFA_DIC33X,    &CPrint_DIC33X     },
		 { 0xC33F,   0xC33F,     0x01,   &CFP_DIC33F,        &CFA_DIC33F,    &CPrint_NONE       },
		 { 0xC341,   0xC348,     0x01,   &CFP_DIC33X,        &CFA_DIC33X,    &CPrint_DIC33X     },
		 { 0xC34F,   0xC34F,     0x01,   &CFP_DIC33F,        &CFA_DIC33F,    &CPrint_NONE       },
		 */	//0xC41F

		};

int Search(DWORD di) {
	int nStart, nTemp, nEnd;
	FA_CODEC stHandle;

	nStart = 0;
	nEnd = sizeof(g_hFAConvertAction) / sizeof(FA_CODEC);
	;

	while (nStart <= nEnd) {
		nTemp = (nStart + nEnd) / 2;
		stHandle = g_hFAConvertAction[nTemp];

		if (stHandle.diStart > di) {
			nEnd = nTemp - 1;
		} else if (stHandle.diEnd < di) {
			nStart = nTemp + 1;
		} else {
			return nTemp;
		}
	}
	return -1;
}

//////////////////////////////////////////////////////////
// 查找转换函数
FA_ENCODE_PF FA_FindEncodePf(DWORD di) {
	FA_ENCODE_PF pf = nullptr;

	int nTemp = Search(di);

	if (nTemp != -1) {
		FA_CODEC stHandle = g_hFAConvertAction[nTemp];
		pf = stHandle.pfEncode;
	}
	return pf;
}

FA_DECODE_PF FA_FindDecodePf(DWORD di) {
	FA_DECODE_PF pf = nullptr;

	int nTemp = Search(di);

	if (nTemp != -1) {
		FA_CODEC stHandle = g_hFAConvertAction[nTemp];
		pf = stHandle.pfDecode;
	}
	return pf;
}

FA_OUTPUT_PF FA_FindOutputPf(DWORD di) {
	FA_OUTPUT_PF pf = nullptr;

	int nTemp = Search(di);

	if (nTemp != -1) {
		FA_CODEC stHandle = g_hFAConvertAction[nTemp];
		pf = stHandle.pfOutput;
	}
	return pf;
}

//////////////////////////////////////////////////////////
// 转换
int FA_Encode(EM_DATA *data, BYTE *lpBuf) {
	int nLen = 0;
	if ((lpBuf != nullptr) && (data != nullptr)) {
		FA_ENCODE_PF pf = FA_FindEncodePf(data->DI);
		if (pf == nullptr) {
			if (data->DataPtr != nullptr) {
				memcpy(lpBuf, data->DataPtr, data->nDataLength);	//数据库数据长度
				nLen = data->nDataLength;
			}
		} else {
			if ((data->DataPtr != nullptr) || (data->DI == 0xC010) || // 日期（如果为空取系统当前日期）
					(data->DI == 0xC011))		// 时间（如果为空取系统当前时间）
					{
				nLen = pf(lpBuf, data->DataPtr);
			}
		}
	}
	return nLen;
}

int FA_Decode(EM_DATA *data, void *pObj) {
	int nLen = 0;
	if ((pObj != nullptr) && (data != nullptr)) {
		if (data->DataPtr != nullptr) {
			FA_DECODE_PF pf = FA_FindDecodePf(data->DI);
			if (pf == nullptr) {
				memcpy(pObj, data->DataPtr, data->Length);		//下行规约长度
				nLen = data->Length;
			} else {
				nLen = pf(pObj, (BYTE*) data->DataPtr);
			}
		}
	}
	return nLen;
}

int FA_Output(EM_DATA *data, char *szOut) {
	FA_OUTPUT_PF pf = FA_FindOutputPf(data->DI);
	if (pf != nullptr) {
		return pf(data, szOut);
	} else {
		sprintf(szOut, "error");
	}
	return 0;
}

} /* namespace pcols */
