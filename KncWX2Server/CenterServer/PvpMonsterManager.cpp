#include "PvpMonsterManager.h"
#include "X2Data/XSLUnitManager.h"


//{{ 2011. 10. 25	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KPvpMonsterManager::KPvpMonsterManager()
{
}

KPvpMonsterManager::~KPvpMonsterManager()
{
}

void KPvpMonsterManager::StartGame()
{
	KRoomMonsterManager::StartGame();
}

void KPvpMonsterManager::EndGame()
{
	KRoomMonsterManager::EndGame();

	//{{ 2011. 07. 19	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
	m_mapPvpNpcInfo.clear();
//#endif SERV_PVP_NEW_SYSTEM
	//}}
}

void KPvpMonsterManager::OnCloseRoom()
{
	KRoomMonsterManager::OnCloseRoom();

	//{{ 2011. 07. 19	������	���� ����
//#ifdef SERV_PVP_NEW_SYSTEM
	m_mapPvpNpcInfo.clear();
//#endif SERV_PVP_NEW_SYSTEM
	//}}
}

bool KPvpMonsterManager::CreatePvpMonster( IN const KNPCUnitReq& kCreateNpcInfo, OUT int& iNpcUID )
{
	iNpcUID = 0;

	// 1. ���� ���� NPC�� ã�´�!
	std::map< UidType, KPvpNpcInfo >::iterator mit;
	for( mit = m_mapPvpNpcInfo.begin(); mit != m_mapPvpNpcInfo.end(); ++mit )
	{
		if( mit->second.m_iNpcID == kCreateNpcInfo.m_NPCID )
			break;
	}

	if( mit == m_mapPvpNpcInfo.end() )
	{
		START_LOG( cerr, L"���� NPC������ ���µ� ������û�� �Դ�!? ���� �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( kCreateNpcInfo.m_NPCID )
			<< END_LOG;
		return false;
	}
	

	// 2. ���� ���� NPC�� UID���� ���� �ش� NPC������ �����Ѵ�!
	const int iPvpNpcUID = static_cast<int>(mit->first);
	m_mapPvpNpcInfo.erase( mit );


	// 3. ���� ���� NPC����!
	if( CreateMonster( iPvpNpcUID, kCreateNpcInfo ) == false )
	{
		START_LOG( cerr, L"���� NPC ���� ����!" )
			<< BUILD_LOG( kCreateNpcInfo.m_NPCID )
			<< END_LOG;
		return false;
	}

	// Ŭ���̾�Ʈ�� ���� NPCUID���� �����Ѵ�.
	iNpcUID = iPvpNpcUID;
	return true;
}

void KPvpMonsterManager::SetPvpNpcInfo( IN const std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo )
{
    if( m_mapPvpNpcInfo.empty() == false )
	{
		START_LOG( cerr, L"���� NPC ������ �����ִµ� ������Ʈ �Ϸ��� �Ѵ�!" )
			<< BUILD_LOG( m_mapPvpNpcInfo.size() )
			<< BUILD_LOG( mapPvpNpcInfo.size() )
			<< END_LOG;
	}

	m_mapPvpNpcInfo = mapPvpNpcInfo;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}


