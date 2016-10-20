#include "stdafx.h"

#include "../FileMapping/FileMapping.h"

#include <windows.h>
#include <process.h>
using namespace std;


#if 0

TEST(FileMapping, CreateMapSuccess)
{
	CFileMapping FileMap;
	ASSERT_EQ(FILEMAP_EXEC_SUCCESS, FileMap.CreateMapping("E:\\File.txt", MODE_READ_WRITE,0, 100));
}
 
 unsigned __stdcall OpenMapThread_FirstMap(void* pArguments)
 {
 
 	CFileMapping FileMap;
 	FileMap.OpenMapping("E:\\File.txt", MODE_READ_WRITE, 2, 3);
 	memcpy(FileMap.GetAddress(),"123",FileMap.GetSize());
 
 	FileMap.CloseMap();
 	return 0;
 
 };
 
 unsigned __stdcall OpenMapThread_SecMap(void* pArguments)
 {
 	CFileMapping FileMap;
 	FileMap.OpenMapping("E:\\File.txt", MODE_READ_WRITE, 8, 3);
 	memcpy(FileMap.GetAddress(),"456",FileMap.GetSize());
 	FileMap.CloseMap();
 	return 0;
 
 };
 
 unsigned __stdcall OpenMapThread_ThirdMap(void* pArguments)
 {
 	CFileMapping FileMap;
 	FileMap.OpenMapping("E:\\File.txt", MODE_READ_WRITE, 15, 3);
 	memcpy(FileMap.GetAddress(),"789",FileMap.GetSize());
 	FileMap.CloseMap();
 	return 0;
 
 };
 
 
 
 TEST(FileMapping, OpenMap)
 {
 	HANDLE hThread1;
 	HANDLE hThread2;
 	HANDLE hThread3;
 	unsigned threadID1;
 	unsigned threadID2;
 	unsigned threadID3;
 	hThread1 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapThread_FirstMap, NULL, 0, &threadID1 );
 
 	hThread2 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapThread_SecMap, NULL, 0, &threadID2 );
 
 	hThread3 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapThread_ThirdMap, NULL, 0, &threadID3 );
 	WaitForSingleObject(hThread1, INFINITE);
 	WaitForSingleObject(hThread2, INFINITE);
 	WaitForSingleObject(hThread3, INFINITE);
 }

//期望123
 TEST(FileMapping, OpenMap_FirstMap)
 {
 	CFileMapping FileMap;
 	FileMap.OpenMapping("E:\\File.txt",MODE_READ_ONLY, 2,3);
 	char pBuffer[10] = {0};
 	memcpy(pBuffer, FileMap.GetAddress(), FileMap.GetSize());
 	ASSERT_EQ(0, strcmp(pBuffer, "123"));
 }
 //期望456
 TEST(FileMapping, OpenMap_SecMap)
 {
 	CFileMapping FileMap;
 	FileMap.OpenMapping("E:\\File.txt",MODE_READ_ONLY, 8,3);
 	char pBuffer[10] = {0};
 	memcpy(pBuffer, FileMap.GetAddress(), FileMap.GetSize());
 	ASSERT_EQ(0, strcmp(pBuffer, "456"));
 }
 
 //期望789
 TEST(FileMapping, OpenMap_ThirdMap)
 {
 	CFileMapping FileMap;
 	FileMap.OpenMapping("E:\\File.txt",MODE_READ_ONLY, 15,3);
 	char pBuffer[10] = {0};
 	memcpy(pBuffer, FileMap.GetAddress(), FileMap.GetSize());
 	ASSERT_EQ(0, strcmp(pBuffer, "789"));
 }

//OpenMaping szOffset和szMapSize越界
TEST(FileMapping, OpenMapFailed)
{
	CFileMapping FileMap;
	EXPECT_NE(FILEMAP_EXEC_SUCCESS, FileMap.OpenMapping("E:\\File.txt", MODE_READ_WRITE, 99,10 ));
	memcpy(FileMap.GetAddress(),"321",FileMap.GetSize());
	FileMap.CloseMap();
}
//创建相同名字的内存映射文件"E:\\File.txt"，重新写入数据，查看最后的数据是什么
TEST(FileMapping, CreateSameMap)
{
	CFileMapping FileMap;
	//EXPECT_EQ(FILEMAP_EXEC_SUCCESS,FileMap.CreateMapping("E:\\File.txt", MODE_READ_WRITE,0, 100) );
	FileMap.CreateMapping("E:\\File.txt", MODE_READ_WRITE,0, 100);
	FileMap.OpenMapping("E:\\File.txt", MODE_READ_WRITE, 2 ,3);
	memcpy(FileMap.GetAddress(),"abc",FileMap.GetSize());
	FileMap.CloseMap();

	FileMap.OpenMapping("E:\\File.txt", MODE_READ_ONLY, 2 ,3);

	char pBuffer[10] = {0};
	memcpy(pBuffer, (char *)FileMap.GetAddress(),FileMap.GetSize());
	ASSERT_EQ(0, strcmp(pBuffer, "abc"));

	FileMap.CloseMap();
}


unsigned __stdcall OpenMapSameSpace_FirstMap(void* pArguments)
{
	CFileMapping FileMap;
	FileMap.OpenMapping("E:\\File1.txt", MODE_READ_WRITE, 2, 3);
	memcpy(FileMap.GetAddress(),"123",FileMap.GetSize());
	FileMap.CloseMap();
	return 0;
};

unsigned __stdcall OpenMapSameSpace_SecMap(void* pArguments)
{
	CFileMapping FileMap;
	FileMap.OpenMapping("E:\\File1.txt", MODE_READ_WRITE, 2, 3);
	memcpy(FileMap.GetAddress(),"456",FileMap.GetSize());
	FileMap.CloseMap();
	return 0;
};


unsigned __stdcall OpenMapSameSpace_ThirdMap(void* pArguments)
{
	CFileMapping FileMap;
	FileMap.OpenMapping("E:\\File1.txt", MODE_READ_WRITE, 2, 3);
	memcpy(FileMap.GetAddress(),"789",FileMap.GetSize());

	FileMap.CloseMap();
	return 0;
};
unsigned __stdcall OpenMapSameSpace_FourMap(void* pArguments)
{
	CFileMapping FileMap;
	FileMap.OpenMapping("E:\\File1.txt", MODE_READ_WRITE, 2, 3);
	memcpy(FileMap.GetAddress(),"abc",FileMap.GetSize());

	FileMap.CloseMap();
	return 0;
};
unsigned __stdcall OpenMapSameSpace_FiveMap(void* pArguments)
{
	CFileMapping FileMap;
	FileMap.OpenMapping("E:\\File1.txt", MODE_READ_WRITE, 2, 3);
	memcpy(FileMap.GetAddress(),"def",FileMap.GetSize());

	FileMap.CloseMap();
	return 0;
};
unsigned __stdcall OpenMapSameSpace_SixMap(void* pArguments)
{
	CFileMapping FileMap;
	FileMap.OpenMapping("E:\\File1.txt", MODE_READ_WRITE, 2, 3);
	memcpy(FileMap.GetAddress(),"ghi",FileMap.GetSize());

	FileMap.CloseMap();
	return 0;
};
//开启六个线程，写入同一个地址
TEST(FileMapping, OpenMapSameSpace)
{
	CFileMapping FileMap;
	FileMap.CreateMapping("E:\\File1.txt", MODE_READ_WRITE, 0 , 100);
	HANDLE hThread4;
	HANDLE hThread5;
	HANDLE hThread6;
	HANDLE hThread7;
	HANDLE hThread8;
	HANDLE hThread9;
	unsigned threadID4;
	unsigned threadID5;
	unsigned threadID6;
	unsigned threadID7;
	unsigned threadID8;
	unsigned threadID9;

	hThread4 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapSameSpace_FirstMap, NULL, 0, &threadID4 );
	
	hThread5 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapSameSpace_SecMap, NULL, 0, &threadID5 );

	hThread6 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapSameSpace_ThirdMap, NULL, 0, &threadID6 );
	
	hThread7 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapSameSpace_FourMap, NULL, 0, &threadID7 );

	hThread8 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapSameSpace_FiveMap, NULL, 0, &threadID8 );
	
	hThread9 = (HANDLE)_beginthreadex( NULL, 0, &OpenMapSameSpace_SixMap, NULL, 0, &threadID9 );

	WaitForSingleObject(hThread4, INFINITE);
	WaitForSingleObject(hThread5, INFINITE);
	WaitForSingleObject(hThread6, INFINITE);
	WaitForSingleObject(hThread7, INFINITE);
	WaitForSingleObject(hThread8, INFINITE);
	WaitForSingleObject(hThread9, INFINITE);

	FileMap.CloseMap();

	FileMap.OpenMapping("E:\\File1.txt", MODE_READ_ONLY, 2, 3);

	char pBuffer[10] = {0};
	BOOL Comp;

	memcpy(pBuffer, FileMap.GetAddress(),FileMap.GetSize());
	Comp = strcmp(pBuffer, "123") && strcmp(pBuffer, "456") && strcmp(pBuffer, "789") && strcmp(pBuffer, "abc") && strcmp(pBuffer, "def") && strcmp(pBuffer, "ghi");

	EXPECT_EQ(0, Comp);

	FileMap.CloseMap();
}

TEST(FileMapping, RemoveFile)
{
	CFileMapping FileMap;
	FileMap.CreateMapping("E:\\RemoveFile.txt", MODE_READ_WRITE,0, 100);
	ASSERT_TRUE(FileMap.RemoveFile("E:\\RemoveFile.txt"));
}
#endif
