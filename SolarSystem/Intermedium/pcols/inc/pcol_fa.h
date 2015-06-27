/*
 * pcol_fa.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef INTERMEDIUM_PCOLS_INC_PCOL_FA_H_
#define INTERMEDIUM_PCOLS_INC_PCOL_FA_H_

#include "FA_protocol.h"

namespace pcols {

#define FA_FRAME_MIN_LENGTH       (13)
#define FA_FRAME_MAX_MTU		  (800)
#define FA_FRAME_MAX_FRAME		  (FA_FRAME_MIN_LENGTH+FA_FRAME_MAX_MTU)

// Frame definition
typedef struct tag_FA_HDR {
	FA_ADDRESS rtua;
	FA_MSTASEQ msta_seq;
	FA_C code;
} FA_HDR;

typedef struct tag_FA_FRAME {
	FA_HDR hdr;
	int length;
	BYTE* data;
} FA_FRAME;

// Request
extern FA_HDR FA_Login(FA_ADDRESS rtua);
extern FA_HDR FA_Logout(FA_ADDRESS rtua);
extern FA_HDR FA_Heartbeat(FA_ADDRESS rtua);
extern FA_HDR FA_ReportTask(FA_ADDRESS rtua);
extern FA_HDR FA_ReportWarn(FA_ADDRESS rtua);
extern FA_HDR FA_ConnectSlave(BYTE host[4]);
extern FA_HDR FA_QuerySlave(BYTE host[4]);

// Respond
extern FA_HDR FA_Respond(FA_HDR hdr, int nISEQ);
extern FA_HDR FA_RespondError(FA_HDR hdr);

// Interface of frame
extern int FA_IsValid(BYTE *lpBuf, int nSize);
extern int FA_MakeFrame(FA_FRAME *pFrm, BYTE *lpBuf, int nSize);
extern int FA_GetFrame(FA_FRAME *pFrm, BYTE *lpBuf, int nSize);

// Convert
extern int FA_Encode(EM_DATA *data, BYTE *lpBuf);
extern int FA_Decode(EM_DATA *data, void *pObj);
extern int FA_Output(EM_DATA *data, char *szOut);

} /* namespace pcols */

#endif /* INTERMEDIUM_PCOLS_INC_PCOL_FA_H_ */
