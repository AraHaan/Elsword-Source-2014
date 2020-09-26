#pragma once


// �� ĳ���ͳ� ���� �߰��� �ؾ��ϴ� �۾�
// 1. EGS_ADMIN_CHANGE_UNIT_CLASS_REQ ó���ϴ� �����ڵ忡 �߰�
// 2. stattable.xls, stattable.mdb, statable.lua
// 3. UI ����, ��ųƮ��, 


class CX2Item;
class CX2Inventory;
class CX2Eqip;
class CX2Unit : public CKTDXDeviceHolder, public CX2Unit_PreHeader
{
	public: 
		enum UNIT_TYPE
		{
			UT_NONE	= 0,
			UT_ELSWORD,
			UT_ARME,
			UT_LIRE,
			UT_RAVEN,
			UT_EVE,
			UT_CHUNG,		/// kimhc // 2010-12-23 �� �߰��� ��ĳ���� û
			UT_ARA,			/// JHKang / �� ĳ���� �ƶ�
			UT_ELESIS,		/// ������ // 2013-05-28 // ��ĳ���� �����ý�
			UT_ADD,			/// ����ȯ // 2013-10-25 // ��ĳ���� �ֵ�


			UT_END,
		};

		enum UNIT_CLASS
		{
			// �ʱ�����
			UC_NONE						= 0,

			UC_ELSWORD_SWORDMAN,		// 1
			UC_ARME_VIOLET_MAGE,		// 2
			UC_LIRE_ELVEN_RANGER,		// 3
			UC_RAVEN_FIGHTER,			// 4
			UC_EVE_NASOD,				// 5
			UC_CHUNG_IRON_CANNON,		// 6 // kimhc // 2010-12-23 �� �߰��� ��ĳ���� û
			UC_ARA_MARTIAL_ARTIST,		/// 7 �ƶ� ������, ������
			UC_ELESIS_KNIGHT,			// 8 /// ������ // 2013-05-28 // ��ĳ���� �����ý�
			UC_ADD_NASOD_RULER,// 9 /// ����ȯ // 2013-10-25 // ��ĳ���� �ֵ�

		
			// 1�� ����
			UC_ELSWORD_KNIGHT			= 10,		
			UC_ELSWORD_MAGIC_KNIGHT,	// 11
			UC_LIRE_COMBAT_RANGER,		// 12
			UC_LIRE_SNIPING_RANGER,		// 13
			UC_ARME_HIGH_MAGICIAN,		// 14
			UC_ARME_DARK_MAGICIAN,		// 15
			UC_RAVEN_SOUL_TAKER,		// 16
			UC_RAVEN_OVER_TAKER,		// 17
			UC_EVE_EXOTIC_GEAR,			// 18
			UC_EVE_ARCHITECTURE,		// 19
			UC_CHUNG_FURY_GUARDIAN,		// 20	// kimhc // û 1�� ����
			UC_CHUNG_SHOOTING_GUARDIAN,	// 21	// kimhc // û 1�� ����
#ifdef ELSWORD_SHEATH_KNIGHT
			UC_ELSWORD_SHEATH_KNIGHT,	// 22
#endif ELSWORD_SHEATH_KNIGHT
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			UC_ARME_BATTLE_MAGICIAN,	// 23
#endif
//#ifdef	SERV_TRAPPING_RANGER_TEST
			UC_LIRE_TRAPPING_RANGER		= 24,		// 24	���� Ʈ���� ������
//#endif	SERV_TRAPPING_RANGER_TEST
#ifdef RAVEN_WEAPON_TAKER
			UC_RAVEN_WEAPON_TAKER		= 25,
#endif RAVEN_WEAPON_TAKER
#ifdef EVE_ELECTRA
			UC_EVE_ELECTRA				= 26,
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			UC_CHUNG_SHELLING_GUARDIAN	= 27,
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
			UC_ARA_LITTLE_HSIEN			= 28,	/// �ƶ� 1-1��, �Ҽ�
#endif
			UC_ELESIS_SABER_KNIGHT		= 29,	/// �����ý� 1-1�� ���̹� ����Ʈ
			UC_ELESIS_PYRO_KNIGHT		= 30,	/// �����ý� 1-2�� ���̷� ����Ʈ
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
			UC_ARA_LITTLE_DEVIL			= 31,	/// �Ҹ� - �ƶ� 1�� ����
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			UC_ADD_PSYCHIC_TRACER		= 32,	/// ����ű Ʈ���̼� - �ֵ� 1�� ����
#endif //SERV_9TH_NEW_CHARACTER


			// 2�� ����
			UC_ELSWORD_LORD_KNIGHT		= 100,	
			UC_ELSWORD_RUNE_SLAYER,		// 101
			UC_LIRE_WIND_SNEAKER,		// 102
			UC_LIRE_GRAND_ARCHER,		// 103
			UC_ARME_ELEMENTAL_MASTER,	// 104
			UC_ARME_VOID_PRINCESS,		// 105
			UC_RAVEN_BLADE_MASTER,		// 106
			UC_RAVEN_RECKLESS_FIST,		// 107
			UC_EVE_CODE_NEMESIS,		// 108
			UC_EVE_CODE_EMPRESS,		// 109
#ifdef CHUNG_SECOND_CLASS_CHANGE
			UC_CHUNG_IRON_PALADIN,		/// 110 ���̾� �ȶ��
			UC_CHUNG_DEADLY_CHASER,		/// 111 ���鸮 ü�̼�
#endif

#ifdef SERV_ELSWORD_INFINITY_SWORD
			UC_ELSWORD_INFINITY_SWORD,	// 112
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
			UC_ARME_DIMENSION_WITCH,	// 113
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
			UC_LIRE_NIGHT_WATCHER,		// 114
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
			UC_RAVEN_VETERAN_COMMANDER,	// 115
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
			UC_EVE_BATTLE_SERAPH,	// 116
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
			UC_CHUNG_TACTICAL_TROOPER,	// 117
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
			UC_ARA_SAKRA_DEVANAM,		/// �ƶ� 1-2��, ��õ
#endif
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
			UC_ARA_YAMA_RAJA			= 119,	// ��� - �ƶ� 2�� ����
#endif // SERV_ARA_CHANGE_CLASS_SECOND
// #ifdef SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 2�� ����, �׷��� ������ / ����¡ ��Ʈ
			UC_ELESIS_GRAND_MASTER		= 120,	// ������ // 2013-09-04 // �����ý� 1-2 �׷��� ������
			UC_ELESIS_BLAZING_HEART		= 121,	// ������ // 2013-09-04 // �����ý� 2-2 ����¡ ��Ʈ
// #endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 2�� ����, �׷��� ������ / ����¡ ��Ʈ

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
			UC_ADD_LUNATIC_PSYKER		= 122,	// �糪ƽ ����Ŀ - �ֵ� 2�� ����
#endif //SERV_ADD_LUNATIC_PSYKER
			
			// ������ // 2013-07-04 // 2������ enum ��ȸ�� ���� �߰�
			UC_VALIDE_END,

			// xslunit.h ���� �߰��ؾ��մϴ�.
//{{ robobeg : 2013-02-27
            UC_END,
//}} robobeg : 2013-02-27
		};


#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ - ��ų �ý��� ����
		enum UNIT_CLASS_TYPE
		{
			UCT_NONE			= -1,
			UCT_BASIC_CLASS		= 0,	/// �⺻ ����
			UCT_FIRST_CLASS		= 1,	/// 1�� ����
			UCT_SECOND_CLASS	= 2,	/// 2�� ����
			UCT_END				= 3,
		};
#endif // UPGRADE_SKILL_SYSTEM_2013

// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		enum UNIT_CLASS_LINE
		{
			UCL_NONE			= 0,
			UCL_FIRST,		
			UCL_SECOND,		
			UCL_THIRD,	
		};
// #endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

		enum NOW_EQIP_SLOT_ID
		{
			NESI_DEFENCE_HAIR_FASHION = 0,		

			NESI_DEFENCE_BODY,				
			NESI_DEFENCE_BODY_FASHION,		

			NESI_DEFENCE_LEG,				
			NESI_DEFENCE_LEG_FASHION,

			NESI_DEFENCE_HAND,				
			NESI_DEFENCE_HAND_FASHION,	

			NESI_DEFENCE_FOOT,				
			NESI_DEFENCE_FOOT_FASHION,		

			NESI_WEAPON_HAND,				
			NESI_WEAPON_HAND_FASHION,

			NESI_AC_FACE1,				//��(��)
			NESI_AC_FACE2,				//��(��)
			NESI_AC_FACE3,				//��(��)
			NESI_AC_BODY,				//����
			NESI_AC_LEG,				//�ٸ�
			NESI_AC_ARM,				//��
			NESI_AC_RING,				//����
			NESI_AC_NECKLESS,			//�����

			NESI_AC_WEAPON,				// ���� �Ǽ��縮
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
			NESI_ONEPIECE_FASHION,		//���ǽ� �ƹ�Ÿ
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

			NESI_END,
		};

		enum NOW_SKILL_SLOT_ID
		{
			NSSI_SKILL_1 = 0,			//1�ܰ� �ʻ��
			NSSI_SKILL_2,				//2�ܰ� �ʻ��
			NSSI_SKILL_3,				//3�ܰ� �ʻ��

			NSSI_END,
		};

		enum NOW_QUICK_SLOT_ID
		{
			NQSI_QUICK_SLOT_1 = 0,
			NQSI_QUICK_SLOT_2,
			NQSI_QUICK_SLOT_3,
			NQSI_QUICK_SLOT_4,
			NQSI_QUICK_SLOT_5,
			NQSI_QUICK_SLOT_6,
			
			NQSI_END,
		};

		enum CONNECT_UNIT_STATE
		{
			CUS_NONE		= 0,
			CUS_FIELD_MAP,
			CUS_MARKET,
			CUS_PVP_LOBBY,
			CUS_PVP_WAIT,
			CUS_PVP_PLAY,
			CUS_TC_PLAY,
			CUS_DUNGEON_PLAY,
			CUS_ARCADE_PLAY,
		};
#ifdef UNIT_EMOTION
		enum EMOTION_TYPE
		{
			ET_NONE		= 0,
			ET_SITWAIT,

			ET_SITREADY,			
			ET_STANDUP,
			ET_ANGRY,
			ET_HELLO,
			ET_NO,
			ET_SAD,			
			ET_SMILE,

			ET_EMOTION_AVATAR1,	// ��� ��
			ET_EMOTION_AVATAR2, // �Ѻ� ��
			ET_EMOTION_AVATAR3, // ������ũ �㽬
			ET_EMOTION_AVATAR4, // ������ũ õ��
			ET_EMOTION_AVATAR5,	// CRAYONPOP
	#ifdef CRAYONPOP_SECOND_EMOTION // ����ȯ
			ET_EMOTION_AVATAR6,	// CRAYONPOP �ѹ�
	#endif // CRAYONPOP_SECOND_EMOTION
	
	#ifdef CRAYONPOP_EMOTION_WITH_MUSIC
			ET_EMOTION_AVATAR7,
	#endif // CRAYONPOP_EMOTION_WITH_MUSIC
	
		};
#endif

#ifdef SERV_GROW_UP_SOCKET
		enum GROW_UP_TYPE
		{
			GUT_NONE,
			GUT_TAG_POINT,
			GUT_QUEST_CLEAR_COUNT,
			GUT_EXCHANGE_COUNT,
		};
#endif //SERV_GROW_UP_SOCKET

		struct UnitTemplet
		{
			UNIT_TYPE			m_UnitType;
			UNIT_CLASS			m_UnitClass;

			wstring				m_Name;
			wstring				m_Description;
			wstring				m_MotionFile;
			wstring				m_FieldMotionFile;

			CX2Stat::Stat		m_Stat;

			int					m_BasicWeaponItemID;
			int					m_BasicHairItemID;
			int					m_BasicFaceItemID;
			int					m_BasicBodyItemID;
			int					m_BasicLegItemID;
			int					m_BasicHandItemID;
			int					m_BasicFootItemID;

			int					m_RavenLeftArmItemID;	// dmlee 2008.07.31 - ���̺� ����, ������ �����鼭
			int					m_SecondWeaponItemID;	// dmlee 2008.12.12 - �� ��° ����

#ifdef SEPARATION_MOTION
			wstring				m_CommonMotion;
			wstring				m_FieldMotion;
			wstring				m_LobbyMotion;
			wstring				m_EmotionMotion;

			wstring				m_GameMotion0;
			wstring				m_GameMotion1;
			wstring				m_GameMotion2;			
#endif

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
			// ���θ�� ������ ID
			int					m_PromotionWeaponItemID;	// ����
			int					m_PromotionBodyItemID;		// ����
			int					m_PromotionLegItemID;		// ����
			int					m_PromotionHandItemID;		// �尩
			int					m_PromotionFootItemID;		// �Ź�
			
			wstring				m_IntroMovieFileName;		// ĳ���� ���� â���� ��� �� ������ �̸�
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh
		};

		struct UnitData
		{
		public:
#ifdef TODAY_RECORD_TEST
			struct DailyAchievement
			{
				int m_iKillCount;
				int m_iAssistCount;
				int m_iDieCount;

				int m_iWinCount;
				int m_iLoseCount;
				int m_iDrawCount;
				int	m_iMaxConsecutivePVPWinCount;
				int m_iMaxConsecutivePVPLoseCount;

				DailyAchievement() :
				m_iKillCount(0),
				m_iAssistCount(0),
				m_iDieCount(0),
				m_iWinCount(0),
				m_iLoseCount(0),
				m_iDrawCount(0),
				m_iMaxConsecutivePVPWinCount(0),
				m_iMaxConsecutivePVPLoseCount(0)
				{
				}
			};
#endif TODAY_RECORD_TEST

			UidType					m_UnitUID;
			UidType					m_UserUID;
			UNIT_CLASS				m_UnitClass;
			
#ifdef _NEXON_KR_
			_UInt32_				m_iNMKSerialNum;	//�ؽ��ڸ��� �޽����� ���� �ø��� ��ȣ
#endif
			wstring				    m_IP;
			int						m_Port;
			//{{ 2013. 1. 9	�ڼ���	Merge ����IP ���� ���н� ����IP�� �õ�( ������ )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
			wstring				    m_InternalIP;
			int						m_InternalPort;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
			//}}

			wstring					m_NickName;
			int						m_Age;
			bool					m_bMan;

			KProtectedType<int>		m_ED;			
			KProtectedType<int>		m_Level;
			KProtectedType<int>		m_EXP;

#ifdef SERV_PVP_NEW_SYSTEM
			KProtectedType<int>		m_iRating;
			KProtectedType<int>		m_iRPoint;
			KProtectedType<int>		m_iAPoint;
#else
			CX2PVPEmblem::PVP_EMBLEM m_PVPEmblem;
			KProtectedType<int>		m_VSPoint;
			KProtectedType<int>		m_VSPointMax;
#endif

#ifdef PVP_SEASON2
			KProtectedType<char>	m_cRank;
			int						m_iPastSeasonWin;
			float					m_fKFactor;
#endif

			int						m_Win;
			int						m_Lose;
			int						m_Seceder;

			KProtectedType<int>		m_NowBaseLevelEXP;
			KProtectedType<int>		m_NextBaseLevelEXP;

	

			CX2Stat::Stat 			m_Stat;
			CX2Stat::Stat 			m_GameStat;
			KProtectedType<int>		m_iSPoint;				// ������ �ִ� ��ų ����Ʈ
#ifdef NEW_SKILL_TREE
			KProtectedType<int>		m_iCSPoint;				// ������ �ִ� ĳ�� ��ų ����Ʈ
			std::wstring			m_wstrCSPointEndDate;
#endif NEW_SKILL_TREE

#ifdef GUILD_SKILL
			//{{ oasis907 : ����� //// 2009-11-12 //// 
			KProtectedType<int>		m_iGuildSPoint;				// ������ �ִ� ��ų ����Ʈ
			KProtectedType<int>		m_iGuildCSPoint;				// ������ �ִ� ĳ�� ��ų ����Ʈ
			KProtectedType<int>	    m_iMaxGuildCSPoint; 
			std::wstring			m_wstrGuildCSPointEndDate;
			//}} oasis907 : ����� //// 2009-11-12 //// 
#endif GUILD_SKILL

			vector<UidType>			m_NowEqipItemUIDList;
			//CX2Inventory*			m_pInventory;
            CX2Inventory            m_Inventory;

			KProtectedType<int>		m_nStraightVictories;
			KProtectedType<int>		m_nMapID;
#ifdef REMEMBER_LOGOUT_POSITION_TEST
			unsigned char			m_ucLastTouchLineIndex;		// ĳ���� �����ؼ� ������ ������ ������ ��ȿ�ϴ�
			unsigned short			m_usLastPosValue;			// ĳ���� �����ؼ� ������ ������ ������ ��ȿ�ϴ�
	#ifdef FIELD_BOSS_RAID
			bool					m_bIgnoreLastTouch;			// LastTouch�� ������� �ʰ� m_iMapID�� �ش��ϴ� �⺻ ��ġ�� ����ϵ��� �˸���.
	#endif // FIELD_BOSS_RAID
#endif REMEMBER_LOGOUT_POSITION_TEST

			bool					m_bIsGameBang;
			float					m_fAddExpRate;

			CX2UserSkillTree		m_UserSkillTree;
			bool					m_bIsParty;

			//{{ 2007. 10. 5  ������  �ټ��� �߰�
			int						m_iSpirit;
			int						m_iSpiritMax;
			//}}

#ifdef SERV_CHINA_SPIRIT_EVENT
			int						m_arrChinaSpirit[6];
#endif SERV_CHINA_SPIRIT_EVENT

			vector<KChatBlackListUnit>	m_BlackList;

			//{{ ����� : [2009/9/25] //	��� �̸� �߰�
#ifdef GUILD_MANAGEMENT
			wstring						m_wstrGuildName;
			BYTE						m_byMemberShipGrade;
			int							m_iGuildHonorPoint;
#endif	//	GUILD_MANAGEMENT
			//}} ����� : [2009/9/25] //

#ifdef TODAY_RECORD_TEST
			DailyAchievement			m_DailyAchievement;
#endif TODAY_RECORD_TEST

//#ifdef TITLE_SYSTEM
            int							m_iTitleId;
//#endif

#ifdef BUFF_TEMPLET_SYSTEM	
			vector<int>					m_vecPremiumBuffList;
#endif BUFF_TEMPLET_SYSTEM

			//{{ 2012.02.20 ��ȿ��	ĳ���� ���� ���μ��� ���� (���� ��� �Ⱓ ����)
#ifdef SERV_UNIT_WAIT_DELETE
			std::wstring				m_wstrLastDate;			// ���� Logout Ÿ��
			bool						m_bDeleted;				// ���� ��� ���� ����
			__int64						m_trDelAbleDate;		// ���� ���� �Ͻ�
			__int64						m_trRestoreAbleDate;	// ���� ���� �Ͻ�
			bool						m_bDeletedMotionPlay;	// ���� ��� ��� ������ //2012.03.07 lygan_������ // ĳ���� ���� ��� �����
#endif SERV_UNIT_WAIT_DELETE
			//}}
#ifdef SERV_ADD_WARP_BUTTON
			__int64						m_trWarpVipEndDate;	// �ں� VIP ������ �ð�
			bool						m_bWarpVip;
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_INVISIBLE_GM
			bool						m_bInvisible;
#endif SERV_INVISIBLE_GM

			//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
			char						m_cWeddingStatus;
			UidType						m_iLoverUnitUID;
#endif //SERV_RELATIONSHIP_SYSTEM
			//}

#ifdef SERV_GROW_UP_SOCKET
			std::map<GROW_UP_TYPE, int>	m_mapGrowUpPoint;	// ���� ����Ʈ
#endif//SERV_GROW_UP_SOCKET

#ifdef SERV_GROW_UP_TITLE
			int							m_iOldLevel;
#endif SERV_GROW_UP_TITLE

#ifdef SERV_NEW_YEAR_EVENT_2014
			u_char						m_ucOldYearMissionRewardedLevel;
			int							m_iNewYearMissionStepID;
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
			int							m_iGateOfDarknessSupportEventTime;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
			UidType						m_iRelationTargetUserUid;
			wstring						m_wstrRelationTargetUserNickname;
			bool						m_bCouple;
#endif SERV_RELATIONSHIP_EVENT_INT
			
#ifdef SERV_ELESIS_UPDATE_EVENT
			int							m_iNoteViewCount;
			bool						m_bReserveShow;
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_EVENT_CHECK_POWER
			unsigned char				m_ucCheckPowerCount;
			__int64						m_iCheckPowerTime;
			bool						m_bCheckPowerShowPopUp;
			unsigned char				m_ucCheckPowerScore;
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
			int							m_iValentineItemCount;
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_4TH_ANNIVERSARY_EVENT
			int							m_iAccountPVPLoseCount;
#endif //SERV_4TH_ANNIVERSARY_EVENT

        private:

			void Init();

		public:
            UnitData() { Init(); }
			~UnitData();

            void    Init( CX2Unit* pOwnerUnit, const KUnitInfo& data );
            void    Init( CX2Unit* pOwnerUnit, const KRoomUserInfo& data );
            void    Init( CX2Unit* pOwnerUnit, const KSquareUserInfo& pKSquareUserInfo );
            void    Init( CX2Unit* pOwnerUnit, const KFieldUserInfo& pKFieldUserInfo );
			
			void SetKUnitInfo( const KUnitInfo& data );
			void SetKRoomUserInfo( const KRoomUserInfo& data );
			void SetKSquareUserInfo( const KSquareUserInfo& pKSquareUserInfo );
			void SetKFieldUserInfo( const KFieldUserInfo& pKFieldUserInfo );

			void SetBlackList( vector<KChatBlackListUnit>& blackList );
			bool AddBlackList( KChatBlackListUnit blackList );
			bool RemoveBlackList( UidType unitUID );
			const vector<KChatBlackListUnit>& GetBlackList() const { return m_BlackList; }
			UidType GetBlackListUnitUID( const WCHAR* wszNickName ) const;
		
#ifdef BUFF_TEMPLET_SYSTEM
			const vector<int>&	GetPremiumBuffList() const { return m_vecPremiumBuffList; }
			void				SetPremiumBuffInfo( const vector<KBuffInfo>& vecBuffInfo_ );
			void				ErasePremiumBuffInfo( const vector<int>& vecBuffInfo_ );
#ifdef PVP_SEASON2
			void				ClearPremiumBuffInfo();
#endif
#endif BUFF_TEMPLET_SYSTEM
					

			void Verify() const;

#ifdef SERV_INVISIBLE_GM
			void SetInvisible( bool bInvisible ) { m_bInvisible = bInvisible; }
			bool IsInvisible() { return m_bInvisible; }
#endif SERV_INVISIBLE_GM

#ifdef SERV_GROW_UP_SOCKET
		protected:
			bool _CheckGrowUpNow( GROW_UP_TYPE eType_ );
		public:
			void SetGrowUpPoint( GROW_UP_TYPE eType_, int const iPoint_, UidType nUnitUID );
			void IncreaseGrowUpPoint( GROW_UP_TYPE eType_, int const iPoint_, UidType iUnitUID_ );
			int  GetGrowUpPoint( GROW_UP_TYPE eType_ );
			int  GetGrowUpLevel( GROW_UP_TYPE eType_ );
			int  GetGrowUpLevelBySocket( const int iType_ );
			int  GetGrowUpLevelByTitle( const int iTitleID );
#endif //SERV_GROW_UP_SOCKET

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		void SetGateOfDarknessSupportEventTime( int iGateOfDarknessSupportEventTime ) { m_iGateOfDarknessSupportEventTime = iGateOfDarknessSupportEventTime; }
		int GetGateOfDarknessSupportEventTime() const { return m_iGateOfDarknessSupportEventTime; }
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		void SetRelationTargetUserUID( UidType iRelationTargetUserUid ) { m_iRelationTargetUserUid = iRelationTargetUserUid; }
		UidType GetRelationTargetUserUID() const { return m_iRelationTargetUserUid; }
		void SetRelationTargetUserNickname( wstring wstrRelationTargetUserNickname ) { m_wstrRelationTargetUserNickname = wstrRelationTargetUserNickname; }
		wstring GetRelationTargetUserNickname() const { return m_wstrRelationTargetUserNickname; }
		void SetCouple( bool bCouple ) { m_bCouple = bCouple; }
		bool GetCouple() const { return m_bCouple; }
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_ELESIS_UPDATE_EVENT
		void SetNoteViewCount( int iNoteViewCount ) { m_iNoteViewCount = iNoteViewCount; }
		int GetNoteViewCount() { return m_iNoteViewCount; }
		void SetReserveShow( bool bReserveShow ) { m_bReserveShow = bReserveShow; }
		bool GetReserveShow() { return m_bReserveShow; }
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		void SetValentineItemCount(int iTempCount) { m_iValentineItemCount = iTempCount; }
		int GetValentineItemCount(void) const { return m_iValentineItemCount; }
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		};

		//{{ kimhc // 2009-12-09 // ũ�������� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
		struct EventData
		{
			KTimeEventSimpleInfo	m_kTimeEventSimpleInfo;
			CKTDXTimer				m_pElapsedTimer;

			EventData( const KTimeEventSimpleInfo& kTimeEventSimpleInfo )
			{
				m_kTimeEventSimpleInfo = kTimeEventSimpleInfo;
				m_pElapsedTimer.restart();
			}
		};
#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-09 // ũ�������� �̺�Ʈ
	public:
		CX2Unit( const KUnitInfo& UnitInfo );
		CX2Unit( const KRoomUserInfo& RoomUserInfo );
		CX2Unit( const KSquareUserInfo& pKSquareUserInfo );
		CX2Unit( const KFieldUserInfo& pKFieldUserInfo );
		virtual ~CX2Unit(void);

		void			Init();

		void			Reset( const KUnitInfo& unitInfo, bool bForceUpdate = false );
		void			ResetExceptDungeonClear( const KUnitInfo& unitInfo );
		void			ResetInventory( std::map< int, int >& mapInventorySlotSize, std::map< UidType, KInventoryItemInfo >& mapItem );
		void			ResetUnitClass( CX2Unit::UNIT_CLASS eUnitClass );

		//{{ kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����
#ifdef	PC_BANG_WORK
		void			ResetInventorySize( const std::map<int, int>& mapInventorySize );
		void			ResetInventoryItems( const std::map< UidType, KInventoryItemInfo >& mapItem );
#endif	PC_BANG_WORK
		//}} kimhc // 2010-01-04 // ������ EGS_SELECT_UNIT_ACK�� �κ������� ���ԵǾ� ���� �� ����

//{{ robobeg : 2013-11-01
        //UnitData*		GetUnitData()							    { return &m_UnitData;							}
  //      const CX2Inventory*	GetInventory() const					{ return m_UnitData.m_pInventory;				}
		const UnitData&		GetUnitData() const							{ return m_UnitData;							}
		UnitData&		AccessUnitData()							{ return m_UnitData;							}
		const CX2Inventory&	GetInventory() const					{ return m_UnitData.m_Inventory;				}
        CX2Inventory&   AccessInventory()                           { return m_UnitData.m_Inventory;				}
//}} robobeg : 2013-11-01
		const UnitTemplet*	GetUnitTemplet() const					{ return m_pUnitTemplet;						}

		UidType			GetUID() const								{ return m_UnitData.m_UnitUID;				}
		UidType			GetOwnerUserUID() const						{ return m_UnitData.m_UserUID;				}
		void			SetOwnerUserUID( UidType uidType )			{ m_UnitData.m_UserUID = uidType;				}
		UNIT_TYPE		GetType() const								{ return m_pUnitTemplet->m_UnitType;			}
		UNIT_CLASS		GetClass() const							{ return m_pUnitTemplet->m_UnitClass;			}

		int				GetClassLevel();
		const WCHAR*	GetNickName() const						{ return m_UnitData.m_NickName.c_str();		}

		int				GetViewEqipNum() const					{ return (int)m_ViewEqipItemUIDList.size();		}
		UidType			GetViewEqipUID( int index )	const		{ return m_ViewEqipItemUIDList[index];			}

		int				GetFashionEqipNum()	const				{ return (int)m_FashionEqipItemUIDList.size();	}
		UidType			GetFashionEqipUID( int index ) const	{ return m_FashionEqipItemUIDList[index];		}

		int				GetNormalEqipNum() const				{ return (int)m_NormalEqipItemUIDList.size();	}
		UidType			GetNormalEqipUID( int index ) const		{ return m_NormalEqipItemUIDList[index];		}

		void			ResetEqip();
		bool			AddEqip( UidType itemUID );
		bool			AddEqip( CX2Item* pItem );
		
		bool			RemoveEqip( UidType itemUID );
		bool			RemoveEqip( CX2Item* pItem );

		bool			IsPossibleAddEqip( UidType itemUID );
		bool			IsPossibleAddEqip( CX2Item* pItem );

		//void			SetUnitData( UnitData*	pUnitData )				{ m_pUnitData = pUnitData;				}
		void			SetUnitData( const KUnitInfo* pUnitInfo )		{ m_UnitData.SetKUnitInfo( *pUnitInfo );	}
		void			SetUnitTemplet( const UnitTemplet* pUnitTemplet )		{ m_pUnitTemplet = pUnitTemplet;		}

		bool			GetIsLevelUp() { return m_bIsLevelUp; }
		void			SetIsLevelUp(bool bCheck) { m_bIsLevelUp = bCheck; }

		int				GetPrevLevel() { return m_PrevLevel; }
		void			SetPrevLevel( int prevLevel ) { m_PrevLevel = prevLevel; }

		int				GetPrevEXP() { return m_PrevEXP; }
		int				GetPrevNowBaseLevelEXP() { return m_PrevNowBaseLevelEXP; }
		int				GetPrevNextBaseLevelEXP() { return m_PrevNextBaseLevelEXP; }

		bool			CheckNowEquipItem( UidType itemUID );
		bool			EqipAbility( int itemID );
		bool			EqipAbility( CX2Item* pItem );

		int				GetLastClearDungeonID();
		bool			IsClearDungeon( int iDungeonID );
		bool			IsClearDungeon( vector<int> &vecDungeonId );

		float			ResetMaxMp( const float fMaxMp_ );

		CX2PVPEmblem::PVP_EMBLEM GetPVPEmblem()	
		{ 
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef PVP_SEASON2
			if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_D )
				return CX2PVPEmblem::PE_RANK_E;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_C )
				return CX2PVPEmblem::PE_RANK_D;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_B )
				return CX2PVPEmblem::PE_RANK_C;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_A )
				return CX2PVPEmblem::PE_RANK_B;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_S )
				return CX2PVPEmblem::PE_RANK_A;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_SS )
				return CX2PVPEmblem::PE_RANK_S;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_SSS )
				return CX2PVPEmblem::PE_RANK_SS;
			else 
				return CX2PVPEmblem::PE_RANK_SSS;
#else
			if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_E )
				return CX2PVPEmblem::PE_RANK_F;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_D )
				return CX2PVPEmblem::PE_RANK_E;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_C )
				return CX2PVPEmblem::PE_RANK_D;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_B )
				return CX2PVPEmblem::PE_RANK_C;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_A )
				return CX2PVPEmblem::PE_RANK_B;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_S )
				return CX2PVPEmblem::PE_RANK_A;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_SS )
				return CX2PVPEmblem::PE_RANK_S;
			else 
				return CX2PVPEmblem::PE_RANK_SS;
#endif
#else
			return m_UnitData.m_PVPEmblem;
#endif
		}

#ifdef SERV_PVP_NEW_SYSTEM
		CX2PVPEmblem::PVP_RANK GetPvpRank()
		{
#ifdef PVP_SEASON2
			char cRank = m_UnitData.m_cRank;
			return CX2PVPEmblem::PVP_RANK( cRank );
#else
			if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_E )
				return CX2PVPEmblem::PVPRANK_RANK_F;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_D )
				return CX2PVPEmblem::PVPRANK_RANK_E;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_C )
				return CX2PVPEmblem::PVPRANK_RANK_D;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_B )
				return CX2PVPEmblem::PVPRANK_RANK_C;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_A )
				return CX2PVPEmblem::PVPRANK_RANK_B;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_S )
				return CX2PVPEmblem::PVPRANK_RANK_A;
			else if( m_UnitData.m_iRating < CX2PVPEmblem::PE_RANK_SS )
				return CX2PVPEmblem::PVPRANK_RANK_S;
			else 
				return CX2PVPEmblem::PVPRANK_RANK_SS;
#endif
		}
#endif

		//{{ 2007. 10. 5  ������  �ټ��� �Լ�
		int				GetSpirit() { return m_UnitData.m_iSpirit; }
		void			SetSpirit( int val ) { m_UnitData.m_iSpirit = val; }
		//}}

#ifdef SERV_CHINA_SPIRIT_EVENT
		int				GetChinaSpiritIndex()
		{
			switch( GetTitleId() )
			{
			case 50120:
				return 0;
			case 50130:
				return 1;
			case 50140:
				return 2;
			case 50150:
				return 3;
			case 50160:
				return 4;
			case 50170:
				return 5;
			default:
				return -1;
			}
		}
#endif SERV_CHINA_SPIRIT_EVENT

		CX2Stat::Stat	GetUnitStat() const;
		CX2Stat::Stat	GetEqipStat() const;
		CX2Stat::Stat	GetSkillStat() const;

		//{{ kimhc // 2009-11-17 //��� ��ų�� ���õ� ����
#ifdef	GUILD_SKILL
		CX2Stat::Stat	GetGuildSkillStat() const;
#endif	GUILD_SKILL
		//}} kimhc // 2009-11-17 //��� ��ų�� ���õ� ����

		//{{ kimhc // �ǽð� ���ҵ� �� ��� �� �ƹ�Ÿ ��ü
#ifdef REAL_TIME_ELSWORD
		bool			IsNormalEquipInThePosition( EQIP_POSITION equipPosition ) const;
		bool			IsFashionEquipInThePosition( EQIP_POSITION equipPosition ) const;
		bool			IsViewEquipInThePosition( EQIP_POSITION equipPosition ) const;
		CX2Item*		CreateBasicEquip( CX2Unit::EQIP_POSITION equipPosition );
#endif REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� ��� �� �ƹ�Ÿ ��ü

		//{{ kimhc // 2010.12.20 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		bool IsBasicEquip( const int iItemID_ );
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.20 //  2010-12-23 New Character CHUNG

		const CX2Stat::EnchantStat& GetEnchantStat( bool bUpdate = true , bool bShowDungeonOptionUI_ = false, bool bShowPvpOptionUI_ = false );

		void AddEchantStatFromSocket( int iSocketOptionID );

		//{{ kimhc // 2011.4.24 // ������� 2�ܰ�, �巡���� ����
#ifdef	SERV_INSERT_GLOBAL_SERVER
		void AddEnchantStatFromDragonBreath( OUT CX2Stat::EnchantStat& enchantStat_ );
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011.4.24 // ������� 2�ܰ�, �巡���� ����

		// ���� ���� ���� �˾�â���� �˷��ֱ����� 
		void 			AddNewlyOpenedDungeon( int iDungeonID ) { m_vecNewlyOpenedDungeon.push_back( iDungeonID ); }
		void 			ClearNewlyOpenedDungeon() { m_vecNewlyOpenedDungeon.clear(); }
		std::vector<int> GetVecNewlyOpenedDungeon() const { return m_vecNewlyOpenedDungeon; }

		void 			AddNewlyOpenedDungeonAtLocalMap( int iDungeonID ) { m_vecNewlyOpenedDungeonAtLocalMap.push_back( iDungeonID ); }
		void 			ClearNewlyOpenedDungeonAtLocalMap() { m_vecNewlyOpenedDungeonAtLocalMap.clear(); }
		std::vector<int> GetVecNewlyOpenedDungeonAtLocalMap() const { return m_vecNewlyOpenedDungeonAtLocalMap; }
		bool			IsNewlyOpenedDungeon( const int iDungeonID )
		{
			for( UINT i=0; i<m_vecNewlyOpenedDungeonAtLocalMap.size(); i++ )
			{
				if( iDungeonID == m_vecNewlyOpenedDungeonAtLocalMap[i] )
				{
					return true;
				}
			}

			return false;
		}

		

		int				GetResurrectionStoneNum() { return m_ResurrectionStoneNum; }
		void			SetResurrectionStoneNum( int quantity ) { m_ResurrectionStoneNum = quantity; }

		//{{ ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
#ifdef AUTO_PAYMENT
		int				GetAutoResStoneNum() { return m_AutoResStoneNum; }
		void			SetAutoResStoneNum( int quantity ) { m_AutoResStoneNum = quantity; }
#endif	//	AUTO_PAYMENT
		//}} ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��

		//{{ 2007. 10. 5  ������  ������ �ȿ��� �� ���� ����
		bool			GetMyInfoInDungeonRoom() { return m_bMyInfoInDungoenRoom; }
		void			SetMyInfoInDungeonRoom( bool val ) { m_bMyInfoInDungoenRoom = val; }
		//}}

		std::map< int, KTCClearInfo >& GetMapTCClear() { return m_mapTCClear; }

		bool AddClearDungeon( int dungeonID );
		void SetBlackList( vector<KChatBlackListUnit>& blackList );
		bool AddBlackList( KChatBlackListUnit blackList );
		bool RemoveBlackList( UidType unitUID );
		const vector<KChatBlackListUnit>& GetBlackList() const;
		UidType GetBlackListUnitUID( const WCHAR* wszNickName );



		CKTDXTimer& GetTimerGuestUser() { return m_timerGuestUser; }
		//{{ kimhc // 2009-12-04 // ũ�������� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
		void AddMarketingEvent( const EventData& eventData )
		{
			if ( m_mapEventData.find( eventData.m_kTimeEventSimpleInfo.m_iEventUID ) == m_mapEventData.end() )
				m_mapEventData.insert( std::make_pair( eventData.m_kTimeEventSimpleInfo.m_iEventUID, eventData ) );
		}

		void DeleteMarketingEvent( int iEventUID )
		{
			if ( m_mapEventData.empty() == false )
				m_mapEventData.erase( iEventUID );
		}

		//const MarketingEventMap& GetKTimeEventSimpleInfoList() { return m_mapKTimeEventSimpleInfo; }		
#else	CHRISTMAS_TREE
		float GetMarketingEventTime() const { return m_fMarketingEventTime; }
		void SetMarketingEventTime(float val) { m_fMarketingEventTime = val; }
		int GetMarketingEventID() const { return m_iMarketingEventID; }
		void SetMarketingEventID(int val) { m_iMarketingEventID = val; }
#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-04 // ũ�������� �̺�Ʈ

		bool GetReceivedGuestUserReward() const { return m_bReceivedGuestUserReward; }
		void SetReceivedGuestUserReward(bool val) { m_bReceivedGuestUserReward = val; }
		bool GetGuestUserReachedLevelLimit() const { return m_bGuestUserReachedLevelLimit; }
		void SetGuestUserReachedLevelLimit(bool val) { m_bGuestUserReachedLevelLimit = val; }
		bool GetGuestUserMovingToCharacterSelect() const { return m_bGuestUserMovingToCharacterSelect; }
		void SetGuestUserMovingToCharacterSelect(bool val) { m_bGuestUserMovingToCharacterSelect = val; }

		void GetSetIDNPartsNum( map<int,int>& mapSetIDNPartsNum );

//#ifdef TITLE_SYSTEM
        int GetTitleId() { return m_UnitData.m_iTitleId; }
        void SetTitleId(int val) { m_UnitData.m_iTitleId = val; }
        void ClearTitle() { m_UnitData.m_iTitleId = 0; }
//#endif


		static bool CanUse( int itemID, CX2Unit* pUnit, int iUnitLevel = -1 );
		static bool CanEquipAsParts( int itemID, CX2Unit* pUnit, int iUnitLevel = -1 );

		float GetRepairDiscountRate();


		void SetStartPos(int val) { m_iStartPos = val; }
		int	GetStartPos() { return m_iStartPos; }

#ifdef UNIT_EMOTION
		const char* GetEmotionName(EMOTION_TYPE eEmotionType);
		wstring GetEmotionAniNameById(EMOTION_TYPE eEmotionType);
#endif

#ifdef SERV_SKILL_NOTE
		void SetSkillNoteMaxPage(char cVal) { m_iMaxSKillNotePage = cVal; }
		void SetSkillNote( std::map<char, int> &mapSkillNote, char iMaxNotePage );
		int GetSkillMemoIdBySlot( char slotId );
		char GetMaxSkillNoteSlot() const { return m_iMaxSKillNotePage; }
		void SetSkillNotePage(char iPage, int iMemo);
#endif


	public:
		//{{ kimhc // 2009-12-04 // ũ�������� �̺�Ʈ
#ifdef	CHRISTMAS_TREE
		typedef std::map< int, EventData > MarketingEventMap;
		MarketingEventMap m_mapEventData;	// pair< EventUID, EventData >
#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-04 // ũ�������� �̺�Ʈ

#ifdef SERV_PET_SYSTEM
		void SetSimplePetInfo( const KFieldPetInfo& kFieldPetInfo_ );
		void SetFullPetInfo( const KPetInfo& kPetInfo_ );
		KPetInfo *GetPetInfo();
		void SetSummonPetUid( UidType petUid ) { m_SummonPetUid = petUid; }
		UidType GetSummonPetUid() { return m_SummonPetUid; }
		void ClearPetInfo() { m_vecPetInfo.clear(); }
#endif
#ifdef PET_DROP_ITEM_PICKUP
		void SetPetAutoLooting( UidType uiPetUID, bool bAutoLooting);
#endif //PET_DROP_ITEM_PICKUP

#ifdef RIDING_SYSTEM
		void SetFullRidingPetInfo( const KRidingPetInfo& pRidingPetInfo_ );
		KRidingPetInfo* GetRidingPetInfo();
		void ClearRidingPetInfo()
		{
			m_vecRidingPetInfo.clear();
		}
#endif //RIDING_SYSTEM

		//{{ kimhc // 2011.1.14 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		void UpdateEnchantStatFromPassiveSkill();	// �нú꽺ų�� ���� �Ӽ� ��ġ ������Ʈ
		const CX2Stat::EnchantStat& GetEnchantStatFromPassiveSkill() { return m_EnchantStatFromPassiveSkill; }
#endif	CHUNG_FIRST_CLASS_CHANGE
		//}} kimhc // 2011.1.14 // û 1�� ����

		bool CheckNowEquipItemByItemId( UidType itemID ); 		
		bool GetAvatarEmotion(wstring &wstrEmotionName, CX2Unit::EMOTION_TYPE &eEmotionID);
		bool GetAvatarEmotionID(CX2Unit::EMOTION_TYPE &eEmotionID);
#ifdef NEW_HENIR_TEST
		void SetHenirRewardCountInfo(KEGS_HENIR_REWARD_COUNT_NOT& kHenirRewardCountInfo){ m_HenirRewardCountInfo = kHenirRewardCountInfo; }
		KEGS_HENIR_REWARD_COUNT_NOT& GetHenirRewardCountInfo() { return m_HenirRewardCountInfo; }
#endif NEW_HENIR_TEST

#ifdef	SERV_EXPAND_QUICK_SLOT
		void		SetExpandQuickSlot(bool bExpandQuickSlot) { m_bExpandQuickSlot = bExpandQuickSlot; }
		bool		IsExpandQuickSlot() const { return m_bExpandQuickSlot; }
#endif  SERV_EXPAND_QUICK_SLOT

		int GetAverageItemlevel() const;
		void			UpdateItemLevel();

#ifdef SET_WORLD_BUFF_AT_RESURRECTION // ����ȯ
		/// �������� �ɾ��ִ� ���� �׸��� ����� ����, ���� ���� ���̵� ����
		vector<BUFF_FACTOR_ID>& GetWorldBuffFactorID() { return m_vecWorldBuffFactorID; }
#else // SET_WORLD_BUFF_AT_RESURRECTION
		vector<CX2BuffFactorPtr>& GetWorldBuffFactorPtr() { return m_vecWorldBuffFactorPtr; }
#endif // SET_WORLD_BUFF_AT_RESURRECTION

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		void		SetUnitCreateDate( wstring wstrUnitCreateDate )			{ m_wstrUnitCreateDate = wstrUnitCreateDate; }
		wstring		GetUnitCreateDate() const					{ return m_wstrUnitCreateDate; }
		void		SetTradeBlockUnit( bool bTradeBlockUnit )				{ m_bTradeBlockUnit = bTradeBlockUnit; }
		bool		IsTradeBlockUnit() const					{ return m_bTradeBlockUnit; }
		void		SetTradeBlockChatMsg( wstring strTradeBlockChatMsg )	{ m_strTradeBlockChatMsg = strTradeBlockChatMsg; }
		wstring		GetTradeBlockChatMsg() const				{ return m_strTradeBlockChatMsg; }
		void		SetNewUnitTradeBlockDay( int iDay )						{ m_iNewUnitTradeBlockDay = iDay; }
		int			GetNewUnitTradeBlockDay() const				{ return m_iNewUnitTradeBlockDay; }
		void		SetNewUnitTradeBlockUnitClass( int iUnitClass )			{ m_iNewUnitTradeBlockUnitClass = iUnitClass; }
		int			GetNewUnitTradeBlockUnitClass() const		{ return m_iNewUnitTradeBlockUnitClass; }
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-16
		int GetRecentEnterDungeonID() const { return m_iRecentEnterDungeonID; }
		void SetRecentEnterDungeonID(int val) { m_iRecentEnterDungeonID = val; }
#endif // SERV_NEW_DEFENCE_DUNGEON

		
#ifdef ADDED_RELATIONSHIP_SYSTEM
		bool IsManCharacter (); const			//���� ĳ�������� ���� ĳ�������� Ȯ��
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_RELATIONSHIP_SYSTEM
		char GetWeddingStatus() const { return m_UnitData.m_cWeddingStatus; }
		UidType GetLoverUnitUID() const { return m_UnitData.m_iLoverUnitUID; }
		void SetLoverUnitUID( UidType UID_ ) { m_UnitData.m_iLoverUnitUID = UID_; }
#endif //SERV_RELATIONSHIP_SYSTEM

		static bool IsAvatarEmotion( const CX2Unit::EMOTION_TYPE eEmotioID_ )
		{
			switch ( eEmotioID_ )
			{
			case ET_EMOTION_AVATAR1:
			case ET_EMOTION_AVATAR2:
			case ET_EMOTION_AVATAR3:
			case ET_EMOTION_AVATAR4:
			case ET_EMOTION_AVATAR5:
#ifdef CRAYONPOP_SECOND_EMOTION
			case ET_EMOTION_AVATAR6:
#endif // CRAYONPOP_SECOND_EMOTION
#ifdef CRAYONPOP_EMOTION_WITH_MUSIC		// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�
			case ET_EMOTION_AVATAR7:
#endif // CRAYONPOP_EMOTION_WITH_MUSIC	// ũ���� �� �ѹ� �ƹ�Ÿ �̸��, ���尡 ��µ�

				return true;
				break;

			default:
				return false;
				break;
			}
		}

#ifdef UPGRADE_SKILL_SYSTEM_2013 // ����ȯ

		void ResetIncreaseSkillLevelByBuff();								/// ������ ���� ��ų ���� ���� ȿ�� �缳�� �Լ�
		void ResetIncreaseSkillLevelBySocket();	/// �������� ���� ��ų ���� ���� ȿ�� �缳�� �Լ�

		int GetIncreaseSkillLevelByBuff();
		int GetIncreaseSkillLevelBySocket( CX2SkillTree::SKILL_ID eSkillID );

#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		void					SetServerGroupID ( const SEnum::SERVER_GROUP_ID eServerGroupID ) { m_eServerGroupID = eServerGroupID;  }
		SEnum::SERVER_GROUP_ID	GetServerGroupID () const 
		{ 
#ifdef FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
			if( SEnum::SGI_END <= m_eServerGroupID)
			{
				return SEnum::SGI_SOLES;
			}
			else
				return m_eServerGroupID;	
#else
			return m_eServerGroupID;
#endif // FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR	
		}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		wstring		GetLocalMapPlayTimesDesc( int iDungeonID );
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

#ifdef SERV_NEW_YEAR_EVENT_2014
		u_char		GetOldYearMissionRewardedLevel() const { return m_UnitData.m_ucOldYearMissionRewardedLevel; }
		void		SetOldYearMissionRewardedLevel( u_char ucOldYearMissionRewardedLevel ) { m_UnitData.m_ucOldYearMissionRewardedLevel = ucOldYearMissionRewardedLevel; }
		int			GetNewYearMissionStepID() const { return m_UnitData.m_iNewYearMissionStepID; }
		void		SetNewYearMissionStepID( int iNewYearMissionStepID ) { m_UnitData.m_iNewYearMissionStepID = iNewYearMissionStepID; }
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_CHECK_POWER
		bool		IsShowCheckPowerPopUp() const { return m_UnitData.m_bCheckPowerShowPopUp; }
		void		SetShowCheckPowerPopUp( bool bCheckPowerShowPopUp ) { m_UnitData.m_bCheckPowerShowPopUp = bCheckPowerShowPopUp; }

		unsigned char	GetCheckPowerCount() const { return m_UnitData.m_ucCheckPowerCount; }
		void			SetCheckPowerCount( unsigned char ucCount ) { m_UnitData.m_ucCheckPowerCount = ucCount; }

		unsigned char	GetCheckPowerScore() const { return m_UnitData.m_ucCheckPowerScore; }
		void			SetCheckPowerScore( unsigned char ucScore ) { m_UnitData.m_ucCheckPowerScore = ucScore; }

		__int64		GetCheckPowerTime() const { return m_UnitData.m_iCheckPowerTime; }
		void		SetCheckPowerTime( __int64 iTime ) { m_UnitData.m_iCheckPowerTime = iTime; }
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
		bool		IsInfinityElEssence() const;
		float		GetSkillCoolTimeDecreaseRate(CX2SkillTree::SKILL_ID eSkillID, CX2SkillTree::SKILL_TYPE eSkillType) const;
		float		GetSkillMpDecreaseRate(CX2SkillTree::SKILL_ID eSkillID, CX2SkillTree::SKILL_TYPE eSkillType) const;
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

	protected:		
		bool			UpdateViewEqipPosition();
		bool			UpdateFashionEqipPosition();
		bool			UpdateNormalEqipPosition();
		void			UpdateEqipPosition( CX2Item* pItem, bool bAdd );
		
		bool			EqipConflict( CX2Item* pItem );
		bool			UpdateViewEqip();

		void			AddItemLevel( IN const CX2Item* pEquipItem_ );

		int GetSumOfItemLevel() const { return m_iSumOfItemLevel; }
		void SetSumOfItemLevel( int iSumOfItemLevel_ ) { m_iSumOfItemLevel = iSumOfItemLevel_; }


		bool			IsShowDungeonOnly( bool bDungeonOnlyItem_, bool bShowDungeonOptionUI_ = false );
		bool			IsShowPvpOnly( bool bPvpOnlyItem_, bool bShowPvpOptionUI_ = false );
		
		//UnitData*					m_pUnitData;
        UnitData					m_UnitData;
		const UnitTemplet*			m_pUnitTemplet;

		bool						m_bIsLevelUp;

		int							m_PrevLevel;

		int							m_PrevEXP;
		int							m_PrevNowBaseLevelEXP;
		int							m_PrevNextBaseLevelEXP;

		bool						m_ViewEqipPosition[EP_END];
		bool						m_FashionEqipPosition[EP_END];
		bool						m_NormalEqipPosition[EP_END];
		
		vector<UidType>				m_ViewEqipItemUIDList;
		vector<UidType>				m_FashionEqipItemUIDList;
		vector<UidType>				m_NormalEqipItemUIDList;




		// Dungeon Clear
		std::map< int, KDungeonClearInfo >	m_mapDungeonClear;
		std::map< int, KTCClearInfo >		m_mapTCClear;				//		�Ʒü�
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		std::map< int, KDungeonPlayInfo >	m_mapDungeonPlay;
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
		
		int									m_iSumOfItemLevel;		/// ������ ��� ����
		

		// ���� ���� ���� �˾�â���� �˷��ֱ����� 
		std::vector<int>			m_vecNewlyOpenedDungeon;				//�濡��
		std::vector<int>			m_vecNewlyOpenedDungeonAtLocalMap;		//���øʿ���

		int							m_ResurrectionStoneNum;

		//{{ ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��
#ifdef AUTO_PAYMENT
		int							m_AutoResStoneNum;
#endif	//	AUTO_PAYMENT
		//}} ����� : [2009/10/14] //	�ڵ� ���� ��Ȱ��

		//{{ 2007. 10. 5  ������  �����뿡�� ������ ���� ����â �ȶ���
		bool						m_bMyInfoInDungoenRoom;
		//}}

		// ü�� ���̵�ó�� ������ �����ð��Ŀ� ������ �����ϴ� ������ �̺�Ʈ ����

		//{{ kimhc // 2009-12-04 // ũ�������� �̺�Ʈ
#ifndef	CHRISTMAS_TREE
		int							m_iMarketingEventID;
		float						m_fMarketingEventTime;
#endif	CHRISTMAS_TREE
		//}} kimhc // 2009-12-04 // ũ�������� �̺�Ʈ
		
		CKTDXTimer					m_timerGuestUser;
		bool						m_bReceivedGuestUserReward;
		bool						m_bGuestUserReachedLevelLimit;
		bool						m_bGuestUserMovingToCharacterSelect;


		CX2Stat::EnchantStat		m_EnchantStat;				// UI�� �ӽ� ����

//{{ kimhc // 2011.1.14 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		CX2Stat::EnchantStat		m_EnchantStatFromPassiveSkill;	// �нú� ��ų � ���� �����Ǵ� EnchantStat
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.14 // û 1�� ����

		int							m_iStartPos;
		
#ifdef SERV_SKILL_NOTE
		std::map<char, int>		m_mapSkillNote;
		char						m_iMaxSKillNotePage;
#endif

#ifdef SERV_PET_SYSTEM
		std::vector<KPetInfo>	m_vecPetInfo;
		UidType m_SummonPetUid;
#endif

#ifdef RIDING_SYSTEM
		std::vector<KRidingPetInfo> m_vecRidingPetInfo;
#endif //RIDING_SYSTEM

#ifdef NEW_HENIR_TEST
		KEGS_HENIR_REWARD_COUNT_NOT	m_HenirRewardCountInfo;
#endif NEW_HENIR_TEST

#ifdef	SERV_EXPAND_QUICK_SLOT
		bool				m_bExpandQuickSlot;
#endif  SERV_EXPAND_QUICK_SLOT

#ifdef SET_WORLD_BUFF_AT_RESURRECTION // ����ȯ
		/// ���� ��Ȱ�� �ش� �������� �ٽ� ������ ��� �ϱ� ������, ���� ���̵� ����
		vector<BUFF_FACTOR_ID>		m_vecWorldBuffFactorID;			/// ���� ���� �����̳�
#else // SET_WORLD_BUFF_AT_RESURRECTION
		vector<CX2BuffFactorPtr>	m_vecWorldBuffFactorPtr;		/// ���� ���� �����̳�
#endif // SET_WORLD_BUFF_AT_RESURRECTION

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		wstring				m_wstrUnitCreateDate;
		bool				m_bTradeBlockUnit;
		wstring				m_strTradeBlockChatMsg;
		int					m_iNewUnitTradeBlockUnitClass;
		int					m_iNewUnitTradeBlockDay;
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-16
		int					m_iRecentEnterDungeonID;	/// ���� �ֱٿ� ������ ���� ���̵�
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh
		SEnum::SERVER_GROUP_ID		m_eServerGroupID;				/// ���� ���� ����, ���� �׷� ID �߰�
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		int					m_iGateOfDarknessSupportEventTime;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
};

inline bool GetX2UnitClassCompatibility( CX2Unit::UNIT_CLASS unitClass, CX2Unit::UNIT_CLASS stuffClass )
{
	// note!! ���� �߰��Ǹ� ����
	switch( unitClass )
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:
		{
			if( stuffClass == CX2Unit::UC_ELSWORD_SWORDMAN )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_ARME_VIOLET_MAGE:
		{
			if( stuffClass == CX2Unit::UC_ARME_VIOLET_MAGE )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_LIRE_ELVEN_RANGER:
		{
			if( stuffClass == CX2Unit::UC_LIRE_ELVEN_RANGER )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_RAVEN_FIGHTER:
		{
			if( stuffClass == CX2Unit::UC_RAVEN_FIGHTER )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_ELSWORD_KNIGHT:
		{
			if( stuffClass == CX2Unit::UC_ELSWORD_SWORDMAN
				|| stuffClass == CX2Unit::UC_ELSWORD_KNIGHT )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
		{
			if( stuffClass == CX2Unit::UC_ELSWORD_SWORDMAN
				|| stuffClass == CX2Unit::UC_ELSWORD_MAGIC_KNIGHT )
			{
				return true;
			}
		}
		break;
#ifdef ELSWORD_SHEATH_KNIGHT
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
		{
			if( stuffClass == CX2Unit::UC_ELSWORD_SWORDMAN
				|| stuffClass == CX2Unit::UC_ELSWORD_SHEATH_KNIGHT )
			{
				return true;
			}
		}
		break;
#endif ELSWORD_SHEATH_KNIGHT

	case CX2Unit::UC_LIRE_COMBAT_RANGER:
		{
			if( stuffClass == CX2Unit::UC_LIRE_ELVEN_RANGER
				|| stuffClass == CX2Unit::UC_LIRE_COMBAT_RANGER )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_LIRE_SNIPING_RANGER:
		{
			if( stuffClass == CX2Unit::UC_LIRE_ELVEN_RANGER
				|| stuffClass == CX2Unit::UC_LIRE_SNIPING_RANGER )
			{
				return true;
			}
		}
		break;

#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
		{
			if( stuffClass == CX2Unit::UC_LIRE_ELVEN_RANGER
				|| stuffClass == CX2Unit::UC_LIRE_TRAPPING_RANGER )
			{
				return true;
			}
		}
		break;
#endif	SERV_TRAPPING_RANGER_TEST

	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
		{
			if( stuffClass == CX2Unit::UC_ARME_VIOLET_MAGE
				|| stuffClass == CX2Unit::UC_ARME_HIGH_MAGICIAN )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_ARME_DARK_MAGICIAN:
		{
			if( stuffClass == CX2Unit::UC_ARME_VIOLET_MAGE
				|| stuffClass == CX2Unit::UC_ARME_DARK_MAGICIAN )
			{
				return true;
			}
		}
		break;

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
		{
			if( stuffClass == CX2Unit::UC_ARME_VIOLET_MAGE
				|| stuffClass == CX2Unit::UC_ARME_BATTLE_MAGICIAN )
			{
				return true;
			}
		}
		break;
#endif

	case CX2Unit::UC_RAVEN_SOUL_TAKER:
		{
			if( stuffClass == CX2Unit::UC_RAVEN_FIGHTER
				|| stuffClass == CX2Unit::UC_RAVEN_SOUL_TAKER )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_RAVEN_OVER_TAKER:
		{
			if( stuffClass == CX2Unit::UC_RAVEN_FIGHTER
				|| stuffClass == CX2Unit::UC_RAVEN_OVER_TAKER )
			{
				return true;
			}
		}
		break;

#ifdef RAVEN_WEAPON_TAKER
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
		{
			if( stuffClass == CX2Unit::UC_RAVEN_FIGHTER
				|| stuffClass == CX2Unit::UC_RAVEN_WEAPON_TAKER )
			{
				return true;
			}
		}
		break;
#endif

	case CX2Unit::UC_EVE_NASOD:
		{
			if( stuffClass == CX2Unit::UC_EVE_NASOD )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_EVE_EXOTIC_GEAR:
		{
			if( stuffClass == CX2Unit::UC_EVE_NASOD || 
				stuffClass == CX2Unit::UC_EVE_EXOTIC_GEAR )
			{
				return true;
			}
		}
		break;

	case CX2Unit::UC_EVE_ARCHITECTURE:
		{
			if( stuffClass == CX2Unit::UC_EVE_NASOD || 
				stuffClass == CX2Unit::UC_EVE_ARCHITECTURE )
			{
				return true;
			}
		}
		break;
#ifdef EVE_ELECTRA
	case CX2Unit::UC_EVE_ELECTRA:
		{
			if( stuffClass == CX2Unit::UC_EVE_NASOD
				|| stuffClass == CX2Unit::UC_EVE_ELECTRA )
			{
				return true;
			}
		}
		break;
#endif EVE_ELECTRA

	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ELSWORD_SWORDMAN:
			case CX2Unit::UC_ELSWORD_KNIGHT:
			case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
				{
					return true;
				} break;
			}
		} break;


	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ELSWORD_SWORDMAN:
			case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
			case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
				{
					return true;
				} break;
			}
		} break;
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ELSWORD_SWORDMAN:
			case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
			case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
				{
					return true;
				} break;
			}
		} break;
#endif




#ifdef RENA_SECOND_CLASS_CHANGE
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_LIRE_ELVEN_RANGER:
			case CX2Unit::UC_LIRE_COMBAT_RANGER:
			case CX2Unit::UC_LIRE_WIND_SNEAKER:
				{
					return true;
				} break;
			}
		} break;


	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_LIRE_ELVEN_RANGER:
			case CX2Unit::UC_LIRE_SNIPING_RANGER:
			case CX2Unit::UC_LIRE_GRAND_ARCHER:
				{
					return true;
				} break;
			}
		} break;
#endif RENA_SECOND_CLASS_CHANGE
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_LIRE_ELVEN_RANGER:
			case CX2Unit::UC_LIRE_TRAPPING_RANGER:
			case CX2Unit::UC_LIRE_NIGHT_WATCHER:
				{
					return true;
				} break;
			}
		} break;
#endif SERV_RENA_NIGHT_WATCHER



	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ARME_VIOLET_MAGE:
			case CX2Unit::UC_ARME_HIGH_MAGICIAN:
			case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
				{
					return true;
				} break;
			}
		} break;

	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ARME_VIOLET_MAGE:
			case CX2Unit::UC_ARME_DARK_MAGICIAN:
			case CX2Unit::UC_ARME_VOID_PRINCESS:
				{
					return true;
				} break;
			}
		} break;
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ARME_VIOLET_MAGE:
			case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
			case CX2Unit::UC_ARME_DIMENSION_WITCH:
				{
					return true;
				} break;
			}
		} break;
#endif

#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_RAVEN_FIGHTER:
			case CX2Unit::UC_RAVEN_SOUL_TAKER:
			case CX2Unit::UC_RAVEN_BLADE_MASTER:
				{
					return true;
				} break;
			}
		} break;

	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_RAVEN_FIGHTER:
			case CX2Unit::UC_RAVEN_OVER_TAKER:
			case CX2Unit::UC_RAVEN_RECKLESS_FIST:
				{
					return true;
				} break;
			}
		} break;
#endif
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_RAVEN_FIGHTER:
			case CX2Unit::UC_RAVEN_WEAPON_TAKER:
			case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
				{
					return true;
				} break;
			}
		} break;
#endif SERV_RAVEN_VETERAN_COMMANDER

	//{{ kimhc // 2010.1.22 //
#ifdef EVE_SECOND_CLASS_CHANGE
	case CX2Unit::UC_EVE_CODE_NEMESIS:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_EVE_NASOD:
			case CX2Unit::UC_EVE_EXOTIC_GEAR:
			case CX2Unit::UC_EVE_CODE_NEMESIS:
				{
					return true;
				} break;
			}
		}
		break;

	case CX2Unit::UC_EVE_CODE_EMPRESS:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_EVE_NASOD:
			case CX2Unit::UC_EVE_ARCHITECTURE:
			case CX2Unit::UC_EVE_CODE_EMPRESS:
				{
					return true;
				} break;
			}
		}
		break;
#endif EVE_SECOND_CLASS_CHANGE
#ifdef SERV_EVE_BATTLE_SERAPH
	case CX2Unit::UC_EVE_BATTLE_SERAPH:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_EVE_NASOD:
			case CX2Unit::UC_EVE_ELECTRA:
			case CX2Unit::UC_EVE_BATTLE_SERAPH:
				{
					return true;
				} break;
			}
		}
		break;
#endif
//}} kimhc // 2010.1.22 //

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UC_CHUNG_IRON_CANNON:
		{
			if( CX2Unit::UC_CHUNG_IRON_CANNON == stuffClass )
			{
				return true;
			}
		}
		break;

		//{{ kimhc // 2011.1.3 // û 1�� ����
	#ifdef	CHUNG_FIRST_CLASS_CHANGE

	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_CHUNG_IRON_CANNON:
			case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
				{
					return true;
				} break;
			} // switch
			
		} break;

	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_CHUNG_IRON_CANNON:
			case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
				{
					return true;
				} break;
			} // switch
		} break;
	#endif	CHUNG_FIRST_CLASS_CHANGE
		//}} kimhc // 2011.1.3 // û 1�� ����

#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_CHUNG_IRON_CANNON:
			case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
				{
					return true;
				} break;
			} // switch
		} break;

#endif

		//{{ JHKang / ������ / 2011.6.13 / û 2�� ����
#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_IRON_PALADIN:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_CHUNG_IRON_CANNON:
			case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
			case CX2Unit::UC_CHUNG_IRON_PALADIN:
				{
					return true;
				} break;
			}
		}
		break;

	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_CHUNG_IRON_CANNON:
			case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
			case CX2Unit::UC_CHUNG_DEADLY_CHASER:
				{
					return true;
				} break;
			}
		}
		break;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_CHUNG_IRON_CANNON:
			case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
			case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
				{
					return true;
				} break;
			}
		}
		break;
#endif
			//}}

#pragma region �ƶ� ���� ����
#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:
		{
			if( CX2Unit::UC_ARA_MARTIAL_ARTIST == stuffClass )
			{
				return true;
			}
		}
		break;
#endif
		
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_LITTLE_HSIEN:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_ARA_MARTIAL_ARTIST:
			case CX2Unit::UC_ARA_LITTLE_HSIEN:
				{
					return true;
				} break;
			} // switch
		} break;

	case CX2Unit::UC_ARA_SAKRA_DEVANAM:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_ARA_MARTIAL_ARTIST:
			case CX2Unit::UC_ARA_LITTLE_HSIEN:
			case CX2Unit::UC_ARA_SAKRA_DEVANAM:
				{
					return true;
				} break;
			}
		}
		break;
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
	case CX2Unit::UC_ARA_LITTLE_DEVIL:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_ARA_MARTIAL_ARTIST:
			case CX2Unit::UC_ARA_LITTLE_DEVIL:
				{
					return true;
				} break;
			} // switch
		} break;

	case CX2Unit::UC_ARA_YAMA_RAJA:
		{
			switch ( stuffClass )
			{
			case CX2Unit::UC_ARA_MARTIAL_ARTIST:
			case CX2Unit::UC_ARA_LITTLE_DEVIL:
			case CX2Unit::UC_ARA_YAMA_RAJA:
				{
					return true;
				} break;
			}
		}
		break;
#endif //SERV_ARA_CHANGE_CLASS_SECOND
#pragma endregion 

#ifdef NEW_CHARACTER_EL
	case CX2Unit::UC_ELESIS_KNIGHT:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ELESIS_KNIGHT:
				return true;
				break;
			default:
				break;
			}
		} break;
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ELESIS_KNIGHT:
			case CX2Unit::UC_ELESIS_SABER_KNIGHT:
				return true;
				break;
			default:
				break;
			}
		} break;
	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ELESIS_KNIGHT:
			case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
				return true;
				break;
			default:
				break;
			}
		} break;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ
	case CX2Unit::UC_ELESIS_GRAND_MASTER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ELESIS_KNIGHT:
			case CX2Unit::UC_ELESIS_SABER_KNIGHT:
			case CX2Unit::UC_ELESIS_GRAND_MASTER:
				return true;
				break;
			default:
				break;
			}
		} break;
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ELESIS_KNIGHT:
			case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
			case CX2Unit::UC_ELESIS_BLAZING_HEART :
				return true;
				break;
			default:
				break;
			}
		} break;

#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // ������, �����ý� 1-2 �׷��� ������, 2-2 ����¡ ��Ʈ

#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ ( ĳ���� �߰��� )
	case CX2Unit::UC_ADD_NASOD_RULER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ADD_NASOD_RULER:
				return true;
				break;
			default:
				break;
			}
		} break;

	case CX2Unit::UC_ADD_PSYCHIC_TRACER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ADD_NASOD_RULER:
			case CX2Unit::UC_ADD_PSYCHIC_TRACER:
				return true;
				break;
			default:
				break;
			}
		} break;

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
	case CX2Unit::UC_ADD_LUNATIC_PSYKER:
		{
			switch( stuffClass )
			{
			case CX2Unit::UC_ADD_NASOD_RULER:
			case CX2Unit::UC_ADD_PSYCHIC_TRACER:
			case CX2Unit::UC_ADD_LUNATIC_PSYKER:
				return true;
				break;
			default:
				break;
			}
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER

#endif //SERV_9TH_NEW_CHARACTER

	}

	return false;
}

typedef CX2Unit::EQIP_POSITION  CX2UnitEQIP_POSITION;

#ifdef SEPARATION_MOTION
inline int GetX2UnitClassGrade( CX2Unit::UNIT_CLASS unitClass )
{
	int iGrade = (int)unitClass / 10;

	if( iGrade == 0)
		return 0;

	if( iGrade <= 9 )
		return 1;

	if( iGrade <= 99 )
		return 2;

	return -1;
}
#endif

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, ���� ���� ����, kimjh

inline CX2Unit::UNIT_CLASS GetFirstLineElswordClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ELSWORD_SWORDMAN;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ELSWORD_KNIGHT;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ELSWORD_LORD_KNIGHT;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Class Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineElswordClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ELSWORD_SWORDMAN;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ELSWORD_MAGIC_KNIGHT;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ELSWORD_RUNE_SLAYER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Class Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineElswordClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ELSWORD_SWORDMAN;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ELSWORD_SHEATH_KNIGHT;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ELSWORD_INFINITY_SWORD;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Class Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetElswordClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineElswordClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineElswordClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineElswordClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}


inline CX2Unit::UNIT_CLASS GetFirstLineArmeClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ARME_VIOLET_MAGE;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ARME_HIGH_MAGICIAN;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ARME_ELEMENTAL_MASTER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineArmeClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ARME_VIOLET_MAGE;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ARME_DARK_MAGICIAN;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ARME_VOID_PRINCESS;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineArmeClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ARME_VIOLET_MAGE;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ARME_BATTLE_MAGICIAN;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ARME_DIMENSION_WITCH;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetArmeClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineArmeClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineArmeClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineArmeClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}


inline CX2Unit::UNIT_CLASS GetFirstLineLireClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_LIRE_ELVEN_RANGER;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_LIRE_COMBAT_RANGER;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_LIRE_WIND_SNEAKER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineLireClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_LIRE_ELVEN_RANGER;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_LIRE_SNIPING_RANGER;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_LIRE_GRAND_ARCHER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineLireClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_LIRE_ELVEN_RANGER;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_LIRE_TRAPPING_RANGER;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_LIRE_NIGHT_WATCHER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetLireClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineLireClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineLireClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineLireClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}


inline CX2Unit::UNIT_CLASS GetFirstLineRavenClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_RAVEN_FIGHTER;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_RAVEN_SOUL_TAKER;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_RAVEN_BLADE_MASTER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineRavenClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_RAVEN_FIGHTER;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_RAVEN_OVER_TAKER;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_RAVEN_RECKLESS_FIST;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineRavenClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_RAVEN_FIGHTER;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_RAVEN_WEAPON_TAKER;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_RAVEN_VETERAN_COMMANDER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetRavenClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineRavenClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineRavenClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineRavenClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}


inline CX2Unit::UNIT_CLASS GetFirstLineEveClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_EVE_NASOD;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_EVE_EXOTIC_GEAR;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_EVE_CODE_NEMESIS;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineEveClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_EVE_NASOD;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_EVE_ARCHITECTURE;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_EVE_CODE_EMPRESS;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineEveClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_EVE_NASOD;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_EVE_ELECTRA;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_EVE_BATTLE_SERAPH;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetEveClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineEveClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineEveClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineEveClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}

#ifdef NEW_CHARACTER_CHUNG

inline CX2Unit::UNIT_CLASS GetFirstLineChungClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_CHUNG_IRON_CANNON;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_CHUNG_FURY_GUARDIAN;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_CHUNG_IRON_PALADIN;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineChungClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_CHUNG_IRON_CANNON;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_CHUNG_DEADLY_CHASER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineChungClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_CHUNG_IRON_CANNON;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_CHUNG_SHELLING_GUARDIAN;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_CHUNG_TACTICAL_TROOPER;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetChungClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineChungClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineChungClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineChungClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}

#endif // NEW_CHARACTER_CHUNG


#ifdef ARA_CHARACTER_BASE

inline CX2Unit::UNIT_CLASS GetFirstLineAraClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ARA_MARTIAL_ARTIST;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ARA_LITTLE_HSIEN;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ARA_SAKRA_DEVANAM;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineAraClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ARA_MARTIAL_ARTIST;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ARA_LITTLE_DEVIL;
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ARA_YAMA_RAJA;
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineAraClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{	
	case CX2Unit::UCT_BASIC_CLASS :
	case CX2Unit::UCT_FIRST_CLASS :
	case CX2Unit::UCT_SECOND_CLASS :
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetAraClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineAraClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineAraClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineAraClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}

#endif // #ifdef ARA_CHARACTER_BASE


#ifdef NEW_CHARACTER_EL

inline CX2Unit::UNIT_CLASS GetFirstLineElesisClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ELESIS_KNIGHT;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ELESIS_SABER_KNIGHT;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ELESIS_GRAND_MASTER;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
		return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineElesisClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ELESIS_KNIGHT;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ELESIS_PYRO_KNIGHT;
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE
	case CX2Unit::UCT_SECOND_CLASS :
		return CX2Unit::UC_ELESIS_BLAZING_HEART;
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineElesisClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
	case CX2Unit::UCT_FIRST_CLASS :
	case CX2Unit::UCT_SECOND_CLASS :
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetElesisClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineElesisClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineElesisClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineElesisClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}

#endif // NEW_CHARACTER_EL


#ifdef SERV_9TH_NEW_CHARACTER

inline CX2Unit::UNIT_CLASS GetFirstLineNewCharacterClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
		return CX2Unit::UC_ADD_NASOD_RULER;
	case CX2Unit::UCT_FIRST_CLASS :
		return CX2Unit::UC_ADD_PSYCHIC_TRACER;
	case CX2Unit::UCT_SECOND_CLASS :
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		return CX2Unit::UC_ADD_LUNATIC_PSYKER;
#endif //SERV_ADD_LUNATIC_PSYKER
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetSecondLineNewCharacterClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
	case CX2Unit::UCT_FIRST_CLASS :
	case CX2Unit::UCT_SECOND_CLASS :
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetThirdLineNewCharacterClassTypeByUnitClassType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_ )
{
	switch ( eUnitClassType_ )
	{
	case CX2Unit::UCT_BASIC_CLASS :
	case CX2Unit::UCT_FIRST_CLASS :
	case CX2Unit::UCT_SECOND_CLASS :
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}

inline CX2Unit::UNIT_CLASS GetNewCharacterClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitClassLine_ )
	{
	case CX2Unit::UCL_FIRST :
		return GetFirstLineNewCharacterClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_SECOND :
		return GetSecondLineNewCharacterClassTypeByUnitClassType ( eUnitClassType_ );
	case CX2Unit::UCL_THIRD :
		return GetThirdLineNewCharacterClassTypeByUnitClassType ( eUnitClassType_ );
	default:
		break;
	}
	ASSERT ( !L"Wrong Unit LINE Type" );
	return CX2Unit::UC_NONE;
}

#endif //  SERV_9TH_NEW_CHARACTER

inline CX2Unit::UNIT_CLASS GetUnitClassByUnitClassTypeAndLineType ( CX2Unit::UNIT_TYPE eUnitType_, CX2Unit::UNIT_CLASS_TYPE eUnitClassType_, CX2Unit::UNIT_CLASS_LINE eUnitClassLine_ )
{
	switch ( eUnitType_ )
	{
	case CX2Unit::UT_ELSWORD :
		return GetElswordClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );
	case CX2Unit::UT_ARME :
		return GetArmeClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );
	case CX2Unit::UT_LIRE :
		return GetLireClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );
	case CX2Unit::UT_RAVEN :
		return GetRavenClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );
	case CX2Unit::UT_EVE :
		return GetEveClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );
#ifdef NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG :
		return GetChungClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );
#endif // NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		return GetAraClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );
#endif // ARA_CHARACTER_BASE
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UT_ELESIS :
		return GetElesisClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER
	case CX2Unit::UT_ADD :
		return GetNewCharacterClassByUnitClassTypeAndLineType ( eUnitClassType_, eUnitClassLine_ );	
#endif // SERV_9TH_NEW_CHARACTER
	default :
		break;
	}

	ASSERT ( !L"Wrong Unit Type" );
	return CX2Unit::UC_NONE;
}


inline D3DXCOLOR GetUnitKeyColorByUnitType ( CX2Unit::UNIT_TYPE eUnitType_ )
{
	switch ( eUnitType_ )
	{
	case CX2Unit::UT_ELSWORD :
		return D3DXCOLOR ( 1.000f, 0.000f, 0.000f, 1.0f );
	case CX2Unit::UT_ARME :
		return D3DXCOLOR ( 0.666f, 0.254f, 0.862f, 1.0f );
	case CX2Unit::UT_LIRE :
		return D3DXCOLOR ( 0.470f, 0.803f, 0.009f, 1.0f );
	case CX2Unit::UT_RAVEN :
		return D3DXCOLOR ( 0.941f, 0.823f, 0.745f, 1.0f );
	case CX2Unit::UT_EVE :
		return D3DXCOLOR ( 1.000f, 0.411f, 0.980f, 1.0f );
#ifdef NEW_CHARACTER_CHUNG
	case CX2Unit::UT_CHUNG :
		return D3DXCOLOR ( 0.294f, 0.843f, 0.901f, 1.0f );
#endif // NEW_CHARACTER_CHUNG
#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UT_ARA:
		return D3DXCOLOR ( 1.000f, 0.549f, 0.156f, 1.0f );
#endif // ARA_CHARACTER_BASE
#ifdef NEW_CHARACTER_EL
	case CX2Unit::UT_ELESIS :
		return D3DXCOLOR ( 1.000f, 0.117f, 0.215f, 1.0f );
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER
	case CX2Unit::UT_ADD :
		return D3DXCOLOR ( 0.647f, 0.588f, 1.0f, 1.0f );
#endif // SERV_9TH_NEW_CHARACTER
	default :
		break;
	}

	ASSERT ( !L"Wrong Unit Type" );
	return D3DXCOLOR ( 0.0f, 0.0f, 0.0f, 0.0f );
}
#endif // REFORM_ENTRY_POINT	// 13-11-11, ���� ���� ����, kimjh