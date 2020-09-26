#pragma once

class CX2PVPGame : public CX2Game
{
	public: 
		struct PVPResultUnitInfo
		{
			UidType					m_UnitUid;

			int                     m_Rank;
			int                     m_KillNum;
			int                     m_MDKillNum;
			int                     m_DieNum;									
			int						m_EXP;

			bool					m_bIsMVP;

#ifdef SERV_PVP_NEW_SYSTEM
			int						m_iRating;
			int						m_iRPoint;
			int						m_iAPoint;
#else
			int                     m_PVPPoint;
#endif

			PVPResultUnitInfo()
			{
				m_UnitUid		= 0;

				m_Rank			= 0;
				m_KillNum		= 0;
				m_MDKillNum		= 0;
				m_DieNum		= 0;				
				m_EXP			= 0;

				m_bIsMVP		= false;

#ifdef SERV_PVP_NEW_SYSTEM
				m_iRating		= 0;
				m_iRPoint		= 0;
				m_iAPoint		= 0;
#else
				m_PVPPoint		= 0;
#endif
			}

			void Set_KPVPUnitResultInfo( KPVPUnitResultInfo& kPVPUnitInfo )
			{
				m_UnitUid		= kPVPUnitInfo.m_UnitUID;

				m_Rank			= kPVPUnitInfo.m_nRanking;
				m_KillNum		= kPVPUnitInfo.m_nKillNum;
				m_MDKillNum		= kPVPUnitInfo.m_nMDKillNum;
				m_DieNum		= kPVPUnitInfo.m_nDeathNum;
				m_EXP			= kPVPUnitInfo.m_nEXP;

				m_bIsMVP		= kPVPUnitInfo.m_bIsMVP;

#ifdef SERV_PVP_NEW_SYSTEM
				m_iRating		= kPVPUnitInfo.m_iRating;
				m_iRPoint		= kPVPUnitInfo.m_iRPoint;
				m_iAPoint		= kPVPUnitInfo.m_iAPoint;
#else
				m_PVPPoint		= kPVPUnitInfo.m_nVSPoint;
#endif

			}
		};

		struct PVPResultInfo
		{
			int		m_WinTeam;
			bool	m_bIsDrawn;
			vector< PVPResultUnitInfo* > m_PVPResultUnitInfoList;

#ifdef SERV_PVP_NEW_SYSTEM
			bool	m_bIsAbnormalEndPlay;
#endif

#ifdef SERV_PVP_REMATCH
			int		m_iRematchCount;
#endif SERV_PVP_REMATCH

			//PVPResultInfo( KEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT& kPacket );
			//PVPResultInfo( KEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT& kPacket );
			//PVPResultInfo( KEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT& kPacket );
			PVPResultInfo( KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacket );
			~PVPResultInfo();
		};


	public:
		CX2PVPGame(void);
		virtual ~CX2PVPGame(void);

		virtual void			Rlease();
		virtual HRESULT			OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT			OnFrameRender();

		virtual void			GameLoading( CX2Room* pRoom );

		virtual void			WorldLoading();
		virtual bool			DeleteUserUnit( int index );

		int						GetWinTeam(){ return m_WinTeam; }
		void					SetWinTeam( int winTeam ){ m_WinTeam = winTeam;  }

		float					GetRemainPlayTime(){ return m_fRemainPlayTime; }
		void					SetRemainPlayTime( float fRemainPlayTime );

		float					GetFirstRemainPlayTime() const { return m_fFirstRemainPlayTime; }

		bool					GetTimeStop(){ return m_bTimeStop; }
		void					TimeStop(){ m_bTimeStop = true; }
		void					TimeOut();

		virtual bool			Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT );
		virtual bool			Handler_EGS_LEAVE_GAME_NOT( KEGS_LEAVE_GAME_NOT& kEGS_LEAVE_GAME_NOT );

		virtual bool			Handler_EGS_USER_UNIT_DIE_ACK( KEGS_USER_UNIT_DIE_ACK& kEGS_USER_UNIT_DIE_ACK );
		virtual bool			Handler_EGS_REMAINING_PLAY_TIME_NOT( KEGS_REMAINING_PLAY_TIME_NOT& kEGS_REMAINING_PLAY_TIME_NOT );

		virtual bool			Handler_EGS_STATE_CHANGE_RESULT_NOT( KEGS_STATE_CHANGE_RESULT_NOT& kEGS_STATE_CHANGE_RESULT_NOT );

		virtual bool			Handler_EGS_END_GAME_NOT( KEGS_END_GAME_NOT& kKEGS_END_GAME_NOT );

				//bool			Handler_EGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT( KEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT& kEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT );
				//bool			Handler_EGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT( KEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT& kEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT );
				//bool			Handler_EGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT( KEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT& kEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT );
		bool					Handler_EGS_END_GAME_PVP_RESULT_DATA_NOT( KEGS_END_GAME_PVP_RESULT_DATA_NOT& kEGS_END_GAME_PVP_RESULT_DATA_NOT );

		virtual void				UserUnitDieNot(	KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT );

#ifdef NEW_CHARACTER_EL
		virtual bool				GetIsExsitancePartyMemberHPLessThanPer( const float fHPRate_, const CX2GUUser* pGUUserWhoCallFunction_ ) const;
#endif // NEW_CHARACTER_EL

#ifdef DUNGEON_ITEM
		int GetPositionCreateItem();
		void SetSepcialItemId(int iVal) { m_iSepcialItemId = iVal; }
		void SetSepcialItemUid(int iVal) { m_iSpecialItemUid = iVal; }
		void ResetCreateItemTimer() { m_fCreateItemTimer = 0.f; }

		void CreateSpecialItem(int itemid, int itemuid, int startpos);
		void ChangeSpecialItem(int itemId, int itemuid);

		void Handler_EGS_CREATE_PVP_ITEM_REQ(int iCreateIdPos);

		void Handler_EGS_CHANGE_PVP_ITEM_POS_REQ(); 
		void Handler_EGS_CHANGE_PVP_ITEM_POS_NOT( UidType iItemUid, int iChangePos );
#endif
	protected:
#ifdef CHUNG_SECOND_CLASS_CHANGE
		virtual void	UpdateUnitPointer();
#endif

	protected:
		CX2PVPRoom*						m_pPVPRoom; /// item���, TimeLimit, WorldID�� ������ �뵵�θ� ��� �ǹǷ� X2Game�� m_pRoom���� ������ �����غ���
		int								m_WinTeam;	/// X2DungeonGame���� ����

		bool							m_bTimeStop;	/// false �� m_fReaminPlayTime�� ���� ��Ű��, END_GAME_NOT �� ������ true�� �ȴ�.
		float							m_fRemainPlayTime;	/// ���� �ð�
		CKTDGFontManager::CUKFont*		m_pFontForTime;		/// ���� �ð� ǥ�ÿ�
		
		map<UidType,int>				m_mapUnitKillNum;	/// Unit�� ų �� Ƚ��
		map<UidType,int>				m_mapUnitDeathNum;	/// Unit�� ���� Ƚ��

		float							m_fFirstRemainPlayTime;	/// EGS_REMAINING_PLAY_TIME_OUT�� ó�� �޾��� �� ��Ŷ���� ���۵� remain���� ����, �׷��� m_iLastElapsedPlayTime�� ������� �ʴ� ����� ���� ����� ������ ����
		int								m_iLastElapsedPlayTime;	/// ������


#ifdef TODAY_RECORD_TEST
		int								m_iConsecutiveWinCount;
		int								m_iConsecutiveLoseCount;
#endif TODAY_RECORD_TEST

#ifdef DUNGEON_ITEM
		bool							m_bFirstCreateItem;	/// �������� ó�� �����ϴ� ������ �ƴ���..
		int								m_iSepcialItemId;	/// ����������� ID // CreatePvpReq�� ���� �� NOT�� �ޱ� ������ �ߺ� ���� ��û�� �����ϱ� ���� 1�� �ӽ� �����ϱ⵵ ��
		int								m_iSpecialItemUid;	/// ����������� UID
		float							m_fCreateItemTimer;	/// ����������� ���� ��� 30�� �Ŀ� �����ǵ��� �ϴ� Ÿ�̸�
		float							m_fChangeItemPosTimer;
#endif

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	public:
		bool IsPvPEscape() { return m_bEscape; }

	private:
		bool m_bEscape;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
};

