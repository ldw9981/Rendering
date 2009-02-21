#include "stdafx.h"
#include "Restore.h"

list<cIRestore*> cStaticRestoreList::m_listRestore;

cIRestore::cIRestore( void )
{
	m_listRestore.push_back(this);	
	m_ItRestore = --m_listRestore.end();	
}

cIRestore::~cIRestore( void )
{
	m_listRestore.erase(m_ItRestore);	
}