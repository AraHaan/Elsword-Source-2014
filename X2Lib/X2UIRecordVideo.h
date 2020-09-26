//{{ 2011.9.16 ����ȣ �ݵ�ķ �۾�
#ifdef	BANDICAM_RECORDING

#pragma once


// namespace		_CONST_UIRANKING_INFO_
// {
// 	const	int		g_iNumRankingPerPage		= 5;		// �� ������ �� ��Ÿ�� �� �ִ� ��ŷ ����
// 	const	int		g_iLimitRankingToDisplay	= 2000;		// �� ���� ������ �˼����� ���� ǥ��
// 	const	int		g_iRankLimit				= 3;		// ������ 3�ڸ� ������
// 	const	int		g_iMaxPageLimit				= 20;
// }

class CX2UIRecordVideo
{
public:

	enum UI_RECORD_VIDEO_CUSTOM_MSG
	{
		URVCM_EXIT				= 	52000,
		URVCM_RECORD_START,
		URVCM_RECORD_PAUSE,
		URVCM_RECORD_STOP,
		URVCM_OPEN_OPTION,
		URVCM_CLOSE_OPTION,
	};


public:
	CX2UIRecordVideo( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	~CX2UIRecordVideo();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );

	//����, ���� Ű �Է����� ���� �� ó�� �Լ� 
	void				PauseKeyHandling();	
	CKTDGUIDialogType	GetDialog()	{	return m_pDlgRecordVideo;	}
private:
	void				InitUIRecordVideo();
	void				ReadyCloseUIRecordVideo();

	//�� ���µ��� ���°� �������� �������� Ui�� �ǹ��Ѵ� ����ȣ
	void				UI_RecdingStart();
	void				UI_RecdingPause();
	void				UI_RecdingStop();
	

	bool				m_bPause;

private:
	bool								m_bShow;
	const CKTDXStage* const				m_pNowStage;
	CKTDGUIDialogType					m_pDlgRecordVideo;

	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;

	CKTDGUIButton*	m_pButtonExit;				// ���ڵ� â �ݴ� ��ư
	CKTDGUIButton*	m_pButtonRecordStart;		// ���ڵ� ���� ��ư
	CKTDGUIButton*	m_pButtonRecordPause;		// ���ڵ� �Ͻ� ���� �� ����� ��ư
	CKTDGUIButton*	m_pButtonRecordStop;		// ���ڵ� ���� ��ư
	CKTDGUIButton*	m_pButtonRecordRec;			// ���ڵ� ���ڵ�
	//CKTDGUIButton*	m_pButtonOpenOption;		// �ɼ�â ���� ��ư
	//CKTDGUIButton*	m_pButtonCloseOption;		// �ɼ�â �ݴ� ��ư

//	CKTDGUIStatic*						m_pStaticHenirInfo;

};

#endif	BANDICAM_RECORDING
//}}
