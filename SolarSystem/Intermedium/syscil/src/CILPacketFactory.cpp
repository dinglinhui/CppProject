#include "CILPacketFactory.h"

///////////////////////////////////////////////////////////////////////////////
// CILPacketFactory
///////////////////////////////////////////////////////////////////////////////
namespace syscil {

CILPacketFactoryImpl* CILPacketFactory::ms_pImpl = nullptr;

void CILPacketFactory::SetImplementation(CILPacketFactoryImpl *ptr) {
	ms_pImpl = ptr;
}

CILPacket* CILPacketFactory::Create(PCOL_TYPE type) {
	if (ms_pImpl != nullptr) {
		return ms_pImpl->Create(type);
	}
	return nullptr;
}

//void * Create(const char * class_name) {
//	std::string strClassName;
//
//#if defined (WIN32)
//	strClassName = ("class ");
//#else
//	char szSize[4] = { 0 };
//	sprintf(szSize, "%d", (int) strlen(class_name));
//	strClassName = szSize;
//#endif
//	strClassName += class_name;
//
//	/* RTTI support */
//	return !strcmp(typeid(_object).name(), strClassName.c_str()) ? (void *) (new Type) : nullptr;
//}

//void *Create(const char * class_name) {
//	void * new_object = 0;
//	const CFactoryList * cur = CFactoryList::_head;
//
//	for (; cur; cur = cur->m_next) {
//		/* if class_name matched, object will then be created and returned */
//		if ((new_object = cur->m_item->Create(class_name)) != nullptr) {
//			break;
//		}
//	}
//
//	return new_object;
//}
}
///////////////////////////////////////////////////////////////////////////////
// End of file: CILPacketFactory.cpp
///////////////////////////////////////////////////////////////////////////////
