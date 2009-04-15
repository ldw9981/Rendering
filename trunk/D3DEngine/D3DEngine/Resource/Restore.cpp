#include "stdafx.h"
#include "Restore.h"

list<IRestore*> cStaticRestoreList::m_listRestore;

IRestore::IRestore( void )
{
	m_listRestore.push_back(this);	
	m_ItRestore = --m_listRestore.end();	
}

IRestore::~IRestore( void )
{
	m_listRestore.erase(m_ItRestore);	
}