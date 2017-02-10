#ifndef CIL_645PACKET_H__
#define CIL_645PACKET_H__

#include "../../cil/inc/CILPacket.h"
#include "pcol_dl645.h"

///////////////////////////////////////////////////////////////////////////////
// CIL645Packet
///////////////////////////////////////////////////////////////////////////////
namespace syscil {
using namespace pcols;

class CIL645Packet: public CILPacket {
public:
	CIL645Packet(void);
	CIL645Packet(int nSize);
	CIL645Packet(BYTE *lpBuf, int nSize);
	virtual ~CIL645Packet(void);

	bool GetHDR(DL645_HDR &hdr);
	void Reversal(void);

	virtual int Create(BYTE *lpBuf, int nSize);
	virtual int IsValid(void);
	virtual int GetData(BYTE *& ptr);

	virtual PCMD_TYPE GetPcmdType(void);
	virtual CILPacket* Clone(void);
	virtual CILPacket* Duplicate(void);
};
}
#endif // __CIL_645PACKET_H__

///////////////////////////////////////////////////////////////////////////////
// End of file: CIL645Packet.h
///////////////////////////////////////////////////////////////////////////////
