#pragma once


#include "./X2MiniMap.h"
#include "./X2FieldMiniMap.h"

#include "./X2WorldMapUI.h"


class CX2MiniMapUI
{
public:
	enum MINI_MAP_UI_CUSTOM_MSG
	{
		MMUCM_DUNGEON_ZOOM_IN		= 22000,
		MMUCM_DUNGEON_ZOOM_OUT,
		
		MMUCM_FIELD_ZOOM_IN,
		MMUCM_FIELD_ZOOM_OUT,
		MMUCM_FIELD_OPEN_WORLD_MAP,

		MMUCM_FIELD_OPEN_NEW_QUEST_MAP,

		MMUCM_ZOOM_BUTTON,

		MMUCM_ENDURANCE_BUTTON_OVER,
		MMUCM_ENDURANCE_BUTTON_OUT,
		
		MMUCM_CHANGE_CHANNEL,
		MMUCM_CHANGE_CHANNEL_ACCEPT,
		MMUCM_CHANGE_CHANNEL_CANCEL,
		MMUCM_CHANGE_CHANNEL_OPEN,
		MMUCM_CHANGE_CHANNEL_CLOSE,
	};

	enum MINI_MAP_TYPE
	{
		MMT_DUNGEON,		// �̴ϸ� �κп� ���θ� �׸��� ���. ����, �ŷ����忡�� ���� �ִ�. MMT_LINEMAP���� �̸� �ٲܱ�?
		MMT_FIELD,			// �̴ϸ� �κп� ���� �׸��� �׸��� ���. �������� ���� �ִ�.
	};

	//{{ kimhc // 2009-12-15 // ������ �߰� �۾�
#ifdef	ADD_SERVER_GROUP
	struct CHANNEL_INFO_IN_MINIMAP
	{
		int				m_iChannelID;
		std::wstring	m_wstrChannelName;

		CHANNEL_INFO_IN_MINIMAP()
		{
			m_iChannelID = 0;
		}

		CHANNEL_INFO_IN_MINIMAP( int iChannelID, const std::wstring& wstrChannelName )
		{
			m_iChannelID		= iChannelID;
			m_wstrChannelName	= wstrChannelName;
		}

		bool operator<( const CHANNEL_INFO_IN_MINIMAP& rhs ) const
		{
#ifdef SERV_CHANNEL_LIST_RENEWAL
			return ( m_iChannelID < rhs.m_iChannelID ? true : false );
#else
			int iLhsOrderNum = GetNumRelatedWithChannelName();
			int iRhsOrderNum = rhs.GetNumRelatedWithChannelName();

			if ( iLhsOrderNum < iRhsOrderNum )
				return true;		// �������� ����
			else if ( iLhsOrderNum > iRhsOrderNum )
				return false;
			else	// ������
			{
				// ChannelID �� ��
				// �纥, ������ ���� �̸��� ���� ��� ä��ID�� ���� ��� ������ �տ� ����

				return ( m_iChannelID < rhs.m_iChannelID ? true : false );
			}
#endif
		}

		int GetNumRelatedWithChannelName() const;
	};
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // ������ �߰� �۾�

public:
	CX2MiniMapUI(void);
	virtual ~CX2MiniMapUI(void);


	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual HRESULT	OnFrameMove( double fTime, float fElapsedTime );


	CX2FieldMiniMap* GetFieldMiniMap() const { return m_pFieldMiniMap; }
	CX2MiniMap* GetDungeonMiniMap() const {	return m_pDungeonMiniMap; }
	CX2WorldMapUI* GetWorldMapUI();

	void CreateDungeonMiniMap()
	{
		CX2MiniMap::DeleteKTDGObject( m_pDungeonMiniMap );
		m_pDungeonMiniMap = CX2MiniMap::CreateMiniMap( this );
		g_pKTDXApp->GetDGManager()->AddObjectChain( (CKTDGObject*) m_pDungeonMiniMap );
	}

	void DeleteDungeonMiniMap()
	{
		CX2MiniMap::DeleteKTDGObject( m_pDungeonMiniMap );
		m_pDungeonMiniMap = NULL;
	}

	void CreateFieldMiniMap()
	{
		CX2FieldMiniMap::DeleteKTDGObject( m_pFieldMiniMap );
		m_pFieldMiniMap = CX2FieldMiniMap::CreateFieldMiniMap( this );

		g_pKTDXApp->GetDGManager()->AddObjectChain( (CKTDGObject*) m_pFieldMiniMap );
	}

	void DeleteFieldMiniMap()
	{
		CX2FieldMiniMap::DeleteKTDGObject( m_pFieldMiniMap );
		m_pFieldMiniMap = NULL;
	}

	void SetShowMiniMap( MINI_MAP_TYPE eMiniMapType, bool bShow );
	void SetStage( CKTDXStage* pStage );

	
	

	//void SetTitle( const wstring& minimapTitle )
	//{
	//	if( NULL != m_pDungeonMiniMap )
	//	{
	//		m_pDungeonMiniMap->SetTitle( minimapTitle*);
	//	}
	//}


	void SetVillageMapID( int iMapID );
	void SetTitle(const WCHAR* val);

	void UpdateMailNotice( std::vector<KPostSmallTitleInfo> &vecUnReadMailTitle );
	void UpdateQuestNotice( int iQuestNum, int TopQuestID, int VID );

	int GetTopQuestID(){ return m_TopQuestID; }
	int GetQuestVillageMapID(){ return m_QuestVillageMapID; }

	void ZoomDLGSetting();

	void CreateNewQuestEffect();
	void CreateNewMailEffect();

	void SetMonsterNum( bool bShow, int iNum = 0 );
	void SetNoExpAtThisDungeon(bool val);

//{{ kimhc // 2010.5.2 // ��д��� �۾�(����ġ)
#ifdef SERV_SECRET_HELL
	void SetWarningForGettingExp( bool bVal );		// ��д������� ����ġ�� ���� ���� �� �� �ִ� ��쿡 true
#endif SERV_SECRET_HELL
//}} kimhc // 2010.5.2 // ��д��� �۾�(����ġ)

	void UpdateEnduranceNotice();
	void SetShowEnduranceUI( bool bShow );

	void UpdateEventNotice();
	void UpdateChannelInfo();

#ifdef EVENT_CARNIVAL_DECORATION
	void UpdateCarnivalDeco();		
#endif EVENT_CARNIVAL_DECORATION

	void UpdateChannelIndex();

	//{{ kimhc // 2009-12-16 // ä�� �޺��ڽ� ����
#ifdef	ADD_SERVER_GROUP
	void SetChannelComboBox();
	int	GetListIndexFromChannelID( int iChannelID );	
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-16 // ä�� �޺��ڽ� ����

#ifdef DIALOG_SHOW_TOGGLE
	MINI_MAP_TYPE GetCurrentMiniMapType() { return m_eCurrentMiniMapType; }
#endif

	//{{ ����� : [2011/3/14/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
	bool			IsOpenDefenceMinimap() { return m_bIsOpenDefenceMinimap; }
#endif SERV_INSERT_GLOBAL_SERVER
		//}} ����� : [2011/3/14/] //	���� �̼�

#ifdef SERV_EVENT_MONEY
	void	SetShowAngelsFeather();
	void	UpdateAngelsFeatherCount(int iVal);
#endif //SERV_EVENT_MONEY

#ifdef SERV_MOMOTI_EVENT
	void	SetShowMomotiURLEvent();
	void	SetShowMomotiQuizEvent();
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_INT_UI_SHOW_EVENT_UI
	void	SetShowIntUIShowEventUI();
#endif //SERV_INT_UI_SHOW_EVENT_UI

	bool	GetShow()const {return m_bShow;}

	void CloseWorldMapUI() {if( NULL != m_pWorldMapUI )m_pWorldMapUI->CloseWorldMapUI();}

#ifdef DISABLE_CHANNEL_CHANGE_IN_SQUARE
	void SetEnableChannelComboBox( bool bVal_ );
#endif //DISABLE_CHANNEL_CHANGE_IN_SQUARE

#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
	void	SetShowTearOfELWoman();
	void	UpdateTearOfELWomanCount(int iVal);
#endif SERV_EVENT_TEAR_OF_ELWOMAN
private:
	CX2MiniMap*			m_pDungeonMiniMap;
	CX2FieldMiniMap*	m_pFieldMiniMap;
	CX2WorldMapUI*		m_pWorldMapUI;

	bool			m_bShow;

	CKTDGUIDialogType	m_pDLGFrameWindow;
	int				m_TopQuestID;			// ����Ʈ ��ư�� ������ �� ǥ������ ����Ʈ�� ID
	int				m_QuestVillageMapID;		// �� ��ư �������� ǥ������ ���� �� ID
	bool			m_bCreateNewQuestEffectWhenOpen;
	bool			m_bCreateNewMailEffectWhenOpen;

	int				m_iNewMail;

	MINI_MAP_TYPE	m_eCurrentMiniMapType;

	CX2UIEnduranceChecker	m_UIEnduranceChecker;

	int				m_iChangeChannelId;

	CKTDGUIDialogType	m_pDLGQuery;
	float			m_fChannelInfoTime;
	float			m_fChangeChannelTime;

	//{{ ����� : [2011/3/14/] //	���� �̼�
#ifdef SERV_INSERT_GLOBAL_SERVER
	bool			m_bIsOpenDefenceMinimap;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} ����� : [2011/3/14/] //	���� �̼�


	//{{ kimhc // 2009-12-16 // ������ �߰� �۾�
#ifdef	ADD_SERVER_GROUP
	std::vector< CHANNEL_INFO_IN_MINIMAP > m_vecChannelInfoInMinimap;
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-16 // ������ �߰� �۾�

#ifdef SERV_EVENT_MONEY
	CKTDGUIDialogType		m_pDLGAngelsFeather;
#endif //SERV_EVENT_MONEY

#ifdef SERV_MOMOTI_EVENT
	CKTDGUIDialogType		m_pDLGMomotiURLEvent;
	CKTDGUIDialogType		m_pDLGMomotiQuizEvent;
#ifdef SERV_MOMOTI_EVENT_ADDQUIZ
	CKTDGUIDialogType		m_pDLGMomotiQuizEvent2;
#endif //SERV_MOMOTI_EVENT_ADDQUIZ
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_INT_UI_SHOW_EVENT_UI
	CKTDGUIDialogType		m_pDLGSIntUIShowEventUI;
#endif //SERV_INT_UI_SHOW_EVENT_UI

#ifdef SERV_EVENT_TEAR_OF_ELWOMAN
	CKTDGUIDialogType		m_pDLGTearOfELWoman;
#endif SERV_EVENT_TEAR_OF_ELWOMAN
};
