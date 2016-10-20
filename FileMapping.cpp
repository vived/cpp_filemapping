// FileMapping.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "FileMapping.h"
#include "FileMapImp.h"
#include "boost/interprocess/file_mapping.hpp"
#include <fstream>

CFileMapping::CFileMapping()
{
	m_pImp = NULL;
}

CFileMapping::~CFileMapping()
{
	CloseMap();
}

int CFileMapping::OpenMapping( const char *pFileName, int iOpenMode, size_t szOffset, size_t szMapSize )
{
	try
	{
		CloseMap();
		using namespace boost::interprocess;
		m_pImp = new CFileMapImp(pFileName,  (0 == iOpenMode) ? read_write : read_only);

		return m_pImp->OpenMapping(szOffset, szMapSize);
	}
	catch (...)
	{
		return FILEMAP_EXEC_FAILED;
	}
}

int CFileMapping::CloseMap()
{
	if (NULL != m_pImp)
	{
		m_pImp->Flush();
		delete m_pImp;
		m_pImp = NULL;
	}
	return FILEMAP_EXEC_SUCCESS;
}

void * CFileMapping::GetAddress()
{
	return m_pImp->GetAddress();
}

int CFileMapping::CreateMapping( const char *pFileName, int iOpenMode, size_t szOffset, size_t szMapSize )
{
	using namespace boost::interprocess;
	using namespace std;
	try
	{
		file_mapping::remove(pFileName);
		std::locale::global(std::locale(""));
		fstream fs;
		fs.open(pFileName, ios::in | ios::out | ios::trunc | ios::binary);
		if (fs.is_open())
		{			
			fs.rdbuf()->pubseekoff(szOffset + szMapSize - 1, ios::beg);
			fs.rdbuf()->sputc(0);
			fs.close();			
			return OpenMapping(pFileName, iOpenMode, szOffset, szMapSize);
		}
		else
		{
			return FILEMAP_EXEC_FAILED;
		}
	}
	catch (...)
	{
		return FILEMAP_EXEC_FAILED;
	}		
}

size_t CFileMapping::GetSize()
{
	return m_pImp->GetSize();
}

bool CFileMapping::RemoveFile( const char *fileName )
{
	using namespace boost::interprocess;
	return file_mapping::remove(fileName);
}
