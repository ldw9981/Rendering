#pragma once

namespace Sophia
{

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

// Validate Heap Memory
class HeapValidator
{
public:
	HeapValidator(void)
	{		
	}
	~HeapValidator(void)
	{
	}
	static void SetDbgFlag();
	// set break when alloc and count same  
	// {128} client block at 0x002550A0, subtype 0, 3 bytes long. 128 is counter 
	static void SetBreakAlloc(long lNewBreakAlloc);
	static void CheckMemory();
};
}