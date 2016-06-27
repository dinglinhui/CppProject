#ifndef _CIL_FLOAT_DL645_INL_H__
#define _CIL_FLOAT_DL645_INL_H__

#include <ctypes.h>

// 9XXX
int DL645_Encode_9xxx( BYTE *buff, void *appl )
{
	EM_DI901X *ptr = (EM_DI901X*)appl;
	double2bcd(ptr->value, buff, 8, 0);
	return 4;
}

int DL645_Decode_9xxx( void *appl, BYTE *buff )
{
	EM_DI901X *ptr = (EM_DI901X*)appl;
	ptr->value = bcd2double(buff, 4, 0);
	return 4;
}

int DL645_Decode_Fxxx( void *appl, BYTE *buff ) 
{ 
	EM_DI901X *ptr = (EM_DI901X*)appl; 
	ptr->value = bcd2double(buff, 4, 0); 
	return 4; 
}
// 9XXF
int DL645_Encode_9xxF( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DI901F *ptr = (EM_DI901F*)appl;
	for( int i=0;i<=LINYANG_EM_MAX_TARIFFNUM;++i )
	{
		nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI901[i]);
	}
	return nPos;
}

int DL645_Decode_9xxF( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DI901F *ptr = (EM_DI901F*)appl;
	for( int i=0;i<=LINYANG_EM_MAX_TARIFFNUM;++i )
	{
		nPos += DL645_Decode_9xxx(&ptr->DI901[i], &buff[nPos]);
	}	
	return nPos;
}

int DL645_Decode_FxxF( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DI901F *ptr = (EM_DI901F*)appl;
	for( int i=0;i<=LINYANG_EM_MAX_TARIFFNUM;++i )
	{
		nPos += DL645_Decode_Fxxx(&ptr->DI901[i], &buff[nPos]);
	}	
	return nPos;
}

// AXXX
int DL645_Encode_Axxx( BYTE *buff, void *appl )
{
	EM_DIA01X *ptr = (EM_DIA01X*)appl;
	double2bcd(ptr->value, buff, 2, 4);
	return 4;
}

int DL645_Decode_Axxx( void *appl, BYTE *buff )
{
	EM_DIA01X *ptr = (EM_DIA01X*)appl;
	ptr->value = bcd2double(buff, 3, 4);
	return 3;
}

// AXXF
int DL645_Encode_AxxF( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIA01F *ptr = (EM_DIA01F*)appl;
	for( int i=0;i<=LINYANG_EM_MAX_TARIFFNUM;++i )
	{
		nPos += DL645_Encode_Axxx(&buff[nPos], &ptr->DIA01[i]);
	}
	return nPos;
}

int DL645_Decode_AxxF( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIA01F *ptr = (EM_DIA01F*)appl;
	for( int i=0;i<=LINYANG_EM_MAX_TARIFFNUM;++i )
	{
		nPos += DL645_Decode_Axxx(&ptr->DIA01[i], &buff[nPos]);
	}
	return nPos;
}

// BXXX
int DL645_Encode_Bxxx( BYTE *buff, void *appl )
{
	EM_DIB01X *ptr = (EM_DIB01X*)appl;
	int nPos = 0;
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	hex2bcd(ptr->day,    &buff[nPos++]);
	hex2bcd(ptr->month,  &buff[nPos++]);
	return nPos;
}

int DL645_Decode_Bxxx( void *appl, BYTE *buff )
{
	EM_DIB01X *ptr = (EM_DIB01X*)appl;
	int nPos = 0;
	ptr->minute	= bcd2hex(buff[nPos++]);
	ptr->hour 	= bcd2hex(buff[nPos++]);
	ptr->day 	= bcd2hex(buff[nPos++]);
	ptr->month 	= bcd2hex(buff[nPos++]);
	return nPos;
}

int DL645_07_Decode_AB00( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIAB0X *ptr = (EM_DIAB0X*)appl;
	
	ptr->value = bcd2double(&buff[nPos], 3, 4);
	nPos += 3;
	
	ptr->minute = bcd2hex(buff[nPos++]);
	ptr->hour   = bcd2hex(buff[nPos++]);
	ptr->day 	  = bcd2hex(buff[nPos++]);
	ptr->month  = bcd2hex(buff[nPos++]);
	ptr->year   = bcd2hex(buff[nPos++]);
	return nPos;
}

int DL645_07_Decode_ABxxx( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIAB0X *ptr = (EM_DIAB0X*)appl;
	
	for (int i = 0; i < 5; i++)
	{
		nPos += DL645_07_Decode_AB00(&ptr[i], &buff[nPos]);
	}
	return nPos;
}

// BXXX
int DL645_Encode_BxxF( BYTE *buff, void *appl )
{
	EM_DIB01F *ptr = (EM_DIB01F*)appl;
	int nPos = 0;
	for( int i=0;i<=LINYANG_EM_MAX_TARIFFNUM;++i )
	{
		nPos += DL645_Encode_Bxxx(&buff[nPos], &ptr->DIB01[i]);
	}
	return nPos;
}

int DL645_Decode_BxxF( void *appl, BYTE *buff )
{
	EM_DIB01F *ptr = (EM_DIB01F*)appl;
	int nPos = 0;
	for( int i=0;i<=LINYANG_EM_MAX_TARIFFNUM;++i )
	{
		nPos += DL645_Decode_Bxxx(&ptr->DIB01[i], &buff[nPos]);
	}
	return nPos;
}

// B210
int DL645_Encode_B210( BYTE *buff, void *appl )
{
	EM_DIB210 *ptr = (EM_DIB210*)appl;
	int nPos = 0;
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	hex2bcd(ptr->day,    &buff[nPos++]);
	hex2bcd(ptr->month,  &buff[nPos++]);
	return nPos;
}

int DL645_Decode_B210( void *appl, BYTE *buff )
{
	EM_DIB210 *ptr = (EM_DIB210*)appl;
	int nPos = 0;
	ptr->minute	= bcd2hex(buff[nPos++]);
	ptr->hour   = bcd2hex(buff[nPos++]);
	ptr->day    = bcd2hex(buff[nPos++]);
	ptr->month  = bcd2hex(buff[nPos++]);
	return nPos;
}

int DL645_07_Encode_B210( BYTE *buff, void *appl )
{
	EM_DIB210_07 *ptr = (EM_DIB210_07*)appl;
	int nPos = 0;
	hex2bcd(ptr->sec,    &buff[nPos++]);
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	hex2bcd(ptr->day,    &buff[nPos++]);
	hex2bcd(ptr->month,  &buff[nPos++]);
	hex2bcd(ptr->year,   &buff[nPos++]);
	return nPos;
}

int DL645_07_Decode_B210( void *appl, BYTE *buff )
{
	EM_DIB210_07 *ptr = (EM_DIB210_07*)appl;
	int nPos = 0;
	ptr->sec    = bcd2hex(buff[nPos++]);
	ptr->minute	= bcd2hex(buff[nPos++]);
	ptr->hour   = bcd2hex(buff[nPos++]);
	ptr->day    = bcd2hex(buff[nPos++]);
	ptr->month  = bcd2hex(buff[nPos++]);
	ptr->year   = bcd2hex(buff[nPos++]);
	return nPos;
}

int DL645_07_Encode_0401( BYTE *buff, void *appl )
{
	EM_DIB210_07 *ptr = (EM_DIB210_07*)appl;
	int nPos = 4;
	hex2bcd(ptr->sec,    &buff[nPos++]);
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	hex2bcd(ptr->day,    &buff[nPos++]);
	hex2bcd(ptr->month,  &buff[nPos++]);
	hex2bcd(ptr->year,   &buff[nPos++]);
	return nPos;
}

int DL645_07_Decode_0401( void *appl, BYTE *buff )
{
	EM_DIB210_07 *ptr = (EM_DIB210_07*)appl;
	int nPos = 4;
	ptr->sec    = bcd2hex(buff[nPos++]);
	ptr->minute	= bcd2hex(buff[nPos++]);
	ptr->hour   = bcd2hex(buff[nPos++]);
	ptr->day    = bcd2hex(buff[nPos++]);
	ptr->month  = bcd2hex(buff[nPos++]);
	ptr->year   = bcd2hex(buff[nPos++]);
	return nPos;
}

// B211
int DL645_Encode_B211( BYTE *buff, void *appl )
{
	return DL645_Encode_B210(buff, appl);
}

int DL645_Decode_B211( void *appl, BYTE *buff )
{
	return DL645_Decode_B210(appl, buff);
}

// B212
int DL645_Encode_B212( BYTE *buff, void *appl )
{
	EM_DIB212 *ptr = (EM_DIB212*)appl;
	word2bcd(ptr->times, &buff[0], &buff[1]);
	return 2;
}

int DL645_Decode_B212( void *appl, BYTE *buff )
{
	EM_DIB212 *ptr = (EM_DIB212*)appl;
	bcd2word(buff[0], buff[1], &ptr->times);
	return 2;
}

int DL645_07_Encode_B212( BYTE *buff, void *appl )
{
	EM_DIB212_07 *ptr = (EM_DIB212_07*)appl;
	long2bcd(ptr->times, &buff[0]);
	return 3;
}

int DL645_07_Decode_B212( void *appl, BYTE *buff )
{
	EM_DIB212_07 *ptr = (EM_DIB212_07*)appl;
	BYTE nVal = 0;
	bcd2long( buff[0], buff[1], buff[2], nVal, &ptr->times );
//	bcd2long(buff, &ptr->times);
	return 3;
}

// B213
int DL645_Encode_B213( BYTE *buff, void *appl )
{
	EM_DIB213 *ptr = (EM_DIB213*)appl;
	word2bcd(ptr->times, &buff[0], &buff[1]);
	return 2;
}

int DL645_Decode_B213( void *appl, BYTE *buff )
{
	EM_DIB213 *ptr = (EM_DIB213*)appl;
	bcd2word(buff[0], buff[1], &ptr->times);
	return 2;
}

// B214
int DL645_Encode_B214( BYTE *buff, void *appl )
{
	EM_DIB214 *ptr = (EM_DIB214*)appl;
	BYTE temp = 0;
	long2bcd(ptr->time, &buff[0], &buff[1], &buff[2], &temp);
	return 3;
}

int DL645_Decode_B214( void *appl, BYTE *buff )
{
	EM_DIB214 *ptr = (EM_DIB214*)appl;
	bcd2long(buff[0], buff[1], buff[2], 0, &ptr->time);
	return 3;
}

// B21F
int DL645_Encode_B21F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB21F *ptr = (EM_DIB21F*)appl;
	nPos += DL645_Encode_B210(&buff[nPos], &ptr->DIB210);
	nPos += DL645_Encode_B211(&buff[nPos], &ptr->DIB211);
	nPos += DL645_Encode_B212(&buff[nPos], &ptr->DIB212);
	nPos += DL645_Encode_B213(&buff[nPos], &ptr->DIB213);
	nPos += DL645_Encode_B214(&buff[nPos], &ptr->DIB214);
	return nPos;
}

int DL645_Decode_B21F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB21F *ptr = (EM_DIB21F*)appl;
	nPos += DL645_Decode_B210(&ptr->DIB210, &buff[nPos]);
	nPos += DL645_Decode_B211(&ptr->DIB211, &buff[nPos]);
	nPos += DL645_Decode_B212(&ptr->DIB212, &buff[nPos]);
	nPos += DL645_Decode_B213(&ptr->DIB213, &buff[nPos]);
	nPos += DL645_Decode_B214(&ptr->DIB214, &buff[nPos]);
	return nPos;
}

// B310
int DL645_Encode_B310( BYTE *buff, void *appl )
{
	EM_DIB310 *ptr = (EM_DIB310*)appl;
	word2bcd(ptr->times, &buff[0], &buff[1]);
	return 2;
}

int DL645_Decode_B310( void *appl, BYTE *buff )
{
	EM_DIB310 *ptr = (EM_DIB310*)appl;
	bcd2word(buff[0], buff[1], &ptr->times);
	return 2;
}

// B31F
int DL645_Encode_B31F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB31F *ptr = (EM_DIB31F*)appl;
	nPos += DL645_Encode_B310(&buff[nPos], &ptr->DIB310);
	nPos += DL645_Encode_B310(&buff[nPos], &ptr->DIB311);
	nPos += DL645_Encode_B310(&buff[nPos], &ptr->DIB312);
	nPos += DL645_Encode_B310(&buff[nPos], &ptr->DIB313);
	return nPos;
}

int DL645_Decode_B31F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB31F *ptr = (EM_DIB31F*)appl;
	nPos += DL645_Decode_B310(&ptr->DIB310, &buff[nPos]);
	nPos += DL645_Decode_B310(&ptr->DIB311, &buff[nPos]);
	nPos += DL645_Decode_B310(&ptr->DIB312, &buff[nPos]);
	nPos += DL645_Decode_B310(&ptr->DIB313, &buff[nPos]);
	return nPos;
}

// B320
int DL645_Encode_B320( BYTE *buff, void *appl )
{
	EM_DIB320 *ptr = (EM_DIB320*)appl;
	BYTE temp = 0;
	long2bcd(ptr->time, &buff[0], &buff[1], &buff[2], &temp);
	return 3;
}

int DL645_Decode_B320( void *appl, BYTE *buff )
{
	EM_DIB320 *ptr = (EM_DIB320*)appl;
	bcd2long(buff[0], buff[1], buff[2], 0, &ptr->time);
	return 3;
}

// B32F
int DL645_Encode_B32F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB32F *ptr = (EM_DIB32F*)appl;
	nPos += DL645_Encode_B320(&buff[nPos], &ptr->DIB320);
	nPos += DL645_Encode_B320(&buff[nPos], &ptr->DIB321);
	nPos += DL645_Encode_B320(&buff[nPos], &ptr->DIB322);
	nPos += DL645_Encode_B320(&buff[nPos], &ptr->DIB323);
	return nPos;
}

int DL645_Decode_B32F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB32F *ptr = (EM_DIB32F*)appl;
	nPos += DL645_Decode_B320(&ptr->DIB320, &buff[nPos]);
	nPos += DL645_Decode_B320(&ptr->DIB321, &buff[nPos]);
	nPos += DL645_Decode_B320(&ptr->DIB322, &buff[nPos]);
	nPos += DL645_Decode_B320(&ptr->DIB323, &buff[nPos]);
	return nPos;
}

// B330
int DL645_Encode_B330( BYTE *buff, void *appl )
{
	EM_DIB330 *ptr = (EM_DIB330*)appl;
	int nPos = 0;
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	hex2bcd(ptr->day,    &buff[nPos++]);
	hex2bcd(ptr->month,  &buff[nPos++]);
	return nPos;
}

int DL645_Decode_B330( void *appl, BYTE *buff )
{
	EM_DIB330 *ptr = (EM_DIB330*)appl;
	int nPos = 0;
	ptr->minute	= bcd2hex(buff[nPos++]);
	ptr->hour	= bcd2hex(buff[nPos++]);
	ptr->day	= bcd2hex(buff[nPos++]);
	ptr->month 	= bcd2hex(buff[nPos++]);
	return nPos;
}

// B33F
int DL645_Encode_B33F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB33F *ptr = (EM_DIB33F*)appl;
	nPos += DL645_Encode_B330(&buff[nPos], &ptr->DIB330);
	nPos += DL645_Encode_B330(&buff[nPos], &ptr->DIB331);
	nPos += DL645_Encode_B330(&buff[nPos], &ptr->DIB332);
	nPos += DL645_Encode_B330(&buff[nPos], &ptr->DIB333);
	return nPos;
}

int DL645_Decode_B33F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB33F *ptr = (EM_DIB33F*)appl;
	nPos += DL645_Decode_B330(&ptr->DIB330, &buff[nPos]);
	nPos += DL645_Decode_B330(&ptr->DIB331, &buff[nPos]);
	nPos += DL645_Decode_B330(&ptr->DIB332, &buff[nPos]);
	nPos += DL645_Decode_B330(&ptr->DIB333, &buff[nPos]);
	return nPos;
}

int DL645_07_Decode_03040000( void *appl, BYTE *buff )
{
	EM_DI03040000 *ptr = (EM_DI03040000*)appl;
	int nPos = 0;
	BYTE b = 0;
	for( int i = 0; i < 3; i++ )
	{
		bcd2long(buff[nPos], buff[nPos+1], buff[nPos+2], b, &ptr->dwCount[i]);
		nPos += 3;
		bcd2long(buff[nPos], buff[nPos+1], buff[nPos+2], b, &ptr->dwTimes[i]);
		nPos += 3;
	}
	return nPos;
}

int DL645_07_Decode_13010001( void *appl, BYTE *buff )
{
	EM_DI130X00FF *ptr = (EM_DI130X00FF*)appl;
	int nPos = 0;
	BYTE b = 0;
	
	bcd2long(buff[nPos], buff[nPos+1], buff[nPos+2], b, &ptr->dwCount);
	nPos += 3;		
	
	return nPos;
}

int DL645_07_Decode_13010002( void *appl, BYTE *buff )
{
	EM_DI130X00FF *ptr = (EM_DI130X00FF*)appl;
	int nPos = 0;
	BYTE b = 0;
	
	bcd2long(buff[nPos], buff[nPos+1], buff[nPos+2], b, &ptr->dwTimes);
	nPos += 3;		
	
	return nPos;
}

int DL645_07_Decode_03040101( void *appl, BYTE *buff )
{
	EM_DI03040101 *ptr = (EM_DI03040101*)appl;
	int nPos = 0;
	ptr->begin.second = bcd2hex(buff[nPos++]);
	ptr->begin.minute = bcd2hex(buff[nPos++]);
	ptr->begin.hour   = bcd2hex(buff[nPos++]);
	ptr->begin.day    = bcd2hex(buff[nPos++]);
	ptr->begin.month  = bcd2hex(buff[nPos++]);
	ptr->begin.year   = bcd2hex(buff[nPos++]);
	//
	ptr->end.second = bcd2hex(buff[nPos++]);
	ptr->end.minute = bcd2hex(buff[nPos++]);
	ptr->end.hour   = bcd2hex(buff[nPos++]);
	ptr->end.day    = bcd2hex(buff[nPos++]);
	ptr->end.month  = bcd2hex(buff[nPos++]);
	ptr->end.year   = bcd2hex(buff[nPos++]);
	return nPos;
}
int DL645_07_Decode_13010101( void *appl, BYTE *buff )
{
	EM_DI03040101 *ptr = (EM_DI03040101*)appl;
	int nPos = 0;
	ptr->begin.second = bcd2hex(buff[nPos++]);
	ptr->begin.minute = bcd2hex(buff[nPos++]);
	ptr->begin.hour   = bcd2hex(buff[nPos++]);
	ptr->begin.day    = bcd2hex(buff[nPos++]);
	ptr->begin.month  = bcd2hex(buff[nPos++]);
	ptr->begin.year   = bcd2hex(buff[nPos++]);
	
	return nPos;
}

int DL645_07_Decode_13012501( void *appl, BYTE *buff )
{
	EM_DI03040101 *ptr = (EM_DI03040101*)appl;
	int nPos = 0;
	
	ptr->end.second = bcd2hex(buff[nPos++]);
	ptr->end.minute = bcd2hex(buff[nPos++]);
	ptr->end.hour   = bcd2hex(buff[nPos++]);
	ptr->end.day    = bcd2hex(buff[nPos++]);
	ptr->end.month  = bcd2hex(buff[nPos++]);
	ptr->end.year   = bcd2hex(buff[nPos++]);
	return nPos;
}

// B611
int DL645_Encode_B611( BYTE *buff, void *appl )
{
	EM_DIB611 *ptr = (EM_DIB611*)appl;
	word2bcd(ptr->value, &buff[0], &buff[1]);
	return 2;
}

int DL645_Decode_B611( void *appl, BYTE *buff )
{
	EM_DIB611 *ptr = (EM_DIB611*)appl;
	bcd2word(buff[0], buff[1], &ptr->value);
	return 2;
}

// B61F
int DL645_Encode_B61F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB61F *ptr = (EM_DIB61F*)appl;
	nPos += DL645_Encode_B611(&buff[nPos], &ptr->DIB611);
	nPos += DL645_Encode_B611(&buff[nPos], &ptr->DIB612);
	nPos += DL645_Encode_B611(&buff[nPos], &ptr->DIB613);
	return nPos;
}

int DL645_Decode_B61F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB61F *ptr = (EM_DIB61F*)appl;
	nPos += DL645_Decode_B611(&ptr->DIB611, &buff[nPos]);
	nPos += DL645_Decode_B611(&ptr->DIB612, &buff[nPos]);
	nPos += DL645_Decode_B611(&ptr->DIB613, &buff[nPos]);
	return nPos;
}

int DL645_07_Decode_B61F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB61F *ptr = (EM_DIB61F*)appl;
	nPos += DL645_Decode_B611(&ptr->DIB611, &buff[nPos]);
	nPos += DL645_Decode_B611(&ptr->DIB612, &buff[nPos]);
	nPos += DL645_Decode_B611(&ptr->DIB613, &buff[nPos]);
	return nPos;
}

// B621
int DL645_Encode_B621( BYTE *buff, void *appl )
{
	EM_DIB621 *ptr = (EM_DIB621*)appl;
	double2bcd(ptr->value, buff, 2, 2);
	return 2;
}

int DL645_Decode_B621( void *appl, BYTE *buff )
{
	EM_DIB621 *ptr = (EM_DIB621*)appl;
	if((buff[1] & 0x80) != 0)//为负数
	{
		buff[1] &= 0x7f;
		ptr->value = bcd2double(buff, 2, 2 ) * (-1);		
	}
	else
	{
		ptr->value = bcd2double(buff, 2, 2 );
	}
	return 2;
}

// 07
int DL645_07_Encode_B621( BYTE *buff, void *appl )
{
	EM_DIB621 *ptr = (EM_DIB621*)appl;
	double2bcd(ptr->value, buff, 3, 3);
	return 3;
}

int DL645_07_Decode_B621( void *appl, BYTE *buff )
{
	EM_DIB621 *ptr = (EM_DIB621*)appl;
	if((buff[2] & 0x80) != 0)//为负数
	{
		buff[2] &= 0x7f;
		ptr->value = bcd2double(buff, 3, 3 ) * -1;
	}
	else
	{
		ptr->value = bcd2double(buff, 3, 3 );
	}
	return 3;
}

// B62F
int DL645_Encode_B62F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB62F *ptr = (EM_DIB62F*)appl;
	nPos += DL645_Encode_B621(&buff[nPos], &ptr->DIB621);
	nPos += DL645_Encode_B621(&buff[nPos], &ptr->DIB622);
	nPos += DL645_Encode_B621(&buff[nPos], &ptr->DIB623);
	return nPos;
}

int DL645_Decode_B62F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB62F *ptr = (EM_DIB62F*)appl;
	nPos += DL645_Decode_B621(&ptr->DIB621, &buff[nPos]);
	nPos += DL645_Decode_B621(&ptr->DIB622, &buff[nPos]);
	nPos += DL645_Decode_B621(&ptr->DIB623, &buff[nPos]);
	return nPos;
}

// 07
int DL645_07_Encode_B62F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB62F *ptr = (EM_DIB62F*)appl;
	nPos += DL645_07_Encode_B621(&buff[nPos], &ptr->DIB621);
	nPos += DL645_07_Encode_B621(&buff[nPos], &ptr->DIB622);
	nPos += DL645_07_Encode_B621(&buff[nPos], &ptr->DIB623);
	return nPos;
}

int DL645_07_Decode_B62F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB62F *ptr = (EM_DIB62F*)appl;
	
	nPos += DL645_07_Decode_B621(&ptr->DIB621, &buff[nPos]);
	nPos += DL645_07_Decode_B621(&ptr->DIB622, &buff[nPos]);
	nPos += DL645_07_Decode_B621(&ptr->DIB623, &buff[nPos]);
	
	return nPos;
}

// B630
int DL645_Encode_B630( BYTE *buff, void *appl )
{
	EM_DIB630 *ptr = (EM_DIB630*)appl;
	double2bcd(ptr->value, buff, 2, 4);
	return 3;
}

int DL645_Decode_B630( void *appl, BYTE *buff )
{
	EM_DIB630 *ptr = (EM_DIB630*)appl;
	if((buff[2] & 0x80) != 0)//为负数
	{
		buff[2] &= 0x7f;
		ptr->value = bcd2double(buff, 3, 4 ) * (-1);		
	}
	else
	{
		ptr->value = bcd2double(buff, 3, 4 );
	}		
	return 3;
}

int DL645_07_Encode_B630( BYTE *buff, void *appl )
{
	EM_DIB630 *ptr = (EM_DIB630*)appl;
	double2bcd(ptr->value, buff, 3, 4);
	return 3;
}

int DL645_07_Decode_B630( void *appl, BYTE *buff )
{
	EM_DIB630 *ptr = (EM_DIB630*)appl;
	if((buff[2] & 0x80) != 0)//为负数
	{
		buff[2] &= 0x7f;
		ptr->value = bcd2double(buff, 3, 4) * -1;
	}
	else
	{
		ptr->value = bcd2double(buff, 3, 4);
	}
	return 3;
}

// B634
int DL645_Encode_B634( BYTE *buff, void *appl )
{
	EM_DIB634 *ptr = (EM_DIB634*)appl;
	double2bcd(ptr->value, buff, 2, 2);
	return 2;
}

int DL645_Decode_B634( void *appl, BYTE *buff )
{
	EM_DIB634 *ptr = (EM_DIB634*)appl;
	ptr->value = bcd2double(buff, 2, 2);
	return 2;
}

// B63F
int DL645_Encode_B63F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB63F *ptr = (EM_DIB63F*)appl;
	nPos += DL645_Encode_B630(&buff[nPos], &ptr->DIB630);
	nPos += DL645_Encode_B630(&buff[nPos], &ptr->DIB631);
	nPos += DL645_Encode_B630(&buff[nPos], &ptr->DIB632);
	nPos += DL645_Encode_B630(&buff[nPos], &ptr->DIB633);
	return nPos;
}

int DL645_Decode_B63F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB63F *ptr = (EM_DIB63F*)appl;
	nPos += DL645_Decode_B630(&ptr->DIB630, &buff[nPos]);
	nPos += DL645_Decode_B630(&ptr->DIB631, &buff[nPos]);
	nPos += DL645_Decode_B630(&ptr->DIB632, &buff[nPos]);
	nPos += DL645_Decode_B630(&ptr->DIB633, &buff[nPos]);
	return nPos;
}

int DL645_07_Encode_B63F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB63F *ptr = (EM_DIB63F*)appl;
	nPos += DL645_07_Encode_B630(&buff[nPos], &ptr->DIB630);
	nPos += DL645_07_Encode_B630(&buff[nPos], &ptr->DIB631);
	nPos += DL645_07_Encode_B630(&buff[nPos], &ptr->DIB632);
	nPos += DL645_07_Encode_B630(&buff[nPos], &ptr->DIB633);
	return nPos;
}

int DL645_07_Decode_B63F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB63F *ptr = (EM_DIB63F*)appl;
	nPos += DL645_07_Decode_B630(&ptr->DIB630, &buff[nPos]);
	nPos += DL645_07_Decode_B630(&ptr->DIB631, &buff[nPos]);
	nPos += DL645_07_Decode_B630(&ptr->DIB632, &buff[nPos]);
	nPos += DL645_07_Decode_B630(&ptr->DIB633, &buff[nPos]);
	return nPos;
}

// B640
int DL645_Encode_B640( BYTE *buff, void *appl )
{
	EM_DIB640 *ptr = (EM_DIB640*)appl;
	double2bcd(ptr->value, buff, 2, 2);
	return 2;
}

int DL645_Decode_B640( void *appl, BYTE *buff )
{
	EM_DIB640 *ptr = (EM_DIB640*)appl;
	ptr->value = bcd2double(buff, 2, 2);
	return 2;
}

// B64F
int DL645_Encode_B64F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB64F *ptr = (EM_DIB64F*)appl;
	nPos += DL645_Encode_B640(&buff[nPos], &ptr->DIB640);
	nPos += DL645_Encode_B640(&buff[nPos], &ptr->DIB641);
	nPos += DL645_Encode_B640(&buff[nPos], &ptr->DIB642);
	nPos += DL645_Encode_B640(&buff[nPos], &ptr->DIB643);
	return nPos;
}

int DL645_Decode_B64F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB64F *ptr = (EM_DIB64F*)appl;
	nPos += DL645_Decode_B640(&ptr->DIB640, &buff[nPos]);
	nPos += DL645_Decode_B640(&ptr->DIB641, &buff[nPos]);
	nPos += DL645_Decode_B640(&ptr->DIB642, &buff[nPos]);
	nPos += DL645_Decode_B640(&ptr->DIB643, &buff[nPos]);
	return nPos;
}

// B650
int DL645_Encode_B650( BYTE *buff, void *appl )
{
	EM_DIB650 *ptr = (EM_DIB650*)appl;
	double2bcd(ptr->value, buff, 2, 3);
	return 2;
}

int DL645_Decode_B650( void *appl, BYTE *buff )
{
	EM_DIB650 *ptr = (EM_DIB650*)appl;
	ptr->value = bcd2double(buff, 2, 3);
	return 2;
}

// B65F
int DL645_Encode_B65F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB65F *ptr = (EM_DIB65F*)appl;
	nPos += DL645_Encode_B650(&buff[nPos], &ptr->DIB650);
	nPos += DL645_Encode_B650(&buff[nPos], &ptr->DIB651);
	nPos += DL645_Encode_B650(&buff[nPos], &ptr->DIB652);
	nPos += DL645_Encode_B650(&buff[nPos], &ptr->DIB653);
	return nPos;
}

int DL645_Decode_B65F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB65F *ptr = (EM_DIB65F*)appl;
	nPos += DL645_Decode_B650(&ptr->DIB650, &buff[nPos]);
	nPos += DL645_Decode_B650(&ptr->DIB651, &buff[nPos]);
	nPos += DL645_Decode_B650(&ptr->DIB652, &buff[nPos]);
	nPos += DL645_Decode_B650(&ptr->DIB653, &buff[nPos]);
	return nPos;
}

// B660
int DL645_Encode_B660( BYTE *buff, void *appl )
{
	EM_DIB660 *ptr = (EM_DIB660*)appl;
	double2bcd(ptr->value, buff, 2, 1);
	return 2;
}

int DL645_Decode_B660( void *appl, BYTE *buff )
{
	EM_DIB660 *ptr = (EM_DIB660*)appl;
	ptr->value = bcd2double(buff, 2, 1);
	return 2;
}

int DL645_07_Encode_B660( BYTE *buff, void *appl )
{
	EM_DIB660 *ptr = (EM_DIB660*)appl;
	double2bcd(ptr->value, buff, 2, 1);
	return 2;
}

int DL645_07_Decode_B660( void *appl, BYTE *buff )
{
	EM_DIB660 *ptr = (EM_DIB660*)appl;
	ptr->value = bcd2double(buff, 2, 1);
	return 2;
}

// B66F
int DL645_Encode_B66F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB66F *ptr = (EM_DIB66F*)appl;
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB660);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB661);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB662);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB663);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB664);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB665);
	return nPos;
}

int DL645_Decode_B66F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB66F *ptr = (EM_DIB66F*)appl;
	nPos += DL645_Decode_B660(&ptr->DIB660, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB661, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB662, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB663, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB664, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB665, &buff[nPos]);
	return nPos;
}

int DL645_07_Encode_B66F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIB66F *ptr = (EM_DIB66F*)appl;
	nPos += DL645_07_Encode_B660(&buff[nPos], &ptr->DIB660);
	nPos += DL645_07_Encode_B660(&buff[nPos], &ptr->DIB661);
	nPos += DL645_07_Encode_B660(&buff[nPos], &ptr->DIB662);
	return nPos;
}

int DL645_07_Decode_B66F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIB66F *ptr = (EM_DIB66F*)appl;
	nPos += DL645_07_Decode_B660(&ptr->DIB660, &buff[nPos]);
	nPos += DL645_07_Decode_B660(&ptr->DIB661, &buff[nPos]);
	nPos += DL645_07_Decode_B660(&ptr->DIB662, &buff[nPos]);
	return nPos;
}

// C010
int DL645_Encode_C010( BYTE *buff, void* )
{
	int nPos = 0;
	
	time_t hTime = time(nullptr);
	tm* tm_sys = localtime(&hTime);

	hex2bcd(tm_sys->tm_wday, &buff[nPos++]);
	hex2bcd(tm_sys->tm_mday, &buff[nPos++]);
	hex2bcd(tm_sys->tm_mon,  &buff[nPos++]);
	hex2bcd(tm_sys->tm_year, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C010( void *appl, BYTE *buff )
{
	EM_DIC010 *ptr = (EM_DIC010*)appl;
	int nPos = 0;
	ptr->w_day	= bcd2hex(buff[nPos++]);
	ptr->m_day	= bcd2hex(buff[nPos++]);
	ptr->month	= bcd2hex(buff[nPos++]);
	ptr->year	= bcd2hex(buff[nPos++]);
	return nPos;
}

// C010
int DL645_Encode_C010_HB( BYTE *buff, void* )
{
	int nPos = 0;
	
	time_t hTime = time(nullptr);
	tm* tm_sys = localtime(&hTime);
	buff[nPos++] = 0;	
	hex2bcd(tm_sys->tm_mday, &buff[nPos++]);
	hex2bcd(tm_sys->tm_mon,  &buff[nPos++]);
	hex2bcd(tm_sys->tm_year, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C010_HB( void *appl, BYTE *buff )
{
	EM_DIC010 *ptr = (EM_DIC010*)appl;
	int nPos = 0;
	ptr->w_day	= 0;
	ptr->m_day	= bcd2hex(buff[nPos++]);
	ptr->month	= bcd2hex(buff[nPos++]);
	ptr->year	= bcd2hex(buff[nPos++]);
	return nPos;
}

// C011
int DL645_Encode_C011( BYTE *buff, void* )
{
	int nPos = 0;
	time_t hTime = time(nullptr);
	tm* tm_sys = localtime(&hTime);

	hex2bcd(tm_sys->tm_sec,  &buff[nPos++]);
	hex2bcd(tm_sys->tm_min,  &buff[nPos++]);
	hex2bcd(tm_sys->tm_hour, &buff[nPos++]);
	
	return nPos;
}

int DL645_Decode_C011( void *appl, BYTE *buff )
{
	EM_DIC011 *ptr = (EM_DIC011*)appl;
	int nPos = 0;
	ptr->sec    = bcd2hex(buff[nPos++]);
	ptr->minute = bcd2hex(buff[nPos++]);
	ptr->hour	= bcd2hex(buff[nPos++]);
	return nPos;
}

//C01F
int DL645_Encode_C01F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIC01F *ptr = (EM_DIC01F*)appl;
	nPos += DL645_Encode_C010(&buff[nPos], &ptr->DIC010);
	nPos += DL645_Encode_C011(&buff[nPos], &ptr->DIC011);	
	return nPos;
}

int DL645_Decode_C01F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIC01F *ptr = (EM_DIC01F*)appl;
	nPos += DL645_Decode_C010(&ptr->DIC010, &buff[nPos]);
	nPos += DL645_Decode_C011(&ptr->DIC011, &buff[nPos]);	
	return nPos;
}

// C020
int DL645_Encode_C020( BYTE *buff, void *appl )
{
	EM_DIC020 *ptr = (EM_DIC020*)appl;
	int nPos = 0;
	buff[nPos++] = ptr->status;
	return nPos;
}

int DL645_Decode_C020( void *appl, BYTE *buff )
{
	EM_DIC020 *ptr = (EM_DIC020*)appl;
	int nPos = 0;
	ptr->status = buff[nPos++];
	return nPos;
}

int DL645_07_Decode_C020( void *appl, BYTE *buff )
{
	memcpy(appl, buff, 14);
	return 14;
	EM_DIC020 *ptr = (EM_DIC020*)appl;
	int nPos = 0;
	ptr->status = buff[nPos++];
	return nPos;
}

// C021
int DL645_Encode_C021( BYTE *buff, void *appl )
{
	EM_DIC021 *ptr = (EM_DIC021*)appl;
	int nPos = 0;
	buff[nPos++] = ptr->status;
	return nPos;
}

int DL645_Decode_C021( void *appl, BYTE *buff )
{
	EM_DIC021 *ptr = (EM_DIC021*)appl;
	int nPos = 0;
	ptr->status = buff[nPos++];
	return nPos;
}

// C022
int DL645_Encode_C022( BYTE *buff, void *appl )
{
	EM_DIC022 *ptr = (EM_DIC022*)appl;
	int nPos = 0;
	buff[nPos++] = ptr->status;
	return nPos;
}

int DL645_Decode_C022( void *appl, BYTE *buff )
{
	EM_DIC022 *ptr = (EM_DIC022*)appl;
	int nPos = 0;
	ptr->status = buff[nPos++];
	return nPos;
}

//C02F
int DL645_Encode_C02F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIC02F *ptr = (EM_DIC02F*)appl;
	nPos += DL645_Encode_C020(&buff[nPos], &ptr->DIC020);
	nPos += DL645_Encode_C021(&buff[nPos], &ptr->DIC021);
	nPos += DL645_Encode_C022(&buff[nPos], &ptr->DIC022);	
	return nPos;
}

int DL645_Decode_C02F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIC02F *ptr = (EM_DIC02F*)appl;
	nPos += DL645_Decode_C020(&ptr->DIC020, &buff[nPos]);
	nPos += DL645_Decode_C021(&ptr->DIC021, &buff[nPos]);	
	nPos += DL645_Decode_C022(&ptr->DIC022, &buff[nPos]);
	return nPos;
}

// C030
int DL645_Encode_C030( BYTE *buff, void *appl )
{
	EM_DIC030 *ptr = (EM_DIC030*)appl;
	BYTE temp = 0;
	long2bcd(ptr->value, &buff[0], &buff[1], &buff[2], &temp);
	return 3;
}

int DL645_Decode_C030( void *appl, BYTE *buff )
{
	EM_DIC030 *ptr = (EM_DIC030*)appl;
	bcd2long(buff[0], buff[1], buff[2], 0, &ptr->value);
	return 3;
}

// C031
int DL645_Encode_C031( BYTE *buff, void *appl )
{
	EM_DIC031 *ptr = (EM_DIC031*)appl;
	BYTE temp = 0;
	long2bcd(ptr->value, &buff[0], &buff[1], &buff[2], &temp);
	return 3;
}

int DL645_Decode_C031( void *appl, BYTE *buff )
{
	EM_DIC031 *ptr = (EM_DIC031*)appl;
	bcd2long(buff[0], buff[1], buff[2], 0, &ptr->value);
	return 3;
}

// C032
int DL645_Encode_C032( BYTE *buff, void *appl )
{
	EM_DIC032 *ptr = (EM_DIC032*)appl;
	memcpy(buff, ptr->host, 6);
	return 6;
}

int DL645_Decode_C032( void *appl, BYTE *buff )
{
	EM_DIC032 *ptr = (EM_DIC032*)appl;
	memcpy(ptr->host, buff, 6);
	return 6;
}

// C033
int DL645_Encode_C033( BYTE *buff, void *appl )
{
	EM_DIC033 *ptr = (EM_DIC033*)appl;
	memcpy(buff, ptr->user, 6);
	return 6;
}

int DL645_Decode_C033( void *appl, BYTE *buff )
{
	EM_DIC033 *ptr = (EM_DIC033*)appl;
	memcpy(ptr->user, buff, 6);
	return 6;
}

// C034
int DL645_Encode_C034( BYTE *buff, void *appl )
{
	EM_DIC034 *ptr = (EM_DIC034*)appl;
	memcpy(buff, ptr->fcode, 6);
	return 6;
}

int DL645_Decode_C034( void *appl, BYTE *buff )
{
	EM_DIC034 *ptr = (EM_DIC034*)appl;
	memcpy(ptr->fcode, buff, 6);
	return 6;
}

//C03F
int DL645_Encode_C03F( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIC03F *ptr = (EM_DIC03F*)appl;
	nPos += DL645_Encode_C030(&buff[nPos], &ptr->DIC030);
	nPos += DL645_Encode_C031(&buff[nPos], &ptr->DIC031);
	nPos += DL645_Encode_C032(&buff[nPos], &ptr->DIC032);	
	nPos += DL645_Encode_C033(&buff[nPos], &ptr->DIC033);
	nPos += DL645_Encode_C034(&buff[nPos], &ptr->DIC034);	
	return nPos;
}

int DL645_Decode_C03F( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIC03F *ptr = (EM_DIC03F*)appl;
	nPos += DL645_Decode_C030(&ptr->DIC030, &buff[nPos]);
	nPos += DL645_Decode_C031(&ptr->DIC031, &buff[nPos]);	
	nPos += DL645_Decode_C032(&ptr->DIC032, &buff[nPos]);
	nPos += DL645_Decode_C033(&ptr->DIC033, &buff[nPos]);	
	nPos += DL645_Decode_C034(&ptr->DIC034, &buff[nPos]);
	return nPos;
}

// C111
int DL645_Encode_C111( BYTE *buff, void *appl )
{
	EM_DIC111 *ptr = (EM_DIC111*)appl;
	int nPos = 0;
	hex2bcd(ptr->minutes, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C111( void *appl, BYTE *buff )
{
	EM_DIC111 *ptr = (EM_DIC111*)appl;
	int nPos = 0;
	ptr->minutes = bcd2hex(buff[nPos++]);
	return nPos;
}

// C112
int DL645_Encode_C112( BYTE *buff, void *appl )
{
	EM_DIC112 *ptr = (EM_DIC112*)appl;
	int nPos = 0;
	hex2bcd(ptr->minutes, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C112( void *appl, BYTE *buff )
{
	EM_DIC112 *ptr = (EM_DIC112*)appl;
	int nPos = 0;
	ptr->minutes = bcd2hex(buff[nPos++]);
	return nPos;
}

// C113
int DL645_Encode_C113( BYTE *buff, void *appl )
{
	EM_DIC113 *ptr = (EM_DIC113*)appl;
	int nPos = 0;
	hex2bcd(ptr->sec, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C113( void *appl, BYTE *buff )
{
	EM_DIC113 *ptr = (EM_DIC113*)appl;
	int nPos = 0;
	ptr->sec = bcd2hex(buff[nPos++]);
	return nPos;
}

// C114
int DL645_Encode_C114( BYTE *buff, void *appl )
{
	EM_DIC114 *ptr = (EM_DIC114*)appl;
	int nPos = 0;
	hex2bcd(ptr->sec, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C114( void *appl, BYTE *buff )
{
	EM_DIC114 *ptr = (EM_DIC114*)appl;
	int nPos = 0;
	ptr->sec = bcd2hex(buff[nPos++]);
	return nPos;
}

// C115
int DL645_Encode_C115( BYTE *buff, void *appl )
{
	EM_DIC115 *ptr = (EM_DIC115*)appl;
	int nPos = 0;
	hex2bcd(ptr->bits, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C115( void *appl, BYTE *buff )
{
	EM_DIC115 *ptr = (EM_DIC115*)appl;
	int nPos = 0;
	ptr->bits = bcd2hex(buff[nPos++]);
	return nPos;
}

// C116
int DL645_Encode_C116( BYTE *buff, void *appl )
{
	EM_DIC116 *ptr = (EM_DIC116*)appl;
	int nPos = 0;
	hex2bcd(ptr->bits, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C116( void *appl, BYTE *buff )
{
	EM_DIC116 *ptr = (EM_DIC116*)appl;
	int nPos = 0;
	ptr->bits = bcd2hex(buff[nPos++]);
	return nPos;
}

// C117
int DL645_Encode_C117( BYTE *buff, void *appl )
{
	EM_DIC117 *ptr = (EM_DIC117*)appl;
	int nPos = 0;
	hex2bcd(ptr->hour, &buff[nPos++]);
	hex2bcd(ptr->day,  &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C117( void *appl, BYTE *buff )
{
	EM_DIC117 *ptr = (EM_DIC117*)appl;
	int nPos = 0;
	ptr->hour = bcd2hex(buff[nPos++]);
	ptr->day  = bcd2hex(buff[nPos++]);
	return nPos;
}

// C118
int DL645_Encode_C118( BYTE *buff, void *appl )
{
	EM_DIC118 *ptr = (EM_DIC118*)appl;
	int nPos = 0;
	hex2bcd(ptr->day, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C118( void *appl, BYTE *buff )
{
	EM_DIC118 *ptr = (EM_DIC118*)appl;
	int nPos = 0;
	ptr->day = bcd2hex(buff[nPos++]);
	return nPos;
}

// C119
int DL645_Encode_C119( BYTE *buff, void *appl )
{
	EM_DIC119 *ptr = (EM_DIC119*)appl;
	double2bcd(ptr->value, buff, 6, 2);
	return 4;
}

int DL645_Decode_C119( void *appl, BYTE *buff )
{
	EM_DIC119 *ptr = (EM_DIC119*)appl;
	ptr->value = bcd2double(buff, 4, 2);
	return 4;
}

// C11A
int DL645_Encode_C11A( BYTE *buff, void *appl )
{
	EM_DIC11A *ptr = (EM_DIC11A*)appl;
	double2bcd(ptr->value, buff, 6, 2);
	return 4;
}

int DL645_Decode_C11A( void *appl, BYTE *buff )
{
	EM_DIC11A *ptr = (EM_DIC11A*)appl;
	ptr->value = bcd2double(buff, 4, 2);
	return 4;
}

// C119_07
int DL645_07_Decode_C119( void *appl, BYTE *buff )
{
	EM_DIC119 *ptr = (EM_DIC119*)appl;
	ptr->value = bcd2double(buff, 3, 0);
	return 3;
}

// C211
int DL645_Encode_C211( BYTE *buff, void *appl )
{
	EM_DIC211 *ptr = (EM_DIC211*)appl;
	word2bcd(ptr->ms, &buff[0], &buff[1]);
	return 2;
}

int DL645_Decode_C211( void *appl, BYTE *buff )
{
	EM_DIC211 *ptr = (EM_DIC211*)appl;
	bcd2word(buff[0], buff[1], &ptr->ms);
	return 2;
}

// C212
int DL645_Encode_C212( BYTE *buff, void *appl )
{
	EM_DIC212 *ptr = (EM_DIC212*)appl;
	for( int i = 0; i < 3; i++ )
	{
		hex2bcd(ptr->pass[i], &buff[i]);
	}
	return 3;
}

int DL645_Decode_C212( void *appl, BYTE *buff )
{
	EM_DIC212 *ptr = (EM_DIC212*)appl;
	for( int i = 0; i < 3; i++ )
	{
		ptr->pass[i] = bcd2hex(buff[i]);
	}
	return 3;
}

// C310
int DL645_Encode_C310( BYTE *buff, void *appl )
{
	EM_DIC310 *ptr = (EM_DIC310*)appl;
	int nPos = 0;
	hex2bcd(ptr->num, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C310( void *appl, BYTE *buff )
{
	EM_DIC310 *ptr = (EM_DIC310*)appl;
	int nPos = 0;
	ptr->num = bcd2hex(buff[nPos++]);
	return nPos;
}

// C311
int DL645_Encode_C311( BYTE *buff, void *appl )
{
	EM_DIC311 *ptr = (EM_DIC311*)appl;
	int nPos = 0;
	hex2bcd(ptr->num, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C311( void *appl, BYTE *buff )
{
	EM_DIC311 *ptr = (EM_DIC311*)appl;
	int nPos = 0;
	ptr->num = bcd2hex(buff[nPos++]);
	return nPos;
}

// C312
int DL645_Encode_C312( BYTE *buff, void *appl )
{
	EM_DIC312 *ptr = (EM_DIC312*)appl;
	int nPos = 0;
	hex2bcd(ptr->num, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C312( void *appl, BYTE *buff )
{
	EM_DIC312 *ptr = (EM_DIC312*)appl;
	int nPos = 0;
	ptr->num = bcd2hex(buff[nPos++]);
	return nPos;
}

// C313
int DL645_Encode_C313( BYTE *buff, void *appl )
{
	EM_DIC313 *ptr = (EM_DIC313*)appl;
	int nPos = 0;
	hex2bcd(ptr->num, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C313( void *appl, BYTE *buff )
{
	EM_DIC313 *ptr = (EM_DIC313*)appl;
	int nPos = 0;
	ptr->num = bcd2hex(buff[nPos++]);
	return nPos;
}

// C314
int DL645_Encode_C314( BYTE *buff, void *appl )
{
	EM_DIC314 *ptr = (EM_DIC314*)appl;
	int nPos = 0;
	hex2bcd(ptr->num, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C314( void *appl, BYTE *buff )
{
	EM_DIC314 *ptr = (EM_DIC314*)appl;
	int nPos = 0;
	ptr->num = bcd2hex(buff[nPos++]);
	return nPos;
}

// C32P
int DL645_Encode_C32P( BYTE *buff, void *appl )
{
	EM_DIC32P *ptr = (EM_DIC32P*)appl;
	int nPos = 0;
	hex2bcd(ptr->NN,    &buff[nPos++]);
	hex2bcd(ptr->day,   &buff[nPos++]);
	hex2bcd(ptr->month, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C32P( void *appl, BYTE *buff )
{
	EM_DIC32P *ptr = (EM_DIC32P*)appl;
	int nPos = 0;
	ptr->NN    = bcd2hex(buff[nPos++]);
	ptr->day   = bcd2hex(buff[nPos++]);
	ptr->month = bcd2hex(buff[nPos++]);
	return nPos;
}

// C3QM
int DL645_Encode_C3QM( BYTE *buff, void *appl )
{
	EM_DIC3QM *ptr = (EM_DIC3QM*)appl;
	int nPos = 0;
	hex2bcd(ptr->NN,     &buff[nPos++]);
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C3QM( void *appl, BYTE *buff )
{
	EM_DIC3QM *ptr = (EM_DIC3QM*)appl;
	int nPos = 0;
	ptr->NN	    = bcd2hex(buff[nPos++]);
	ptr->minute = bcd2hex(buff[nPos++]);
	ptr->hour	= bcd2hex(buff[nPos++]);
	return nPos;
}

// C3QF
int DL645_Encode_C3QF( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIC3QF *ptr = (EM_DIC3QF*)appl;
	nPos += DL645_Encode_C3QM(&buff[nPos], &ptr->DIC3QM[0]);
	nPos += DL645_Encode_C3QM(&buff[nPos], &ptr->DIC3QM[1]);
	nPos += DL645_Encode_C3QM(&buff[nPos], &ptr->DIC3QM[2]);
	nPos += DL645_Encode_C3QM(&buff[nPos], &ptr->DIC3QM[3]);
	nPos += DL645_Encode_C3QM(&buff[nPos], &ptr->DIC3QM[4]);
	nPos += DL645_Encode_C3QM(&buff[nPos], &ptr->DIC3QM[5]);
	nPos += DL645_Encode_C3QM(&buff[nPos], &ptr->DIC3QM[6]);
	nPos += DL645_Encode_C3QM(&buff[nPos], &ptr->DIC3QM[7]);
	return nPos;
}

int DL645_Decode_C3QF( void *appl, BYTE *buff )
{
	int nPos = 0;
	EM_DIC3QF *ptr = (EM_DIC3QF*)appl;
	nPos += DL645_Decode_C3QM(&ptr->DIC3QM[0], &buff[nPos]);
	nPos += DL645_Decode_C3QM(&ptr->DIC3QM[1], &buff[nPos]);
	nPos += DL645_Decode_C3QM(&ptr->DIC3QM[2], &buff[nPos]);
	nPos += DL645_Decode_C3QM(&ptr->DIC3QM[3], &buff[nPos]);
	nPos += DL645_Decode_C3QM(&ptr->DIC3QM[4], &buff[nPos]);
	nPos += DL645_Decode_C3QM(&ptr->DIC3QM[5], &buff[nPos]);
	nPos += DL645_Decode_C3QM(&ptr->DIC3QM[6], &buff[nPos]);
	nPos += DL645_Decode_C3QM(&ptr->DIC3QM[7], &buff[nPos]);
	return nPos;
}

// C41N
int DL645_Encode_C41N( BYTE *buff, void *appl )
{
	EM_DIC41N *ptr = (EM_DIC41N*)appl;
	int nPos = 0;
	hex2bcd(ptr->NN,    &buff[nPos++]);
	hex2bcd(ptr->day,   &buff[nPos++]);
	hex2bcd(ptr->month, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C41N( void *appl, BYTE *buff )
{
	EM_DIC41N *ptr = (EM_DIC41N*)appl;
	int nPos = 0;
	ptr->NN	   = bcd2hex(buff[nPos++]);
	ptr->day   = bcd2hex(buff[nPos++]);
	ptr->month = bcd2hex(buff[nPos++]);
	return nPos;
}

// C41E
int DL645_Encode_C41E( BYTE *buff, void *appl )
{
	EM_DIC41E *ptr = (EM_DIC41E*)appl;
	int nPos = 0;
	hex2bcd(ptr->NN, &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C41E( void *appl, BYTE *buff )
{
	EM_DIC41E *ptr = (EM_DIC41E*)appl;
	int nPos = 0;
	ptr->NN = bcd2hex(buff[nPos++]);
	return nPos;
}

// C510
int DL645_Encode_C510( BYTE *buff, void *appl )
{
	EM_DIC510 *ptr = (EM_DIC510*)appl;
	int nPos = 0;
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	hex2bcd(ptr->day,    &buff[nPos++]);
	hex2bcd(ptr->month,  &buff[nPos++]);
	return nPos;
}

int DL645_Decode_C510( void *appl, BYTE *buff )
{
	EM_DIC510 *ptr = (EM_DIC510*)appl;
	int nPos = 0;
	ptr->minute = bcd2hex(buff[nPos++]);
	ptr->hour	= bcd2hex(buff[nPos++]);
	ptr->day	= bcd2hex(buff[nPos++]);
	ptr->month  = bcd2hex(buff[nPos++]);
	return nPos;
}

// C511
int DL645_Encode_C511( BYTE *buff, void *appl )
{
	EM_DIC511 *ptr = (EM_DIC511*)appl;
	word2bcd(ptr->minute, &buff[0], &buff[1]);
	return 2;
}

int DL645_Decode_C511( void *appl, BYTE *buff )
{
	EM_DIC511 *ptr = (EM_DIC511*)appl;
	bcd2word(buff[0], buff[1], &ptr->minute);
	return 2;
}

// E207
int DL645_Encode_E207( BYTE *buff, void *appl )
{
	int nPos = 0;
	EM_DIE207 *ptr = (EM_DIE207*)appl;
	nPos += DL645_Encode_C011(&buff[nPos], &ptr->DIC011);
	nPos += DL645_Encode_B630(&buff[nPos], &ptr->DIB630);
	nPos += DL645_Encode_B630(&buff[nPos], &ptr->DIB640);
	nPos += DL645_Encode_B611(&buff[nPos], &ptr->DIB611);
	nPos += DL645_Encode_B611(&buff[nPos], &ptr->DIB612);
	nPos += DL645_Encode_B611(&buff[nPos], &ptr->DIB613);
	nPos += DL645_Encode_B621(&buff[nPos], &ptr->DIB621);
	nPos += DL645_Encode_B621(&buff[nPos], &ptr->DIB622);
	nPos += DL645_Encode_B621(&buff[nPos], &ptr->DIB623);
	nPos += DL645_Encode_Axxx(&buff[nPos], &ptr->DIA010);
	nPos += DL645_Encode_Axxx(&buff[nPos], &ptr->DIA020);
	return nPos;	
}

// E207
int DL645_Decode_E207( void *appl,  BYTE *buff)
{
	int nPos = 0;
	EM_DIE207 *ptr = (EM_DIE207*)appl;
	nPos += DL645_Decode_C011(&ptr->DIC011, &buff[nPos]);
	nPos += DL645_Decode_B630(&ptr->DIB630, &buff[nPos]);
	nPos += DL645_Decode_B630(&ptr->DIB640, &buff[nPos]);
	nPos += DL645_Decode_B611(&ptr->DIB611, &buff[nPos]);
	nPos += DL645_Decode_B611(&ptr->DIB612, &buff[nPos]);
	nPos += DL645_Decode_B611(&ptr->DIB613, &buff[nPos]);
	nPos += DL645_Decode_B621(&ptr->DIB621, &buff[nPos]);
	nPos += DL645_Decode_B621(&ptr->DIB622, &buff[nPos]);
	nPos += DL645_Decode_B621(&ptr->DIB623, &buff[nPos]);	
	nPos += DL645_Decode_Axxx(&ptr->DIA010, &buff[nPos]);
	nPos += DL645_Decode_Axxx(&ptr->DIA020, &buff[nPos]);	
	return nPos;	
}

// E209
int DL645_Encode_E209( BYTE *buff, void *appl )
{	
	int nPos = 0;
	EM_DIE209 *ptr = (EM_DIE209*)appl;
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB661);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB662);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB663);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB664);
	nPos += DL645_Encode_B660(&buff[nPos], &ptr->DIB665);

	return nPos;	
}

// E209
int DL645_Decode_E209( void *appl, BYTE *buff )
{	
	int nPos = 0;
	EM_DIE209 *ptr = (EM_DIE209*)appl;
	nPos += DL645_Decode_B660(&ptr->DIB661, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB662, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB663, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB664, &buff[nPos]);
	nPos += DL645_Decode_B660(&ptr->DIB665, &buff[nPos]);

	return nPos;	
}

// E211
int DL645_Encode_E211( BYTE *buff, void *appl )
{	
	int nPos = 0;
	EM_DIE211 *ptr = (EM_DIE211*)appl;
	nPos += DL645_Encode_C011(&buff[nPos], &ptr->DIC011);
	nPos += DL645_Encode_C010_HB(&buff[nPos], &ptr->DIC010);	
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9410);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9510);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9530);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9010);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9110);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9130);
	for(BYTE i = 0; i < 16; i++)
	{
		buff[nPos++] = 0;		
	}
	buff[nPos++] = ptr->DIC117;
	
	return nPos;	
}

// E211
int DL645_Decode_E211( void *appl,  BYTE *buff)
{
	int nPos = 0;
	EM_DIE211 *ptr = (EM_DIE211*)appl;
	nPos += DL645_Decode_C011(&ptr->DIC011, &buff[nPos]);
	nPos += DL645_Decode_C010_HB(&ptr->DIC010, &buff[nPos]);	
	nPos += DL645_Decode_9xxx(&ptr->DI9410, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9510, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9530, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9010, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9110, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9130, &buff[nPos]);
	nPos += 16;
	ptr->DIC117 = buff[nPos++];
	return nPos;	
}

// E221
int DL645_Encode_E221( BYTE *buff, void *appl )
{	
	int nPos = 0;
	EM_DIE221 *ptr = (EM_DIE221*)appl;
	nPos += DL645_Encode_C011(&buff[nPos], &ptr->DIC011);
	nPos += DL645_Encode_C010_HB(&buff[nPos], &ptr->DIC010);	
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9410);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9510);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9530);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9010);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9110);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DI9130);
	for(BYTE i = 0; i < 12; i++)
	{
		buff[nPos++] = 0;		
	}
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DIE401);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DIE402);
	nPos += DL645_Encode_9xxx(&buff[nPos], &ptr->DIE403);
	for(BYTE i = 0; i < 4; i++)
	{
		buff[nPos++] = 0;		
	}
	buff[nPos++] = ptr->DIC117;
	
	return nPos;	
}

// E221
int DL645_Decode_E221( void *appl,  BYTE *buff)
{
	int nPos = 0;
	EM_DIE221 *ptr = (EM_DIE221*)appl;
	nPos += DL645_Decode_C011(&ptr->DIC011, &buff[nPos]);
	nPos += DL645_Decode_C010_HB(&ptr->DIC010, &buff[nPos]);	
	nPos += DL645_Decode_9xxx(&ptr->DI9410, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9510, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9530, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9010, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9110, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DI9130, &buff[nPos]);
	nPos += 12;
	nPos += DL645_Decode_9xxx(&ptr->DIE401, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DIE402, &buff[nPos]);
	nPos += DL645_Decode_9xxx(&ptr->DIE403, &buff[nPos]);
	
	nPos += 4;		
	
	ptr->DIC117 = buff[nPos++];
	return nPos;	
}
//D40B
int DL645_Encode_D40B( BYTE *buff, void *appl )
{
	EM_DID40B *ptr = (EM_DID40B*)appl;
	int nPos = 0;
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	hex2bcd(ptr->day,    &buff[nPos++]);
	hex2bcd(ptr->month,  &buff[nPos++]);
	hex2bcd(ptr->duminute, &buff[nPos++]);
	hex2bcd(ptr->duhour,   &buff[nPos++]);
	hex2bcd(ptr->duday,    &buff[nPos++]);
	hex2bcd(ptr->dumonth,  &buff[nPos++]);
	return nPos;
}

int DL645_Decode_D40B( void *appl, BYTE *buff )
{
	EM_DID40B *ptr = (EM_DID40B*)appl;
	int nPos = 0;
	ptr->minute   = bcd2hex(buff[nPos++]);
	ptr->hour	  = bcd2hex(buff[nPos++]);
	ptr->day	  = bcd2hex(buff[nPos++]);
	ptr->month    = bcd2hex(buff[nPos++]);
	ptr->duminute = bcd2hex(buff[nPos++]);
	ptr->duhour   = bcd2hex(buff[nPos++]);
	ptr->duday	  = bcd2hex(buff[nPos++]);
	ptr->dumonth  = bcd2hex(buff[nPos++]);
	return nPos;
}

//D40D
int DL645_Encode_D40D( BYTE *buff, void *appl )
{
	EM_DID40D *ptr = (EM_DID40D*)appl;
	word2bcd(ptr->times, &buff[0], &buff[1]);
	word2bcd(ptr->dutimes, &buff[2], &buff[3]);
	return 4;
}

int DL645_Decode_D40D( void *appl, BYTE *buff )
{
	EM_DID40D *ptr = (EM_DID40D*)appl;
	bcd2word(buff[0], buff[1], &ptr->times);
	bcd2word(buff[2], buff[3], &ptr->dutimes);
	return 4;
}

//D415
int DL645_Encode_D415( BYTE *buff, void *appl )
{
	EM_DID415 *ptr = (EM_DID415*)appl;
	int nPos = 0;
	word2bcd(ptr->times,  &buff[nPos++]);
	hex2bcd(ptr->minute, &buff[nPos++]);
	hex2bcd(ptr->hour,   &buff[nPos++]);
	hex2bcd(ptr->day,    &buff[nPos++]);
	hex2bcd(ptr->month,  &buff[nPos++]);
	hex2bcd(ptr->year,   &buff[nPos++]);
	
	
	return nPos;
}

int DL645_Decode_D415( void *appl, BYTE *buff )
{
	EM_DID415 *ptr = (EM_DID415*)appl;
	int nPos = 0;
	ptr->times     = bcd2word(buff[nPos++]);
	ptr->minute   = bcd2hex(buff[nPos++]);
	ptr->hour	  = bcd2hex(buff[nPos++]);
	ptr->day	  = bcd2hex(buff[nPos++]);
	ptr->month    = bcd2hex(buff[nPos++]);
    ptr->year     = bcd2hex(buff[nPos++]);
	
	
	return nPos;
}

#endif