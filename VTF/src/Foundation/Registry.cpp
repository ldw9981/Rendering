#include "StdAfx.h"
#include "Registry.h"
#include <tchar.h>
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi")
namespace Sophia
{

Registry::Registry(void)
{
}

Registry::~Registry(void)
{
}

BOOL Registry::WriteStringToINI(LPCTSTR lpFileName,LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPCTSTR lpString)
{
	if (lpFileName!=NULL)
	{
		return ::WritePrivateProfileString(lpSectionName,lpKeyName,lpString,lpFileName);
	}
	return ::WriteProfileString(lpSectionName,lpKeyName,lpString);
}

DWORD Registry::ReadStringFromINI( LPCTSTR lpFileName,LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPCTSTR lpDefault,LPTSTR lpReturnedString,DWORD nBufferSize )
{
	if (lpFileName!=NULL)
	{
		return ::GetPrivateProfileString(lpSectionName,lpKeyName,lpDefault,lpReturnedString,nBufferSize,lpFileName);
	}
	return ::GetProfileString(lpSectionName,lpKeyName,lpDefault,lpReturnedString,nBufferSize);
}

BOOL Registry::WriteIntToINI( LPCTSTR lpFileName,LPCTSTR lpSectionName,LPCTSTR lpKeyName,INT nValue )
{
	TCHAR buffer[1024]={0,};
	_itot_s(nValue,&buffer[0],1024,10);
	return WriteStringToINI(lpFileName,lpSectionName,lpKeyName,buffer);
}

UINT Registry::ReadIntFromINI( LPCTSTR lpFileName,LPCTSTR lpSectionName,LPCTSTR lpKeyName,INT nDefault )
{
	if (lpFileName!=NULL)
	{
		return ::GetPrivateProfileInt(lpSectionName,lpKeyName,nDefault,lpFileName);
	}
	return ::GetProfileInt(lpSectionName,lpKeyName,nDefault);	
}



// 정수값을 읽는다.
int Registry::ReadIntFromREG(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault)
{
	HKEY key;
	DWORD dwDisp;
	UINT Result;
	DWORD Size;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return 0;
	Size=sizeof(LONG);
	if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)&Result, &Size)
		!=ERROR_SUCCESS)
		Result=nDefault;
	RegCloseKey(key);
	return Result;
}

// 문자열을 읽는다.
bool Registry::ReadStringFromREG(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nBufferSize)
{
	HKEY key;
	DWORD dwDisp;
	DWORD Size;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return false;
	Size=nBufferSize;
	if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)lpRet, &Size)
		!=ERROR_SUCCESS) {
			lstrcpy(lpRet, lpDefault);
			return FALSE;
	}
	RegCloseKey(key);
	return true;
}

// 정수값을 쓴다.
bool Registry::WriteIntToREG(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) {
			return false;
	}
	if (RegSetValueEx(key, lpValue,0,REG_DWORD,(LPBYTE)&nData,sizeof(UINT))
		!=ERROR_SUCCESS) 
		return FALSE;
	RegCloseKey(key);
	return true;
}

// 문자열을 쓴다.
bool Registry::WriteStringToREG(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return false;
	if (RegSetValueEx(key, lpValue,0,REG_SZ,(LPBYTE)lpData,lstrlen(lpData)+1)
		!=ERROR_SUCCESS) 
		return FALSE;
	RegCloseKey(key);
	return true;
}

// 레지스트리 키를 삭제한다.
bool Registry::DeleteREGKey(HKEY hKey, LPCTSTR lpKey)
{
	return (::SHDeleteKey(hKey,lpKey) == ERROR_SUCCESS);
}

// 멤버 함수들. 생성자에 등록한 하이브와 루트키 아래의 서브키만 액세스한다.
int Registry::ReadIntFromREG(LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault)
{
	TCHAR Key[MAX_PATH];

	wsprintf(Key,_T("%s\\%s"),m_szRegPath,lpKey);
	return ReadIntFromREG(HiveKey,Key,lpValue,nDefault);
}

bool Registry::ReadStringFromREG(LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nBufferSize)
{
	TCHAR Key[MAX_PATH];

	wsprintf(Key,_T("%s\\%s"),m_szRegPath,lpKey);
	return ReadStringFromREG(HiveKey,Key,lpValue,lpDefault,lpRet,nBufferSize);
}

bool Registry::WriteIntToREG(LPCTSTR lpKey, LPCTSTR lpValue, UINT nData)
{
	TCHAR Key[MAX_PATH];

	wsprintf(Key,_T("%s\\%s"),m_szRegPath,lpKey);
	return WriteIntToREG(HiveKey,Key,lpValue,nData);
}

bool Registry::WriteStringToREG(LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	TCHAR Key[MAX_PATH];

	wsprintf(Key,_T("%s\\%s"),m_szRegPath,lpKey);
	return WriteStringToREG(HiveKey,Key,lpValue,lpData);
}

}