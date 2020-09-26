#pragma once

#include "define.h"
#include "patch.h"

extern HWND g_hWnd;
extern KPatcher* g_patcher;// PatcherLib
extern HANDLE      g_hMutex;    ///< Mutex of game client program.
extern std::ofstream       g_logStream;        ///< Log file stream.
extern LPSTR				g_lpCmdLine;
extern float g_fTotalDownloadRatio;
extern float g_fFileDownloadRatio;
extern std::string g_CurFilename;

class CMainDlg;
class CPatchReadyDlg;

struct ThreadParam
{
	std::string strPatchAddr;
	std::string strPatchPath;
	std::string strZoneAddr;
	std::string strZonePort;
	CMainDlg* pMainDlg;
	CPatchReadyDlg* pPatchReadyDlg;
	//HANDLE hThread;
};

/// �α����Ͽ� �α� �޼����� �����.
/// @param szLogMsg �α� �޼���
/// @return void
void Log( const char* szLogMsg );

/// �α����Ͽ� �α� ���� �޼����� �����.
/// @param szLogMsg �α� �޼���
/// @return void
void LogError( const char* szLogMsg );

void ThreadReady(HWND hWnd);
void SetFileDownloadRatio(float fRatio);
float GetFileDownloadRatio();
void SetTotalDownloadRatio(float fRatio);
float GetTotalDownloadRatio();
bool TotalProgress(float p);
bool FileProgress(float p);

//DWORD WINAPI ThreadProc_1( LPVOID lpParameter );
DWORD WINAPI ThreadProc( LPVOID lpParameter );
