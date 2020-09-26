// file   : KPCHWebBrowser.h
// since  : 2005-06-02 ���� 3:36:40
// author : pch413@kogstudios.com
// This requires IE 5.0 ( or better ).

// Park Chang-hyeon's web browser class header
#pragma once

// Parkch's �������� �ʱ�ȭ �Լ�
BOOL InitPCHWebBrowser( void );

// Parkch's �������� �ı� �Լ�
BOOL UnInitPCHWebBrowser( void );

// Parkch's �������� ��Ű ���� �Լ�
void SetPCHWebBrowserCookie( const std::string& strID, const std::string& strNickName, const std::string& strEtc );

class KPCHWebBrowser
{
public:
    // Constructor
    KPCHWebBrowser( void );
    // Destructor
    ~KPCHWebBrowser( void );

    // Methods
    BOOL Create( DWORD dwStyle, const RECT& rc, HWND hWndParent, UINT id );
    void Destroy( void );
	BOOL Navigate( const char* szURL, const char* strPostString_ );
	void OnKeyboardEvents( LPMSG pMsg );

private:
    HWND    m_hWrapperWnd;
    HWND    m_hBrowserWnd;
};