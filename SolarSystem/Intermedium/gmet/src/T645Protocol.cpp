/*
 * CT645Protocol.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include <cassert>
#include "T645Protocol.h"

namespace pcols {

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

typedef struct tag_CMD_T645 {
	BYTE gtt;
	BYTE cmd;
	WORD di;
} CMD_T645;

///////////////////////////////////////////////////////////////////////////////
// static helper data
///////////////////////////////////////////////////////////////////////////////
static const CMD_T645 l_commands[] = {
// 当前数据
		{ 0x01, 0x01, 0xB61F },     // 电压
		{ 0x01, 0x01, 0xB62F },     // 电流
		{ 0x01, 0x01, 0xB63F },     // 有功功率
		{ 0x01, 0x01, 0xB64F },     // 无功功率
		{ 0x01, 0x01, 0xB65F },     // 功率因素
		{ 0x01, 0x01, 0xB66F },	// 相角数据，扩展规约，每种电表不一样gs，2009.6.8

		{ 0x01, 0x01, 0x901F },     // (当前)正向有功总电能
		{ 0x01, 0x01, 0x902F },     // (当前)反向有功总电能
		{ 0x01, 0x01, 0x9110 },		// (当前)正向无功总电能
		{ 0x01, 0x01, 0x9120 },		// (当前)反向无功总电能
		{ 0x01, 0x01, 0x9130 },     // (当前)一象限无功总电能
		{ 0x01, 0x01, 0x9140 },     // (当前)四象限无功总电能
		{ 0x01, 0x01, 0x9150 },     // (当前)二象限无功总电能
		{ 0x01, 0x01, 0x9160 },     // (当前)三象限无功总电能
		{ 0x01, 0x01, 0xA01F },     // (当前)正向有功总最大需量
		{ 0x01, 0x01, 0xA02F },     // (当前)反向有功总最大需量
		{ 0x01, 0x01, 0xA11F },     // (当前)正向无功总最大需量
		{ 0x01, 0x01, 0xA12F },     // (当前)反向无功总最大需量
		{ 0x01, 0x01, 0xB01F },     // (当前)正向有功最大需量发生时间
		{ 0x01, 0x01, 0xB02F },     // (当前)反向有功最大需量发生时间
		{ 0x01, 0x01, 0xB11F },     // (当前)正向无功最大需量发生时间
		{ 0x01, 0x01, 0xB12F },     // (当前)反向无功最大需量发生时间

		{ 0x01, 0x01, 0xB210 },		// 最近一次编程时间
		{ 0x01, 0x01, 0xB211 },		// 最近一次最大需量清零时间
		{ 0x01, 0x01, 0xB212 },		// 编程次数
		{ 0x01, 0x01, 0xB213 },		// 最大需量清零次数
		{ 0x01, 0x01, 0xB214 },		// 电池工作时间

		{ 0x01, 0x01, 0xB31F },     // 断相次数
		{ 0x01, 0x01, 0xB32F },     // 断相时间
		{ 0x01, 0x01, 0xB33F },		// 最近一次断相起始时刻
		{ 0x01, 0x01, 0xB34F },		// 最近一次断相结束时刻

		{ 0x01, 0x01, 0xC010 },     // 日期及周日
		{ 0x01, 0x01, 0xC011 },     // 时间
		{ 0x01, 0x01, 0xC020 },     // 表计状态字
		{ 0x01, 0x01, 0xC021 },		// 电网状态字
		{ 0x01, 0x01, 0xC022 },		// 周休状态字
		{ 0x01, 0x01, 0xC030 },     // 电表常数(有功)
		{ 0x01, 0x01, 0xC031 },     // 电表常数(无功)
		{ 0x01, 0x01, 0xC032 },     // 表号
		{ 0x01, 0x01, 0xC033 },     // 用户号
		{ 0x01, 0x01, 0xC034 },     // 设备号

		{ 0x01, 0x01, 0xC117 },		// 自动抄表日
		{ 0x01, 0x01, 0xC119 },     // 有功电能表起始读数
		{ 0x01, 0x01, 0xC11A },     // 无功电能表起始读数

		{ 0x01, 0x01, 0xC331 },		// 第一日时段表第1时段开始时间及费率号
		{ 0x01, 0x01, 0xC332 },		// 第一日时段表第2时段开始时间及费率号
		{ 0x01, 0x01, 0xC333 },		// 第一日时段表第3时段开始时间及费率号
		{ 0x01, 0x01, 0xC334 },		// 第一日时段表第4时段开始时间及费率号
		{ 0x01, 0x01, 0xC335 },		// 第一日时段表第5时段开始时间及费率号
		{ 0x01, 0x01, 0xC336 },		// 第一日时段表第6时段开始时间及费率号
		{ 0x01, 0x01, 0xC337 },		// 第一日时段表第7时段开始时间及费率号
		{ 0x01, 0x01, 0xC338 },		// 第一日时段表第8时段开始时间及费率号

		// 上月数据
		{ 0x02, 0x01, 0x941F },     // (上月)正向有功总电能
		{ 0x02, 0x01, 0x942F },     // (上月)反向有功总电能
		{ 0x02, 0x01, 0x9510 },     // (上月)正向无功总电能
		{ 0x02, 0x01, 0x9520 },     // (上月)反向无功总电能
		{ 0x02, 0x01, 0x9530 },     // (上月)一象限无功总电能
		{ 0x02, 0x01, 0x9540 },     // (上月)四象限无功总电能
		{ 0x02, 0x01, 0x9550 },     // (上月)二象限无功总电能
		{ 0x02, 0x01, 0x9560 },     // (上月)三象限无功总电能
		{ 0x02, 0x01, 0xA41F },     // (上月)正向有功总最大需量
		{ 0x02, 0x01, 0xA42F },     // (上月)反向有功总最大需量
		{ 0x02, 0x01, 0xA51F },     // (上月)正向无功总最大需量
		{ 0x02, 0x01, 0xA52F },     // (上月)反向无功总最大需量
		{ 0x02, 0x01, 0xB41F },     // (上月)正向有功总最大需量发生时间
		{ 0x02, 0x01, 0xB42F },     // (上月)反向有功总最大需量发生时间
		{ 0x02, 0x01, 0xB51F },     // (上月)正向无功总最大需量发生时间
		{ 0x02, 0x01, 0xB52F },     // (上月)反向无功总最大需量发生时间

		// 抄表日冻结
		{ 0x03, 0x01, 0x901F },     // (当前)正向有功总电能
		{ 0x03, 0x01, 0x902F },     // (当前)反向有功总电能
		{ 0x03, 0x01, 0x9110 },		// (当前)正向无功总电能
		{ 0x03, 0x01, 0x9120 },		// (当前)反向无功总电能
		{ 0x03, 0x01, 0x9130 },     // (当前)一象限无功总电能
		{ 0x03, 0x01, 0x9140 },     // (当前)四象限无功总电能
		{ 0x03, 0x01, 0x9150 },     // (当前)二象限无功总电能
		{ 0x03, 0x01, 0x9160 },     // (当前)三象限无功总电能
		{ 0x03, 0x01, 0xA01F },     // (当前)正向有功总最大需量
		{ 0x03, 0x01, 0xA02F },     // (当前)反向有功总最大需量
		{ 0x03, 0x01, 0xA11F },     // (当前)正向无功总最大需量
		{ 0x03, 0x01, 0xA12F },     // (当前)反向无功总最大需量
		{ 0x03, 0x01, 0xB01F },     // (当前)正向有功最大需量发生时间
		{ 0x03, 0x01, 0xB02F },     // (当前)反向有功最大需量发生时间
		{ 0x03, 0x01, 0xB11F },     // (当前)正向无功最大需量发生时间
		{ 0x03, 0x01, 0xB12F },     // (当前)反向无功最大需量发生时间
		};

//单相表
static const CMD_T645 l_commands_1[] = { { 0x01, 1, 0x9010 },
		{ 0x01, 1, 0x901F },   // (当前)正向有功总电能
		{ 0x01, 1, 0x9010 },   //
		{ 0x01, 1, 0x9011 },   //
		{ 0x01, 1, 0x9012 },   //
		{ 0x01, 1, 0x9013 },   //
		{ 0x01, 1, 0x9014 },   //

		{ 0x01, 1, 0xC010 },   // 日期及周日
		{ 0x01, 1, 0xC011 },   // 时间
		{ 0x01, 1, 0xC020 },   // 表计状态字

		{ 0x01, 1, 0x9020 },   //
		{ 0x01, 1, 0x902F },   // (当前)反向有功总电能
		{ 0x01, 1, 0x9020 },     //
		{ 0x01, 1, 0x9021 },     //
		{ 0x01, 1, 0x9022 },     //
		{ 0x01, 1, 0x9023 },     //
		{ 0x01, 1, 0x9024 },     //

		{ 0x01, 0x01, 0xB61F },     // 电压
		{ 0x01, 0x01, 0xB611 },   //
		{ 0x01, 0x01, 0xB612 },   //
		{ 0x01, 0x01, 0xB613 },   //
		{ 0x01, 0x01, 0xB62F },     // 电流
		{ 0x01, 0x01, 0xB621 },   //
		{ 0x01, 0x01, 0xB622 },   //
		{ 0x01, 0x01, 0xB623 },   //
		{ 0x01, 0x01, 0xB63F },     // 有功功率
		{ 0x01, 0x01, 0xB64F },     // 无功功率
		{ 0x01, 0x01, 0xB65F },     // 功率因素

		// 上月数据
		{ 0x02, 1, 0x9410 },   //
		{ 0x02, 1, 0x941F },   // (上月)正向有功总电能
		{ 0x02, 1, 0x9410 },   //
		{ 0x02, 1, 0x9411 },   //
		{ 0x02, 1, 0x9412 },   //
		{ 0x02, 1, 0x9413 },   //
		{ 0x02, 1, 0x9414 },   //

		{ 0x02, 1, 0x9420 },   //
		{ 0x02, 1, 0x942F },   // (上月)反向有功总电能
		{ 0x02, 1, 0x9420 },   //
		{ 0x02, 1, 0x9421 },   //
		{ 0x02, 1, 0x9422 },   //
		{ 0x02, 1, 0x9423 },   //
		{ 0x02, 1, 0x9424 },

		// 抄表日冻结
		{ 0x03, 1, 0x9010 },   //
		{ 0x03, 1, 0x901F },   // (当前)正向有功总电能
		{ 0x03, 1, 0x9010 },   //
		{ 0x03, 1, 0x9011 },   //
		{ 0x03, 1, 0x9012 },   //
		{ 0x03, 1, 0x9013 },   //
		{ 0x03, 1, 0x9014 },   //

		{ 0x03, 1, 0x9020 },   //
		{ 0x03, 1, 0x902F },   // (当前)反向有功总电能
		{ 0x03, 1, 0x9020 },   //
		{ 0x03, 1, 0x9021 },   //
		{ 0x03, 1, 0x9022 },   //
		{ 0x03, 1, 0x9023 },   //
		{ 0x03, 1, 0x9024 },   //
		};

///////////////////////////////////////////////////////////////////////////////
// global function
// Add 2009-7-27 for save 645 di

void* FindDIChlLen(WORD di, void* value, int i) {
	void* pBuf = nullptr;
	switch (di | 0x000F) {
	case 0x901F: {
		pBuf = &((EM_DI901F*) value)->DI901[i];
	}
		break;

	case 0x902F: {
		pBuf = &((EM_DI902F*) value)->DI902[i];
	}
		break;

	case 0x911F: {
		pBuf = &((EM_DI911F*) value)->DI911[i];
	}
		break;

	case 0x912F: {
		pBuf = &((EM_DI912F*) value)->DI912[i];
	}
		break;

	case 0x913F: {
		pBuf = &((EM_DI913F*) value)->DI913[i];
	}
		break;

	case 0x914F: {
		pBuf = &((EM_DI914F*) value)->DI914[i];
	}
		break;

	case 0x915F: {
		pBuf = &((EM_DI915F*) value)->DI915[i];
	}
		break;

	case 0x916F: {
		pBuf = &((EM_DI916F*) value)->DI916[i];
	}
		break;

		// 上月数据
	case 0x941F: {
		pBuf = &((EM_DI941F*) value)->DI941[i];
	}
		break;

	case 0x942F: {
		pBuf = &((EM_DI942F*) value)->DI942[i];
	}
		break;

	case 0x951F: {
		pBuf = &((EM_DI951F*) value)->DI951[i];
	}
		break;

	case 0x952F: {
		pBuf = &((EM_DI952F*) value)->DI952[i];
	}
		break;

	case 0x953F: {
		pBuf = &((EM_DI953F*) value)->DI953[i];
	}
		break;

	case 0x954F: {
		pBuf = &((EM_DI954F*) value)->DI954[i];
	}
		break;

	case 0x955F: {
		pBuf = &((EM_DI955F*) value)->DI955[i];
	}
		break;

	case 0x956F: {
		pBuf = &((EM_DI956F*) value)->DI956[i];
	}
		break;
	}

	return pBuf;
}
// end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// CT645Protocol
///////////////////////////////////////////////////////////////////////////////

CT645Protocol::CT645Protocol(BYTE nMPT, PF_DL645_SAVE pSave) :
		CGMProtocol(nMPT), m_pfSave(pSave) {
}

CT645Protocol::~CT645Protocol(void) {
}
////函数功能:645组帧
////参数:pObj:测量点类指针,取电表地址;pCMD:命令类指针,gtt,cmd,id;lpBuf:存放645帧数据;nBufSize:数据缓冲区大小
////返回值:>0 成功<0 失败
int CT645Protocol::HandleTx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
		int nBufSize) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);

	DL645_FRAME frame;
	BYTE lpData[32] = { 0 };
	frame.data = lpData;

	CMD_T645 *ptr = (CMD_T645*) pCMD->m_body;
	int nRet = 0;

	switch (ptr->cmd) {
	case EM_CMD_READDATA:
		nRet = DL645_ReadData(&frame, pObj->m_host, ptr->di);
		break;

	default:
		nRet = -1;
		break;
	}

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
////函数功能:处理收到的645组帧
////参数:pObj:测量点类指针,取电表地址;pCMD:命令类指针;lpBuf:存放645帧数据;nBufSize:数据缓冲区大小
////返回值:>0 成功<0 失败
int CT645Protocol::HandleRx(CGMPoint *pObj, Command *pCMD, BYTE *lpBuf,
		int nBufSize) {
	assert(pObj != nullptr);
	assert(pCMD != nullptr);
	assert(lpBuf != nullptr);
//	assert( m_pSave != nullptr );

	CMD_T645 *ptr = (CMD_T645*) pCMD->m_body;

	BYTE lpRecv[128] = { 0 };
	DL645_FRAME frm;
	frm.data = lpRecv;

	if (DL645_GetFrame(&frm, lpBuf, nBufSize) > 0) {
		if (frm.hdr.code.mask.DIR == 1)  ///电表发送过来的标志
				{
//			CMD_T645 *ptr = (CMD_T645*)pCMD->m_body;
			if ((frm.hdr.code.mask.CMD == ptr->cmd)
					&& (frm.hdr.code.mask.ACD == 0)) //////与发出的命令帧进行比较 命令字相同且不是异常数据
					{
				WORD di = MAKEWORD(frm.data[0], frm.data[1]);  ////标识符
				if (di == ptr->di)  ////标识符相等
						{
					int nDataLen = 0;
					if (frm.length > 2) {
						nDataLen = frm.length - 2;
					} else {
						return 0;
					}
					BYTE szData[128] = { 0 };
					memset(szData, 0, 128);
					memcpy(szData, &frm.data[2], nDataLen);

					int value[128 / sizeof(int)] = { 0 };
					DL645_Decode(di, value, &frm.data[2]);
					//////////////////////////////////////////////////////////////////
					// Add 2009-7-27 解决抄电表数据块时电表上报不全
					if ((HIBYTE(di) & 0xF0) == 0x90)	// 电能值
							{
						WORD chlDI = 0;

						BYTE num[5] = { 0, 1, 2, 3, 4 }; //正常序号为总尖峰平谷

						if ((nDataLen >= 16) && (nDataLen < 20)) //总加三费率，一般为峰平谷
								{
							num[0] = 0;
							num[1] = 2;
							num[2] = 3;
							num[3] = 4;
							num[4] = 1;
						}

						for (int i = 0; i < 5; i++) {
							chlDI = (di & 0xFFF0) | num[i];

							if ((i + 1) * 4 > nDataLen) {
								break;
								//	memset( value, 0xEE, sizeof(value) );
								//	m_pfSave(pObj, (void*)ptr->gtt, chlDI, FindDIChlLen(di, (void*)value, i));
								//	continue;
							}

							m_pfSave(pObj, (void*) ptr->gtt, chlDI,
									FindDIChlLen(di, (void*) value, i));
						}
					} else {
						if (di == 0xC010) {
							EM_DIC010 *pC010 = (EM_DIC010 *) value;
//							OSDateTime time = OSDateTime::GetCurrentTime();
//							if ((pC010->year != time.GetYear())
//									|| (pC010->month != time.GetMonth())
//									|| (pC010->m_day != time.GetDay())) {
//								pObj->m_nMPTimeEnable &= (~0x01);
//							} else {
//								pObj->m_nMPTimeEnable |= 0x01;
//							}
						} else if (di == 0xC011) {
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
						m_pfSave(pObj, (void*) ptr->gtt, di, (void*) value);
					}
				}
			}
		}
		return 0;
	}
	// Modify 2009-4-28
	/*	else if( ptr->gtt == 1 )
	 // end
	 {
	 int value[128/sizeof(int)]={0xEE};
	 memset(value,0xEE,128/sizeof(int));
	 m_pfSave(pObj,(void*)ptr->gtt,ptr->di,(void*)value);
	 //return 0;
	 }*/
	return -1;
}
////函数功能:得到命令类指针对象
////参数:parms ,pCMDs:Command 类指针 *& 可以改变指针指向;
////返回值:返回数组下标索引

int CT645Protocol::GetCommands(void *parms, Command *&pCMDs, PointType type) {
	Command **ppCMD = &pCMDs;
	BYTE gtt = (BYTE) *(int *)parms;;
	int nNums = 0;
	int nSize = 0;

	// 增加判断 单、三相表
	nSize = sizeof(l_commands_1) / sizeof(CMD_T645);
	for (int i = 0; i < nSize; ++i) {
		if (gtt == l_commands_1[i].gtt) {
			*ppCMD = new Command;
			if (*ppCMD != nullptr) {
				(*ppCMD)->m_nAck = 1;
				((CMD_T645*) ((*ppCMD)->m_body))->gtt = l_commands_1[i].gtt;
				((CMD_T645*) ((*ppCMD)->m_body))->cmd = l_commands_1[i].cmd;
				((CMD_T645*) ((*ppCMD)->m_body))->di = l_commands_1[i].di;
				(*ppCMD)->m_pNext = nullptr;
				(*ppCMD)->m_pAck = nullptr;
				ppCMD = &(*ppCMD)->m_pNext;
				nNums++;
			}
		}
	}

	if (Mul_Point == type) {
		nSize = sizeof(l_commands) / sizeof(CMD_T645);
		for (int i = 0; i < nSize; ++i) {
			if (gtt == l_commands[i].gtt) {
				*ppCMD = new Command;
				if (*ppCMD != nullptr) {
					(*ppCMD)->m_nAck = 1;
					((CMD_T645*) ((*ppCMD)->m_body))->gtt = l_commands[i].gtt;
					((CMD_T645*) ((*ppCMD)->m_body))->cmd = l_commands[i].cmd;
					((CMD_T645*) ((*ppCMD)->m_body))->di = l_commands[i].di;
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

int CT645Protocol::GetDefaultComDcb(ComDcb *pDcb) {
	assert(pDcb != nullptr);

	pDcb->nBaud = CB_1200;
	pDcb->databit = DB_8;
	pDcb->parity = CP_EVEN;
	pDcb->stopbit = CS_ONE;

	return 0;
}
} /* namespace pcols */
