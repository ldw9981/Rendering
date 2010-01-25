#pragma once

#define DECLARE_SHAREDCLASS(classname) \
class Shared##classname \
{ \
protected: \
	static classname m_Shared##classname; \
};


#define IMPLEMENT_SHAREDCLASS(classname) \
classname Shared##classname::m_Shared##classname;

