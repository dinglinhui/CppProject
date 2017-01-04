#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H
#include "StaticMemory.h"

/** @class ObjectManager
 * 实现利用内存池创建对象
 * 要求对象具有缺省构造函数
 */
template<typename T>
class ObjectManager {
public:
	typedef T ObjectType;

	static ObjectType* Create(StaticMemory* pool) {
		void* pObj = pool->Malloc(sizeof(T));
		new (pObj) ObjectType();
		return static_cast<ObjectType*>(pObj);
	}

	static void Delete(StaticMemory* pool, ObjectType* pObj) {
		pObj->~ObjectType();
		pool->Free(pObj);
	}
};
#endif
