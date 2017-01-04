#ifndef CIL_FKPACKET_H__
#define CIL_FKPACKET_H__

#include "pcol_fk.h"
#include "CILPacket.h"

///////////////////////////////////////////////////////////////////////////////
// CILFKPacket
///////////////////////////////////////////////////////////////////////////////
namespace syscil {
using namespace pcols;

class CILFKPacket: public CILPacket {
public:
	CILFKPacket(void);
	CILFKPacket(int nSize);
	CILFKPacket(BYTE *lpBuf, int nSize);
	virtual ~CILFKPacket(void);

	bool GetUData(FK_UDATA &udt);

	virtual int Create(BYTE *lpBuf, int nSize);
	virtual int IsValid(void);
	virtual int GetData(BYTE *& ptr);

	virtual PCMD_TYPE GetPcmdType(void);
	virtual CILPacket* Clone(void);
	virtual CILPacket* Duplicate(void);
};
}
#endif // __CIL_FKPACKET_H__

///////////////////////////////////////////////////////////////////////////////
// End of file: CILFKPacket.h
///////////////////////////////////////////////////////////////////////////////
