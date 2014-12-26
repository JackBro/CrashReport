// WANGZhan -> wangzhan.1985@gmail.com
#pragma once
#include <windows.h>
#include <iostream>


// Handle ������
class CHandleHelper
{
public:
	CHandleHelper() : m_hHandle(NULL) {}
	CHandleHelper(HANDLE hHandle) : m_hHandle(hHandle) {}

	~CHandleHelper()
	{
		if (m_hHandle != NULL && m_hHandle != INVALID_HANDLE_VALUE)
			CloseHandle(m_hHandle);
	}

	operator HANDLE& ()
	{
		return m_hHandle;
	}

private:
	HANDLE m_hHandle;
};


namespace crashdump
{

	struct CrashDumpData
	{
		std::wstring strDumpPath;

		DWORD dwPID;
		DWORD dwTID;
		EXCEPTION_POINTERS *pExceptionPointer;
	};


	// ���ʱ����̵��쳣�ڴ�
	bool GenerateDumpFile(CrashDumpData *pCrashDumpData);

	// ֱ�ӷ��ʱ������̵��쳣�ڴ棨�漰����̣�
	bool GenerateDumpFile2(CrashDumpData *pCrashDumpData);

}
