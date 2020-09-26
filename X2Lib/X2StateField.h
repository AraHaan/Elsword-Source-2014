#pragma once

using namespace std;

class CX2StateField : public CX2StateMenu
{
public:
	friend class CX2StateAutoChanger;	

	struct DRAWFACE_RHW_VERTEX
	{
		float x, y, z, rhw;
		DWORD color;
		float u, v;
	};
	
public:
	enum FIELD_QUERY
	{
		FQ_DUNGEON_ACCEPT = 15200,
		FQ_DUNGEON_CANCEL,
		FQ_PARTY_JOIN_REQ,
		FQ_PARTY_JOIN_CANCEL,
	};

	enum GUIDE_UI_CUSTOM_MESSAGE
	{
		GUI_EPIC_EXIT	= 42001,
		GUI_ITEM_EXIT,
#ifdef SERV_EVENT_GUIDELINE_POPUP
		GUI_GUIDELINE_CLOSE,
#endif //SERV_EVENT_GUIDELINE_POPUP
	};

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	enum IDENTITY_CONFIRM_CUSTOM_MESSAGE
	{
		ICCM_OK	= 50000,
		ICCM_CANCEL,
	};
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	CX2StateField(void);
	virtual ~CX2StateField(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL

	virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

private:
	void SetVillageName();
	void ResourcePreLoad();

protected:
	//virtual bool ClickMenuBackButton(void) { return false; }	

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		

	void SetShowStateDLG(bool);
	void SetShowCommonBG();

	bool GetNowMovingToBattleField() const { return m_bNowMovingToBattleField; }
	void SetNowMovingToBattleField( const bool bNowMovingToBattleField_ ) { m_bNowMovingToBattleField = bNowMovingToBattleField_; }

	FORCEINLINE bool MoveFromVillageToBattleField( const int iVillageStartPosIdToBattleField_ ) const;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    bool MoveToOtherPlace( float fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	bool MoveToOtherPlace();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void DrawMovingSmallBar();
	void CreateMovingSmallBar();
	void DrawFace( const float fX_, const float fY_, const CKTDGUIControl::UITextureData& texData_, D3DCOLOR color_ /* = 0xffffffff */, const float fWidthPercent_ = 1.0f );

	virtual void UserAndPetListPopUpMenuProcess();
#ifdef PLAY_EMOTION_BY_USER_SELECT
	virtual void PlayEmotionByUserSelect();
#endif // PLAY_EMOTION_BY_USER_SELECT

public:

	void JoinSector( KFieldUserInfo *pKFieldUserInfo);
	void LeaveSector( UidType iUnitUID );


	bool Handler_EGS_STATE_CHANGE_FIELD_REQ();
	bool Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_FIELD_LOADING_COMPLETE_REQ();
	bool Handler_EGS_FIELD_LOADING_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool Handler_EGS_LEAVE_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); // ���Ϳ��� ���������� ������ �˷���
	//bool Handler_EGS_JOIN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); // ���Ϳ��� ���������� ������ �˷���

	//bool Handler_EGS_DEL_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); // ���� �̵��� ���� ������ ���ص� �Ǵ� ĳ���� ����Ʈ
	//bool Handler_EGS_NEW_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); // ���� �̵��� ���� �߰��� ������ �ؾߵǴ� ĳ���� ����Ʈ
	bool Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	 
	// 1�� ��ũ����
	bool Handler_EGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// �������� �ٷ� �̵�
	void Handler_EGS_CREATE_TUTORIAL_ROOM_REQ(int iDungeonId);
	bool Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	// �ŷ����� ����
	bool Handler_EGS_JOIN_SQUARE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	// ��Ƽ���� (���� ��Ƽ�ڽ�)
	//bool Handler_EGS_PARTY_TALKBOX_INFO_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	// ���� ��������� ��Ƽ�� ��Ż ó�� 
	bool Handler_EGS_LEAVE_ROOM_REQ();
	bool Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// �ʵ峻 �������� ��Ƽ����
	bool Handler_EGS_UPDATE_PARTY_UID_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_INVISIBLE_GM
	bool Handler_EGS_TOGGLE_INVISIBLE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_INVISIBLE_GM

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	bool Handler_EGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); // 2011.10.06 lygan_������ // ��ħ�� �˸� �˾� ǥ�ÿ�
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

#ifdef SERV_EPAY_SYSTEM
	bool Handler_EGS_EPAY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); // 2011.10.06 lygan_������ // ��ħ�� �˸� �˾� ǥ�ÿ�
#endif //SERV_EPAY_SYSTEM

	virtual bool Handler_EGS_CHAR_LEVEL_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifndef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE	/// �ش� ��� CX2State�� �̵� ( ���� ���� �� �����ϴٰ� �Ǵܽ� ���� ���� ���� ���� )
#ifdef NEW_DEFENCE_DUNGEON
	void SetDefenceDungeonChangeLevelNotice();																	//����� �� ���̵� ����� �˸� �ؽ�Ʈ ����ϴ� �Լ�
	void SetDefenceDungeonNoticeTime( float fDefenceDungeonNoticeTime ) { m_fDefenceDungeonNoticeTime = fDefenceDungeonNoticeTime; }	//��µ� �ؽ�Ʈ ǥ�� �ð�
	void SetDefenceDungeonNoticeType( int   iDefenceDungeonNoticeType ) { m_iDefenceDungeonNoticeType = iDefenceDungeonNoticeType; }	//��µ� �ؽ�Ʈ ����
#endif NEW_DEFENCE_DUNGEON
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	void SetTogetherFestivalEarthQuakeNotice();			/// ������ ���� ���� �̺�Ʈ �߻��� �����Ǵ� ����
	void SetTogetherFestivalEarthQuakeNoticeTime( float fTogetherFestivalEarthQuakeTime_ ) { m_fTogetherFestivalEarthQuakeTime = fTogetherFestivalEarthQuakeTime_; }	//��µ� �ؽ�Ʈ ǥ�� �ð�
#endif TOGETHER_FESTIVAL_2012_AUTUMN

#ifdef ADDED_RELATIONSHIP_SYSTEM
	/// mauntain // 2013-05-03 // ��ȥ �ý��� ����� ��ȯ ���
	virtual bool Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // ADDED_RELATIONSHIP_SYSTEM

private:
	CKTDGUIDialogType		m_pDLGJoinParty;
	CKTDGUIDialogType		m_pDLGExitMsgBox;
	CKTDGUIDialogType		m_pDLGSquareTitle;	
#ifdef SERV_EVENT_GUIDELINE_POPUP
	CKTDGUIDialogType		m_pDLGDungeonReformGuide;
#endif //SERV_EVENT_GUIDELINE_POPUP

	D3DXVECTOR3			m_vOldEyePt;
	D3DXVECTOR3			m_vOldLookAtPt;

	bool				m_bNowMovingToBattleField;	/// ���� �ʵ�� �̵� ���̶��
	
	float				m_fLoadCompleteTimer;
	bool				m_bSendLoadCompleteReq;
	bool				m_bLoadCompleteReq;

	UidType				m_TalkBoxunitUIDToFind;	// ����
	UidType				m_TalkBoxunitUID;

	int					m_iTalkNpcIndex;

	CKTDXCheckElapsedTime				m_TimerWaitingPortal;			// �̵� ��� �ð� ���	
	CKTDGUIControl::UITextureData		m_TexDataMovingGageBG;
	CKTDGUIControl::UITextureData		m_TexDataMovingGage;
	CKTDGStateManager::KStateID			m_RenderStateID;	/// �������̽� ����� ���� 2D ����Լ�

#ifndef FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE	/// �ش� ��� CX2State�� �̵� ( ���� ���� �� �����ϴٰ� �Ǵܽ� ���� ���� ���� ���� )
#ifdef NEW_DEFENCE_DUNGEON
	CKTDGFontManager::CUKFont*	m_pFontDefenceDungeonNotice;	// ����� �� ���̵� ����� ȭ�鿡 ��µ� �ؽ�Ʈ
	float						m_fDefenceDungeonNoticeTime;	// ��µ� �ؽ�Ʈ ǥ�� �ð�
	int							m_iDefenceDungeonNoticeType;	// ��µ� �ؽ�Ʈ ����
#endif NEW_DEFENCE_DUNGEON
#endif FIX_NEW_DEFENCE_DUNGEON_LEVEL_NOTICE

#ifdef TOGETHER_FESTIVAL_2012_AUTUMN
	CKTDGFontManager::CUKFont*	m_pFontTogetherFestivalEarthQuakeNotice;	/// ������ ���� ���� �̺�Ʈ �߻��� �����Ǵ� ����
	float						m_fTogetherFestivalEarthQuakeTime;			/// ��µ� �ؽ�Ʈ ǥ�� �ð�
#endif TOGETHER_FESTIVAL_2012_AUTUMN

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	SHORT						m_SumDelta;		/// ���콺 �� ���尪
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL
};


