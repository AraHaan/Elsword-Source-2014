#pragma once


#include "Event.h"
#include "CommonPacket.h"
#include "RoomUserManager.h"
#include "X2Data/XSLDungeon.h"

//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

class KBadAttitudeManager
{

public:

	struct KSubStageRankInfo 
	{
		int					m_iStage;
		int					m_iSubStage;
		char				m_cRank;

		KSubStageRankInfo()
		{
			m_iStage		= -1;
			m_iSubStage		= -1;
			m_cRank			= CXSLDungeon::RT_NONE;
		}
	};

	struct KBadAttitudeInfo 
	{
		UidType				m_iUnitUID;				// ��� ����
		KSubStageRankInfo	m_kRankInfo;			// ���� �������� ����
		int					m_iBadAttitudePoint;	// �ҷ�����
		int					m_iVotePoint;			// ��ǥ��
		std::set<UidType>	m_setVotedUnitUID;		// ���� ��ǥ�� �������
		bool				m_bIsBadAttitudeUnit;	// �ҷ� ���� �ΰ�?
		bool				m_bForceExit;			// ���� �Ǿ�� �ϴ°�?

		KBadAttitudeInfo( UidType iUnitUID )
		{
			m_iUnitUID					= iUnitUID;
			m_iBadAttitudePoint			= 0;
			m_iVotePoint				= 0;
			m_setVotedUnitUID.clear();
			m_bIsBadAttitudeUnit		= false;
			m_bForceExit				= false;
		}
	};
	
	

	KBadAttitudeManager(void);
	~KBadAttitudeManager(void);

	// �ʱ�ȭ
	void init( IN KRoomUserManagerPtr m_spRoomUserManager, IN int iDungeonIDAndDiff );
	// ���� �÷��� ���� ���� ���� ����
	bool RemoveUnit( IN UidType iUnitUID, OUT KBadAttitudeInfo& kUnitData );
	// �ҷ� ���� 1�� �߰�
	void IncreaseBadAttitudeOnePoint( IN UidType iUnitUID );
	// ���� ��ǥ�ϱ�
	void IncreaseVoteOnePoint( IN UidType iBadAttitudeUnitUID, IN UidType iVoteUnitUID );
	// �ҷ� ���� �� ��ü ���� ��, ��ǥ���� ���Ͽ� 
	void CheckBadAttitudeUnit( OUT std::vector<UidType>& vecNewBadAttitudeUnit, OUT std::vector<UidType>& vecNewForceExitUnit );
	// �ҷ� ������ �Ǿ��ٸ� �ٸ� �ҷ� �������� ������ ��ǥ�ϰ���
	void BadAttitudeForceVote( IN UidType iBadAttitudeUnitUID );
	// ���� ���������� ������ �� �޾Ҵ°�?
	bool IsAllUnitGetScore();
	// �ֽ� ���� ���������� ������ ��������
	void SetUnitSubStageInfo( IN UidType iUnitUID, IN int iStage, IN int iSubStage, IN int iDungeonIDDiff, IN int iEndNumMember, IN int iCombo, IN int iTech );
	// ���� ����
	void IncreaseSubStageMonsterDieCount( IN int iStageID, IN int iSubStageID, IN const char cMonsterTypeFactor, IN bool bIsDefenceDungeon = false );
	// ���潺
	void SetDefenceDungeonWaveID( IN int iWaveID );
	// ���� ���
	bool GetUnitData( IN UidType iUnitUID, OUT KBadAttitudeInfo& kUnitData );

private:
	std::map<UidType, KBadAttitudeInfo>		m_mapUnitInfoList;
	CXSLDungeon::DUNGEON_TYPE				m_eDungeonType;
	std::map< std::pair< int, int >, int >	m_mapSubStageMonsterScore;   //Key(1:stage,2:substage)
	int										m_iWaveID;
};

#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}