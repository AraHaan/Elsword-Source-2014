#pragma once
#include "RoomMonsterManager.h"


//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KPvpMonsterManager : public KRoomMonsterManager
{
public:


public:
    KPvpMonsterManager();
    virtual ~KPvpMonsterManager();

	void StartGame();
	void EndGame();
	void OnCloseRoom();

	bool CreatePvpMonster( IN const KNPCUnitReq& kCreateNpcInfo, OUT int& iNpcUID );

	void SetPvpNpcInfo( IN const std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo );
	void GetPvpNpcInfo( OUT std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo ) { mapPvpNpcInfo = m_mapPvpNpcInfo; }
	
protected:
	//{{ 2011. 07. 19	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
	std::map< UidType, KPvpNpcInfo >	m_mapPvpNpcInfo;
//#endif SERV_PVP_NEW_SYSTEM
	//}}
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


