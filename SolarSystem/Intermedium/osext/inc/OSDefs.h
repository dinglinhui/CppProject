#ifndef OSDEFS_H_
#define OSDEFS_H_

using LONGLONG = signed long long;
using ULONGLONG = unsigned long long;

static const auto OS_THREAD_PAUSE = 100;
static const auto OS_MAX_TASK_COUNT = 100;
static const auto OS_MAX_OBJECT_NUM = 10;

enum dimension {
	one_dimension = 1, two_dimension, three_dimension
};

enum class OSRet {
	OK, ERROR, THREAD_ERROR
};

enum class MSGType {
	MSG_NULL,		// NULL消息
	MSG_ACT,		// 异步完成消息
	MSG_GETSERVICE,	// 获取系统服务
	MSG_CREATE,
	MSG_DESTROY,
	MSG_MOVE,
	MSG_SIZE,
	MSG_USER,
	CIL_SENDPACKET,
	CIL_RECVPACKET,
	CIL_TIMEOUT
};

// Protocol type
typedef enum {
	PCOL_STREAM = 0,
	PCOL_DL645 = 1,
	PCOL_FA = 2,
	PCOL_FK = 3,
	PCOL_RTM = 4,
	PCOL_UPGRADE = 5,
	PCOL_EAST = 6,
	PCOL_GB = 7
} PCOL_TYPE;

// Command type
typedef enum {
	PCMD_REQUEST = 1, PCMD_REQUESTANY = 2, PCMD_RESPOND = 3, PCMD_BROADCAST = 4
} PCMD_TYPE;

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
//#undef  LONGLONG
#undef  UINT8
#undef  UINT16
#undef  UINT32
#undef  ULONG
//#undef  ULONGLONG
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
//typedef signed long long LONGLONG;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long ULONG;
//typedef unsigned long long ULONGLONG;
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

#ifdef __cplusplus
}
#endif

#endif //__OS_DEFS_H__
