/*
 * CT6452007Protocol.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "T6452007Protocol.h"
#include <cstdio>
#include <cassert>
#include "ctypes.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

typedef struct tag_CMD_T645_2007 {
	BYTE gtt;
	BYTE cmd; //命令字
	DWORD di;
	DWORD ddi; //2007 规约标识符
} CMD_T645_2007;

///////////////////////////////////////////////////////////////////////////////
// static helper data
///////////////////////////////////////////////////////////////////////////////
static const CMD_T645_2007 l_commands_2007[] = {
// 抄表间隔执行(此时分钟抄表不执行,故此时也抄实时数据)
		{ 0x01, 0x01, 0xB61F, 0x0201ff00 },     // 电压
		{ 0x01, 0x01, 0xB62F, 0x0202ff00 },     // 电流
		{ 0x01, 0x01, 0x0205ff00, 0x0205ff00 }, // 视在功率
		{ 0x01, 0x01, 0x02800001, 0x02800001 }, // 零线电流
		{ 0x01, 0x01, 0x0207ff00, 0x0207ff00 }, // 相角数据
		{ 0x01, 0x01, 0x901F, 0x0001ff00 },     // (当前)正向有功总电能
		{ 0x01, 0x01, 0x902F, 0x0002ff00 },     // (当前)反向有功总电能
		{ 0x01, 0x01, 0x9110, 0x00030000 },	  // (当前)正向无功总电能
		{ 0x01, 0x01, 0x9120, 0x00040000 },	  // (当前)反向无功总电能
		{ 0x01, 0x01, 0x9130, 0x00050000 },     // (当前)一象限无功总电能
		{ 0x01, 0x01, 0x9140, 0x00080000 },     // (当前)四象限无功总电能
		{ 0x01, 0x01, 0x9150, 0x00060000 },     // (当前)二象限无功总电能
		{ 0x01, 0x01, 0x9160, 0x00070000 },     // (当前)三象限无功总电能

		// 分相电能
		{ 0x01, 0x01, 0x9070, 0x00150000 }, // 当前A相正向有功电能
		{ 0x01, 0x01, 0x9080, 0x00160000 }, // 当前A相反向有功电能
		{ 0x01, 0x01, 0x9170, 0x00170000 }, // 当前A相正向无功电能
		{ 0x01, 0x01, 0x9180, 0x00180000 }, // 当前A相反向无功电能
		{ 0x01, 0x01, 0x9071, 0x00290000 }, // 当前B相正向有功电能
		{ 0x01, 0x01, 0x9081, 0x002A0000 }, // 当前B相反向有功电能
		{ 0x01, 0x01, 0x9171, 0x002B0000 }, // 当前B相正向无功电能
		{ 0x01, 0x01, 0x9181, 0x002C0000 }, // 当前B相反向无功电能
		{ 0x01, 0x01, 0x9072, 0x003D0000 }, // 当前C相正向有功电能
		{ 0x01, 0x01, 0x9082, 0x003E0000 }, // 当前C相反向有功电能
		{ 0x01, 0x01, 0x9172, 0x003F0000 }, // 当前C相正向无功电能
		{ 0x01, 0x01, 0x9182, 0x00400000 }, // 当前C相反向无功电能

		// 需量
		{ 0x01, 0x01, 0x0101ff00, 0x0101ff00 },     // (当前)正向有功总最大需量
		{ 0x01, 0x01, 0x0102ff00, 0x0102ff00 },     // (当前)反向有功总最大需量
		{ 0x01, 0x01, 0x0101ff00, 0x0101ff00 },     // (当前)正向有功总最大需量
		{ 0x01, 0x01, 0x0102ff00, 0x0102ff00 },     // (当前)反向有功总最大需量
		{ 0x01, 0x01, 0x0103ff00, 0x0103ff00 },     // (当前)正向无功总最大需量
		{ 0x01, 0x01, 0x0104ff00, 0x0104ff00 },     // (当前)反向无功总最大需量

		{ 0x01, 0x01, 0xC010, 0x04000101 },     // 日期及周日
		{ 0x01, 0x01, 0xC011, 0x04000102 },     // 时间
		{ 0x01, 0x01, 0xB214, 0x0280000a },	  // 电池工作时间
		{ 0x01, 0x01, 0x03300000, 0x03300000 }, // 编程总次数
		{ 0x01, 0x01, 0x03300001, 0x03300001 }, // 最近一次编程时间
		{ 0x01, 0x01, 0x03300100, 0x03300100 }, // 清零总次数
		{ 0x01, 0x01, 0x03300101, 0x03300101 }, // 最近一次清零发生时刻
		{ 0x01, 0x01, 0x03300200, 0x03300200 }, // 需量清零总次数
		{ 0x01, 0x01, 0x03300201, 0x03300201 }, // 最近一次需量清零发生时刻
		{ 0x01, 0x01, 0x03300300, 0x03300300 }, // 事情清零总次数
		{ 0x01, 0x01, 0x03300301, 0x03300301 }, // 最近一次事件清零发生时刻
		{ 0x01, 0x01, 0x03300400, 0x03300400 }, // 校时总次数
		{ 0x01, 0x01, 0x03300401, 0x03300401 }, // 最近一次校时发生时刻
		{ 0x01, 0x01, 0x03300500, 0x03300500 }, // 时段表编程总次数
		{ 0x01, 0x01, 0x03300501, 0x03300501 }, // 最近一次时段表编程发生时刻
		{ 0x01, 0x01, 0x040005ff, 0x040005ff }, // 电表状态字1-7

		// 断相
		{ 0x01, 0x01, 0x03040000, 0x03040000 }, // 断相总次数及累计时间
		{ 0x01, 0x01, 0x03040101, 0x03040101 }, // A相最近一次断相起始/结束时刻
		{ 0x01, 0x01, 0x03040201, 0x03040201 }, // B相最近一次断相起始/结束时刻
		{ 0x01, 0x01, 0x03040301, 0x03040301 }, // C相最近一次断相起始/结束时刻

		//备案文件中
		{ 0x01, 0x01, 0x13010001, 0x13010001 }, // A相断相总次数
		{ 0x01, 0x01, 0x13010002, 0x13010002 }, // A相断相累计时间
		{ 0x01, 0x01, 0x13020001, 0x13020001 }, // B相断相总次数
		{ 0x01, 0x01, 0x13020002, 0x13020002 }, // B相断相累计时间
		{ 0x01, 0x01, 0x13030001, 0x13030001 }, // C相断相总次数
		{ 0x01, 0x01, 0x13030002, 0x13030002 }, // C相断相累计时间

		{ 0x01, 0x01, 0x13010101, 0x13010101 }, // A相最近一次断相起始/结束时刻
		{ 0x01, 0x01, 0x13012501, 0x13012501 }, // A相最近一次断相起始/结束时刻
		{ 0x01, 0x01, 0x13020101, 0x13020101 }, // B相最近一次断相起始/结束时刻
		{ 0x01, 0x01, 0x13022501, 0x13022501 }, // B相最近一次断相起始/结束时刻
		{ 0x01, 0x01, 0x13030101, 0x13030101 }, // C相最近一次断相起始/结束时刻
		{ 0x01, 0x01, 0x13032501, 0x13032501 }, // C相最近一次断相起始/结束时刻
		//end

		{ 0x01, 0x01, 0xC117, 0x04000B01 },		// 自动抄表日  2007中为结算日(第一结算日)
		{ 0x01, 0x01, 0x04000409, 0x04000409 },  // 有功脉冲常数
		{ 0x01, 0x01, 0x0400040A, 0x0400040A },  // 无功脉冲常数

		{ 0x01, 0x01, 0xC33F, 0x04010001 }, // 07 第一套时区表数据 14个时段开始时间及费率号

		// 上月数据
		{ 0x02, 0x01, 0x941F, 0x0001ff01 },     // (上月)正向有功总电能
		{ 0x02, 0x01, 0x942F, 0x0002ff01 },     // (上月)反向有功总电能
		{ 0x02, 0x01, 0x9510, 0x00030001 },     // (上月)正向无功总电能
		{ 0x02, 0x01, 0x9520, 0x00040001 },     // (上月)反向无功总电能
		{ 0x02, 0x01, 0x9530, 0x00050001 },     // (上月)一象限无功总电能
		{ 0x02, 0x01, 0x9540, 0x00080001 },     // (上月)四象限无功总电能
		{ 0x02, 0x01, 0x9550, 0x00060001 },     // (上月)二象限无功总电能
		{ 0x02, 0x01, 0x9560, 0x00070001 },     // (上月)三象限无功总电能

		// 分相电能
		{ 0x02, 0x01, 0x9470, 0x00150001 }, // 上月A相正向有功电能
		{ 0x02, 0x01, 0x9480, 0x00160001 }, // 上月A相反向有功电能
		{ 0x02, 0x01, 0x9570, 0x00170001 }, // 上月A相正向无功电能
		{ 0x02, 0x01, 0x9580, 0x00180001 }, // 上月A相反向无功电能
		{ 0x02, 0x01, 0x9471, 0x00290001 }, // 上月B相正向有功电能
		{ 0x02, 0x01, 0x9481, 0x002A0001 }, // 上月B相反向有功电能
		{ 0x02, 0x01, 0x9571, 0x002B0001 }, // 上月B相正向无功电能
		{ 0x02, 0x01, 0x9581, 0x002C0001 }, // 上月B相反向无功电能
		{ 0x02, 0x01, 0x9472, 0x003D0001 }, // 上月C相正向有功电能
		{ 0x02, 0x01, 0x9482, 0x003E0001 }, // 上月C相反向有功电能
		{ 0x02, 0x01, 0x9572, 0x003F0001 }, // 上月C相正向无功电能
		{ 0x02, 0x01, 0x9582, 0x00400001 }, // 上月C相反向无功电能

		{ 0x02, 0x01, 0x0101ff01, 0x0101ff01 }, // (上月)正向有功总最大需量及发生时间
		{ 0x02, 0x01, 0x0102ff01, 0x0102ff01 }, // (上月)反向有功总最大需量及发生时间
		{ 0x02, 0x01, 0x0103ff01, 0x0103ff01 }, // (上月)正向无功总最大需量及发生时间
		{ 0x02, 0x01, 0x0104ff01, 0x0104ff01 }, // (上月)反向无功总最大需量及发生时间

		// 抄表日冻结
		{ 0x03, 0x01, 0xB63F, 0x0203ff00 },     // 有功功率
		{ 0x03, 0x01, 0x901F, 0x0001ff00 },     // (当前)正向有功总电能
		{ 0x03, 0x01, 0x902F, 0x0002ff00 },     // (当前)反向有功总电能
		{ 0x03, 0x01, 0x9110, 0x00030000 },	  // (当前)正向无功总电能
		{ 0x03, 0x01, 0x9120, 0x00040000 },	  // (当前)反向无功总电能
		{ 0x03, 0x01, 0x9130, 0x00050000 },     // (当前)一象限无功总电能
		{ 0x03, 0x01, 0x9140, 0x00080000 },     // (当前)四象限无功总电能
		{ 0x03, 0x01, 0x9150, 0x00060000 },     // (当前)二象限无功总电能
		{ 0x03, 0x01, 0x9160, 0x00070000 },     // (当前)三象限无功总电能

		{ 0x03, 0x01, 0x01010000, 0x01010000 },     // (当前)正向有功总最大需量
		{ 0x03, 0x01, 0x01020000, 0x01020000 },     // (当前)反向有功总最大需量
		{ 0x03, 0x01, 0x0101ff00, 0x0101ff00 }, // (当前)正向有功总最大需量及发生时间
		{ 0x03, 0x01, 0x0102ff00, 0x0102ff00 }, // (当前)反向有功总最大需量及发生时间
		{ 0x03, 0x01, 0x0103ff00, 0x0103ff00 }, // (当前)正向无功总最大需量及发生时间
		{ 0x03, 0x01, 0x0104ff00, 0x0104ff00 }, // (当前)反向无功总最大需量及发生时间

		// 日冻结
		{ 0x06, 0x01, 0x9110, 0x05060301 },     // 上一冻结日正向无功总电能
		{ 0x06, 0x01, 0x9120, 0x05060401 },     // 上一冻结日反向无功总电能
		{ 0x06, 0x01, 0x9130, 0x05060501 },		// 上一冻结日一象限无功总电能
		{ 0x06, 0x01, 0x9140, 0x05060801 },		// 上一冻结日四象限无功总电能
		{ 0x06, 0x01, 0x9150, 0x05060601 },		// 上一冻结日二象限无功总电能
		{ 0x06, 0x01, 0x9160, 0x05060701 },		// 上一冻结日三象限无功总电能

		{ 0x06, 0x01, 0x0101ff00, 0x05060901 }, // 正向有功总最大需量及发生时间
		{ 0x06, 0x01, 0x0102ff00, 0x05060A01 }, // 反向有功总最大需量及发生时间
		{ 0x06, 0x01, 0x0103ff00, 0x05060B01 }, // 正向有功总最大需量及发生时间
		{ 0x06, 0x01, 0x0104ff00, 0x05060C01 }, // 反向有功总最大需量及发生时间
		};

//单相表
static const CMD_T645_2007 l_commands_2007_1[] = { { 0x01, 0x01, 0x901F,
		0x0001ff00 },   // (当前)正向有功总电能

		{ 0x01, 0x01, 0xC010, 0x04000101 },   // 日期及周日
		{ 0x01, 0x01, 0xC011, 0x04000102 },   // 时间
		{ 0x01, 0x01, 0x040005ff, 0x040005ff }, // 电表状态字1-7

		{ 0x01, 0x01, 0x0201ff00, 0x0201ff00 }, // 电压
		{ 0x01, 0x01, 0x02010100, 0x02010100 }, // A电压
		{ 0x01, 0x01, 0x0202ff00, 0x0202ff00 }, // 电流
		{ 0x01, 0x01, 0x02020100, 0x02020100 }, // A电流
		{ 0x01, 0x01, 0xB63F, 0x0203ff00 }, // 有功功率
		{ 0x01, 0x01, 0xB630, 0x02030000 }, // 总有功功率
		{ 0x01, 0x01, 0xB631, 0x02030100 }, // A有功功率
		{ 0x01, 0x01, 0xB64F, 0x0204ff00 }, // 无功功率
		{ 0x01, 0x01, 0xB640, 0x02040000 }, // 总无功功率
		{ 0x01, 0x01, 0xB641, 0x02040100 }, // A无功功率
		{ 0x01, 0x01, 0xB65F, 0x0206ff00 },	  // 功率因素
		{ 0x01, 0x01, 0xB650, 0x02060000 },	  // 总功率因素
		{ 0x01, 0x01, 0xB651, 0x02060100 },	  // A功率因素

		{ 0x01, 0x01, 0x902F, 0x0002ff00 },   // (当前)反向有功总电能

		// 上月数据
		{ 0x02, 0x01, 0x941F, 0x0001ff01 },   // (上月)正向有功总电能
		{ 0x02, 0x01, 0x942F, 0x0002ff01 },   // (上月)反向有功总电能

		// 抄表日冻结
		{ 0x03, 0x01, 0x901F, 0x0001ff00 },   // (当前)正向有功总电能
		{ 0x03, 0x01, 0x902F, 0x0002ff00 },   // (当前)反向有功总电能

		// 日冻结
		{ 0x06, 0x01, 0x05060001, 0x05060001 }, // 上一冻结日冻结时间
		{ 0x06, 0x01, 0x901F, 0x05060101 },     // 上一冻结日正向有功总电能
		{ 0x06, 0x01, 0x902F, 0x05060201 },     // 上一冻结日反向有功总电能
		};
///////////////////////////////////////////////////////////////////////////////
// CT645_2007Protocol
///////////////////////////////////////////////////////////////////////////////

CT645_2007Protocol::CT645_2007Protocol(BYTE nMPT, PF_DL645_2007_SAVE pSave) :
		CGMProtocol(nMPT), m_pfSave(pSave) {
	memset(m_iTimes, 0, sizeof(int));
	memset(m_iMinutes, 0, sizeof(int));
	for (BYTE i = 0; i < 4; i++) {
		memset(m_sBegin[i], 0, 4);
		memset(m_sEnd[i], 0, 4);
	}
}

CT645_2007Protocol::~CT645_2007Protocol(void) {
}
////函数功能:645_2007组帧
////参数:pObj:测量点类指针,取电表地址;pCMD:命令类指针,gtt,cmd,id;lpBuf:存放645_2007帧数据;nBufSize:数据缓冲区大小
////返回值:>0 成功<0 失败
int CT645_2007Protocol::HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
		int nBufSize) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);

	DL645_FRAME frame;
	BYTE lpData[32] = { 0 };
	frame.data = lpData;

	CMD_T645_2007 *ptr = (CMD_T645_2007*) pCMD->m_body;
	int nRet = 0;

	switch (ptr->cmd) {
	case EM_CMD_READDATA: {
		nRet = DL645_07_ReadData(&frame, pObj->m_host, ptr->ddi); ///New Add for Read 2007 protocol;
	}
		break;

	default:
		nRet = -1;
		break;
	}

	//TRACE0("DL645-2007HandleTX:%X(%d)\r\n", ptr->di, pObj->m_TN);

	int nSize = 0;
	if (nRet == 0) {
		int nPos = 0;
		lpBuf[nPos++] = 0xFE;
		lpBuf[nPos++] = 0xFE;
		nSize = DL645_MakeFrame(&frame, &lpBuf[nPos], nBufSize - nPos);
		nSize += nPos;
	}
	return nSize;
}
void CT645_2007Protocol::GetRecord(BYTE val[4][4]) {
	////Minute Hour Day Month
	BYTE bFlag = 0;
	for (BYTE i = 0; i <= 3; i++)
		val[0][i] = val[1][i];

	for (BYTE i = 2; i <= 3; i++) {
		if (val[0][3] >= val[i][3])
			if (val[0][2] >= val[i][2])
				if (val[0][1] >= val[i][1])
					if (val[0][0] >= val[i][0])
						bFlag = 0;
					else
						bFlag = 1;
				else
					bFlag = 1;
			else
				bFlag = 1;
		else
			bFlag = 1;
		if (bFlag) {
			for (BYTE j = 0; j <= 3; j++) {
				val[0][j] = val[i][j];
			}
		}
	}
}
////函数功能:处理收到的645_2007组帧
////参数:pObj:测量点类指针,取电表地址;pCMD:命令类指针;lpBuf:存放645_2007帧数据;nBufSize:数据缓冲区大小
////返回值:>0 成功<0 失败
int CT645_2007Protocol::HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
		int nBufSize) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);
	assert(lpBuf != nullptr);
	assert(m_pfSave != nullptr);

	BYTE lpRecv[128] = { 0 };
	DL645_FRAME frm;
	frm.data = lpRecv;

	//按2007规约解析

	if (DL645_GetFrame(&frm, lpBuf, nBufSize) > 0) {
		if (frm.hdr.code.mask.DIR == 1) {
			CMD_T645_2007 *ptr = (CMD_T645_2007*) pCMD->m_body;
			if ((frm.hdr.code.mask.CMD == 0x11)
					&& (frm.hdr.code.mask.ACD == 0)) {
				DWORD ddi = MAKELONG(MAKEWORD(frm.data[0],frm.data[1]),
						MAKEWORD(frm.data[2],frm.data[3]));
				if (ddi == ptr->ddi) {
					int value[128 / sizeof(int)] = { 0 };
					switch (ptr->ddi) {
					case 0x0203ff00:   //有功功率
					case 0x02030000:
					case 0x02030100:
					case 0x0204ff00:   //无功功率
					case 0x02040000:
					case 0x02040100: {
						double tmp;
						for (BYTE i = 1; i <= 4; i++) {
							tmp = 1;
							if ((frm.data[1 + i * 3 + 2] & 0x80) != 0) {
								frm.data[1 + i * 3 + 2] &= 0x7f;
								tmp = -1;
							}

							double Ia = bcd2double(&frm.data[1 + i * 3], 3, 4);
							Ia = Ia * tmp;
							memcpy(value + (i - 1) * 2, &Ia, sizeof(double));

							if (HIBYTE(LOWORD(ptr->ddi)) != 0xFF)
								break;
						}
						m_pfSave(pObj, (void*) ptr->gtt, ptr->di,
								(void*) value);
					}
						break;

					case 0x0206ff00:  //功率因数
					case 0x02060000:
					case 0x02060100: {
						double tmp;
						for (BYTE i = 1; i <= 4; i++) {
							tmp = 1;
							if ((frm.data[3 + i * 2] & 0x80) != 0) {
								frm.data[3 + i * 2] &= 0x7f;
								tmp = -1;
							}

							double Ia = bcd2double(&frm.data[2 + i * 2], 2, 3);
							Ia = Ia * tmp;
							memcpy(value + (i - 1) * 2, &Ia, sizeof(double));

							if (HIBYTE(LOWORD(ptr->ddi)) != 0xFF)
								break;
						}
						m_pfSave(pObj, (void*) ptr->gtt, ptr->di,
								(void*) value);
					}
						break;
						/*
						 case 0x03040000: /////总断相次数及断相时间
						 {
						 memset(m_iTimes, 0, sizeof(int));
						 memset(m_iMinutes, 0, sizeof(int));
						 char p[16];
						 for(BYTE i = 0; i < 3; i++)
						 {
						 memset(p,0,16);
						 sprintf(p,"%02x%02x%02x",frm.data[4+i*6+2],frm.data[4+i*6+1],frm.data[4+i*6]);   ////???????
						 m_iTimes[i+1]=atoi(p);
						 m_iTimes[0]+=m_iTimes[i+1];
						 sprintf(p,"%02x%02x%02x",frm.data[4+i*6+5],frm.data[4+i*6+4],frm.data[4+i*6+3]); ////???????
						 m_iMinutes[i+1]=atoi(p);
						 m_iMinutes[0]+=m_iMinutes[i+1];
						 }
						 memcpy(value,m_iTimes,sizeof(WORD)*4);
						 m_pfSave(pObj,(void*)ptr->gtt,0xb31f,(void*)value);
						 memset(value,0,32);
						 memcpy(value,m_iMinutes,sizeof(int)*4);
						 m_pfSave(pObj,(void*)ptr->gtt,0xb32f,(void*)value);
						 }
						 break;

						 case 0x03040101:  ///A相
						 case 0x1301ff01:
						 {
						 memset(m_sBegin[1],0,4);
						 memset(m_sEnd[1],0,4);
						 for(BYTE i=0;i<=3;i++)
						 {
						 m_sBegin[1][i] = bcd2hex(frm.data[5+i]);
						 m_sEnd[1][i]   = bcd2hex(frm.data[11+i]);
						 }
						 }
						 break;

						 case 0x03040201:  ///B相
						 case 0x1302ff01:
						 {
						 memset(m_sBegin[2],0,4);
						 memset(m_sEnd[2],0,4);
						 for(BYTE i=0;i<=3;i++)
						 {
						 m_sBegin[2][i] = bcd2hex(frm.data[5+i]);
						 m_sEnd[2][i]   = bcd2hex(frm.data[11+i]);
						 }
						 }
						 break;

						 case 0x03040301:  ///C相
						 case 0x1303ff01:
						 {
						 memset(m_sBegin[3],0,4);
						 memset(m_sEnd[3],0,4);
						 for(BYTE i=0;i<=3;i++)
						 {
						 m_sBegin[3][i] = bcd2hex(frm.data[5+i]);
						 m_sEnd[3][i]   = bcd2hex(frm.data[11+i]);
						 }
						 GetRecord(m_sBegin);
						 GetRecord(m_sEnd);
						 BYTE dateBegin[16]={0};
						 BYTE dateEnd[16]={0};
						 for(BYTE i=0;i<4;i++)
						 {
						 for(BYTE j=0;j<4;j++)
						 {
						 dateBegin[i*4+j]=m_sBegin[i][3-j];
						 dateEnd[i*4+j]=m_sEnd[i][3-j];
						 }
						 }
						 memcpy(value,dateBegin,16);
						 m_pfSave(pObj,(void*)ptr->gtt,0xb33f,(void*)value);
						 memcpy(value,dateEnd,16);
						 m_pfSave(pObj,(void*)ptr->gtt,0xb34f,(void*)value);
						 }
						 break;
						 */
					case 0: // 异常DI
					{
						break;
					}

					default: {
						DL645_07_Decode(ptr->di, value, &frm.data[4]);
						if (ddi == 0x04000101) // 日期
								{
							EM_DIC010 *pC010 = (EM_DIC010 *) value;
//							OSDateTime time = OSDateTime::GetCurrentTime();
//							if ((pC010->year != time.GetYear())
//									|| (pC010->month != time.GetMonth())
//									|| (pC010->m_day != time.GetDay())) {
//								pObj->m_nMPTimeEnable &= (~0x01);
//							} else {
//								pObj->m_nMPTimeEnable |= 0x01;
//							}
						} else if (ddi == 0x04000102) // 时间
								{
							EM_DIC011 *pC011 = (EM_DIC011 *) value;
//							OSDateTime time = OSDateTime::GetCurrentTime();
//							if ((pC011->hour != time.GetHour())
//							//	|| (pC011->minute != time.GetMinute())
//							) {
//								pObj->m_nMPTimeEnable &= (~0x02);
//							} else {
//								pObj->m_nMPTimeEnable |= 0x02;
//							}
						}

						m_pfSave(pObj, (void*) ptr->gtt, ptr->di,
								(void*) value);
					}
						break;
					}
				}
			}
		}
		return 0;
	}
	/*
	 if ((nErrFlag==1) || (nErrFlag==2))
	 {
	 CMD_T645_2007 *ptr = (CMD_T645_2007*)pCMD->m_body;

	 {
	 int value[128/sizeof(int)]={0xee};
	 memset(value,0xEE,128);
	 TRACE0("ptr->di = %x save ee\r\n",ptr->di);
	 if ((ptr->ddi) && (ptr->di))
	 {
	 m_pfSave(pObj, (void*)ptr->gtt, ptr->di, (void*)value);
	 }

	 }
	 }
	 */
	return -1;
}

////函数功能:得到命令类指针对象
////参数:parms ,pCMDs:Command 类指针 *& 可以改变指针指向;
////返回值:返回数组下标索引

int CT645_2007Protocol::GetCommands(void *parms, Command *&pCMDs,
		PointType type) {
	Command **ppCMD = &pCMDs;
	BYTE gtt = (BYTE) *(int *)parms;;
	int nNums = 0;
	int nSize = sizeof(l_commands_2007_1) / sizeof(CMD_T645_2007);
	for (int i = 0; i < nSize; ++i) {
		if (gtt == l_commands_2007_1[i].gtt) {
			*ppCMD = new Command;
			if (*ppCMD != nullptr) {
				(*ppCMD)->m_nAck = 1;
				((CMD_T645_2007*) ((*ppCMD)->m_body))->gtt =
						l_commands_2007_1[i].gtt;
				((CMD_T645_2007*) ((*ppCMD)->m_body))->cmd =
						l_commands_2007_1[i].cmd;
				((CMD_T645_2007*) ((*ppCMD)->m_body))->di =
						l_commands_2007_1[i].di;
				((CMD_T645_2007*) ((*ppCMD)->m_body))->ddi =
						l_commands_2007_1[i].ddi;
				(*ppCMD)->m_pNext = nullptr;
				(*ppCMD)->m_pAck = nullptr;
				ppCMD = &(*ppCMD)->m_pNext;
				nNums++;
			}
		}
	}

	// 增加判断 单、三相表
	if (Mul_Point == type) {
		nSize = sizeof(l_commands_2007) / sizeof(CMD_T645_2007);
		for (int i = 0; i < nSize; ++i) {
			if (gtt == l_commands_2007[i].gtt) {
				*ppCMD = new Command;
				if (*ppCMD != nullptr) {
					(*ppCMD)->m_nAck = 1;
					((CMD_T645_2007*) ((*ppCMD)->m_body))->gtt =
							l_commands_2007[i].gtt;
					((CMD_T645_2007*) ((*ppCMD)->m_body))->cmd =
							l_commands_2007[i].cmd;
					((CMD_T645_2007*) ((*ppCMD)->m_body))->di =
							l_commands_2007[i].di;
					((CMD_T645_2007*) ((*ppCMD)->m_body))->ddi =
							l_commands_2007[i].ddi; ///New Add 2009.2.1 11:22
					(*ppCMD)->m_pNext = nullptr;
					(*ppCMD)->m_pAck = nullptr;
					ppCMD = &(*ppCMD)->m_pNext;
					nNums++;
				}
			}
		}
	}

	return nNums;
}
////函数功能:设置默认的串口参数
////参数:pObj:pDcb串口数据结构指针
////返回值:0返回正确

int CT645_2007Protocol::GetDefaultComDcb(ComDcb *pDcb) {
	assert(pDcb != nullptr);

	pDcb->nBaud = CB_2400;
	pDcb->databit = DB_8;
	pDcb->parity = CP_EVEN;
	pDcb->stopbit = CS_ONE;

	return 0;
}

} /* namespace pcols */
