#pragma once

#define SAFE_RELEASE(p) if (p) { (p)->Release(); (p) = NULL; }
#define SAFE_DELETE(p) if (p) { delete p; (p) = NULL; }
#define SAFE_DELETEARRAY(p) if (p) { delete[] p; (p) = NULL; }

template <typename T>
void FuncDeleteType(T p)
{
	delete p;
}



	