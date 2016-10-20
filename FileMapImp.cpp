#include "StdAfx.h"
#include "FileMapImp.h"

CFileMapImp::CFileMapImp( const char *fileName, boost::interprocess::mode_t mode )
	:m_fMap(fileName, mode)
{
	m_pMapRegion = NULL;	
}

CFileMapImp::~CFileMapImp(void)
{
	UnMap();
}

int CFileMapImp::OpenMapping( size_t szOffset, size_t szMapSize )
{	
	UnMap();
	try
	{
		m_pMapRegion = new boost::interprocess::mapped_region(m_fMap, m_fMap.get_mode(), szOffset, szMapSize, 0);
	}
	catch (boost::interprocess::interprocess_exception ex)
	{
		return ex.get_native_error();
	}
	return FILEMAP_EXEC_SUCCESS;	
}

int CFileMapImp::UnMap()
{
	try
	{

		if (NULL != m_pMapRegion)
		{
			delete m_pMapRegion;
			m_pMapRegion = NULL;
		}
	}
	catch (boost::interprocess::interprocess_exception ex)
	{
		return ex.get_native_error();
	}
	return FILEMAP_EXEC_SUCCESS;
}

void * CFileMapImp::GetAddress()
{
	return (NULL != m_pMapRegion) ? m_pMapRegion->get_address(): NULL;
}

int CFileMapImp::Flush( size_t szOffSetStart /*= 0*/, size_t szNumsToFlush /*= 0*/, bool bAsync /*= true*/ )
{
	if (NULL == m_pMapRegion)
	{
		return FILEMAP_EXEC_FAILED;
	}
	else
	{
		try
		{
			return (int)m_pMapRegion->flush(szOffSetStart, szNumsToFlush, bAsync);
		}
		catch (boost::interprocess::interprocess_exception ex)
		{
			return ex.get_native_error();
		}		
	}
}

size_t CFileMapImp::GetSize()
{
	return (NULL != m_pMapRegion) ? m_pMapRegion->get_size() : 0;
}
