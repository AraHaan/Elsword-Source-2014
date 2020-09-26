#pragma once
#include "X2Data/XSLBattleFieldManager.h"


//{{ 2011. 10. 18	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KBattleFieldGameManager
{
public:
    KBattleFieldGameManager();
    virtual ~KBattleFieldGameManager();

	// ���� ����, ���� ó��
	void	StartGame();
	void	EndGame();

	// ���� ���
	int		GetDangerousValue() const				{ return m_iDangerousValue; }
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	int		GetOldDangerousValue() const				{ return m_iOldDangerousValue; }
	void	ResetDangerousValue()						{ m_iDangerousValue = 0; m_iOldDangerousValue = 0; }
#else
	void	ResetDangerousValue()					{ m_iDangerousValue = 0; }
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	KDangerousEventInfo& GetDangerousEventInfo()	{ return m_kDangerousEvent; }

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-28	// �ڼ���
	void	UpdateDangerousValue( IN const int iDangerousValue );
#else // SERV_BATTLE_FIELD_BOSS
	void	IncreaseDangerousValue( IN const int iIncreaseValue );
	
	// ó�� �Լ�
	void	OnNpcUnitDie( IN const int iPlayerCount,
						  IN const bool bIsAttribNpc, 
						  IN const char cDifficultyLevel, 
						  IN const char cMonsterGrade 
						  //{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
						  , IN const bool bIncreaseDanger 
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
						  //}
						  );
#endif // SERV_BATTLE_FIELD_BOSS

	bool	CheckAndDeleteReservedDangerousEvent( IN const KDangerousEventInfo::DANGEROUS_EVENT eEventEnum );

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-28	// �ڼ���
public:
	void	CheckReserveWarningEvent( IN const int iBeforeDangerousValue );
	void	CheckReserveEliteMonsterDropEvent( IN const int iBeforeDangerousValue );
	void	CheckReserveBossDropEvent( IN const int iPlayerCount );
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	void	CheckReserveMiddleBossDropEvent( IN const int iPlayerCount );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#else // SERV_BATTLE_FIELD_BOSS
protected:
	void	CheckReserveWarningEvent( IN const int iBeforeDangerousValue );
	void	CheckReserveEliteMonsterDropEvent( IN const int iBeforeDangerousValue );
	void	CheckReserveBossDropEvent( IN const int iPlayerCount );
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	void	CheckReserveMiddleBossDropEvent( IN const int iPlayerCount );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#endif // SERV_BATTLE_FIELD_BOSS
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	void	CheckReserveEventBossDropEvent( IN const int iPlayerCount );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT			
protected:
	int								m_iDangerousValue;			// ���赵
	//{{ 2013. 02. 15   �ʵ� �߰� ���� - ��μ�
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	int								m_iOldDangerousValue;			// ���� ���赵
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	KDangerousEventInfo				m_kDangerousEvent;			// �̺�Ʈ ����
	KLottery						m_kLotEliteMonsterDrop;		// ����Ʈ ���� ��� Ȯ��
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


