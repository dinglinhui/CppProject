/*
 * cildl645.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef PCOL_DL645_H_
#define PCOL_DL645_H_

#include <time.h>
#include <EM_protocol.h>

namespace pcols {

typedef struct tag_DL645_HDR {
	BYTE rtua[6];
	EM_C code;
} DL645_HDR;

typedef struct tag_DL645_FRAME {
	DL645_HDR hdr;
	BYTE* data;
	int length;
} DL645_FRAME;

typedef int (*pf_encode)(BYTE*, void*);
typedef int (*pf_decode)(void*, BYTE*);

typedef struct tag_DL645_DATA {
	DWORD DI;
	WORD Plus;
	int nPackLen;
	int nDataLen;
	pf_encode pfEncode;
	pf_decode pfDecode;
} DL645_DATA;

extern const DL645_DATA s_DL645[];
extern int GetLenofs_DL645(void);

extern int DL645_GetPackLength(WORD di);
extern int DL645_07_GetPackLength(DWORD di);
extern int DL645_GetDataLength(WORD di);

extern int DL645_ReadData(DL645_FRAME *pFrame, BYTE host[6], WORD di);
extern int DL645_07_ReadData(DL645_FRAME *pFrame, BYTE host[6], DWORD di);
extern int DL645_CheckTime(DL645_FRAME *pFrame, time_t htm);

extern DL645_HDR DL645_Respond(DL645_HDR hdr);
extern DL645_HDR DL645_RespondError(DL645_HDR hdr);

extern int DL645_IsValid(BYTE *lpBuf, int nSize);
extern int DL645_MakeFrame(DL645_FRAME *pFrm, BYTE *lpBuf, int nSize);
extern int DL645_GetFrame(DL645_FRAME *pFrm, BYTE *lpBuf, int nSize);

extern int DL645_Encode(WORD di, BYTE *pTran, void *pAppl);
extern int DL645_07_Encode(DWORD di, BYTE *pTran, void *pAppl);
extern int DL645_Decode(WORD di, void *pAppl, BYTE *pTran);
extern int DL645_07_Decode(DWORD di, void *pAppl, BYTE *pTran);
} /* namespace pcols */

#endif /* PCOL_DL645_H_ */
