#ifndef MEMORY_CHUNK_H
#define MEMORY_CHUNK_H
#include <cstdio>
#include <cassert>
#include <cstdlib>

#ifdef WIN32
#include <windows.h>
typedef CRITICAL_SECTION MUTEXTYPE;
#define INITMUTEX(hMutex) InitializeCriticalSection(&hMutex)
#define DELMUTEX(hMutex) DeleteCriticalSection(&hMutex)
#define LOCK(hMutex) EnterCriticalSection(&hMutex)
#define UNLOCK(hMutex) LeaveCriticalSection(&hMutex)
#else
#include <pthread.h>
typedef pthread_mutex_t MUTEXTYPE;
#define INITMUTEX(hMutex) pthread_mutex_init(&hMutex,nullptr)
#define DELMUTEX(hMutex) pthread_mutex_destroy(&hMutex)
#define LOCK(hMutex) pthread_mutex_lock(&hMutex)
#define UNLOCK(hMutex) pthread_mutex_unlock(&hMutex)
#endif

class MemoryChunk;

/** @struct MemoryBlock
 *
 */
struct BlockHeader {
	MemoryChunk* pChunk;
	size_t len;
};
struct MemoryBlock;
struct BlockData {
	union {
		MemoryBlock* pNext;
		char pBuffer;
	};
};

struct MemoryBlock {
	BlockHeader header;
	BlockData data;
};

/** @class MemoryChunk
 *
 */

class MemoryChunk {
public:
	MemoryChunk(size_t size, int count) {
		INITMUTEX(_hMutex);

		this->_pFreeList = nullptr;
		this->_size = size;
		this->_count = 0;

		MemoryBlock* pBlock = nullptr;
		while (count--) {
			pBlock = CreateBlock();
			if (!pBlock)
				break;

			pBlock->data.pNext = _pFreeList;
			_pFreeList = pBlock;
		}
	}

	~MemoryChunk() {
		int tempcount = 0;
		MemoryBlock* pBlock;
		while ((pBlock = _pFreeList) != nullptr) {
			_pFreeList = pBlock->data.pNext;
			DeleteBlock(pBlock);
			++tempcount;
		}

		assert(tempcount == _count); //!确保释放完全
		DELMUTEX(_hMutex);
	}

	void* malloc() {
		MemoryBlock* pBlock = nullptr;

		LOCK(_hMutex);
		if (_pFreeList != nullptr) {
			pBlock = _pFreeList;
			_pFreeList = pBlock->data.pNext;
		} else {
			if (!(pBlock = CreateBlock())) {
				UNLOCK(_hMutex);
				return nullptr;
			}
		}
		UNLOCK(_hMutex);

		return &pBlock->data.pBuffer;
	}

	static void free(void* pMem) {
		MemoryBlock* pBlock = (MemoryBlock*) ((char*) pMem - sizeof(BlockHeader));
		pBlock->header.pChunk->free(pBlock);
	}

	void free(MemoryBlock* pBlock) {
		LOCK(_hMutex);

		pBlock->data.pNext = _pFreeList;
		_pFreeList = pBlock;

		UNLOCK(_hMutex);
	}

//	MemoryChunk* Next() {
//		return _pNext;
//	}

protected:
	MemoryBlock* CreateBlock() {
		MemoryBlock* pBlock = (MemoryBlock*) ::malloc(sizeof(BlockHeader) + _size);

		if (pBlock != nullptr) {
			pBlock->header.pChunk = this;
			pBlock->header.len = _size;

			++_count;
		}
		return pBlock;
	}
	void DeleteBlock(MemoryBlock* pBlock) {
		::free(pBlock);
	}
private:
	MemoryBlock* _pFreeList;
	size_t _size; //!Block大小
	int _count; //!Block数目
//	MemoryChunk* _pNext;
	MUTEXTYPE _hMutex;
};
#endif
