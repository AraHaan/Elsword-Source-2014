// ���� ����Ʈ:
// http://vbdream.tistory.com/entry/Kernel-DLL-��������-ȿ��������-�����ϴ�-���-Caller-Verification

#include "stdafx.h"
#include "X2DLLManager.h"

//{{ �ڱ��� : [2010/05/05]		 //	��Ƽ�� �ڵ�
#ifdef DLL_MANAGER

#include <stdio.h>
#include <windows.h>
#include <winnt.h>
#include <algorithm>

#include <boost/tokenizer.hpp>
#include <boost/bind.hpp>

ImplementSingleton( CX2DLLManager );

typedef LONG NTSTATUS;
#define FASTCALL __fastcall
#define NT_SUCCESS(Status) (((NTSTATUS)Status) >= 0)
#ifndef NTAPI
#define NTAPI WINAPI
#endif
#ifndef UNICODE_STRING
typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;
#endif

typedef HMODULE (WINAPI *LOADLIBRARYA)(LPCSTR lpFileName);
typedef HMODULE (WINAPI *LOADLIBRARYW)(LPCWSTR lpFileName);
typedef HMODULE (WINAPI *LOADLIBRARYEXA)(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags);
typedef HMODULE (WINAPI *LOADLIBRARYEXW)(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags);
typedef NTSTATUS (NTAPI *LDRLOADDLL)(PWCHAR, ULONG, PUNICODE_STRING, PHANDLE);
#define CALCULATE_JUMP_OFFSET(_BaseOffset, _RelativeOffset)		\
								( (LONG)(_BaseOffset) + (LONG)(_RelativeOffset) + 5	)
#define MAKE_JUMP_OFFSET(_BaseOffset, _TargetOffset)			\
								( (LONG)(_TargetOffset) - (LONG)(_BaseOffset) - 5 )

LOADLIBRARYA _LoadLibraryA, _tLoadLibraryA;
LOADLIBRARYW _LoadLibraryW, _tLoadLibraryW;
LOADLIBRARYEXA _LoadLibraryExA, _tLoadLibraryExA;
LOADLIBRARYEXW _LoadLibraryExW, _tLoadLibraryExW;
LDRLOADDLL _LdrLoadDll, _tLdrLoadDll;
DWORD dwUser32Start, dwUser32End;

//---------------------------------------------------------------------------------------------
BOOL WINAPI CheckCallerAddress(IN const wstring& wstrFilename, IN PVOID Caller)
{
	ELSWORD_VIRTUALIZER_START

	if(SiCX2DLLManager()->IsVerifiedDLL(wstrFilename))
		return TRUE;

#ifdef APPLY_THEMIDA
	ENCODE_START
#endif

	BOOL bVerify = TRUE;

	// �� �����忡�� ȣ��Ǿ����� Ȯ��
	if( Caller == NULL )
	{
		// New Thread DLL ������
		CX2DLLManager::DLLEvent e(	CX2DLLManager::DLLEvent::DET_DLL_INJECTION, 
									CX2DLLManager::DLLEvent::HMT_NEWTHEREADDLL, 
									wstrFilename,
									Caller );
		SiCX2DLLManager()->ProcessEvent(e);

		bVerify = FALSE;
		//return FALSE;
	}
	// Ŀ�� ��� �޸�?
	else if( (DWORD)Caller >= (DWORD)0x80000000L )
	{
		CX2DLLManager::DLLEvent e(	CX2DLLManager::DLLEvent::DET_DLL_INJECTION, 
									CX2DLLManager::DLLEvent::HMT_KERNELMODEMEMORY, 
									wstrFilename,
									Caller );
		SiCX2DLLManager()->ProcessEvent(e);

		bVerify = FALSE;
		//return FALSE;
	}
	// �������� �޸�?
	else if(IsBadCodePtr( (FARPROC)Caller ))
	{
		CX2DLLManager::DLLEvent e(	CX2DLLManager::DLLEvent::DET_DLL_INJECTION, 
									CX2DLLManager::DLLEvent::HMT_BADCODEPTR, 
									wstrFilename,
									Caller );
		SiCX2DLLManager()->ProcessEvent(e);

		bVerify = FALSE;
		//return FALSE;
	}
	else
	{
		MEMORY_BASIC_INFORMATION mbi;
		VirtualQuery(Caller, &mbi, sizeof(MEMORY_BASIC_INFORMATION));

		// DLL Injection - SetThreadContext
		// Private Memory���� ȣ��Ǿ���
		if(mbi.Type == MEM_PRIVATE)
		{
			CX2DLLManager::DLLEvent e(	CX2DLLManager::DLLEvent::DET_DLL_INJECTION, 
										CX2DLLManager::DLLEvent::HMT_SETTHREADCONTEXT,
										wstrFilename,
										Caller );
			SiCX2DLLManager()->ProcessEvent(e);

			bVerify = FALSE;
			//return FALSE;
		}
		else
		{
			// DLL Injection - CreateRemoteThread
			// Ȥ�� Caller�� BaseThreadStartThunk ���� �����߳�?
			LPBYTE pbCaller = (LPBYTE) Caller;
			if(*pbCaller == (BYTE)0x50 && 
				/*pbCaller[1] == (BYTE)0xE8 && */
				CALCULATE_JUMP_OFFSET(pbCaller + 1, *(PLONG)(pbCaller + 2)) == (LONG)ExitThread)
			{
				CX2DLLManager::DLLEvent e(	CX2DLLManager::DLLEvent::DET_DLL_INJECTION, 
											CX2DLLManager::DLLEvent::HMT_CREATEREMOTETHREAD,
											wstrFilename,
											Caller );
				SiCX2DLLManager()->ProcessEvent(e);

				bVerify = FALSE;
				//return FALSE;
			}

			// DLL Injection - SetWindowsHookEx
			// Ȥ�� Caller�� ClientLoadLibrary ���� �����߳�?
			if(((DWORD)pbCaller >= dwUser32Start && (DWORD)pbCaller < dwUser32End) ||
				*(pbCaller-7) == (BYTE)0x1C)
			{
				CX2DLLManager::DLLEvent e(	CX2DLLManager::DLLEvent::DET_DLL_INJECTION, 
											CX2DLLManager::DLLEvent::HMT_SETWINDOWHOOKEX,
											wstrFilename,
											Caller );
				SiCX2DLLManager()->ProcessEvent(e);

				bVerify = FALSE;
				//return FALSE;
			}
		}
	}


	// NOTE: �ѱ������� ���� �ڵ忡�� ���������� ���� ��η� DLL injection�� �õ��Ǿ������� ���� �˻� ���δ� �����ϰ� 
	//       �Ʒ��� �ڵ忡�� �ٽ� �˻縦 �Ѵ�. ���� �ڵ�� �˻����� ��쿡 ���ҵ尡 ������� �ʴ� ��찡 �־��� �����̶�� �Ѵ�. 
	
	


	// NOTE: input locale�� ��ŷ�� �Ǵ��ϴ� ��ƾ�� winmain()���� ����ϰ� ������ ����� ����� ������ 
	//       �߱��� input locale ����� �߿��� dll injection�� �ϴ� ������ ��ŷ���� �Ǵ��ϱ� ���ؼ��̴�. 
	
#ifdef APPLY_PROTECT_DLL
	if( g_pMain == NULL || g_pMain->GetSystemInfo() == NULL )
	{
		bVerify = TRUE;
	}
	else 
	{		
		bool bCheck = true;

		if( g_pMain != NULL && g_pMain->GetSystemInfo() != NULL && g_pMain->GetSystemInfo()->GetSystemPrimaryLocaleID() != LANG_CHINESE )
		{
			bCheck = false;
		}

		UINT uContryCode = GetACP();		
		switch(uContryCode)
		{
		case 936:
		case 950:
		case 20000:
		case 20002:
		case 20936:
		case 50227:
		case 50229:
		case 50935:
		case 50936:
		case 54936:
			bCheck = true;
			break;
		}

#ifdef SYS_INFO

		//{{ �ڱ��� : [2010/06/07]	//	�Է� �����Ͽ� ���� �߱� ���� ���� ó��
#ifdef ADD_INFO_INPUT_LOCALE
		vector<LANGID> vecInputLocaleID;
		CX2SystemInfo::QueryInputLocale(vecInputLocaleID);
		BOOST_TEST_FOREACH( LANGID, lid, vecInputLocaleID)
		{
			if(PRIMARYLANGID(lid) == LANG_CHINESE)
				bCheck = true;
		}
#endif // ADD_INFO_INPUT_LOCALE
		//}}

		//{{ �ڱ��� : [2010/06/07]	//	�ν��� �����Ͽ� ���� �߱� ���� ���� ó��
#ifdef ADD_INFO_INSTALL_LOCALE
		LANGID InstallLocale = PRIMARYLANGID( CX2SystemInfo::QueryInstallLocaleID() );
		switch( InstallLocale )
		{
			// �������� �ʴ� �����쿡 ���ؼ��� �����Ѵ�.
		case LANG_CHINESE:
			{
				// ������ �� ���� ������!!!
				//MessageBox(NULL, L"�������� �ʴ� �������Դϴ�.", L"Error!", MB_OK);
				bCheck = true;
			}
			break;
		default:		
			break;
		} 
#endif // ADD_INFO_INSTALL_LOCALE

#endif // SYS_INFO

		if( bCheck == false )
		{
			bVerify = TRUE;
		}
	}

#else //APPLY_PROTECT_DLL

#ifdef _SERVICE_
	bVerify = TRUE;
#endif

#endif //APPLY_PROTECT_DLL

#ifdef APPLY_THEMIDA
	ENCODE_END
#endif

	return bVerify;


	ELSWORD_VIRTUALIZER_END
}

BOOL WINAPI CheckCallerAddressA(IN PVOID lpFileName, IN PVOID Caller)
{
	wstring wstrFileName;
	ConvertCharToWCHAR(wstrFileName, (LPCSTR)lpFileName);

#ifdef APPLY_THEMIDA
	ENCODE_START
#endif

	BOOL bVerify = CheckCallerAddress(wstrFileName, Caller);


	if(bVerify)
	{
		CX2DLLManager::DLLEvent e(	CX2DLLManager::DLLEvent::DET_DLL_LOAD, 
									CX2DLLManager::DLLEvent::HMT_UNKNOWN, 
									wstrFileName,
									Caller );
		SiCX2DLLManager()->ProcessEvent(e);
	}

#ifdef APPLY_THEMIDA
	ENCODE_END
#endif

	return bVerify;
}

BOOL WINAPI CheckCallerAddressW(IN PVOID lpFileName, IN PVOID Caller)
{
	wstring wstrFileName((LPCWSTR)lpFileName);

#ifdef APPLY_THEMIDA
	ENCODE_START
#endif

	BOOL bVerify = CheckCallerAddress(wstrFileName, Caller);


	if(bVerify)
	{
		CX2DLLManager::DLLEvent e(	CX2DLLManager::DLLEvent::DET_DLL_LOAD, 
									CX2DLLManager::DLLEvent::HMT_UNKNOWN, 
									wstrFileName,
									Caller );
		SiCX2DLLManager()->ProcessEvent(e);
	}

#ifdef APPLY_THEMIDA
	ENCODE_END
#endif

	return bVerify;
}

__declspec(naked) HMODULE WINAPI _nLoadLibraryA(LPCSTR lpFileName)
{		
	__asm
	{
		PUSH EBP
		MOV EBP, ESP
		XOR EAX, EAX
//		JNZ $+30000

		PUSHAD						// �������� �� ����
		PUSH DWORD PTR [ESP+0x24]	// ȣ���� �ּ� push
		PUSH DWORD PTR [EBP+0x08]	// DLL ���ϸ� push
		CALL CheckCallerAddressA	// ȣ���� ����

//#ifdef _IN_HOUSE_
		// �系������ DLL Injection ����� �ε� ����
		TEST EAX, EAX				// ��ȯ���� FALSE �ΰ�?
		JZ _NotVerifiedCaller		// �߸��� ȣ��
//#endif _IN_HOUSE_

		// �ùٸ� ȣ��
		POPAD
		POP EBP
		JMP _tLoadLibraryA

//#ifdef _IN_HOUSE_
_NotVerifiedCaller:
		POPAD
		POP EBP
		XOR EAX, EAX				// EAX = 0
		RETN 0x4					// ����
//#endif _IN_HOUSE_
	}
}

__declspec(naked) HMODULE WINAPI _nLoadLibraryW(LPCWSTR lpFileName)
{
	__asm
	{
		PUSH EBP
		MOV EBP, ESP
		XOR EAX, EAX
//		JNZ $+30000

		PUSHAD						// �������� �� ����
		PUSH DWORD PTR [ESP+0x24]	// ȣ���� �ּ� push
		PUSH DWORD PTR [EBP+0x08]	// DLL ���ϸ� push
		CALL CheckCallerAddressW	// ȣ���� ����

//#ifdef _IN_HOUSE_
		// �系������ DLL Injection ����� �ε� ����
		TEST EAX, EAX				// ��ȯ���� FALSE �ΰ�?
		JZ _NotVerifiedCaller		// �߸��� ȣ��
//#endif _IN_HOUSE_

		// �ùٸ� ȣ��
		POPAD
		POP EBP
		JMP _tLoadLibraryW

//#ifdef _IN_HOUSE_
_NotVerifiedCaller:
		POPAD
		POP EBP
		XOR EAX, EAX				  // EAX = 0
		RETN 0x4					  // ����
//#endif _IN_HOUSE_
	}
}

__declspec(naked) HMODULE WINAPI _nLoadLibraryExA(LPCSTR lpFileName, HANDLE hFile, DWORD dwFlags)
{
	__asm
	{
		PUSH EBP
		MOV EBP, ESP
		XOR EAX, EAX
//		JNZ $+30000

		PUSHAD						// �������� �� ����
		PUSH DWORD PTR [ESP+0x24]	// ȣ���� �ּ� push
		PUSH DWORD PTR [EBP+0x08]	// DLL ���ϸ� push
		CALL CheckCallerAddressA	// ȣ���� ����

//#ifdef _IN_HOUSE_
		// �系������ DLL Injection ����� �ε� ����
		TEST EAX, EAX				// ��ȯ���� FALSE �ΰ�?
		JZ _NotVerifiedCaller		// �߸��� ȣ��
//#endif _IN_HOUSE_

		// �ùٸ� ȣ��
		POPAD
		POP EBP
		JMP _tLoadLibraryExA

//#ifdef _IN_HOUSE_
_NotVerifiedCaller:
		POPAD
		POP EBP
		XOR EAX, EAX				  // EAX = 0
		RETN 0xC					  // ����
//#endif _IN_HOUSE_
	}
}

__declspec(naked) HMODULE WINAPI _nLoadLibraryExW(LPCWSTR lpFileName, HANDLE hFile, DWORD dwFlags)
{
	__asm
	{
		PUSH EBP
		MOV EBP, ESP
		XOR EAX, EAX
//		JNZ $+30000

		PUSHAD						// �������� �� ����
		PUSH DWORD PTR [ESP+0x24]	// ȣ���� �ּ� push
		PUSH DWORD PTR [EBP+0x08]	// DLL ���ϸ� push
		CALL CheckCallerAddressW	// ȣ���� ����

//#ifdef _IN_HOUSE_
		// �系������ DLL Injection ����� �ε� ����
		TEST EAX, EAX				// ��ȯ���� FALSE �ΰ�?
		JZ _NotVerifiedCaller		// �߸��� ȣ��
//#endif _IN_HOUSE_

		// �ùٸ� ȣ��
		POPAD
		POP EBP
		JMP _tLoadLibraryExW

//#ifdef _IN_HOUSE_
_NotVerifiedCaller:
		POPAD
		POP EBP
		XOR EAX, EAX				// EAX = 0
		RETN 0xC					// ����
//#endif _IN_HOUSE_
	}
}

__declspec(naked) NTSTATUS NTAPI _nLdrLoadDll(
	IN PWCHAR               PathToFile OPTIONAL,
	IN ULONG                Flags OPTIONAL,
	IN PUNICODE_STRING      ModuleFileName,
	OUT PHANDLE             ModuleHandle )
{
	__asm
	{
		PUSH EBP
		MOV EBP, ESP
		XOR EAX, EAX
//		JNZ $+30000

		PUSHAD									// �������� �� ����
		PUSH DWORD PTR [ESP+0x24]				// ȣ���� �ּ� push]	
		MOV EBX, DWORD PTR [ModuleFileName]		// DLL ���ϸ� push
		MOV ECX, DWORD PTR [EBX+0x04]
		PUSH ECX
		CALL CheckCallerAddressW				// ȣ���� ����

//#ifdef _IN_HOUSE_
		// �系������ DLL Injection ����� �ε� ����
		TEST EAX, EAX							// ��ȯ���� FALSE �ΰ�?
		JZ _NotVerifiedCaller					// �߸��� ȣ��
//#endif _IN_HOUSE_

		// �ùٸ� ȣ��
		POPAD
		POP EBP
		JMP _tLdrLoadDll

//#ifdef _IN_HOUSE_
_NotVerifiedCaller:
		POPAD
		POP EBP
		XOR EAX, EAX							// EAX = 0
		RETN 0x10								// ����
//#endif _IN_HOUSE_
	}
}

//---------------------------------------------------------------------------------------------
bool CX2DLLManager::HookInjectionApi( LPBYTE TargetFunction, LPBYTE Trampoline, LPVOID DetouredFunction, DWORD MinimumSize )
{
	// XP������ ������ �����
	// �ٸ� OS���� �׽�Ʈ�� �ʿ���
	if(m_bHook)
		return false;

	DWORD OldProtect;
	BOOL result = VirtualProtect( TargetFunction-5, 5+MinimumSize, PAGE_EXECUTE_READWRITE, &OldProtect );
	if(result)
	{
		if(MinimumSize == 2)
		{
			LPBYTE JmpOffset = TargetFunction - 5;
			*JmpOffset = (BYTE)0xE9; // JMP LONG
			*(PLONG)(JmpOffset + 1) = MAKE_JUMP_OFFSET(JmpOffset, DetouredFunction);
			RtlMoveMemory(Trampoline, TargetFunction, MinimumSize);

			JmpOffset = Trampoline + MinimumSize;
			*JmpOffset = (BYTE)0xE9;
			*(PLONG)(JmpOffset + 1) = MAKE_JUMP_OFFSET(JmpOffset, (LPBYTE)TargetFunction + MinimumSize);

			*TargetFunction = (BYTE)0xEB; // JMP SHORT
			TargetFunction[1] = (BYTE)0xF9;
		}
		else if(MinimumSize == 5)
		{
			RtlMoveMemory(Trampoline, TargetFunction, MinimumSize);

			LPBYTE JmpOffset = Trampoline + MinimumSize;
			*JmpOffset = (BYTE)0xE9;
			*(PLONG)(JmpOffset + 1) = MAKE_JUMP_OFFSET(JmpOffset, (LPBYTE)TargetFunction + MinimumSize);

			JmpOffset = TargetFunction;
			*JmpOffset = (BYTE)0xE9; // JMP LONG
			*(PLONG)(JmpOffset + 1) = MAKE_JUMP_OFFSET(JmpOffset, DetouredFunction);
		}

		VirtualProtect( TargetFunction-5, 5+MinimumSize, OldProtect, &OldProtect );

#ifdef _DEBUG
		OutputDebugString(boost::str(boost::wformat(L"DLL-Manager: %1% ��ŷ ����\n") % TargetFunction).c_str());
#endif

		m_bHook = true;
		return true;
	}
	else
	{
		m_bHook = false;
		return false;
	}
}

void CX2DLLManager::UnHookInjectionApi( LPBYTE TargetFunction, LPBYTE Trampoline, DWORD MinimumSize )
{
	if(!m_bHook)
		return;

	DWORD OldProtect;
	VirtualProtect( TargetFunction, MinimumSize, PAGE_EXECUTE_READWRITE, &OldProtect );
	RtlMoveMemory( TargetFunction, Trampoline, MinimumSize );
	VirtualProtect( TargetFunction, MinimumSize, OldProtect, &OldProtect );

#ifdef _DEBUG
	OutputDebugString(boost::str(boost::wformat(L"DLL-Manager: %1% ��ŷ ����\n") % TargetFunction).c_str());
#endif
}

//---------------------------------------------------------------------------------------------
CX2DLLManager::DLLEvent::DLLEvent(DLL_EVENT_TYPE EventType, INJECTION_METHOD_TYPE MethodType, const wstring& FileName, PVOID pCaller)
: m_EventType(EventType)
, m_MethodType(MethodType)
, m_DLLFileName(FileName)
, m_pCaller(pCaller)
{
}

bool CX2DLLManager::DLLEvent::operator==(const DLLEvent& event) const
{
	if(	this->GetDLLFileName() == event.GetDLLFileName() &&
		this->GetEventType() == event.GetEventType() && 
		this->GetMethodType() == event.GetMethodType() &&
		this->GetCallerAddr() == event.GetCallerAddr() )
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------------------------
CX2DLLManager::CX2DLLManager()
: m_bHook(false)
#ifdef CHECK_FROM_DLLMANAGER
, m_nOldDLLEventCount(0)
#endif
{
	
}

CX2DLLManager::~CX2DLLManager()
{
	Final();
}

bool CX2DLLManager::Init()
{
	bool result = true;

	m_PreLoadedDllLIst = "\n";

#ifdef MORE_INFO_ABOUT_HACK	
	// DLL ����

	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, GetCurrentProcessId() );
	if (NULL == hProcess)
	{
		m_PreLoadedDllLIst += "Cannot access to process.\r\n";
		return result;
	}

	HMODULE hMods[1024];
	DWORD cbNeeded;	
	if(EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		int numProcess = cbNeeded / sizeof(HMODULE);
		for (int i = 0; i < numProcess; i++ )
		{
			char szModName[MAX_PATH];

			// Get the full path to the module's file.
			if (!GetModuleFileNameExA( hProcess, hMods[i], szModName, ARRAY_SIZE(szModName) ))
			{
				strncpy(szModName, "[Hidden]" ,MAX_PATH-1);
				//strcpy(szModName, "[Hidden]");
			}

			m_PreLoadedDllLIst += ( boost::str(boost::format("[0x%08X] %s \n") % hMods[i] % szModName ) );
		}
	}

	CloseHandle( hProcess );

	m_PreLoadedDllLIst += "\n\n";

	//if( g_pMain != NULL )
	//	g_pMain->AddDLLInfo(m_PreLoadedDllLIst);
#endif // MORE_INFO_ABOUT_HACK

	LoadVerifiedDLLLists();

	result &= CallerVerificationStart();

	return result;
}

void CX2DLLManager::Final()
{
	CallerVerificationStop();
	RemoveAllListener();
}

#ifdef FORCE_FREE_MODULE
void CX2DLLManager::ForceFree()
{
#if defined( _SERVICE_ ) 	
	ELSWORD_VIRTUALIZER_START
#endif

//	THEMIDA_VM_START

#ifdef _IN_HOUSE_
	FILE *fp = fopen("CheckModuleList.txt", "w");
#endif

	// �������� �ε�� dll���� �̾Ƴ���.
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, GetCurrentProcessId() );
	if ( hProcess != NULL )
	{
		HMODULE hMods[1024];
		DWORD cbNeeded;	
		if(EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
			int numProcess = cbNeeded / sizeof(HMODULE);
			for (int i = 0; i < numProcess; i++ )
			{
				WCHAR szModName[MAX_PATH];

				// Get the full path to the module's file.
				GetModuleFileNameExW( hProcess, hMods[i], szModName, ARRAY_SIZE( szModName ) );

				
				
				WCHAR wstrFileName[MAX_PATH], wstrFileExt[256];
				_wsplitpath( szModName, NULL, NULL, wstrFileName, wstrFileExt );
				StringCchCatW( wstrFileName, MAX_PATH, wstrFileExt) ;
				//StrCatW( wstrFileName, wstrFileExt) ;

				wstring wstrModuleName = wstrFileName;
				wstring wstrFileExtBuf = wstrFileExt;

				MakeUpperCase(wstrModuleName);				
				MakeUpperCase(wstrFileExtBuf);

#ifdef _IN_HOUSE_				
				if( fp != NULL )
					fwprintf(fp, L"\n%s ", wstrModuleName.c_str());
#endif
				// �����׸�
				if( wstrFileExtBuf == L".TMP" || StrStrW(wstrFileExtBuf.c_str(), L"TMP") )
					continue;
				if( wstrFileExtBuf == L".IME" || StrStrW(wstrFileExtBuf.c_str(), L"IME") )
					continue;
				if( wstrFileExtBuf == L".DRV" || StrStrW(wstrFileExtBuf.c_str(), L"DRV") )
					continue;
				if( wstrModuleName == L"X2.EXE" )
					continue;

				// ���� dll�׸�				
				if( m_vecExcludedDlls.find( wstrModuleName ) != m_vecExcludedDlls.end() )
					continue;

				// �׿� ����� ������ ������Ų��. (�ش� �����嵵 �����ų��?)
#ifdef _IN_HOUSE_				
				if( fp != NULL )
					fwprintf(fp, L" (FreeLibrary)" );
#endif				

				// ���� ������ �ƴϰ� Ȥ�ø� Ŭ���̾�Ʈ ���Ḧ ���� free�Ǵ� ��� �α׸� �����.
				ErrorLogMsg(KEM_ERROR423, wstrModuleName.c_str());

				FreeLibrary(hMods[i]);
			}
		}

		CloseHandle( hProcess );	
	}

#ifdef _IN_HOUSE_
	fclose(fp);
#endif

#ifdef _IN_HOUSE_
	FILE *fpAfter = fopen("AfterFreeLoadedModule.txt", "w");

	// �������� �ε�� dll���� �̾Ƴ���.
	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, GetCurrentProcessId() );
	if ( hProcess != NULL )
	{
		HMODULE hMods[1024];
		DWORD cbNeeded;	
		if(EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
			int numProcess = cbNeeded / sizeof(HMODULE);
			for (int i = 0; i < numProcess; i++ )
			{
				WCHAR szModName[MAX_PATH];

				// Get the full path to the module's file.
				GetModuleFileNameExW( hProcess, hMods[i], szModName, ARRAY_SIZE( szModName ) );

				WCHAR wstrFileName[MAX_PATH], wstrFileExt[256];
				_wsplitpath( szModName, NULL, NULL, wstrFileName, wstrFileExt );
				StringCchCatW( wstrFileName, MAX_PATH, wstrFileExt) ;
				//StrCatW( wstrFileName, wstrFileExt) ;

				wstring wstrModuleName = wstrFileName;
				wstring wstrFileExtBuf = wstrFileExt;

				MakeUpperCase(wstrModuleName);				

				fwprintf(fpAfter, L"%s\n", wstrModuleName.c_str());
			}
		}

		CloseHandle( hProcess );	
	}
	fclose(fpAfter);
#endif


//	THEMIDA_VM_END
#if defined( _SERVICE_ ) 	
	ELSWORD_VIRTUALIZER_END
#endif	
}
#endif


std::wstring trim(const std::wstring& s, const std::wstring& drop = L" \a\b\f\n\r\t\v")
{
	std::wstring strTemp(s);
	std::wstring r = strTemp.erase(strTemp.find_last_not_of(drop)+1);
	return r.erase(0,r.find_first_not_of(drop));
}

void CX2DLLManager::LoadVerifiedDLLLists()
{
	// Ȯ�ε� DLL ����Ʈ �ε�
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( L"GameHelper.ini" );		// DLL ������� �̸��� �������� ����Ͽ� ����
	if( Info == NULL )
	{
//		ErrorLogMsg( XEM_ERROR64, "GameHelper.ini ���� �ҷ����� ����" );
		MessageBox( g_pKTDXApp->GetHWND(), L"GameHelper.ini �ҷ����� ����", L"GameHelper.ini", MB_OK );
		return;
	}

	wstring DLLlist;
	ConvertUtf8ToWCHAR(DLLlist, Info->pRealData, Info->size);
	MakeUpperCase(DLLlist);
#ifdef _DEBUG
	wstringstream stream;
	stream << L"������ DLL ���: ";
#endif

	typedef boost::tokenizer<boost::char_separator<wchar_t, wstring::traits_type>, wstring::const_iterator, wstring > tokenizer;
//	boost::char_separator<wchar_t, wstring::traits_type> sep(L"\n");
	boost::char_separator<wchar_t, wstring::traits_type> sep(L"\n", L"\n", boost::keep_empty_tokens);
	tokenizer tok(DLLlist, sep);
	for (tokenizer::iterator iter = tok.begin(); iter != tok.end(); ++iter)
	{
		wstring DLLName = trim(*iter);
		if(DLLName.length() != 0 && DLLName != L"\n")
		{
			m_vecExcludedDlls.insert(DLLName);

#ifdef _DEBUG
			stream << DLLName << ", ";
#endif
		}
	}

#ifdef _DEBUG
	stream << std::endl;
	OutputDebugString(stream.str().c_str());
#endif
}

//---------------------------------------------------------------------------------------------
void CX2DLLManager::AddListener(DLLEventListener* pListener)
{
	m_vecDLLEventListeners.push_back(pListener);
}

void CX2DLLManager::RemoveListener(DLLEventListener* pListener)
{
	m_vecDLLEventListeners.erase( std::remove(m_vecDLLEventListeners.begin(), m_vecDLLEventListeners.end(), pListener), m_vecDLLEventListeners.end() );
}

void CX2DLLManager::RemoveAllListener()
{
	m_vecDLLEventListeners.clear();
}

//---------------------------------------------------------------------------------------------
void CX2DLLManager::ProcessEvent(const CX2DLLManager::DLLEvent& event)
{
	switch(event.GetEventType())
	{
	case DLLEvent::DET_DLL_LOAD:
		{
			BOOST_TEST_FOREACH( DLLEventListener*, pListener, m_vecDLLEventListeners )
			{
				pListener->OnLoadDLLEvent(event);
			}
			break;
		}
	case DLLEvent::DET_DLL_INJECTION:
		{
			// DLL Injection
			BOOST_TEST_FOREACH( DLLEventListener*, pListener, m_vecDLLEventListeners )
			{
				pListener->OnInverifiedDLLEvent(event);
			}

			break;
		}
	default:
		{
			// ���� ó��
		}
	}

	// �̺�Ʈ ���
	vector<DLLEvent>::const_iterator iter = find_if(m_vecDLLEvent.begin(), m_vecDLLEvent.end(), 
													boost::bind(std::equal_to<DLLEvent>(), event, _1) );
	if(iter == m_vecDLLEvent.end())
		m_vecDLLEvent.push_back(event);
}

//---------------------------------------------------------------------------------------------
bool CX2DLLManager::IsVerifiedDLL(const wstring& wstrFileName) const
{
	wstring FileName(wstrFileName);
	MakeUpperCase(FileName);
	LPCWSTR pFileName = PathFindFileNameW(FileName.c_str());
	int pos = pFileName - FileName.c_str();

	if(m_vecExcludedDlls.end() != m_vecExcludedDlls.find(FileName.substr(pos)))
		return true;

	return false;
}

//---------------------------------------------------------------------------------------------
bool CX2DLLManager::CallerVerificationStart()
{
#ifdef REMOVE_HOOKING
	return false;
#endif //REMOVE_HOOKING

	HMODULE hKernel32 = GetModuleHandle( L"kernel32" );
	HMODULE hNtDLL = GetModuleHandle( L"ntdll" );
	HMODULE hUser32 = GetModuleHandle( L"user32" );

	if(!hKernel32 || !hNtDLL) return false;
	if(!hUser32)
	{
		hUser32 = LoadLibrary( L"user32" );
		if(!hUser32) return false;
	}

	if(!(_LoadLibraryA = (LOADLIBRARYA) GetProcAddress(hKernel32, "LoadLibraryA"))) return false;
	if(!(_LoadLibraryW = (LOADLIBRARYW) GetProcAddress(hKernel32, "LoadLibraryW"))) return false;
	if(!(_LoadLibraryExA = (LOADLIBRARYEXA) GetProcAddress(hKernel32, "LoadLibraryExA"))) return false;
	if(!(_LoadLibraryExW = (LOADLIBRARYEXW) GetProcAddress(hKernel32, "LoadLibraryExW"))) return false;
	if(!(_LdrLoadDll = (LDRLOADDLL) GetProcAddress(hNtDLL, "LdrLoadDll"))) return false;

	if(!(_tLoadLibraryA = (LOADLIBRARYA) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return false;
	if(!(_tLoadLibraryW = (LOADLIBRARYW) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return false;
	if(!(_tLoadLibraryExA = (LOADLIBRARYEXA) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return false;
	if(!(_tLoadLibraryExW = (LOADLIBRARYEXW) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return false;
	if(!(_tLdrLoadDll = (LDRLOADDLL) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return false;

	// USER32.DLL ���� ����
	dwUser32Start = (DWORD)hUser32;

	PIMAGE_DOS_HEADER idh = (PIMAGE_DOS_HEADER)hUser32;
	if(idh->e_magic != IMAGE_DOS_SIGNATURE) 
		return false;	// IMAGE_DOS_SIGNATURE üũ
	
	PIMAGE_NT_HEADERS inh = reinterpret_cast<PIMAGE_NT_HEADERS>(dwUser32Start +	idh->e_lfanew);
	if(inh->Signature != IMAGE_NT_SIGNATURE) 
		return false;	// IMAGE_NT_SIGNATURE üũ

	dwUser32End ^= (dwUser32Start ^ dwUser32End);
	dwUser32End += inh->OptionalHeader.SizeOfImage;

	// API Hooking Start~!

	//				  | Initial Opcode Len
	// -------------------------------------
	// LoadLibraryA	  |	  5 Bytes
	// LoadLibraryW	  |	  5 Bytes
	// LoadLibraryExA |	  5 Bytes
	// LoadLibraryExW |	  2 Bytes
	// LdrLoadDll	  |	  5 Bytes

	HookInjectionApi( (LPBYTE)_LoadLibraryA,	(LPBYTE)_tLoadLibraryA,		(LPVOID)_nLoadLibraryA,		5 );
	HookInjectionApi( (LPBYTE)_LoadLibraryW,	(LPBYTE)_tLoadLibraryW,		(LPVOID)_nLoadLibraryW,		5 );
	HookInjectionApi( (LPBYTE)_LoadLibraryExA,	(LPBYTE)_tLoadLibraryExA,	(LPVOID)_nLoadLibraryExA,	5 );
	HookInjectionApi( (LPBYTE)_LoadLibraryExW,	(LPBYTE)_tLoadLibraryExW,	(LPVOID)_nLoadLibraryExW,	2 );
	HookInjectionApi( (LPBYTE)_LdrLoadDll,		(LPBYTE)_tLdrLoadDll,		(LPVOID)_nLdrLoadDll,		5 );

	return true;
}

void CX2DLLManager::CallerVerificationStop()
{
#ifdef REMOVE_HOOKING
	return;
#endif //REMOVE_HOOKING

	UnHookInjectionApi( (LPBYTE)_LoadLibraryA,		(LPBYTE)_tLoadLibraryA,		5 );
	VirtualFree( _tLoadLibraryA,	0,	MEM_RELEASE );

	UnHookInjectionApi( (LPBYTE)_LoadLibraryW,		(LPBYTE)_tLoadLibraryW,		5 );
	VirtualFree( _tLoadLibraryW,	0,	MEM_RELEASE );

	UnHookInjectionApi( (LPBYTE)_LoadLibraryExA,	(LPBYTE)_tLoadLibraryExA,	5 );
	VirtualFree( _tLoadLibraryExA,	0,	MEM_RELEASE );

	UnHookInjectionApi( (LPBYTE)_LoadLibraryExW,	 (LPBYTE)_tLoadLibraryExW,	2 );
	VirtualFree( _tLoadLibraryExW,	0,	MEM_RELEASE );

	UnHookInjectionApi( (LPBYTE)_LdrLoadDll,		(LPBYTE)_tLdrLoadDll,		5 );
	VirtualFree( _tLdrLoadDll,		0,	MEM_RELEASE );
}

//---------------------------------------------------------------------------------------------
class IsInverifiedEvent
{
public:
	bool operator()(CX2DLLManager::DLLEvent& event)
	{
		if( event.GetEventType() == CX2DLLManager::DLLEvent::DET_DLL_INJECTION)
			return true;
		else
			return false;
	}
};

void CX2DLLManager::ReportEvent()
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif


//	THEMIDA_VM_START

	vector<DLLEvent>::const_iterator iter = find_if(m_vecDLLEvent.begin(), m_vecDLLEvent.end(), 
													IsInverifiedEvent() );
	if(iter == m_vecDLLEvent.end())
		return;

	wstringstream wstrmEvent;
	wstrmEvent << L"- DLL Injection -" << std::endl;

	boost::wformat fmt(L"MType:%2%, CAddr:%3%, [%1%]");
	BOOST_TEST_FOREACH(const DLLEvent&, event, m_vecDLLEvent)
	{
		if(event.GetEventType() != DLLEvent::DET_DLL_INJECTION)
			continue;

		wstrmEvent	<< ( fmt	% event.GetDLLFileName() 
								% event.GetMethodType()
								% event.GetCallerAddr() )
					<< "\n";
	}

	wstrmEvent << std::endl;
	wstrmEvent << L"- DLL Load -" << std::endl;
	BOOST_TEST_FOREACH(const DLLEvent&, event, m_vecDLLEvent)
	{
		if(event.GetEventType() == DLLEvent::DET_DLL_INJECTION)
			continue;

		wstrmEvent	<< boost::str( fmt	% event.GetDLLFileName() 
										% event.GetMethodType()
										% event.GetCallerAddr() )
					<< "\n";
	}



#ifdef CHECK_FROM_DLLMANAGER
	int nInjectionCount = m_vecDLLEvent.size();
	if( m_nOldDLLEventCount == nInjectionCount )
	{
		return;
	}
	m_nOldDLLEventCount = nInjectionCount;
#endif

#if 0 // ���� ��� �뵵�� �����Ƿ� �����Ѵ�.
	string message;
	ConvertWCHARToChar(message, wstrmEvent.str());
	g_pMain->SendHackMail_DLLInjection(message.c_str());
#endif

	// ������� �ʵ��� �ּ� ó��
//	m_pKTDXApp->SetFindHacking( true );


//	THEMIDA_VM_END

#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_END
#endif
}

wstring CX2DLLManager::GetReportEvent()
{
#if defined( _SERVICE_ ) 
	ELSWORD_VIRTUALIZER_START
#endif

	vector<DLLEvent>::const_iterator iter = find_if(m_vecDLLEvent.begin(), m_vecDLLEvent.end(), 
		IsInverifiedEvent() );
	if(iter == m_vecDLLEvent.end())
		return L"";

	wstringstream wstrmEvent;
	wstrmEvent << L"- DLL Injection -" << std::endl;

	boost::wformat fmt(L"MType:%2%, CAddr:%3%, [%1%]");
	BOOST_TEST_FOREACH(const DLLEvent&, event, m_vecDLLEvent)
	{
		if(event.GetEventType() != DLLEvent::DET_DLL_INJECTION)
			continue;

		wstrmEvent	<< ( fmt	% event.GetDLLFileName() 
			% event.GetMethodType()
			% event.GetCallerAddr() )
			<< "\n";
	}

	wstrmEvent << std::endl;
	wstrmEvent << L"- DLL Load -" << std::endl;
	BOOST_TEST_FOREACH(const DLLEvent&, event, m_vecDLLEvent)
	{
		if(event.GetEventType() == DLLEvent::DET_DLL_INJECTION)
			continue;

		wstrmEvent	<< boost::str( fmt	% event.GetDLLFileName() 
			% event.GetMethodType()
			% event.GetCallerAddr() )
			<< "\n";
	}

	//string message;
	//ConvertWCHARToChar(message, wstrmEvent.str());
	//g_pMain->SendHackMail_DLLInjection(message.c_str());
	// ������� �ʵ��� �ּ� ó��
	//	m_pKTDXApp->SetFindHacking( true );

	return  wstrmEvent.str();

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif
}

#ifdef CHECK_FROM_DLLMANAGER
bool CX2DLLManager::FindModule(const wstring &wstrModuleName)
{
	BOOST_TEST_FOREACH(const DLLEvent&, event, m_vecDLLEvent)
	{
		WCHAR wstrFileName[MAX_PATH], wstrFileExt[256];
		_wsplitpath(event.GetDLLFileName().c_str(), NULL, NULL, wstrFileName, wstrFileExt);

		StringCchCatW(wstrFileName,MAX_PATH, wstrFileExt);
		//StrCatW(wstrFileName, wstrFileExt);

		//if( StrStrW(event.GetDLLFileName().c_str(), wstrModuleName.c_str()) )
		if( StrCmpW(wstrModuleName.c_str(), wstrFileName ) == 0 )
		{
			return true;
		}
	}

	return false;
}
bool CX2DLLManager::FindModuleIn(const wstring &wstrModuleName)
{
	BOOST_TEST_FOREACH(const DLLEvent&, event, m_vecDLLEvent)
	{
		WCHAR wstrFileName[MAX_PATH], wstrFileExt[256];
		_wsplitpath(event.GetDLLFileName().c_str(), NULL, NULL, wstrFileName, wstrFileExt);

		StringCchCatW(wstrFileName,MAX_PATH, wstrFileExt);
		//StrCatW(wstrFileName, wstrFileExt);

		wstring wstrDllName = event.GetDLLFileName();
		MakeUpperCase( wstrDllName );

		if( StrStrW(wstrDllName.c_str(), wstrModuleName.c_str()) )
		//if( StrCmpW(wstrModuleName.c_str(), wstrFileName ) == 0 )
		{
			return true;
		}
	}

	return false;
}
#endif

#ifdef FREE_MODULE
void CX2DLLManager::ForceFreeModule(wstring wstrFreeModuleName)
{
#if defined( _SERVICE_ ) 	
	ELSWORD_VIRTUALIZER_START
#endif

		//	THEMIDA_VM_START


	// �������� �ε�� dll���� �̾Ƴ���.
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE, GetCurrentProcessId() );
	if ( hProcess != NULL )
	{
		HMODULE hMods[1024];
		DWORD cbNeeded;	
		if(EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
			int numProcess = cbNeeded / sizeof(HMODULE);
			for (int i = 0; i < numProcess; i++ )
			{
				WCHAR szModName[MAX_PATH];

				// Get the full path to the module's file.
				GetModuleFileNameExW( hProcess, hMods[i], szModName, ARRAY_SIZE( szModName ) );

				WCHAR wstrFileName[MAX_PATH], wstrFileExt[256];
				_wsplitpath( szModName, NULL, NULL, wstrFileName, wstrFileExt );
				StringCchCatW( wstrFileName, MAX_PATH, wstrFileExt) ;
				//StrCatW( wstrFileName, wstrFileExt) ;

				wstring wstrModuleName = wstrFileName;
				wstring wstrFileExtBuf = wstrFileExt;

				MakeUpperCase(wstrModuleName);

				if( wstrModuleName != wstrFreeModuleName )
					continue;
				
				// ���� ������ �ƴϰ� Ȥ�ø� Ŭ���̾�Ʈ ���Ḧ ���� free�Ǵ� ��� �α׸� �����.
				ErrorLogMsg(KEM_ERROR423, wstrModuleName.c_str());

				FreeLibrary(hMods[i]);
			}
		}

		CloseHandle( hProcess );	
	}
	
	//	THEMIDA_VM_END
#if defined( _SERVICE_ ) 	
	ELSWORD_VIRTUALIZER_END
#endif	
}
#endif //FREE_MODULE

#endif //DLL_MANAGER

//}} �ڱ��� : [2010/05/05]