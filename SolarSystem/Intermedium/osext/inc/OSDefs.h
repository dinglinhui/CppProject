#ifndef OSDEFS_H_
#define OSDEFS_H_

using LONGLONG = signed long long;
using ULONGLONG = unsigned long long;
using INT16U = unsigned short;

static const auto OS_THREAD_PAUSE = 1000;
static const auto OS_MONITOR_THREAD_PAUSE = OS_THREAD_PAUSE * 2;
static const auto OS_MAX_TASK_COUNT = 100;
static const auto OS_MAX_OBJECT_NUM = 10;
static const auto OS_MAX_MESSAGE_NUM = 10;

enum dimension {
	one_dimension = 1,
	two_dimension,
	three_dimension
};

enum class OSRet {
	OK,
	ERROR,
	NULLPTR,
	THREAD_OK,
	THREAD_ERROR
};

enum class TStat {
	NoStart,
	Starting,
	Started,
	Running,
	Stopping,
	Stopped
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
	CIL_TIMEOUT,
	GM_STEP,
	GM_RUN,
	GM_SUCCESS,
	GM_FINISH
};

typedef enum {
	GM_OK = 0x01,
	GM_ERROR = 0x02,
	GM_ITEM = 0x10,
	GM_POINT = 0x20,
	GM_COMPLETE = 0x30,
	GM_EXCEPTION = 0x42
} GMRET;

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
	PCMD_REQUEST = 1,
	PCMD_REQUESTANY = 2,
	PCMD_RESPOND = 3,
	PCMD_BROADCAST = 4
} PCMD_TYPE;

typedef enum tagCOMEVENT {
	COM_BAD = 0xFF,
	COM_OVERFLOW = 0x01,
	COM_RX = 0x02,
	COM_TX = 0x03,
	COM_PARITYERR = 0x04,
	COM_NOOPEN = 0x05,
	COM_TIMEOUT = 0x06,
	COM_OVERTIME = 0x07,
	COM_UNKNOW = 0x08
} ComEvent;

typedef enum tagCOMPORT {
	COM_0 = 0,
	COM_1 = 1,
	COM_2 = 2,
	COM_3 = 3,
	COM_4 = 4,
	COM_5 = 5,
	COM_6 = 6,
	COM_7 = 7,
	COM_8 = 8,
	COM_NONE = 0xFF
} ComPort;

typedef enum tagCOMBAUD {
	CB_110 = 110,
	CB_300 = 300,
	CB_600 = 600,
	CB_1200 = 1200,
	CB_2400 = 2400,
	CB_4800 = 4800,
	CB_9600 = 9600,
	CB_14400 = 14400,
	CB_19200 = 19200,
	CB_38400 = 38400,
	CB_56000 = 56000,
	CB_57600 = 57600,
	CB_115200 = 115200,
	CB_128000 = 128000,
	CB_256000 = 256000
} ComBaud;

typedef enum tagCOMDATABIT {
	DB_5 = 0,
	DB_6 = 1,
	DB_7 = 2,
	DB_8 = 3
} ComDataBit;

typedef enum tagCOMPARITY {
	CP_NONE = 0x00,
	CP_ODD = 0x08,
	CP_EVEN = 0x18,
	CP_MARK = 0x28,
	CP_SPACE = 0x38
} ComParity;

typedef enum tagCOMSTOPBIT {

	CS_ONE = 0,
//	CS_ONE5 		= 2,
	CS_TWO = 4
} ComStopbit;

typedef struct tagCOMDCB {
	ComBaud nBaud;
	ComDataBit databit;
	ComParity parity;
	ComStopbit stopbit;
} ComDcb;

///////////////////////////////////////////////////////////////////////////////
// Constant definitions
///////////////////////////////////////////////////////////////////////////////

typedef enum {
	PRIO_DISPATCHER = 0,	// System dispatcher
	PRIO_RESERVED,    // 保留
	PRIO_DIMONITOR,    // I/O
	PRIO_RUN,	// dispatcher message
	PRIO_PULSE,	// 脉冲采集
	PRIO_GPRS_PORT,	// GPRS 串口收发管理
	PRIO_ATT7022,	// 交流采样							//
	PRIO_THDPOOL,	// CIL thread pool
	PRIO_THDPOOL2,
	PRIO_UPDATE,	// update
	PRIO_CILSERVICE,	// Communication interface layer
	PRIO_CILDSP,   // Dispatcher cil packet
	PRIO_SCHEDULE,   // schedule
	PRIO_FKDBT,   // 数据库更新触发器
	PRIO_GPRS,	// GPRS 调度管理
	PRIO_GMET_L1,	// Gather meter on RS-4851
	PRIO_GMET_L2,	// Gahter meter on RS-4852
	PRIO_EVENTMGR,   // 事件管理
	//PRIO_UPLINK         ,	// 级联任务
	PRIO_TASKMGR,	// 任务管理
	PRIO_SYSTEMHELPER,	// System helper
	PRIO_GUI,	// GUI
	PRIO_LOCAL_UPDATE,	// 本地USB升级
	PRIO_DATAPROCESSOR,	// 数据处理
	PRIO_ENET_SCHED,   // 以太网调度
	PRIO_TCPSERVER,   // 以太网TCPSERVER
	PRIO_TCPCLIENT,   // 以太网TCPCLIENT
	PRIO_UDPSERVER,   // 以太网UDPSERVER
	PRIO_UDPCLIENT,   // 以太网UDPCLIENT
	PRIO_FTPCLIENT,	// ftp客户端
	PRIO_PLCDEV, 	// 载波抄表设备
	PRIO_PLCGM,	// 载波抄表调度
	PRIO_DEBUG,   // 系统调试
	PRIO_WATCHDOG,	// 看门狗
					//

	THREAD_NUMS			 	// 线程总数
} THREAD_PRIO;

typedef enum {
	FKCH_LOCAL = 0,		// 本地/红外端口
	FKCH_REMOTE = 1,		// 远程端口
	FKCH_GMET_A = 2,		// 第一路RS-485
	FKCH_GMET_B = 3,		// 第二路RS-485
	FKCH_GMET_C = 4			// 电流环接口
} FK_CHANNEL;

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

#define NL_FK_MAX_485_MEASURED_POINT (25)			// 二个485口（12*2）+ 1个交采

#define NL_FK_MAX_PLC_MEASURED_POINT (1024)			// 载波表总数量
#define NL_FK_MAX_PLC_MULTI_MEASURED_POINT	(1024)	//载波表中的最大三相表数量
#define NL_FK_MAX_PLC_SINGLE_MEASURED_POINT (1024)	// 载波表中最大单相表数量，可以全部是单相表

#define NL_FK_MAX_SUIT				(4)		// 最大总加组数量
#define NL_FK_MAX_DIFFER			(4)		// 最大有功总电能差动越限组
#define NL_FK_MAX_TARIFF			(4)		// 最大费率个数
#define NL_FK_MAX_CURVE				(31)	// 曲线冻结数据保点数(天)
#define NL_FK_MAX_DAY				(31)	// 日冻结数据保存点数
#define NL_FK_MAX_GMET				(1)		// 抄表日冻结数据保存点数
#define NL_FK_MAX_MON				(12)	// 月冻结数据保存点数
#define NL_FK_MAX_EVENT				(5)		// 最大事件存储数量
#define NL_FK_MAX_DTCHG				(32)
#define NK_FK_MAX_CNINFO			(10)	// 中文信息条数
#define NL_FK_MAX_TASKS				(16)	// 最大任务数量
#define NL_FK_MAX_TASK_DTS			(32)	// 任务支持的最大DT数量

#define NL_FK_MAX_RETRANS			(8)		// 允许最大中继转发数量(终端上行中继)

#define NL_FK_MAX_YX				(2)		// 最大输入开关量数
#define NL_FK_MAX_YK				(4)		// 最大输出开关量数
#define NL_FK_MAX_ML				(1)		// 最大模拟量数
#define NL_FK_MAX_MC				(4)		// 最大脉冲量数
#define NL_FK_PARA_MAX_COM			(31)	// 通讯端口数

#define NL_FK_PARA_MAX_IMPORTANT	(20)	// 最大重点用户

#ifndef NL_JZ_MAX_MEASURED_POINT
#define NL_JZ_MAX_MEASURED_POINT  (NL_FK_MAX_485_MEASURED_POINT+NL_FK_MAX_PLC_MEASURED_POINT)
#endif

#define NL_FK_MAX_PARAM_MEASURED_POINT (NL_JZ_MAX_MEASURED_POINT)	//参数测量点类型

//#ifndef NL_JZ_MAX_04F10LSBUF_POINT
#define NL_JZ_MAX_04F10LSBUF_POINT  (36)	//一次主站最多设置的电表数量
//#endif

//最大存储点数
#ifndef NL_FK_MAX_PERIOD_POINT
#define NL_FK_MAX_PERIOD_POINT 		(96)
#endif

//最大存储点数
#ifndef NL_FK_MAX_CURVE_MEASURE_POINT
#define NL_FK_MAX_CURVE_MEASURE_POINT 	(32)
#endif

#define NL_FK_MAX_BIG_KIND			(7)		//最大用户大类号，包括默认
#define NL_FK_MAX_SMALL_KIND		(16)	//最大用户小类号--当前只有单相载波表和三相载波表

#define NL_FK_MAX_DT_ITEM			(31)	//信息组最大数

#define NL_FK_MAX_ROUTE				(7)	//最大中继路由数量（载波中继）
#define NL_FK_MAX_ROUTE_SERIES		(7)	//最大中间路由级数

//默认参数
#define POWER_METER_PORT_ID 		(1)
#define PLC_METER_PORT_ID 			(31)

//电表冻结
#define CONGEAL_MAX_ASSUMPSIT 			12	//定时冻结
#define CONGEAL_MAX_INSTANTANEOUS 		3	//瞬时冻结
#define CONGEAL_MAX_TIMEZONECHANGE 		2	//时区切换
#define CONGEAL_MAX_PERIODCHANGE 		2	//时段切换
#define CONGEAL_MAX_DAY 				31	//日冻结

#endif //__OS_DEFS_H__
