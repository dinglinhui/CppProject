/*
 * cil_fk.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef PCOL_FK_H_
#define PCOL_FK_H_

#include "OSDefs.h"
#include <FK_protocol.h>

namespace pcols {

// 数据单元
typedef struct tag_FK_CELL {
	BYTE* lpBuf;			// 数据区
	int nLen;			// 数据区长度
} FK_CELL;

// 链路用户数据（应用层）
typedef struct tag_FK_LDATA {
	BYTE afn;		// 应用层功能码
	FK_SEQ seq;		// 帧序列域
	FK_CELL cell;		// 数据单元
	FK_AUX aux;		// 附加信息域
} FK_LDATA;

// 用户数据区
typedef struct tag_FK_UDATA {
	FK_C code;		// 功能码
	BYTE broadcast;	// 地址类型
	FK_ADDRESS rtua;		// 地址域
	FK_LDATA lnkData;	// 链路用户数据
} FK_UDATA;

extern void FK_RequestLink(FK_UDATA *udt);
extern void FK_ReportData(BYTE afn, FK_UDATA *udt);
extern BOOL FK_RespondEx(FK_UDATA *dst, FK_UDATA *src, BOOL bAck);

extern int FK_HasAUX(BYTE afn);
extern int FK_IsValid(BYTE *lpBuf, int nSize);
extern int FK_MakeFrame(FK_UDATA *udt, BYTE *lpBuf, int nSize);
extern int FK_GetFrame(FK_UDATA *udt, BYTE *lpBuf, int nSize);

extern WORD plms_cvt_da(WORD pn);
extern WORD plms_cvt_dt(BYTE fn);
extern BYTE plms_cvt_fn(WORD dt);
extern WORD plms_cvt_pn(WORD da);

extern int plms_tdh_tran(BYTE *pTran, FK_TDH *hTd);
extern int plms_tdh_appl(BYTE *pTran, FK_TDH *hTd);
extern int plms_tdd_tran(BYTE *pTran, FK_TDD *hTd);
extern int plms_tdd_appl(BYTE *pTran, FK_TDD *hTd);
extern int plms_tdc_tran(BYTE *pTran, FK_TDC *hTd);
extern int plms_tdc_appl(BYTE *pTran, FK_TDC *hTd);
extern int plms_tdm_tran(BYTE *pTran, FK_TDM *hTd);
extern int plms_tdm_appl(BYTE *pTran, FK_TDM *hTd);

extern int plms_fmt_appl(int nClass, BYTE* pTran, void* pAppl);
extern int plms_fmt_tran(int nClass, BYTE* pTran, void* pAppl);

extern int plms_fmt_dt(BYTE afn, WORD dt, BYTE* pTran, void* pAppl);
extern int plms_fmt_fn(BYTE afn, BYTE fn, BYTE* pTran, void* pAppl);

} /* namespace pcols */

#endif /* PCOL_FK_H_ */
