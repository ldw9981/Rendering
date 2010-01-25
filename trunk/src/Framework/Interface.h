#pragma once
#include <windows.h>
#include <list>

using namespace std;
class cGUIBase;

namespace Framework
{
	class IUnknown
	{
	public:
		IUnknown(void){};
		virtual ~IUnknown(void){};
	};

	class IRenderer
	{
	public:
		virtual void Render()=0;
	};



	class IRenderable
	{
	public:
		list<IRenderable*>::iterator m_ItRenderable;
		virtual void ProcessRender()=0;
	};


	class IControlable
	{
	public:
		list<IControlable*>::iterator m_ItControlable;
		virtual void Control()=0;
	};

	class IUpdatable
	{
	public:
		list<IUpdatable*>::iterator m_ItProgressable;
		virtual void Update(DWORD elapseTime)=0;
	};

	
	class INotifiable
	{
	public:
		virtual void Notify(cGUIBase* pSource,DWORD msg,DWORD lParam,DWORD wParam)=0;
	};


}
