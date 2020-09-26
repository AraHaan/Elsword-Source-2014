#pragma once
#include "UserServerBuff.h"
#include "X2Data/XSLRoom.h"


//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM


class KUserServerBuffManager
{
public:
	KUserServerBuffManager(void);
	~KUserServerBuffManager(void);

	void Clear();
	void Init( IN const KUnitBuffInfo& kUnitBuffInfo );

	// ���� Ȱ��ȭ & ��Ȱ��ȭ
	bool ActivateBuff( IN KBuffInfo& kBuff, OUT std::vector< KBuffInfo >& vecActivatedBuff, IN const CTime tUseTime = 0, IN int iPeriod = 0 );
	bool DeactivateBuff( IN const int iBuffID, IN OUT std::vector< int >& vecDeleteBuff );	

	// �˻�
	bool IsBuffActivated( IN const int iBuffID ) const	{ return ( m_mapBuff.find( iBuffID ) != m_mapBuff.end() ); }

	//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool GetActivateBuff( IN const int iBuffID, OUT KBuffInfo& kBuff ) const;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	
	// ���� ����Ʈ ���
	void GetBuffList( OUT std::vector< KBuffInfo >& vecBuffInfo, IN const int iRoomType = CXSLRoom::RT_NONE ) const;
	void GetBuffIDList( OUT std::vector< int >& vecBuffID, IN const int iRoomType = CXSLRoom::RT_NONE ) const;
	void GetBuffFactorList( OUT std::vector< KBuffFactor >& vecBuffFactorID ) const;

	// ���� ȿ�� ���
	void GetBuffBonusRate( IN OUT KRoomUserInfo& kRoomUserInfo, IN const int iRoomType = CXSLRoom::RT_NONE );
	
	// ���� üũ
	void CheckBuffPeriod( OUT std::vector< int >& vecDeactivatedBuff, IN const int& iRoomType = CXSLRoom::RT_NONE );
	void OnLeaveField( IN KGSUserPtr spUser, OUT std::vector< int >& vecDeactivatedBuff );
	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	void OnLeaveDungeonRoom( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivateBuffList, IN bool bBadAttitudeUser );
#else
	void OnLeaveDungeonRoom( IN KGSUserPtr spUser, std::vector< int >& vecDeactivateBuffList );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	void OnLeaveBattleField( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivatedBuff );

#ifdef SERV_FIX_REVENGE_BUFF// �۾���¥: 2013-08-09	// �ڼ���
	void OnLeavePVPRoom( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivatedBuff );
#endif // SERV_FIX_REVENGE_BUFF

	void OnEndDungeonGame( IN KGSUserPtr spUser, IN const bool bDungeonClearSuccess, IN const bool bStartedByAutoParty, OUT std::vector< int >& vecDeactivatedBuff );
	void OnUserDied( IN KGSUserPtr spUser, std::vector< int >& vecDeactivatedBuff );

	// db update
	void GetDBUpdateInfo( OUT std::vector< KRecordBuffInfo >& vecRecordBuffInfo ) const;
	
private:
	std::map< int, KBuffInfo >			m_mapBuff;					// <���� ���̵�, ��������>
	__int64								m_iInsertTime;				// ���� �Էµ� ���� �ð�
};


#endif SERV_SERVER_BUFF_SYSTEM
//}}


