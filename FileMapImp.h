#pragma once

#include "boost/interprocess/file_mapping.hpp"
#include "boost/interprocess/mapped_region.hpp"
#include "FileMapping.h"

class CFileMapImp
{
public:
	CFileMapImp(const char *fileName, boost::interprocess::mode_t mode);
	~CFileMapImp(void);
	int OpenMapping(size_t szOffset, size_t szMapSize);
	void *GetAddress();
	int Flush(size_t szOffSetStart = 0, size_t szNumsToFlush = 0, bool bAsync = true);
	size_t GetSize();
private:
	int UnMap();
private:
	boost::interprocess::file_mapping m_fMap;
	boost::interprocess::mapped_region *m_pMapRegion;
};
