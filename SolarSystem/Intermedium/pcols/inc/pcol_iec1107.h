/*
 * pcol_iec1107.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef INTERMEDIUM_PCOLS_INC_PCOL_IEC1107_H_
#define INTERMEDIUM_PCOLS_INC_PCOL_IEC1107_H_

#include "OSDefs.h"

namespace pcols {

typedef enum tag_IEC_FLAG {
	IEC_SOH = 0x01,
	IEC_STX = 0x02,
	IEC_ETX = 0x03,
	IEC_EOT = 0x04,
	IEC_ACK = 0x06,
	IEC_LF = 0x0A,
	IEC_CR = 0x0D,
	IEC_NAK = 0x15
} IEC_FLAG;

typedef enum tag_IEC_CODE {
	IEC_START = 0x01,	// 启动
	IEC_READDATA = 0x02,	// 读计费数据
	IEC_ORIGREAD = 0x03
} IEC_CODE;

typedef struct tag_IEC_HDR {
	BYTE host[6];
	BYTE code;
	WORD length;
} IEC_HDR;

typedef struct tag_IEC_FRAME {
	IEC_HDR hdr;
	BYTE* data;
} IEC_FRAME;

int IEC_Start(IEC_FRAME *frm);
int IEC_ReadData(IEC_FRAME *frm, BYTE nBaud);
int IEC_MakeFrame(IEC_FRAME *frm, BYTE *lpBuf, int nBufSize);
int IEC_GetFrame(IEC_FRAME *frm, BYTE *lpRecv, int nSize);
int IEC_Decode(BYTE type, BYTE *lpBuf, char* data, int nSize, int *nLastMonth);

} /* namespace pcols */

#endif /* INTERMEDIUM_PCOLS_INC_PCOL_IEC1107_H_ */
