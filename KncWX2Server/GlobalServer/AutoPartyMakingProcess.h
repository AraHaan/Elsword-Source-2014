#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <list>
#include "Enum/Enum.h"
#include "AutoParty.h"
#include "AutoPartyUserWaitList.h"
#include "CommonPacket.h"
#include "CompareLimitList.h"

//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KAutoPartyCompleteTimeManager
{
public:
	enum COMPLETE_TIME_ENUM
	{
		CTE_MAX_COUNT = 12,
	};

	//{{ ���� �ڵ� ��Ī ���� �ð� ���� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_ESTIMATED_TIME
	struct KAutoPartyTime 
	{
		CTime		m_tAutoPartyTime;
		int			m_iWatiTime;		// ��Ī�� �����Ҷ����� ����� �ð�(�ʴ���)

		KAutoPartyTime()
		{
			m_iWatiTime = 0;
		}
	};
#endif SERV_AUTO_PARTY_MATCHING_ESTIMATED_TIME
	//}

	KAutoPartyCompleteTimeManager();
	~KAutoPartyCompleteTimeManager();

	unsigned int GetListSize() const { return m_listCompleteTime.size(); }
	void AddCompleteTime( IN const int iCompleteTime );
	int	GetAutoPartyMakingEstimatedTime() const;

private:
	//{{ ���� �ڵ� ��Ī ���� �ð� ���� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_ESTIMATED_TIME
	std::list< KAutoPartyTime >		m_listCompleteTime;
#else
	std::list< int >		m_listCompleteTime;
#endif SERV_AUTO_PARTY_MATCHING_ESTIMATED_TIME
	//}
};


class KAutoPartyMakingProcess
{
	enum WAIT_TIME_OUT
	{
		WTO_DIFFERENT_UNIT_TYPE = 0,
		WTO_SAME_UNIT_TYPE,
		WTO_DIRECT_GAME_START,
	};

public:
	struct SLevelCondition
	{
		int		m_iListType;		// �ش� �������ݸ��� ListType�� �Ű����ϴ�. ���� 0���� �����մϴ�.
		int		m_iBeginLevel;
		int		m_iEndLevel;

		SLevelCondition()
		{
			m_iListType = 0;
			m_iBeginLevel = 0;
			m_iEndLevel = 0;
		}
	};

public:
    KAutoPartyMakingProcess( IN const KAutoPartyMakingProcessCondition& kInfo );
    ~KAutoPartyMakingProcess();

	// Tick
	void	Tick();
	
	// Init
	void	ClearAllAutoPartyInfo();
	void	AddLevelCondition( IN const int iBeginLevel, IN const int iEndLevel );

	// get info
	int		GetDungeonID() const										{ return m_iDungeonID; }
	int		GetDifficultyLevel() const									{ return m_iDifficultyLevel; }
	int		GetDungeonIDWithDif() const									{ return m_iDungeonID + m_iDifficultyLevel; }
	char	GetDungeonMode() const										{ return static_cast<char>(m_iDungeonMode); }
	//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	int		GetWaitUserCountByListType( IN const int iListType ) const;
	int		GetWaitUserCountByWaitNumber( IN const UidType iWaitNumber );
#else
	int		GetWaitUserCount() const									{ return static_cast<int>(m_mapAutoPartyWaitUser.size()); }
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}

	// WaitNumber	
	UidType	NewWaitNumber();
	static UidType ExtractDungeonIDAtWaitNumber( IN const UidType iUID_ );
	static UidType ExtractDungeonModeAtWaitNumber( IN const UidType iUID_ );
	static void SetDungeonIDAtWaitNumber( IN OUT UidType& iDestUID_, IN const UidType iSrcUID_ );
	static void SetDungeonModeAtWaitNumber( IN OUT UidType& iDestUID_, IN const UidType iSrcUID_ );

	// function
	bool	IsExistInWaitListByUnitUID( IN const UidType iUnitUID )		{ return ( m_mapAutoPartyWaitUser.find( iUnitUID ) != m_mapAutoPartyWaitUser.end() ); }

	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	bool	RegAutoPartyWaitList( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList, 
								  IN const KAutoPartyConditionInfo& kAutoPartyCondition, 
								  OUT UidType& iWaitNumber, 
								  IN const bool bRegFrontOfList = false );
#else
	bool	RegAutoPartyWaitList( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList, 
								  OUT UidType& iWaitNumber, 
								  IN const bool bRegFrontOfList = false );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	bool	UnRegAutoPartyWaitListByUnitUID( IN const UidType iUnitUID, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName = L"" );
	bool	UnRegAutoPartyWaitListByWaitNumber( IN const UidType iWaitNumber, IN const int iUnRegReason, IN const std::wstring& wstrCancelUnitNickName = L"" );
	void	MakingAutoPartyProcessing( OUT std::vector< KAutoPartyPtr >& vecMadeAutoParty );
	int		GetAutoPartyMakingEstimatedTime() const						{ return m_kCompleteTimeManager.GetAutoPartyMakingEstimatedTime(); }

	bool	GetListTypeByLevelCondition( IN const u_char ucLevel, OUT int& iListType ) const;

	// �ڵ� ��Ƽ ���ʽ�
	bool	IsActivatedAutoPartyBonus() const							{ return m_bActivatedAutoPartyBonus; }
	bool	IsActivatedDelayTimeBonus();
	void	SetActivatedAutoPartyBonus( IN const bool bActivate );
	bool	IsAutoPartyBonusTime() const;
	void	RestartGameStartLastTime()									{ m_tGameStartLastTime.restart(); }

protected:
	void	AutoPartyProcess_GetWaitUserListByOptimalConditionSort( IN const KAutoPartyUserWaitListPtr spFrontAutoPartyWaitUser,
																	IN const double fElapsedWaitTime,
																    IN const std::list< KAutoPartyUserWaitListPtr >& listAutoPartyMakingWaitList, 																    
																	OUT KCompareLimitList< KAutoPartyUserWaitListPtr >& listOptimalAutoPartyWaitList );

	void	AutoPartyProcess_GetWaitNumberListForCompleteMakeAutoParty( IN const KAutoPartyUserWaitListPtr spFrontAutoPartyWaitUser,
																		IN const std::list< KAutoPartyUserWaitListPtr >& listSortedWaitUserList,
																		OUT std::vector< UidType >& vecSelectedWaitUserList,
																		OUT int& iSumAutoPartyPlayerCount
																		//{{ 2012. 11. 30	�ڼ���	������ �ڵ� ��Ƽ ��Ī ���� �ο��� ����
																#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																		, OUT char& iDifficulty
																#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
																		//}}
																		);

protected:
	// WaitUserList
	KAutoPartyUserPtr			GetAutoPartyWaitUserByUnitUID( IN const UidType iUnitUID );
	KAutoPartyUserWaitListPtr	GetAutoPartyWaitListByWaitNumber( IN const UidType iWaitNumber );

	// AutoParty
	KAutoPartyPtr	GetAutoParty( IN const UidType iAutoPartyUID );
	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	KAutoPartyPtr	CreateAutoParty( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserInfo, IN const KAutoPartyConditionInfo& kInfo );
#else
	KAutoPartyPtr	CreateAutoParty( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserInfo );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	void			DestroyAutoParty( IN const UidType iAutoPartyUID );

	// make object
	KAutoPartyUserPtr	MakeUser( IN const KAutoPartyUserInfo& kInfo );
	//{{ 2012. 11. 14	������		�ڵ���Ƽ ��⿭ ���̵� ����
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	KAutoPartyPtr		MakeAutoParty( IN const KAutoPartyConditionInfo& kInfo );
#else
	KAutoPartyPtr		MakeAutoParty();
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}	

	//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	void			IncreaseAutoPartyWaitUserCount( IN const int iListType, IN const int iCount );
	void			DecreaseAutoPartyWaitUserCount( IN const int iListType, IN const int iCount );
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}

protected:
	template < class T > void SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data );
	template < class T > void BroadCastGameServer( IN unsigned short usEventID, IN const T& data );

protected:
	// �ڵ� ��Ƽ �����
	UidType											m_iLastWaitNumber;
	std::map< UidType, KAutoPartyUserPtr >			m_mapAutoPartyWaitUser;			// key : UnitUID
	std::map< UidType, KAutoPartyUserWaitListPtr >	m_mapAutoPartyWaitList;			// key : WaitNumber
	std::list< KAutoPartyUserWaitListPtr >			m_listAutoPartyWaitList;
	static FSMclassPtr								ms_spUserFSM;

	// �ڵ� ��Ƽ ���� ����
	int												m_iDungeonMode;
	int												m_iDungeonID;
	int												m_iDifficultyLevel;
	std::vector< SLevelCondition >					m_vecLevelCondition;			// ������ ���� �����ϱ� ���� �����Դϴ�.

	// �ڵ� ��Ƽ ��� ����
	KAutoPartyCompleteTimeManager					m_kCompleteTimeManager;

	// �ڵ� ��Ƽ ���ʽ� ����
	bool											m_bActivatedAutoPartyBonus;		// �ڵ� ��Ƽ ���ʽ� ����
	boost::timer									m_tGameStartLastTime;			// �ڵ� ��Ƽ ���ʽ� Ÿ�̸� [Ÿ�̸� �ð��� 2���� ������ ���ʽ�����!]
	std::vector< boost::timer >						m_vecDelayBonusTimer;			// UI���Ű� ���� ���ʽ� ����ð� ������ ������ �޲ٱ� ���� Ÿ�̸�

	//{{ 2012. 11. 21	������		����ο� ǥ�� ���� �������� ����
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	std::map< int, int >							m_mapAutoPartyWaitUserCount;	// �ڵ� ��Ƽ ����ο� �� ( ���������� )
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_BY_LIST_TYPE
	//}}
};


template < class T >
void KAutoPartyMakingProcess::SendToGSCharacter( IN UidType iGSUID, IN UidType iUnitUID, IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { iGSUID, -1 };
	KncSend( PI_GLOBAL_AUTO_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, iUnitUID, anTrace, usEventID, data );
}

template < class T >
void KAutoPartyMakingProcess::BroadCastGameServer( IN unsigned short usEventID, IN const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_AUTO_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


