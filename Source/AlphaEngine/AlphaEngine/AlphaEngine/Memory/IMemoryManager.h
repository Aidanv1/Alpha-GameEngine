#ifndef IMEMORYMANAGER_H
#define IMEMORYMANAGER_H
//======================================================================
class IMemoryManager
{
public:
	virtual void* Alloc() = 0;
	virtual void Free(void* pBlock) = 0;
	virtual unsigned int GetTotalMemoryAllocated() = 0;
};

#endif
//======================================================================