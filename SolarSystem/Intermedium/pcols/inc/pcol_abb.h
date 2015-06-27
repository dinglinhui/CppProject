/*
 * cil_abb.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef INTERMEDIUM_PCOLS_INC_PCOL_ABB_H_
#define INTERMEDIUM_PCOLS_INC_PCOL_ABB_H_

#include "OSDefs.h"

namespace pcols {

extern int Abb_IsValid(BYTE *lpBuf, int nSize);
extern int Abb_ShakeHand(BYTE host[6], BYTE *lpBuf, int nSize); ///握手
extern int Abb_CheckPassword(char *pfarPwd, char *pkeyPwd, BYTE *lpBuf,
		int nSize); //密码认证
extern unsigned int CalCRC(unsigned char *ptr, int count); //计算校验
extern unsigned long GetPassword(char *farPassword, char *keyPassword); //
extern int Abb_ReadClass(BYTE nClass, BYTE *lpBuf, int nSize);
extern void Abb_GetPassword(BYTE *lpBuf, char *pPwd);
extern int Abb_SendNextFrame(BYTE *lpBuf, int nSize); //抄读后续数据
extern bool Abb_HaveNextFrame(BYTE *lpBuf, int nSize); //有没有后续帧
extern int Abb_OverDialog(BYTE *lpBuf); //结束对话
extern int Abb_ReadHistoryData(BYTE nMonth, BYTE *lpBuf, int nIndex); //读历史数据
extern int Abb_GetCurrentMonth();
extern int Abb_ReadRealData(BYTE nStartDataNum, BYTE nPhase, BYTE *lpBuf);

} /* namespace pcols */

#endif /* INTERMEDIUM_PCOLS_INC_PCOL_ABB_H_ */
