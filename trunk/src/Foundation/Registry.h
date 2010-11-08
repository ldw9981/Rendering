#pragma once
#include <Windows.h>

#define SHLM HKEY_LOCAL_MACHINE
#define SHCU HKEY_CURRENT_USER
#define SHCR HKEY_CLASSES_ROOT

/*

WIN.INI	  -> INI파일명
[Desktop] -> 섹션 또는 AppName
Wallpaper=(없음) -> 키=키값(스트링orUINT)
TileWallpaper=0

*/
class Registry 
{
public:
	Registry(void);
	Registry(LPCTSTR szRegPath,HKEY aHive=SHCU) {
		HiveKey=aHive;
		lstrcpy(m_szRegPath,szRegPath);
		if (m_szRegPath[lstrlen(m_szRegPath)] == '\\') {
			m_szRegPath[lstrlen(m_szRegPath)]=0;
		}
	}
	~Registry(void);
private:
	HKEY HiveKey;
	TCHAR m_szRegPath[MAX_PATH];

public:
	// Write,Read (* INI파일명이 NULL이면 WIN.INI이 대상이된다.)
	static BOOL		WriteStringToINI(LPCTSTR lpFilePath,LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPCTSTR lpString);
	static BOOL		WriteIntToINI(LPCTSTR lpFilePath,LPCTSTR lpSectionName,LPCTSTR lpKeyName,INT nValue);
	static DWORD	ReadStringFromINI(LPCTSTR lpFilePath,LPCTSTR lpSectionName,LPCTSTR lpKeyName,LPCTSTR lpDefault,LPTSTR lpReturnedString,DWORD nBufferSize);
	static UINT		ReadIntFromINI(LPCTSTR lpFilePath,LPCTSTR lpSectionName,LPCTSTR lpKeyName,INT nDefault);

public:
	// Registry
	static int	ReadIntFromREG(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault);
	static bool ReadStringFromREG(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nBufferSize);
	static bool WriteIntToREG(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData);
	static bool WriteStringToREG(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
	static bool DeleteREGKey(HKEY hKey, LPCTSTR lpKey);

	int	 ReadIntFromREG(LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault);
	bool ReadStringFromREG(LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nBufferSize);
	bool WriteIntToREG(LPCTSTR lpKey, LPCTSTR lpValue, UINT nData);
	bool WriteStringToREG(LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
};
