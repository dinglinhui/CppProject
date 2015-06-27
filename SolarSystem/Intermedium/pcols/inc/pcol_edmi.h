/*
 * pcol_edmi.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef INTERMEDIUM_PCOLS_INC_PCOL_EDMI_H_
#define INTERMEDIUM_PCOLS_INC_PCOL_EDMI_H_

#include "OSDefs.h"

namespace pcols {

typedef enum tag_MKX_FLAG {
	MKX_STX = 0x02,
	MKX_ETX = 0x03,
	MKX_ACK = 0x06,
	MKX_DLE = 0x10,
	MKX_XON = 0x11,
	MKX_XOFF = 0x13,
	MKX_NAK = 0x15,
	MKX_CAN = 0x18
} MKX_FLAG;

typedef struct tag_MKX_HDR {
	BYTE code;
	BYTE srce[4];
	BYTE dest[6];
	int length;
	WORD nIndx;
} MKX_HDR;

typedef struct tag_MKX_FRAME {
	MKX_HDR hdr;
	BYTE* data;
} MKX_FRAME;

int MKX_Access(MKX_FRAME *frm);
int MKX_Logon(MKX_FRAME *frm, const char* szId, const char* szPwd);
int MKX_Logoff(MKX_FRAME *frm);
int MKX_Read(MKX_FRAME *frm, DWORD reg_num);
int MKX_MakeFrame(MKX_FRAME *frm, BYTE *lpBuf, int nBufSize);
int MKX_GetFrame(MKX_FRAME *frm, BYTE *lpBuf, int nSize);
int MKX_Convert(BYTE *lpBuf, BYTE *lpSrc);
} /* namespace pcols */

#endif /* INTERMEDIUM_PCOLS_INC_PCOL_EDMI_H_ */
