#include "MemoryPool.h"
// -----------------------------------------------------------------------
MemoryPool::MemoryPool() :
m_ppMemoryBlock(NULL),
m_pHead(NULL),
m_blockSize(0),
m_chunkSize(0),
m_totalChunkSize(0),
m_memoryArraySize(0),
m_numChunks(0)
{
}
// -----------------------------------------------------------------------
MemoryPool::~MemoryPool()
{
	delete[] m_ppMemoryBlock;
}
// -----------------------------------------------------------------------
bool MemoryPool::Init(int chunkSize, int numChunks)
{
	m_totalChunkSize = chunkSize + CHUNK_HEADER_SIZE;
	m_memoryArraySize = numChunks * (m_totalChunkSize);
	m_ppMemoryBlock = new unsigned char*[m_memoryArraySize];
	if (!m_ppMemoryBlock)
	{
		return false;
	}
	//setup linked list
	for (int i = 0; i < numChunks * (m_totalChunkSize) - m_totalChunkSize; i += m_totalChunkSize)
	{
		//make first element of the memory array point to next chunk header
		int index = i + m_totalChunkSize + (int)m_ppMemoryBlock;
		unsigned char* tempPtr = (unsigned char*)(index);
		m_ppMemoryBlock[i] = tempPtr;
	}
	m_ppMemoryBlock[(numChunks - 1) * (m_totalChunkSize)] = NULL;
	m_pHead = (unsigned char*)m_ppMemoryBlock;
	return true;
}
// -----------------------------------------------------------------------
void* MemoryPool::Alloc()
{
	if (!m_pHead)
	{
		return NULL;
	}
	unsigned char* pCurr = m_pHead;
	unsigned char* pNext = GetNext(m_pHead);
	m_pHead = pNext;
	return pCurr + CHUNK_HEADER_SIZE;
}
// -----------------------------------------------------------------------
void MemoryPool::Free(void* pChunk)
{
	if (pChunk != NULL)
	{
		int index = (int)pChunk - (int)m_ppMemoryBlock - CHUNK_HEADER_SIZE;
		m_ppMemoryBlock[index] = m_pHead;
		m_pHead = (unsigned char*)pChunk - CHUNK_HEADER_SIZE;
		pChunk = NULL;
	}
}
// -----------------------------------------------------------------------
unsigned char* MemoryPool::GetNext(unsigned char* pCurr)
{ 
	int index = ((int)pCurr - (int)m_ppMemoryBlock);
	unsigned char* newPtr = m_ppMemoryBlock[index];
	return newPtr;

}
// -----------------------------------------------------------------------
unsigned int MemoryPool::GetSize()
{
	return m_memoryArraySize;
}
//========================================================================
