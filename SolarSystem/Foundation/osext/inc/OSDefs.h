#ifndef OSDEFS_H_
#define OSDEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

//define Data format
#undef  BOOL 
#undef  CHAR
#undef  UCHAR
#undef  BYTE
#undef  WORD
#undef  DWORD
#undef  INT8
#undef  INT16
#undef  INT32
#undef  LONG
#undef  UINT8
#undef  UINT16
#undef  UINT32
#undef  ULONG
#undef  INT
#undef 	RET
#undef  UINT
#undef  FLOAT
#undef  DOUBLE

typedef int BOOL;
typedef char CHAR;
typedef unsigned char UCHAR;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef signed char INT8;
typedef signed short INT16;
typedef signed int INT32;
typedef signed long LONG;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long ULONG;
typedef signed int INT;
typedef signed int RET;
typedef unsigned int UINT;
typedef float FLOAT;
typedef double DOUBLE;

#undef  MAKEWORD
#undef  MAKELONG
#undef  LOWORD
#undef  HIWORD
#undef  LOBYTE
#undef  HIBYTE
#undef  BigtoLittle16
#undef  BigtoLittle32

#define MAKEWORD(a, b)      ((WORD)(((BYTE)((a) & 0xff)) | ((WORD)((BYTE)((b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG )((WORD)(a) & 0xffff) | ((DWORD)((WORD)(b) & 0xffff)) << 16)

#define LOWORD(l)           ((WORD)((DWORD)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((WORD)(w) & 0xff))
#define HIBYTE(w)           ((BYTE)((WORD)(w) >> 8))

#define BigtoLittle16(A)  ((((UINT16)(A) & 0xff00) >> 8) | \
                                 (((UINT16)(A) & 0x00ff) << 8))

#define BigtoLittle32(A)  ((((UINT32)(A) & 0xff000000) >> 24) | \
                                  (((UINT32)(A) & 0x00ff0000) >> 8) | \
                                  (((UINT32)(A) & 0x0000ff00) << 8) | \
                                  (((UINT32)(A) & 0x000000ff) << 24))

#define OSSleep(milliseconds)  shr_sleep(milliseconds)

#ifndef NULL
#define NULL		0
#endif

#ifndef TRUE
#define TRUE		1
#endif

#ifndef FALSE
#define FALSE		0
#endif

#ifndef ENABLE
#define ENABLE	  	1
#endif

#ifndef DISABLE
#define DISABLE	    0
#endif

#ifndef YES
#define YES	    	1
#endif

#ifndef NO
#define NO	    	0
#endif

#ifndef SUCCESS
#define SUCCESS	    1
#endif

#ifndef FAILURE
#define FAILURE	    0
#endif

#ifndef OS_NO_ERR
#define OS_NO_ERR 	0
#endif

#ifndef OS_ERR
#define OS_ERR		-1
#endif

#ifdef __cplusplus
}
#endif

#endif //__OS_DEFS_H__
