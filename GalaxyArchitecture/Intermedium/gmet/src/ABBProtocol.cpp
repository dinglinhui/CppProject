/*
 * ABBProtocol.cpp
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#include "ABBProtocol.h"
#include "pcol_dl645.h"
#include "pcol_abb.h"
#include <cstdio>
#include <cassert>
#include "ctypes.h"

namespace pcols {

typedef struct tag_CMD_ABB {
	BYTE gtt;
	BYTE cmd;
	BYTE classVal; /////类的值
	BYTE phase;
} CMD_ABB;

static const CMD_ABB l_commands[] = { { 0x01, 0x18, 06 }, //
		{ 0x01, 0x18, 01 }, //
		{ 0x01, 5, 0 }, //
		{ 0x01, 5, 11 },
		//*********************************************************
		//以下是抄电压电流功率功率因数 会影响表的精度
		{ 0x01, 0x1c, 6, 0 },    //A相  Voltage
		{ 0x01, 0x1c, 6, 2 },    //B    Voltage
		{ 0x01, 0x1c, 6, 4 },    //C    Voltage

		{ 0x01, 0x1c, 5, 0 },    //A相	Current
		{ 0x01, 0x1c, 5, 2 },    //B    Current
		{ 0x01, 0x1c, 5, 4 },    //C相  Current

		{ 0x01, 0x1c, 7, 0 },    //A    kVar
		{ 0x01, 0x1c, 7, 2 },    //B    kVard
		{ 0x01, 0x1c, 7, 4 },    //C    kVar
		//{0x01,0x1c,7,7},

		{ 0x01, 0x1c, 9, 0 },	//A    kW
		{ 0x01, 0x1c, 9, 2 },	//B    kW
		{ 0x01, 0x1c, 9, 4 },	//C    kW
		//{0x01,0x1c,9,7},
		////Add 2009.3.7 zjz
		{ 0x01, 0x1c, 15, 7 }, //Sum kW
		{ 0x01, 0x1c, 22, 7 }, //Sum kVar
		//////////////
		{ 0x01, 0x1c, 17, 0 },	//A   Total Power factor
		{ 0x01, 0x1c, 17, 2 },	//B   Total Power factor
		{ 0x01, 0x1c, 17, 4 },	//C   Total Power factor
		{ 0x01, 0x1c, 17, 7 },
		//***********************************************************
		{ 0x01, 0x80, 0 },	//

		{ 0x02, 0x18, 06 }, //
		{ 0x02, 0x18, 01 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 5, 12 }, //
		{ 0x02, 0x80, 0 }, //

		{ 0x03, 0x18, 06 }, //
		{ 0x03, 0x18, 01 }, //
		{ 0x03, 5, 0 }, //
		{ 0x03, 5, 11 }, //
		{ 0x03, 0x80, 0 }, };

CABBProtocol::CABBProtocol(BYTE nMPT, PF_ABB_SAVE pSave) :
		CGMProtocol(nMPT),
		m_pfSave(pSave),
		bHaveNextFrame(false),
		bFirstReadClass(true),
		dl_DotPos(0),
		xl_DotPos(0),
		zxyg_dl(0.0),
		zxwg_dl(0.0),
		dy(0),
		realdata(0.0),
		dl(0.0),
		xl(0.0)

{

}
CABBProtocol::~CABBProtocol(void) {

}

int CABBProtocol::HandleTx(CGMPoint *pObj, Command *pCmd, BYTE *lpBuf, int nBufSize) {
	assert(pObj != nullptr);
	assert(pCmd != nullptr);
	static BYTE nfrmIndex = 0;	//帧序号
	CMD_ABB *ptr = (CMD_ABB*) pCmd->m_body;
	int nSize = 0;
//	int nRet  = 0;

	switch (ptr->cmd) {
	case 0x18: {
		if (ptr->classVal == 0x06) //通讯握手
				{
			nSize = Abb_ShakeHand(pObj->m_host, lpBuf, 0);
		} else if (ptr->classVal == 0x01) //密码认证
				{
			nSize = 0;
			nSize = Abb_CheckPassword(m_farpwd, pObj->m_password, lpBuf, 0);
		}
		break;
	}
	case 0x5:    //读类数据
	{
		if ((ptr->gtt == 1) || (ptr->gtt == 3)) {
			if (bFirstReadClass) {
				nSize = Abb_ReadClass(ptr->classVal, lpBuf, 0);
				bFirstReadClass = false;
			}
		}
		if (ptr->gtt == 2) {

			nSize = Abb_ReadHistoryData(Abb_GetCurrentMonth(), lpBuf, nfrmIndex);
			if (nfrmIndex++ == 8)
				nfrmIndex = 0;

		}

		break;
	}
	case 0x1c:     //读实时数据
	{
		nSize = Abb_ReadRealData(ptr->classVal, ptr->phase, lpBuf);
		break;
	}
	case 0x80: {
		nSize = Abb_OverDialog(lpBuf);
		break;
	}
	case 0x81: {
		if ((ptr->gtt == 1) || (ptr->gtt == 3)) //当前数据或者日冻结数据
				{
			if (bHaveNextFrame)
				nSize = Abb_SendNextFrame(lpBuf, 0);
		}
	}
	}

	return nSize;
}
int CABBProtocol::HandleRx(CGMPoint *pObj, Command *pCmd, BYTE *lpBuf, int nBufSize) {
	assert(pObj != nullptr);
	assert(pCmd != nullptr);
	assert(lpBuf != nullptr);
	assert(m_pfSave != nullptr);
	static BYTE ifrmIndex = 0;
	CMD_ABB *ptr = (CMD_ABB*) pCmd->m_body;
	//static BYTE iMaxShakeHands=0;//最大握手次数
	static BYTE iMaxReReadNums = 0; //最大重读次数
	switch (ptr->cmd) {
	case 0x18: {
		if (ptr->classVal == 0x6) {
			if ((nBufSize > 0) && Abb_IsValid(lpBuf, nBufSize) == 0) {
				Abb_GetPassword(lpBuf, m_farpwd);

				//////////2009.3.6
				return 0;
				//////////
			} else {
				return -1;
			}
		} else if (ptr->classVal == 0x1) {
			if ((nBufSize > 0) && Abb_IsValid(lpBuf, nBufSize) == 0) {
				if (lpBuf[2] == 0) {
					bFirstReadClass = true;
					///////Add 2009.3.6
					return 0;
					//////
				}
			}
		}
		break;
	}
	case 0x5: {

		if (ptr->classVal == 11) /////11类数据读当前的电量、需量、需量时间
				{
			if ((nBufSize > 0) && (Abb_IsValid(lpBuf, nBufSize) == 0)) //判断帧的正确性
					{
				bHaveNextFrame = Abb_HaveNextFrame(lpBuf, nBufSize);
				if (bHaveNextFrame) {
					ifrmIndex++;
					ParseClass11(ifrmIndex, pObj, pCmd, lpBuf, nBufSize);
					Command *pRes = new Command;
					pRes->m_nAck = 1;
					((CMD_ABB*) pRes->m_body)->gtt = ptr->gtt; ////modify 2008.12.26 old =1;
					((CMD_ABB*) pRes->m_body)->cmd = 0x81;
					((CMD_ABB*) pRes->m_body)->classVal = 0;
					pRes->m_pNext = nullptr;
					pRes->m_pAck = nullptr;
					pCmd->m_pAck = pRes;
				}
				return 0;
			}
		} else if (ptr->classVal == 0) //读电量小数位、需量小数位
				{
			if ((nBufSize > 0) && (Abb_IsValid(lpBuf, nBufSize) == 0)) {
				ParseClass0(lpBuf, nBufSize);
				bFirstReadClass = true;
				/////2009.3.6
				return 0;
				///////
			}
		} else if (ptr->classVal == 0xc) ///处理历史数据
				{
			ifrmIndex++;
			ParseClass11(ifrmIndex, pObj, pCmd, lpBuf, nBufSize);
			if (ifrmIndex == 9)
				ifrmIndex = 0;
			////2009.3.6
			return 0;
			////
		}
		break;
	}
	case 0x1c: //读实时数据如电压电流功率功率因数等
	{
		if ((nBufSize > 0) && (Abb_IsValid(lpBuf, nBufSize) == 0)) //判断帧的正确性
				{
			if (lpBuf[2] == 0xc) ////电表正在处理 继续发当前帧
					{

				if (iMaxReReadNums++ == 10)
					iMaxReReadNums = 0;
				else {
					Command *pRes = new Command;
					pRes->m_nAck = 1;
					((CMD_ABB*) pRes->m_body)->gtt = 1;
					((CMD_ABB*) pRes->m_body)->cmd = 0x1c;
					((CMD_ABB*) pRes->m_body)->classVal = ptr->classVal;
					((CMD_ABB*) pRes->m_body)->phase = ptr->phase;
					pRes->m_pNext = nullptr;
					pRes->m_pAck = nullptr;
					pCmd->m_pAck = pRes;
					////Add 2009.3.6
					return 0;
					////
				}
			} else if (lpBuf[2] == 0xb) ////没有该数据项不进行处理
					{
				iMaxReReadNums = 0;
				/////Add 2009.3.6
				return 0;
				/////
			} else {
				ParseClass1(pObj, pCmd, lpBuf, nBufSize);
				iMaxReReadNums = 0;
				/////Add 2009.3.6
				return 0;
				/////
			}
		}
		break;
	}
	case 0x81: {
		bHaveNextFrame = Abb_HaveNextFrame(lpBuf, nBufSize);
		if (bHaveNextFrame) {
			ifrmIndex++;
			ParseClass11(ifrmIndex, pObj, pCmd, lpBuf, nBufSize);
			if (ifrmIndex == 9)
				ifrmIndex = 0;
			Command *pRes = new Command;
			pRes->m_nAck = 1;
			((CMD_ABB*) pRes->m_body)->gtt = ptr->gtt; ////modify 2008.12.26
			((CMD_ABB*) pRes->m_body)->cmd = 0x81;
			((CMD_ABB*) pRes->m_body)->classVal = 0;
			pRes->m_pNext = nullptr;
			pRes->m_pAck = nullptr;
			pCmd->m_pAck = pRes;
			/////Add 2009.3.6
			return 0;
			/////
		} else {
			ifrmIndex++;
			ParseClass11(ifrmIndex, pObj, pCmd, lpBuf, nBufSize);
			ifrmIndex = 0;
			////Add 2009.3.6
			return 0;
			/////
		}

		break;
	}
	case 0x80: {
		static BYTE iRec = 0;
		if (iRec++ == 2) {
			iRec = 0;
			return 0;
		}
	}
	}
	//bHaveNextFrame=Abb_HaveNextFrame(lpBuf,nBufSize);
	return -1;

}

/////function:处理电量
//pValue:输出电量
//nType:类型
//dotPos:小数点位置
int CABBProtocol::GetDotVal(int idotPos) {
	int val = 1;
	for (int i = 0; i < idotPos; i++) {
		val *= 10;
	}
	return val;
}
void CABBProtocol::DisposeDL(int* pValue, BYTE nType, BYTE *lpBuf, BYTE dotPos) {
	char p[32];
	memset(p, 0, 32);
	if (nType == 1) /////Q4象限
			{
		sprintf(p, "%02x%02x%02x%02x", *(lpBuf + 5), *(lpBuf + 6), *(lpBuf + 7), *(lpBuf + 8));
	} else if (nType == 2) {
		sprintf(p, "%02x%02x%02x%02x", *(lpBuf + 26), *(lpBuf + 27), *(lpBuf + 28), *(lpBuf + 29));
	} else if (nType == 3) ///Q3象限
			{
		sprintf(p, "%02x%02x%02x%02x", *(lpBuf + 12), *(lpBuf + 13), *(lpBuf + 14), *(lpBuf + 15));
	} else if (nType == 4) ///Q2象限
			{
		sprintf(p, "%02x%02x%02x%02x", *(lpBuf + 19), *(lpBuf + 20), *(lpBuf + 21), *(lpBuf + 22));
	} else if (nType == 5) ///Q1象限
			{
		sprintf(p, "%02x%02x%02x%02x", *(lpBuf + 26), *(lpBuf + 27), *(lpBuf + 28), *(lpBuf + 29));
	}

	int val = GetDotVal(dotPos);
	dl = atof(p) / val;
	memcpy(pValue, &dl, sizeof(double));
}
void CABBProtocol::DisposeXL(int *pValue, BYTE nType, BYTE *lpBuf, BYTE dotPos) {
	char p[32];
	memset(p, 0, 32);
	if (nType == 1) {
		sprintf(p, "%02x%02x%02x", *(lpBuf + 12), *(lpBuf + 13), *(lpBuf + 14));
	} else if (nType == 2) {
		sprintf(p, "%02x%02x%02x", *(lpBuf + 33), *(lpBuf + 34), *(lpBuf + 35));
	}
	int val = GetDotVal(dotPos);
	xl = atof(p) / val;
	memcpy(pValue, &xl, sizeof(double));
}
void CABBProtocol::DisposeDY(BYTE nDY, int *pValue, BYTE *lpBuf) {
	char p[32];
	memset(p, 0, 32);
	if (nDY == 6) {
		sprintf(p, "%02x%02x%02x%02x", *(lpBuf + 5), *(lpBuf + 6), *(lpBuf + 7), *(lpBuf + 8)); //4位整数
		dy = StringToHex(p);
		memcpy(pValue, &dy, sizeof(WORD));
	} else {
		sprintf(p, "%02x%02x", *(lpBuf + 9), *(lpBuf + 10)); //2位小数
		realdata = StringToHex(p);
		realdata /= 65535;
		sprintf(p, "%02x%02x%02x%02x", *(lpBuf + 5), *(lpBuf + 6), *(lpBuf + 7), *(lpBuf + 8)); //4位整数
		realdata += StringToHex(p);

		//realdata+=StringToHex(p)/65535;
		//realdata=4.98;
		memcpy(pValue, &realdata, sizeof(double));
	}
}
void CABBProtocol::DisposeXLSJ(int *pValue, BYTE nType, BYTE *lpBuf) {
	BYTE d[4];
	char tmp[6] = { 0 };
	memset(d, 0, 4);
	memset(tmp, 0, 6);
	int nBos = 0;
	if (nType == 1) {
		while (nBos < 4) {
			sprintf(tmp, "%02x", *(lpBuf + 16 + nBos));
			d[nBos++] = atoi(tmp);
		}
	} else if (nType == 2) {
		while (nBos < 4) {
			sprintf(tmp, "%02x", *(lpBuf + 37 + nBos));
			d[nBos++] = atoi(tmp);
		}
	}
	memcpy(pValue, d, 4);
}

void CABBProtocol::ParseClass0(BYTE *lpBuf, int nBufSize) {
	char tmp[6];
	sprintf(tmp, "%02x", *(lpBuf + 16));
	dl_DotPos = atoi(tmp);
	sprintf(tmp, "%02x", *(lpBuf + 17));
	xl_DotPos = atoi(tmp);

}
void CABBProtocol::ParseClass1(CGMPoint* pObj, Command* pCMD, BYTE* lpBuf, int nBufSize) {
	int value[64 / sizeof(int)] = { 0 };
	CMD_ABB *ptr = (CMD_ABB*) pCMD->m_body;
	//memset(value,0,64);
	DisposeDY(ptr->classVal, value, lpBuf);
	switch (ptr->classVal) {
	case 5:		//Current
	{
		if (ptr->phase == 0)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB621, (void*) value);
		else if (ptr->phase == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB622, (void*) value);
		else if (ptr->phase == 4)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB623, (void*) value);
		break;
	}
	case 6:		//Voltage
	{
		if (ptr->phase == 0)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB611, (void*) value);
		else if (ptr->phase == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB612, (void*) value);
		else if (ptr->phase == 4)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB613, (void*) value);
		break;
	}
	case 7:		//KVar
	{
		if (ptr->phase == 0)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB641, (void*) value);
		else if (ptr->phase == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB642, (void*) value);
		else if (ptr->phase == 4)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB643, (void*) value);
		else if (ptr->phase == 7)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB640, (void*) value);
		break;
	}
	case 9:		//KW
	{
		if (ptr->phase == 0)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB631, (void*) value);
		else if (ptr->phase == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB632, (void*) value);
		else if (ptr->phase == 4)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB633, (void*) value);
		else if (ptr->phase == 7)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB630, (void*) value);
		break;
	}
	case 17:		//Power factor
	{
		if (ptr->phase == 0)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB651, (void*) value);
		else if (ptr->phase == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB652, (void*) value);
		else if (ptr->phase == 4)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB653, (void*) value);
		else if (ptr->phase == 7)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xB650, (void*) value);
		break;
	}
		/////////Add 2009.03.07 zjz
	case 15:		// Sum kW
		m_pfSave(pObj, (void*) &ptr->gtt, 0xB630, (void*) value);
		break;
	case 22:		// Sum kVar
		m_pfSave(pObj, (void*) &ptr->gtt, 0xB640, (void*) value);
		break;
		//////////
	}

}

/*
 sRec.Mid(10,14);     //5,7  //电量
 sRec.Mid(24,6);      //12,3 //需量
 sRec.Mid(30,10);     //15,5 //需量时间
 sRec.Mid(52,14);     //26,7  //电量
 sRec.Mid(66,6);      //33,3  //需量
 sRec.Mid(72,10);     //36,5  //需量时间
 */
void CABBProtocol::ParseClass11(BYTE frmIndex, CGMPoint* pObj, Command* pCMD, BYTE *lpBuf, int nBufSize) {
	int value[64 / sizeof(int)];
	//if(nBufSize<30)
	//	return ;
	CMD_ABB *ptr = (CMD_ABB*) pCMD->m_body;
	if (frmIndex == 1)  //正向有功
			{

		memset(value, 0, 64);
		DisposeDL(value, 1, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9412, (void*) value);	//峰电量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9012, (void*) value);	//峰电量
		zxyg_dl = dl;

		memset(value, 0, 64);
		DisposeDL(value, 2, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9413, (void*) value);	//平电量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9013, (void*) value);	//平电量
		zxyg_dl += dl;

		memset(value, 0, 64);
		DisposeXL(value, 1, lpBuf, xl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa412, (void*) value); //峰需量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa012, (void*) value); //峰需量

		memset(value, 0, 64);
		DisposeXL(value, 2, lpBuf, xl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa413, (void*) value); //平需量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa013, (void*) value); //平需量

		memset(value, 0, 64);
		DisposeXLSJ(value, 1, lpBuf);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb412, (void*) value); //峰需量时间
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb012, (void*) value); //峰需量时间

		memset(value, 0, 64);
		DisposeXLSJ(value, 2, lpBuf);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb413, (void*) value); //平需量时间
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb013, (void*) value); //平需量时间
	} else if (frmIndex == 2) //正向有功
			{
		memset(value, 0, 64);
		DisposeDL(value, 1, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9414, (void*) value);	//谷电量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9014, (void*) value);	//谷电量
		zxyg_dl += dl;

		memcpy(value, &zxyg_dl, sizeof(double));
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9410, (void*) value);	//正向有功总电量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9010, (void*) value);	//正向有功总电量

		memset(value, 0, 64);
		DisposeXL(value, 1, lpBuf, xl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa414, (void*) value); //谷需量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa014, (void*) value); //谷需量

		memset(value, 0, 64);
		DisposeXLSJ(value, 1, lpBuf);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb414, (void*) value); //谷需量时间
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb014, (void*) value); //峰需量时间
	} else if (frmIndex == 3) //正向无功
			{
		memset(value, 0, 64);
		DisposeDL(value, 1, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9512, (void*) value);	//峰电量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9112, (void*) value);	//峰电量
		zxwg_dl = dl;

		memset(value, 0, 64);
		DisposeDL(value, 2, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9513, (void*) value);	//平电量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9113, (void*) value);	//平电量
		zxwg_dl += dl;

		memset(value, 0, 64);
		DisposeXL(value, 1, lpBuf, xl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa512, (void*) value); //峰需量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa112, (void*) value); //峰需量

		memset(value, 0, 64);
		DisposeXL(value, 2, lpBuf, xl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa513, (void*) value); //平需量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa113, (void*) value); //平需量

		memset(value, 0, 64);
		DisposeXLSJ(value, 1, lpBuf);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb512, (void*) value); //峰需量时间
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb112, (void*) value); //峰需量时间

		memset(value, 0, 64);
		DisposeXLSJ(value, 2, lpBuf);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb513, (void*) value); //平需量时间
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb113, (void*) value); //平需量时间
	} else if (frmIndex == 4) //正向无功
			{
		memset(value, 0, 64);
		DisposeDL(value, 1, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9514, (void*) value);	//谷电量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9114, (void*) value);	//谷电量

		zxwg_dl += dl;
		memcpy(value, &zxwg_dl, sizeof(double));
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9510, (void*) value);	//正向无功总电量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9110, (void*) value);	//正向无功总电量

		memset(value, 0, 64);
		DisposeXL(value, 1, lpBuf, xl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa514, (void*) value); //谷需量
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xa114, (void*) value); //谷需量

		memset(value, 0, 64);
		DisposeXLSJ(value, 1, lpBuf);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb514, (void*) value); //峰需量时间
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0xb114, (void*) value); //峰需量时间
	} else if (frmIndex == 9)   //四象限
			{
		memset(value, 0, 64);
		DisposeDL(value, 1, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9540, (void*) value);	//Q4象限
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9140, (void*) value);	//Q4象限
		//zwg_dl=dl;

		memset(value, 0, 64);
		DisposeDL(value, 3, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9560, (void*) value);	//Q3象限
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9160, (void*) value); //Q3
		//zwg_dl+=dl;

		memset(value, 0, 64);
		DisposeDL(value, 4, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9550, (void*) value);	//Q2象限
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9150, (void*) value); //Q2
		//zwg_dl+=dl;

		memset(value, 0, 64);
		DisposeDL(value, 5, lpBuf, dl_DotPos);
		if (ptr->gtt == 2)
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9530, (void*) value);	//Q1象限
		else
			m_pfSave(pObj, (void*) &ptr->gtt, 0x9130, (void*) value); //Q1
		//zwg_dl+=dl;

		//memcpy(value,&zwg_dl,sizeof(double));
		//if (ptr->gtt==2)
		//	m_pfSave(pObj,(void*)&ptr->gtt,0x95D0,(void*)value);	//总无功电量
		//else
		//	m_pfSave(pObj,(void*)&ptr->gtt,0x91D0,(void*)value);	//总无功电量
	}
}

int CABBProtocol::GetCommands(void *parms, Command *&pCMDs, PointType) {
	Command **ppCMD = &pCMDs;
	BYTE gtt = (BYTE) *((int *) parms);
	int nNums = 0;
	int nSize = sizeof(l_commands) / sizeof(CMD_ABB);

//	FK_DATA data;
//	FK_DT04F94 fk_dt04f94;
//	data.DataPtr=(void*)&fk_dt04f94;
//	theApp.m_database.Read(GetAFNDT(0x04,94),&data);
	for (int i = 0; i < nSize; ++i) {
		if (gtt == l_commands[i].gtt) {
			*ppCMD = new Command;
			if (*ppCMD != nullptr) {
//				if ((fk_dt04f94.A1==3205) || (fk_dt04f94.A1==3202)) /////如果是苏州无锡的则全抄
//				{
//					(*ppCMD)->m_nAck = 1;
//					((CMD_ABB*)(*ppCMD)->m_body)->gtt = l_commands[i].gtt;
//					((CMD_ABB*)(*ppCMD)->m_body)->cmd = l_commands[i].cmd;
//					((CMD_ABB*)(*ppCMD)->m_body)->classVal = l_commands[i].classVal;
//					((CMD_ABB*)(*ppCMD)->m_body)->phase = l_commands[i].phase;
//					(*ppCMD)->m_pNext = nullptr;
//					(*ppCMD)->m_pAck  = nullptr;
//					ppCMD = &(*ppCMD)->m_pNext;
//					nNums++;
//				}

//				else/////不是的则则实时数据的不抄
				{
					if (l_commands[i].cmd != 0x1c) {
						(*ppCMD)->m_nAck = 1;
						((CMD_ABB*) (*ppCMD)->m_body)->gtt = l_commands[i].gtt;
						((CMD_ABB*) (*ppCMD)->m_body)->cmd = l_commands[i].cmd;
						((CMD_ABB*) (*ppCMD)->m_body)->classVal = l_commands[i].classVal;
						((CMD_ABB*) (*ppCMD)->m_body)->phase = l_commands[i].phase;
						(*ppCMD)->m_pNext = nullptr;
						(*ppCMD)->m_pAck = nullptr;
						ppCMD = &(*ppCMD)->m_pNext;
						nNums++;
					}
				}

			}
		}
	}
	return nNums;
}

int CABBProtocol::GetDefaultComDcb(ComDcb *pDcb) {
	assert(pDcb != nullptr);
	pDcb->nBaud = CB_1200;
	pDcb->databit = DB_8;
	pDcb->parity = CP_NONE;
	pDcb->stopbit = CS_ONE;
	return 0;
}

int CABBProtocol::GetRetryCount(void) {
	return 10;	// abb 10 times
}

} /* namespace pcols */
