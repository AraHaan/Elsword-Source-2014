// X2EncryptFileViewer.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "X2EncryptFileViewer.h"
#include "MainFrm.h"

#include "X2EncryptFileViewerDoc.h"
#include "X2EncryptFileViewerView.h"
#include <fstream>
#include "FileCrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


lua_State* g_pLua	= NULL;

// CX2EncryptFileViewerApp

BEGIN_MESSAGE_MAP(CX2EncryptFileViewerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CX2EncryptFileViewerApp ����

CX2EncryptFileViewerApp::CX2EncryptFileViewerApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CX2EncryptFileViewerApp ��ü�Դϴ�.

CX2EncryptFileViewerApp theApp;

// CX2EncryptFileViewerApp �ʱ�ȭ

BOOL CX2EncryptFileViewerApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.
	InitCommonControls();

	CWinApp::InitInstance();

	//lua init
	{
		g_pLua = lua_open();

		if( g_pLua == NULL )
		{
			::MessageBoxW( NULL, L"��� ���� ����.!", L"", MB_OK );
			return FALSE;
		}

		luaopen_base( g_pLua );  // ����Լ� tonumber�� ����ؾ� �Ѵ�.
		luaopen_table( g_pLua );
		luaopen_io( g_pLua );
		luaopen_string( g_pLua );
		luaopen_math( g_pLua );
		lua_settop( g_pLua, 0 );

		lua_tinker::lua_state::open( g_pLua );
	}

	// Laod Config
	LoadConfig();	
	
	// lua close
	lua_close( g_pLua );




	//{{ dmlee 2008.05.26 command line param�� ������ ��ȣȭ�� ������ �Ϲ� �ؽ�Ʈ�� ��ȯ�ؼ� ���
	LPSTR* szArglist = NULL;
	int nArgs = 0;

	szArglist = CommandLineToArgvA( (PCHAR) GetCommandLine(), &nArgs );
	if( NULL != szArglist && nArgs == 3 )
	{
		for( int i=0; i<nArgs; i++) 
		{
			printf( "%s", szArglist[i] );
		}

		std::string srcPath = szArglist[1];
		std::string destPath = szArglist[2];

		// Free memory allocated for CommandLineToArgvW arguments.
		LocalFree(szArglist);


		char* cBuffer = NULL;

		std::ifstream ifs;
		ifs.open( srcPath.c_str(), std::ios::binary );
		if( ifs.is_open() == false )
		{
			return FALSE;
		}

		std::string strBuf;
		char cReadCur;
		while( !ifs.eof() )
		{
			ifs.get( cReadCur );
			strBuf.append( 1, cReadCur );
		}	

		if( cBuffer != NULL )
			delete[] cBuffer;

		cBuffer = CFileCrypt::FileDecrypt( theApp.GetEncryptionKeyList(), strBuf.c_str(), (int)strBuf.size() );

		std::ofstream ofs;	
		ofs.open( destPath.c_str(), std::ios::binary );
		if( ofs.is_open() == true )
		{
			if( NULL != cBuffer )
			{
				ofs.write( cBuffer, (std::streamsize)strlen( cBuffer ) );
			}
			ofs.close();
		}

		if( cBuffer != NULL )
			delete[] cBuffer;


		// exit the application
		return FALSE;
	}
	//}} dmlee 2008.05.26 command line param�� ������ ��ȣȭ�� ������ �Ϲ� �ؽ�Ʈ�� ��ȯ�ؼ� ���




	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.
	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	// ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CX2EncryptFileViewerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CX2EncryptFileViewerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ����ٿ� ������ ����� ����ġ�մϴ�. ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	// SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.









	return TRUE;
}



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDecryptSave();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_COMMAND(ID_DECRYPT_SAVE, OnDecryptSave)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CX2EncryptFileViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CX2EncryptFileViewerApp �޽��� ó����

void CX2EncryptFileViewerApp::LoadConfig()
{
	lua_tinker::class_<CX2EncryptFileViewerApp>("CX2EncryptFileViewerApp")		
		.def("AddEncryptionKey",			CX2EncryptFileViewerApp::AddEncryptionKey_Lua)
		;

	lua_tinker::decl( "CX2EncryptFileViewerApp", this );

	char acCurPath[_MAX_PATH] = {0};
	GetCurrentDirectoryA(_MAX_PATH, acCurPath);
	std::string strPath = acCurPath;
	strPath += "\\";
	strPath += "Config.lua";

	if( 0 != lua_dofile( g_pLua, strPath.c_str() ) )
	{
		::MessageBoxW( NULL, L"Config.lua open failed!", L"", MB_OK );
	}	
}

bool CX2EncryptFileViewerApp::AddEncryptionKey_Lua()
{
	KLuaManager luaManager( g_pLua );

	int iEncryptionKey;

	LUA_GET_VALUE_RETURN(	luaManager,	"m_iEncryptionKey",		iEncryptionKey,		0,	goto LoadFail; );

	m_vecEncryptionKey.push_back( iEncryptionKey );

	return true;

LoadFail:
	::MessageBoxW( NULL, L"AddEncryptionKey �Ľ� ����", L"", MB_OK );

	return false;
}
void CAboutDlg::OnDecryptSave()
{
	MessageBox( "" );
}







PCHAR* CX2EncryptFileViewerApp::CommandLineToArgvA( PCHAR CmdLine, int* _argc )
{
	PCHAR* argv;
	PCHAR  _argv;
	ULONG   len;
	ULONG   argc;
	CHAR   a;
	ULONG   i, j;

	BOOLEAN  in_QM;
	BOOLEAN  in_TEXT;
	BOOLEAN  in_SPACE;

	len = (ULONG)strlen(CmdLine);
	i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

	argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
		i + (len+2)*sizeof(CHAR));

	_argv = (PCHAR)(((PUCHAR)argv)+i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

	while( a = CmdLine[i] ) {
		if(in_QM) {
			if(a == '\"') {
				in_QM = FALSE;
			} else {
				_argv[j] = a;
				j++;
			}
		} else {
			switch(a) {
case '\"':
	in_QM = TRUE;
	in_TEXT = TRUE;
	if(in_SPACE) {
		argv[argc] = _argv+j;
		argc++;
	}
	in_SPACE = FALSE;
	break;
case ' ':
case '\t':
case '\n':
case '\r':
	if(in_TEXT) {
		_argv[j] = '\0';
		j++;
	}
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	break;
default:
	in_TEXT = TRUE;
	if(in_SPACE) {
		argv[argc] = _argv+j;
		argc++;
	}
	_argv[j] = a;
	j++;
	in_SPACE = FALSE;
	break;
			}
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = NULL;

	(*_argc) = argc;
	return argv;
}