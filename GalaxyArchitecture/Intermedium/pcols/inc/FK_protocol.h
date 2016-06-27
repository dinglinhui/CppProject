#ifndef FK_PROTOCOL_H__
#define FK_PROTOCOL_H__

#include "FA_protocol.h"
#include "EM_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

//规约版本号：05---2005年规约（Q/GDW130-2005）
#ifndef FK_VER
#define FK_VER	0x50
#endif // FK_VER

//通道类型
#define	FK_CHANNEL_UNKOWN			0		//未知
#define	FK_CHANNEL_RADIO			1		//数传电台
#define	FK_CHANNEL_GPRS				2		//公网GPRS
#define	FK_CHANNEL_CDMA				3		//公网CDMA
#define	FK_CHANNEL_SMS				4		//公网短信
#define	FK_CHANNEL_CSD				5		//公网CSD
#define	FK_CHANNEL_Ird				6		//红外
#define	FK_CHANNEL_RS232			7		//RS-232

/******************* 预定义(根据硬件)参数 ********************************/
//#include "_DEFAULT_PROVITE.h"
/******************* 预定义(根据硬件)参数结束 **************************/

typedef enum tag_FKGM_PROTOCOL {
	GP_RELAY = 0xFF,		// 中继
	GP_T645 = 0x01,		// DL/T 645-1997
	GP_METER = 0x02,		// 交流采样
	GP_WEISHENG = 0x03,		// 威胜
	GP_T645_11 = 11, // DT/T 645-1997 峰平谷尖
	GP_T645_12 = 12, // DT/T 645-1997 2费率表
	GP_T645SINGLE = 0x13, //单条抄国标
//	GP_LANDIS_D = 0x36,		// Landis ZD
//	GP_LANDIS_B = 0x46,		// Landis ZB
//	GP_EDMI	 	= 0x32,		// EDMI
//	GP_EDMI_MK3 = 0x44,		// EDMI MK3
//	GP_ELSTER	= 0x55		// Elster

	//用国标软件测试时用36；用主台软件时，用4，

	GP_LANDIS_D33 = 4,		// Landis ZD  4

	GP_LANDIS_D34 = 5,		// Landis ZB  5
	GP_LANDIS_B33 = 6,		// Landis ZB  6
	GP_LANDIS_B34 = 7,		// Landis ZB  7

	//用国标软件测试时用42；用主台软件时，用8，
	GP_ELSTER1 = 8,		// Elster 方表8
	GP_ELSTER2 = 9,	 	// Elster 园表9
	GP_EDMI = 44,		// EDMI

	GP_DLT645_2007 = 30,
	GP_PLC_Meter = 31,		// PLC Meter
	GP_ZJ = 0x20		// 浙江97规约
//GP_EDMI_MK3 = 10,		// EDMI MK3
} FKGM_PROTOCOL;

typedef enum tag_FKGM_BIG_TYPE {
	GM_BIG_TRANSFORMER = 1,	// 大型专变用户
	GM_SMALL_TRANSFORM = 2,	// 小型专变用户
	GM_SMALL_FACTORY = 3,	// 低压三相工商用户
	GM_SINGLE_FACTORY = 4,	// 低压单相工商用户
	GM_DWELLER = 5,	// 居民用户
	GM_PUBLIC_TRANSFORM = 6	// 公变考核计量点
} FKGM_BIG_TYPE;

typedef enum tag_FKGM_SMALL_TYPE {
	GM_MAX_METER = 0,	//单相表
	GM_SINGLE_METER = 1,	//单相表
	GM_MUL_METER = 2	// 三相表
} FKGM_SMALL_TYPE;

typedef enum tag_MEnergyType {
	ForwardActive = 0,
	ReverseActive = 1,
	ForwardReactive = 2,
	ReverseReactive = 3
} MEnergyType;

typedef enum tag_DataType {
	DT_HOUR = 1,
	DT_CURVE = 2,
	DT_DAY = 3,
	DT_MONTH = 4,
	DT_GMETDAY = 5
} DataType;

typedef enum tag_SourceType {
	ST_RS485 = 1,
	ST_PULSE = 2,
	ST_LOCAL = 4,
	ST_COMBIN = 8,
	ST_RSSINGLE = 16,
	ST_CURVE = 32
} SourceType;

typedef enum tag_CongealType {
	CT_REALTIME = 1,	// 实时冻结
	CT_HOUR = 2,	// 小时冻结
	CT_CURVE = 3,	// 曲线冻结
	CT_DAY = 4,	// 日冻结
	CT_MONTH = 5,	// 月冻结
	CT_GMETDAY = 6,	// 抄表日冻结
	CT_MONTH_G = 7		// 增补月冻结采用电表月数据
} CongealType;
///////////////////////////////////////////////////////////////////////////////
// MetaData helper method
typedef struct tag_MetaData {
	WORD FN;		//功能号
	DataType DT;		//数据类型
	SourceType ST;		//数据源类型
	int nDataLen;		//数据长度
} MetaData;		//数据元

typedef struct tag_ShiftData {
	const MetaData* lpMeta;
	BYTE* lpBuf;
	int nSize;
	BYTE PN;
	BYTE nDensity;
} ShiftData;

///////////////////////////////////////////////////////////////////////////////
// MetaClass helper method
typedef struct tag_MetaClass {
	DataType DT;
	WORD wCount;
	const MetaData* MD;
	WORD wSaveNums;
} MetaClass;

///////////////////////////////////////////////////////////////////////////////
// MetaGroup helper method
typedef struct tag_MetaGroup {
	SourceType ST;
	WORD nMaxPN;
	WORD wCount;
	const MetaClass* lpClass;
} MetaGroup;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 数据格式定义
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//data format defined
typedef struct tag_FK_FORMAT {
	BYTE nItem;
	const char* Name;
} FK_FORMAT;

//
static const FK_FORMAT FK_FORMAT_DEFINE[] = {
//nItem,	Nmae
		{ 0, nullptr }, { 1, nullptr }, { 2, nullptr }, { 3, nullptr }, { 4, nullptr }, { 5, nullptr }, { 6, nullptr }, { 7, nullptr }, { 8, nullptr }, { 9, nullptr }, { 10, nullptr }, { 11, nullptr }, { 12, nullptr }, { 13, nullptr }, { 14,
		nullptr }, { 15, nullptr }, { 16, nullptr }, { 17, nullptr }, { 18, nullptr }, { 19, nullptr }, { 20, nullptr }, { 21, nullptr }, { 22, nullptr }, { 23, nullptr }, };

//数据格式01
typedef struct tag_FK_DATAMASK01 {
	BYTE nSec;
	BYTE nMin;
	BYTE nHour;
	BYTE nDay;
	union {
		BYTE body;
		struct {
			BYTE nMonth :5;
			BYTE nWeek :3;
		} mask;
	} nMonWeek;
	BYTE nYear;
} FK_DATAMASK01;

//数据格式02
typedef struct tag_FK_DATAMASK02 {
	double dbVal;
} FK_DATAMASK02;

//数据格式03
typedef struct tag_FK_DATAMASK03 {
	double dbVal;
} FK_DATAMASK03;

//数据格式04
typedef struct tag_FK_DATAMASK04 {
	BYTE S0;
	BYTE nVal;
} FK_DATAMASK04;

//数据格式05
typedef struct tag_FK_DATAMASK05 {
	short sVal;
} FK_DATAMASK05;

//数据格式06
typedef struct tag_FK_DATAMASK06 {
	short sVal;
} FK_DATAMASK06;

//数据格式07
typedef struct tag_FK_DATAMASK07 {
	WORD wVal;
} FK_DATAMASK07;

//数据格式08
typedef struct tag_FK_DATAMASK08 {
	WORD wVal;
} FK_DATAMASK08;

//数据格式09
typedef struct tag_FK_DATAMASK09 {
	int nVal;
} FK_DATAMASK09;

//数据格式10
typedef struct tag_FK_DATAMASK10 {
	DWORD dwVal;
} FK_DATAMASK10;

//数据格式11
typedef struct tag_FK_DATAMASK11 {
	DWORD dwVal;
} FK_DATAMASK11;

//数据格式12
typedef struct tag_FK_DATAMASK12 {
	unsigned long long llVal;
} FK_DATAMASK12;

//数据格式13
typedef struct tag_FK_DATAMASK13 {
	DWORD dwVal;
} FK_DATAMASK13;

//数据格式14
typedef struct tag_FK_DATAMASK14 {
	unsigned long long llVal;
} FK_DATAMASK14;

//数据格式15
typedef struct tag_FK_DATAMASK15 {
	BYTE nMin;
	BYTE nHour;
	BYTE nDay;
	BYTE nMonth;
	BYTE nYear;
} FK_DATAMASK15;

//数据格式16
typedef struct tag_FK_DATAMASK16 {
	BYTE nSec;
	BYTE nMin;
	BYTE nHour;
	BYTE nDay;
} FK_DATAMASK16;

//数据格式17
typedef struct tag_FK_DATAMASK17 {
	BYTE nMonth;
	BYTE nDay;
	BYTE nHour;
	BYTE nMin;
} FK_DATAMASK17;

//数据格式18
typedef struct tag_FK_DATAMASK18 {
	BYTE nDay;
	BYTE nHour;
	BYTE nMin;
} FK_DATAMASK18;

//数据格式19
typedef struct tag_FK_DATAMASK19 {
	BYTE nMin;
	BYTE nHour;
} FK_DATAMASK19;

//数据格式20
typedef struct tag_FK_DATAMASK20 {
	BYTE nDay;
	BYTE nMonth;
	BYTE nYear;
} FK_DATAMASK20;

//数据格式21
typedef struct tag_FK_DATAMASK21 {
	BYTE nMonth;
	BYTE nYear;
} FK_DATAMASK21;

//数据格式22
typedef struct tag_FK_DATAMASK22 {
	BYTE nVal;
} FK_DATAMASK22;

//数据格式23
typedef struct tag_FK_DATAMASK23 {
	DWORD dwVal;
} FK_DATAMASK23;
//数据格式24
typedef struct tag_FK_DATAMASK24 {
	BYTE hour;
	BYTE day;
} FK_DATAMASK24;

//数据格式25
typedef struct tag_FK_DATAMASK25 {
	double dbVal;
} FK_DATAMASK25;

//数据格式26
typedef struct tag_FK_DATAMASK26 {
	WORD wVal;
} FK_DATAMASK26;

//数据格式27
typedef struct tag_FK_DATAMASK27 {
	DWORD dbVal;
} FK_DATAMASK27;

// 小时冻结时标
typedef struct tag_FK_TDH {
	BYTE nHour;
	BYTE nDensity;
} FK_TDH;

// 曲线冻结时标
typedef struct tag_FK_TDC {
	FK_DATAMASK15 date;
	BYTE nDensity;
	BYTE nCount;
} FK_TDC;

// 日冻结时标
typedef struct tag_FK_TDD {
	FK_DATAMASK20 date;
} FK_TDD;

// 月冻结时标
typedef struct tag_FK_TDM {
	FK_DATAMASK21 date;
} FK_TDM;

// 终端地址
typedef struct tag_FK_ADDRESS {
	WORD A1;
	WORD A2;
	BYTE A3;
} FK_ADDRESS;

//控制码C
typedef union tag_FK_C {
	BYTE body;
	struct {
		BYTE cmd :4;				//功能码
		BYTE FCV :1;				//帧记数有效
		BYTE FCB_ACD :1;				//帧记数，要求访问
		BYTE PRM :1;				//启动标志
		BYTE DIR :1;				//传输方向
	} body_mask;
} FK_C;

// 帧序列域
typedef union tag_FK_SEQ {
	BYTE body;
	struct {
		BYTE PRSEQ :4;						// 启动帧序号/响应帧序号
		BYTE CON :1;						// 请求确认
		BYTE FIN :1;						// 0: 多帧(中间帧) 	1:	多帧(结束帧)	0:	多帧(第一帧)	1: 单帧
		BYTE FIR :1;						// 0:				0:					1:					1:
		BYTE TpV :1;						// 帧时间有效位
	} body_mask;
} FK_SEQ;

//数据单元标识
typedef struct tag_FK_DADT {
	struct {
		WORD DA;
		struct {
			WORD DA1 :8;
			WORD DA2 :8;
		} body_mask;
	} DA;
	struct {
		WORD DT;
		struct {
			WORD DT1 :8;
			WORD DT2 :8;
		} body_mask;
	} DT;
} FK_DADT;

// 附加信息域
// 消息认证码
typedef struct tag_FK_AUX_PW {
	BYTE PW[16];
} FK_AUX_PW;

// 事件计数器
typedef struct tag_FK_AUX_EC {
	BYTE EC1;								// 重要事件计数器
	BYTE EC2;								// 一般事件计数器
} FK_AUX_EC;

// 时间标签
typedef struct tag_FK_AUX_Tp {
	BYTE PFC;						// 启动帧帧序号计数器
	FK_DATAMASK16 time;						// 启动帧发送时标
	BYTE delay;					// 允许传输延时
} FK_AUX_Tp;

typedef struct tag_FK_AUX {
	FK_AUX_PW PW;						// 消息认证码
	FK_AUX_EC EC;						// 事件计数器
	FK_AUX_Tp Tp;						// 时间标签
} FK_AUX;

//功能码
typedef enum tag_FK_FUNCTION {
	FK_CMD_ACK = 0x00,				//确认/否认
	FK_CMD_RESET = 0x01,				//复位
	FK_CMD_LINKSTATE = 0x02,				//链路接口检测
	FK_CMD_RELAX = 0x03,				//中继站命令
	FK_CMD_SETPARAM = 0x04,				//设置参数
	FK_CMD_CONTROL = 0x05,				//控制命令
	FK_CMD_IDENTITY = 0x06,				//身份认证及密钥协商
	FK_CMD_SLAVEREPORT = 0x08,				//请求被级联终端主动上报
	FK_CMD_ASKCONFIG = 0x09,				//请求终端配置
	//
	FK_CMD_ASKPARAM = 0x0A,				//查询参数
	FK_CMD_ASKTASK = 0x0B,				//请求任务数据
	FK_CMD_CLASS1 = 0x0C,				//请求一类数据
	FK_CMD_CLASS2 = 0x0D,				//请求二类数据
	FK_CMD_CLASS3 = 0x0E,				//请求三类数据
	//
	FK_CMD_FILE = 0x0F,				//文件传输
	FK_CMD_TRANSMIT = 0x10,				//数据转发
	//
	//自定义命令，0x81开始
	FK_CMD_UPDATEPARM = 0x81,				//参数升级
	FK_CMD_ASKJCDATA = 0x90,				//请求交采数据,新增
	FK_CMD_SETJCPARA = 0x91,				//请求或设置交采参数,新增
	FK_CMD_UPDATE = 0x99				//软件升级
} FK_FUNCTION;

//data unit defined
typedef struct tag_FK_UNIT {
	BYTE nItem;
	const char * Name;
} FK_UINT;

static const FK_UINT FK_UNIT_DEFINE[] = {
//nItem,	Nmae
		{ 0, nullptr }, { 1, "V" }, { 2, "A" }, { 3, "kV" }, { 4, "kA" }, { 5, "kW" }, { 6, "kvar" }, { 7, "kWh" }, { 8, "kvarh" }, { 9, "%" }, { 10, "分" }, { 11, "秒" }, { 12, "BIT/S" }, { 13, "KVA" }, {
				14, "度" }, { 15, "" } };

typedef struct tag_FK_DATA_DEFINE {
	DWORD dwAFNDT;						//标识编码(FK_VER + AFN + DT)
	union {
		DWORD body;						//编码范围(增量值,内部使用)
		struct {
			DWORD nTotal :8;         //总存储次数
			DWORD vValid :8;		//数据个数（循环个数）
			DWORD wStep :16;		//数据个数步长
		} body_mask;
	} nPlus;							//编码范围

	union {
		BYTE body;						//隐码
		struct {
			BYTE nValid :1;			//数据完好性标志 data packet verified（1--数据完好，0--数据破坏）
			BYTE nChange :1;			//数据改变（1--数据改变，0--数据没有改变）
			BYTE nTypeSingle :1;         //0: 三相表存储。 1：单相表存储(暂不用)
			BYTE nStoreStyle :1;         //1：分项校验存储  0：整块校验存储
			BYTE nIsCheck :1;            //1：要校验  0：不要校验(暂不用)
			BYTE nIsSort :1;             //1：要排序  0：不要排序(暂不用)
			BYTE space :2;			//备用
		} body_mask;
	} DataMask;

	BYTE nFlag;							//数据类型标志(0--终端，1-测量点，2-直流模拟量点，3-总加点，4-控制轮次,5-任务,6-事件，7-中文信息）
	BYTE nUnit;							//单位，具体见 FK_UNIT_DEFINE 的定义
	BYTE nFormat;						//数据格式
	WORD nLength;						//数据长度(归约定义的长度)

	void *DataPtr;						//数据体
	WORD nDataLength;					//数据体长度(内部定义的数据结构长度)

	const char *Name;						//数据项名称

	DWORD dwAFNDT2;						//结束编

} FK_DATA_DEFINE, FK_DATA, *PFK_DATA;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 终端参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//组1
//F1
typedef struct tag_FK_DT04F01 {
	BYTE nRTStimes;
	BYTE nStartMins;
	WORD wOverTimes;
	BYTE nRetryCount;
	BYTE nAckFlag;						// 	D0：1类数据自动上报， 	D1：2类数据自动上报， 	D2：3类数据自动上报，
	BYTE nHearBeat;
} FK_DT04F01;

//F2
typedef struct tag_FK_DT04F02 {
	BYTE nAgree;
	BYTE nNum;
	WORD wAddr[NL_FK_MAX_RETRANS];
} FK_DT04F02;

//F3
typedef struct tag_FK_IP {
	BYTE nAddr1;
	BYTE nAddr2;
	BYTE nAddr3;
	BYTE nAddr4;
	WORD nPort;
} FK_IP;

typedef struct tag_FK_DT04F03 {
	FK_IP IPHost;
	FK_IP IPAux;
	char APN[16];
} FK_DT04F03;

//F4
typedef struct tag_FK_PhoneNumber {
	BYTE Code[8];
} FK_PhoneNumber;

typedef struct tag_FK_DT04F04 {
	FK_PhoneNumber hHost;
	FK_PhoneNumber hSmsCenter;
} FK_DT04F04;

//F5
typedef struct tag_FK_DT04F05 {
	BYTE nID;
	BYTE nParam[2];
} FK_DT04F05;

//F6
typedef struct tag_FK_DT04F06 {
	WORD nGroupAdr[8];
} FK_DT04F06;

typedef struct tag_FK_TMN_IP {
	BYTE nAddr1;
	BYTE nAddr2;
	BYTE nAddr3;
	BYTE nAddr4;
} FK_TMN_IP;

//F7
typedef struct tag_FK_DT04F07 {
	FK_IP IPDevice;						//终端IP  终端侦听端口
	FK_TMN_IP IPMask;						//子网掩码
	FK_TMN_IP IPGetway;						//网关
	BYTE AgentType;						//代理类型
	FK_IP IPAgent;						//代理服务器
	BYTE AgentConnectType;						//代理服务器连接方式
	BYTE UserNameLength;						//用户名长度
	BYTE UserName[20];						//用户名
	BYTE PasswordLength;						//密码长度
	BYTE Password[20];						//密码
} FK_DT04F07;

//F8
typedef struct tag_FK_DT04F08 {
	union {
		BYTE body;
		struct {
			BYTE nGPRS2SMS :2; // 三种在线模式1-永久在线, 2-被动激活, 3-时段在线
			BYTE space_D23 :2; // 备用
			BYTE nWorkMode :2; // 工作模式
			BYTE space_D6 :1; // 备用
			BYTE nTCPorUDP :1; // 0: TCP, 1: UDP
		} mask;
	} nGPRS;
	WORD wSecs;
	BYTE nCount;
	BYTE nMins;
	BYTE nFlag[3];
} FK_DT04F08;
//组2
//F9
typedef struct tag_FK_DT04F09 {
	BYTE nRecordFlag[8];
	BYTE nImportFlag[8];
} FK_DT04F09;

//F10
typedef struct tag_FK_METER {
	WORD nIndex;		//电能表或交流采样装置序号
	WORD nTN;			//所属的测量点号
	WORD nStoreIndex;	//数据存储在单（三）相表的序号
	BYTE nPort;		//端口号
	BYTE nBaud;		//通信速率
	BYTE nProtocol;	//通信规约类型
	BYTE addr[6];		//测量点地址
	BYTE pwd[6];		//测量点密码
	BYTE nTariffs;		//费率个数
	BYTE nDecimal;		//小数位
	BYTE nInter;		//整数位
	BYTE collAddr[6];	//所属采集器地址：如果采集器地址为0表示：无效
	BYTE nUserBigKind;	//用户大类号：
	BYTE nUserSmallKind;	//用户小类号
} FK_METER;

typedef struct tag_FK_DT04F10_LS {
	WORD nCount;
	FK_METER hMeter[NL_JZ_MAX_04F10LSBUF_POINT];
} FK_DT04F10_LS;

typedef struct tag_FK_DT04F10	//所有电表参数(485+交采+载波)
{
//	WORD 	 nCount;
	FK_METER hMeter[NL_JZ_MAX_MEASURED_POINT];
} FK_DT04F10;

//F11
typedef struct tag_FK_PULSE {
	BYTE nPort;
	BYTE nTN; 		//脉冲测量点号数值范围为0-64
	BYTE nKind;
	WORD nMK;
	WORD nStoreIndex;
} FK_PULSE;

typedef struct tag_FK_DT04F11_LS {
	BYTE nCount;
	FK_PULSE hPulse[NL_FK_MAX_MC];
} FK_DT04F11_LS;

typedef struct tag_FK_DT04F11 {
//	BYTE 	 nCount;
	FK_PULSE hPulse[NL_FK_MAX_MC];
} FK_DT04F11;

//F12
typedef struct tag_FK_DT04F12 {
	BYTE nFlag;
	BYTE nKind;
} FK_DT04F12;

//F13
typedef struct tag_FK_DMS {
	BYTE nPort;	//模拟量端口号范围为1-64
	BYTE nTN;	//模拟量测量点号数值范围为0-64
	BYTE nKind;
	WORD nStoreIndex;
} FK_DMS;

typedef struct tag_FK_DT04F13_LS {
	BYTE nCount;
	FK_DMS hDMS[NL_FK_MAX_ML];
} FK_DT04F13_LS;

typedef struct tag_FK_DT04F13 {
//	BYTE   nCount;
	FK_DMS hDMS[NL_FK_MAX_ML];
} FK_DT04F13;

//F14
typedef struct tag_FK_SUIT {
	BYTE nIndex;
	BYTE nMPs;
	union {
		BYTE body;
		struct {
			BYTE TN :6;
			BYTE direct :1;
			BYTE operat :1;
		} mask;
	} flag[64];	//每个总加组可以支持64个测量点
} FK_SUIT;

typedef struct tag_FK_DT04F14_LS {
	BYTE nCount;
	FK_SUIT hSuit[NL_FK_MAX_SUIT];
} FK_DT04F14_LS;

typedef struct tag_FK_DT04F14 {
//	BYTE    nCount;
	FK_SUIT hSuit[NL_FK_MAX_SUIT];
} FK_DT04F14;

//F15
typedef struct tag_FK_DIFFER {
	BYTE nIndex;
	BYTE nDstSN;
	BYTE nSrcSN;
	BYTE nFlag;
	BYTE nRelativeVal;
	FK_DATAMASK03 dmUtterly;
} FK_DIFFER;

typedef struct tag_FK_DT04F15_LS {
	BYTE nCount;
	FK_DIFFER hDiffer[NL_FK_MAX_DIFFER];
} FK_DT04F15_LS;

typedef struct tag_FK_DT04F15 {
//	BYTE 		nCount;
	FK_DIFFER hDiffer[NL_FK_MAX_DIFFER];
} FK_DT04F15;

//F16
typedef struct tag_FK_DT04F16 {
	char szVNetUser[32];
	char szVNetPwd[32];
} FK_DT04F16;

//组3
//F17
typedef struct tag_FK_DT04F17 {
	FK_DATAMASK02 dmValue;
} FK_DT04F17;

//F18
typedef struct tag_FK_DT04F18 {
	union {
		BYTE body;
		struct {
			BYTE ctrl1 :2;
			BYTE ctrl2 :2;
			BYTE ctrl3 :2;
			BYTE ctrl4 :2;
		} mask;
	} period[12];
} FK_DT04F18;

//F19
typedef struct tag_FK_DT04F19 {
	FK_DATAMASK04 dmValue;
} FK_DT04F19;

//F20
typedef struct tag_FK_DT04F20 {
	FK_DATAMASK04 dmValue;
} FK_DT04F20;

//F21
typedef struct tag_FK_DT04F21 {
	BYTE period[48];
	BYTE nTOUs;
} FK_DT04F21;

//F22
typedef struct tag_FK_DT04F22 {
	BYTE nTOUs;
	FK_DATAMASK03 dmTOU[14];
} FK_DT04F22;

//F23
typedef struct tag_FK_DT04F23 {
	BYTE nWarnFlag[3];
} FK_DT04F23;

//组4
//F25
typedef struct tag_FK_DT04F25 {
	WORD nPT;
	WORD nCT;
	FK_DATAMASK07 dmVoltage;
	FK_DATAMASK22 dmCurrent;
	FK_DATAMASK23 dmLoad;
	BYTE nConnect;
} FK_DT04F25;

typedef struct tag_FK_EventTime {
	BYTE OverLimitTime;	//越限持续时间
	FK_DATAMASK05 OverLimitComeBackCoefficient;	//越限恢复系数
} FK_EventTime;
//F26
typedef struct tag_FK_DT04F26 {
	FK_DATAMASK07 ulVolt;		// 电压合格上限
	FK_DATAMASK07 llVolt;		// 电压合格下限
	FK_DATAMASK07 blVolt;		// 电压断相门限

	FK_DATAMASK07 uulVolt;		// 电压上上 限澹(过压门限)
	FK_EventTime uulVoltLimitValue;		//y越限持续时间和恢复系数

	FK_DATAMASK07 lllVolt;		// 电压下下限
	FK_EventTime lllVoltLimitValue;		//y越限持续时间和恢复系数

	FK_DATAMASK25 uulICurr;		// 相电流上上限
	FK_EventTime uulICurrLimitValue;		//y越限持续时间和恢复系?

	FK_DATAMASK25 ulICurr;		// 相电流上限
	FK_EventTime ulICurrLimitValue;		//y越限持续时间和恢复系数

	FK_DATAMASK25 ul0Curr;		// 零序电流上限
	FK_EventTime ul0CurrLimitValue;		//y越限持续时间和恢复系?

	FK_DATAMASK23 uulPower;		// 视在功率上上限
	FK_EventTime uulPowerLimitValue;		//y越限持续时间和恢复系?

	FK_DATAMASK23 ulPower;		// 视在功率上限
	FK_EventTime ulPowerLimitValue;		//y越限持续时间和恢复系数

	FK_DATAMASK05 lVoltImb;		// 三相电压不平衡限值
	FK_EventTime lVoltImbLimitValue;		//y越限持续时间和恢复系数

	FK_DATAMASK05 lCurrImb;		// 三相电流不平衡限值
	FK_EventTime lCurrImbLimitValue;		//y越限持续时间和恢复系数

	BYTE nTimes;		// 连续失压时间限值
} FK_DT04F26;

//F27
typedef struct tag_FK_DT04F27 {
	FK_DATAMASK26 PhaseRA[3];		//A、B、C相电阻RA
	FK_DATAMASK26 PhaseXA[3];		//A、B、C相电抗XA
	FK_DATAMASK26 PhaseGA[3];		//A、B、C相电导GA
	FK_DATAMASK26 PhaseBA[3];		//A、B、C相电纳BA
} FK_DT04F27;

//F28
typedef struct tag_FK_DT04F28 {
	FK_DATAMASK05 limit1;
	FK_DATAMASK05 limit2;
} FK_DT04F28;
//F29
typedef struct tag_FK_DT04F29 {
	char meternum[12];		//当地电能表显示号
} FK_DT04F29;
//F30
typedef struct tag_FK_DT04F30 {
	BYTE nCollectSwitch;		//台区集中抄表停抄/投抄设置
} FK_DT04F30;

//F31
typedef struct tag_FK_DT04F31 {
	BYTE AppertainCount;		//设置的载波从节点附属节点地址个数 n（n≤20）
	BYTE AppertainAddress[20][6];		//载波从节点附属节点地址
} FK_DT04F31;

//组5
//F33
typedef struct tag_FK_PORT_WORK_MODE {
	BYTE nComPort;		//终端通信端口号
	WORD wCtrlByte;		//台区集中抄表运行控制字
	BYTE day[4];		//抄表日-日期
	FK_DATAMASK19 dmTime;		//抄表日-时间
	BYTE GMMins;		//抄表间隔时间
	FK_DATAMASK18 CheckTime;		//对电表广播校时定时时间
	BYTE primeNum;		//允许抄表时段数m（0≤m≤24）
	FK_DATAMASK19 GMPeriod[25][2];		//第1......M个允许抄表时段开始时间 第1......M个允许抄表时段结束时间
} FK_PORT_WORK_MODE;

typedef struct tag_FK_DT04F33_LS {
	BYTE nCount;
	FK_PORT_WORK_MODE hPortMode[NL_FK_PARA_MAX_COM];		//需要更改端口总数
} FK_DT04F33_LS;

typedef struct tag_FK_DT04F33 {
	FK_PORT_WORK_MODE hPortMode[NL_FK_PARA_MAX_COM];		//需要更改端口总数
} FK_DT04F33;
//F34
typedef struct tag_FK_PORT_MODE {
	BYTE Port;		//终端通信端口号
	BYTE PortDCB;		//与终端接口端的通信控制字
	DWORD PortBPS;		//与终端接口对应端的通信速率（bps）
} FK_PORT_MODE;

typedef struct tag_FK_DT04F34_LS {
	BYTE nCount;
	FK_PORT_MODE hDevicePara[NL_FK_PARA_MAX_COM];		//需要更改端口总数
} FK_DT04F34_LS;

typedef struct tag_FK_DT04F34 {
	FK_PORT_MODE hDevicePara[NL_FK_PARA_MAX_COM];		//需要更改端口总数
} FK_DT04F34;

//F35
typedef struct tag_FK_DT04F35 {
	BYTE ImCount;
	WORD ImporIndex[NL_FK_PARA_MAX_IMPORTANT];		//台区集中抄表重点户设置
} FK_DT04F35;
//F36
typedef struct tag_FK_DT04F36 {
	DWORD dwMaxStream;		//月通信流量门限
} FK_DT04F36;
//F37 //终端级联通信参数

#define CASCADED_MAX_TMN	4

typedef struct tag_FK_TMNADDR {
	BYTE bAddr1[2];		//被级联的终端行政区划码/级联方的终端行政区划码
	WORD bAddr2;		//被级联的终端地址/级联方的终端地址

} FK_TMNADDR;

typedef struct tag_FK_DT04F37 {
	BYTE bCom;		//终端级联通信端口号
	BYTE bCtrlbyte;		//终端级联通信控制字
	BYTE bRecvFrameOverTime;		//接收等待报文超时时间
	BYTE bRecvByteOverTime;		//接收等待字节超时时间
	BYTE bReSendTimes;		//级联方（主动站）接收失败重发次数
	BYTE bAskPeriod;		//级联巡测周期
	BYTE bCascadedNum;		//级联/被级联标志、及其对应的被级联/级联的终端个数n
	FK_TMNADDR bCascadedAddr[CASCADED_MAX_TMN];		//级联终端行政区码和地址
} FK_DT04F37;

typedef struct tag_FK_ITEM_META {
	BYTE nUserSmallKind;				//用户小类号
	BYTE nDTCount;						//设置的信息类数
	BYTE DTFlag[NL_FK_MAX_DT_ITEM];		// 0~NL_FK_MAX_DT_ITEM 对应的信息组
} FK_ITEM_META;

typedef struct tag_FK_ITEM_CONFIG {
	BYTE nUserBigKind;		//本次设置所对应的用户大类号
	BYTE nSuitCount;			//本次设置的组数
	FK_ITEM_META hItemMeta[NL_FK_MAX_SMALL_KIND];			//
} FK_ITEM_CONFIG;

//F38 1类数据配置设置
typedef struct tag_FK_DT04F38 {
	FK_ITEM_CONFIG hItemConfig[NL_FK_MAX_BIG_KIND];
} FK_DT04F38;

//F39 2类数据配置设置
typedef struct tag_FK_DT04F39 {
	FK_ITEM_CONFIG hItemConfig[NL_FK_MAX_BIG_KIND];
} FK_DT04F39;

//组6
//F41
typedef struct tag_FK_SCHEME {
	BYTE nIndex;
	FK_DATAMASK02 param[8];
} FK_SCHEME;

typedef struct tag_FK_DT04F41 {
	BYTE nFlag;
	FK_SCHEME scheme[3];
} FK_DT04F41;

//F42
typedef struct tag_FK_DT04F42 {
	FK_DATAMASK02 value;
	FK_DATAMASK19 time;
	BYTE delay;
	union {
		BYTE body;
		struct {
			BYTE space :1;
			BYTE Mon :1;
			BYTE Tues :1;
			BYTE Wed :1;
			BYTE Thurs :1;
			BYTE Friday :1;
			BYTE Sat :1;
			BYTE Sun :1;
		} mask;
	} day;
} FK_DT04F42;

//F43
typedef struct tag_FK_DT04F43 {
	BYTE nMins;
} FK_DT04F43;

//F44
typedef struct tag_FK_DT04F44 {
	FK_DATAMASK20 tmStart;
	FK_DATAMASK20 tmEnd;
	FK_DATAMASK02 value;
} FK_DT04F44;

//F45
typedef struct tag_FK_DT04F45 {
	BYTE nFlag;
} FK_DT04F45;

//F46
typedef struct tag_FK_DT04F46 {
	FK_DATAMASK03 value;
} FK_DT04F46;

//F47
typedef struct tag_FK_DT04F47 {
	DWORD dwSNO;
	BYTE nFlag;
	FK_DATAMASK03 value;
	FK_DATAMASK03 lWarn;
	FK_DATAMASK03 lControl;
} FK_DT04F47;

//F48
typedef struct tag_FK_DT04F48 {
	BYTE nFlag;
} FK_DT04F48;

//组7
//F49
typedef struct tag_FK_DT04F49 {
	BYTE nMins;
} FK_DT04F49;

//组8
//F57
typedef struct tag_FK_DT04F57 {
	BYTE nWarnFlag[3];
} FK_DT04F57;

//F58
typedef struct tag_FK_DT04F58 {
	BYTE nHours;
} FK_DT04F58;

//F59
typedef struct tag_FK_DT04F59 {
	FK_DATAMASK22 nMTCC;
	FK_DATAMASK22 nMTFly;
	BYTE nMTStop;
	BYTE nMTChkTime;
} FK_DT04F59;

//F60
typedef struct tag_FK_DT04F60 {
	FK_DATAMASK05 lvSumVolt;
	FK_DATAMASK05 lvOddVolt;
	FK_DATAMASK05 lvEvenVolt;
	FK_DATAMASK05 lv2Volt;
	FK_DATAMASK05 lv4Volt;
	FK_DATAMASK05 lv6Volt;
	FK_DATAMASK05 lv8Volt;
	FK_DATAMASK05 lv10Volt;
	FK_DATAMASK05 lv12Volt;
	FK_DATAMASK05 lv14Volt;
	FK_DATAMASK05 lv16Volt;
	FK_DATAMASK05 lv18Volt;
	FK_DATAMASK05 lv3Volt;
	FK_DATAMASK05 lv5Volt;
	FK_DATAMASK05 lv7Volt;
	FK_DATAMASK05 lv9Volt;
	FK_DATAMASK05 lv11Volt;
	FK_DATAMASK05 lv13Volt;
	FK_DATAMASK05 lv15Volt;
	FK_DATAMASK05 lv17Volt;
	FK_DATAMASK05 lv19Volt;
	FK_DATAMASK06 lvSumCurr;
	FK_DATAMASK06 lv2Curr;
	FK_DATAMASK06 lv4Curr;
	FK_DATAMASK06 lv6Curr;
	FK_DATAMASK06 lv8Curr;
	FK_DATAMASK06 lv10Curr;
	FK_DATAMASK06 lv12Curr;
	FK_DATAMASK06 lv14Curr;
	FK_DATAMASK06 lv16Curr;
	FK_DATAMASK06 lv18Curr;
	FK_DATAMASK06 lv3Curr;
	FK_DATAMASK06 lv5Curr;
	FK_DATAMASK06 lv7Curr;
	FK_DATAMASK06 lv9Curr;
	FK_DATAMASK06 lv11Curr;
	FK_DATAMASK06 lv13Curr;
	FK_DATAMASK06 lv15Curr;
	FK_DATAMASK06 lv17Curr;
	FK_DATAMASK06 lv19Curr;
} FK_DT04F60;

//F61
typedef struct tag_FK_DT04F61 {
	BYTE nFlag;
} FK_DT04F61;

//组9
//F65
typedef struct tag_FK_DT04F65 {
	BYTE nCycle;
	FK_DATAMASK01 tmBasic;
	BYTE nRS;
	BYTE nCount;
	DWORD nDT[NL_FK_MAX_TASK_DTS];
} FK_DT04F65;

//F66
typedef struct tag_FK_DT04F66 {
	BYTE nCycle;
	FK_DATAMASK01 tmBasic;
	BYTE nRS;
	BYTE nCount;
	DWORD nDT[NL_FK_MAX_TASK_DTS];			//数据项个数
} FK_DT04F66;

//F67
typedef struct tag_FK_DT04F67 {
	BYTE nFlag;
} FK_DT04F67;

//F68
typedef struct tag_FK_DT04F68 {
	BYTE nFlag;
} FK_DT04F68;

//组10
//F73
typedef struct tag_FK_CAPACITOR {
	BYTE nMode;
	FK_DATAMASK02 capacity;
} FK_CAPACITOR;

typedef struct tag_FK_DT04F73 {
	FK_CAPACITOR capacitor[16];
} FK_DT04F73;

//F74
typedef struct tag_FK_DT04F74 {
	FK_DATAMASK05 nFunction;
	FK_DATAMASK23 lvAddPower;
	FK_DATAMASK23 lvRemovePower;
	BYTE ntmDelay;
	BYTE ntmSpan;
} FK_DT04F74;

//F75
typedef struct tag_FK_DT04F75 {
	FK_DATAMASK07 ulVolt;
	FK_DATAMASK07 ublVolt;
	FK_DATAMASK07 llVolt;
	FK_DATAMASK07 lblVolt;
	FK_DATAMASK05 ulJCurr;
	FK_DATAMASK05 ublJCurr;
	FK_DATAMASK05 ulJVolt;
	FK_DATAMASK05 ubljVolt;
} FK_DT04F75;

//F76
typedef struct tag_FK_DT04F76 {
	BYTE nControl;
} FK_DT04F76;

//组11
//F81
typedef struct tag_FK_DT04F81 {
	FK_DATAMASK02 nDMSStart;
	FK_DATAMASK02 nDMSEnd;
} FK_DT04F81;

//F82
typedef struct tag_FK_DT04F82 {
	FK_DATAMASK02 ulDMS;
	FK_DATAMASK02 llDMS;
} FK_DT04F82;

//F83
typedef struct tag_FK_DT04F83 {
	BYTE nDensity;
} FK_DT04F83;

// 江苏增补部分

typedef struct tag_FK_DT04F85 {
	WORD A1;
	WORD A2;
} FK_DT04F85;			//低压集抄中主站设置地址

//以上为规约中基本定义

typedef struct tag_FK_DT04F92 {
	char strOffice[12];
	BYTE nFeeType;
} FK_DT04F92;

//终端密码
typedef struct tag_FK_DT04F95 {
	DWORD wPWD;
} FK_DT04F95;

typedef struct tag_FK_DT04F96 {
	BYTE b4851Act;			//0-抄表口，1-被抄
	BYTE b4852Act;			//0-负控规约，1-被抄，2-级联
} FK_DT04F96;

typedef struct tag_FK_DT04F97 {
	BYTE bMstSlv; //0-主终端，1-从终端
} FK_DT04F97;

//针对电表特别扩充的数据项，为了应用支持不同的测量点数量，该值固定，取比较大的值
typedef struct tag_FK_DT04F100 {
	BYTE Offset485Phase[8];			//最多64块485电表（包括交采），按位表示

	//下面两个参数仅针对载波表
	BYTE OffsetMultiPhase[128];		//最多128*8 = 1024个三相表
	BYTE OffsetSinglePhase[128];	//最多128*8 = 1024个单相表
} FK_DT04F100;

//载波调试信息
typedef struct tag_FK_DT04F101 {
	DWORD RouterInfo;
} FK_DT04F101;

/*	参数: F169:终端上行通信信道设置
 数据内容	      数据格式	单位	字节数
 上行通信信道内容	BIN		        1
 其中，上行通信信道类型取值如下：
 0	RS232串口
 2	GPRS/CDMA
 3	以太网
 4	短信
 5	红外
 10	PSTN*/
typedef struct tag_FK_DT04F169 {
	BYTE nChannel;
} FK_DT04F169;

/*
 参数: F170 载波方案号
 数据内容	数据格式	单位	字节数
 载波方案号	BIN		            1
 载波方案号如下:
 0x02	青岛鼎鑫
 0x04	青岛东软
 0x10	北京晓程
 */
typedef struct tag_FK_DT04F170 {
	WORD wPLCPcol;
} FK_DT04F170;

typedef struct tag_FK_DHCP_PARAM {
	FK_TMN_IP IPDhcp;	//终端IP
	FK_TMN_IP IPDhcpMask;	//子网掩码
	FK_TMN_IP IPDhcpGateway;	//网关
	FK_TMN_IP IPDhcpDns;	//DNS
} DHCP_PARAM;

typedef struct tag_FK_DT04F171 {
	BYTE IsGetDhcpIp;	//1 自动获取，0 手动设置
	DHCP_PARAM DhcpParam;
} FK_DT04F171;

typedef struct tag_DT04F249_ITEM {
	WORD nCurveType;	//曲线数据类型
	WORD nIndex;	//曲线类型对应的索引号
	WORD nCurveStoreIndex;	//2类数据中曲线存储位置
} DT04F249_ITEM;

//2类数据中曲线存储位置动态调整
typedef struct tag_FK_DT04F249 {
	DT04F249_ITEM DTF249[NL_FK_MAX_CURVE_MEASURE_POINT];
} FK_DT04F249;

//一类数据，但具有参数属性，故移到参数区
//F3
typedef struct tag_FK_DT0CF03 {
	BYTE nFlag[31];
} FK_DT0CF03;

//F5
typedef struct tag_FK_CTRL_SET {
	BYTE nScheme;
	BYTE tmFlag;
	BYTE stGK;
	BYTE stDK;
	BYTE stGKLC;
	BYTE stDKLC;
} FK_CTRL_SET;

typedef struct tag_FK_DT0CF05 {
	BYTE nState;
	BYTE nSuitFlag;
	FK_CTRL_SET hSuit[NL_FK_MAX_SUIT];
} FK_DT0CF05;

// 本地调试开关
typedef struct tag_FK_DT0FF96 {
	BYTE nFlag;
} FK_DT0FF96;

//tcp/ip类型
typedef struct tag_FK_DT0FF97 {
	BYTE nType;
} FK_DT0FF97;

//上日正向有/无功电能示值、一/四象限无功电能示值(总、费率1~M)
typedef struct tag_FK_DT0FF33 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm1QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm4QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0FF33;
//上日反向有/无功电能示值、二/三象限无功电能示值(总、费率1~M)
typedef struct tag_FK_DT0FF34 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm2QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm3QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0FF34;

//自定义电压合格运行时间和终端运行时间
typedef struct tag_FK_DT0FF35 {
	WORD wVoltOkMin;
	WORD wTmnRunMin;
} FK_DT0FF35;

typedef struct tag_FK_DT0FF98 {
	int nDBLoadFlag;
} FK_DT0FF98;
//zzd
//手动保电标志和已保电时间
typedef struct tag_FK_DT0FF99 {
	BYTE nManualFlag;
	DWORD dwManualMin;
} FK_DT0FF99;

typedef struct tag_FK_DT0FF100 {
	WORD ref[3];

} FK_DT0FF100;

//上月正向有/无功电能示值、一/四象限无功电能示值(总、费率1~M)
typedef struct tag_FK_DT0FF101 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm1QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm4QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0FF101;
//上月反向有/无功电能示值、二/三象限无功电能示值(总、费率1~M)
typedef struct tag_FK_DT0FF102 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm2QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm3QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0FF102;

//升级参数
typedef struct tag_FK_DT0FF103 {
	DWORD dwOldType;
	DWORD dwOldMS;
	DWORD dwOldLS;
} FK_DT0FF103;

//轮显参数
typedef struct tag_FK_DTCYCGUI {
	WORD wDI;
	BYTE bFlag;	//D0位默认为0，显示二次值，1为显示一次值;D1位代表该数据项是否选中，1为选中，0表示未选中

} FK_DTCYCGUI;

typedef struct tag_FK_DT0FF104 {
	FK_DTCYCGUI cycGui[15];
} FK_DT0FF104;

// 04F111 集中器自维护定时任务参数
typedef struct tag_FK_DT04F111 {
	BYTE nPort; //通讯端口
	BYTE nStart; // 启用关闭标志：00－关闭，01－启用
	FK_DATAMASK18 dmStartTime;
	BYTE nHour;  //允许执行时长
} FK_DT04F111;

/*// 载波主动上报的电表信息
 typedef struct tag_FK_DT04F112
 {
 WORD nCount;
 REPORT_METER nMeter[NL_FK_MAX_PLC_MEASURED_POINT];
 }FK_DT04F112;
 */

typedef struct tag_FK_DT05F63 {
	BYTE nStart; // 启用关闭标志：00－关闭，01－启用
	BYTE nPort;  //通讯端口号
} FK_DT05F63;

//载波模块程序版本号
typedef struct tag_FK_DT0CF186 {
	char szFactoryCode[2]; //厂商代码
	char szChipCode[2];    //芯片代码
	BYTE nDay;        //版本日期-日
	BYTE nMonth;      //版本日期-月
	BYTE nYear;       //版本日期-年
	WORD nVersion;    //版本
} FK_DT0CF186;

////////////////////////////////////////////////////////////////

typedef struct tag_FK_04F {
	FK_DT04F01 DT04F01;
	FK_DT04F02 DT04F02;
	FK_DT04F03 DT04F03;
	FK_DT04F04 DT04F04;
	FK_DT04F05 DT04F05;
	FK_DT04F06 DT04F06;
	FK_DT04F07 DT04F07;
	FK_DT04F08 DT04F08;
	FK_DT04F09 DT04F09;
	FK_DT04F10 DT04F10;
	FK_DT04F11 DT04F11;
	FK_DT04F12 DT04F12;
	FK_DT04F13 DT04F13;
	FK_DT04F14 DT04F14;
	FK_DT04F15 DT04F15;
	FK_DT04F16 DT04F16;
	FK_DT04F17 DT04F17;
	FK_DT04F18 DT04F18;
	FK_DT04F19 DT04F19;
	FK_DT04F20 DT04F20;
	FK_DT04F21 DT04F21;
	FK_DT04F22 DT04F22;
	FK_DT04F23 DT04F23;

	FK_DT04F25 DT04F25[NL_JZ_MAX_MEASURED_POINT];	//三相表+单相表
	FK_DT04F26 DT04F26[NL_JZ_MAX_MEASURED_POINT];
	FK_DT04F27 DT04F27[NL_JZ_MAX_MEASURED_POINT];  //变压器损耗参数  可以去掉
	FK_DT04F28 DT04F28[NL_JZ_MAX_MEASURED_POINT];
	FK_DT04F29 DT04F29[NL_JZ_MAX_MEASURED_POINT];
	FK_DT04F30 DT04F30[NL_JZ_MAX_MEASURED_POINT];
	FK_DT04F31 DT04F31[NL_JZ_MAX_MEASURED_POINT];

	//组5
	FK_DT04F33 DT04F33;
	FK_DT04F34 DT04F34;
	FK_DT04F35 DT04F35;
	FK_DT04F36 DT04F36;
	FK_DT04F37 DT04F37;
	FK_DT04F38 DT04F38;  //配置一类参数据项
	FK_DT04F39 DT04F39;  //配置二类参数据项

	//组6
	FK_DT04F41 DT04F41[NL_FK_MAX_SUIT];
	FK_DT04F42 DT04F42[NL_FK_MAX_SUIT];
	FK_DT04F43 DT04F43[NL_FK_MAX_SUIT];
	FK_DT04F44 DT04F44[NL_FK_MAX_SUIT];
	FK_DT04F45 DT04F45[NL_FK_MAX_SUIT];
	FK_DT04F46 DT04F46[NL_FK_MAX_SUIT];
	FK_DT04F47 DT04F47[NL_FK_MAX_SUIT];
	FK_DT04F48 DT04F48[NL_FK_MAX_SUIT];
	FK_DT04F49 DT04F49[NL_FK_MAX_YK];
	FK_DT04F57 DT04F57;
	FK_DT04F58 DT04F58;
	FK_DT04F59 DT04F59;
	FK_DT04F60 DT04F60;
	FK_DT04F61 DT04F61;

	FK_DT04F65 DT04F65[NL_FK_MAX_TASKS];
	FK_DT04F66 DT04F66[NL_FK_MAX_TASKS];
	FK_DT04F67 DT04F67[NL_FK_MAX_TASKS];
	FK_DT04F68 DT04F68[NL_FK_MAX_TASKS];

	/*
	 //组10 电容器暂不具备
	 FK_DT04F73	DT04F73[NL_JZ_MAX_MEASURED_POINT];
	 FK_DT04F74	DT04F74[NL_JZ_MAX_MEASURED_POINT];
	 FK_DT04F75	DT04F75[NL_JZ_MAX_MEASURED_POINT];
	 FK_DT04F76	DT04F76[NL_JZ_MAX_MEASURED_POINT];

	 //组11 模拟量暂不具备
	 FK_DT04F81	DT04F81[NL_FK_MAX_ML];
	 FK_DT04F82	DT04F82[NL_FK_MAX_ML];
	 FK_DT04F83	DT04F83[NL_FK_MAX_ML];
	 */

	//江苏增补
	FK_DT04F85 DT04F85;	// 低压集抄中用于设置终端地址
	FK_DT04F92 DT04F92[NL_JZ_MAX_MEASURED_POINT];	//电能表局编号和费率排序
	FK_DT04F95 DT04F95;	//终端密码
	FK_DT04F96 DT04F96;	//485口功能设置
	FK_DT04F97 DT04F97;	//主从终端设置

	//中间变量
	FK_DT04F100 DT04F100;	//存储测量位置标识  单三相中间变量
	FK_DT04F101 DT04F101;
	FK_DT04F169 DT04F169;	// 通道类型
	FK_DT04F170 DT04F170;	// 载波方案
	FK_DT04F171 DT04F171;	// 自动DHCP/手动获取IP
	FK_DT04F249 DT04F249;	//2类数据中曲线存储位置

	FK_DT0CF03 DT0CF03;	//一类数据,但具有参数属性，故移到参数区
	FK_DT0CF05 DT0CF05;	//一类数据,但具有参数属性，故移到参数区

	FK_DT0FF96 DT0FF96;
	FK_DT0FF97 DT0FF97;
	FK_DT0FF99 DT0FF99;
	FK_DT0FF100 DT0FF100;
	FK_DT0FF103 DT0FF103;
	FK_DT0FF104 DT0FF104;
	//
	FK_DT04F111 DT04F111;
//	FK_DT04F112 DT04F112;
	FK_DT05F63 DT05F63;
	FK_DT0CF186 DT0CF186;
} FK_04F;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 终端控制参数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//组1
//F1
typedef struct tag_FK_DT05F01 {
	BYTE ntmLimit;
	BYTE ntmDelay;
} FK_DT05F01;

//组2
//F9
typedef struct tag_FK_DT05F09 {
	BYTE nFlag;
	BYTE nScheme;
} FK_DT05F09;

//F12
typedef struct tag_FK_DT05F12 {
	BYTE ntmWindow;
	FK_DATAMASK04 nQuotiety;
	BYTE ntmDelay;
	BYTE nCtrlTime; //分钟
	BYTE nWarnTime[4];
} FK_DT05F12;

//组4
//F25
typedef struct tag_FK_DT05F25 {
	BYTE nTime;
} FK_DT05F25;

//F31
typedef struct tag_FK_DT05F31 {
	FK_DATAMASK01 time;
} FK_DT05F31;

//F32
typedef struct tag_FK_DT05F32 {
	BYTE nIndex;
	BYTE nType;
	BYTE nLength;
	char str[200];
} FK_DT05F32;

//组6
//F41
typedef struct tag_FK_DT05F41 {
	BYTE nFlag[2];
} FK_DT05F41;

//F42
typedef struct tag_FK_DT05F42 {
	BYTE nFlag[2];
} FK_DT05F42;

//组7
//F49
typedef struct tag_FK_DT05F49 {
	BYTE nPort;
} FK_DT05F49;

//F50
typedef struct tag_FK_DT05F50 {
	BYTE nPort;
} FK_DT05F50;
//F51
typedef struct tag_FK_DT05F51 {
	BYTE nPort;
} FK_DT05F51;
//F52
typedef struct tag_FK_DT05F52 {
	BYTE nPort;
} FK_DT05F52;
//F53
typedef struct tag_FK_DT05F53 {
	BYTE nPort;
} FK_DT05F53;

//
typedef struct tag_FK_05F {
	FK_DT05F01 DT05F01[NL_FK_MAX_YK];
	FK_DT05F09 DT05F09[NL_FK_MAX_SUIT];
	FK_DT05F12 DT05F12[NL_FK_MAX_SUIT];
	FK_DT05F25 DT05F25;
	FK_DT05F31 DT05F31;
	FK_DT05F32 DT05F32;

//  电容器暂不需要
//	FK_DT05F41	DT05F41[NL_JZ_MAX_MEASURED_POINT];
//	FK_DT05F42	DT05F42[NL_JZ_MAX_MEASURED_POINT];

	FK_DT05F49 DT05F49[NL_FK_PARA_MAX_COM];   //可能根据需要调整数据
	FK_DT05F50 DT05F50[NL_FK_PARA_MAX_COM];
	FK_DT05F51 DT05F51[NL_FK_PARA_MAX_COM];
	FK_DT05F52 DT05F52[NL_FK_PARA_MAX_COM];
	FK_DT05F53 DT05F53[NL_FK_PARA_MAX_COM];
} FK_05F;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//终端配置
//F1
typedef struct tag_FK_DT09F01 {
	char szFactoryCode[4];
	char szDeviceCode[8];
	char szSoftwareVersion[4];
	FK_DATAMASK20 dmSoftReleaseDate;
	char szConfig[11];
	char szProtocolVersion[4];
	char szHardwareVersion[4];
	FK_DATAMASK20 dmHardReleaseDate;
} FK_DT09F01;

//F2
typedef union tag_FK_PORT_TYPE {
	WORD body;
	struct {
		WORD nPort :5;
		WORD nType :2;
		WORD nMode :1;
		WORD nRemain :5;
		WORD nApp :3;
	} mask;
} FK_PORT_TYPE;

typedef struct tag_FK_PORT_INFO {
	FK_PORT_TYPE PortType;
	DWORD dwMaxSpeed;
	WORD wMaxDevice;
	WORD wMaxRcvBuf;
	WORD wMaxSndBuf;
} FK_PORT_INFO;

typedef struct tag_FK_DT09F02 {
	BYTE nFKInMaxMC;
	BYTE nFKInMaxYX;
	BYTE nFKInMaxML;
	BYTE nFKOutMaxYK;
	WORD wFKLinkMaxMeter;
	WORD wFKUpLinkMaxRcvBuf;
	WORD wFKUpLinkMaxSndBuf;
	BYTE nMACAddr[6];
	BYTE nFKCommPortTotal;
	FK_PORT_INFO PortInfo[6];
} FK_DT09F02;

//F3
typedef struct tag_FK_DT09F03 {
	WORD wFKMaxMeasurce;
	BYTE nFKMaxCombin;
	BYTE nFKMaxTask;
	BYTE nFKMaxDiff;
	BYTE nFKMaxTariff;
	BYTE nFKMaxMTCongealDensity;
	BYTE nFKMaxCPCongealDensity;
	BYTE nFKMaxCQCongealDensity;
	BYTE nFKMaxCECongealDensity;
	BYTE nFKMaxCVCongealDensity;
	BYTE nFKMaxStoreDay;
	BYTE nFKMaxStoreMonth;
	BYTE nFKMaxSpanControl;
	BYTE nFKMaxHarmDetect;
	BYTE nFKMaxQCalibrate;
	BYTE nFKMaxImportUser;
	WORD wFKUserBigKinds;
	BYTE nFKUserSmallKind[16];
} FK_DT09F03;

typedef struct tag_FK_DT09F04 {
	BYTE nFKMaxParamDataType;
	BYTE nFKParamDataArray[31];
} FK_DT09F04;

typedef struct tag_FK_DT09F05 {
	BYTE nFKMaxControlDataType;
	BYTE nFKControlDataArray[31];
} FK_DT09F05;

typedef struct tag_FK_CST_CONFIG {
	BYTE nCount;
	BYTE hInfoSet[NL_FK_MAX_DT_ITEM];
} FK_CST_CONFIG;

//终端支持的1类数据配置
typedef struct tag_FK_DT09F06 {
	WORD wBigFlag;
	FK_CST_CONFIG hItem[NL_FK_MAX_BIG_KIND];   //支持6个可设置的+默认的
} FK_DT09F06;

//终端支持的2类数据配置
typedef struct tag_FK_DT09F07 {
	WORD wBigFlag;
	FK_CST_CONFIG hItem[NL_FK_MAX_BIG_KIND];
} FK_DT09F07;

typedef struct tag_FK_DT09F08 {
	BYTE nErcMask[8];
} FK_DT09F08;

//终端配置
typedef struct tag_FK_09F {
	FK_DT09F01 DT09F01;
	FK_DT09F02 DT09F02;
	FK_DT09F03 DT09F03;
	FK_DT09F04 DT09F04;
	FK_DT09F05 DT09F05;
	FK_DT09F06 DT09F06;
	FK_DT09F07 DT09F07;
	FK_DT09F08 DT09F08;

} FK_09F;

//F2
typedef struct tag_FK_DT0CF02 {
	FK_DATAMASK01 dmTime;
} FK_DT0CF02;

//
//F3 移到参数区
//typedef struct tag_FK_DT0CF03
//{
//	BYTE nFlag[31];
//}FK_DT0CF03;

//F4
typedef struct tag_FK_DT0CF04 {
	BYTE nReport;
	BYTE nTalk;
} FK_DT0CF04;

//一类数据，但具有参数属性，故移到参数区
//F5
//typedef struct tag_FK_CTRL_SET
//{
//	BYTE nScheme;
//	BYTE tmFlag;
//	BYTE stGK;
//	BYTE stDK;
//	BYTE stGKLC;
//	BYTE stDKLC;
//}FK_CTRL_SET;

//typedef struct tag_FK_DT0CF05
//{
//	BYTE 		nState;
//	BYTE 		nSuitFlag;
//	FK_CTRL_SET	hSuit[NL_FK_MAX_SUIT];
//} FK_DT0CF05;

//F6
typedef struct tag_FK_CTRL_CUR {
	FK_DATAMASK02 dmRating;
	FK_DATAMASK04 dmQuotiety;
	BYTE nstGKOn;
	BYTE nstYDKOn;
	BYTE nstGDKOn;
	BYTE nstGKWarn;
	BYTE nstDKWarn;
} FK_CTRL_CUR;

typedef struct tag_FK_DT0CF06 {
	BYTE nstOut;
	BYTE nstFeeWarn;
	BYTE nSuitFlag;
	FK_CTRL_CUR hSuit[NL_FK_MAX_SUIT];
} FK_DT0CF06;

//F7
typedef struct tag_FK_DT0CF07 {
	BYTE nEC1;
	BYTE nEC2;
} FK_DT0CF07;

//F8
typedef struct tag_FK_DT0CF08 {
	BYTE nFlag[8];
} FK_DT0CF08;

//组2
//F9
typedef struct tag_FK_DT0CF09 {
	BYTE nST;
	BYTE nCD;
} FK_DT0CF09;

typedef struct tag_FK_DT0CF10 {
	DWORD dwDayStream;
	DWORD dwMonStream;
} FK_DT0CF10;

typedef struct tag_FK_GMET_STATE {
	BYTE port;   //端口
	WORD num;   //抄表总数
	BYTE state;   //工作状态
	WORD suc_count;   //成功总数
	BYTE suc_zd_count;   //重点表块数
	FK_DATAMASK01 dmBeginTime;   //开始时间
	FK_DATAMASK01 dmEndTime;   //结束时间
} FK_GMet_State;

typedef struct tag_FK_DT0CF11 {
	BYTE nCount;
	FK_GMet_State hMetState[NL_FK_PARA_MAX_COM];
} FK_DT0CF11;

typedef struct tag_FK_DT0CF12 {
	FK_DATAMASK05 dBm;
} FK_DT0CF12;

//终端上次停电时间（增补）
typedef struct tag_FK_DT0CF15 {
	FK_DATAMASK15 dmTime;
} FK_DT0CF15;

//中文信息
typedef struct tag_FK_DT0CF16 {
	FK_DATAMASK01 dmTime;
	BYTE nType;
	BYTE nIndex;
	BYTE nLen;
	char strInfo[200];
} FK_DT0CF16;

//组3
//F17
typedef struct tag_FK_DT0CF17 {
	FK_DATAMASK02 dmPower;
} FK_DT0CF17;

//F18
typedef struct tag_FK_DT0CF18 {
	FK_DATAMASK02 dmPower;
} FK_DT0CF18;

//F19
typedef struct tag_FK_DT0CF19 {
	BYTE nTariffs;
	FK_DATAMASK03 dmPower[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF19;

//F20
typedef struct tag_FK_DT0CF20 {
	BYTE nTariffs;
	FK_DATAMASK03 dmPower[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF20;

//F21
typedef struct tag_FK_DT0CF21 {
	BYTE nTariffs;
	FK_DATAMASK03 dmPower[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF21;

//F22
typedef struct tag_FK_DT0CF22 {
	BYTE nTariffs;
	FK_DATAMASK03 dmPower[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF22;

//F23
typedef struct tag_FK_DT0CF23 {
	FK_DATAMASK03 dmEnergy;
} FK_DT0CF23;

//F24
typedef struct tag_FK_DT0CF24 {
	FK_DATAMASK02 dmPower;
} FK_DT0CF24;

//组4
//F25
typedef struct tag_FK_DT0CF25 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK09 dmPs;
	FK_DATAMASK09 dmPa;
	FK_DATAMASK09 dmPb;
	FK_DATAMASK09 dmPc;
	FK_DATAMASK09 dmQs;
	FK_DATAMASK09 dmQa;
	FK_DATAMASK09 dmQb;
	FK_DATAMASK09 dmQc;
	FK_DATAMASK05 dmFs;
	FK_DATAMASK05 dmFa;
	FK_DATAMASK05 dmFb;
	FK_DATAMASK05 dmFc;
	FK_DATAMASK07 dmUa;
	FK_DATAMASK07 dmUb;
	FK_DATAMASK07 dmUc;
	FK_DATAMASK25 dmIa;
	FK_DATAMASK25 dmIb;
	FK_DATAMASK25 dmIc;
	FK_DATAMASK25 dmI0;
	FK_DATAMASK09 dmAPs;
	FK_DATAMASK09 dmAPa;
	FK_DATAMASK09 dmAPb;
	FK_DATAMASK09 dmAPc;
} FK_DT0CF25;

//F26
typedef struct tag_FK_DT0CF26 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK08 dmDsRefs;
	FK_DATAMASK08 dmDaRefs;
	FK_DATAMASK08 dmDbRefs;
	FK_DATAMASK08 dmDcRefs;
	FK_DATAMASK10 dmDsTimes;
	FK_DATAMASK10 dmDaTimes;
	FK_DATAMASK10 dmDbTimes;
	FK_DATAMASK10 dmDcTimes;
	FK_DATAMASK17 dmDsBeginTime;
	FK_DATAMASK17 dmDaBeginTime;
	FK_DATAMASK17 dmDbBeginTime;
	FK_DATAMASK17 dmDcBeginTime;
	FK_DATAMASK17 dmDsEndTime;
	FK_DATAMASK17 dmDaEndTime;
	FK_DATAMASK17 dmDbEndTime;
	FK_DATAMASK17 dmDcEndTime;
} FK_DT0CF26;

//F27
typedef struct tag_FK_DT0CF27 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK01 dmMeterTime;
	FK_DATAMASK27 dmBatteryRun;
	FK_DATAMASK10 dmProgramCount;
	FK_DATAMASK01 dmLastProgram;
	FK_DATAMASK10 dmClrAllCount;
	FK_DATAMASK01 dmLastClrAll;
	FK_DATAMASK10 dmClrDemandCount;
	FK_DATAMASK01 dmLastClrDemand;
	FK_DATAMASK10 dmClrEventCount;
	FK_DATAMASK01 dmLastClrEvent;
	FK_DATAMASK10 dmCheckTimeCount;
	FK_DATAMASK01 dmLastCheckTime;
} FK_DT0CF27;

typedef struct tag_FK_DT0CF28 {
	FK_DATAMASK15 dmTime;
	WORD dmXchge[7];		// 电表运行状态字变位标志
	WORD dmState[7];		// 电表运行状态字

} FK_DT0CF28;

typedef struct tag_FK_DT0CF29 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmEnergyCu;
	FK_DATAMASK14 dmEnergyFe;
} FK_DT0CF29;

typedef struct tag_FK_DT0CF30 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmEnergyCu;
	FK_DATAMASK14 dmEnergyFe;
} FK_DT0CF30;

typedef struct tag_FK_DT0CF31 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmZPEnergy_A;
	FK_DATAMASK14 dmFPEnergy_A;
	FK_DATAMASK11 dmQ1Energy_A;
	FK_DATAMASK11 dmQ2Energy_A;
	FK_DATAMASK14 dmZPEnergy_B;
	FK_DATAMASK14 dmFPEnergy_B;
	FK_DATAMASK11 dmQ1Energy_B;
	FK_DATAMASK11 dmQ2Energy_B;
	FK_DATAMASK14 dmZPEnergy_C;
	FK_DATAMASK14 dmFPEnergy_C;
	FK_DATAMASK11 dmQ1Energy_C;
	FK_DATAMASK11 dmQ2Energy_C;
} FK_DT0CF31;

typedef struct tag_FK_DT0CF32 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmZPEnergy_A;
	FK_DATAMASK14 dmFPEnergy_A;
	FK_DATAMASK11 dmQ1Energy_A;
	FK_DATAMASK11 dmQ2Energy_A;
	FK_DATAMASK14 dmZPEnergy_B;
	FK_DATAMASK14 dmFPEnergy_B;
	FK_DATAMASK11 dmQ1Energy_B;
	FK_DATAMASK11 dmQ2Energy_B;
	FK_DATAMASK14 dmZPEnergy_C;
	FK_DATAMASK14 dmFPEnergy_C;
	FK_DATAMASK11 dmQ1Energy_C;
	FK_DATAMASK11 dmQ2Energy_C;
} FK_DT0CF32;

//组5
//F33
typedef struct tag_FK_DT0CF33 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm1QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm4QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF33;

//F34
typedef struct tag_FK_DT0CF34 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm2QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm3QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF34;

//F35
typedef struct tag_FK_DEMAND {
	FK_DATAMASK23 dmValue;
	FK_DATAMASK17 dmTime;
} FK_DEMAND;

typedef struct tag_FK_DT0CF35 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF35;

//F36
typedef struct tag_FK_DT0CF36 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF36;

//F37
typedef struct tag_FK_DT0CF37 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm1QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm4QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF37;

//F38
typedef struct tag_FK_DT0CF38 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm2QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm3QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF38;

//F39
typedef struct tag_FK_DT0CF39 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF39;

//F40
typedef struct tag_FK_DT0CF40 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF40;

//组6
//F41
typedef struct tag_FK_DT0CF41 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF41;

//F42
typedef struct tag_FK_DT0CF42 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF42;

//F43
typedef struct tag_FK_DT0CF43 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF43;

//F44
typedef struct tag_FK_DT0CF44 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF44;

//F45
typedef struct tag_FK_DT0CF45 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF45;

//F46
typedef struct tag_FK_DT0CF46 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF46;

//F47
typedef struct tag_FK_DT0CF47 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF47;

//F48
typedef struct tag_FK_DT0CF48 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF48;

//组7
//F49
typedef struct tag_FK_DT0CF49 {
	FK_DATAMASK05 dmAngleUa;
	FK_DATAMASK05 dmAngleUb;
	FK_DATAMASK05 dmAngleUc;
	FK_DATAMASK05 dmAngleIa;
	FK_DATAMASK05 dmAngleIb;
	FK_DATAMASK05 dmAngleIc;
} FK_DT0CF49;

//增补
typedef struct tag_FK_FEE {
	BYTE hour;
	BYTE min;
	BYTE NN;
} FK_FEE;

typedef struct tag_FK_GDAY {
	BYTE day;
	BYTE hour;
} FK_GDAY;

//读取电表参数
typedef struct tag_FK_DT0CF50 {
	FK_DATAMASK15 dmTime;
	FK_GDAY dmGDay;		// 自动抄表日
	DWORD dwPConst;	// 有功常数
	DWORD dwQConst;	// 无功常数
	DWORD dwPEBase;	// 有功起始读数
	DWORD dwQEBase;	// 无功起始读数
	BYTE addr[6];	// 表号
	BYTE user[6];	// 用户号
	BYTE fcid[6];	// 设备码
	FK_FEE fee[8];	// 费率时段
} FK_DT0CF50;

//组8
//F57
typedef struct tag_FK_DT0CF57 {
	BYTE nHarmonics;
	FK_DATAMASK07 dmHUa[18];
	FK_DATAMASK07 dmHUb[18];
	FK_DATAMASK07 dmHUc[18];
	FK_DATAMASK06 dmHIa[18];
	FK_DATAMASK06 dmHIb[18];
	FK_DATAMASK06 dmHIc[18];
} FK_DT0CF57;

//F58
typedef struct tag_FK_DT0CF58 {
	BYTE nHarmonics;
	FK_DATAMASK05 nUaSHRatio;
	FK_DATAMASK05 nUaNHRatio[18];
	FK_DATAMASK05 nUbSHRatio;
	FK_DATAMASK05 nUbNHRatio[18];
	FK_DATAMASK05 nUcSHRatio;
	FK_DATAMASK05 nUcNHRatio[18];
	FK_DATAMASK05 nIaSHRatio;
	FK_DATAMASK05 nIaNHRatio[18];
	FK_DATAMASK05 nIbSHRatio;
	FK_DATAMASK05 nIbNHRatio[18];
	FK_DATAMASK05 nIcSHRatio;
	FK_DATAMASK05 nIcNHRatio[18];
} FK_DT0CF58;

//组9
//F65
typedef struct tag_FK_DT0CF65 {
	BYTE nType;
	BYTE nFlag[2];
} FK_DT0CF65;

//F66
typedef struct tag_FK_DT0CF66 {
	DWORD dwTimes[9];
	DWORD dwCount[9];
} FK_DT0CF66;

//F67
typedef struct tag_FK_DT0CF67 {
	FK_DATAMASK13 dmDayQEnergy;
	FK_DATAMASK13 dmMonQEnergy;
} FK_DT0CF67;

//组10
//F73
typedef struct tag_FK_DT0CF73 {
	FK_DATAMASK02 dmDMS;
} FK_DT0CF73;

//组11
//F81
typedef struct tag_FK_DT0CF81 {
	FK_DATAMASK02 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF81;

//F82
typedef struct tag_FK_DT0CF82 {
	FK_DATAMASK02 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF82;

//F83
typedef struct tag_FK_DT0CF83 {
	FK_DATAMASK03 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF83;

//F84
typedef struct tag_FK_DT0CF84 {
	FK_DATAMASK03 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF84;

//组12
//F89
typedef struct tag_FK_DT0CF89 {
	FK_DATAMASK09 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF89;

//F90
typedef struct tag_FK_DT0CF90 {
	FK_DATAMASK09 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF90;

//F91
typedef struct tag_FK_DT0CF91 {
	FK_DATAMASK09 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF91;

//F92
typedef struct tag_FK_DT0CF92 {
	FK_DATAMASK09 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF92;

//F93
typedef struct tag_FK_DT0CF93 {
	FK_DATAMASK09 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF93;

//F94
typedef struct tag_FK_DT0CF94 {
	FK_DATAMASK09 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF94;

//F95
typedef struct tag_FK_DT0CF95 {
	FK_DATAMASK09 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF95;

//F96
typedef struct tag_FK_DT0CF96 {
	FK_DATAMASK09 dmPower[4];
	FK_TDH hTd;
} FK_DT0CF96;

//组13
//F97
typedef struct tag_FK_DT0CF97 {
	FK_DATAMASK07 dmVoltage[4];
	FK_TDH hTd;
} FK_DT0CF97;

//F98
typedef struct tag_FK_DT0CF98 {
	FK_DATAMASK07 dmVoltage[4];
	FK_TDH hTd;
} FK_DT0CF98;

//F99
typedef struct tag_FK_DT0CF99 {
	FK_DATAMASK07 dmVoltage[4];
	FK_TDH hTd;
} FK_DT0CF99;

//F100
typedef struct tag_FK_DT0CF100 {
	FK_DATAMASK25 dmCurrent[4];
	FK_TDH hTd;
} FK_DT0CF100;

//F101
typedef struct tag_FK_DT0CF101 {
	FK_DATAMASK25 dmCurrent[4];
	FK_TDH hTd;
} FK_DT0CF101;

//F102
typedef struct tag_FK_DT0CF102 {
	FK_DATAMASK25 dmCurrent[4];
	FK_TDH hTd;
} FK_DT0CF102;

//F103
typedef struct tag_FK_DT0CF103 {
	FK_DATAMASK25 dmCurrent[4];
	FK_TDH hTd;
} FK_DT0CF103;

//组14
//F105
typedef struct tag_FK_DT0CF105 {
	FK_DATAMASK13 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF105;

//F106
typedef struct tag_FK_DT0CF106 {
	FK_DATAMASK13 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF106;

//F107
typedef struct tag_FK_DT0CF107 {
	FK_DATAMASK13 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF107;

//F108
typedef struct tag_FK_DT0CF108 {
	FK_DATAMASK13 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF108;

//F109
typedef struct tag_FK_DT0CF109 {
	FK_DATAMASK11 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF109;

//F110
typedef struct tag_FK_DT0CF110 {
	FK_DATAMASK11 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF110;

//F111
typedef struct tag_FK_DT0CF111 {
	FK_DATAMASK11 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF111;

//F112
typedef struct tag_FK_DT0CF112 {
	FK_DATAMASK11 dmEnergy[4];
	FK_TDH hTd;
} FK_DT0CF112;

//组15
//F113
typedef struct tag_FK_DT0CF113 {
	FK_DATAMASK05 dmFunctor[4];
	FK_TDH hTd;
} FK_DT0CF113;

//F114
typedef struct tag_FK_DT0CF114 {
	FK_DATAMASK05 dmFunctor[4];
	FK_TDH hTd;
} FK_DT0CF114;

//F115
typedef struct tag_FK_DT0CF115 {
	FK_DATAMASK05 dmFunctor[4];
	FK_TDH hTd;
} FK_DT0CF115;

//F116
typedef struct tag_FK_DT0CF116 {
	FK_DATAMASK05 dmFunctor[4];
	FK_TDH hTd;
} FK_DT0CF116;

//组16
//F121
typedef struct tag_FK_DT0CF121 {
	FK_DATAMASK02 dmDMS[4];
	FK_TDH hTd;
} FK_DT0CF121;

//F129
typedef struct tag_FK_DT0CF129 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF129;

//F131
typedef struct tag_FK_DT0CF131 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF131;

//组20
//F153
typedef struct tag_FK_DT0CF153 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF153;

typedef struct tag_FK_DT0CF154 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF154;

typedef struct tag_FK_DT0CF155 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF155;

typedef struct tag_FK_DT0CF156 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF156;

typedef struct tag_FK_DT0CF157 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF157;

typedef struct tag_FK_DT0CF158 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF158;

typedef struct tag_FK_DT0CF159 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF159;

typedef struct tag_FK_DT0CF160 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF160;

typedef struct tag_FK_DT0CF161 {
	FK_DATAMASK15 dmTime;
	BYTE mStause;	//电能表通电状态
	FK_DATAMASK15 mStartTime;	//最近一次通电时间
	FK_DATAMASK15 mStopTime;	//最近一次断电时间
} FK_DT0CF161;
//F165
typedef struct tag_FK_DT0CF165 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK08 mProgramKeyTimes;	//编程开关开关次数
	FK_DATAMASK15 ProgramKeyTime;	//最近一次开关时间
	FK_DATAMASK08 mTrailCoverTimes;	//尾盖打开次数
	FK_DATAMASK15 TrailCoverTime;	//最近一次尾盖打开时间
} FK_DT0CF165;
//F166
typedef struct tag_FK_DT0CF166 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK08 mCheckClockTimes;	//电能表对时次数
	FK_DATAMASK15 CheckClockTime;	//最近一次对时时间
	FK_DATAMASK08 mPeriodChangeTimes;	//时段更改次数
	FK_DATAMASK15 PeriodChangeTime;	//最近一次时段更改时间
} FK_DT0CF166;
//F167
typedef struct tag_FK_DT0CF167 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK08 mBuyTimes;	//购电次数
	FK_DATAMASK14 RemainRMB;	//剩余金额
	FK_DATAMASK14 TotalBuyRMB;	//全部购电金额
	FK_DATAMASK11 RemainEnergy;	//剩余电量
	FK_DATAMASK11 OverEnergy;	//透支电量
	FK_DATAMASK11 TotalBuyEnergy;	//累计购电量
	FK_DATAMASK11 OverdraftStopEnergy;	//拖欠门限电量
	FK_DATAMASK11 AlarmEnergy;	//报警电量
	FK_DATAMASK11 FailureEnergy;	//故障电量
} FK_DT0CF167;
//F168
typedef struct tag_FK_DT0CF168 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 StrikeEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK14 UnStrikeEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0CF168;

typedef struct tag_FK_ROUTER {
	BYTE nRelaySeries;
	FK_DATAMASK12 RelayAddr[NL_FK_MAX_ROUTE_SERIES];
} FK_ROUTER;

//F169
typedef struct tag_FK_DT0CF169 {
	BYTE nPort;
	BYTE nRouter;

	FK_ROUTER Router[NL_FK_MAX_ROUTE];

} FK_DT0CF169;

//F170
typedef struct tag_FK_DT0CF170 {
	BYTE nPort;
	BYTE nRelaySeries;
	BYTE commPhase;
	BYTE commQuanlity;
	BYTE commFlag;
	FK_DATAMASK01 commSuccTime;
	FK_DATAMASK01 commFailTime;
	BYTE commFailTimes;

} FK_DT0CF170;

// 补充自定义协议
// 2011-09-21

typedef struct tag_FAILD_METER {
	WORD nIndex;     // 电表序号
	WORD nTN;        // 所属测量点号
	BYTE nProtocol;  // 电表协议类型
	BYTE addr[6];	 // 测量点地址
	BYTE collAddr[6];	 // 电表对应采集器地址
} FAILD_METER;

// 所有冻结失败的载波表
typedef struct tag_FK_DT0CF180 {
	//WORD nCount;
	FAILD_METER nMeter;	 //[NL_FK_MAX_PLC_MEASURED_POINT*2];
} FK_DT0CF180;

typedef struct tag_REPORT_METER {
	WORD nIndex;     // 序号
	BYTE nProtocol;  // 电表协议类型
	BYTE addr[6];	 // 测量点地址
	BYTE collAddr[6];	 // 电表对应采集器地址
	BYTE nState;     // 变更状态
} REPORT_METER;

// 载波主动上报的电表信息
typedef struct tag_FK_DT0CF181 {
	//WORD nCount;
	REPORT_METER nMeter;     //[NL_FK_MAX_PLC_MEASURED_POINT*2];
} FK_DT0CF181;

// 自定义规约
// 终端启动时间和Build号
typedef struct tag_FK_DT0CF240 {
	FK_DATAMASK01 tmStart;
	FK_DATAMASK08 wBuildNum;
} FK_DT0CF240;

// 抄表成功标识
typedef struct tag_FK_DT0CF241 {
	BYTE gMetSucFlag[132]; // 132*8 = 1056测量点, 目前最大1024, 按位表示 0/1 失败/成功
} FK_DT0CF241;

// 冻结成功标识
typedef struct tag_FK_DT0CF242 {
	BYTE gMetSucFlag[132]; // 132*8 = 1056测量点, 目前最大1024, 按位表示 0/1 失败/成功
} FK_DT0CF242;

// end

typedef struct tag_FK_0CF {
	FK_DT0CF02 DT0CF02;
//	FK_DT0CF03	DT0CF03;
	FK_DT0CF04 DT0CF04;
//	FK_DT0CF05	DT0CF05;	//具有参数属性，移到参数区
	FK_DT0CF06 DT0CF06;
	FK_DT0CF07 DT0CF07;
	FK_DT0CF08 DT0CF08;
	FK_DT0CF09 DT0CF09;
	FK_DT0CF10 DT0CF10;	// 通讯流量
	FK_DT0CF11 DT0CF11;	// 抄表状态
	FK_DT0CF12 DT0CF12;	// 信号品质
	FK_DT0CF15 DT0CF15;	// 自定义（终端上次停电时间）
	FK_DT0CF16 DT0CF16[NK_FK_MAX_CNINFO];	//消息
	FK_DT0CF17 DT0CF17[NL_FK_MAX_SUIT];
	FK_DT0CF18 DT0CF18[NL_FK_MAX_SUIT];
	FK_DT0CF19 DT0CF19[NL_FK_MAX_SUIT];
	FK_DT0CF20 DT0CF20[NL_FK_MAX_SUIT];
	FK_DT0CF21 DT0CF21[NL_FK_MAX_SUIT];
	FK_DT0CF22 DT0CF22[NL_FK_MAX_SUIT];
	FK_DT0CF23 DT0CF23[NL_FK_MAX_SUIT];
	FK_DT0CF24 DT0CF24[NL_FK_MAX_SUIT];
	//组4
	FK_DT0CF25 DT0CF25[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF26 DT0CF26[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF27 DT0CF27[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF28 DT0CF28[NL_FK_MAX_485_MEASURED_POINT];

	//F29铜损、铁损暂不需要
	//F30

	FK_DT0CF31 DT0CF31[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF32 DT0CF32[NL_FK_MAX_485_MEASURED_POINT];

	//组5
	FK_DT0CF33 DT0CF33[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF34 DT0CF34[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF35 DT0CF35[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF36 DT0CF36[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF37 DT0CF37[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF38 DT0CF38[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF39 DT0CF39[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF40 DT0CF40[NL_FK_MAX_485_MEASURED_POINT];
	//组6
	FK_DT0CF41 DT0CF41[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF42 DT0CF42[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF43 DT0CF43[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF44 DT0CF44[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF45 DT0CF45[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF46 DT0CF46[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF47 DT0CF47[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF48 DT0CF48[NL_FK_MAX_485_MEASURED_POINT];
	//组7
	FK_DT0CF49 DT0CF49[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF50 DT0CF50[NL_FK_MAX_485_MEASURED_POINT];  //补充存储读取电表参数

	/*
	 //组8 谐波数据暂不需要
	 FK_DT0CF57	DT0CF57[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0CF58	DT0CF58[NL_FK_MAX_485_MEASURED_POINT];
	 */

	/*电容器数据暂不需要
	 //组9
	 FK_DT0CF65	DT0CF65[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0CF66	DT0CF66[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0CF67	DT0CF67[NL_FK_MAX_485_MEASURED_POINT];
	 */

	/*直流模拟量数据暂不需要
	 //组10
	 FK_DT0CF73	DT0CF73[NL_FK_MAX_ML];
	 */

	//组20 时区电量
	FK_DT0CF153 DT0CF153[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF153 DT0CF154[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF153 DT0CF155[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF153 DT0CF156[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF153 DT0CF157[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF153 DT0CF158[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF153 DT0CF159[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF153 DT0CF160[NL_FK_MAX_485_MEASURED_POINT];
	//组21
	FK_DT0CF161 DT0CF161[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF165 DT0CF165[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF166 DT0CF166[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF167 DT0CF167[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0CF168 DT0CF168[NL_FK_MAX_485_MEASURED_POINT];

	FK_DT0CF169 DT0CF169[NL_JZ_MAX_MEASURED_POINT];
	FK_DT0CF170 DT0CF170[NL_JZ_MAX_MEASURED_POINT];

	FK_DT0FF33 DT0FF33[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0FF34 DT0FF34[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0FF35 DT0FF35[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0FF98 DT0FF98;
	FK_DT0FF101 DT0FF101[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0FF102 DT0FF102[NL_FK_MAX_485_MEASURED_POINT];

	// 2012-01-05增加
	FK_DT0CF241 DT0CF241;
	FK_DT0CF242 DT0CF242;
} FK_0CF;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 终端二类数据/参数
// 二类数据项是一个数据单元的集合，下列对二类数据项的定义仅为数据集合中的一个数据项
// 并且不包含时标信息
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//组1
//F1
typedef struct tag_FK_DT0DF01 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm1QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm4QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF01;

//F2
typedef struct tag_FK_DT0DF02 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm2QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm3QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF02;

//F3
typedef struct tag_FK_DT0DF03 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF03;

//F4
typedef struct tag_FK_DT0DF04 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF04;

//F5
typedef struct tag_FK_DT0DF05 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF05;

//F6
typedef struct tag_FK_DT0DF06 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF06;

//F7
typedef struct tag_FK_DT0DF07 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF07;

//F8
typedef struct tag_FK_DT0DF08 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF08;

//组2
//F9
typedef struct tag_FK_DT0DF09 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm1QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm4QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF09;

//F10
typedef struct tag_FK_DT0DF10 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm2QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm3QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF10;

//F11
typedef struct tag_FK_DT0DF11 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF11;

//F12
typedef struct tag_FK_DT0DF12 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF12;

//组3
//F17
typedef struct tag_FK_DT0DF17 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm1QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm4QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF17;

//F18
typedef struct tag_FK_DT0DF18 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm2QEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK11 dm3QEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF18;

//F19
typedef struct tag_FK_DT0DF19 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF19;

//F20
typedef struct tag_FK_DT0DF20 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
	FK_DEMAND dmMaxQ[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF20;

//F21
typedef struct tag_FK_DT0DF21 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF21;

//F22
typedef struct tag_FK_DT0DF22 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF22;

//F23
typedef struct tag_FK_DT0DF23 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF23;

//F24
typedef struct tag_FK_DT0DF24 {
	BYTE nTariffs;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF24;

//组4
//F25
typedef struct tag_FK_DT0DF25 {
	FK_DATAMASK23 dmMaxPs;
	FK_DATAMASK18 dmMaxPsTime;
	FK_DATAMASK23 dmMaxPa;
	FK_DATAMASK18 dmMaxPaTime;
	FK_DATAMASK23 dmMaxPb;
	FK_DATAMASK18 dmMaxPbTime;
	FK_DATAMASK23 dmMaxPc;
	FK_DATAMASK18 dmMaxPcTime;
	WORD nPsTimes;
	WORD nPaTimes;
	WORD nPbTimes;
	WORD nPcTimes;
} FK_DT0DF25;

//F26
typedef struct tag_FK_DT0DF26 {
	FK_DATAMASK23 dmMaxSDemand;
	FK_DATAMASK18 dmMaxSDemandTime;
	FK_DATAMASK23 dmMaxADemand;
	FK_DATAMASK18 dmMaxADemandTime;
	FK_DATAMASK23 dmMaxBDemand;
	FK_DATAMASK18 dmMaxBDemandTime;
	FK_DATAMASK23 dmMaxCDemand;
	FK_DATAMASK18 dmMaxCDemandTime;
} FK_DT0DF26;

//F27
typedef struct tag_FK_DT0DF27 {
	WORD nuulVaTimes;
	WORD nddlVaTimes;
	WORD nulVaTimes;
	WORD ndlVaTimes;
	WORD nokVaTimes;
	WORD nuulVbTimes;
	WORD nddlVbTimes;
	WORD nulVbTimes;
	WORD ndlVbTimes;
	WORD nokVbTimes;
	WORD nuulVcTimes;
	WORD nddlVcTimes;
	WORD nulVcTimes;
	WORD ndlVcTimes;
	WORD nokVcTimes;
	FK_DATAMASK07 dmMaxVa;
	FK_DATAMASK18 dmMaxVaTime;
	FK_DATAMASK07 dmMinVa;
	FK_DATAMASK18 dmMinVaTime;
	FK_DATAMASK07 dmMaxVb;
	FK_DATAMASK18 dmMaxVbTime;
	FK_DATAMASK07 dmMinVb;
	FK_DATAMASK18 dmMinVbTime;
	FK_DATAMASK07 dmMaxVc;
	FK_DATAMASK18 dmMaxVcTime;
	FK_DATAMASK07 dmMinVc;
	FK_DATAMASK18 dmMinVcTime;
	FK_DATAMASK07 dmAvgVa;
	FK_DATAMASK07 dmAvgVb;
	FK_DATAMASK07 dmAvgVc;
} FK_DT0DF27;

//F28
typedef struct tag_FK_DT0DF28 {
	WORD nITimes;
	WORD nVTimes;
	FK_DATAMASK05 dmMaxIValue;
	FK_DATAMASK18 dmMaxITime;
	FK_DATAMASK05 dmMaxVValue;
	FK_DATAMASK18 dmMaxVTime;
} FK_DT0DF28;

//F29
typedef struct tag_FK_DT0DF29 {
	WORD nuulIa;
	WORD nulIa;
	WORD nuulIb;
	WORD nulIb;
	WORD nuulIc;
	WORD nulIc;
	WORD nulI0;
	FK_DATAMASK25 dmMaxIa;
	FK_DATAMASK18 dmMaxIaTime;
	FK_DATAMASK25 dmMaxIb;
	FK_DATAMASK18 dmMaxIbTime;
	FK_DATAMASK25 dmMaxIc;
	FK_DATAMASK18 dmMaxIcTime;
	FK_DATAMASK25 dmMaxI0;
	FK_DATAMASK18 dmMaxI0Time;
} FK_DT0DF29;

//F30
typedef struct tag_FK_DT0DF30 {
	WORD nuulPTimes;
	WORD nulPTimes;
} FK_DT0DF30;

//F31
typedef struct tag_FK_DT0DF31 {
	FK_DATAMASK05 dmMaxPs;
	FK_DATAMASK18 dmMaxPsTime;
	FK_DATAMASK05 dmMinPs;
	FK_DATAMASK18 dmMinPsTime;
} FK_DT0DF31;

//F32
typedef struct tag_FK_DT0DF32 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK08 dmPBsRefs;
	FK_DATAMASK08 dmPBaRefs;
	FK_DATAMASK08 dmPBbRefs;
	FK_DATAMASK08 dmPBcRefs;
	FK_DATAMASK10 dmPBsTimes;
	FK_DATAMASK10 dmPBaTimes;
	FK_DATAMASK10 dmPBbTimes;
	FK_DATAMASK10 dmPBcTimes;
	FK_DATAMASK17 dmPBBeginTime;
	FK_DATAMASK17 dmPBaBeginTime;
	FK_DATAMASK17 dmPBbBeginTime;
	FK_DATAMASK17 dmPBcBeginTime;
	FK_DATAMASK17 dmPBEndTime;
	FK_DATAMASK17 dmPBaEndTime;
	FK_DATAMASK17 dmPBbEndTime;
	FK_DATAMASK17 dmPBcEndTime;
} FK_DT0DF32;
//组5
//F33
typedef struct tag_FK_DT0DF33 {
	FK_DATAMASK23 dmMaxPs;
	FK_DATAMASK18 dmMaxPsTime;
	FK_DATAMASK23 dmMaxPa;
	FK_DATAMASK18 dmMaxPaTime;
	FK_DATAMASK23 dmMaxPb;
	FK_DATAMASK18 dmMaxPbTime;
	FK_DATAMASK23 dmMaxPc;
	FK_DATAMASK18 dmMaxPcTime;
	WORD nPsTimes;
	WORD nPaTimes;
	WORD nPbTimes;
	WORD nPcTimes;
} FK_DT0DF33;

//F34
typedef struct tag_FK_DT0DF34 {
	FK_DATAMASK23 dmMaxSDemand;
	FK_DATAMASK18 dmMaxSDemandTime;
	FK_DATAMASK23 dmMaxADemand;
	FK_DATAMASK18 dmMaxADemandTime;
	FK_DATAMASK23 dmMaxBDemand;
	FK_DATAMASK18 dmMaxBDemandTime;
	FK_DATAMASK23 dmMaxCDemand;
	FK_DATAMASK18 dmMaxCDemandTime;
} FK_DT0DF34;

//F35
typedef struct tag_FK_DT0DF35 {
	WORD nuulVaTimes;
	WORD nddlVaTimes;
	WORD nulVaTimes;
	WORD ndlVaTimes;
	WORD nokVaTimes;
	WORD nuulVbTimes;
	WORD nddlVbTimes;
	WORD nulVbTimes;
	WORD ndlVbTimes;
	WORD nokVbTimes;
	WORD nuulVcTimes;
	WORD nddlVcTimes;
	WORD nulVcTimes;
	WORD ndlVcTimes;
	WORD nokVcTimes;
	FK_DATAMASK07 dmMaxVa;
	FK_DATAMASK18 dmMaxVaTime;
	FK_DATAMASK07 dmMinVa;
	FK_DATAMASK18 dmMinVaTime;
	FK_DATAMASK07 dmMaxVb;
	FK_DATAMASK18 dmMaxVbTime;
	FK_DATAMASK07 dmMinVb;
	FK_DATAMASK18 dmMinVbTime;
	FK_DATAMASK07 dmMaxVc;
	FK_DATAMASK18 dmMaxVcTime;
	FK_DATAMASK07 dmMinVc;
	FK_DATAMASK18 dmMinVcTime;
	FK_DATAMASK07 dmAvgVa;
	FK_DATAMASK07 dmAvgVb;
	FK_DATAMASK07 dmAvgVc;
} FK_DT0DF35;

//F36
typedef struct tag_FK_DT0DF36 {
	WORD nITimes;
	WORD nVTimes;
	FK_DATAMASK05 dmMaxIValue;
	FK_DATAMASK17 dmMaxITime;
	FK_DATAMASK05 dmMaxVValue;
	FK_DATAMASK17 dmMaxVTime;
} FK_DT0DF36;

//F37
typedef struct tag_FK_DT0DF37 {
	WORD nuulIa;
	WORD nulIa;
	WORD nuulIb;
	WORD nulIb;
	WORD nuulIc;
	WORD nulIc;
	WORD nulI0;
	FK_DATAMASK25 dmMaxIa;
	FK_DATAMASK18 dmMaxIaTime;
	FK_DATAMASK25 dmMaxIb;
	FK_DATAMASK18 dmMaxIbTime;
	FK_DATAMASK25 dmMaxIc;
	FK_DATAMASK18 dmMaxIcTime;
	FK_DATAMASK25 dmMaxI0;
	FK_DATAMASK18 dmMaxI0Time;
} FK_DT0DF37;

//F38
typedef struct tag_FK_DT0DF38 {
	WORD nuulPTimes;
	WORD nulPTimes;
} FK_DT0DF38;

typedef struct tag_FK_DT0DF39 {
	FK_DATAMASK05 dmMaxPs;
	FK_DATAMASK17 dmMaxPsTime;
	FK_DATAMASK05 dmMinPs;
	FK_DATAMASK17 dmMinPsTime;
} FK_DT0DF39;

//组6
//F41
typedef struct tag_FK_DT0DF41 {
	DWORD dwTimes[9];
	DWORD dwCount[9];
} FK_DT0DF41;

//F42
typedef struct tag_FK_DT0DF42 {
	FK_DATAMASK13 dmDayEnergy;
	FK_DATAMASK13 dmMonEnergy;
} FK_DT0DF42;

//F43
typedef struct tag_FK_DT0DF43 {
	WORD wT1Times;
	WORD wT2Times;
	WORD wT3Times;
} FK_DT0DF43;

//F44
typedef struct tag_FK_DT0DF44 {
	WORD wT1Times;
	WORD wT2Times;
	WORD wT3Times;
} FK_DT0DF44;

//组7
//F49
typedef struct tag_FK_DT0DF49 {
	WORD wRunTimes;
	WORD wResetRefs;
} FK_DT0DF49;

//F50
typedef struct tag_FK_DT0DF50 {
	BYTE nMECtrlRefs;
	BYTE nGECtrlRefs;
	BYTE nPCtrlRefs;
	BYTE nRCtrlRefs;
} FK_DT0DF50;

//F51
typedef struct tag_FK_DT0DF51 {
	WORD wRunTimes;
	WORD wResetRefs;
} FK_DT0DF51;

//F52
typedef struct tag_FK_DT0DF52 {
	BYTE nMECtrlRefs;
	BYTE nGECtrlRefs;
	BYTE nPCtrlRefs;
	BYTE nRCtrlRefs;
} FK_DT0DF52;
//F53
typedef struct tag_FK_DT0DF53 {
	DWORD dwDayCommFlow;
} FK_DT0DF53;

typedef struct tag_FK_DT0DF54 {
	DWORD dwMonCommFlow;
} FK_DT0DF54;

//组8
//F57
typedef struct tag_FK_DT0DF57 {
	FK_DATAMASK02 dmMaxP;
	FK_DATAMASK18 dmMaxPTime;
	FK_DATAMASK02 dmMinP;
	FK_DATAMASK18 dmMinPTime;
	WORD wP0Times;
} FK_DT0DF57;

//F58
typedef struct tag_FK_DT0DF58 {
	BYTE nTariffs;
	FK_DATAMASK03 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF58;

//F59
typedef struct tag_FK_DT0DF59 {
	BYTE nTariffs;
	FK_DATAMASK03 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF59;

//F60
typedef struct tag_FK_DT0DF60 {
	FK_DATAMASK02 dmMaxP;
	FK_DATAMASK18 dmMaxPTime;
	FK_DATAMASK02 dmMinP;
	FK_DATAMASK18 dmMinPTime;
	WORD wP0Times;
} FK_DT0DF60;

//F61
typedef struct tag_FK_DT0DF61 {
	BYTE nTariffs;
	FK_DATAMASK03 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF61;

//F62
typedef struct tag_FK_DT0DF62 {
	BYTE nTariffs;
	FK_DATAMASK03 dmEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF62;

//组9
//F65
typedef struct tag_FK_DT0DF65 {
	WORD wTimes;
	FK_DATAMASK03 dmEnergy;
} FK_DT0DF65;

//F66
typedef struct tag_FK_DT0DF66 {
	WORD wTimes;
	FK_DATAMASK03 dmEnergy;
} FK_DT0DF66;

// 组10
// F73
typedef struct tag_FK_DT0DF73 {
	FK_TDC hTd;
	FK_DATAMASK02 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF73;

// F74
typedef struct tag_FK_DT0DF74 {
	FK_TDC hTd;
	FK_DATAMASK02 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF74;

// F75
typedef struct tag_FK_DT0DF75 {
	FK_TDC hTd;
	FK_DATAMASK03 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF75;

// F76
typedef struct tag_FK_DT0DF76 {
	FK_TDC hTd;
	FK_DATAMASK03 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF76;

// 组11
// F81
typedef struct tag_FK_DT0DF81 {
	FK_TDC hTd;
	FK_DATAMASK09 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF81;

// F82
typedef struct tag_FK_DT0DF82 {
	FK_TDC hTd;
	FK_DATAMASK09 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF82;

// F83
typedef struct tag_FK_DT0DF83 {
	FK_TDC hTd;
	FK_DATAMASK09 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF83;

// F84
typedef struct tag_FK_DT0DF84 {
	FK_TDC hTd;
	FK_DATAMASK09 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF84;

// F85
typedef struct tag_FK_DT0DF85 {
	FK_TDC hTd;
	FK_DATAMASK09 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF85;

// F86
typedef struct tag_FK_DT0DF86 {
	FK_TDC hTd;
	FK_DATAMASK09 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF86;

// F87
typedef struct tag_FK_DT0DF87 {
	FK_TDC hTd;
	FK_DATAMASK09 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF87;

// F88
typedef struct tag_FK_DT0DF88 {
	FK_TDC hTd;
	FK_DATAMASK09 dmPower[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF88;

// 组12
// F89
typedef struct tag_FK_DT0DF89 {
	FK_TDC hTd;
	FK_DATAMASK07 dmVoltage[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF89;

// F90
typedef struct tag_FK_DT0DF90 {
	FK_TDC hTd;
	FK_DATAMASK07 dmVoltage[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF90;

// F91
typedef struct tag_FK_DT0DF91 {
	FK_TDC hTd;
	FK_DATAMASK07 dmVoltage[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF91;

// F92
typedef struct tag_FK_DT0DF92 {
	FK_TDC hTd;
	FK_DATAMASK25 dmCurrent[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF92;

// F93
typedef struct tag_FK_DT0DF93 {
	FK_TDC hTd;
	FK_DATAMASK25 dmCurrent[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF93;

// F94
typedef struct tag_FK_DT0DF94 {
	FK_TDC hTd;
	FK_DATAMASK25 dmCurrent[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF94;

// F95
typedef struct tag_FK_DT0DF95 {
	FK_TDC hTd;
	FK_DATAMASK25 dmCurrent[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF95;

// 组13
// F97
typedef struct tag_FK_DT0DF97 {
	FK_TDC hTd;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF97;

// F98
typedef struct tag_FK_DT0DF98 {
	FK_TDC hTd;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF98;

// F99
typedef struct tag_FK_DT0DF99 {
	FK_TDC hTd;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF99;

// F100
typedef struct tag_FK_DT0DF100 {
	FK_TDC hTd;
	FK_DATAMASK13 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF100;

// F101
typedef struct tag_FK_DT0DF101 {
	FK_TDC hTd;
	FK_DATAMASK11 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF101;

// F102
typedef struct tag_FK_DT0DF102 {
	FK_TDC hTd;
	FK_DATAMASK11 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF102;

// F103
typedef struct tag_FK_DT0DF103 {
	FK_TDC hTd;
	FK_DATAMASK11 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF103;

// F104
typedef struct tag_FK_DT0DF104 {
	FK_TDC hTd;
	FK_DATAMASK11 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF104;

// 组14
// F105
typedef struct tag_FK_DT0DF105 {
	FK_TDC hTd;
	FK_DATAMASK05 dmFunctor[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF105;

// F106
typedef struct tag_FK_DT0DF106 {
	FK_TDC hTd;
	FK_DATAMASK05 dmFunctor[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF106;

// F107
typedef struct tag_FK_DT0DF107 {
	FK_TDC hTd;
	FK_DATAMASK05 dmFunctor[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF107;

// F108
typedef struct tag_FK_DT0DF108 {
	FK_TDC hTd;
	FK_DATAMASK05 dmFunctor[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF108;
// F109
typedef struct tag_FK_DT0DF109 {
	FK_TDC hTd;
	FK_DATAMASK05 dmFunctor[NL_FK_MAX_PERIOD_POINT][3];
} FK_DT0DF109;

// F110
typedef struct tag_FK_DT0DF110 {
	FK_TDC hTd;
	FK_DATAMASK05 dmFunctor[NL_FK_MAX_PERIOD_POINT][3];
} FK_DT0DF110;

//组15
//F113
typedef struct tag_FK_IHARM {
	FK_DATAMASK06 value;
	FK_DATAMASK17 time;
} FK_IHARM;

typedef struct tag_FK_DT0DF113 {
	FK_IHARM hHarm[18];
	FK_DATAMASK06 dmMaxI;
	FK_DATAMASK17 dmMaxITime;
} FK_DT0DF113;

//F114
typedef struct tag_FK_DT0DF114 {
	FK_IHARM hHarm[18];
	FK_DATAMASK06 dmMaxI;
	FK_DATAMASK17 dmMaxITime;
} FK_DT0DF114;

//F115
typedef struct tag_FK_DT0DF115 {
	FK_IHARM hHarm[18];
	FK_DATAMASK06 dmMaxI;
	FK_DATAMASK17 dmMaxITime;
} FK_DT0DF115;

//F116
typedef struct tag_FK_UHARM {
	FK_DATAMASK05 value;
	FK_DATAMASK17 time;
} FK_UHARM;

typedef struct tag_FK_DT0DF116 {
	FK_UHARM hHarm[18];
	FK_DATAMASK05 dmMaxV;
	FK_DATAMASK17 dmMaxVTime;
} FK_DT0DF116;

//F117
typedef struct tag_FK_DT0DF117 {
	FK_UHARM hHarm[18];
	FK_DATAMASK05 dmMaxV;
	FK_DATAMASK17 dmMaxVTime;
} FK_DT0DF117;

//F118
typedef struct tag_FK_DT0DF118 {
	FK_UHARM hHarm[18];
	FK_DATAMASK05 dmMaxV;
	FK_DATAMASK17 dmMaxVTime;
} FK_DT0DF118;

//组16
//F121
typedef struct tag_FK_DT0DF121 {
	BYTE nHarms;
	WORD wVsTimes;
	WORD wVpTimes[18];
	WORD wIsTimes;
	WORD wIpTimes[18];
} FK_DT0DF121;

//F122
typedef struct tag_FK_DT0DF122 {
	BYTE nHarms;
	WORD wVsTimes;
	WORD wVpTimes[18];
	WORD wIsTimes;
	WORD wIpTimes[18];
} FK_DT0DF122;

//F123
typedef struct tag_FK_DT0DF123 {
	BYTE nHarms;
	WORD wVsTimes;
	WORD wVpTimes[18];
	WORD wIsTimes;
	WORD wIpTimes[18];
} FK_DT0DF123;

//组17
//F129
typedef struct tag_FK_DT0DF129 {
	WORD wulTimes;
	WORD wllTimes;
	FK_DATAMASK02 dmMax;
	FK_DATAMASK18 dmMaxTime;
	FK_DATAMASK02 dmMin;
	FK_DATAMASK18 dmMinTime;
} FK_DT0DF129;

//F130
typedef struct tag_FK_DT0DF130 {
	WORD wulTimes;
	WORD wllTimes;
	FK_DATAMASK02 dmMax;
	FK_DATAMASK18 dmMaxTime;
	FK_DATAMASK02 dmMin;
	FK_DATAMASK18 dmMinTime;
} FK_DT0DF130;

// F138
typedef struct tag_FK_DT0DF138 {
	FK_DATAMASK02 dmDMS[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF138;

// F145
typedef struct tag_FK_DT0DF145 {
	FK_TDC hTd;
	FK_DATAMASK11 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF145;
// F146
typedef struct tag_FK_DT0DF146 {
	FK_TDC hTd;
	FK_DATAMASK11 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF146;
// F147
typedef struct tag_FK_DT0DF147 {
	FK_TDC hTd;
	FK_DATAMASK11 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF147;
// F148
typedef struct tag_FK_DT0DF148 {
	FK_TDC hTd;
	FK_DATAMASK11 dmEnergy[NL_FK_MAX_PERIOD_POINT];
} FK_DT0DF148;
// F153
typedef struct tag_FK_DT0DF153 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmPEnergyA;
	FK_DATAMASK14 dmPEnergyB;
	FK_DATAMASK14 dmPEnergyC;
} FK_DT0DF153;
// F154
typedef struct tag_FK_DT0DF154 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK11 dmQEnergyA;
	FK_DATAMASK11 dmQEnergyB;
	FK_DATAMASK11 dmQEnergyC;
} FK_DT0DF154;
// F155
typedef struct tag_FK_DT0DF155 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmPEnergyA;
	FK_DATAMASK14 dmPEnergyB;
	FK_DATAMASK14 dmPEnergyC;
} FK_DT0DF155;
// F156
typedef struct tag_FK_DT0DF156 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK11 dmQEnergyA;
	FK_DATAMASK11 dmQEnergyB;
	FK_DATAMASK11 dmQEnergyC;
} FK_DT0DF156;
// F157
typedef struct tag_FK_DT0DF157 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmPEnergyA;
	FK_DATAMASK14 dmPEnergyB;
	FK_DATAMASK14 dmPEnergyC;
} FK_DT0DF157;
// F158
typedef struct tag_FK_DT0DF158 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK11 dmQEnergyA;
	FK_DATAMASK11 dmQEnergyB;
	FK_DATAMASK11 dmQEnergyC;
} FK_DT0DF158;
// F159
typedef struct tag_FK_DT0DF159 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmPEnergyA;
	FK_DATAMASK14 dmPEnergyB;
	FK_DATAMASK14 dmPEnergyC;
} FK_DT0DF159;
// F160
typedef struct tag_FK_DT0DF160 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK11 dmQEnergyA;
	FK_DATAMASK11 dmQEnergyB;
	FK_DATAMASK11 dmQEnergyC;
} FK_DT0DF160;

// F161
typedef struct tag_FK_DT0DF161 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF161;

// F162
typedef struct tag_FK_DT0DF162 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF162;

// F163
typedef struct tag_FK_DT0DF163 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF163;

// F164
typedef struct tag_FK_DT0DF164 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF164;

//F177
typedef struct tag_FK_DT0DF177 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF177;

//F178
typedef struct tag_FK_DT0DF178 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF178;

//F179
typedef struct tag_FK_DT0DF179 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF179;

//F180
typedef struct tag_FK_DT0DF180 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK11 dmQEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF180;

//F185
typedef struct tag_FK_DT0DF185 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF185;
//F201
typedef struct tag_FK_DT0DF201 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} FK_DT0DF201;
//F209
typedef struct tag_FK_DT0DF209 {
	FK_DATAMASK15 dmTime;
	BYTE mStause;  //电能表通电状态
	FK_DATAMASK15 mStartTime;  //最近一次通电时间
	FK_DATAMASK15 mStopTime;  //最近一次断电时间
} FK_DT0DF209;

//F213
typedef struct tag_FK_DT0DF213 {
	FK_DATAMASK15 dmTime;
	WORD mProgramKeyTimes;  //编程开关开关次数
	FK_DATAMASK15 ProgramKeyTime;  //最近一次开关时间
	WORD mTrailCoverTimes;  //尾盖打开次数
	FK_DATAMASK15 TrailCoverTime;  //最近一次尾盖打开时间
} FK_DT0DF213;
//F214
typedef struct tag_FK_DT0DF214 {
	FK_DATAMASK15 dmTime;
	WORD mCheckClockTimes;  //电能表对时次数
	FK_DATAMASK15 CheckClockTime;  //最近一次对时时间
	WORD mPeriodChangeTimes;  //时段更改次数
	FK_DATAMASK15 PeriodChangeTime;  //最近一次时段更改时间
} FK_DT0DF214;
//F215
typedef struct tag_FK_DT0DF215 {
	FK_DATAMASK15 dmTime;
	WORD mBuyTimes;  //购电次数
	FK_DATAMASK14 RemainRMB;  //剩余金额
	FK_DATAMASK14 TotalBuyRMB;  //全部购电金额
	FK_DATAMASK11 RemainEnergy;  //剩余电量
	FK_DATAMASK11 OverEnergy;  //透支电量
	FK_DATAMASK11 TotalBuyEnergy;  //累计购电量
	FK_DATAMASK11 OverdraftStopEnergy;  //拖欠门限电量
	FK_DATAMASK11 AlarmEnergy;  //报警电量
	FK_DATAMASK11 FailureEnergy;  //故障电量
} FK_DT0DF215;
//F216
typedef struct tag_FK_DT0DF216 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 StrikeEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK14 UnStrikeEnergy[NL_FK_MAX_TARIFF + 1];
	FK_DATAMASK14 ErrorEnergy;
} FK_DT0DF216;

//
typedef struct tag_FK_0DF {
	//只放在内存，不存储到实时FLASH中
	//组1
	FK_DT0DF01 DT0DF01[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF02 DT0DF02[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF03 DT0DF03[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF04 DT0DF04[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF05 DT0DF05[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF06 DT0DF06[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF07 DT0DF07[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF08 DT0DF08[NL_FK_MAX_485_MEASURED_POINT];
	//组2
	FK_DT0DF09 DT0DF09[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF10 DT0DF10[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF11 DT0DF11[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF12 DT0DF12[NL_FK_MAX_485_MEASURED_POINT];
	//组3
	FK_DT0DF17 DT0DF17[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF18 DT0DF18[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF19 DT0DF19[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF20 DT0DF20[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF21 DT0DF21[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF22 DT0DF22[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF23 DT0DF23[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF24 DT0DF24[NL_FK_MAX_485_MEASURED_POINT];
	//end

	//组4
	FK_DT0DF25 DT0DF25[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF26 DT0DF26[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF27 DT0DF27[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF28 DT0DF28[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF29 DT0DF29[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF30 DT0DF30[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF31 DT0DF31[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF32 DT0DF32[NL_FK_MAX_485_MEASURED_POINT];
	//组5
	FK_DT0DF33 DT0DF33[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF34 DT0DF34[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF35 DT0DF35[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF36 DT0DF36[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF37 DT0DF37[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF38 DT0DF38[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF39 DT0DF39[NL_FK_MAX_485_MEASURED_POINT];
	//组6
	//FK_DT0DF41	DT0DF41[NL_FK_MAX_485_MEASURED_POINT];
	//FK_DT0DF42	DT0DF42[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF43 DT0DF43[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF44 DT0DF44[NL_FK_MAX_485_MEASURED_POINT];
	//组7
	FK_DT0DF49 DT0DF49;
	FK_DT0DF50 DT0DF50;
	FK_DT0DF51 DT0DF51;
	FK_DT0DF52 DT0DF52;
	FK_DT0DF53 DT0DF53;
	FK_DT0DF54 DT0DF54;
	//组8
	FK_DT0DF57 DT0DF57[NL_FK_MAX_SUIT];
	FK_DT0DF58 DT0DF58[NL_FK_MAX_SUIT];
	FK_DT0DF59 DT0DF59[NL_FK_MAX_SUIT];
	FK_DT0DF60 DT0DF60[NL_FK_MAX_SUIT];
	FK_DT0DF61 DT0DF61[NL_FK_MAX_SUIT];
	FK_DT0DF62 DT0DF62[NL_FK_MAX_SUIT];
	//组9
	FK_DT0DF65 DT0DF65[NL_FK_MAX_SUIT];
	FK_DT0DF66 DT0DF66[NL_FK_MAX_SUIT];
	/*
	 //组10
	 FK_DT0DF73	DT0DF73[NL_FK_MAX_SUIT];
	 FK_DT0DF74	DT0DF74[NL_FK_MAX_SUIT];
	 FK_DT0DF75	DT0DF75[NL_FK_MAX_SUIT];
	 FK_DT0DF76	DT0DF76[NL_FK_MAX_SUIT];
	 //组11
	 FK_DT0DF81	DT0DF81[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF82	DT0DF82[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF83	DT0DF83[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF84	DT0DF84[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF85	DT0DF85[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF86	DT0DF86[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF87	DT0DF87[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF88	DT0DF88[NL_FK_MAX_CURVE_MEASURE_POINT];
	 //组12
	 FK_DT0DF89	DT0DF89[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF90	DT0DF90[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF91	DT0DF91[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF92	DT0DF92[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF93	DT0DF93[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF94	DT0DF94[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF95	DT0DF95[NL_FK_MAX_CURVE_MEASURE_POINT];
	 //组13
	 FK_DT0DF97	DT0DF97[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF98	DT0DF98[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF99	DT0DF99[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF100	DT0DF100[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF101	DT0DF101[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF102	DT0DF102[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF103	DT0DF103[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF104	DT0DF104[NL_FK_MAX_CURVE_MEASURE_POINT];
	 //组14
	 FK_DT0DF105	DT0DF105[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF106	DT0DF106[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF107	DT0DF107[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF108	DT0DF108[NL_FK_MAX_CURVE_MEASURE_POINT];


	 FK_DT0DF109	DT0DF109[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF110	DT0DF110[NL_FK_MAX_CURVE_MEASURE_POINT];
	 //组15
	 FK_DT0DF113	DT0DF113[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0DF114	DT0DF114[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0DF115	DT0DF115[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0DF116	DT0DF116[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0DF117	DT0DF117[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0DF118	DT0DF118[NL_FK_MAX_485_MEASURED_POINT];
	 //组16
	 FK_DT0DF121	DT0DF121[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0DF122	DT0DF122[NL_FK_MAX_485_MEASURED_POINT];
	 FK_DT0DF123	DT0DF123[NL_FK_MAX_485_MEASURED_POINT];

	 //组17
	 FK_DT0DF129	DT0DF129[NL_FK_MAX_ML];
	 FK_DT0DF130	DT0DF130[NL_FK_MAX_ML];
	 //组18
	 FK_DT0DF138	DT0DF138[NL_FK_MAX_ML];

	 FK_DT0DF145	DT0DF145[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF146	DT0DF146[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF147	DT0DF147[NL_FK_MAX_CURVE_MEASURE_POINT];
	 FK_DT0DF148	DT0DF148[NL_FK_MAX_CURVE_MEASURE_POINT];
	 */
	//组20
	FK_DT0DF153 DT0DF153[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF154 DT0DF154[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF155 DT0DF155[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF156 DT0DF156[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF157 DT0DF157[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF158 DT0DF158[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF159 DT0DF159[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF160 DT0DF160[NL_FK_MAX_485_MEASURED_POINT];

	//组26
	FK_DT0DF201 DT0DF201[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF201 DT0DF202[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF201 DT0DF203[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF201 DT0DF204[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF201 DT0DF205[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF201 DT0DF206[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF201 DT0DF207[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF201 DT0DF208[NL_FK_MAX_485_MEASURED_POINT];

	FK_DT0DF209 DT0DF209[NL_FK_MAX_485_MEASURED_POINT];

	FK_DT0DF213 DT0DF213[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF214 DT0DF214[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF215 DT0DF215[NL_FK_MAX_485_MEASURED_POINT];
	FK_DT0DF216 DT0DF216[NL_FK_MAX_485_MEASURED_POINT];

} FK_ODF;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 终端三类数据
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//事件记录格式
typedef struct tag_FK_ERC {
	BYTE code;
	BYTE length;
	void* body;
} FK_ERC;

//事件1
typedef struct tag_FK_ERC1 {
	FK_DATAMASK15 dmTime;
	union {
		BYTE body;
		struct {
			BYTE init :1;		// 参数/数据初始化
			BYTE ver :1;		// 版本变更
			BYTE space :6;		// 保留
		} mask;
	} flag;
	char sVerOld[4];
	char sVerNew[4];
} FK_ERC1;

//事件2
typedef struct tag_FK_ERC2 {
	FK_DATAMASK15 dmTime;
	union {
		BYTE body;
		struct {
			BYTE tmpLost :1;	// 终端参数丢失
			BYTE mppLost :1;	// 测量点参数丢失
			BYTE space :6;	// 保留
		} mask;
	} flag;
} FK_ERC2;

//事件3
typedef struct tag_FK_ERC3 {
	FK_DATAMASK15 dmTime;
	BYTE Master;
	DWORD DT[NL_FK_MAX_DTCHG];
} FK_ERC3;

//事件4
typedef struct tag_FK_ERC4 {
	FK_DATAMASK15 dmTime;
	BYTE nState;
	BYTE nRevState;
} FK_ERC4;

//事件5
typedef struct tag_FK_ERC5 {
	FK_DATAMASK15 dmTime;
	BYTE nWheel;
	FK_DATAMASK02 dmPCur;
	FK_DATAMASK02 dmPAft;
} FK_ERC5;

//事件6
typedef struct tag_FK_ERC6 {
	FK_DATAMASK15 dmTime;
	BYTE nPN;
	BYTE nWheel;
	union {
		BYTE body;
		struct {
			BYTE ctlPeriod :1;	// 时段控
			BYTE ctlRest :1; // 厂休控
			BYTE ctlStop :1; // 营业报停控
			BYTE ctlSlow :1; // 当前功率下浮控
			BYTE space :4;	// 保留
		} mask;
	} type;
	FK_DATAMASK02 dmPCur;
	FK_DATAMASK02 dmPAft;
	FK_DATAMASK02 dmPSet;
} FK_ERC6;

//事件7
typedef struct tag_FK_ERC7 {
	FK_DATAMASK15 dmTime;
	BYTE nPN;
	BYTE nWheel;
	union {
		BYTE body;
		struct {
			BYTE ctlMon :1;	// 月电控
			BYTE ctlBuy :1;	// 购电控
			BYTE space :6;	// 保留
		} mask;
	} type;
	FK_DATAMASK03 dmECur;
	FK_DATAMASK03 dmESet;
} FK_ERC7;

//事件8
typedef struct tag_FK_ERC8 {
	FK_DATAMASK15 dmTime;
	WORD nPN;
	union {
		BYTE body;
		struct {
			BYTE bTraffic :1;	// 时段费率更改
			BYTE bProgram :1;	// 编程时间更改
			BYTE bGMetday :1;	// 抄表日更改
			BYTE bPulse :1;	// 脉冲常数更改
			BYTE bPTCT :1;	// 互感器倍率更改
			BYTE bDemand :1;	// 最大需量清零
			BYTE space :2;  // 保留
		} mask;
	} flag;
} FK_ERC8;

//事件9
typedef struct tag_FK_ERC9 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	union {
		BYTE body;
		struct {
			BYTE phase :3;		// 相位
			BYTE space :3;		// 保留
			BYTE type :2;		// 异常类型
		} mask;
	} flag;
	FK_DATAMASK07 dmVa;
	FK_DATAMASK07 dmVb;
	FK_DATAMASK07 dmVc;
	FK_DATAMASK25 dmIa;
	FK_DATAMASK25 dmIb;
	FK_DATAMASK25 dmIc;
	FK_DATAMASK14 dmEnergy;
} FK_ERC9;

//事件10
typedef struct tag_FK_ERC10 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	union {
		BYTE body;
		struct {
			BYTE phase :3;		// 相位
			BYTE space :3;		// 保留
			BYTE type :2;		// 异常类型
		} mask;
	} flag;
	FK_DATAMASK07 dmVa;
	FK_DATAMASK07 dmVb;
	FK_DATAMASK07 dmVc;
	FK_DATAMASK25 dmIa;
	FK_DATAMASK25 dmIb;
	FK_DATAMASK25 dmIc;
	FK_DATAMASK14 dmEnergy;
} FK_ERC10;

//事件11
typedef struct tag_FK_ERC11 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	FK_DATAMASK05 dmAngleUa;
	FK_DATAMASK05 dmAngleUb;
	FK_DATAMASK05 dmAngleUc;
	FK_DATAMASK05 dmAngleIa;
	FK_DATAMASK05 dmAngleIb;
	FK_DATAMASK05 dmAngleIc;
	FK_DATAMASK14 dmEnergy;
} FK_ERC11;

//事件12
typedef struct tag_FK_ERC12 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
} FK_ERC12;

//事件13
typedef struct tag_FK_ERC13 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	union {
		BYTE body;
		struct {
			BYTE bProgram :1;	// 电能表编程次数或最大需量清零次数发生变化
			BYTE bDX :1;	// 电能表断相次数变化
			BYTE bSY :1;	// 电能表失压次数变化
			BYTE bTD :1;	// 电能表停电次数变化
			BYTE bQY :1;	// 电能表电池欠压
			BYTE space :3;  // 保留
		} mask;
	} flag;
} FK_ERC13;

//事件14
typedef struct tag_FK_ERC14 {
	FK_DATAMASK15 dmPowerOff;
	FK_DATAMASK15 dmPowerOn;
} FK_ERC14;

//事件15
typedef struct tag_FK_ERC15 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	BYTE nFlag;
	BYTE nHarmFlag[3];
	union {
		struct {
			FK_DATAMASK05 hcSum;
			FK_DATAMASK05 hcVal[18];
		} U;
		struct {
			FK_DATAMASK06 ivSum;
			FK_DATAMASK06 ivCur[18];
		} I;
	} harm;
} FK_ERC15;

//事件16
typedef struct tag_FK_ERC16 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	BYTE nDNO;
	BYTE nFlag;
	FK_DATAMASK02 dmDMS;
} FK_ERC16;

//事件17
typedef struct tag_FK_ERC17 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	BYTE nFlag;
	FK_DATAMASK05 dmImbU;
	FK_DATAMASK05 dmImbI;
	FK_DATAMASK07 dmVa;
	FK_DATAMASK07 dmVb;
	FK_DATAMASK07 dmVc;
	FK_DATAMASK25 dmIa;
	FK_DATAMASK25 dmIb;
	FK_DATAMASK25 dmIc;
} FK_ERC17;

//事件18
typedef struct tag_FK_ERC18 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	BYTE nFlag;
	BYTE nSuit[2];
	FK_DATAMASK05 dmFunctor;
	FK_DATAMASK23 dmPower;
	FK_DATAMASK07 dmVolt;
} FK_ERC18;

//事件19
typedef struct tag_FK_ERC19 {
	FK_DATAMASK15 dmTime;
	BYTE nPN;
	DWORD nIndex;
	BYTE nFlag;
	FK_DATAMASK03 dmBuy;
	FK_DATAMASK03 dmWarn;
	FK_DATAMASK03 dmCtrl;
	FK_DATAMASK03 dmPreFree;
	FK_DATAMASK03 dmAftFree;
} FK_ERC19;

//事件20
typedef struct tag_FK_ERC20 {
	FK_DATAMASK15 dmTime;
	BYTE nPW[16];
	BYTE nMSA;
} FK_ERC20;

//事件21
typedef struct tag_FK_ERC21 {
	FK_DATAMASK15 dmTime;
	BYTE nError;
} FK_ERC21;

//事件22
typedef struct tag_FK_ERC22 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	BYTE nPN;
	FK_DATAMASK03 dmDstEnergy;
	FK_DATAMASK03 dmSrcEnergy;
	BYTE nRelate;
	FK_DATAMASK03 dmUtterly;
	BYTE nDstTN;
	FK_DATAMASK14 dmDstTnEnergy[64];
	BYTE nSrcTN;
	FK_DATAMASK14 dmSrcTnEnergy[64];
} FK_ERC22;

//事件23
typedef struct tag_FK_ERC23 {
	FK_DATAMASK15 dmTime;
	BYTE nPN;
	BYTE nWheel;
	union {
		BYTE body;
		struct {
			BYTE ctlMon :1;	// 月电控
			BYTE ctlBuy :1;	// 购电控
			BYTE space :6;	// 保留
		} mask;
	} type;
	FK_DATAMASK03 dmECur;
	FK_DATAMASK03 dmESet;
} FK_ERC23;

//事件24
typedef struct tag_FK_ERC24 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	BYTE nPhases;
	BYTE nType;
	FK_DATAMASK07 dmVa;
	FK_DATAMASK07 dmVb;
	FK_DATAMASK07 dmVc;
} FK_ERC24;

//事件25
typedef struct tag_FK_ERC25 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	BYTE nPhases;
	BYTE nType;
	FK_DATAMASK25 dmIa;
	FK_DATAMASK25 dmIb;
	FK_DATAMASK25 dmIc;
} FK_ERC25;

//事件26
typedef struct tag_FK_ERC26 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	BYTE nType;
	FK_DATAMASK23 dmPCur;
	FK_DATAMASK23 dmPSet;
} FK_ERC26;

//事件27
typedef struct tag_FK_ERC27 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	FK_DATAMASK14 dmPreEnergy;
	FK_DATAMASK14 dmAftEnergy;
} FK_ERC27;

//事件28
typedef struct tag_FK_ERC28 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	FK_DATAMASK14 dmEnergy1;
	FK_DATAMASK14 dmEnergy2;
	FK_DATAMASK22 valSet;
} FK_ERC28;

//事件29
typedef struct tag_FK_ERC29 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	FK_DATAMASK14 dmPreEnergy;
	FK_DATAMASK14 dmAftEnergy;
	FK_DATAMASK22 valSet;
} FK_ERC29;

//事件30
typedef struct tag_FK_ERC30 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD nPN;
	FK_DATAMASK14 dmEnergy;
	BYTE valSet;
} FK_ERC30;

//事件31
typedef struct tag_FK_ERC31 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD wPN;
	FK_DATAMASK15 dmLastCbTime;
	FK_DATAMASK14 dmPEnergy;
	FK_DATAMASK11 dmQnergy;
} FK_ERC31;

//事件32
typedef struct tag_FK_ERC32 {
	FK_DATAMASK15 dmTime;
	DWORD dwCommFlow;
	DWORD dwFlowSet;
} FK_ERC32;

//事件33
typedef struct tag_FK_ERC33 {
	FK_DATAMASK15 dmTime;
	WORD wPN;
	WORD wMetRunStateChge[7];
	WORD wMetRunState[7];
} FK_ERC33;

//事件34
typedef struct tag_FK_ERC34 {
	FK_DATAMASK15 dmTime;
	BYTE nBE;
	WORD wPN;
	BYTE nPhases;
	BYTE nType;
} FK_ERC34;

//事件35
typedef struct tag_FK_NEWMETER {
	FK_DATAMASK12 dmAddr;
	BYTE nPhase;
	BYTE nQuality;
	BYTE nProtocol;
} FK_NEWMETER;

typedef struct tag_FK_ERC35 {
	FK_DATAMASK15 dmTime;
	BYTE nPort;
	BYTE nNum;
	FK_NEWMETER dMeter[5];
} FK_ERC35;

typedef struct tag_FK_ERC36 {
	FK_DATAMASK15 dmPowerOff;
	FK_DATAMASK14 dmPosPEnergy;
	FK_DATAMASK14 dmNegPEnergy;
	FK_DATAMASK11 dm1QEnergy;
	FK_DATAMASK11 dm2QEnergy;
	FK_DATAMASK11 dm3QEnergy;
	FK_DATAMASK11 dm4QEnergy;
	FK_DATAMASK07 dmUa;
	FK_DATAMASK07 dmUb;
	FK_DATAMASK07 dmUc;
	FK_DATAMASK25 dmIa;
	FK_DATAMASK25 dmIb;
	FK_DATAMASK25 dmIc;
	FK_DATAMASK09 dmPs;
	FK_DATAMASK09 dmPa;
	FK_DATAMASK09 dmPb;
	FK_DATAMASK09 dmPc;
	FK_DATAMASK09 dmQs;
	FK_DATAMASK09 dmQa;
	FK_DATAMASK09 dmQb;
	FK_DATAMASK09 dmQc;
	FK_DATAMASK05 dmFs;
	FK_DATAMASK05 dmFa;
	FK_DATAMASK05 dmFb;
	FK_DATAMASK05 dmFc;
	WORD dmState[7];
	BYTE dmTmnState;
} FK_ERC36;

typedef struct tag_FK_0EF {
	FK_ERC1 ERC1[NL_FK_MAX_EVENT];
	FK_ERC2 ERC2[NL_FK_MAX_EVENT];
	FK_ERC3 ERC3[NL_FK_MAX_EVENT];
	FK_ERC4 ERC4[NL_FK_MAX_EVENT];
	FK_ERC5 ERC5[NL_FK_MAX_EVENT];
	FK_ERC6 ERC6[NL_FK_MAX_EVENT];
	FK_ERC7 ERC7[NL_FK_MAX_EVENT];
	FK_ERC8 ERC8[NL_FK_MAX_EVENT];
	FK_ERC9 ERC9[NL_FK_MAX_EVENT];
	FK_ERC10 ERC10[NL_FK_MAX_EVENT];
	FK_ERC11 ERC11[NL_FK_MAX_EVENT];
	FK_ERC12 ERC12[NL_FK_MAX_EVENT];
	FK_ERC13 ERC13[NL_FK_MAX_EVENT];
	FK_ERC14 ERC14[NL_FK_MAX_EVENT];
	FK_ERC15 ERC15[NL_FK_MAX_EVENT];
	FK_ERC16 ERC16[NL_FK_MAX_EVENT];
	FK_ERC17 ERC17[NL_FK_MAX_EVENT];
	FK_ERC18 ERC18[NL_FK_MAX_EVENT];
	FK_ERC19 ERC19[NL_FK_MAX_EVENT];
	FK_ERC20 ERC20[NL_FK_MAX_EVENT];
	FK_ERC21 ERC21[NL_FK_MAX_EVENT];
	FK_ERC22 ERC22[NL_FK_MAX_EVENT];
	FK_ERC23 ERC23[NL_FK_MAX_EVENT];
	FK_ERC24 ERC24[NL_FK_MAX_EVENT];
	FK_ERC25 ERC25[NL_FK_MAX_EVENT];
	FK_ERC26 ERC26[NL_FK_MAX_EVENT];
	FK_ERC27 ERC27[NL_FK_MAX_EVENT];
	FK_ERC28 ERC28[NL_FK_MAX_EVENT];
	FK_ERC29 ERC29[NL_FK_MAX_EVENT];
	FK_ERC30 ERC30[NL_FK_MAX_EVENT];
	FK_ERC31 ERC31[NL_FK_MAX_EVENT];
	FK_ERC32 ERC32[NL_FK_MAX_EVENT];
	FK_ERC33 ERC33[NL_FK_MAX_EVENT];
	FK_ERC34 ERC34[NL_FK_MAX_EVENT];
	FK_ERC35 ERC35[NL_FK_MAX_EVENT];
	FK_ERC36 ERC36[NL_FK_MAX_EVENT];
} FK_0EF;
///////////////////////////////////////////////////
//扩展数据项
//视在功率gs
typedef struct tag_FK_UIPOWER {
	int nVal;
} FK_UIPOWER;

//F25
typedef struct tag_SFK_DT0CF25 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK09 dmPa;
	FK_DATAMASK09 dmQa;
	FK_DATAMASK05 dmFa;
	FK_DATAMASK07 dmUa;
	FK_DATAMASK25 dmIa;
	FK_DATAMASK09 dmAPs;
} SFK_DT0CF25;

//F26
typedef struct tag_SFK_DT0CF26 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK08 dmDaRefs;
	FK_DATAMASK10 dmDaTimes;
	FK_DATAMASK17 dmDaBeginTime;
	FK_DATAMASK17 dmDaEndTime;
} SFK_DT0CF26;

//F27
/*typedef struct tag_SFK_DT0CF27
 {
 FK_DATAMASK15	dmTime;
 FK_DATAMASK01	dmMeterTime;
 FK_DATAMASK27	dmBatteryRun;
 FK_DATAMASK10	dmProgramCount;
 FK_DATAMASK01	dmLastProgram;
 FK_DATAMASK10	dmClrAllCount;
 FK_DATAMASK01	dmLastClrAll;
 FK_DATAMASK10	dmClrDemandCount;
 FK_DATAMASK01	dmLastClrDemand;
 FK_DATAMASK10	dmClrEventCount;
 FK_DATAMASK01	dmLastClrEvent;
 FK_DATAMASK10	dmCheckTimeCount;
 FK_DATAMASK01	dmLastCheckTime;

 BYTE			nMeterRunState;//电表运行状态字
 BYTE			nENetState;//电网状态字
 }SFK_DT0CF27;*/

typedef struct tag_SFK_DT0CF31 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmZPEnergy_A;
} SFK_DT0CF31;

typedef struct tag_SFK_DT0CF32 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK14 dmZPEnergy_A;
} SFK_DT0CF32;

//组5
//F33
typedef struct tag_SFK_DT0CF33 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0CF33;

//F34
typedef struct tag_SFK_DT0CF34 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0CF34;

//F35
typedef struct tag_SFK_DT0CF35 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0CF35;

//F36
typedef struct tag_SFK_DT0CF36 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0CF36;

//F37
typedef struct tag_SFK_DT0CF37 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0CF37;

//F38
typedef struct tag_SFK_DT0CF38 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0CF38;

//F39
typedef struct tag_SFK_DT0CF39 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0CF39;

//F40
typedef struct tag_SFK_DT0CF40 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0CF40;

typedef struct tag_SFK_0CF {
	SFK_DT0CF25 SDT0CF25[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF26 SDT0CF26[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF27 SDT0CF27[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF28 SDT0CF28[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF31 SDT0CF31[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF32 SDT0CF32[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF33 SDT0CF33[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF34 SDT0CF34[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF35 SDT0CF35[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF36 SDT0CF36[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF37 SDT0CF37[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF38 SDT0CF38[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF39 SDT0CF39[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF40 SDT0CF40[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF41 SDT0CF41[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF43 SDT0CF43[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF45 SDT0CF45[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF47 SDT0CF47[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	//组21
	FK_DT0CF161 SDT0CF161[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];

	FK_DT0CF165 SDT0CF165[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF166 SDT0CF166[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF167 SDT0CF167[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF168 SDT0CF168[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
} SFK_0CF;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 终端单相表二类数据
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//组1
//F1
typedef struct tag_SFK_DT0DF01 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF01;

//F2
typedef struct tag_SFK_DT0DF02 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF02;

//F3
typedef struct tag_SFK_DT0DF03 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF03;

//F4
typedef struct tag_SFK_DT0DF04 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF04;

//组2
//F9
typedef struct tag_SFK_DT0DF09 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF09;

//F10
typedef struct tag_SFK_DT0DF10 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF10;

//F11
typedef struct tag_SFK_DT0DF11 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF11;

//F12
typedef struct tag_SFK_DT0DF12 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF12;

//组3
//F17
typedef struct tag_SFK_DT0DF17 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF17;

//F18
typedef struct tag_SFK_DT0DF18 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DATAMASK14 dmPEnergy[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF18;

//F19
typedef struct tag_SFK_DT0DF19 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF19;

//F20
typedef struct tag_SFK_DT0DF20 {
	FK_DATAMASK15 dmTime;
	BYTE nTariffs;
	FK_DEMAND dmMaxP[NL_FK_MAX_TARIFF + 1];
} SFK_DT0DF20;

//组4
//F25
typedef struct tag_SFK_DT0DF25 {
	FK_DATAMASK23 dmMaxPa;
	FK_DATAMASK18 dmMaxPaTime;
	WORD nPaTimes;
} SFK_DT0DF25;

//F26
typedef struct tag_SFK_DT0DF26 {
	FK_DATAMASK23 dmMaxADemand;
	FK_DATAMASK18 dmMaxADemandTime;
} SFK_DT0DF26;

//F27
typedef struct tag_SFK_DT0DF27 {
	WORD nuulVaTimes;
	WORD nddlVaTimes;
	WORD nulVaTimes;
	WORD ndlVaTimes;
	WORD nokVaTimes;
	FK_DATAMASK07 dmMaxVa;
	FK_DATAMASK18 dmMaxVaTime;
	FK_DATAMASK07 dmMinVa;
	FK_DATAMASK18 dmMinVaTime;
	FK_DATAMASK07 dmAvgVa;
} SFK_DT0DF27;

//F29
typedef struct tag_SFK_DT0DF29 {
	WORD nuulIa;
	WORD nulIa;
	FK_DATAMASK25 dmMaxIa;
	FK_DATAMASK18 dmMaxIaTime;
} SFK_DT0DF29;
//F32
typedef struct tag_SFK_DT0DF32 {
	FK_DATAMASK15 dmTime;
	FK_DATAMASK08 dmPBaRefs;
	FK_DATAMASK10 dmPBaTimes;
	FK_DATAMASK17 dmPBaBeginTime;
	FK_DATAMASK17 dmPBaEndTime;
} SFK_DT0DF32;
//组5
//F33
typedef struct tag_SFK_DT0DF33 {
	FK_DATAMASK23 dmMaxPa;
	FK_DATAMASK18 dmMaxPaTime;
	WORD nPaTimes;
} SFK_DT0DF33;

//F34
typedef struct tag_SFK_DT0DF34 {
	FK_DATAMASK23 dmMaxADemand;
	FK_DATAMASK18 dmMaxADemandTime;
} SFK_DT0DF34;

//F35
typedef struct tag_SFK_DT0DF35 {
	WORD nuulVaTimes;
	WORD nddlVaTimes;
	WORD nulVaTimes;
	WORD ndlVaTimes;
	WORD nokVaTimes;
	FK_DATAMASK07 dmMaxVa;
	FK_DATAMASK18 dmMaxVaTime;
	FK_DATAMASK07 dmMinVa;
	FK_DATAMASK18 dmMinVaTime;
	FK_DATAMASK07 dmAvgVa;
} SFK_DT0DF35;

//F37
typedef struct tag_SFK_DT0DF37 {
	WORD nuulIa;
	WORD nulIa;
	FK_DATAMASK25 dmMaxIa;
	FK_DATAMASK18 dmMaxIaTime;
} SFK_DT0DF37;

//
typedef struct tag_SFK_0DF {
	//组1
	SFK_DT0DF01 SDT0DF01[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF02 SDT0DF02[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF03 SDT0DF03[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF04 SDT0DF04[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF05 SDT0DF05[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF07 SDT0DF07[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	//组2
	SFK_DT0DF09 SDT0DF09[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF10 SDT0DF10[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF11 SDT0DF11[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF12 SDT0DF12[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	//组3
	SFK_DT0DF17 SDT0DF17[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF18 SDT0DF18[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF19 SDT0DF19[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF20 SDT0DF20[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF21 SDT0DF21[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF23 SDT0DF23[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	//组4
	SFK_DT0DF25 SDT0DF25[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF26 SDT0DF26[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF27 SDT0DF27[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF28 SDT0DF28[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF29 SDT0DF29[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF32 SDT0DF32[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	//组5
	SFK_DT0DF33 SDT0DF33[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF34 SDT0DF34[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF35 SDT0DF35[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
//	SFK_DT0DF36	SDT0DF36[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0DF37 SDT0DF37[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	//组18
	//FK_DT0DF138	DT0DF138[NL_FK_MAX_ML];
	FK_DT0CF153 SDT0CF153[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF153 SDT0CF155[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF153 SDT0CF157[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF153 SDT0CF159[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];

	/*	FK_DT0DF201 SDT0DF201[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	 FK_DT0DF201 SDT0DF202[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	 FK_DT0DF201 SDT0DF203[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	 FK_DT0DF201 SDT0DF204[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	 FK_DT0DF201 SDT0DF205[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	 FK_DT0DF201 SDT0DF206[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	 FK_DT0DF201 SDT0DF207[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	 FK_DT0DF201 SDT0DF208[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	 */
	FK_DT0DF209 SDT0DF209[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];

	FK_DT0DF213 SDT0DF213[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF214 SDT0DF214[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF215 SDT0DF215[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF216 SDT0DF216[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
} SFK_ODF;

//载波中三相表数据
typedef struct tag_FK_PLC3_0CF {
	FK_DT0CF25 DT0CF25[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF26 DT0CF26[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF27 DT0CF27[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF28 DT0CF28[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF31 DT0CF31[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF32 DT0CF32[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF33 DT0CF33[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF34 DT0CF34[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF35 DT0CF35[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF36 DT0CF36[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
} FK_PLC3_0CF;

typedef struct tag_FK_PLC3_0CMonF {
	FK_DT0CF37 DT0CF37[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF38 DT0CF38[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF39 DT0CF39[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0CF40 DT0CF40[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
} FK_PLC3_0CMonF;

//载波中单相表数据
typedef struct tag_FK_PLC1_0CF {
	FK_DT0CF25 DT0CF25[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF26 DT0CF26[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF27 DT0CF27[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF28 DT0CF28[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF31 DT0CF31[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF32 DT0CF32[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF129 DT0CF129[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF131 DT0CF131[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF35 SDT0CF35[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	SFK_DT0CF36 SDT0CF36[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];

} FK_PLC1_0CF;

typedef struct tag_FK_PLC1_0CMonF {
	FK_DT0CF37 DT0CF37[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0CF38 DT0CF38[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
} FK_PLC1_0CMonF;

//载波中三相表数据
typedef struct tag_FK_PLC3_0DF {
	FK_DT0DF01 DT0DF01[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0DF02 DT0DF02[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0DF17 DT0DF17[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
	FK_DT0DF18 DT0DF18[NL_FK_MAX_PLC_MULTI_MEASURED_POINT];
} FK_PLC3_0DF;

//载波中单相表数据
typedef struct tag_FK_PLC1_0DF {
	FK_DT0DF161 DT0DF161[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF162 DT0DF162[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF163 DT0DF163[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF164 DT0DF164[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF177 DT0DF177[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF178 DT0DF178[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF179 DT0DF179[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
	FK_DT0DF180 DT0DF180[NL_FK_MAX_PLC_SINGLE_MEASURED_POINT];
} FK_PLC1_0DF;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 数据转发
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tag_FK_DT10F01 {
	BYTE nPort;
	BYTE nCtrl;
	BYTE nTranTime;	// 10ms
	BYTE nRecvTime;	// 10ms
	WORD nCMDLen;
	BYTE* lpCMD;
} FK_DT10F01;

//end 交采数据

//需量清零
typedef struct tag_FK_DT91F18 {
	BYTE who[3];
	BYTE flag;
} FK_DT91F18;

//总清
typedef struct tag_FK_DT91F19 {
	BYTE who[3];
	BYTE flag;
} FK_DT91F19;

//事件清零
typedef struct tag_FK_DT91F20 {
	BYTE who[3];
	BYTE flag;
} FK_DT91F20;

/*
 //
 ////////////////////////////////////////////////////
 //电压限值
 typedef struct tag_EM_DIVOLTAGE_LIMIT
 {
 WORD volLimit;
 }EM_DIVOLTAGE_LIMIT;
 ////////////////////////////////////////////////////
 //报警时间限值
 typedef struct tag_EM_EVENT_TIME
 {
 BYTE nSecond;
 }EM_EVENT_TIME;
 ////////////////////////////////////////////////////
 //电流限值
 typedef struct tag_EM_DICURENT_LIMIT
 {
 DWORD curentLimit;
 }EM_DICURENT_LIMIT;
 ////////////////////////////////////////////////////
 //功率限值
 typedef struct tag_EM_DIPOWER_LIMIT
 {
 DWORD powerLimit;
 }EM_DIPOWER_LIMIT;
 ////////////////////////////////////////////////////
 //不平衡限值
 typedef struct tag_EM_DIBALANCE_LIMIT
 {
 DWORD balanceLimit;
 }EM_DIBALANCE_LIMIT;

 typedef struct tag_EM_DI0409FFFF
 {
 EM_DIVOLTAGE_LIMIT	DI04090101;
 EM_DIVOLTAGE_LIMIT	DI04090102;
 EM_DICURENT_LIMIT	DI04090103;
 EM_EVENT_TIME	DI04090104;

 EM_DIVOLTAGE_LIMIT	DI04090201;
 EM_EVENT_TIME	DI04090202;

 EM_DIVOLTAGE_LIMIT	DI04090301;
 EM_EVENT_TIME	DI04090302;

 EM_DIVOLTAGE_LIMIT	DI04090401;
 EM_DICURENT_LIMIT	DI04090402;
 EM_EVENT_TIME	DI040900403;

 EM_DIBALANCE_LIMIT DI04090501;
 EM_EVENT_TIME	DI040900502;

 EM_DIBALANCE_LIMIT DI04090601;
 EM_EVENT_TIME	DI040900602;


 EM_DIVOLTAGE_LIMIT	DI04090701;
 EM_DICURENT_LIMIT	DI04090702;
 EM_DICURENT_LIMIT	DI04090703;
 EM_EVENT_TIME	DI04090704;

 EM_DICURENT_LIMIT	DI04090801;
 EM_EVENT_TIME	DI04090802;

 EM_DIVOLTAGE_LIMIT	DI04090901;
 EM_DICURENT_LIMIT	DI04090902;
 EM_EVENT_TIME	DI04090903;

 EM_DIPOWER_LIMIT	DI04090A01;
 EM_EVENT_TIME	DI04090A02;

 EM_DIPOWER_LIMIT	DI04090B01;
 EM_EVENT_TIME	DI04090B02;


 EM_DIVOLTAGE_LIMIT	DI04090C01;
 EM_DIVOLTAGE_LIMIT	DI04090C02;


 EM_DIPOWER_LIMIT	DI04090D01;
 EM_DIPOWER_LIMIT	DI04090D02;
 EM_EVENT_TIME	DI04090D03;

 EM_DIBALANCE_LIMIT DI04090E01;
 EM_EVENT_TIME	DI040900E02;

 EM_DIBALANCE_LIMIT DI04090F01;
 EM_EVENT_TIME	DI040900F02;
 }EM_DI0409FFFF;
 //for 交采参数
 */

//for 交采数据
typedef struct tag_FK_90F {
	//当前电量及上1个冻结日
	//当前电量
	MT_DI000XFF00 DT0000FF00[13];
	MT_DI000XFF00 DT0001FF00[13];
	MT_DI000XFF00 DT0002FF00[13];
	MT_DI000XFF00 DT0003FF00[13];
	MT_DI000XFF00 DT0004FF00[13];
	MT_DI000XFF00 DT0005FF00[13];
	MT_DI000XFF00 DT0006FF00[13];
	MT_DI000XFF00 DT0007FF00[13];
	MT_DI000XFF00 DT0008FF00[13];
	MT_DI000XFF00 DT0009FF00[13];
	MT_DI000XFF00 DT000AFF00[13];
	MT_DI000XFF00 DT000B0000[13];													//add 周期组合有功总累计用电量
	MT_DI008F0000_ARRAY DT008F0000[13];
	MT_DI009001FF DT009001FF[13];															//add 剩余电量透支电量
	MT_DI009002FF DT009002FF[13];															//add 剩余金额透支金额
	MT_DI_ENERGY_PHASE_X_ARRAY DT_CUR_ENERGY_PHASE_A[13];
	MT_DI_ENERGY_PHASE_X_ARRAY DT_CUR_ENERGY_PHASE_B[13];
	MT_DI_ENERGY_PHASE_X_ARRAY DT_CUR_ENERGY_PHASE_C[13];
	MT_DI010XFF00 DT0101FF00[13];																	//需量
	MT_DI010XFF00 DT0102FF00[13];
	MT_DI010XFF00 DT0103FF00[13];
	MT_DI010XFF00 DT0104FF00[13];
	MT_DI010XFF00 DT0105FF00[13];
	MT_DI010XFF00 DT0106FF00[13];
	MT_DI010XFF00 DT0107FF00[13];
	MT_DI010XFF00 DT0108FF00[13];
	MT_DI010XFF00 DT0109FF00[13];
	MT_DI010XFF00 DT010AFF00[13];
	MT_DI_DEMAND_PHASE_X DT_CUR_DEMAND_PHASE_A[13];
	MT_DI_DEMAND_PHASE_X DT_CUR_DEMAND_PHASE_B[13];
	MT_DI_DEMAND_PHASE_X DT_CUR_DEMAND_PHASE_C[13];

	//变量
	MT_DI0201FF00 DT0201FF00;																						//变量
	MT_DI0202FF00 DT0202FF00;
	MT_DI0203FF00 DT0203FF00;
	MT_DI0204FF00 DT0204FF00;
	MT_DI0205FF00 DT0205FF00;
	MT_DI0206FF00 DT0206FF00;
	MT_DI0207FF00 DT0207FF00;
	MT_DI0208FF00 DT0208FF00;
	MT_DI0209FF00 DT0209FF00;
	MT_DI020A0XFF DT020A01FF;
	MT_DI020A0XFF DT020A02FF;
	MT_DI020A0XFF DT020A03FF;
	MT_DI020B0XFF DT020B01FF;
	MT_DI020B0XFF DT020B02FF;
	MT_DI020B0XFF DT020B03FF;
	MT_DI028000FF DT028000FF;

	//全失压
	MT_DI03050000 DT03050000; 																							  //全失压
	MT_DI0305000X DT03050001[10];

	//辅助电源失电总次数
	MT_DI03060000 DT03060000; 																							  //辅助电源失电
	MT_DI0306000X DT03060001[10];

	//掉电总次数
	MT_DI03110000 DT03110000;																								//掉电记录
	MT_DI0311000X DT03110001[10];

	//编程总次数
	MT_DI03300000 DT03300000;																								//编程记录
	MT_DI0330000X DT03300001[10];

	//需量清零总次数
	MT_DI03300200 DT03300200;																								//需量清0记录
	MT_DI0330020X DT03300201[10];

	//事件清零总次数
	MT_DI03300300 DT03300300;																								//事件清0记录
	MT_DI0330030X DT03300301[10];

	//校时总次数
	MT_DI03300400 DT03300400;																								//校时记录
	MT_DI0330040X DT03300401[10];

	//时段表编程总次数
	MT_DI03300500 DT03300500;																								//时段表记录
	MT_DI0330050X DT03300501[10];

	//时区表编程总次数
	MT_DI03300600 DT03300600;																								//时区表记录
	MT_DI0330060X DT03300601[10];

	//周休日编程总次数
	MT_DI03300700 DT03300700;																								//周休日记录
	MT_DI0330070X DT03300701[10];

	//节假日编程总次数
	MT_DI03300800 DT03300800;																								//节假日记录
	MT_DI0330080X DT03300801[10];

	//有功组合方式编程总次数
	MT_DI03300900 DT03300900;																								//有功组合方式记录
	MT_DI0330090X DT03300901[10];

	//无功组合方式1编程总次数
	MT_DI03300A00 DT03300A00;																								//无功组合方式1记录
	MT_DI03300A0X DT03300A01[10];

	//无功组合方式2编程总次数
	MT_DI03300B00 DT03300B00;																								//无功组合方式2记录
	MT_DI03300B0X DT03300B01[10];

	//结算日编程总次数
	MT_DI03300C00 DT03300C00;																								//结算日记录
	MT_DI03300C0X DT03300C01[10];

	//开表盖总次数
	MT_DI03300D00 DT03300D00;																								//开表盖记录
	MT_DI03300D0X DT03300D01[10];

	//开端钮盖总次数
	MT_DI03300E00 DT03300E00;																								//开端钮盒记录
	MT_DI03300E0X DT03300E01[10];

	//定时冻结数据
	MT_DI0500000X DT05000001[12];
	MT_DI000XFF00 DT05000101[12];
	MT_DI000XFF00 DT05000201[12];
	MT_DI000XFF00 DT05000301[12];
	MT_DI000XFF00 DT05000401[12];
	MT_DI000XFF00 DT05000501[12];
	MT_DI000XFF00 DT05000601[12];
	MT_DI000XFF00 DT05000701[12];
	MT_DI000XFF00 DT05000801[12];
	MT_DI010XFF00 DT05000901[12];
	MT_DI010XFF00 DT05000A01[12];
	MT_DI0500100X DT05001001[12];

	//瞬时冻结
	MT_DI0500000X DT05010001[3];
	MT_DI000XFF00 DT05010101[3];
	MT_DI000XFF00 DT05010201[3];
	MT_DI000XFF00 DT05010301[3];
	MT_DI000XFF00 DT05010401[3];
	MT_DI000XFF00 DT05010501[3];
	MT_DI000XFF00 DT05010601[3];
	MT_DI000XFF00 DT05010701[3];
	MT_DI000XFF00 DT05010801[3];
	MT_DI010XFF00 DT05010901[3];
	MT_DI010XFF00 DT05010A01[3];
	MT_DI0500100X DT05011001[3];

	//时区表切换冻结
	MT_DI0500000X DT05020001[2];
	MT_DI000XFF00 DT05020101[2];
	MT_DI000XFF00 DT05020201[2];
	MT_DI000XFF00 DT05020301[2];
	MT_DI000XFF00 DT05020401[2];
	MT_DI000XFF00 DT05020501[2];
	MT_DI000XFF00 DT05020601[2];
	MT_DI000XFF00 DT05020701[2];
	MT_DI000XFF00 DT05020801[2];
	MT_DI010XFF00 DT05020901[2];
	MT_DI010XFF00 DT05020A01[2];
	MT_DI0500100X DT05021001[2];

	//时段表切换冻结
	MT_DI0500000X DT05030001[2];
	MT_DI000XFF00 DT05030101[2];
	MT_DI000XFF00 DT05030201[2];
	MT_DI000XFF00 DT05030301[2];
	MT_DI000XFF00 DT05030401[2];
	MT_DI000XFF00 DT05030501[2];
	MT_DI000XFF00 DT05030601[2];
	MT_DI000XFF00 DT05030701[2];
	MT_DI000XFF00 DT05030801[2];
	MT_DI010XFF00 DT05030901[2];
	MT_DI010XFF00 DT05030A01[2];
	MT_DI0500100X DT05031001[2];

	//日冻结
	MT_DI0500000X DT05060001[31];
	MT_DI000XFF00 DT05060101[31];
	MT_DI000XFF00 DT05060201[31];
	MT_DI000XFF00 DT05060301[31];
	MT_DI000XFF00 DT05060401[31];
	MT_DI000XFF00 DT05060501[31];
	MT_DI000XFF00 DT05060601[31];
	MT_DI000XFF00 DT05060701[31];
	MT_DI000XFF00 DT05060801[31];
	MT_DI010XFF00 DT05060901[31];
	MT_DI010XFF00 DT05060A01[31];
	MT_DI0500100X DT05061001[31];
	//end 冻结数据

	//事件记录
	//10 失压
	MT_DI100000FF DT100000FF;																								//失压	//备案文件修改的事件
	MT_DI1000FF01 DT1000FF01;
	MT_DI100X00FF DT100X00FF[3];
	MT_DI100XFF0X DT100XFF01[3][10];
	//11 欠压
	MT_DI100X00FF DT110X00FF[3];																						   //欠压
	MT_DI100XFF0X DT110XFF01[3][10];
	//13 断相
	MT_DI100X00FF DT130X00FF[3];																						   //断相
	MT_DI100XFF0X DT130XFF01[3][10];
	//14 电压逆向序
	MT_DI100X00FF DT140000FF;   																						//电压逆相序
	MT_DI1400FF0X DT1400FF01[10];
	//18 失流
	MT_DI100X00FF DT180X00FF[3];																						   //失流
	MT_DI180XFF0X DT180XFF01[3][10];
	//1A 断流
	MT_DI100X00FF DT1A0X00FF[3];																						   //过流
	MT_DI180XFF0X DT1A0XFF01[3][10];

	//12 过压
	//MT_DI100X00FF 								DT120X00FF[3];																						   //过压
	//MT_DI100XFF0X 								DT120XFF01[3];
	//15 电流逆向序
	//MT_DI100X00FF 								DT150000FF;   																						//电流逆相序
	//MT_DI1400FF0X 								DT1500FF01;
	//16 电压不平衡
	//MT_DI100X00FF								DT160000FF;
	//MT_DI1600FF0X								DT1600FF01;
	//17 电流不平衡
	//MT_DI100X00FF								DT170000FF;
	//MT_DI1600FF0X								DT1700FF01;
	//19 过流
	//MT_DI100X00FF 								DT190X00FF[3];																						   //过流
	//MT_DI180XFF0X 								DT190XFF01[3];
	//1B 潮流反向
	//MT_DI100X00FF 								DT1B0X00FF[3];
	//MT_DI1400FF0X								DT1B0XFF01[3];
	//1C 过载
	//MT_DI100X00FF 								DT1C0X00FF[3];
	//MT_DI1400FF0X								DT1C0XFF01[3];
	//1D 跳闸
	//MT_DI1D000001								DT1D000001;
	//MT_DI1D00FF0X								DT1D0XFF01;
	//1E 合闸
	//1F 总功率因素超下限
	//MT_DI100X00FF 								DT1F0000FF;																										//功率因数超下限
	//MT_DI1F00FF0X 								DT1F00FF01;

	//以上为2007备案文件调整的内容

} FK_90F;

//end 交采数据
/*//需量清零
 typedef struct tag_FK_DT09F18
 {
 BYTE flag;
 BYTE who[4];
 } FK_DT91F18;

 //总清
 typedef struct tag_FK_DT09F19
 {
 BYTE flag;
 BYTE who[4];
 } FK_DT91F19;

 //事件清零
 typedef struct tag_FK_DT09F20
 {
 BYTE flag;
 BYTE who[4];
 BYTE di[4];
 } FK_DT91F20;
 */
//for 交采参数
typedef struct tag_FK_91F {
	MT_DI03300100 DT03300100;	//电表清0记录
	MT_DI0330010X DT03300101[10];

	EM_DI040001FF DT040001FF;
	EM_DI040002FF DT040002FF;
	EM_DI040003FF DT040003FF;
	EM_DI040004FF DT040004FF;
	EM_DI040005FF DT040005FF;
	EM_DI040006FF DT040006FF;
	EM_DI040007FF DT040007FF;
	EM_DI040008FF DT040008FF;
	EM_DI040009FF DT040009FF;
	EM_DI04000BFF DT04000BFF;
	EM_DI04000CFF DT04000CFF;
	EM_DI04000EFF DT04000EFF;
	EM_DI04000FFF DT04000FFF;
	EM_DI040010FF DT040010FF;
	EM_DI040011FF DT040011FF;
	EM_DI040012FF DT040012FF;
	EM_DI040013FF DT040013FF;
	EM_DI040014FF DT040014FF;
	EM_DI04010000 DT04010000;
	EM_DI040100FF DT040100FF;
	EM_DI04020000 DT04020000;
	EM_DI040200FF DT040200FF;
	EM_DI040300FF DT040300FF;
	EM_DI040401FF DT040401FF;
	EM_DI040402FF DT040402FF;
	EM_DI040501FF DT040501FF;
	EM_DI040502FF DT040502FF;
	EM_DI040600FF DT040600FF;
	EM_DI040601FF DT040601FF;
	EM_DI040602FF DT040602FF;
	EM_DI040603FF DT040603FF;

	EM_DI040901FF DT040901FF;
	EM_DI040902FF DT040902FF;
	EM_DI040903FF DT040903FF;
	EM_DI040904FF DT040904FF;
	EM_DI040905FF DT040905FF;
	EM_DI040906FF DT040906FF;
	EM_DI040907FF DT040907FF;
	EM_DI040908FF DT040908FF;
	EM_DI040909FF DT040909FF;
	EM_DI04090AFF DT04090AFF;
	EM_DI04090BFF DT04090BFF;
	EM_DI04090CFF DT04090CFF;
	EM_DI04090DFF DT04090DFF;
	EM_DI04090EFF DT04090EFF;
	EM_DI04090FFF DT04090FFF;

	EM_DI048000FF DT048000FF;

	EM_DI04820000 DT04820000;	//定时冻结参数

} FK_91F;
///////////////////////////////////////////////////

//end 交采参数

typedef struct tag_FK_DTGMET {
	BYTE day[4];
	FK_DATAMASK19 dmTime;
} FK_DTGMET;

#ifdef __cplusplus
}
#endif

#endif //FK_PROTOCOL_H__
