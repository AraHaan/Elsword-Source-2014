#pragma once

class CKTDGUIDialogManager : public CKTDXStage
{
	public:
		CKTDGUIDialogManager(void);
		~CKTDGUIDialogManager(void);

#ifdef DIALOG_SHOW_TOGGLE
		bool	GetShow() { return m_bShow; }
		void	ToggleShow() { SetShow(!m_bShow);}
#endif
		void	SetShow( bool bShow ) { m_bShow = bShow; }

		void	RegisterLuaBind();

        bool	AddDlg( CKTDGUIDialogType pDialog );
		//bool	AddDlg( CKTDGUIDialogHandle hDialog_ );
//{{ robobeg : 2008-01-07
        bool    UnmanageDlg( CKTDGUIDialogType pDialog );
//}} robobeg : 2008-01-07

        bool	DeleteDlg( CKTDGUIDialogType pDialog );
		//{{ seojt // 2009-8-18, 18:00
		//bool	DeleteDlg( CKTDGUIDialogHandle hDialog_ );
		//}} seojt // 2009-8-18, 18:00
//{{ robobeg : 2008-01-07
		//bool	DeleteDlgByName( const WCHAR* pName );
//}} robobeg : 2008-01-07
		void	ClearAllDlg();
		bool	ClearAllOKAndCancelMsgDlg();
		
//{{ robobeg : 2009-01-07
		//std::vector<CKTDGUIDialogType>& GetDialogList() { return m_vecDialog; }
//}} robobeg : 2009-01-07

		bool	CheckDlg( CKTDGUIDialogType pDialog );
		bool	CheckModalDlg();
		bool	CheckFrontModalDlg( CKTDGUIDialogType pPivotDialog ); //�ش� ���̾�α� �տ� ��� ���̾�αװ� �ִ��� Ȯ��
		CKTDGUIDialogType GetFirstFrontModalDlg();		//���� �Ǿտ��� ��� ������ �ϴ� ���̾�α� ��������
		bool	CheckMostFrontDlg( CKTDGUIDialogType pDialog ); //�� ���̾�αװ� �� �տ� �������� ���̾�α����� Ȯ��		

		void	CountModalDialog( CKTDGUIDialog* pDialog = NULL );


		bool	ChangeLayer( CKTDGUIDialogType pDialog, int layerToChange );
        bool    ChangeFront( CKTDGUIDialogType pDialog, bool bFront ); ///< front, rear ����
        bool	ChangeSequence( CKTDGUIDialogType pDialog, bool bTop ); //�����ִ� ���̾�� ���� ������ �ű���̳� ���� �ڷ� �ű���̳�?
		//{{ seojt // 2009-8-18, 18:12, qff
		//bool	ChangeSequence( CKTDGUIDialogHandle hDialog_, bool bTop ); //�����ִ� ���̾�� ���� ������ �ű���̳� ���� �ڷ� �ű���̳�?
		//}} seojt // 2009-8-18, 18:12
		bool	ChangeSequence( CKTDGUIDialogType pDialog, CKTDGUIDialogType pPivotDialog, bool bOver, bool bChangeLayer = false );

		CKTDGUIDialog* GetDialog_LUA( const char* pName ); 
		CKTDGUIDialogType GetDialog( const WCHAR* pName );

#ifdef PRIORITY_EVERY_UI
		bool ExistDialogAtMousePos();
#endif PRIORITY_EVERY_UI


		virtual	HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual	HRESULT OnFrameRender( bool bFront = false );

		virtual HRESULT OnFrameRenderTopModal();

		virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual	HRESULT OnResetDevice();
		virtual	HRESULT OnLostDevice();

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		void	SetFont( int uiFontID, CKTDGFontManager::CKTDGFont* pFont );
		CKTDGFontManager::CKTDGFont* GetFont( int fontID ){ return m_mapFontList[fontID]; }
#endif

		void	SetUKFont( int uiUKFontID, CKTDGFontManager::CUKFont* pFont );
		CKTDGFontManager::CUKFont* GetUKFont( int fontID ){ return m_mapUKFontList[fontID]; }

#ifdef  KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		void    MapFontToUKFont( int fontID, int uiUKFontID )
		{
			if ( m_mapUKFontList.find( uiUKFontID ) != m_mapUKFontList.end() )
			{
				m_mapFontToUKFont[ fontID ] = uiUKFontID;
			}//if
		}

		CKTDGFontManager::CUKFont* GetMappedUKFont( int fontID )
		{
			std::map< int, int >::const_iterator iter = m_mapFontToUKFont.find( fontID );
			if ( iter == m_mapFontToUKFont.end() )
				return  NULL;
			return  GetUKFont( iter->second );
		}
#endif

//{{ robobeg : 2009-01-07
		//CKTDGUIDialogType GetDialogByKeyID( int iKeyID );
//}} robobeg : 2009-01-07
		
		bool IsDraggingDialog();

		CKTDGUIDialogType GetCloseDialogByESC();

		void SetStage( CKTDXStage* pStage );

		bool GetFocusInEditBox();	// imeedit or editbox�� ��Ŀ���� �ִ��� �˻��Ѵ�.

#ifdef DIALOG_SHOW_TOGGLE
		bool GetHideDialog();
		void SetCanHide(bool bVal) { m_bCanHide = bVal; }
#endif

private:

        void    _InsertIntoDialogListTop( CKTDGUIDialogType pkInDialog_ );
        void    _InsertIntoDialogListBottom( CKTDGUIDialogType pkInDialog_ );
        bool    _RemoveFromDialogList( CKTDGUIDialogType pkInDialog_ );

		inline void	IncreaseDialogListChangeCount()
		{
			m_iDialogListChangeCount += 1;
			if( m_iDialogListChangeCount > INT_MAX - 10 )
			{
				m_iDialogListChangeCount = 1;
			}
		}

private:

		bool										m_bShow;

//{{ robobeg : 2009-01-07
		CKTDGUIDialogMap							m_mapDialog;			// dialog name���� dialog handle�� �˻��� �� ���
		CKTDGUIDialogSet							m_setDialog;			// dialog handle�� dialog�� �ִ��� ������ �˻��� �� ���
        CKTDGUIDialogList                           m_listDialog;			// dialog�� ���������� traverse�� �� ���
        std::vector<CKTDGUIDialogList::iterator>    m_vecIterator;
        std::vector<CKTDGUIDialogList::iterator>    m_vecReverseIterator;
//}} robobeg : 2009-01-07


#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		map< int, CKTDGFontManager::CKTDGFont* >	m_mapFontList;
#endif  KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT

		map< int, CKTDGFontManager::CUKFont* >		m_mapUKFontList;

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		std::map< int, int >                        m_mapFontToUKFont;
#endif


		int m_iModalDialogCount;		// ���� dialog list�� ���� �Ǿ��ִ� modal dialog ����
		int m_iDialogListChangeCount;	// dialog�� dialog list�� �߰��ɶ�, list�� ����� ȸ���� ����. dialog list�� �߰��� ���Ŀ� dialog list�� ������ �־����� Ȯ���ϱ� ���� ���.
		std::map< std::wstring, int > m_mapDialogListChangeCountWhenLastGetDialog; // < name, dialog_list_change_count> // ���������� GetDialog(name) �Լ��� ȣ�������� dialog list change count

#ifdef DIALOG_SHOW_TOGGLE
		bool m_bCanHide;
#endif
};
