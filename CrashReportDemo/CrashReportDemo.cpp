// WANGZhan -> wangzhan.1985@gmail.com
#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>


// NOTE: ��Releaseģʽ�²��ԣ�������������־
// C:\Users\James\AppData\Local\Temp\crashdump_1_15432_20352.dmp


void DumpException(PEXCEPTION_POINTERS pExceptionPointers);

static void myParameterHandler(const wchar_t *a1, const wchar_t *a2, const wchar_t *a3, unsigned int a4, uintptr_t a5)
{
	printf("myParameterHandler\n");

	// ���������쳣����myExceptionFilter����
	throw 1;
}

static void myPurecallHandler()
{
	printf("myPurecallHandler\n");

	// ���������쳣����myExceptionFilter����
	throw 2;
}

static LONG WINAPI myExceptionFilter(PEXCEPTION_POINTERS ExceptionInfo)
{
	printf("myExceptionFilter\n");

	DumpException(ExceptionInfo);

	::TerminateProcess(::GetCurrentProcess(), 100);
	return EXCEPTION_EXECUTE_HANDLER;
}


class CDerived;
class CBase
{
public:
	CBase(CDerived *pDerived) : m_pDerived(pDerived) {}
	virtual ~CBase();
	virtual void function() = 0;

private:
	CDerived *m_pDerived;
};

class CDerived : public CBase
{
public:
	CDerived() : CBase(this) {}
	virtual void function() {}
};

CBase::~CBase()
{
	m_pDerived->function();
}

// ������Ч���� myParameterHandler -> myExceptionFilter
static void Test1()
{
	// ����δ�����쳣��������
	SetUnhandledExceptionFilter(myExceptionFilter);

	// ������Ч������������
	_set_invalid_parameter_handler(myParameterHandler);

	printf(NULL);
}

// ���Դ��麯�� myPurecallHandler -> myExceptionFilter
static void Test2()
{
	// ����δ�����쳣��������
	SetUnhandledExceptionFilter(myExceptionFilter);

	// ���ô��麯�����ô�������
	_set_purecall_handler(myPurecallHandler);

	CDerived myDerived;
}

void xxx()
{
	char *p = NULL;
	*p = 0;
}

// ����δ�����쳣 -> myExceptionFilter
static void Test3()
{
	// ����δ�����쳣��������
	SetUnhandledExceptionFilter(myExceptionFilter);

	xxx();
}


static void SetExceptionsHandler()
{
	// ����δ�����쳣��������
	SetUnhandledExceptionFilter(myExceptionFilter);

	// ������Ч������������
	_set_invalid_parameter_handler(myParameterHandler);

	// ���ô��麯�����ô�������
	_set_purecall_handler(myPurecallHandler);
}



static int g_iExceptionHandlerType = 1;

static void DumpException(PEXCEPTION_POINTERS pExceptionPointers)
{
	// �����澡������ϵͳ���á�crt֮��Ķ�������ֹ��������Ϊ�п����ڷ�������ʱ����Щ��Դ���ڱ�ռ�ã������ٴ�����ʱ�ͻᱻ��ס
	DWORD dwPID = ::GetCurrentProcessId();
	DWORD dwTID = ::GetCurrentThreadId();

	wchar_t szModule[1024] = { 0 };
	::GetModuleFileName(NULL, szModule, _countof(szModule));

	// ��ȡĿ¼
	wchar_t* p = wcsrchr(szModule, '\\');
	if (!p)
		return;

	*p = 0;

	wchar_t szModulePath[1024] = { 0 };
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi = { 0 };
	wsprintfW(szModulePath, L"\"%s\\CrashReporter.exe\" %d %d %d %d", szModule, g_iExceptionHandlerType,
		dwPID, dwTID, pExceptionPointers);

	// TODO: ���ڵ���
	//wprintf(szModulePath);
	//Sleep(50 * 60 * 1000);

	if (::CreateProcess(NULL, szModulePath, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}


int _tmain(int argc, _TCHAR* argv[])
{
	Test1();
	//Test2();
	//Test3();
	
	return 0;
}

