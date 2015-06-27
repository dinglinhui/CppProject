#ifndef __EM_PROTOCOL_H__
#define __EM_PROTOCOL_H__

#include <time.h>
#include "OSDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************* 预定义参数 ********************/
#ifndef LINYANG_EM_MAX_ENERGY
#define LINYANG_EM_MAX_ENERGY				999999.99
#endif

//最大时段数
#ifndef	 LINYANG_EM_MAX_TIMEMASK
#define  LINYANG_EM_MAX_TIMEMASK			14
#endif

//最大费率数
#ifndef LINYANG_EM_MAX_TARIFFNUM
#define LINYANG_EM_MAX_TARIFFNUM			4
#endif

//最大年时区数
#ifndef LINYANG_EM_MAX_SEASON
#define LINYANG_EM_MAX_SEASON				14
#endif

//最大日时段表数
#ifndef LINYANG_EM_MAX_TIMETABLE
#define LINYANG_EM_MAX_TIMETABLE			8
#endif

//最大公共假日数
#ifndef LINYANG_EM_MAX_HOLIDAY
#define LINYANG_EM_MAX_HOLIDAY				10
#endif

//最大自动巡显屏数
#ifndef LINYANG_EM_MAX_AUTODISPLAY
#define LINYANG_EM_MAX_AUTODISPLAY			254
#endif

//最大按键巡显屏数
#ifndef LINYANG_EM_MAX_HANDDISPLAY
#define LINYANG_EM_MAX_HANDDISPLAY			254
#endif

/******************* 预定义(根据硬件)参数结束 ********************/

typedef union tag_EM_C {
	BYTE body;
	struct {
		BYTE CMD :5;   // 命令
		BYTE SERIES :1;   // 后续帧标记
		BYTE ACD :1;   // 异常标志
		BYTE DIR :1;   // 传输方向
	} mask;
} EM_C;

typedef enum tag_EM_FUNCTION {
	EM_CMD_WITHHOLD = 0x00,  // 保留
	EM_CMD_READDATA = 0x01,  // 读数据
	EM_CMD_READSERIESDATA = 0x02,  // 读后续数据
	EM_CMD_READDATAAGAIN = 0x03,  // 重读数据
	EM_CMD_WRITEDATA = 0x04,  // 写数据
	EM_CMD_CHECKTIME = 0x08,  // 广播对时
	EM_CMD_WRITEADDRESS = 0x0A,  // 写设备地址
	EM_CMD_CHANGERATE = 0x0C,  // 更改通信速率
	EM_CMD_READADDRESS = 0x0D,  // 广播读取地址2009.12.29gs add
	EM_CMD_CHANGEPASSWORD = 0x0F,  // 更改密码
	EM_CMD_CLEARXL = 0x10,  // 最大需量清零
	EM_CMD_CONGEAL = 0x12,  // 广播冻结电量
	EM_CMD_CLEARALL = 0x1A,   // 总清

	EM_CMD_READDATA_07 = 0x11,  // 读数据
	EM_CMD_READSERIESDATA_07 = 0x12,  // 读后续数据
	EM_CMD_READADDRESS_07 = 0x13,  // 读通讯地址
	EM_CMD_WRITEDATA_07 = 0x14,  // 写数据
	EM_CMD_CHECKTIME_07 = 0x08,  // 广播对时
	EM_CMD_WRITEADDRESS_07 = 0x15,  // 写设备地址
	EM_CMD_CHANGERATE_07 = 0x17,  // 更改通信速率
	EM_CMD_CHANGEPASSWORD_07 = 0x18,  // 更改密码
	EM_CMD_CLEARXL_07 = 0x19,  // 最大需量清零
	EM_CMD_CONGEAL_07 = 0x16,  // 广播冻结电量
	EM_CMD_CLEARALL_07 = 0x1A,   // 电表清零
	EM_CMD_CLEARRECORD_07 = 0x1B   // 事件清零
} EM_FUNCTION;

typedef struct tag_EM_UNIT {
	BYTE nItem;
	const char * Name;
} EM_UINT;

static const EM_UINT EM_UNIT_DEFINE[] = {
		//nItem,	Nmae
		{ 0, nullptr }, { 1, "V" }, { 2, "A" }, { 3, "kV" }, { 4, "kA" }, { 5,
				"kW" }, { 6, "kvar" }, { 7, "kWh" }, { 8, "kvarh" }, { 9, "%" },
		{ 10, "分" }, { 11, "秒" }, { 12, "BIT/S" }, { 13, "kVA" }, { 14, "度" }, {
				15, "MDHM" }, };

typedef struct tag_EM_DATA_DEFINE {
	DWORD DI;							//标识编码
	BYTE Plus;						//编码范围(增量值,内部使用)

	union {
		BYTE body;					//隐码
		struct {
			BYTE READ :1;		//读功能
			BYTE WRITE :1;		//写功能
			BYTE SAVE :1;
			BYTE VALID :1;		//
			BYTE unit :4;		//单位，具体见 FA_UNIT_DEFINE 的定义
		} body_mask;
	} Access;
	WORD Length;						//数据格式长度(归约定义的长度)
	//
	void *DataPtr;					//数据体
	WORD nDataLength;				//数据体长度(内部定义的数据结构长度)
	//
	const char *Name;					//数据项名称
} EM_DATA_DEFINE, EM_DATA, *PEM_DATA;

///////////////////////////////////////////////////////////////////
//电量
///////////////////////////////////////////////////////////////////
//当前
typedef struct tag_EM_DI901X {
	double value;
} EM_DI901X;

typedef struct tag_EM_DI901F {
	EM_DI901X DI901[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI901F;

typedef struct tag_EM_DI902X {
	double value;
} EM_DI902X;

typedef struct tag_EM_DI902F {
	EM_DI902X DI902[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI902F;

typedef struct tag_EM_DI911X {
	double value;
} EM_DI911X;

typedef struct tag_EM_DI911F {
	EM_DI911X DI911[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI911F;

typedef struct tag_EM_DI912X {
	double value;
} EM_DI912X;

typedef struct tag_EM_DI912F {
	EM_DI912X DI912[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI912F;

typedef struct tag_EM_DI913X {
	double value;
} EM_DI913X;

typedef struct tag_EM_DI913F {
	EM_DI913X DI913[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI913F;

typedef struct tag_EM_DI914X {
	double value;
} EM_DI914X;

typedef struct tag_EM_DI914F {
	EM_DI914X DI914[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI914F;

typedef struct tag_EM_DI915X {
	double value;
} EM_DI915X;

typedef struct tag_EM_DI915F {
	EM_DI915X DI915[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI915F;

typedef struct tag_EM_DI916X {
	double value;
} EM_DI916X;

typedef struct tag_EM_DI916F {
	EM_DI916X DI916[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI916F;

//上月电量
typedef struct tag_EM_DI941X {
	double value;
} EM_DI941X;

typedef struct tag_EM_DI941F {
	EM_DI941X DI941[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI941F;

typedef struct tag_EM_DI942X {
	double value;
} EM_DI942X;

typedef struct tag_EM_DI942F {
	EM_DI942X DI942[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI942F;

typedef struct tag_EM_DI951X {
	double value;
} EM_DI951X;

typedef struct tag_EM_DI951F {
	EM_DI951X DI951[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI951F;

typedef struct tag_EM_DI952X {
	double value;
} EM_DI952X;

typedef struct tag_EM_DI952F {
	EM_DI952X DI952[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI952F;

typedef struct tag_EM_DI953X {
	double value;
} EM_DI953X;

typedef struct tag_EM_DI953F {
	EM_DI953X DI953[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI953F;

typedef struct tag_EM_DI954X {
	double value;
} EM_DI954X;

typedef struct tag_EM_DI954F {
	EM_DI954X DI954[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI954F;

typedef struct tag_EM_DI955X {
	double value;
} EM_DI955X;

typedef struct tag_EM_DI955F {
	EM_DI955X DI955[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI955F;

typedef struct tag_EM_DI956X {
	double value;
} EM_DI956X;

typedef struct tag_EM_DI956F {
	EM_DI956X DI956[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI956F;

//上上月电量
typedef struct tag_EM_DI981X {
	double value;
} EM_DI981X;

typedef struct tag_EM_DI981F {
	EM_DI981X DI981[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI981F;

typedef struct tag_EM_DI982X {
	double value;
} EM_DI982X;

typedef struct tag_EM_DI982F {
	EM_DI982X DI982[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI982F;

typedef struct tag_EM_DI991X {
	double value;
} EM_DI991X;

typedef struct tag_EM_DI991F {
	EM_DI991X DI991[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI991F;

typedef struct tag_EM_DI992X {
	double value;
} EM_DI992X;

typedef struct tag_EM_DI992F {
	EM_DI992X DI992[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI992F;

typedef struct tag_EM_DI993X {
	double value;
} EM_DI993X;

typedef struct tag_EM_DI993F {
	EM_DI993X DI993[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI993F;

typedef struct tag_EM_DI994X {
	double value;
} EM_DI994X;

typedef struct tag_EM_DI994F {
	EM_DI994X DI994[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI994F;

typedef struct tag_EM_DI995X {
	double value;
} EM_DI995X;

typedef struct tag_EM_DI995F {
	EM_DI995X DI995[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI995F;

typedef struct tag_EM_DI996X {
	double value;
} EM_DI996X;

typedef struct tag_EM_DI996F {
	EM_DI996X DI996[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DI996F;

typedef struct tag_EM_ENERGY {
	EM_DI901F DI901F;
	EM_DI902F DI902F;
	EM_DI911F DI911F;
	EM_DI912F DI912F;
	EM_DI913F DI913F;
	EM_DI914F DI914F;
	EM_DI915F DI915F;
	EM_DI916F DI916F;
	//
	EM_DI941F DI941F;
	EM_DI942F DI942F;
	EM_DI951F DI951F;
	EM_DI952F DI952F;
	EM_DI953F DI953F;
	EM_DI954F DI954F;
	EM_DI955F DI955F;
	EM_DI956F DI956F;
	//
	EM_DI981F DI981F;
	EM_DI982F DI982F;
	EM_DI991F DI991F;
	EM_DI992F DI992F;
	EM_DI993F DI993F;
	EM_DI994F DI994F;
	EM_DI995F DI995F;
	EM_DI996F DI996F;
} EM_ENERGY;

////////////////////////////////////////////////////////////////
//最大需量
// 补充07规约需量定义
typedef struct tag_EM_DIAB0X {
	double value;
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIAB0X;

////////////////////////////////////////////////////////////////
//当前
typedef struct tag_EM_DIA01X {
	double value;
} EM_DIA01X;

typedef struct tag_EM_DIA01F {
	EM_DIA01X DIA01[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA01F;

typedef struct tag_EM_DIA02X {
	double value;
} EM_DIA02X;

typedef struct tag_EM_DIA02F {
	EM_DIA02X DIA02[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA02F;

typedef struct tag_EM_DIA11X {
	double value;
} EM_DIA11X;

typedef struct tag_EM_DIA11F {
	EM_DIA11X DIA11[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA11F;

typedef struct tag_EM_DIA12X {
	double value;
} EM_DIA12X;

typedef struct tag_EM_DIA12F {
	EM_DIA12X DIA12[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA12F;

typedef struct tag_EM_DIA13X {
	double value;
} EM_DIA13X;

typedef struct tag_EM_DIA13F {
	EM_DIA13X DIA13[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA13F;

typedef struct tag_EM_DIA14X {
	double value;
} EM_DIA14X;

typedef struct tag_EM_DIA14F {
	EM_DIA14X DIA14[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA14F;

typedef struct tag_EM_DIA15X {
	double value;
} EM_DIA15X;

typedef struct tag_EM_DIA15F {
	EM_DIA15X DIA15[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA15F;

typedef struct tag_EM_DIA16X {
	double value;
} EM_DIA16X;

typedef struct tag_EM_DIA16F {
	EM_DIA16X DIA16[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA16F;

//上月需量
typedef struct tag_EM_DIA41X {
	double value;
} EM_DIA41X;

typedef struct tag_EM_DIA41F {
	EM_DIA41X DIA41[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA41F;

typedef struct tag_EM_DIA42X {
	double value;
} EM_DIA42X;

typedef struct tag_EM_DIA42F {
	EM_DIA42X DIA42[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA42F;

typedef struct tag_EM_DIA51X {
	double value;
} EM_DIA51X;

typedef struct tag_EM_DIA51F {
	EM_DIA51X DIA51[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA51F;

typedef struct tag_EM_DIA52X {
	double value;
} EM_DIA52X;

typedef struct tag_EM_DIA52F {
	EM_DIA52X DIA52[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA52F;

typedef struct tag_EM_DIA53X {
	double value;
} EM_DIA53X;

typedef struct tag_EM_DIA53F {
	EM_DIA53X DIA53[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA53F;

typedef struct tag_EM_DIA54X {
	double value;
} EM_DIA54X;

typedef struct tag_EM_DIA54F {
	EM_DIA54X DIA54[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA54F;

typedef struct tag_EM_DIA55X {
	double value;
} EM_DIA55X;

typedef struct tag_EM_DIA55F {
	EM_DIA55X DIA55[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA55F;

typedef struct tag_EM_DIA56X {
	double value;
} EM_DIA56X;

typedef struct tag_EM_DIA56F {
	EM_DIA56X DIA56[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA56F;

//上上月需量
typedef struct tag_EM_DIA81X {
	double value;
} EM_DIA81X;

typedef struct tag_EM_DIA81F {
	EM_DIA81X DIA81[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA81F;

typedef struct tag_EM_DIA82X {
	double value;
} EM_DIA82X;

typedef struct tag_EM_DIA82F {
	EM_DIA82X DIA82[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA82F;

typedef struct tag_EM_DIA91X {
	double value;
} EM_DIA91X;

typedef struct tag_EM_DIA91F {
	EM_DIA91X DIA91[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA91F;

typedef struct tag_EM_DIA92X {
	double value;
} EM_DIA92X;

typedef struct tag_EM_DIA92F {
	EM_DIA92X DIA92[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA92F;

typedef struct tag_EM_DIA93X {
	double value;
} EM_DIA93X;

typedef struct tag_EM_DIA93F {
	EM_DIA93X DIA93[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA93F;

typedef struct tag_EM_DIA94X {
	double value;
} EM_DIA94X;

typedef struct tag_EM_DIA94F {
	EM_DIA94X DIA94[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA94F;

typedef struct tag_EM_DIA95X {
	double value;
} EM_DIA95X;

typedef struct tag_EM_DIA95F {
	EM_DIA95X DIA95[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA95F;

typedef struct tag_EM_DIA96X {
	double value;
} EM_DIA96X;

typedef struct tag_EM_DIA96F {
	EM_DIA96X DIA96[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIA96F;

typedef struct tag_EM_DEMAND {
	EM_DIA01F DIA01F;
	EM_DIA02F DIA02F;
	EM_DIA11F DIA11F;
	EM_DIA12F DIA12F;
	EM_DIA13F DIA13F;
	EM_DIA14F DIA14F;
	EM_DIA15F DIA15F;
	EM_DIA16F DIA16F;
	//
	EM_DIA41F DIA41F;
	EM_DIA42F DIA42F;
	EM_DIA51F DIA51F;
	EM_DIA52F DIA52F;
	EM_DIA53F DIA53F;
	EM_DIA54F DIA54F;
	EM_DIA55F DIA55F;
	EM_DIA56F DIA56F;
	//
	EM_DIA81F DIA81F;
	EM_DIA82F DIA82F;
	EM_DIA91F DIA91F;
	EM_DIA92F DIA92F;
	EM_DIA93F DIA93F;
	EM_DIA94F DIA94F;
	EM_DIA95F DIA95F;
	EM_DIA96F DIA96F;
} EM_DEMAND;

//
////////////////////////////////////////////////////////////////
//最大需量发生时间
////////////////////////////////////////////////////////////////
//当前最大需量发生时间
typedef struct tag_EM_DIB01X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB01X;

typedef struct tag_EM_DIB01F {
	EM_DIB01X DIB01[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB01F;

typedef struct tag_EM_DIB02X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB02X;

typedef struct tag_EM_DIB02F {
	EM_DIB02X DIB02[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB02F;

typedef struct tag_EM_DIB11X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB11X;

typedef struct tag_EM_DIB11F {
	EM_DIB11X DIB11[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB11F;

typedef struct tag_EM_DIB12X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB12X;

typedef struct tag_EM_DIB12F {
	EM_DIB12X DIB12[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB12F;

typedef struct tag_EM_DIB13X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB13X;

typedef struct tag_EM_DIB13F {
	EM_DIB13X DIB13[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB13F;

typedef struct tag_EM_DIB14X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB14X;

typedef struct tag_EM_DIB14F {
	EM_DIB14X DIB14[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB14F;

typedef struct tag_EM_DIB15X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB15X;

typedef struct tag_EM_DIB15F {
	EM_DIB15X DIB15[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB15F;

typedef struct tag_EM_DIB16X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB16X;

typedef struct tag_EM_DIB16F {
	EM_DIB16X DIB16[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB16F;

//上月需量发生时间
typedef struct tag_EM_DIB41X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB41X;

typedef struct tag_EM_DIB41F {
	EM_DIB41X DIB41[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB41F;

typedef struct tag_EM_DIB42X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB42X;

typedef struct tag_EM_DIB42F {
	EM_DIB42X DIB42[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB42F;

typedef struct tag_EM_DIB51X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB51X;

typedef struct tag_EM_DIB51F {
	EM_DIB51X DIB51[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB51F;

typedef struct tag_EM_DIB52X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB52X;

typedef struct tag_EM_DIB52F {
	EM_DIB52X DIB52[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB52F;

typedef struct tag_EM_DIB53X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB53X;

typedef struct tag_EM_DIB53F {
	EM_DIB53X DIB53[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB53F;

typedef struct tag_EM_DIB54X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB54X;

typedef struct tag_EM_DIB54F {
	EM_DIB54X DIB54[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB54F;

typedef struct tag_EM_DIB55X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB55X;

typedef struct tag_EM_DIB55F {
	EM_DIB55X DIB55[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB55F;

typedef struct tag_EM_DIB56X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB56X;

typedef struct tag_EM_DIB56F {
	EM_DIB56X DIB56[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB56F;

//上上月需量发生时间
typedef struct tag_EM_DIB81X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB81X;

typedef struct tag_EM_DIB81F {
	EM_DIB81X DIB81[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB81F;

typedef struct tag_EM_DIB82X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB82X;

typedef struct tag_EM_DIB82F {
	EM_DIB82X DIB82[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB82F;

typedef struct tag_EM_DIB91X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB91X;

typedef struct tag_EM_DIB91F {
	EM_DIB91X DIB91[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB91F;

typedef struct tag_EM_DIB92X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB92X;

typedef struct tag_EM_DIB92F {
	EM_DIB92X DIB92[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB92F;

typedef struct tag_EM_DIB93X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB93X;

typedef struct tag_EM_DIB93F {
	EM_DIB93X DIB93[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB93F;

typedef struct tag_EM_DIB94X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB94X;

typedef struct tag_EM_DIB94F {
	EM_DIB94X DIB94[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB94F;

typedef struct tag_EM_DIB95X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB95X;

typedef struct tag_EM_DIB95F {
	EM_DIB95X DIB95[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB95F;

typedef struct tag_EM_DIB96X {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB96X;

typedef struct tag_EM_DIB96F {
	EM_DIB96X DIB96[LINYANG_EM_MAX_TARIFFNUM + 1];
} EM_DIB96F;

typedef struct tag_EM_DEMAND_TIME {
	EM_DIB01F DIB01F;
	EM_DIB02F DIB02F;
	EM_DIB11F DIB11F;
	EM_DIB12F DIB12F;
	EM_DIB13F DIB13F;
	EM_DIB14F DIB14F;
	EM_DIB15F DIB15F;
	EM_DIB16F DIB16F;
	//
	EM_DIB41F DIB41F;
	EM_DIB42F DIB42F;
	EM_DIB51F DIB51F;
	EM_DIB52F DIB52F;
	EM_DIB53F DIB53F;
	EM_DIB54F DIB54F;
	EM_DIB55F DIB55F;
	EM_DIB56F DIB56F;
	//
	EM_DIB81F DIB81F;
	EM_DIB82F DIB82F;
	EM_DIB91F DIB91F;
	EM_DIB92F DIB92F;
	EM_DIB93F DIB93F;
	EM_DIB94F DIB94F;
	EM_DIB95F DIB95F;
	EM_DIB96F DIB96F;
} EM_DEMAND_TIME;

//
////////////////////////////////////////////////////////////////
//变量
////////////////////////////////////////////////////////////////

//最近一次编程时间
typedef struct tag_EM_DIB210 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB210;

typedef struct tag_EM_DIB210_07 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	BYTE sec;
} EM_DIB210_07;

//最近一次最大需量清零时间
typedef struct tag_EM_DIB211 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB211;

//编程次数
typedef struct tag_EM_DIB212 {
	WORD times;
} EM_DIB212;

//最大需量清零次数
typedef struct tag_EM_DIB213 {
	WORD times;
} EM_DIB213;

//电池工作时间
typedef struct tag_EM_DIB214 {
	DWORD time;
} EM_DIB214;

// 扩展
// 清零总次数
typedef struct tag_EM_DIB212_07 {
	DWORD times;
} EM_DIB212_07;

// 扩展
typedef struct tag_EM_DIB21F {
	EM_DIB210 DIB210;
	EM_DIB211 DIB211;
	EM_DIB212 DIB212;
	EM_DIB213 DIB213;
	EM_DIB214 DIB214;
} EM_DIB21F;

//总断相次数
typedef struct tag_EM_DIB310 {
	WORD times;
} EM_DIB310;

//A 相断相次数
typedef struct tag_EM_DIB311 {
	WORD times;
} EM_DIB311;

//B 相断相次数
typedef struct tag_EM_DIB312 {
	WORD times;
} EM_DIB312;

//C 相断相次数
typedef struct tag_EM_DIB313 {
	WORD times;
} EM_DIB313;

typedef struct tag_EM_DIB31F {
	EM_DIB310 DIB310;
	EM_DIB311 DIB311;
	EM_DIB312 DIB312;
	EM_DIB313 DIB313;
} EM_DIB31F;

//断相时间累计值
typedef struct tag_EM_DIB320 {
	DWORD time;
} EM_DIB320;

//A断相时间累计值
typedef struct tag_EM_DIB321 {
	DWORD time;
} EM_DIB321;

//B断相时间累计值
typedef struct tag_EM_DIB322 {
	DWORD time;
} EM_DIB322;

//C断相时间累计值
typedef struct tag_EM_DIB323 {
	DWORD time;
} EM_DIB323;

typedef struct tag_EM_DIB32F {
	EM_DIB320 DIB320;
	EM_DIB321 DIB321;
	EM_DIB322 DIB322;
	EM_DIB323 DIB323;
} EM_DIB32F;

//最近一次断相起始时刻
typedef struct tag_EM_DIB330 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB330;

//A最近一次断相起始时刻
typedef struct tag_EM_DIB331 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB331;

//B最近一次断相起始时刻
typedef struct tag_EM_DIB332 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB332;

//C最近一次断相起始时刻
typedef struct tag_EM_DIB333 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB333;

typedef struct tag_EM_DIB33F {
	EM_DIB330 DIB330;
	EM_DIB331 DIB331;
	EM_DIB332 DIB332;
	EM_DIB333 DIB333;
} EM_DIB33F;

//最近一次断相的结束时刻
typedef struct tag_EM_DIB340 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB340;

//A最近一次断相的结束时刻
typedef struct tag_EM_DIB341 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB341;

//B最近一次断相的结束时刻
typedef struct tag_EM_DIB342 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB342;

//C最近一次断相的结束时刻
typedef struct tag_EM_DIB343 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIB343;

typedef struct tag_EM_DIB34F {
	EM_DIB340 DIB340;
	EM_DIB341 DIB341;
	EM_DIB342 DIB342;
	EM_DIB343 DIB343;
} EM_DIB34F;

// 补充07规约
typedef struct tag_EM_DI03040000 {
	DWORD dwCount[3];
	DWORD dwTimes[3];
} EM_DI03040000;

typedef struct tag_EM_DI130X00FF {
	DWORD dwCount;
	DWORD dwTimes;
} EM_DI130X00FF;

typedef struct tag_EM_RECORDTIME {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	BYTE second;
} EM_RECORDTIME;

typedef struct tag_EM_DI03040101 {
	EM_RECORDTIME begin;
	EM_RECORDTIME end;
} EM_DI03040101;

//A 相电压
typedef struct tag_EM_DIB611 {
	WORD value;
} EM_DIB611;

//B 相电压
typedef struct tag_EM_DIB612 {
	WORD value;
} EM_DIB612;

//C 相电压
typedef struct tag_EM_DIB613 {
	WORD value;
} EM_DIB613;

typedef struct tag_EM_DIB61F {
	EM_DIB611 DIB611;
	EM_DIB612 DIB612;
	EM_DIB613 DIB613;
} EM_DIB61F;

//A 相电流
typedef struct tag_EM_DIB621 {
	double value;
} EM_DIB621;

//B 相电流
typedef struct tag_EM_DIB622 {
	double value;
} EM_DIB622;

//C 相电流
typedef struct tag_EM_DIB623 {
	double value;
} EM_DIB623;

typedef struct tag_EM_DIB62F {
	EM_DIB621 DIB621;
	EM_DIB622 DIB622;
	EM_DIB623 DIB623;
} EM_DIB62F;

//瞬时有功功率
typedef struct tag_EM_DIB630 {
	double value;
} EM_DIB630;

//A 相有功功率
typedef struct tag_EM_DIB631 {
	double value;
} EM_DIB631;

//B 相有功功率
typedef struct tag_EM_DIB632 {
	double value;
} EM_DIB632;

//C 相有功功率
typedef struct tag_EM_DIB633 {
	double value;
} EM_DIB633;

typedef struct tag_EM_DIB63F {
	EM_DIB630 DIB630;
	EM_DIB631 DIB631;
	EM_DIB632 DIB632;
	EM_DIB633 DIB633;
} EM_DIB63F;

//正向有功功率上限值
typedef struct tag_EM_DIB634 {
	double value;
} EM_DIB634;

//反向有功功率上限值
typedef struct tag_EM_DIB635 {
	double value;
} EM_DIB635;

//瞬时无功功率
typedef struct tag_EM_DIB640 {
	double value;
} EM_DIB640;

//A 相无功功率
typedef struct tag_EM_DIB641 {
	double value;
} EM_DIB641;

//B 相无功功率
typedef struct tag_EM_DIB642 {
	double value;
} EM_DIB642;

//C 相无功功率
typedef struct tag_EM_DIB643 {
	double value;
} EM_DIB643;

typedef struct tag_EM_DIB64F {
	EM_DIB640 DIB640;
	EM_DIB641 DIB641;
	EM_DIB642 DIB642;
	EM_DIB643 DIB643;
} EM_DIB64F;

//总功率因数
typedef struct tag_EM_DIB650 {
	double value;
} EM_DIB650;

//A 相功率因数
typedef struct tag_EM_DIB651 {
	double value;
} EM_DIB651;

//B 相功率因数
typedef struct tag_EM_DIB652 {
	double value;
} EM_DIB652;

//C 相功率因数
typedef struct tag_EM_DIB653 {
	double value;
} EM_DIB653;

typedef struct tag_EM_DIB65F {
	EM_DIB650 DIB650;
	EM_DIB651 DIB651;
	EM_DIB652 DIB652;
	EM_DIB653 DIB653;
} EM_DIB65F;

typedef struct tag_EM_DIB660 {
	double value;
} EM_DIB660;

typedef struct tag_EM_DIB661 {
	double value;
} EM_DIB661;

typedef struct tag_EM_DIB662 {
	double value;
} EM_DIB662;

typedef struct tag_EM_DIB663 {
	double value;
} EM_DIB663;

typedef struct tag_EM_DIB664 {
	double value;
} EM_DIB664;

typedef struct tag_EM_DIB665 {
	double value;
} EM_DIB665;

typedef struct tag_EM_DIB66F {
	EM_DIB660 DIB660;
	EM_DIB661 DIB661;
	EM_DIB662 DIB662;
	EM_DIB663 DIB663;
	EM_DIB664 DIB664;
	EM_DIB665 DIB665;
} EM_DIB66F;

typedef struct tag_EM_DIB66F_07 {
	EM_DIB660 DIB660;
	EM_DIB661 DIB661;
	EM_DIB662 DIB662;
} EM_DIB66F_07;

typedef struct tag_EM_VOLATILE {
	EM_DIB21F DIB21F;
	EM_DIB214 DIB214;
	EM_DIB31F DIB31F;
	EM_DIB32F DIB32F;
	EM_DIB33F DIB33F;
	EM_DIB34F DIB34F;
	EM_DIB61F DIB61F;
	EM_DIB62F DIB62F;
	EM_DIB63F DIB63F;
	EM_DIB634 DIB634;
	EM_DIB635 DIB635;
	EM_DIB64F DIB64F;
	EM_DIB65F DIB65F;
	EM_DIB66F DIB66F;
} EM_VOLATILE;

//参变量

//日期及周次
typedef struct tag_EM_DIC010 {
	BYTE year;
	BYTE month;
	BYTE m_day;
	BYTE w_day;
} EM_DIC010;

//时间
typedef struct tag_EM_DIC011 {
	BYTE hour;
	BYTE minute;
	BYTE sec;
} EM_DIC011;

typedef struct tag_EM_DIC01F {
	EM_DIC010 DIC010;
	EM_DIC011 DIC011;
} EM_DIC01F;

//电表运行状态字
typedef struct tag_EM_DIC020 {
	BYTE status;
} EM_DIC020;

//电网状态字
typedef struct tag_EM_DIC021 {
	BYTE status;
} EM_DIC021;

//周休日状态字
typedef struct tag_EM_DIC022 {
	BYTE status;
} EM_DIC022;

//电表状态字1-7
typedef struct tag_EM_DIC023 {
	WORD status[7];
} EM_DIC023;

typedef struct tag_EM_DIC02F {
	EM_DIC020 DIC020;
	EM_DIC021 DIC021;
	EM_DIC022 DIC022;
} EM_DIC02F;

//电表常数(有功)
typedef struct tag_EM_DIC030 {
	DWORD value;
} EM_DIC030;

//电表常数(无功)
typedef struct tag_EM_DIC031 {
	DWORD value;
} EM_DIC031;

//表号
typedef struct tag_EM_DIC032 {
	BYTE host[6];
} EM_DIC032;

//用户号
typedef struct tag_EM_DIC033 {
	BYTE user[6];
} EM_DIC033;

//设备码
typedef struct tag_EM_DIC034 {
	BYTE fcode[6];
} EM_DIC034;

typedef struct tag_EM_DIC03F {
	EM_DIC030 DIC030;
	EM_DIC031 DIC031;
	EM_DIC032 DIC032;
	EM_DIC033 DIC033;
	EM_DIC034 DIC034;
} EM_DIC03F;

//最大需量周期
typedef struct tag_EM_DIC111 {
	BYTE minutes;
} EM_DIC111;

//滑差时间
typedef struct tag_EM_DIC112 {
	BYTE minutes;
} EM_DIC112;

//循显时间
typedef struct tag_EM_DIC113 {
	BYTE sec;
} EM_DIC113;

//停显时间
typedef struct tag_EM_DIC114 {
	BYTE sec;
} EM_DIC114;

//显示电能小数位数
typedef struct tag_EM_DIC115 {
	BYTE bits;
} EM_DIC115;

//显示功率(最大需量)小数位数
typedef struct tag_EM_DIC116 {
	BYTE bits;
} EM_DIC116;

//自动抄表日期
typedef struct tag_EM_DIC117 {
	BYTE hour;
	BYTE day;
} EM_DIC117;

//负荷代表日
typedef struct tag_EM_DIC118 {
	BYTE day;
} EM_DIC118;

//有功电能起始读数
typedef struct tag_EM_DIC119 {
	double value;
} EM_DIC119;

//无功电能起始读数
typedef struct tag_EM_DIC11A {
	double value;
} EM_DIC11A;

typedef struct tag_EM_DIC11F {
	EM_DIC111 DIC111;
	EM_DIC112 DIC112;
	EM_DIC113 DIC113;
	EM_DIC114 DIC114;
	EM_DIC115 DIC115;
	EM_DIC116 DIC116;
	EM_DIC117 DIC117;
	EM_DIC118 DIC118;
	EM_DIC119 DIC119;
	EM_DIC11A DIC11A;

} EM_DIC11F;

//输出脉冲宽度
typedef struct tag_EM_DIC211 {
	WORD ms;
} EM_DIC211;

//密码权限及密码
typedef struct tag_EM_DIC212 {
	BYTE pass[3];
} EM_DIC212;

typedef struct tag_EM_DIC21F {
	EM_DIC211 DIC211;
	EM_DIC212 DIC212;/*-0级密码-*/
	EM_DIC212 DIC213;
} EM_DIC21F;

//年时区数P
typedef struct tag_EM_DIC310 {
	BYTE num;
} EM_DIC310;

//日时段表数q
typedef struct tag_EM_DIC311 {
	BYTE num;
} EM_DIC311;

//日时段(每日切换数)m
typedef struct tag_EM_DIC312 {
	BYTE num;
} EM_DIC312;

//费率数k
typedef struct tag_EM_DIC313 {
	BYTE num;
} EM_DIC313;

//公共假日数n
typedef struct tag_EM_DIC314 {
	BYTE num;
} EM_DIC314;

typedef struct tag_EM_DIC31F {
	EM_DIC310 DIC310;
	EM_DIC311 DIC311;
	EM_DIC312 DIC312;
	EM_DIC313 DIC313;
	EM_DIC314 DIC314;
} EM_DIC31F;

// P 时区起始日期及日时段表号
// P=1....
typedef struct tag_EM_DIC32P {
	BYTE month;
	BYTE day;
	BYTE NN;
} EM_DIC32P;

typedef struct tag_EM_DIC32F {
	EM_DIC32P DIC32[LINYANG_EM_MAX_SEASON];
} EM_DIC32F;

// 第Q日时段表第M时段起始时间及费率号
// Q=3、4、5...
// M=1、2、3...
typedef struct tag_EM_DIC3QM {
	BYTE hour;
	BYTE minute;
	BYTE NN;
} EM_DIC3QM;

// 时段
typedef struct tag_EM_DIC3QF {
	EM_DIC3QM DIC3QM[LINYANG_EM_MAX_TIMEMASK];
} EM_DIC3QF;

// 时段表集合
typedef struct tag_EM_DIC33F {
	EM_DIC3QF DIC3QF[LINYANG_EM_MAX_TIMETABLE];
} EM_DIC3FF;

// 时段切换日，add for hubei_pb
typedef struct tag_EM_DIC340 {
	BYTE year;
	BYTE month;
	BYTE day;
} EM_DIC340;

// 第N公共假日日期及日时段表号
// N=1、2、3...
typedef struct tag_EM_DIC41N {
	BYTE month;
	BYTE day;
	BYTE NN;
} EM_DIC41N;

typedef struct tag_EM_DIC41F {
	EM_DIC41N DIC41N[LINYANG_EM_MAX_HOLIDAY];
} EM_DIC41F;

// 周休日采用的日时段表号
typedef struct tag_EM_DIC41E {
	BYTE NN;
} EM_DIC41E;

// 负荷记录起始时间
typedef struct tag_EM_DIC510 {
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DIC510;

// 负荷记录间隔时间
typedef struct tag_EM_DIC511 {
	WORD minute;
} EM_DIC511;

typedef struct tag_EM_PARAM {
	EM_DIC010 DIC010;
	EM_DIC011 DIC011;
	EM_DIC020 DIC020;
	EM_DIC021 DIC021;
	EM_DIC022 DIC022;
	EM_DIC023 DIC023;					//add for DLT645-2007
	EM_DIC030 DIC030;
	EM_DIC031 DIC031;
	EM_DIC032 DIC032;
	EM_DIC033 DIC033;
	EM_DIC034 DIC034;
	EM_DIC111 DIC111;
	EM_DIC112 DIC112;
	EM_DIC113 DIC113;
	EM_DIC114 DIC114;
	EM_DIC115 DIC115;
	EM_DIC116 DIC116;
	EM_DIC117 DIC117;
	EM_DIC118 DIC118;
	EM_DIC119 DIC119;
	EM_DIC11A DIC11A;
	EM_DIC211 DIC211;
	EM_DIC212 DIC212;
	EM_DIC310 DIC310;
	EM_DIC311 DIC311;
	EM_DIC312 DIC312;
	EM_DIC313 DIC313;
	EM_DIC314 DIC314;
	EM_DIC32F DIC32F;
	EM_DIC3FF DIC3FF;
	EM_DIC41F DIC41F;
	EM_DIC41E DIC41E;
	EM_DIC510 DIC510;
	EM_DIC511 DIC511;
} EM_PARAM;

// 自定义部分
// 上次月结时间
typedef struct tag_EM_DIF000 {
	BYTE nClrFlag;
	time_t tmCongeal;
} EM_DIF000;

// 校表参数
typedef struct tag_EM_DIF001 {
	BYTE value[256];
} EM_DIF001;

// 接线方式
typedef struct tag_EM_DIF002 {
	BYTE nConnect;
} EM_DIF002;

typedef struct tag_EM_EXPAND {
	EM_DIF000 DIF000;
	EM_DIF001 DIF001;
	EM_DIF002 DIF002;
} EM_EXPAND;

//7号数据包
typedef struct tag_EM_DIE207 {
	EM_DIC011 DIC011;
	EM_DIB630 DIB630;
	EM_DIB640 DIB640;
	EM_DIB611 DIB611;
	EM_DIB612 DIB612;
	EM_DIB613 DIB613;
	EM_DIB621 DIB621;
	EM_DIB622 DIB622;
	EM_DIB623 DIB623;
	EM_DIA01X DIA010;
	EM_DIA02X DIA020;
	WORD DIC020;
} EM_DIE207;

//9号数据包
typedef struct tag_EM_DIE209 {
	EM_DIB661 DIB661;
	EM_DIB662 DIB662;
	EM_DIB663 DIB663;
	EM_DIB664 DIB664;
	EM_DIB665 DIB665;

} EM_DIE209;

//10号数据包
typedef struct tag_EM_DIE211 {
	EM_DIC010 DIC010;
	EM_DIC011 DIC011;
	EM_DI901X DI9410;
	EM_DI901X DI9510;
	EM_DI901X DI9530;
	EM_DI901X DI9010;
	EM_DI901X DI9110;
	EM_DI901X DI9130;
	WORD DIC020;
	BYTE DIC117;
} EM_DIE211;

//11号数据包
typedef struct tag_EM_DIE221 {
	EM_DIC010 DIC010;
	EM_DIC011 DIC011;
	EM_DI901X DI9410;
	EM_DI901X DI9510;
	EM_DI901X DI9530;
	EM_DI901X DI9010;
	EM_DI901X DI9110;
	EM_DI901X DI9130;
	EM_DI901X DIE401;
	EM_DI901X DIE402;
	EM_DI901X DIE403;
	WORD DIC020;
	BYTE DIC117;

} EM_DIE221;

typedef struct tag_EM_DI05040001 {
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DI05040001;

//安徽电表
//增加项
typedef struct tag_EM_DID415 {
	WORD times;
	BYTE year;
	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
} EM_DID415;
typedef struct tag_EM_DID40B {

	BYTE month;
	BYTE day;
	BYTE hour;
	BYTE minute;
	BYTE dumonth;
	BYTE duday;
	BYTE duhour;
	BYTE duminute;
} EM_DID40B;

typedef struct tag_EM_DID40D {
	WORD times;
	WORD dutimes;

} EM_DID40D;

#define NL_FK_MAX_GUI_NUMBER  60
typedef struct tag_GUI_DISP_NUMBER					//显示内容顺序
{
	unsigned long ParaFlag;
	BYTE nCount;
	WORD nNumber[NL_FK_MAX_GUI_NUMBER];
	BYTE nCs;
} GUI_DISP_NUMBER;

typedef struct tag_EM_GUI_DISP {
	GUI_DISP_NUMBER guiDisp[3];
} EM_GUI_DISP;

#ifdef __cplusplus
}
#endif

#endif //__EM_PROTOCOL_H__
