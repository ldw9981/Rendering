#pragma once
#include "Resource.h"

class cRscTransformAnm:
	public cResource
{
public:
	cRscTransformAnm();
	~cRscTransformAnm();
protected:
private:
public:
	// cResource
	virtual	BOOL			Create();	
	virtual	void			Free();

	// cIRestore
	virtual void			Restore();
};