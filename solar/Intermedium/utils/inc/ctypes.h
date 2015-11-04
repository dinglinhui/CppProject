/*  hex2bcd.h

 Convert hex to bcd code and convert to bcd code to hex.

 Copyright (c) 2005 by Nanjing Linyang Power tech co.,ltd
 All Rights Reserved.
 */

#ifndef CTYPES_H__
#define CTYPES_H__

#include "OSDefs.h"

BOOL hex2bcd(BYTE hex, BYTE *b1);
BYTE bcd2hex(BYTE bcd);

BOOL word2bcd(WORD hex, BYTE *b1, BYTE *b2);
BOOL word2bcd(WORD hex, BYTE bcd[]);
WORD bcd2word(WORD bcd);
BOOL bcd2word(BYTE b1, BYTE b2, WORD *hex);
WORD bcd2word(BYTE bcd[]);

BOOL long2bcd(DWORD hex, BYTE *b1, BYTE *b2, BYTE *b3, BYTE *b4);
BOOL long2bcd(DWORD hex, BYTE bcd[]);
DWORD bcd2long(DWORD bcd);
BOOL bcd2long(BYTE b1, BYTE b2, BYTE b3, BYTE b4, DWORD *hex);
BOOL bcd2long(BYTE bcd[], DWORD *hex);

double bcd2double(BYTE *lpBuf, int nBufSize, int nDecimalDigits);
BOOL double2bcd(double dbValue, BYTE *lpBuf, int nIntegerDigits, int nDecimalDigits);

int indexOf(const char *s, int n, char c);
int indexOf(const char* lpszString, const char* lpszSubstring);
int indexOf(const char* lpszString, int start, const char* lpszSubstring);
void subString(const char* lpszString, int start, int end, char* lpszResult);

DWORD StringToHex(char *str);

void setbytebit(BYTE *value, int num, BOOL bRet);
BOOL getbytebit(BYTE value, int num);
void setwordbit(WORD *value, int num, BOOL bRet);
BOOL getwordbit(WORD value, int num);
long str2dec(char *str, int count);
BOOL str2IP(DWORD &ip, const char *str, int nCount);

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#endif //CTYPES_H__
