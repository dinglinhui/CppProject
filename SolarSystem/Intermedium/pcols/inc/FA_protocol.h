/*
 * FA_protocol.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef INTERMEDIUM_PCOLS_INC_FA_PROTOCOL_H_
#define INTERMEDIUM_PCOLS_INC_FA_PROTOCOL_H_

#include "OSDefs.h"

#include "EM_protocol.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifndef FA_VER
#define FA_VER 0x1000
#endif // FA_VER

/******************* 预定义(根据硬件)参数 ********************/
//电能量最大值
#ifndef  LINYANG_FA_MAX_ENERGY
#define  LINYANG_FA_MAX_ENERGY				1000000.00
#endif

//最大时段
#ifndef	 LINYANG_FA_MAX_TIMEMASK
#define  LINYANG_FA_MAX_TIMEMASK			8
#endif

//最大任务数
#ifndef	 LINYANG_FA_MAX_TASK
#define  LINYANG_FA_MAX_TASK				64
#endif

//最大输入开关量数
#ifndef	 LINYANG_FA_MAX_YX
#define  LINYANG_FA_MAX_YX					6
#endif

//最大输出开关量数
#ifndef	 LINYANG_FA_MAX_YK
#define  LINYANG_FA_MAX_YK					4
#endif

//最大模拟量数
#ifndef	 LINYANG_FA_MAX_ML
#define  LINYANG_FA_MAX_ML					4
#endif

//最大脉冲量数
#ifndef	 LINYANG_FA_MAX_MC
#define  LINYANG_FA_MAX_MC					4
#endif

//最大输入信号量
#ifndef	 LINYANG_FA_MAX_SEMA
#define  LINYANG_FA_MAX_SEMA				4
#endif

//最大串行通讯端口数
#ifndef	 LINYANG_FA_MAX_UART
#define  LINYANG_FA_MAX_UART				5
#endif

//最大测量点数
#ifndef	 LINYANG_FA_MAX_MEASURED_POINT
#define  LINYANG_FA_MAX_MEASURED_POINT		9
#endif

//最大运算量数目
#ifndef	 LINYANG_FA_MAX_SUM
#define  LINYANG_FA_MAX_SUM					8
#endif

//最大差动量数目
#ifndef	 LINYANG_FA_MAX_CHECK
#define  LINYANG_FA_MAX_CHECK				8
#endif

//最大心跳方案数目
#ifndef	 LINYANG_FA_MAX_HEARTBEAT
#define  LINYANG_FA_MAX_HEARTBEAT			8
#endif

// 最大事件数量
#ifndef LINYANG_FA_MAX_EVENTNUMS
#define LINYANG_FA_MAX_EVENTNUMS			512
#endif

//2.0
//最大谐波次数
#ifndef	LINYANG_FA_MAX_HARMONIC
#define LINYANG_FA_MAX_HARMONIC				18
#endif

#ifndef FA_LINYANG_MAX_WARNNUMS
#define FA_LINYANG_MAX_WARNNUMS				32
#endif

/******************* 预定义(根据硬件)参数结束 ********************/

//最大任务数容量
#if	LINYANG_FA_MAX_TASK > 128
#define LINYANG_FA_MAX_TASK_BUFF  253
#elif	LINYANG_FA_MAX_TASK > 64
#define LINYANG_FA_MAX_TASK_BUFF  128
#elif	LINYANG_FA_MAX_TASK > 32
#define LINYANG_FA_MAX_TASK_BUFF  64
#elif	LINYANG_FA_MAX_TASK > 16
#define LINYANG_FA_MAX_TASK_BUFF  32
#elif	LINYANG_FA_MAX_TASK > 8
#define LINYANG_FA_MAX_TASK_BUFF  16
#elif	LINYANG_FA_MAX_TASK > 4
#define LINYANG_FA_MAX_TASK_BUFF  8
#elif	LINYANG_FA_MAX_TASK > 2
#define LINYANG_FA_MAX_TASK_BUFF  4
#else//	LINYANG_FA_MAX_TASK > 0
#define LINYANG_FA_MAX_TASK_BUFF  2
#endif

#ifdef LINYANG_FA_MAX_TASK_BUFF
#define LINYANG_FA_COMMON_TASK_MAX_COUNT	48
#define LINYANG_FA_RELAY_TASK_MAX_COUNT		32
#define LINYANG_FA_EVENT_TASK_MAX_COUNT		48

#endif

//最大输入开关量容量
#if	LINYANG_FA_MAX_YX > 8
#define LINYANG_FA_MAX_YX_BUFF  15
#elif	LINYANG_FA_MAX_YX > 4
#define LINYANG_FA_MAX_YX_BUFF  8
#elif	LINYANG_FA_MAX_YX > 2
#define LINYANG_FA_MAX_YX_BUFF  4
#else//	LINYANG_FA_MAX_YX > 0
#define LINYANG_FA_MAX_YX_BUFF  2
#endif

//最大输入信号量容量
#if	LINYANG_FA_MAX_SEMA > 8
#define LINYANG_FA_MAX_SEMA_BUFF  15
#elif	LINYANG_FA_MAX_SEMA > 4
#define LINYANG_FA_MAX_SEMA_BUFF  8
#elif	LINYANG_FA_MAX_SEMA > 2
#define LINYANG_FA_MAX_SEMA_BUFF  4
#else//	LINYANG_FA_MAX_SEMA > 0
#define LINYANG_FA_MAX_SEMA_BUFF  2
#endif

//最大输出开关量容量
#if	LINYANG_FA_MAX_YK > 8
#define LINYANG_FA_MAX_YK_BUFF  15
#elif	LINYANG_FA_MAX_YK > 4
#define LINYANG_FA_MAX_YK_BUFF  8
#elif	LINYANG_FA_MAX_YK > 2
#define LINYANG_FA_MAX_YK_BUFF  4
#else//	LINYANG_FA_MAX_YK > 0
#define LINYANG_FA_MAX_YK_BUFF  2
#endif

//最大脉冲量数容量
#if	LINYANG_FA_MAX_MC > 128
#define LINYANG_FA_MAX_MC_BUFF  253
#elif	LINYANG_FA_MAX_MC > 64
#define LINYANG_FA_MAX_MC_BUFF  128
#elif	LINYANG_FA_MAX_MC > 32
#define LINYANG_FA_MAX_MC_BUFF  64
#elif	LINYANG_FA_MAX_MC > 16
#define LINYANG_FA_MAX_MC_BUFF  32
#elif	LINYANG_FA_MAX_MC > 8
#define LINYANG_FA_MAX_MC_BUFF  16
#elif	LINYANG_FA_MAX_MC > 4
#define LINYANG_FA_MAX_MC_BUFF  8
#elif	LINYANG_FA_MAX_MC > 2
#define LINYANG_FA_MAX_MC_BUFF  4
#else//	LINYANG_FA_MAX_MC > 0
#define LINYANG_FA_MAX_MC_BUFF  2
#endif

//最大脉冲量数容量
#if	LINYANG_FA_MAX_ML > 128
#define LINYANG_FA_MAX_ML_BUFF  253
#elif	LINYANG_FA_MAX_ML > 64
#define LINYANG_FA_MAX_ML_BUFF  128
#elif	LINYANG_FA_MAX_ML > 32
#define LINYANG_FA_MAX_ML_BUFF  64
#elif	LINYANG_FA_MAX_ML > 16
#define LINYANG_FA_MAX_ML_BUFF  32
#elif	LINYANG_FA_MAX_ML > 8
#define LINYANG_FA_MAX_ML_BUFF  16
#elif	LINYANG_FA_MAX_ML > 4
#define LINYANG_FA_MAX_ML_BUFF  8
#elif	LINYANG_FA_MAX_ML > 2
#define LINYANG_FA_MAX_ML_BUFF  4
#else//	LINYANG_FA_MAX_ML > 0
#define LINYANG_FA_MAX_ML_BUFF  2
#endif

//最大运算量容量
#if	LINYANG_FA_MAX_SUM > 128
#define LINYANG_FA_MAX_SUM_BUFF  253
#elif	LINYANG_FA_MAX_SUM > 64
#define LINYANG_FA_MAX_SUM_BUFF  128
#elif	LINYANG_FA_MAX_SUM > 32
#define LINYANG_FA_MAX_SUM_BUFF  64
#elif	LINYANG_FA_MAX_SUM > 16
#define LINYANG_FA_MAX_SUM_BUFF  32
#elif	LINYANG_FA_MAX_SUM > 8
#define LINYANG_FA_MAX_SUM_BUFF  16
#elif	LINYANG_FA_MAX_SUM > 4
#define LINYANG_FA_MAX_SUM_BUFF  8
#elif	LINYANG_FA_MAX_SUM > 2
#define LINYANG_FA_MAX_SUM_BUFF  4
#else//	LINYANG_FA_MAX_SUM > 0
#define LINYANG_FA_MAX_SUM_BUFF  2
#endif

//最大差动量数目容量
#if	LINYANG_FA_MAX_CHECK > 128
#define LINYANG_FA_MAX_CHECK_BUFF  253
#elif	LINYANG_FA_MAX_CHECK > 64
#define LINYANG_FA_MAX_CHECK_BUFF  128
#elif	LINYANG_FA_MAX_CHECK > 32
#define LINYANG_FA_MAX_CHECK_BUFF  64
#elif	LINYANG_FA_MAX_CHECK > 16
#define LINYANG_FA_MAX_CHECK_BUFF  32
#elif	LINYANG_FA_MAX_CHECK > 8
#define LINYANG_FA_MAX_CHECK_BUFF  16
#elif	LINYANG_FA_MAX_CHECK > 4
#define LINYANG_FA_MAX_CHECK_BUFF  8
#elif	LINYANG_FA_MAX_CHECK > 2
#define LINYANG_FA_MAX_CHECK_BUFF  4
#else//	LINYANG_FA_MAX_CHECK > 0
#define LINYANG_FA_MAX_CHECK_BUFF  2
#endif

#define  LINYANG_FA_MAX_TIMEMASK_BUFF LINYANG_FA_MAX_TIMEMASK

//最大心跳数目容量
#if	LINYANG_FA_MAX_HEARTBEAT > 8
#define LINYANG_FA_MAX_HEARTBEAT_BUFF  14
#elif	LINYANG_FA_MAX_HEARTBEAT      > 4
#define LINYANG_FA_MAX_HEARTBEAT_BUFF  8
#elif	LINYANG_FA_MAX_HEARTBEAT      > 2
#define LINYANG_FA_MAX_HEARTBEAT_BUFF  4
#else//	LINYANG_FA_MAX_HEARTBEAT      > 0
#define LINYANG_FA_MAX_HEARTBEAT_BUFF  2
#endif

//最大交流采样历史月数
#define LINYANG_DLT645_2007_MAX_HISMON_BUFF  12
#define LINYANG_DLT645_2007_MAX_TOTAL_MON_BUFF  (LINYANG_DLT645_2007_MAX_HISMON_BUFF+1)
//最大交流采样日冻结次数
#define LINYANG_FA_MAX_HISDAY_BUFF  14

//最大交流采样事件记录总数
#define LINYANG_FA_MAX_RECORD_BUFF  10

#define LINYANG_FA_MAX_RESETALL_RECORD_BUFF  10 //总清记录

//主站地址与命令序列
typedef union tag_FA_MSTASEQ {
	WORD body;
	struct {
		WORD MSTA :6;					//主站地址
		WORD FSEQ :7;					//帧序号
		WORD ISEQ :3;					//帧内序号
	} body_mask;
} FA_MSTASEQ;

typedef enum tag_FA_MSTA {
	FA_MSTA_SMS = 31,
	FA_MSTA_GPRS = 32,
	FA_MSTA_DTMF = 33,
	FA_MSTA_CSD = 34,
	FA_MSTA_Ethernet = 35,
	FA_MSTA_SPACE1 = 36,
	FA_MSTA_SPACE2 = 37,
	FA_MSTA_SPACE3 = 38,
	FA_MSTA_SPACE4 = 39,
	FA_MSTA_SPACE5 = 40,
	FA_MSTA_SPACE6 = 41,
	FA_MSTA_SPACE7 = 42,
	FA_MSTA_SPACE8 = 43,
	FA_MSTA_SPACE9 = 44,
	FA_MSTA_SPACE10 = 45,
	FA_MSTA_SPACE11 = 46,
	FA_MSTA_SPACE12 = 47,
	FA_MSTA_SPACE13 = 48,
	FA_MSTA_SPACE14 = 49
} FA_MSTA;

//控制码C
typedef union tag_FA_C {
	BYTE body;
	struct {
		BYTE cmd :6;					//功能码
		BYTE ACD :1;					//异常标志
		BYTE DIR :1;					//传输方向
	} body_mask;
} FA_C;

//功能码
typedef enum tag_FA_FUNCION {
	FA_CMD_RELAX = 0x00,				//读中继
	FA_CMD_READDATA = 0x01,				//读当前数据
	FA_CMD_READSESSION = 0x02,				//读任务数据
	FA_CMD_READLOG = 0x04,				//读日志
	FA_CMD_RTWRITEPARA = 0x07,				//实时写对象参数
	FA_CMD_WRITEPARA = 0x08,				//写对象参数
	FA_CMD_FAIL = 0x09,				//异常告警
	FA_CMD_FAIL_ACK = 0x0A,				//告警确认
	FA_CMD_USER_CMD = 0x0F,				//用户自定义
	FA_CMD_QUERYENERGY = 0x17,				//集中器抄读终端日冻结电量
	FA_CMD_CONNECTSLAVE = 0x18,				//连接到从设备
	FA_CMD_QUERYSLAVE = 0x28,				//查询从设备有无上报数据
	FA_CMD_LOGIN = 0x21,				//登录
	FA_CMD_LOGOUT = 0x22,				//登录退出
	FA_CMD_HEARTBEAT = 0x24,				//心跳检测
	FA_CMD_SMSASK = 0x28,				//请求短信发送
	FA_CMD_SMSACK = 0x29				//收到短信上报
} FA_FUNCION;

// 终端地址
typedef struct tag_FA_ADDRESS {
	BYTE A1;
	BYTE A2;
	BYTE B1;
	BYTE B2;
} FA_ADDRESS;

// 错误编码
//
typedef enum tag_FA_ERROR_CODE {
	FA_OK = 0x00,			//正确，无错误
	FA_NORELAY = 0x01,			//中继命令没有返回
	FA_INVALID = 0x02,			//设置内容非法
	FA_NOENTIRE = 0x03,			//密码权限不足
	FA_SPACE = 0x04,			//无此项数据
	FA_INVALIDTIME = 0x05,                   //命令时间无效
	FA_NOADDR = 0x11,			//目标地址不存在
	FA_SENDERR = 0x12,			//发送失败
	FA_MOREFAME = 0x13			//短消息帧太长
} FA_ERROR_CODE;

//
// 供电电压编码
//
typedef enum tag_FA_VOLT_CODE {
	FA_100V = 'B',			// 110V
	FA_220V = 'C',			// 220V
	FA_380V = 'D',			// 380V
	FA_6kV = 'F',			// 6KV
	FA_10V = 'G',			// 10Kv
	FA_35kV = 'H',			// 35kV
	FA_110kV = '1',			// 110kV
	FA_220kV = '2',			// 220kV
	FA_500kV = '5'	      // 500kV
} FA_VOLT_CODE;

//数据单元标识
typedef union tag_FA_DI {
	WORD body;
	struct {
		WORD DI0 :8;
		WORD DI1 :8;
	} body_mask;
} FA_DI;

typedef struct tag_FA_UNIT {
	BYTE nItem;
	const char * Name;
} FA_UINT;

static const FA_UINT FA_UNIT_DEFINE[] = {
		//nItem,	Nmae
		{ 0, nullptr }, { 1, "V" }, { 2, "A" }, { 3, "kV" }, { 4, "kA" },
		{ 5, "kW" }, { 6, "kvar" }, { 7, "kWh" }, { 8, "kvarh" }, { 9, "%" }, {
				10, "分" }, { 11, "秒" }, { 12, "BIT/S" }, { 13, "kVA" }, { 14,
				"度" }, { 15, "MDHM" }, };

typedef struct tag_FA_DATA_DEFINE {
	WORD DI;							//标识编码
	BYTE Plus;						//编码范围(增量值,内部使用)

	union {
		BYTE body;					//隐码
		struct {
			BYTE LA :1;		//低级权限
			BYTE AA :1;		//高级权限
			BYTE READ :1;		//读功能
			BYTE WRITE :1;		//写功能
			BYTE unit :4;		//单位，具体见 FA_UNIT_DEFINE 的定义
		} body_mask;
	} Access;
	WORD Length;						//数据格式长度(归约定义的长度)
	//
	void *DataPtr;					//数据体
	WORD nDataLength;				//数据体长度(内部定义的数据结构长度)
	//
	const char *Name;					//数据项名称
} FA_DATA_DEFINE, FA_DATA, *PFA_DATA;

//
// 测量点数据
//

typedef struct tag_FA_DI9010 {
	double value;
} FA_DI9010;

typedef struct tag_FA_DI9011 {
	double value;
} FA_DI9011;

typedef struct tag_FA_DI9012 {
	double value;
} FA_DI9012;

typedef struct tag_FA_DI9013 {
	double value;
} FA_DI9013;

typedef struct tag_FA_DI9014 {
	double value;
} FA_DI9014;

typedef struct tag_FA_DI901F {
	FA_DI9010 DI9010;
	FA_DI9011 DI9011;
	FA_DI9012 DI9012;
	FA_DI9013 DI9013;
	FA_DI9014 DI9014;
} FA_DI901F;

typedef struct tag_FA_DI9020 {
	double value;
} FA_DI9020;

typedef struct tag_FA_DI9021 {
	double value;
} FA_DI9021;

typedef struct tag_FA_DI9022 {
	double value;
} FA_DI9022;

typedef struct tag_FA_DI9023 {
	double value;
} FA_DI9023;

typedef struct tag_FA_DI9024 {
	double value;
} FA_DI9024;

typedef struct tag_FA_DI902F {
	FA_DI9020 DI9020;
	FA_DI9021 DI9021;
	FA_DI9022 DI9022;
	FA_DI9023 DI9023;
	FA_DI9024 DI9024;
} FA_DI902F;

typedef struct tag_FA_DI9070 {
	double value;
} FA_DI9070;

typedef struct tag_FA_DI9071 {
	double value;
} FA_DI9071;

typedef struct tag_FA_DI9072 {
	double value;
} FA_DI9072;

typedef struct tag_FA_DI9073 {
	double value;
} FA_DI9073;

typedef struct tag_FA_DI907F {
	FA_DI9070 DI9070;
	FA_DI9071 DI9071;
	FA_DI9072 DI9072;
	FA_DI9073 DI9073;
} FA_DI907F;

typedef struct tag_FA_DI9080 {
	double value;
} FA_DI9080;

typedef struct tag_FA_DI9081 {
	double value;
} FA_DI9081;

typedef struct tag_FA_DI9082 {
	double value;
} FA_DI9082;

typedef struct tag_FA_DI9083 {
	double value;
} FA_DI9083;

typedef struct tag_FA_DI908F {
	FA_DI9080 DI9080;
	FA_DI9081 DI9081;
	FA_DI9082 DI9082;
	FA_DI9083 DI9083;
} FA_DI908F;

typedef struct tag_FA_DI9130 {
	double value;
} FA_DI9130;

typedef struct tag_FA_DI9131 {
	double value;
} FA_DI9131;

typedef struct tag_FA_DI9132 {
	double value;
} FA_DI9132;

typedef struct tag_FA_DI9133 {
	double value;
} FA_DI9133;

typedef struct tag_FA_DI9134 {
	double value;
} FA_DI9134;

typedef struct tag_FA_DI913F {
	FA_DI9130 DI9130;
	FA_DI9131 DI9131;
	FA_DI9132 DI9132;
	FA_DI9133 DI9133;
	FA_DI9134 DI9134;
} FA_DI913F;

//061205正反向无功总电能
typedef struct tag_FA_DI9110 {
	double value;
} FA_DI9110;

typedef struct tag_FA_DI9111 {
	double value;
} FA_DI9111;

typedef struct tag_FA_DI9112 {
	double value;
} FA_DI9112;

typedef struct tag_FA_DI9113 {
	double value;
} FA_DI9113;

typedef struct tag_FA_DI9114 {
	double value;
} FA_DI9114;

typedef struct tag_FA_DI911F {
	FA_DI9110 DI9110;
	FA_DI9111 DI9111;
	FA_DI9112 DI9112;
	FA_DI9113 DI9113;
	FA_DI9114 DI9114;
} FA_DI911F;
typedef struct tag_FA_DI9120 {
	double value;
} FA_DI9120;

typedef struct tag_FA_DI9121 {
	double value;
} FA_DI9121;

typedef struct tag_FA_DI9122 {
	double value;
} FA_DI9122;

typedef struct tag_FA_DI9123 {
	double value;
} FA_DI9123;

typedef struct tag_FA_DI9124 {
	double value;
} FA_DI9124;

typedef struct tag_FA_DI912F {
	FA_DI9120 DI9120;
	FA_DI9121 DI9121;
	FA_DI9122 DI9122;
	FA_DI9123 DI9123;
	FA_DI9124 DI9124;
} FA_DI912F;
//

typedef struct tag_FA_DI9140 {
	double value;
} FA_DI9140;

typedef struct tag_FA_DI9141 {
	double value;
} FA_DI9141;

typedef struct tag_FA_DI9142 {
	double value;
} FA_DI9142;

typedef struct tag_FA_DI9143 {
	double value;
} FA_DI9143;

typedef struct tag_FA_DI9144 {
	double value;
} FA_DI9144;

typedef struct tag_FA_DI914F {
	FA_DI9140 DI9140;
	FA_DI9141 DI9141;
	FA_DI9142 DI9142;
	FA_DI9143 DI9143;
	FA_DI9144 DI9144;
} FA_DI914F;

typedef struct tag_FA_DI9150 {
	double value;
} FA_DI9150;

typedef struct tag_FA_DI9151 {
	double value;
} FA_DI9151;

typedef struct tag_FA_DI9152 {
	double value;
} FA_DI9152;

typedef struct tag_FA_DI9153 {
	double value;
} FA_DI9153;

typedef struct tag_FA_DI9154 {
	double value;
} FA_DI9154;

typedef struct tag_FA_DI915F {
	FA_DI9150 DI9150;
	FA_DI9151 DI9151;
	FA_DI9152 DI9152;
	FA_DI9153 DI9153;
	FA_DI9154 DI9154;
} FA_DI915F;

typedef struct tag_FA_DI9160 {
	double value;
} FA_DI9160;

typedef struct tag_FA_DI9161 {
	double value;
} FA_DI9161;

typedef struct tag_FA_DI9162 {
	double value;
} FA_DI9162;

typedef struct tag_FA_DI9163 {
	double value;
} FA_DI9163;

typedef struct tag_FA_DI9164 {
	double value;
} FA_DI9164;

typedef struct tag_FA_DI916F {
	FA_DI9160 DI9160;
	FA_DI9161 DI9161;
	FA_DI9162 DI9162;
	FA_DI9163 DI9163;
	FA_DI9164 DI9164;
} FA_DI916F;

typedef struct tag_FA_DI9170 {
	double value;
} FA_DI9170;

typedef struct tag_FA_DI9171 {
	double value;
} FA_DI9171;

typedef struct tag_FA_DI9172 {
	double value;
} FA_DI9172;

//#if (defined(_GUANGDONG_FOSHAN) || defined(_GUANGDONG_SHANTOU))
typedef struct tag_FA_DI9173 {
	double value;
} FA_DI9173;
//#endif

typedef struct tag_FA_DI917F {
	FA_DI9170 DI9170;
	FA_DI9171 DI9171;
	FA_DI9172 DI9172;
	FA_DI9173 DI9173;
} FA_DI917F;

typedef struct tag_FA_DI9180 {
	double value;
} FA_DI9180;

typedef struct tag_FA_DI9181 {
	double value;
} FA_DI9181;

typedef struct tag_FA_DI9182 {
	double value;
} FA_DI9182;

typedef struct tag_FA_DI9183 {
	double value;
} FA_DI9183;

typedef struct tag_FA_DI918F {
	FA_DI9180 DI9180;
	FA_DI9181 DI9181;
	FA_DI9182 DI9182;
	FA_DI9183 DI9183;
} FA_DI918F;

typedef struct tag_FA_DI9210 {
	double value;
} FA_DI9210;

typedef struct tag_FA_DI9220 {
	double value;
} FA_DI9220;

typedef struct tag_FA_DI9270 {
	double value;
} FA_DI9270;

typedef struct tag_FA_DI9271 {
	double value;
} FA_DI9271;

typedef struct tag_FA_DI9272 {
	double value;
} FA_DI9272;

typedef struct tag_FA_DI927F {
	FA_DI9270 DI9270;
	FA_DI9271 DI9271;
	FA_DI9272 DI9272;
} FA_DI927F;

typedef struct tag_FA_DI9280 {
	double value;
} FA_DI9280;

typedef struct tag_FA_DI9281 {
	double value;
} FA_DI9281;

typedef struct tag_FA_DI9282 {
	double value;
} FA_DI9282;

typedef struct tag_FA_DI928F {
	FA_DI9280 DI9280;
	FA_DI9281 DI9281;
	FA_DI9282 DI9282;
} FA_DI928F;

typedef struct tag_FA_DI9310 {
	double value;
} FA_DI9310;

typedef struct tag_FA_DI9320 {
	double value;
} FA_DI9320;

typedef struct tag_FA_DI9330 {
	double value;
} FA_DI9330;

typedef struct tag_FA_DI9340 {
	double value;
} FA_DI9340;

typedef struct tag_FA_DI9350 {
	double value;
} FA_DI9350;

typedef struct tag_FA_DI9360 {
	double value;
} FA_DI9360;

typedef struct tag_FA_DI9370 {
	double value;
} FA_DI9370;

typedef struct tag_FA_DI9371 {
	double value;
} FA_DI9371;

typedef struct tag_FA_DI9372 {
	double value;
} FA_DI9372;

typedef struct tag_FA_DI937F {
	FA_DI9370 DI9370;
	FA_DI9371 DI9371;
	FA_DI9372 DI9372;
} FA_DI937F;

typedef struct tag_FA_DI9380 {
	double value;
} FA_DI9380;

typedef struct tag_FA_DI9381 {
	double value;
} FA_DI9381;

typedef struct tag_FA_DI9382 {
	double value;
} FA_DI9382;

typedef struct tag_FA_DI938F {
	FA_DI9380 DI9380;
	FA_DI9381 DI9381;
	FA_DI9382 DI9382;
} FA_DI938F;

typedef struct tag_FA_DI9410 {
	double value;
} FA_DI9410;

typedef struct tag_FA_DI9411 {
	double value;
} FA_DI9411;

typedef struct tag_FA_DI9412 {
	double value;
} FA_DI9412;

typedef struct tag_FA_DI9413 {
	double value;
} FA_DI9413;

typedef struct tag_FA_DI9414 {
	double value;
} FA_DI9414;

typedef struct tag_FA_DI941F {
	FA_DI9410 DI9410;
	FA_DI9411 DI9411;
	FA_DI9412 DI9412;
	FA_DI9413 DI9413;
	FA_DI9414 DI9414;
} FA_DI941F;

typedef struct tag_FA_DI9420 {
	double value;
} FA_DI9420;

typedef struct tag_FA_DI9421 {
	double value;
} FA_DI9421;

typedef struct tag_FA_DI9422 {
	double value;
} FA_DI9422;

typedef struct tag_FA_DI9423 {
	double value;
} FA_DI9423;

typedef struct tag_FA_DI9424 {
	double value;
} FA_DI9424;

typedef struct tag_FA_DI942F {
	FA_DI9420 DI9420;
	FA_DI9421 DI9421;
	FA_DI9422 DI9422;
	FA_DI9423 DI9423;
	FA_DI9424 DI9424;
} FA_DI942F;

//
typedef struct tag_FA_DI9470 {
	double value;
} FA_DI9470;

typedef struct tag_FA_DI9471 {
	double value;
} FA_DI9471;

typedef struct tag_FA_DI9472 {
	double value;
} FA_DI9472;

typedef struct tag_FA_DI9473 {
	double value;
} FA_DI9473;

typedef struct tag_FA_DI947F {
	FA_DI9470 DI9470;
	FA_DI9471 DI9471;
	FA_DI9472 DI9472;
	FA_DI9473 DI9473;
} FA_DI947F;

typedef struct tag_FA_DI9480 {
	double value;
} FA_DI9480;

typedef struct tag_FA_DI9481 {
	double value;
} FA_DI9481;

typedef struct tag_FA_DI9482 {
	double value;
} FA_DI9482;

typedef struct tag_FA_DI9483 {
	double value;
} FA_DI9483;

typedef struct tag_FA_DI948F {
	FA_DI9480 DI9480;
	FA_DI9481 DI9481;
	FA_DI9482 DI9482;
	FA_DI9483 DI9483;
} FA_DI948F;

typedef struct tag_FA_DI9510 {
	double value;
} FA_DI9510;

typedef struct tag_FA_DI9511 {
	double value;
} FA_DI9511;

typedef struct tag_FA_DI9512 {
	double value;
} FA_DI9512;

typedef struct tag_FA_DI9513 {
	double value;
} FA_DI9513;

typedef struct tag_FA_DI9514 {
	double value;
} FA_DI9514;

typedef struct tag_FA_DI951F {
	FA_DI9510 DI9510;
	FA_DI9511 DI9511;
	FA_DI9512 DI9512;
	FA_DI9513 DI9513;
	FA_DI9514 DI9514;
} FA_DI951F;

typedef struct tag_FA_DI9520 {
	double value;
} FA_DI9520;

typedef struct tag_FA_DI9521 {
	double value;
} FA_DI9521;

typedef struct tag_FA_DI9522 {
	double value;
} FA_DI9522;

typedef struct tag_FA_DI9523 {
	double value;
} FA_DI9523;

typedef struct tag_FA_DI9524 {
	double value;
} FA_DI9524;

typedef struct tag_FA_DI952F {
	FA_DI9520 DI9520;
	FA_DI9521 DI9521;
	FA_DI9522 DI9522;
	FA_DI9523 DI9523;
	FA_DI9524 DI9524;
} FA_DI952F;

typedef struct tag_FA_DI9530 {
	double value;
} FA_DI9530;

typedef struct tag_FA_DI9531 {
	double value;
} FA_DI9531;

typedef struct tag_FA_DI9532 {
	double value;
} FA_DI9532;

typedef struct tag_FA_DI9533 {
	double value;
} FA_DI9533;

typedef struct tag_FA_DI9534 {
	double value;
} FA_DI9534;

typedef struct tag_FA_DI953F {
	FA_DI9530 DI9530;
	FA_DI9531 DI9531;
	FA_DI9532 DI9532;
	FA_DI9533 DI9533;
	FA_DI9534 DI9534;
} FA_DI953F;

typedef struct tag_FA_DI9540 {
	double value;
} FA_DI9540;

typedef struct tag_FA_DI9541 {
	double value;
} FA_DI9541;

typedef struct tag_FA_DI9542 {
	double value;
} FA_DI9542;

typedef struct tag_FA_DI9543 {
	double value;
} FA_DI9543;

typedef struct tag_FA_DI9544 {
	double value;
} FA_DI9544;

typedef struct tag_FA_DI954F {
	FA_DI9540 DI9540;
	FA_DI9541 DI9541;
	FA_DI9542 DI9542;
	FA_DI9543 DI9543;
	FA_DI9544 DI9544;
} FA_DI954F;

typedef struct tag_FA_DI9550 {
	double value;
} FA_DI9550;

typedef struct tag_FA_DI9551 {
	double value;
} FA_DI9551;

typedef struct tag_FA_DI9552 {
	double value;
} FA_DI9552;

typedef struct tag_FA_DI9553 {
	double value;
} FA_DI9553;

typedef struct tag_FA_DI9554 {
	double value;
} FA_DI9554;

typedef struct tag_FA_DI955F {
	FA_DI9550 DI9550;
	FA_DI9551 DI9551;
	FA_DI9552 DI9552;
	FA_DI9553 DI9553;
	FA_DI9554 DI9554;
} FA_DI955F;

typedef struct tag_FA_DI9560 {
	double value;
} FA_DI9560;

typedef struct tag_FA_DI9561 {
	double value;
} FA_DI9561;

typedef struct tag_FA_DI9562 {
	double value;
} FA_DI9562;

typedef struct tag_FA_DI9563 {
	double value;
} FA_DI9563;

typedef struct tag_FA_DI9564 {
	double value;
} FA_DI9564;

typedef struct tag_FA_DI956F {
	FA_DI9560 DI9560;
	FA_DI9561 DI9561;
	FA_DI9562 DI9562;
	FA_DI9563 DI9563;
	FA_DI9564 DI9564;
} FA_DI956F;

//
typedef struct tag_FA_DI9570 {
	double value;
} FA_DI9570;

typedef struct tag_FA_DI9571 {
	double value;
} FA_DI9571;

typedef struct tag_FA_DI9572 {
	double value;
} FA_DI9572;

typedef struct tag_FA_DI9573 {
	double value;
} FA_DI9573;

typedef struct tag_FA_DI957F {
	FA_DI9570 DI9570;
	FA_DI9571 DI9571;
	FA_DI9572 DI9572;
	FA_DI9573 DI9573;
} FA_DI957F;

typedef struct tag_FA_DI9580 {
	double value;
} FA_DI9580;

typedef struct tag_FA_DI9581 {
	double value;
} FA_DI9581;

typedef struct tag_FA_DI9582 {
	double value;
} FA_DI9582;

typedef struct tag_FA_DI9583 {
	double value;
} FA_DI9583;

typedef struct tag_FA_DI958F {
	FA_DI9580 DI9580;
	FA_DI9581 DI9581;
	FA_DI9582 DI9582;
	FA_DI9583 DI9583;
} FA_DI958F;

typedef struct tag_FA_DI95FF {
	FA_DI941F DI941F;
	FA_DI942F DI942F;
	FA_DI951F DI951F;
	FA_DI952F DI952F;
	FA_DI953F DI953F;
	FA_DI954F DI954F;
	FA_DI955F DI955F;
	FA_DI956F DI956F;
} FA_DI95FF;

typedef struct tag_FA_DIA010 {
	double value;
} FA_DIA010;

typedef struct tag_FA_DIA011 {
	double value;
} FA_DIA011;

typedef struct tag_FA_DIA012 {
	double value;
} FA_DIA012;

typedef struct tag_FA_DIA013 {
	double value;
} FA_DIA013;

typedef struct tag_FA_DIA014 {
	double value;
} FA_DIA014;

typedef struct tag_FA_DIA01F {
	FA_DIA010 DIA010;
	FA_DIA011 DIA011;
	FA_DIA012 DIA012;
	FA_DIA013 DIA013;
	FA_DIA014 DIA014;
} FA_DIA01F;

typedef struct tag_FA_DIA11F {
	FA_DIA010 DIA110;
	FA_DIA011 DIA111;
	FA_DIA012 DIA112;
	FA_DIA013 DIA113;
	FA_DIA014 DIA114;
} FA_DIA11F;

typedef struct tag_FA_DIA020 {
	double value;
} FA_DIA020;

typedef struct tag_FA_DIA021 {
	double value;
} FA_DIA021;

typedef struct tag_FA_DIA022 {
	double value;
} FA_DIA022;

typedef struct tag_FA_DIA023 {
	double value;
} FA_DIA023;

typedef struct tag_FA_DIA024 {
	double value;
} FA_DIA024;

typedef struct tag_FA_DIA02F {
	FA_DIA020 DIA020;
	FA_DIA021 DIA021;
	FA_DIA022 DIA022;
	FA_DIA023 DIA023;
	FA_DIA024 DIA024;
} FA_DIA02F;

typedef struct tag_FA_DIA12F {
	FA_DIA020 DIA120;
	FA_DIA020 DIA121;
	FA_DIA020 DIA122;
	FA_DIA020 DIA123;
	FA_DIA020 DIA124;
} FA_DIA12F;

typedef struct tag_FA_DIA410 {
	double value;
} FA_DIA410;

typedef struct tag_FA_DIA411 {
	double value;
} FA_DIA411;

typedef struct tag_FA_DIA412 {
	double value;
} FA_DIA412;

typedef struct tag_FA_DIA413 {
	double value;
} FA_DIA413;

typedef struct tag_FA_DIA414 {
	double value;
} FA_DIA414;

typedef struct tag_FA_DIA41F {
	FA_DIA410 DIA410;
	FA_DIA411 DIA411;
	FA_DIA412 DIA412;
	FA_DIA413 DIA413;
	FA_DIA414 DIA414;
} FA_DIA41F;

typedef struct tag_FA_DIA51F {
	FA_DIA410 DIA510;
	FA_DIA411 DIA511;
	FA_DIA412 DIA512;
	FA_DIA413 DIA513;
	FA_DIA414 DIA514;
} FA_DIA51F;

typedef struct tag_FA_DIA52F {
	FA_DIA410 DIA520;
	FA_DIA411 DIA521;
	FA_DIA412 DIA522;
	FA_DIA413 DIA523;
	FA_DIA414 DIA524;
} FA_DIA52F;

typedef struct tag_FA_DIA420 {
	double value;
} FA_DIA420;

typedef struct tag_FA_DIA421 {
	double value;
} FA_DIA421;

typedef struct tag_FA_DIA422 {
	double value;
} FA_DIA422;

typedef struct tag_FA_DIA423 {
	double value;
} FA_DIA423;

typedef struct tag_FA_DIA424 {
	double value;
} FA_DIA424;

typedef struct tag_FA_DIA42F {
	FA_DIA420 DIA420;
	FA_DIA421 DIA421;
	FA_DIA422 DIA422;
	FA_DIA423 DIA423;
	FA_DIA424 DIA424;
} FA_DIA42F;

typedef struct tag_FA_DIB010 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB010;

typedef struct tag_FA_DIB011 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB011;

typedef struct tag_FA_DIB012 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB012;

typedef struct tag_FA_DIB013 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB013;

typedef struct tag_FA_DIB014 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB014;

typedef struct tag_FA_DIB01F {
	FA_DIB010 DIB010;
	FA_DIB011 DIB011;
	FA_DIB012 DIB012;
	FA_DIB013 DIB013;
	FA_DIB014 DIB014;
} FA_DIB01F;

typedef struct tag_FA_DIB11F {
	FA_DIB010 DIB110;
	FA_DIB010 DIB111;
	FA_DIB010 DIB112;
	FA_DIB010 DIB113;
	FA_DIB010 DIB114;
} FA_DIB11F;

typedef struct tag_FA_DIB020 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB020;

typedef struct tag_FA_DIB021 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB021;

typedef struct tag_FA_DIB022 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB022;

typedef struct tag_FA_DIB023 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB023;

typedef struct tag_FA_DIB024 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB024;

typedef struct tag_FA_DIB02F {
	FA_DIB020 DIB020;
	FA_DIB021 DIB021;
	FA_DIB022 DIB022;
	FA_DIB023 DIB023;
	FA_DIB024 DIB024;
} FA_DIB02F;

typedef struct tag_FA_DIB12F {
	FA_DIB020 DIB120;
	FA_DIB021 DIB121;
	FA_DIB022 DIB122;
	FA_DIB023 DIB123;
	FA_DIB024 DIB124;
} FA_DIB12F;

typedef struct tag_FA_DIB410 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB410;

typedef struct tag_FA_DIB411 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB411;

typedef struct tag_FA_DIB412 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB412;

typedef struct tag_FA_DIB413 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB413;

typedef struct tag_FA_DIB414 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB414;

typedef struct tag_FA_DIB41F {
	FA_DIB410 DIB410;
	FA_DIB411 DIB411;
	FA_DIB412 DIB412;
	FA_DIB413 DIB413;
	FA_DIB414 DIB414;
} FA_DIB41F;

typedef struct tag_FA_DIB420 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB420;

typedef struct tag_FA_DIB421 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB421;

typedef struct tag_FA_DIB422 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB422;

typedef struct tag_FA_DIB423 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB423;

typedef struct tag_FA_DIB424 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB424;

typedef struct tag_FA_DIB42F {
	FA_DIB420 DIB420;
	FA_DIB421 DIB421;
	FA_DIB422 DIB422;
	FA_DIB423 DIB423;
	FA_DIB424 DIB424;
} FA_DIB42F;

typedef struct tag_FA_DIB51F {
	FA_DIB420 DIB510;
	FA_DIB421 DIB511;
	FA_DIB422 DIB512;
	FA_DIB423 DIB513;
	FA_DIB424 DIB514;
} FA_DIB51F;

typedef struct tag_FA_DIB52F {
	FA_DIB420 DIB520;
	FA_DIB421 DIB521;
	FA_DIB422 DIB522;
	FA_DIB423 DIB523;
	FA_DIB424 DIB524;
} FA_DIB52F;

typedef struct tag_FA_DIB210 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB210;

typedef struct tag_FA_DIB211 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB211;

typedef struct tag_FA_DIB212 {
	WORD count;
} FA_DIB212;

typedef struct tag_FA_DIB213 {
	WORD count;
} FA_DIB213;

typedef struct tag_FA_DIB214 {
	DWORD dwMins;
} FA_DIB214;

typedef struct tag_FA_DIB21F {
	FA_DIB210 DIB210;
	FA_DIB211 DIB211;
	FA_DIB212 DIB212;
	FA_DIB213 DIB213;
	FA_DIB214 DIB214;
} FA_DIB21F;

typedef struct tag_FA_DIB310 {
	WORD count;
} FA_DIB310;

typedef struct tag_FA_DIB311 {
	WORD count;
} FA_DIB311;

typedef struct tag_FA_DIB312 {
	WORD count;
} FA_DIB312;

typedef struct tag_FA_DIB313 {
	WORD count;
} FA_DIB313;

typedef struct tag_FA_DIB31F {
	FA_DIB310 DIB310;
	FA_DIB311 DIB311;
	FA_DIB312 DIB312;
	FA_DIB313 DIB313;
} FA_DIB31F;

typedef struct tag_FA_DIB320 {
	DWORD value;
} FA_DIB320;

typedef struct tag_FA_DIB321 {
	DWORD value;
} FA_DIB321;

typedef struct tag_FA_DIB322 {
	DWORD value;
} FA_DIB322;

typedef struct tag_FA_DIB323 {
	DWORD value;
} FA_DIB323;

typedef struct tag_FA_DIB32F {
	FA_DIB320 DIB320;
	FA_DIB321 DIB321;
	FA_DIB322 DIB322;
	FA_DIB323 DIB323;
} FA_DIB32F;

typedef struct tag_FA_DIB330 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB330;

typedef struct tag_FA_DIB331 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB331;

typedef struct tag_FA_DIB332 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB332;

typedef struct tag_FA_DIB333 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB333;

typedef struct tag_FA_DIB334 {
	double value;
} FA_DIB334;

typedef struct tag_FA_DIB335 {
	double value;
} FA_DIB335;

typedef struct tag_FA_DIB336 {
	double value;
} FA_DIB336;

typedef struct tag_FA_DIB33F {
	FA_DIB330 DIB330;
	FA_DIB331 DIB331;
	FA_DIB332 DIB332;
	FA_DIB333 DIB333;
	//FA_DIB334	DIB334;
	//FA_DIB335	DIB335;
	//FA_DIB336	DIB336;
} FA_DIB33F;

typedef struct tag_FA_DIB340 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB340;

typedef struct tag_FA_DIB341 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB341;

typedef struct tag_FA_DIB342 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB342;

typedef struct tag_FA_DIB343 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} FA_DIB343;

typedef struct tag_FA_DIB344 {
	double value;
} FA_DIB344;

typedef struct tag_FA_DIB345 {
	double value;
} FA_DIB345;

typedef struct tag_FA_DIB346 {
	double value;
} FA_DIB346;

typedef struct tag_FA_DIB34F {
	FA_DIB340 DIB340;
	FA_DIB341 DIB341;
	FA_DIB342 DIB342;
	FA_DIB343 DIB343;
	//FA_DIB344	DIB344;
	//FA_DIB345	DIB345;
	//FA_DIB346	DIB346;
} FA_DIB34F;

typedef struct tag_FA_DIB350 {
	DWORD value;
} FA_DIB350;

typedef struct tag_FA_DIB351 {
	DWORD value;
} FA_DIB351;

typedef struct tag_FA_DIB352 {
	DWORD value;
} FA_DIB352;

typedef struct tag_FA_DIB353 {
	double value;
} FA_DIB353;

typedef struct tag_FA_DIB354 {
	double value;
} FA_DIB354;

typedef struct tag_FA_DIB355 {
	double value;
} FA_DIB355;

typedef struct tag_FA_DIB356 {
	double value;
} FA_DIB356;

typedef struct tag_FA_DIB35F {
	FA_DIB350 DIB350;
	FA_DIB351 DIB351;
	FA_DIB352 DIB352;
	FA_DIB353 DIB353;
	FA_DIB354 DIB354;
	FA_DIB355 DIB355;
	FA_DIB356 DIB356;
} FA_DIB35F;

typedef struct tag_FA_DIB611 {
	WORD value;
} FA_DIB611;

typedef struct tag_FA_DIB612 {
	WORD value;
} FA_DIB612;

typedef struct tag_FA_DIB613 {
	WORD value;
} FA_DIB613;

typedef struct tag_FA_DIB61F {
	FA_DIB611 DIB611;
	FA_DIB612 DIB612;
	FA_DIB613 DIB613;
} FA_DIB61F;

typedef struct tag_FA_DIB621 {
	double value;
} FA_DIB621;

typedef struct tag_FA_DIB622 {
	double value;
} FA_DIB622;

typedef struct tag_FA_DIB623 {
	double value;
} FA_DIB623;

typedef struct tag_FA_DIB624 {
	double value;
} FA_DIB624;

typedef struct tag_FA_DIB62F {
	FA_DIB621 DIB621;
	FA_DIB622 DIB622;
	FA_DIB623 DIB623;
	FA_DIB624 DIB624;
} FA_DIB62F;

typedef struct tag_FA_DIB630 {
	double value;
} FA_DIB630;

typedef struct tag_FA_DIB631 {
	double value;
} FA_DIB631;

typedef struct tag_FA_DIB632 {
	double value;
} FA_DIB632;

typedef struct tag_FA_DIB633 {
	double value;
} FA_DIB633;

typedef struct tag_FA_DIB634 {
	double value;
} FA_DIB634;

typedef struct tag_FA_DIB635 {
	double value;
} FA_DIB635;

typedef struct tag_FA_DIB63F {
	FA_DIB630 DIB630;
	FA_DIB631 DIB631;
	FA_DIB632 DIB632;
	FA_DIB633 DIB633;
	FA_DIB634 DIB634;
	FA_DIB635 DIB635;
} FA_DIB63F;

typedef struct tag_FA_DIB640 {
	double value;
} FA_DIB640;

typedef struct tag_FA_DIB641 {
	double value;
} FA_DIB641;

typedef struct tag_FA_DIB642 {
	double value;
} FA_DIB642;

typedef struct tag_FA_DIB643 {
	double value;
} FA_DIB643;

typedef struct tag_FA_DIB64F {
	FA_DIB640 DIB640;
	FA_DIB641 DIB641;
	FA_DIB642 DIB642;
	FA_DIB643 DIB643;
} FA_DIB64F;

typedef struct tag_FA_DIB650 {
	double value;
} FA_DIB650;

typedef struct tag_FA_DIB651 {
	double value;
} FA_DIB651;

typedef struct tag_FA_DIB652 {
	double value;
} FA_DIB652;

typedef struct tag_FA_DIB653 {
	double value;
} FA_DIB653;

#if (defined(_GUANGDONG_FOSHAN) || defined(_GUANGDONG_SHANTOU))
typedef struct tag_FA_DIB654
{
	double value;
}FA_DIB654;

typedef struct tag_FA_DIB655
{
	double value;
}FA_DIB655;
#endif

typedef struct tag_FA_DIB65F {
	FA_DIB650 DIB650;
	FA_DIB651 DIB651;
	FA_DIB652 DIB652;
	FA_DIB653 DIB653;
//#if (defined(_GUANGDONG_FOSHAN) || defined(_GUANGDONG_SHANTOU))
//	FA_DIB654	DIB654;
//	FA_DIB655	DIB655;
//#endif
} FA_DIB65F;

typedef struct tag_FA_DIB660 {
	double dAngleVoltag;
} FA_DIB660;

typedef struct tag_FA_DIB661 {
	double dAngleVoltag;
} FA_DIB661;

typedef struct tag_FA_DIB662 {
	double dAngleVoltag;
} FA_DIB662;

typedef struct tag_FA_DIB663 {
	double dAngleCurrent;
} FA_DIB663;

typedef struct tag_FA_DIB664 {
	double dAngleCurrent;
} FA_DIB664;
typedef struct tag_FA_DIB665 {
	double dAngleCurrent;
} FA_DIB665;

typedef struct tag_FA_DIB66F {
	FA_DIB660 DIB660;
	FA_DIB661 DIB661;
	FA_DIB662 DIB662;
	FA_DIB663 DIB663;
	FA_DIB664 DIB664;
	FA_DIB665 DIB665;
} FA_DIB66F;

typedef struct tag_FA_DIB6FF {
	FA_DIB61F DIB61F;
	FA_DIB62F DIB62F;
	FA_DIB63F DIB63F;
	FA_DIB64F DIB64F;
	FA_DIB65F DIB65F;
	FA_DIB66F DIB66F;
} FA_DIB6FF;

typedef struct tag_FA_DIC010 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE week;
} FA_DIC010;

typedef struct tag_FA_DIC011 {
	BYTE hour;
	BYTE minute;
	BYTE second;
} FA_DIC011;

typedef struct tag_FA_DIC01F {
	FA_DIC010 DIC010;
	FA_DIC011 DIC011;
} FA_DIC01F;

typedef struct tag_FA_DIC020 {
	BYTE flag;
} FA_DIC020;

typedef struct tag_FA_DIC119 {
	double value;
} FA_DIC119;

typedef struct tag_FA_DIC11A {
	double value;
} FA_DIC11A;

typedef struct tag_FA_DIC030 {
	DWORD value;
} FA_DIC030;

typedef struct tag_FA_DIC031 {
	DWORD value;
} FA_DIC031;

typedef struct tag_FA_DIC331 {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} FA_DIC331;

typedef struct tag_FA_DIC332 {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} FA_DIC332;

typedef struct tag_FA_DIC333 {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} FA_DIC333;

typedef struct tag_FA_DIC334 {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} FA_DIC334;

typedef struct tag_FA_DIC335 {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} FA_DIC335;

typedef struct tag_FA_DIC336 {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} FA_DIC336;

typedef struct tag_FA_DIC337 {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} FA_DIC337;

typedef struct tag_FA_DIC338 {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} FA_DIC338;

typedef struct tag_FA_DIC33F {
	FA_DIC331 DIC331;
	FA_DIC332 DIC332;
	FA_DIC333 DIC333;
	FA_DIC334 DIC334;
	FA_DIC335 DIC335;
	FA_DIC336 DIC336;
	FA_DIC337 DIC337;
	FA_DIC338 DIC338;
} FA_DIC33F;

typedef enum tag_FA_TASK_ENUM {
	FA_COMMON = 0x01, FA_RELAY = 0x02, FA_ABNORMITY = 0x04
} FA_TASK_ENUM;

typedef enum tag_FA_TIMEINTERVAL_UNIT {
	FA_MINUTE = 2,   // 分钟
	FA_HOUR = 3,   // 小时
	FA_DAY = 4,   // 日
	FA_MONTH = 5    // 月
} FA_TIMEINTERVAL_UNIT;

typedef struct tag_FA_ACRUNSTATE {
	BYTE bCurTarrif;
	BYTE bCurQuadrant;

} FA_ACRUNSTATE, FK_DT08F97;

// 湖北增补
typedef struct tag_FA_PACKETTIME {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	BYTE second;
} FA_PACKETTIME;

//
// 测量点数据
//
//2007-645
typedef struct tag_MT_DI00000X00 {
	double value;
} MT_DI00000X00;

typedef struct tag_MT_DI0000FF00 {
	MT_DI00000X00 DT00000X00[5];
} MT_DI000XFF00;

typedef struct tag_MT_DI008F0000 {
	MT_DI00000X00 DT00800000;
	MT_DI00000X00 DT00810000;
	MT_DI00000X00 DT00820000;
	MT_DI00000X00 DT00830000;
	MT_DI00000X00 DT00840000;
	MT_DI00000X00 DT00850000;
	MT_DI00000X00 DT00860000;
} MT_DI008F0000;

typedef struct tag_MT_DI008F0000_ARRAY {
	MT_DI00000X00 DT00800000[7];
} MT_DI008F0000_ARRAY;

typedef struct tag_MT_DI_ENERGY_PHASE_X {
	MT_DI00000X00 DT00150000;
	MT_DI00000X00 DT00160000;
	MT_DI00000X00 DT00170000;
	MT_DI00000X00 DT00180000;
	MT_DI00000X00 DT00190000;
	MT_DI00000X00 DT001A0000;
	MT_DI00000X00 DT001B0000;
	MT_DI00000X00 DT001C0000;
	MT_DI00000X00 DT001D0000;
	MT_DI00000X00 DT001E0000;
	MT_DI00000X00 DT00940000;
	MT_DI00000X00 DT00950000;
	MT_DI00000X00 DT00960000;
	MT_DI00000X00 DT00970000;
	MT_DI00000X00 DT00980000;
	MT_DI00000X00 DT00990000;
	MT_DI00000X00 DT009A0000;
} MT_DI_ENERGY_PHASE_X;

typedef struct tag_MT_DI_ENERGY_PHASE_X_ARRAY {
	MT_DI00000X00 DT00150000[17];
} MT_DI_ENERGY_PHASE_X_ARRAY;

typedef struct tag_MT_DI_ENERGY {
	MT_DI000XFF00 DT0000FF00;
	MT_DI000XFF00 DT0001FF00;
	MT_DI000XFF00 DT0002FF00;
	MT_DI000XFF00 DT0003FF00;
	MT_DI000XFF00 DT0004FF00;
	MT_DI000XFF00 DT0005FF00;
	MT_DI000XFF00 DT0006FF00;
	MT_DI000XFF00 DT0007FF00;
	MT_DI000XFF00 DT0008FF00;
	MT_DI000XFF00 DT0009FF00;
	MT_DI000XFF00 DT000AFF00;
	MT_DI008F0000_ARRAY DT008F0000;
	MT_DI_ENERGY_PHASE_X_ARRAY DT_CUR_ENERGY_PHASE_A;
	MT_DI_ENERGY_PHASE_X_ARRAY DT_CUR_ENERGY_PHASE_B;
	MT_DI_ENERGY_PHASE_X_ARRAY DT_CUR_ENERGY_PHASE_C;

} MT_DI_ENERGY;

typedef struct tag_MT_DI009001FF {
	MT_DI00000X00 DT00900100;
	MT_DI00000X00 DT00900101;

} MT_DI009001FF;

typedef struct tag_MT_DI009002FF {
	MT_DI00000X00 DT00900200;
	MT_DI00000X00 DT00900201;

} MT_DI009002FF;

//最大需量及发生时间
typedef struct tag_MT_DI_TIME {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} MT_DI_TIME;

typedef struct tag_MT_DI010X0X00 {
	double value;
	MT_DI_TIME demandTime;
} MT_DI010X0X00;

typedef struct tag_MT_DI_010XFF00 {
	MT_DI010X0X00 DT01010X00[5];
} MT_DI010XFF00;

typedef struct tag_MT_DI_DEMAND_PHASE_X {
	MT_DI010X0X00 DT01150000;
	MT_DI010X0X00 DT01160000;
	MT_DI010X0X00 DT01170000;
	MT_DI010X0X00 DT01180000;
	MT_DI010X0X00 DT01190000;
	MT_DI010X0X00 DT011A0000;
	MT_DI010X0X00 DT011B0000;
	MT_DI010X0X00 DT011C0000;
	MT_DI010X0X00 DT011D0000;
	MT_DI010X0X00 DT011E0000;
} MT_DI_DEMAND_PHASE_X;

typedef struct tag_MT_DI_DEMAND {
	MT_DI010XFF00 DT0101FF00;
	MT_DI010XFF00 DT0102FF00;
	MT_DI010XFF00 DT0103FF00;
	MT_DI010XFF00 DT0104FF00;
	MT_DI010XFF00 DT0105FF00;
	MT_DI010XFF00 DT0106FF00;
	MT_DI010XFF00 DT0107FF00;
	MT_DI010XFF00 DT0108FF00;
	MT_DI010XFF00 DT0109FF00;
	MT_DI010XFF00 DT010AFF00;
	MT_DI_DEMAND_PHASE_X DT_CUR_DEMAND_PHASE_A;
	MT_DI_DEMAND_PHASE_X DT_CUR_DEMAND_PHASE_B;
	MT_DI_DEMAND_PHASE_X DT_CUR_DEMAND_PHASE_C;
} MT_DI_DEMAND;

//电压
typedef struct tag_MT_DI02010X00 {
	WORD value;
} MT_DI02010X00;

typedef struct tag_MT_DI0201FF00 {
	MT_DI02010X00 DT02010X00[3];
} MT_DI0201FF00;

//电流
typedef struct tag_MT_DI02020X00 {
	double value;
} MT_DI02020X00;

typedef struct tag_MT_DI0202FF00 {
	MT_DI02020X00 DT02020X00[3];
} MT_DI0202FF00;

//有功功率
typedef struct tag_MT_DI02030X00 {
	double value;
} MT_DI02030X00;

typedef struct tag_MT_DI0203FF00 {
	MT_DI02030X00 DT02030X00[4];
} MT_DI0203FF00;

//无功功率
typedef struct tag_MT_DI02040X00 {
	double value;
} MT_DI02040X00;

typedef struct tag_MT_DI0204FF00 {
	MT_DI02040X00 DT02040X00[4];
} MT_DI0204FF00;

//视在功率
typedef struct tag_MT_DI02050X00 {
	double value;
} MT_DI02050X00;

typedef struct tag_MT_DI0205FF00 {
	MT_DI02050X00 DT02050X00[4];
} MT_DI0205FF00;

//功率因数
typedef struct tag_MT_DI02060X00 {
	double value;
} MT_DI02060X00;

typedef struct tag_MT_DI0206FF00 {
	MT_DI02060X00 DT02060X00[4];
} MT_DI0206FF00;

//相角
typedef struct tag_MT_DI02070X00 {
	double value;
} MT_DI02070X00;

typedef struct tag_MT_DI0207FF00 {
	MT_DI02070X00 DT02070X00[3];
} MT_DI0207FF00;

//电压波形失真度
typedef struct tag_MT_DI02080X00 {
	WORD value;
} MT_DI02080X00;

typedef struct tag_MT_DI0208FF00 {
	MT_DI02080X00 DT02080X00[3];
} MT_DI0208FF00;

//电流波形失真度
typedef struct tag_MT_DI02090X00 {
	WORD value;
} MT_DI02090X00;

typedef struct tag_MT_DI0209FF00 {
	MT_DI02090X00 DT02090X00[3];
} MT_DI0209FF00;

//电压谐波含量
typedef struct tag_MT_DI020A0X0X {
	WORD value;
} MT_DI020A0X0X;

typedef struct tag_MT_DI020A0XFF {
	MT_DI020A0X0X DT020A0X0X[15];
} MT_DI020A0XFF;

//电流谐波含量
typedef struct tag_MT_DI020B0X0X {
	WORD value;
} MT_DI020B0X0X;

typedef struct tag_MT_DI020B0XFF {
	MT_DI020B0X0X DT020B0X0X[15];
} MT_DI020B0XFF;

//
typedef struct tag_MT_DI02800001 {
	double zeroCurrent;
} MT_DI02800001;

typedef struct tag_MT_DI02800002 {
	WORD freq;
} MT_DI02800002;

typedef struct tag_MT_DI02800003 {
	double powerZP;
} MT_DI02800003;

typedef struct tag_MT_DI02800004 {
	double demandP;
} MT_DI02800004;

typedef struct tag_MT_DI02800005 {
	double demandQ;
} MT_DI02800005;

typedef struct tag_MT_DI02800006 {
	double demandS;
} MT_DI02800006;

typedef struct tag_MT_DI02800007 {
	WORD temp;
} MT_DI02800007;

typedef struct tag_MT_DI02800008 {
	WORD volBatteryIns;
} MT_DI02800008;

typedef struct tag_MT_DI02800009 {
	WORD volBatteryPowerOff;
} MT_DI02800009;

typedef struct tag_MT_DI0280000A {
	DWORD minsBatteryRun;
} MT_DI0280000A;

typedef struct tag_MT_DI0280000B {
	double energyFee;
} MT_DI0280000B;

typedef struct tag_MT_DI0280000F {
	MT_DI02800001 DT02800001;
	MT_DI02800002 DT02800002;
	MT_DI02800003 DT02800003;
	MT_DI02800004 DT02800004;
	MT_DI02800005 DT02800005;
	MT_DI02800006 DT02800006;
	MT_DI02800007 DT02800007;
	MT_DI02800008 DT02800008;
	MT_DI02800009 DT02800009;
	MT_DI0280000A DT0280000A;
	MT_DI0280000B DT0280000B;

} MT_DI028000FF;

//参变量

//日期及周次
typedef struct tag_EM_DI04000101 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE week;
} EM_DI04000101;

//时间
typedef struct tag_EM_DI04000102 {
	BYTE hour;
	BYTE minute;
	BYTE second;
} EM_DI04000102;

//最大需量周期
typedef struct tag_EM_DI04000103 {
	BYTE minutes;
} EM_DI04000103;

//滑差时间
typedef struct tag_EM_DI04000104 {
	BYTE minutes;
} EM_DI04000104;

//输出脉冲宽度
typedef struct tag_EM_DI04000105 {
	WORD ms;
} EM_DI04000105;

typedef struct tag_EM_DI04000106 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DI04000106;

typedef struct tag_EM_DI04000107 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DI04000107;

typedef struct tag_EM_DI04000108 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DI04000108;

typedef struct tag_EM_DI04000109 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DI04000109;

typedef struct tag_EM_DI040001FF {
	EM_DI04000101 DI04000101;
	EM_DI04000102 DI04000102;
	EM_DI04000103 DI04000103;
	EM_DI04000104 DI04000104;
	EM_DI04000105 DI04000105;
	EM_DI04000106 DI04000106;
	EM_DI04000107 DI04000107;
	EM_DI04000108 DI04000108;
	EM_DI04000109 DI04000109;
} EM_DI040001FF;

//年时区数P
typedef struct tag_EM_DI04000201 {
	BYTE num;
} EM_DI04000201;

//日时段表数q
typedef struct tag_EM_DI04000202 {
	BYTE num;
} EM_DI04000202;

//日时段(每日切换数)m
typedef struct tag_EM_04000203 {
	BYTE num;
} EM_DI04000203;

//费率数k
typedef struct tag_EM_DI04000204 {
	BYTE num;
} EM_DI04000204;

//公共假日数n
typedef struct tag_EM_DI04000205 {
	WORD num;
} EM_DI04000205;

//谐波分析次数
typedef struct tag_EM_DI04000206 {
	BYTE num;
} EM_DI04000206;

//阶梯数
typedef struct tag_EM_DI04000207 {
	BYTE num;
} EM_DI04000207;

typedef struct tag_EM_DI040002FF {
	EM_DI04000201 DI04000201;
	EM_DI04000202 DI04000202;
	EM_DI04000203 DI04000203;
	EM_DI04000204 DI04000204;
	EM_DI04000205 DI04000205;
	EM_DI04000206 DI04000206;
	EM_DI04000207 DI04000207;
} EM_DI040002FF;

//自动循环显示屏数
typedef struct tag_EM_DI04000301 {
	BYTE num;
} EM_DI04000301;

//每屏显示时间
typedef struct tag_EM_DI04000302 {
	BYTE sec;
} EM_DI04000302;

//显示电能小数位数
typedef struct tag_EM_DI04000303 {
	BYTE bits;
} EM_DI04000303;

//显示功率(最大需量)小数位数
typedef struct tag_EM_DI04000304 {
	BYTE bits;
} EM_DI04000304;

//按键循环显示屏数
typedef struct tag_EM_DI04000305 {
	BYTE num;
} EM_DI04000305;

//电流互感器变比
typedef struct tag_EM_DI04000306 {
	DWORD ct;
} EM_DI04000306;

//电压互感器变比
typedef struct tag_EM_DI04000307 {
	DWORD pt;
} EM_DI04000307;

typedef struct tag_EM_DI040003FF {
	EM_DI04000301 DI04000301;
	EM_DI04000302 DI04000302;
	EM_DI04000303 DI04000303;
	EM_DI04000304 DI04000304;
	EM_DI04000305 DI04000305;
	EM_DI04000306 DI04000306;
	EM_DI04000307 DI04000307;
} EM_DI040003FF;

//通信地址
typedef struct tag_EM_DI04000401 {
	BYTE address[6];
} EM_DI04000401;

//表号
typedef struct tag_EM_DI04000402 {
	BYTE host[6];
} EM_DI04000402;

//资产管理编码
typedef struct tag_EM_DI04000403 {
	//char propertyNum[32];
	char propertyNum[12]; //省出一些字节给额定电压电流
} EM_DI04000403;

//额定电压
typedef struct tag_EM_DI04000404 {
	WORD ratedVol;
	char ratedVolchar[6];
} EM_DI04000404;

//额定电流
typedef struct tag_EM_DI04000405 {
	double ratedCurr;
	char ratedCurrchar[6];
} EM_DI04000405;

//最大电流
typedef struct tag_EM_DI04000406 {
	double maxCurr;
	char maxCurrchar[6];
} EM_DI04000406;

//有功准确度等级
typedef struct tag_EM_DI04000407 {
	char actpowerGrade[4];
} EM_DI04000407;

//无功准确度等级
typedef struct tag_EM_DI04000408 {
	char inactpowerGrade[4];
} EM_DI04000408;

//电表常数(有功)
typedef struct tag_EM_DI04000409 {
	DWORD value;
} EM_DI04000409;

//电表常数(无功)
typedef struct tag_EM_DI0400040A {
	DWORD value;
} EM_DI0400040A;

//电表型号
typedef struct tag_EM_DI0400040B {
	char meterType[10];
} EM_DI0400040B;

//生产日期
typedef struct tag_EM_DI0400040C {
	char manudate[10];
} EM_DI0400040C;

//协议版本号
typedef struct tag_EM_DI0400040D {
	char prtlversion[16];
} EM_DI0400040D;

//客户编号
typedef struct tag_EM_DI0400040E {
	char clientnum[6];
} EM_DI0400040E;

typedef struct tag_EM_DI040004FF {
	EM_DI04000401 DI04000401;
	EM_DI04000402 DI04000402;
	EM_DI04000403 DI04000403;
	EM_DI04000404 DI04000404;
	EM_DI04000405 DI04000405;
	EM_DI04000406 DI04000406;
	EM_DI04000407 DI04000407;
	EM_DI04000408 DI04000408;
	EM_DI04000409 DI04000409;
	EM_DI0400040A DI0400040A;
	EM_DI0400040B DI0400040B;
	EM_DI0400040C DI0400040C;
	EM_DI0400040D DI0400040D;
	EM_DI0400040E DI0400040E;

} EM_DI040004FF;

//电表运行状态字
typedef struct tag_EM_DI0400050X {
	WORD status;
} EM_DI0400050X;

typedef struct tag_EM_DI040005FF {
	EM_DI0400050X DI0400050X[7];

} EM_DI040005FF;

//有无功组合特征字
typedef struct tag_EM_DI0400060X {
	BYTE powercombWord;
} EM_DI0400060X;

typedef struct tag_EM_DI040006FF {
	EM_DI0400060X DI04000601;
	EM_DI0400060X DI04000602;
	EM_DI0400060X DI04000603;
} EM_DI040006FF;

//串口通讯速率特征字
typedef struct tag_EM_DI0400070X {
	BYTE comRateWord;
} EM_DI0400070X;

typedef struct tag_EM_DI040007FF {
	EM_DI0400070X DI04000701;
	EM_DI0400070X DI04000702;
	EM_DI0400070X DI04000703;
	EM_DI0400070X DI04000704;
	EM_DI0400070X DI04000705;

} EM_DI040007FF;

//周休日特征字
typedef struct tag_EM_DI04000801 {
	BYTE status;
} EM_DI04000801;

typedef struct tag_EM_DI04000802 {
	BYTE num;
} EM_DI04000802;

typedef struct tag_EM_DI040008FF {
	EM_DI04000801 DI04000801;
	EM_DI04000802 DI04000802;

} EM_DI040008FF;

//负荷记录模式字
typedef struct tag_EM_DI04000901 {
	BYTE loadByte;
} EM_DI04000901;

//冻结数据模式字
typedef struct tag_EM_DI04000902 {
	BYTE crtlByte;
} EM_DI04000902;

typedef struct tag_EM_DI040009FF {
	EM_DI04000901 DI04000901;
	EM_DI04000902 DI04000902[5];

} EM_DI040009FF;

//负荷记录参数
typedef struct tag_EM_DI04000A01 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DI04000A01;

typedef struct tag_EM_DI04000A012 {
	WORD minute;
} EM_DI04000A02;

typedef struct tag_EM_DI04000AFF {
	EM_DI04000A01 DI04000A01;
	EM_DI04000A02 DI04000A02[6];

} EM_DI04000AFF;

//结算日期
typedef struct tag_EM_DI04000B0X {
	BYTE hour;
	BYTE day;
} EM_DI04000B0X;

typedef struct tag_EM_DI04000BFF {
	EM_DI04000B0X DI04000B0X[3];
} EM_DI04000BFF;

//密码
typedef struct tag_EM_DI04000C0X {
	BYTE pass[4];
} EM_DI04000C0X;

typedef struct tag_EM_DI04000CFF {
	EM_DI04000C0X DI04000C0X[10];
} EM_DI04000CFF;

//有功功率上限值
typedef struct tag_EM_DI04000E01 {
	double powerUpLimit;
} EM_DI04000E01;

//电压限值
typedef struct tag_EM_DI04000E03 {
	WORD volUpLimit;
} EM_DI04000E03;

typedef struct tag_EM_DI04000EFF {
	EM_DI04000E01 DI04000E01;
	EM_DI04000E01 DI04000E02;
	EM_DI04000E03 DI04000E03;
	EM_DI04000E03 DI04000E04;
} EM_DI04000EFF;

//电量限值
typedef struct tag_EM_DI04000F0X {
	double value;
} EM_DI04000F0X;

typedef struct tag_EM_DI04000FFF {
	EM_DI04000F0X DI04000F01;
	EM_DI04000F0X DI04000F02;
	EM_DI04000F0X DI04000F03;
	EM_DI04000F0X DI04000F04;
} EM_DI04000FFF;

//金额限值
typedef struct tag_EM_DI0400100X {
	double value;
} EM_DI0400100X;

typedef struct tag_EM_DI040010FF {
	EM_DI0400100X DI04001001;
	EM_DI0400100X DI04001002;
	EM_DI0400100X DI04001003;
	EM_DI0400100X DI04001004;
	EM_DI0400100X DI04001005;
} EM_DI040010FF;

//电表运行特征字
typedef struct tag_EM_DI04001101 {
	BYTE status;
} EM_DI04001101;

typedef struct tag_EM_DI040011FF {
	EM_DI04001101 DI04001101;

} EM_DI040011FF;

//整点冻结起始时间
typedef struct tag_EM_DI04001201 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;

} EM_DI04001201;

//整点冻结时间间隔
typedef struct tag_EM_DI04001202 {
	BYTE minute;

} EM_DI04001202;

//日冻结时间
typedef struct tag_EM_DI04001203 {
	BYTE hour;
	BYTE minute;

} EM_DI04001203;

typedef struct tag_EM_DI040012FF {
	EM_DI04001201 DI04001201;
	EM_DI04001202 DI04001202;
	EM_DI04001203 DI04001203;

} EM_DI040012FF;

//无线通信在线及信号强弱指示
typedef struct tag_EM_DI04001301 {
	BYTE status;

} EM_DI04001301;

typedef struct tag_EM_DI040013FF {
	EM_DI04001301 DI04001301;

} EM_DI040013FF;

//跳闸延时时间
typedef struct tag_EM_DI04001401 {
	WORD minute;

} EM_DI04001401;

typedef struct tag_EM_DI040014FF {
	EM_DI04001401 DI04001401;

} EM_DI040014FF;

typedef struct tag_EM_DI_PERIOD {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} EM_DI_PERIOD;

typedef struct tag_EM_DI_TIMEZONE {
	BYTE month;
	BYTE day;
	BYTE NN;
} EM_DI_TIMEZONE;

// 第一套时区
typedef struct tag_EM_DI04010000 {
	EM_DI_TIMEZONE DI_TIMEZONE[LINYANG_EM_MAX_TIMEMASK];
} EM_DI04010000;

// 时段
typedef struct tag_EM_DI0401000X {
	EM_DI_PERIOD DI_PERIOD[LINYANG_EM_MAX_TIMEMASK];
} EM_DI0401000X;

// 时段表集合(第一套第1-8日)
typedef struct tag_EM_DI040100FF {
	EM_DI0401000X DI0401000X[LINYANG_EM_MAX_TIMETABLE];
} EM_DI040100FF;

// 第二套时区
typedef struct tag_EM_DI04020000 {
	EM_DI_TIMEZONE DI_TIMEZONE[LINYANG_EM_MAX_TIMEMASK];
} EM_DI04020000;

// 第二套时段
typedef struct tag_EM_DI0402000X {
	EM_DI_PERIOD DI_PERIOD[LINYANG_EM_MAX_TIMEMASK];
} EM_DI0402000X;

// 第二套时段表集合
typedef struct tag_EM_DI040200FF {
	EM_DI0401000X DI0401000X[LINYANG_EM_MAX_TIMETABLE];
} EM_DI040200FF;

//公共假日
typedef struct tag_EM_DI0403000X {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE NN;
} EM_DI040300XX;

typedef struct tag_EM_DI040300FF {
	EM_DI040300XX DI040300XX[LINYANG_EM_MAX_HOLIDAY];
} EM_DI040300FF;

//自动巡显数据项
typedef struct tag_EM_DI0404010X {
	DWORD di;
} EM_DI040401XX;

typedef struct tag_EM_DI040401FF {
	EM_DI040401XX DI040401XX[LINYANG_EM_MAX_AUTODISPLAY];
} EM_DI040401FF;

//按键显示数据项
typedef struct tag_EM_DI0404020X {
	DWORD di;
} EM_DI040402XX;

typedef struct tag_EM_DI040402FF {
	EM_DI040402XX DI040402XX[LINYANG_EM_MAX_HANDDISPLAY];
} EM_DI040402FF;

//第一套费率电价
typedef struct tag_EM_DI040501XX {
	double value;
} EM_DI040501XX;

typedef struct tag_EM_DI040501FF {
	EM_DI040501XX DI040501XX[63];
} EM_DI040501FF;

typedef struct tag_EM_DI040502XX {
	double value;
} EM_DI040502XX;

typedef struct tag_EM_DI040502FF {
	EM_DI040502XX DI040502XX[63];
} EM_DI040502FF;

//第一套N阶梯值
typedef struct tag_EM_DI040600XX {
	double value;
} EM_DI040600XX;

typedef struct tag_EM_DI040600FF {
	EM_DI040600XX DI040600XX[14];
} EM_DI040600FF;

//第二套N阶梯值
typedef struct tag_EM_DI040602FF {
	EM_DI040600XX DI040600XX[14];
} EM_DI040602FF;

//第一套N阶电价
typedef struct tag_EM_DI040601XX {
	double value;
} EM_DI040601XX;

typedef struct tag_EM_DI040601FF {
	EM_DI040601XX DI040601XX[14];
} EM_DI040601FF;

//第二套N阶电价
typedef struct tag_EM_DI040603FF {
	EM_DI040601XX DI040601XX[14];
} EM_DI040603FF;

//失压事件参数
typedef struct tag_EM_DI04090101 {
	WORD value;
} EM_DI04090101;

typedef struct tag_EM_DI04090102 {
	WORD value;
} EM_DI04090102;

typedef struct tag_EM_DI04090103 {
	double value;
} EM_DI04090103;

typedef struct tag_EM_DI04090104 {
	BYTE sec;
} EM_DI04090104;

typedef struct tag_EM_DI040901FF {
	EM_DI04090101 DI04090101;
	EM_DI04090102 DI04090102;
	EM_DI04090103 DI04090103;
	EM_DI04090104 DI04090104;
} EM_DI040901FF;

//欠压事件参数
typedef struct tag_EM_DI04090201 {
	WORD value;
} EM_DI04090201;

typedef struct tag_EM_DI04090202 {
	BYTE sec;
} EM_DI04090202;

typedef struct tag_EM_DI040902FF {
	EM_DI04090201 DI04090201;
	EM_DI04090202 DI04090202;

} EM_DI040902FF;

//过压事件参数
typedef struct tag_EM_DI04090301 {
	WORD value;
} EM_DI04090301;

typedef struct tag_EM_DI04090302 {
	BYTE sec;
} EM_DI04090302;

typedef struct tag_EM_DI040903FF {
	EM_DI04090301 DI04090301;
	EM_DI04090302 DI04090302;

} EM_DI040903FF;

//断相事件参数
typedef struct tag_EM_DI04090401 {
	WORD value;
} EM_DI04090401;

typedef struct tag_EM_DI04090402 {
	double value;
} EM_DI04090402;

typedef struct tag_EM_DI04090403 {
	BYTE sec;
} EM_DI04090403;

typedef struct tag_EM_DI040904FF {
	EM_DI04090401 DI04090401;
	EM_DI04090402 DI04090402;
	EM_DI04090403 DI04090403;
} EM_DI040904FF;

//电压不平衡事件参数
typedef struct tag_EM_DI04090501 {
	double value;
} EM_DI04090501;

typedef struct tag_EM_DI04090502 {
	BYTE sec;
} EM_DI04090502;

typedef struct tag_EM_DI040905FF {
	EM_DI04090501 DI04090501;
	EM_DI04090502 DI04090502;

} EM_DI040905FF;

//电流不平衡事件参数
typedef struct tag_EM_DI04090601 {
	double value;
} EM_DI04090601;

typedef struct tag_EM_DI04090602 {
	BYTE sec;
} EM_DI04090602;

typedef struct tag_EM_DI040906FF {
	EM_DI04090601 DI04090601;
	EM_DI04090602 DI04090602;

} EM_DI040906FF;

//失流事件参数
typedef struct tag_EM_DI04090701 {
	WORD value;
} EM_DI04090701;

typedef struct tag_EM_DI04090702 {
	double value;
} EM_DI04090702;

typedef struct tag_EM_DI04090703 {
	double value;
} EM_DI04090703;

typedef struct tag_EM_DI04090704 {
	BYTE sec;
} EM_DI04090704;

typedef struct tag_EM_DI040907FF {
	EM_DI04090701 DI04090701;
	EM_DI04090702 DI04090702;
	EM_DI04090703 DI04090703;
	EM_DI04090704 DI04090704;
} EM_DI040907FF;

//过流事件参数
typedef struct tag_EM_DI04090801 {
	double value;
} EM_DI04090801;

typedef struct tag_EM_DI04090802 {
	BYTE sec;
} EM_DI04090802;

typedef struct tag_EM_DI040908FF {
	EM_DI04090801 DI04090801;
	EM_DI04090802 DI04090802;

} EM_DI040908FF;

//断流事件参数
typedef struct tag_EM_DI04090901 {
	WORD value;
} EM_DI04090901;

typedef struct tag_EM_DI04090902 {
	double value;
} EM_DI04090902;

typedef struct tag_EM_DI04090903 {
	BYTE sec;
} EM_DI04090903;

typedef struct tag_EM_DI040909FF {
	EM_DI04090901 DI04090901;
	EM_DI04090902 DI04090902;
	EM_DI04090903 DI04090903;
} EM_DI040909FF;

//潮流反向事件参数
typedef struct tag_EM_DI04090A01 {
	double value;
} EM_DI04090A01;

typedef struct tag_EM_DI04090A02 {
	BYTE sec;
} EM_DI04090A02;

typedef struct tag_EM_DI04090AFF {
	EM_DI04090A01 DI04090A01;
	EM_DI04090A02 DI04090A02;

} EM_DI04090AFF;

//过载事件参数
typedef struct tag_EM_DI04090B01 {
	double value;
} EM_DI04090B01;

typedef struct tag_EM_DI04090B02 {
	BYTE sec;
} EM_DI04090B02;

typedef struct tag_EM_DI04090BFF {
	EM_DI04090B01 DI04090B01;
	EM_DI04090B02 DI04090B02;

} EM_DI04090BFF;

//电压考核参数
typedef struct tag_EM_DI04090C01 {
	WORD value;
} EM_DI04090C01;

typedef struct tag_EM_DI04090C02 {
	WORD value;
} EM_DI04090C02;

typedef struct tag_EM_DI04090CFF {
	EM_DI04090C01 DI04090C01;
	EM_DI04090C02 DI04090C02;

} EM_DI04090CFF;

//需量超限参数
typedef struct tag_EM_DI04090D01 {
	double value;
} EM_DI04090D01;

typedef struct tag_EM_DI04090D02 {
	double value;
} EM_DI04090D02;

typedef struct tag_EM_DI04090D03 {
	BYTE sec;
} EM_DI04090D03;

typedef struct tag_EM_DI04090DFF {
	EM_DI04090D01 DI04090D01;
	EM_DI04090D02 DI04090D02;
	EM_DI04090D03 DI04090D03;

} EM_DI04090DFF;

//功率因数超限参数
typedef struct tag_EM_DI04090E01 {
	double value;
} EM_DI04090E01;

typedef struct tag_EM_DI04090E02 {
	BYTE sec;
} EM_DI04090E02;

typedef struct tag_EM_DI04090EFF {
	EM_DI04090E01 DI04090E01;
	EM_DI04090E02 DI04090E02;

} EM_DI04090EFF;

//电流不平衡超限参数
typedef struct tag_EM_DI04090F01 {
	double value;
} EM_DI04090F01;

typedef struct tag_EM_DI04090F02 {
	BYTE sec;
} EM_DI04090F02;

typedef struct tag_EM_DI04090FFF {
	EM_DI04090F01 DI04090F01;
	EM_DI04090F02 DI04090F02;

} EM_DI04090FFF;

//版本号
typedef struct tag_EM_DI0480000X {
	BYTE version[32];
} EM_DI0480000X;

typedef struct tag_EM_DI048000FF {
	EM_DI0480000X DI04800001;
	EM_DI0480000X DI04800002;
	EM_DI0480000X DI04800003;
} EM_DI048000FF;

//失压次数、累计时间

typedef struct tag_MT_DI10000001 {
	DWORD times;
} MT_DI10000001, MT_DI10010001, MT_DI10020001, MT_DI10030001;

typedef struct tag_MT_DI10000002 {
	DWORD mins;
} MT_DI10000002, MT_DI10010002, MT_DI10020002, MT_DI10030002;

typedef struct tag_MT_DI100000FF {
	MT_DI10000001 DT10000001;
	MT_DI10000002 DT10000002;
} MT_DI100000FF;

typedef struct tag_MT_DI100X00FF {
	MT_DI10010001 DT10010001;
	MT_DI10010002 DT10010002;
} MT_DI100X00FF;

typedef struct tag_MT_DI10000101 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	BYTE second;

} MT_DI10000101, MT_DI10000201, MT_DI_TIMEYS;

typedef struct tag_MT_DI1000FF01 {
	MT_DI10000101 DT10000101;
	MT_DI10000201 DT10000201;
} MT_DI1000FF01;

//失压记录

typedef struct tag_MT_DI_LOSTV_TOTAL {
	MT_DI00000X00 DT10010201;
	MT_DI00000X00 DT10010301;
	MT_DI00000X00 DT10010401;
	MT_DI00000X00 DT10010501;
} MT_DI_LOSTV_TOTAL;

typedef struct tag_MT_DI_LOSTV_AH {
	double Ah[4];

} MT_DI_LOSTV_AH;

typedef struct tag_MT_DI_START_LOSTV_PHASE {
	MT_DI00000X00 DT10010601;
	MT_DI00000X00 DT10010701;
	MT_DI00000X00 DT10010801;
	MT_DI00000X00 DT10010901;

	MT_DI02010X00 DT10010A01;
	MT_DI02020X00 DT10010B01;
	MT_DI02030X00 DT10010C01;
	MT_DI02040X00 DT10010D01;
	MT_DI02060X00 DT10010E01;

} MT_DI_START_LOSTV_PHASE;

typedef struct tag_MT_DI_END_LOSTV_PHASE {
	MT_DI00000X00 DT10010601;
	MT_DI00000X00 DT10010701;
	MT_DI00000X00 DT10010801;
	MT_DI00000X00 DT10010901;
} MT_DI_END_LOSTV_PHASE;

typedef struct tag_MT_DI100XFF0X {
	MT_DI_TIMEYS DT_START_LV_TIME;

	MT_DI_LOSTV_TOTAL DT_START_LV_TOTAL;

	MT_DI_START_LOSTV_PHASE DT_START_LV_PHASE[3];

	MT_DI_LOSTV_AH DT_LV_AH;

	MT_DI_TIMEYS DT_END_LV_TIME;

	MT_DI_LOSTV_TOTAL DT_END_LV_TOTAL;

	MT_DI_END_LOSTV_PHASE DT_END_LV_PHASE[3];

	/*
	 MT_DI00000X00 DT10010201;
	 MT_DI00000X00 DT10010301;
	 MT_DI00000X00 DT10010401;
	 MT_DI00000X00 DT10010501;
	 MT_DI00000X00 DT10010601;
	 MT_DI00000X00 DT10010701;
	 MT_DI00000X00 DT10010801;
	 MT_DI00000X00 DT10010901;

	 MT_DI02010X00 DT10010A01;
	 MT_DI02020X00 DT10010B01;
	 MT_DI02030X00 DT10010C01;
	 MT_DI02040X00 DT10010D01;
	 MT_DI02060X00 DT10010E01;

	 MT_DI00000X00 DT10010F01;
	 MT_DI00000X00 DT10011001;
	 MT_DI00000X00 DT10011101;
	 MT_DI00000X00 DT10011201;

	 MT_DI02010X00 DT10011301;
	 MT_DI02020X00 DT10011401;
	 MT_DI02030X00 DT10011501;
	 MT_DI02040X00 DT10011601;
	 MT_DI02060X00 DT10011701;

	 MT_DI00000X00 DT10011801;
	 MT_DI00000X00 DT10011901;
	 MT_DI00000X00 DT10011A01;
	 MT_DI00000X00 DT10011B01;

	 MT_DI02010X00 DT10011C01;
	 MT_DI02020X00 DT10011D01;
	 MT_DI02030X00 DT10011E01;
	 MT_DI02040X00 DT10011F01;
	 MT_DI02060X00 DT10012001;
	 double        DT10012101;
	 double        DT10012201;
	 double        DT10012301;
	 double        DT10012401;

	 MT_DI_TIMEYS  DT10012501;
	 MT_DI00000X00 DT10012601;
	 MT_DI00000X00 DT10012701;
	 MT_DI00000X00 DT10012801;
	 MT_DI00000X00 DT10012901;

	 MT_DI00000X00 DT10012A01;
	 MT_DI00000X00 DT10012B01;
	 MT_DI00000X00 DT10012C01;
	 MT_DI00000X00 DT10012D01;

	 MT_DI00000X00 DT10012E01;
	 MT_DI00000X00 DT10012F01;
	 MT_DI00000X00 DT10013001;
	 MT_DI00000X00 DT10013101;

	 MT_DI00000X00 DT10013201;
	 MT_DI00000X00 DT10013301;
	 MT_DI00000X00 DT10013401;
	 MT_DI00000X00 DT10013501;
	 */

} MT_DI100XFF0X;

//电压逆相序记录

typedef struct tag_MT_DI1400FF0X {
	MT_DI_TIMEYS DT_STR_PS_ERR_TIME;

	MT_DI_LOSTV_TOTAL DT_STR_PS_ERR_TOTAL;

	MT_DI_LOSTV_TOTAL DT_STR_PS_ERR_PHASE[3];

	MT_DI_TIMEYS DT_END_PS_ERR_TIME;

	MT_DI_LOSTV_TOTAL DT_END_PS_ERR_TOTAL;

	MT_DI_LOSTV_TOTAL DT_END_PS_ERR_PHASE[3];

} MT_DI1400FF0X;
/*
 typedef struct tag_MT_DI1400FF0X
 {
 MT_DI_TIMEYS  DT14000101;
 MT_DI00000X00 DT14000201;
 MT_DI00000X00 DT14000301;
 MT_DI00000X00 DT14000401;
 MT_DI00000X00 DT14000501;
 MT_DI00000X00 DT14000601;
 MT_DI00000X00 DT14000701;
 MT_DI00000X00 DT14000801;
 MT_DI00000X00 DT14000901;
 MT_DI00000X00 DT14000A01;
 MT_DI00000X00 DT14000B01;
 MT_DI00000X00 DT14000C01;
 MT_DI00000X00 DT14000D01;
 MT_DI00000X00 DT14000E01;
 MT_DI00000X00 DT14000F01;
 MT_DI00000X00 DT14001001;
 MT_DI00000X00 DT14001101;
 MT_DI_TIMEYS  DT14001201;
 MT_DI00000X00 DT14001301;
 MT_DI00000X00 DT14001401;
 MT_DI00000X00 DT14001501;
 MT_DI00000X00 DT14001601;
 MT_DI00000X00 DT14001701;
 MT_DI00000X00 DT14001801;
 MT_DI00000X00 DT14001901;
 MT_DI00000X00 DT14001A01;
 MT_DI00000X00 DT14001B01;
 MT_DI00000X00 DT14001C01;
 MT_DI00000X00 DT14001D01;
 MT_DI00000X00 DT14001E01;
 MT_DI00000X00 DT14001F01;
 MT_DI00000X00 DT14002001;
 MT_DI00000X00 DT14002101;
 MT_DI00000X00 DT14002201;

 }MT_DI1400FF0X;

 */
//失流记录
typedef struct tag_MT_DI180XFF0X {
	MT_DI_TIMEYS DT_START_LI_TIME;

	MT_DI_LOSTV_TOTAL DT_START_LI_TOTAL;

	MT_DI_START_LOSTV_PHASE DT_START_LI_PHASE[3];

	MT_DI_TIMEYS DT_END_LI_TIME;

	MT_DI_LOSTV_TOTAL DT_END_LI_TOTAL;

	MT_DI_END_LOSTV_PHASE DT_END_LI_PHASE[3];

} MT_DI180XFF0X;

//电压、电流不平衡记录
typedef struct tag_MT_DI160XFF0X {
	MT_DI_TIMEYS DT_STR_PS_ERR_TIME;

	MT_DI_LOSTV_TOTAL DT_STR_PS_ERR_TOTAL;

	MT_DI_LOSTV_TOTAL DT_STR_PS_ERR_PHASE[3];

	MT_DI_TIMEYS DT_END_PS_ERR_TIME;

	MT_DI_LOSTV_TOTAL DT_END_PS_ERR_TOTAL;

	MT_DI_LOSTV_TOTAL DT_END_PS_ERR_PHASE[3];

	WORD Balance;
} MT_DI160XFF0X;
/*
 typedef struct tag_MT_DI180XFF0X
 {
 MT_DI_TIMEYS  DT10010101;
 MT_DI00000X00 DT10010201;
 MT_DI00000X00 DT10010301;
 MT_DI00000X00 DT10010401;
 MT_DI00000X00 DT10010501;
 MT_DI00000X00 DT10010601;
 MT_DI00000X00 DT10010701;
 MT_DI00000X00 DT10010801;
 MT_DI00000X00 DT10010901;

 MT_DI02010X00 DT10010A01;
 MT_DI02020X00 DT10010B01;
 MT_DI02030X00 DT10010C01;
 MT_DI02040X00 DT10010D01;
 MT_DI02060X00 DT10010E01;

 MT_DI00000X00 DT10010F01;
 MT_DI00000X00 DT10011001;
 MT_DI00000X00 DT10011101;
 MT_DI00000X00 DT10011201;

 MT_DI02010X00 DT10011301;
 MT_DI02020X00 DT10011401;
 MT_DI02030X00 DT10011501;
 MT_DI02040X00 DT10011601;
 MT_DI02060X00 DT10011701;

 MT_DI00000X00 DT10011801;
 MT_DI00000X00 DT10011901;
 MT_DI00000X00 DT10011A01;
 MT_DI00000X00 DT10011B01;

 MT_DI02010X00 DT10011C01;
 MT_DI02020X00 DT10011D01;
 MT_DI02030X00 DT10011E01;
 MT_DI02040X00 DT10011F01;
 MT_DI02060X00 DT10012001;

 MT_DI_TIMEYS  DT10012101;
 MT_DI00000X00 DT10012201;
 MT_DI00000X00 DT10012301;
 MT_DI00000X00 DT10012401;
 MT_DI00000X00 DT10012501;

 MT_DI00000X00 DT10012601;
 MT_DI00000X00 DT10012701;
 MT_DI00000X00 DT10012801;
 MT_DI00000X00 DT10012901;

 MT_DI00000X00 DT10012A01;
 MT_DI00000X00 DT10012B01;
 MT_DI00000X00 DT10012C01;
 MT_DI00000X00 DT10012D01;

 MT_DI00000X00 DT10012E01;
 MT_DI00000X00 DT10012F01;
 MT_DI00000X00 DT10013001;
 MT_DI00000X00 DT10013101;

 }MT_DI180XFF0X ;
 */

//功率因数超下限记录
typedef struct tag_MT_DI1F00FF0X {
	MT_DI_TIMEYS DT_START_PF_DOWN_TIME;

	MT_DI_LOSTV_TOTAL DT_START_PF_DOWN_TOTAL;

	MT_DI_TIMEYS DT_END_PF_DOWN_TIME;

	MT_DI_LOSTV_TOTAL DT_END_PF_DOWN_TOTAL;

} MT_DI1F00FF0X;

/*
 typedef struct tag_MT_DI1F00FF0X
 {
 MT_DI_TIMEYS  DT1F000101;
 MT_DI00000X00 DT1F000201;
 MT_DI00000X00 DT1F000301;
 MT_DI00000X00 DT1F000401;
 MT_DI00000X00 DT1F000501;
 MT_DI_TIMEYS  DT1F000601;
 MT_DI00000X00 DT1F000701;
 MT_DI00000X00 DT1F000801;
 MT_DI00000X00 DT1F000901;
 MT_DI00000X00 DT1F000A01;

 }MT_DI1F00FF0X;
 */

//全失压
typedef struct tag_MT_DI03050000 {
	DWORD timesAll;
	DWORD minsAll;
} MT_DI03050000;

//全失压记录
typedef struct tag_MT_DI0305000X {
	MT_DI_TIMEYS startTime;
	MT_DI02020X00 nCurrent;
	MT_DI_TIMEYS endTime;
} MT_DI0305000X;

//失电
typedef struct tag_MT_DI03060000 {
	DWORD timesAll;
	DWORD minsAll;
} MT_DI03060000;

//失电记录
typedef struct tag_MT_DI0306000X {
	MT_DI_TIMEYS startTime;
	MT_DI_TIMEYS endTime;
} MT_DI0306000X;

typedef struct tag_MT_DITIME_SEC {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	BYTE second;
} MT_DITIME_SEC;

//掉电
typedef struct tag_MT_DI03110000 {
	DWORD timesAll;
} MT_DI03110000;

//掉电记录
typedef struct tag_MT_DI0311000X {
	MT_DI_TIMEYS startTime;
	MT_DI_TIMEYS endTime;
} MT_DI0311000X;

//逆相序
typedef struct tag_MT_DI03070000 {
	DWORD timesAll;
	DWORD minsAll;
} MT_DI03070000;

//逆相序记录
typedef struct tag_MT_DI0307000X {
	MT_DI_TIMEYS startTime;
	MT_DI_TIMEYS endTime;
	MT_DI00000X00 nPosPEnergy;
	MT_DI00000X00 nNegPEnergy;
	MT_DI00000X00 nZhQ1Energy;
	MT_DI00000X00 nZhQ2Energy;

	MT_DI00000X00 nPosAPEnergy;
	MT_DI00000X00 nNegAPEnergy;
	MT_DI00000X00 nZhQ1AEnergy;
	MT_DI00000X00 nZhQ2AEnergy;

	MT_DI00000X00 nPosBPEnergy;
	MT_DI00000X00 nNegBPEnergy;
	MT_DI00000X00 nZhQ1BEnergy;
	MT_DI00000X00 nZhQ2BEnergy;

	MT_DI00000X00 nPosCPEnergy;
	MT_DI00000X00 nNegCPEnergy;
	MT_DI00000X00 nZhQ1CEnergy;
	MT_DI00000X00 nZhQ2CEnergy;
} MT_DI0307000X;

//失流次数、累计时间
typedef struct tag_MT_DI030B0000 {
	DWORD timesA;
	DWORD minsA;
	DWORD timesB;
	DWORD minsB;
	DWORD timesC;
	DWORD minsC;
} MT_DI030B0000;

//失流记录
typedef struct tag_MT_DI030B010X {
	MT_DI_TIMEYS startTime;
	MT_DI_TIMEYS endTime;
	MT_DI00000X00 nPosPEnergy;
	MT_DI00000X00 nNegPEnergy;
	MT_DI00000X00 nZhQ1Energy;
	MT_DI00000X00 nZhQ2Energy;

	MT_DI00000X00 nPosAPEnergy;
	MT_DI00000X00 nNegAPEnergy;
	MT_DI00000X00 nZhQ1AEnergy;
	MT_DI00000X00 nZhQ2AEnergy;
	MT_DI02010X00 UA;
	MT_DI02020X00 IA;
	MT_DI02030X00 PA;
	MT_DI02040X00 QA;
	MT_DI02060X00 PFA;

	MT_DI00000X00 nPosBPEnergy;
	MT_DI00000X00 nNegBPEnergy;
	MT_DI00000X00 nZhQ1BEnergy;
	MT_DI00000X00 nZhQ2BEnergy;
	MT_DI02010X00 UB;
	MT_DI02020X00 IB;
	MT_DI02030X00 PB;
	MT_DI02040X00 QB;
	MT_DI02060X00 PFB;

	MT_DI00000X00 nPosCPEnergy;
	MT_DI00000X00 nNegCPEnergy;
	MT_DI00000X00 nZhQ1CEnergy;
	MT_DI00000X00 nZhQ2CEnergy;
	MT_DI02010X00 UC;
	MT_DI02020X00 IC;
	MT_DI02030X00 PC;
	MT_DI02040X00 QC;
	MT_DI02060X00 PFC;

} MT_DI030B010X;

//编程次数
typedef struct tag_MT_DI03300000 {
	DWORD times;
} MT_DI03300000;

//编程记录
typedef struct tag_MT_DI0330000X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	DWORD programDI[10];
} MT_DI0330000X;

//电表清0次数
typedef struct tag_MT_DI03300100 {
	DWORD times;
} MT_DI03300100;

//电表清0记录
typedef struct tag_MT_DI0330010X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	MT_DI00000X00 nPosPEnergy;
	MT_DI00000X00 nNegPEnergy;
	MT_DI00000X00 nQ1Energy;
	MT_DI00000X00 nQ2Energy;
	MT_DI00000X00 nQ3Energy;
	MT_DI00000X00 nQ4Energy;

	MT_DI00000X00 nPosPAEnergy;
	MT_DI00000X00 nNegPAEnergy;
	MT_DI00000X00 nQ1AEnergy;
	MT_DI00000X00 nQ2AEnergy;
	MT_DI00000X00 nQ3AEnergy;
	MT_DI00000X00 nQ4AEnergy;

	MT_DI00000X00 nPosPBEnergy;
	MT_DI00000X00 nNegPBEnergy;
	MT_DI00000X00 nQ1BEnergy;
	MT_DI00000X00 nQ2BEnergy;
	MT_DI00000X00 nQ3BEnergy;
	MT_DI00000X00 nQ4BEnergy;

	MT_DI00000X00 nPosPCEnergy;
	MT_DI00000X00 nNegPCEnergy;
	MT_DI00000X00 nQ1CEnergy;
	MT_DI00000X00 nQ2CEnergy;
	MT_DI00000X00 nQ3CEnergy;
	MT_DI00000X00 nQ4CEnergy;

} MT_DI0330010X;

//需量清0次数
typedef struct tag_MT_DI03300200 {
	DWORD times;
} MT_DI03300200;

//需量清0记录
typedef struct tag_MT_DI0330020X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	MT_DI010X0X00 nPosPDemand;
	MT_DI010X0X00 nNegPDemand;
	MT_DI010X0X00 nQ1Demand;
	MT_DI010X0X00 nQ2Demand;
	MT_DI010X0X00 nQ3Demand;
	MT_DI010X0X00 nQ4Demand;

	MT_DI010X0X00 nPosPADemand;
	MT_DI010X0X00 nNegPADemand;
	MT_DI010X0X00 nQ1ADemand;
	MT_DI010X0X00 nQ2ADemand;
	MT_DI010X0X00 nQ3ADemand;
	MT_DI010X0X00 nQ4ADemand;

	MT_DI010X0X00 nPosPBDemand;
	MT_DI010X0X00 nNegPBDemand;
	MT_DI010X0X00 nQ1BDemand;
	MT_DI010X0X00 nQ2BDemand;
	MT_DI010X0X00 nQ3BDemand;
	MT_DI010X0X00 nQ4BDemand;

	MT_DI010X0X00 nPosPCDemand;
	MT_DI010X0X00 nNegPCDemand;
	MT_DI010X0X00 nQ1CDemand;
	MT_DI010X0X00 nQ2CDemand;
	MT_DI010X0X00 nQ3CDemand;
	MT_DI010X0X00 nQ4CDemand;

} MT_DI0330020X;

//事件清0次数
typedef struct tag_MT_DI03300300 {
	DWORD times;
} MT_DI03300300;

//事件清0记录
typedef struct tag_MT_DI0330030X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	DWORD ercDI;
} MT_DI0330030X;

//校时次数
typedef struct tag_MT_DI03300400 {
	DWORD times;
} MT_DI03300400;

//校时记录
typedef struct tag_MT_DI0330040X {
	DWORD handler;
	MT_DI_TIMEYS timeBefore;
	MT_DI_TIMEYS timeAfter;

} MT_DI0330040X;

//时段表编程总次数
typedef struct tag_MT_DI03300500 {
	DWORD times;
} MT_DI03300500;

//时段表编程记录
typedef struct tag_MT_DI0330050X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI040100FF DT040100FF;
	EM_DI040200FF DT040200FF;
} MT_DI0330050X;

//时区表编程总次数
typedef struct tag_MT_DI03300600 {
	DWORD times;
} MT_DI03300600;

//时区表编程记录
typedef struct tag_MT_DI0330060X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI04010000 DT04010000;
	EM_DI04020000 DT04020000;
} MT_DI0330060X;

//周休日编程总次数
typedef struct tag_MT_DI03300700 {
	DWORD times;
} MT_DI03300700;

//周休日编程记录
typedef struct tag_MT_DI0330070X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI04000802 DT04000802;
} MT_DI0330070X;

//节假日编程总次数
typedef struct tag_MT_DI03300800 {
	DWORD times;
} MT_DI03300800;

//节假日编程记录
typedef struct tag_MT_DI0330080X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI040300FF DT040300FF;
} MT_DI0330080X;

//有功组合方式编程总次数
typedef struct tag_MT_DI03300900 {
	DWORD times;
} MT_DI03300900;

//有功组合方式编程记录
typedef struct tag_MT_DI0330090X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI0400060X DT040000601;
} MT_DI0330090X;

//无功组合方式1编程总次数
typedef struct tag_MT_DI03300A00 {
	DWORD times;
} MT_DI03300A00;

//无功组合方式1编程记录
typedef struct tag_MT_DI03300A0X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI0400060X DT040000602;
} MT_DI03300A0X;

//无功组合方式2编程总次数
typedef struct tag_MT_DI03300B00 {
	DWORD times;
} MT_DI03300B00;

//无功组合方式2编程记录
typedef struct tag_MT_DI03300B0X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI0400060X DT040000603;
} MT_DI03300B0X;

//结算日编程总次数
typedef struct tag_MT_DI03300C00 {
	DWORD times;
} MT_DI03300C00;

//结算日编程记录
typedef struct tag_MT_DI03300C0X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI04000BFF DT04000BFF;
} MT_DI03300C0X;

//开表盖编程总次数
typedef struct tag_MT_DI03300D00 {
	DWORD times;
} MT_DI03300D00;

//开表盖编程记录
typedef struct tag_MT_DI03300D0X {
	MT_DI_TIMEYS happenStartTime;
	MT_DI_TIMEYS happenEndTime;
	MT_DI00000X00 DT00010000A;
	MT_DI00000X00 DT00020000A;
	MT_DI00000X00 DT00050000A;
	MT_DI00000X00 DT00060000A;
	MT_DI00000X00 DT00070000A;
	MT_DI00000X00 DT00080000A;

	MT_DI00000X00 DT00010000B;
	MT_DI00000X00 DT00020000B;
	MT_DI00000X00 DT00050000B;
	MT_DI00000X00 DT00060000B;
	MT_DI00000X00 DT00070000B;
	MT_DI00000X00 DT00080000B;

} MT_DI03300D0X;

//开端钮盒编程总次数
typedef struct tag_MT_DI03300E00 {
	DWORD times;
} MT_DI03300E00;

//开端钮盒编程记录
typedef struct tag_MT_DI03300E0X {
	MT_DI_TIMEYS happenTime;
	DWORD handler;
	EM_DI04010000 DT04010000;
	EM_DI04020000 DT04020000;
} MT_DI03300E0X;

//定时冻结时间
typedef struct tag_MT_DI0500000X {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;

} MT_DI0500000X;

typedef struct tag_MT_DI0500100X {
	MT_DI0203FF00 power;
	MT_DI0204FF00 inpower;

} MT_DI0500100X;

//冻结数据
typedef struct tag_MT_DI050XFF0X {
	MT_DI0500000X DT05000001;
	MT_DI000XFF00 DT05000101;
	MT_DI000XFF00 DT05000201;
	MT_DI000XFF00 DT05000301;
	MT_DI000XFF00 DT05000401;
	MT_DI000XFF00 DT05000501;
	MT_DI000XFF00 DT05000601;
	MT_DI000XFF00 DT05000701;
	MT_DI000XFF00 DT05000801;
	MT_DI010XFF00 DT05000901;
	MT_DI010XFF00 DT05000A01;
	MT_DI0500100X DT05001001;
} MT_DI050XFF0X;

//冻结时间参数结构
typedef struct tag_EM_DI04820000 {
	BYTE minute;
	BYTE hour;
	BYTE day;
	BYTE month;

} EM_DI04820000;

#ifdef __cplusplus
}
#endif

#endif /* INTERMEDIUM_PCOLS_INC_FA_PROTOCOL_H_ */
