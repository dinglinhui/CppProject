/*
 * cil_abb.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "pcol_abb.h"

#include <cstdlib>
#include "ctypes.h"
#include <cstdio>
//#include <OSDateTime.h>

namespace pcols {

int Abb_IsValid(BYTE *lpBuf, int nSize) {
	if (nSize <= 2)
		return 1;
	int x = CalCRC(lpBuf, nSize - 2);
	if ((lpBuf[nSize - 2] == HIBYTE(x)) && (lpBuf[nSize - 1] == LOBYTE(x)))
		return 0;
	else
		return 1;
}

int Abb_ShakeHand(BYTE host[6], BYTE *lpBuf, int nSize) {
	lpBuf[0] = 0x2;
	lpBuf[1] = 0x18;
	lpBuf[2] = 0x6;
	lpBuf[3] = 0x0;
	lpBuf[4] = 0x1;
	lpBuf[5] = bcd2hex(host[0]);
	unsigned int x = CalCRC(lpBuf, 6);
	lpBuf[6] = HIBYTE(x);
	lpBuf[7] = LOBYTE(x);

	return 8;
}

int Abb_CheckPassword(char *pfarPwd, char *pkeyPwd, BYTE *lpBuf, int nSize) {
	unsigned long lPwd = GetPassword(pfarPwd, pkeyPwd);
	lpBuf[0] = 0x2;
	lpBuf[1] = 0x18;
	lpBuf[2] = 0x1;
	lpBuf[3] = 0x0;
	lpBuf[4] = 0x4;
	lpBuf[5] = HIBYTE(HIWORD(lPwd));
	lpBuf[6] = LOBYTE(HIWORD(lPwd));
	lpBuf[7] = HIBYTE(LOWORD(lPwd));
	lpBuf[8] = LOBYTE(LOWORD(lPwd));
	unsigned int x = CalCRC(lpBuf, 9);
	lpBuf[9] = HIBYTE(x);
	lpBuf[10] = LOBYTE(x);
	return 11;
}

unsigned int CalCRC(unsigned char *ptr, int count) {
	WORD crc = 0;
	int i = 0;
	crc = 0;
	while (--count >= 0) {
		crc = crc ^ (int) *ptr++ << 8;
		for (i = 0; i < 8; ++i)
			if (crc & 0x8000)
				crc = crc << 1 ^ 0x1021;
			else
				crc = crc << 1;
	}
	return crc;
}

unsigned long GetPassword(char *farPassword, char *keyPassword) {
	unsigned long pword; //password
	char * stopper; //required by strtoul
	int i;         //loop index
	int j, k = 0;     //used to simulate rotate
	union {
		unsigned long key; //encryption key
		struct {            //broken into bytes
			unsigned char byta, bytb, bytc, bytd;
		} parts;
	} val;
	//Get input values, 8 hex digits each

	//Add an arbitrary number to the key just for fun.
	val.key = strtoul(farPassword, &stopper, 0x10);
	pword = strtoul(keyPassword, &stopper, 0x10);
	val.key += 0xab41;
	// Generate a four bit checksum to be used as loop index.
	i = val.parts.byta + val.parts.bytb + val.parts.bytc + val.parts.bytd;
	i = i & 0xf;
	while (i >= 0) {
		//Set 'j' to the value of the high bit before shifting. simulates carry flag.
		if (val.parts.bytd >= 0x80)
			j = 1;
		else
			j = 0;
		//shift the key. add in the carry flag from the previous loop
		val.key = val.key << 1;
		val.key += k;
		k = j;
		//apply the key to the password.
		pword ^= val.key;
		i--;
	}
	return pword;
}

int Abb_SendNextFrame(BYTE *lpBuf, int nSize) {
	lpBuf[0] = 2;
	lpBuf[1] = 0x81;
	unsigned int x = CalCRC(lpBuf, 2);
	lpBuf[2] = HIBYTE(x);
	lpBuf[3] = LOBYTE(x);
	return 4;
}

bool Abb_HaveNextFrame(BYTE *lpBuf, int nSize) {

	if ((lpBuf[4] & 0x80) >> 7 == 1)
		return false;
	else
		return true;
}

int Abb_ReadClass(BYTE nClass, BYTE *lpBuf, int nSize) {
	lpBuf[0] = 0x2;
	lpBuf[1] = 0x5;
	lpBuf[2] = 0x0;
	lpBuf[3] = 0x0;
	lpBuf[4] = 0x0;
	lpBuf[5] = 0x0;
	lpBuf[6] = 0x0;
	lpBuf[7] = nClass;
	unsigned int x = CalCRC(lpBuf, 8);
	lpBuf[8] = HIBYTE(x);
	lpBuf[9] = LOBYTE(x);
	return 10;
}

void Abb_GetPassword(BYTE *lpBuf, char *pPwd) {
	if ((lpBuf[0] == 0x2) && (lpBuf[2] == 0x7) && (lpBuf[3] == 0x13))
		sprintf(pPwd, "%02x%02x%02x%02x", lpBuf[9], lpBuf[10], lpBuf[11], lpBuf[12]);
}

int Abb_OverDialog(BYTE *lpBuf) {
	lpBuf[0] = 2;
	lpBuf[1] = 0x80;
	unsigned int x = CalCRC(lpBuf, 2);
	lpBuf[2] = HIBYTE(x);
	lpBuf[3] = LOBYTE(x);
	return 4;
}

//nMonth 月份
//lpBuf 输出数据
//nIndex 第几帧
int Abb_ReadHistoryData(BYTE nMonth, BYTE *lpBuf, int nIndex) {
	lpBuf[0] = 0x2;
	lpBuf[1] = 0x5;
	lpBuf[2] = 0x0;
	lpBuf[3] = 0x0;
	lpBuf[4] = 42;            //读的帧长度
	int nLen = nMonth * 366 + 2 + nIndex * 42;            //偏移位置
	lpBuf[5] = HIBYTE(nLen);
	lpBuf[6] = LOBYTE(nLen);
	lpBuf[7] = 0xc;
	unsigned int x = CalCRC(lpBuf, 8);
	lpBuf[8] = HIBYTE(x);
	lpBuf[9] = LOBYTE(x);
	return 10;
}

int Abb_GetCurrentMonth() {
//	OSDateTime dt= OSDateTime::GetCurrentTime();
//	BYTE nMonth = dt.GetMonth();
//	if (nMonth==1)
//		return 12;
//	else
//		return nMonth-1;
	return 12;
}

int Abb_ReadRealData(BYTE nStartDataNum, BYTE nPhase, BYTE *lpBuf) {
	lpBuf[0] = 2;            //STX;
	lpBuf[1] = 0x1c;
	lpBuf[2] = 1;            //FUNC
	lpBuf[3] = 0;            //PAD
	lpBuf[4] = 3;            //Len
	lpBuf[5] = nStartDataNum;
	lpBuf[6] = nPhase;
	lpBuf[7] = 0;
	unsigned int x = CalCRC(lpBuf, 8);
	lpBuf[8] = HIBYTE(x);
	lpBuf[9] = LOBYTE(x);
	return 10;
}

} /* namespace pcols */
