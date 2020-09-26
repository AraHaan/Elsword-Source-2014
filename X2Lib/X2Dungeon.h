#pragma once
// ���� �߰��� �� �۾��ؾ��� ���� ����Ʈ
//		x2dungeon.h
//		xsldungeon.h
//		dungeondata.lua
//		channels.lua		-- ���� ������
//		dlg_map_local_velder_north.lua
//		dlg_map_local_velder_north_icon.lua
////	dlg_map_local_velder_icon_info.lua


#define CASE_DEFENCE_DUNGEON \
	case SEnum::DI_DEFENCE_DUNGEON_ELDER_NORMAL:	\
	case SEnum::DI_DEFENCE_DUNGEON_ELDER_HARD:		\
	case SEnum::DI_DEFENCE_DUNGEON_ELDER_EXPERT:	\


// ����: mapdata.lua ���� local, village ���̵� �߰�, ������ ���� ����


class CX2Dungeon
{
	public:
		enum DIFFICULTY_LEVEL
		{
			DL_NORMAL  = 0,
			DL_HARD,
			DL_EXPERT,
		};



#ifdef HENIR_TEST
		enum DUNGEON_TYPE
		{
			DT_INVALID,
			DT_NORMAL, 
			DT_HENIR,
			DT_SECRET,
		};

		enum DUNGEON_MODE
		{
			DM_INVALID,
			DM_HENIR_PRACTICE,
			DM_HENIR_CHALLENGE,
			DM_SECRET_NORMAL,
			DM_SECRET_HELL,
		};
#endif HENIR_TEST

		//Ŭ���̾�Ʈ�� �ʿ��� UI Data�� .. 
		struct DungeonDataUI
		{
			wstring		m_TextureName;
			wstring		m_PieceName;

			wstring		m_Explanation;

			D3DXVECTOR2 m_LocalStarPos;					//���� ������ ���� ��ǥ�� ��ġ ��ǥ
			D3DXVECTOR2 m_PopUpOffsetPos;				//���� ������ ���� �˾�â ������ ��ġ ��ǥ
#ifdef SERV_EPIC_QUEST
			D3DXVECTOR2 m_LocalQuestPos;
#endif SERV_EPIC_QUEST

			vector<wstring>		m_vecAutoRoomTitle;		// ������ ������ �ڵ����� �����Ǵ� �� ����

			//{{ 2009.1.22 ���¿� ������ �ε�ȭ��
#ifdef ENTER_SCREEN_FOR_DUNGEON
			wstring		m_LoadingScreenFileName;
			wstring		m_LoadingScreenFileName2;
#endif
			//}}

			DungeonDataUI()
                : m_TextureName()
                , m_PieceName()
                , m_Explanation()
                , m_LocalStarPos( 0, 0 )
                , m_PopUpOffsetPos( 0, 0 )
#ifdef SERV_EPIC_QUEST
                , m_LocalQuestPos( 0, 0 )
#endif SERV_EPIC_QUEST
                , m_vecAutoRoomTitle()
#ifdef ENTER_SCREEN_FOR_DUNGEON
                , m_LoadingScreenFileName()
                , m_LoadingScreenFileName2()
#endif  ENTER_SCREEN_FOR_DUNGEON
			{
			}

		};

		struct DungeonData
		{
		public:
			typedef std::vector< wstring > EndingSpeech;									// ����Ǵ� ��� ����
			typedef std::vector< EndingSpeech > EndingSpeechSet;							// ����Ǵ� ������ ����
			typedef std::map< CX2Unit::UNIT_TYPE, EndingSpeechSet > EndingSpeechSetMap;		// ĳ���� Ÿ�Ժ� ����Ǵ� ������ ������ map
			typedef std::map< CX2Unit::UNIT_TYPE, vector<int> >		mapBossDropItem;		// ĳ���� Ÿ�Ժ� �������� ȹ�氡���� ������ map

			

		public:
			std::vector< EndingSpeechSetMap > m_vecEndingSpeechSetMap;

			SEnum::DUNGEON_ID	m_DungeonID;
			wstring				m_DungeonName;
			wstring				m_DataFileName;
			SEnum::DUNGEON_ID	m_RequireDungeonID;
			vector< SEnum::DUNGEON_ID > m_vecExtraRequireDungeonID;		// ��д����� ���� Ư���� require dungeon id�� �������� ��쿡 ���⿡ �߰�


			bool		m_bHellMode;		// hell mode ������ ���������� �Ǳ� ������ ���� �����ܵ� ������ �ʾƾ� �Ѵ�
			
			//{{ 2012. 05. 22	������	���� ���� ������ ����
#ifdef SERV_DUNGEON_REQUIRE_ITEM_LEVEL
			int				m_RequireItemLevel;
#endif SERV_DUNGEON_REQUIRE_ITEM_LEVEL
			//}}
			
			//{{ 2007. 8. 29  ������  �������� ���� ������
			int			m_RequireItemID;
			int			m_RequireItemCount;
			//}}
			
			int			m_RequireItemID2;
			int			m_RequireItemCount2;
			

			//{{ 2007. 10. 4  ������  �ټ���
			int			m_RequireSpirit;
			//}}
			
			int			m_MinLevel;
			int			m_MaxLevel;

			CX2World::WORLD_ID m_eDefaultDungeonLoungeWorldID;

			DIFFICULTY_LEVEL m_eDifficulty;

			bool		m_bNormalOnly;			// ���̵��� "����"�� �ִ� �����̴�
			float		m_fTimeLimit;			// ���� �÷��� �ð� ����
#ifdef SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
			bool		m_bShowStageTime;
#endif SHOW_REMAIN_TIME_IN_CLEAR_CONDITION

#ifdef SERV_DUNGEON_OPTION_IN_LUA
			bool			m_bLevelEqualized;
			bool			m_bDamageEqualized;
			bool			m_bEventDungeon;
			unsigned short	m_usFixedMembers;
#endif SERV_DUNGEON_OPTION_IN_LUA

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
			int				m_iLimitedPlayTimes;
			int				m_iLimitedClearTimes;
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

			DungeonDataUI m_UIData;


#ifdef HENIR_TEST
			
			DUNGEON_TYPE	m_eDungeonType;
			bool			m_bRelativeMonsterLevel;

#endif HENIR_TEST
			wstring		m_DungeonDescription;
			vector<int>	m_vecBossItemList;
			mapBossDropItem m_mapBossDropItem;

#ifdef HIDE_LOADING_TIP
			bool		m_bHideLoadingTip;
#endif HIDE_LOADING_TIP

#ifdef X2TOOL
			int				m_iNpcLevel;
#endif
		};

		
		struct DungeonMapData
		{
			D3DXVECTOR2		m_vPos;
			bool			m_bOpen;		// user�� �� stage�� �����ߴ���
#ifdef NEW_HENIR_TEST
			bool			m_bAutoOpen;	// user�� �� stage�� �������� �ʾ�����, ���� ������ ��ǥ�� ��ġ�� �ٸ� stage�� ����Ǿ��� ����� ó��. 
			bool			m_bRestRoomState;
#endif NEW_HENIR_TEST
			bool			m_bIsBossStage;
			bool			m_bIsNextStageRandom;

			DungeonMapData()
			{
				m_vPos = D3DXVECTOR2(0,0);
				m_bOpen = false;
#ifdef NEW_HENIR_TEST
				m_bAutoOpen = false;
				m_bRestRoomState = false;
#endif NEW_HENIR_TEST
				m_bIsBossStage = false;
				m_bIsNextStageRandom = false;
			}
		};

		struct FullStageIndex
		{
			int m_iStageIndex;
			int m_iSubStageIndex;

			FullStageIndex()
			{
				m_iStageIndex = -1;
				m_iSubStageIndex = -1;
			}

			FullStageIndex( int iStageIndex, int iSubStageIndex )
			{
				m_iStageIndex = iStageIndex;
				m_iSubStageIndex = iSubStageIndex;
			}

			bool operator < ( CONST FullStageIndex& rhs ) const
			{
				if( m_iStageIndex < rhs.m_iStageIndex )
					return true;
				else if( m_iStageIndex > rhs.m_iStageIndex )
					return false;

				if( m_iSubStageIndex < rhs.m_iSubStageIndex )
					return true;

				return false;
			}
		};

	public:
		CX2Dungeon( const DungeonData* pDungeonData );
		//{{ 2007. 9. 14  ������  NPC ��ũ��Ʈ ���� �ޱ� ���θ� ������ �� �ִ� ������
		CX2Dungeon( const DungeonData* pDungeonData, bool bIsNpcLoad );
		//}}		
		~CX2Dungeon(void);

		void OnFrameMove( double fTime, float fElapsedTime );

		CX2DungeonStage*	CreateStage( int stageNum );
        void                CreateStageNPCs();

		void				FlushNPCs();
		CX2DungeonStage*	GetNowStage(){ return m_pNowStage; }

		int					GetNowStageIndex(){ return m_NowStageIndex; }
		int					GetNumberOfStageData(){ return m_StageDataList.size(); }
		
		void				SetStageStaticNPC( int stageNum, std::map<int, KNPCList>& mapNPCData );

		const DungeonData*	GetDungeonData() { return m_pDungeonData; }

#ifdef X2TOOL
		map< std::pair<int,int>, DungeonMapData >& GetMapDungeonMapData4Tool() { return m_mapDungeonMapData; }
#endif
		const map< std::pair<int,int>, DungeonMapData >& GetMapDungeonMapData() const { return m_mapDungeonMapData; }
		const map< std::pair< FullStageIndex, FullStageIndex>, bool >& GetMapStageLine() const { return m_mapStageLine; }
		void SetDungeonMapOpen( int iStageIndex, int iSubStageIndex, bool bOpen );
		bool GetDungeonMapOpen( int iStageIndex, int iSubStageIndex );
		bool IsStageExist( int iStageIndex, int iSubStageIndex );
		bool GetStageLineOpen( int iPrevStageIndex, int iPrevSubStageIndex, int iStageIndex, int iSubStageIndex );

		D3DXCOLOR GetWorldColor() const { return m_WorldColor; }
		D3DXCOLOR GetUnitColor() const { return m_UnitColor; }
		//{{ 2007. 10. 30  ������  
		void SetWorldColor( D3DXCOLOR worldColor ) { m_WorldColor = worldColor; }
		void SetUnitColor( D3DXCOLOR unitColor ) { m_UnitColor = unitColor; }
		//}}

//#ifdef CORRECTION_DAMAGE_FREE_CHANNEL
		static bool IsEventDungeon( const SEnum::DUNGEON_ID eDungeonId_ );
		static bool IsHenirDungeon( const SEnum::DUNGEON_ID eDungeonId_ );
#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		static bool IsSecretDungeon( const SEnum::DUNGEON_ID eDungeonId_ );
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
		static bool IsEventValentineDungeon( const SEnum::DUNGEON_ID eDungeonId_ );
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

		static bool IsDamageFreeGame();	// FieldFix: �̰� ������ �ִٴ°� ��ü�� �߸� �� ��
//#endif

#ifdef SERV_DUNGEON_OPTION_IN_LUA
		static bool IsFixedMembers( const SEnum::DUNGEON_ID eDungeonID_ );
		static short GetFixedMembers( const SEnum::DUNGEON_ID eDungeonID_ );
#endif SERV_DUNGEON_OPTION_IN_LUA

		static const WCHAR* GetDungeonDifficultyString( const DUNGEON_TYPE eDungeonType_, const char cDungeonMode_, const DIFFICULTY_LEVEL eDufficulty_ );

#ifdef	SERV_DUNGEON_FORCED_EXIT_SYSTEM
		void						SetBeforeSubStageIndexUsingPacket (const int index_ ) { m_iBeforeSubStageIndexUsingPacket = index_; }		// ���� SubStage �� ���� ����, Packet ������ ���ؼ� ����Ѵ�.
		void						SetBeforeStageIndexUsingPacket (const int index_ ) { m_iBeforeStageIndexUsingPacket = index_; }				// ���� Stage �� ���� ����, Packet ������ ���ؼ� ����Ѵ�.
		int							GetBeforeSubStageIndexUsingPacket () const { return m_iBeforeSubStageIndexUsingPacket; }					// ���� SubStage �� ���� ��ȯ, Packet ������ ���ؼ� ����Ѵ�.
		int							GetBeforeStageIndexUsingPacket () const { return m_iBeforeStageIndexUsingPacket; }							// ���� Stage �� ���� ��ȯ, Packet ������ ���ؼ� ����Ѵ�.
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM	

		//{{ 2007. 9. 18  ������  ���������� ����ϴ� �Լ�
	public: // for tool only
		vector<CX2DungeonStage::StageData*>& GetStageDataList() { return m_StageDataList; }
		//}}

	protected:
		bool OpenScriptFile( const WCHAR* pFileName );

		const DungeonData*						m_pDungeonData;			// ���� ���� ������ ��ü, �ѹ� �ε�Ǹ� ���� �ȵǵ���.
		CX2DungeonStage*						m_pNowStage;			// ���� �÷����ϰ� �ִ� �������� ��ü, �ε�� �Ŀ� ���� �� �� �ִ�
		int										m_NowStageIndex;
		float									m_fElapsedTime;
		bool									m_bIsNpcLoad;			// NPC ��ũ��Ʈ ���� �ҷ����� ����

		vector<CX2DungeonStage::StageData*>		m_StageDataList;

		
		map< std::pair<int,int>, DungeonMapData > m_mapDungeonMapData;	// stage index, substage index - dungeonmapdata
		map< std::pair< FullStageIndex, FullStageIndex>, bool > m_mapStageLine; // ������ substage ������ �� 
		
		FullStageIndex							m_PrevStageIndex;
		
		
		D3DXCOLOR	m_WorldColor;
		D3DXCOLOR	m_UnitColor;


#ifdef	SERV_DUNGEON_FORCED_EXIT_SYSTEM
	private :
		int							m_iBeforeSubStageIndexUsingPacket;			// ���� ���� ���������� ���� �����ϴ� ����, Packet �� �������� ����Ѵ�.
		int							m_iBeforeStageIndexUsingPacket;				// ���� ���������� ���� �����ϴ� ����, Packet �� �������� ����Ѵ�.
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM	

};

