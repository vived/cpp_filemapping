#pragma once
/*
	FileMapping.h 内存文件映射头文件
	介绍：内存映射相关接口，可根据需求进行相关扩展
	版本：1.0
	作者：zhangfang 20140220
*/


#ifdef FILE_MAPPING_CPP
#define  DLLIMPORTOREXPORT	__declspec(dllexport)
#else
#define  DLLIMPORTOREXPORT	__declspec(dllimport)
#endif // FILE_MAPPING_CPP

const int MODE_READ_WRITE	=	0;
const int MODE_READ_ONLY	=	1;

const int FILEMAP_EXEC_SUCCESS	=	0;
const int FILEMAP_EXEC_FAILED	=	1;

class CFileMapImp;
class DLLIMPORTOREXPORT CFileMapping
{
public:
	CFileMapping();
	~CFileMapping();
public:
	/*创建新文件，并映射，iOpentMode 为MODE_READ_WRITE或Mode_Read_only
		创建szOffset+szMapSize大小的映射文件
	*/
	int CreateMapping(const char *pFileName, int iOpenMode, size_t szOffset, size_t szMapSize);
	/*打开已在在的文件，并映射，szOffSet为从某个位置开始，映射szMapSize大小的字节
		注意：如果szOffset或者szMapSize输入错误(如越界)，则可能导致未知错误
	*/
	int OpenMapping(const char *pFileName, int iOpenMode, size_t szOffset, size_t szMapSize);
	/*可手动关闭映射*/
	int CloseMap();
	/*获取映射大小*/
	size_t GetSize();
	/*返回映射的地址*/
	void *GetAddress();
	bool RemoveFile(const char *fileName);
private:
	CFileMapImp *m_pImp;
};