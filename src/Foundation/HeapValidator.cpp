#include "StdAfx.h"
#include "HeapValidator.h"


void HeapValidator::SetDbgFlag()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

void HeapValidator::CheckMemory()
{
	_ASSERT_EXPR(_CrtCheckMemory(),L"an error or memory inconsistency is encountered in the underlying base heap, the debug header information, or the overwrite buffers");
}

void HeapValidator::SetBreakAlloc( long lNewBreakAlloc )
{
	_CrtSetBreakAlloc(lNewBreakAlloc);	
}