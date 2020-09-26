#include "BaseServer.h"
#include ".\GSBattleFieldRoom.h"
#include "Enum\Enum.h"
#include "X2Data\XSLRoom.h"
#include "Lottery.h"
#include "NetError.h"


//{{ 2011. 11. 2	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KGSBattleFieldRoom::KGSBattleFieldRoom( const KBattleFieldRoomInfo& kInfo ) : 
m_iJoinUserCountWithReservedUserCount( 0 )
{
	UpdateRoomInfo( kInfo );
}

KGSBattleFieldRoom::~KGSBattleFieldRoom(void)
{
}

void KGSBattleFieldRoom::UpdateRoomInfo( IN const KBattleFieldRoomInfo& kInfo )
{
	m_kRoomInfo = kInfo;
	m_iJoinUserCountWithReservedUserCount = CalcJoinUserCountWithReservedUserCount();
}

int	KGSBattleFieldRoom::GetPartyMemberCountByPartyUID( IN const UidType iPartyUID )
{
	int iPartyMemberCount = 0;

	std::map< UidType, UidType >::const_iterator mit;
	for( mit = m_kRoomInfo.m_mapUnitUIDPartyUID.begin(); mit != m_kRoomInfo.m_mapUnitUIDPartyUID.end(); ++mit )
	{
		const UidType iJoinUserPartyUID = mit->second;

		// ��Ƽ �Ҽ��� �ƴ϶�� �Ѹ� �߰��Ѵ�.
		if( iPartyUID != iJoinUserPartyUID )
			continue;

		++iPartyMemberCount;
	}

	return iPartyMemberCount;
}

int KGSBattleFieldRoom::CalcJoinUserCountWithReservedUserCount() const
{
	int iNumMemberSum = 0;

	std::map< UidType, UidType >::const_iterator mit;
	for( mit = m_kRoomInfo.m_mapUnitUIDPartyUID.begin(); mit != m_kRoomInfo.m_mapUnitUIDPartyUID.end(); ++mit )
	{
		const UidType iUnitUID = mit->first;
		const UidType iPartyUID = mit->second;

		// ��Ƽ �Ҽ��� �ƴ϶�� �Ѹ� �߰��Ѵ�.
		if( iPartyUID == 0 )
		{
			++iNumMemberSum;
			continue;
		}

		std::map< UidType, std::set< UidType > >::const_iterator mitPL;
		mitPL = m_kRoomInfo.m_mapPartyList.find( iPartyUID );
		if( mitPL == m_kRoomInfo.m_mapPartyList.end() )
		{
			// �����αװ� �ʹ� ���������� �α� ������ �����.
			START_LOG( cwarn, L"��ƼUID���� �ִµ� ��Ƽ ����Ʈ���� �ش� ��Ƽ ������ ����? �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( iPartyUID )
				<< END_LOG;
			continue;
		}

		const std::set< UidType >& setPartyMemberList = mitPL->second;
		if( setPartyMemberList.find( iUnitUID ) == setPartyMemberList.end() )
		{
			// �����αװ� �ʹ� ���������� �α� ������ �����.
			START_LOG( cwarn, L"��Ƽ ����Ʈ�� ��Ƽ ������ �ִµ� �ش� ������ ��Ƽ �Ҽ��� �ƴϴ�? �Ͼ���� �ȵǴ� ����!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( iPartyUID )
				<< BUILD_LOG( setPartyMemberList.size() )
				<< END_LOG;
			continue;
		}

		// �ڱ� �ڽ��� ������ �ο����� �����Ѵ�.
		iNumMemberSum += ( static_cast<int>(mitPL->second.size()) - 1 );
	}

	return iNumMemberSum;
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


