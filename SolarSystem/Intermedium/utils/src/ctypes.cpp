#include <float.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ctypes.h"

BOOL hex2bcd(BYTE hex, BYTE *b1) {
	if (hex > 99) {
		return false;
	}
	*b1 = ((hex / 10) << 4) | (hex % 10);
	return true;
}

BYTE bcd2hex(BYTE bcd) {
	return ((((bcd >> 4) & 0x0f) * 10) + (bcd & 0x0F));
}

BOOL word2bcd(WORD hex, BYTE *b1, BYTE *b2) {
	if (hex > 9999)
		return false;

	*b1 = *b2 = 0L;
	BYTE t1 = hex / 1000;
	hex %= 1000;
	//
	BYTE t2 = hex / 100;
	hex %= 100;
	//
	BYTE t3 = hex / 10;
	hex %= 10;
	//
	BYTE t4 = (BYTE) hex;
	//
	*b1 = t3 << 4 | t4;
	*b2 = t1 << 4 | t2;
	//
	return true;
}

BOOL word2bcd(WORD hex, BYTE bcd[]) {
	return word2bcd(hex, &bcd[0], &bcd[1]);
}

BOOL bcd2word(BYTE b1, BYTE b2, WORD *hex) {
	WORD temp = b2 << 8 | b1;
	*hex = bcd2word(temp);
	return true;
}

WORD bcd2word(BYTE bcd[]) {
	WORD hex = 0;
	bcd2word(bcd[0], bcd[1], &hex);
	return hex;
}

WORD bcd2word(WORD bcd) {
	BYTE b[4];

	for (BYTE i = 0; i < 4; i++) {
		b[i] = (unsigned char) bcd & 0x0f;
		bcd >>= 4;
	}

	return (WORD) ((DWORD) b[0] + (b[1] * 10) + (b[2] * 100) + (b[3] * 1000));
}

DWORD bcd2long(DWORD bcd) {
	WORD b1 = (WORD) (bcd >> 16);
	WORD b2 = (WORD) bcd;

	b1 = bcd2word(b1);
	b2 = bcd2word(b2);

	return ((DWORD) b1 * 10000 + b2);
}

BOOL long2bcd(DWORD hex, BYTE *b1, BYTE *b2, BYTE *b3, BYTE *b4) {
	if (hex > 99999999) {
		return false;
	}

	*b1 = *b2 = *b3 = *b4 = 0L;
	word2bcd((WORD) (hex % 10000), b1, b2);
	word2bcd((WORD) (hex / 10000), b3, b4);
	//
	return true;
}

BOOL long2bcd(DWORD hex, BYTE bcd[]) {
	return long2bcd(hex, &bcd[0], &bcd[1], &bcd[2], &bcd[3]);
}

BOOL bcd2long(BYTE b1, BYTE b2, BYTE b3, BYTE b4, DWORD *hex) {
	WORD t1 = (WORD) (b2 << 8) | b1;
	WORD t2 = (WORD) (b4 << 8) | b3;

	t1 = bcd2word(t1);
	t2 = bcd2word(t2);

	*hex = (DWORD) t2 * 10000 + t1;
	return true;
}

BOOL bcd2long(BYTE bcd[], DWORD *hex) {
	return bcd2long(bcd[0], bcd[1], bcd[2], bcd[3], hex);
}

double bcd2double(BYTE *lpBuf, int nBufSize, int nDecimalDigits) {
	double rv = 0.0;
	int max = nBufSize - 1;

	long long temp = 0;
	for (int i = max; i >= 0; --i) {
		temp = (temp * 100) + (lpBuf[i] & 0x0f) + (((lpBuf[i] & 0xf0) >> 4) * 10);
	}

	rv = (double) temp;

	switch (nDecimalDigits) {
	case 1:
		rv /= 10;
		break;

	case 2:
		rv /= 100;
		break;

	case 3:
		rv /= 1000;
		break;

	case 4:
		rv /= 10000;
		break;

	default:
		break;
	}

	return rv;
}

BOOL double2bcd(double dbValue, BYTE *lpBuf, int nIntegerDigits, int nDecimalDigits) {
	// 2007.4.13
	if ((dbValue > (double) 999999999) || (dbValue < (double) -999999999)) {
		return false;
	}

	if ((nIntegerDigits == 0 && nDecimalDigits == 0) || (nIntegerDigits + nDecimalDigits > DBL_DIG))
		return false;

	char wrkbuf[128] = { 0 };
	memset(wrkbuf, 0, 50);
	char format[DBL_DIG + 1] = { 0 };
	memset(format, 0, DBL_DIG + 1);

	int y_sub = 0, x_sub = 0, rv = 0;

	if (nDecimalDigits > 0) {
		nIntegerDigits += nDecimalDigits;
		dbValue *= pow((double) 10, (double) nDecimalDigits);
	}

	long long llVal = (long long) dbValue;
	if (llVal < 0)
		llVal = -1 * llVal;

	sprintf(format, "%%0%dlld", nIntegerDigits);
	sprintf(wrkbuf, format, llVal);

	y_sub = 0;
	x_sub = (int) strlen(wrkbuf);
	if (x_sub > nIntegerDigits)
		return false;

	if ((rv = (nIntegerDigits / 2) + (nIntegerDigits % 2 != 0)) == 0)
		rv = 1;
	memset(lpBuf, 0, rv);

	x_sub--;
	while (--nIntegerDigits >= 0) {
		lpBuf[y_sub] = wrkbuf[x_sub--] - '0';
		if (--nIntegerDigits < 0)
			break;
		lpBuf[y_sub++] |= ((wrkbuf[x_sub--] - '0') << 4);
	}

	return true;
}

int indexOf(const char *s, int n, char c) {
	int i;
	for (i = 0; i < n && *s; i++, s++) {
		if (*s == c)
			return i;
	}
	return -1;
}

int indexOf(const char* lpszString, const char* lpszSubstring) {
	char* pdest = strstr((char*) lpszString, lpszSubstring);

	if (pdest == NULL)
		return -1;
	else
		return (pdest - lpszString);
}

int indexOf(const char* lpszString, int start, const char* lpszSubstring) {
	int index = indexOf(lpszString + start, lpszSubstring);

	if (index != -1)
		index = index + start;

	return index;
}

void subString(const char* lpszString, int start, int end, char* lpszResult) {
	int i = 0;
	int j = 0;
	for (i = start, j = 0; i < end; i++, j++)
		lpszResult[j] = lpszString[i];

	lpszResult[j] = '\0';
}

DWORD StringToHex(char *str) {
	char *p;
	DWORD val = 0;
	for (p = str; *p; ++p) {
		val <<= 4;
		switch (*p) {
		case '0':
			val += 0;
			break;
		case '1':
			val += 1;
			break;
		case '2':
			val += 2;
			break;
		case '3':
			val += 3;
			break;
		case '4':
			val += 4;
			break;
		case '5':
			val += 5;
			break;
		case '6':
			val += 6;
			break;
		case '7':
			val += 7;
			break;
		case '8':
			val += 8;
			break;
		case '9':
			val += 9;
			break;
		case 'A':
		case 'a':
			val += 10;
			break;
		case 'B':
		case 'b':
			val += 11;
			break;
		case 'C':
		case 'c':
			val += 12;
			break;
		case 'D':
		case 'd':
			val += 13;
			break;
		case 'E':
		case 'e':
			val += 14;
			break;
		case 'F':
		case 'f':
			val += 15;
			break;
		}
	}
	return val;
}

///////////////////////////////////////////////////////////////////////////////
// 
void setbytebit(BYTE *value, int num, BOOL bRet) {
	BYTE temp;

	temp = 0x01;
	temp <<= num;
	if (!bRet) {
		temp = ~temp;
		*value &= temp;
	} else {
		*value |= temp;
	}
}

//
// return value 
// 0 --- the bit in num is 0
// 1 --- the bit is 1
//
BOOL getbytebit(BYTE value, int num) {
	return ((value >> num) & 0x01);
}

void setwordbit(WORD *value, int num, BOOL bRet) {
	WORD temp;

	temp = 0x01;
	temp <<= num;
	if (!bRet) {
		temp = ~temp;
		*value &= temp;
	} else {
		*value |= temp;
	}
}

//
// return value 
// 0 --- the bit in num is 0
// 1 --- the bit is 1
//
BOOL getwordbit(WORD value, int num) {
	return ((value >> num) & 0x01);
}

long str2dec(char *str, int count) {
	unsigned long data = 0;
	BOOL prefix = false;

	for (int i = 0; i < count; i++) {
		if (i == 0) {
			if (str[i] == '-')
				prefix = true;
		}

		if (str[i] < '0' || str[i] > '9') {
			if (data != 0)
				return -1;
			continue;
		}
		data *= 10;
		data += str[i] - '0';
	}

	if (prefix)
		return data * -1;
	else
		return data;
}

BOOL str2IP(DWORD &ip, const char *str, int nCount) {
	BYTE addr[4] = { 0 };
	int index = 0;
	int data = 0;

	for (int i = 0; i < nCount; i++) {
		if (str[i] < '0' || str[i] > '9') {
			addr[index] = data;
			data = 0;
			//
			index++;
			if (index >= 4)
				return false;
			continue;
		}
		data *= 10;
		data += str[i] - '0';
	}
	addr[index] = data;
	//
	ip = MAKELONG(MAKEWORD(addr[0],addr[1]), MAKEWORD(addr[2],addr[3]));
	return true;
}
