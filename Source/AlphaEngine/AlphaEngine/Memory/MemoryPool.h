/*Description:
MemoryPool implements a simple memory pool which uses a singly linked list data structure
*/
#pragma once
#include "../AlphaStd.h"
//========================================================================
class MemoryPool
{
	const static int CHUNK_HEADER_SIZE = sizeof(unsigned char*);	
public:
	MemoryPool();
	~MemoryPool();
	bool Init(int chunkSize, int numChunks);
	void* Alloc();
	void Free(void* pChunk);
	unsigned int GetSize();
private:
	unsigned char* GetNext(unsigned char* pCurr);

private:
	unsigned char** m_ppMemoryBlock;
	unsigned char* m_pHead;
	int m_blockSize;
	int m_chunkSize;
	int m_totalChunkSize;
	int m_numChunks;
	int m_memoryArraySize;
};
//========================================================================
