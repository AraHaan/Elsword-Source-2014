#include "GSUser.h"
#include ".\UserGameOption.h"


KUserGameOption::KUserGameOption(void) : 
m_cDenyFriendShip(KDenyOptions::DOS_OFF),
m_cDenyInviteGuild(KDenyOptions::DOS_OFF),
m_cDenyParty(KDenyOptions::DOS_OFF),
m_cDenyPersonalTrade(KDenyOptions::DOS_OFF)
//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
, m_cDenyRequestCouple(KDenyOptions::DOS_OFF)
#endif SERV_RELATIONSHIP_SYSTEM
,m_cDenyInvitePractivePVP( KDenyOptions::DOS_OFF )
//}

{
}

KUserGameOption::~KUserGameOption(void)
{
}

const KDenyOptions KUserGameOption::GetDenyOptions()
{
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	return KDenyOptions( m_cDenyFriendShip, m_cDenyInviteGuild, m_cDenyParty, m_cDenyPersonalTrade, m_cDenyRequestCouple, m_cDenyInvitePractivePVP );
#else
	return KDenyOptions( m_cDenyFriendShip, m_cDenyInviteGuild, m_cDenyParty, m_cDenyPersonalTrade );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
}

bool KUserGameOption::SetDenyOptions( const KDenyOptions& kDenyOptions )
{
	if( kDenyOptions.m_cDenyFriendShip < 0  ||  kDenyOptions.m_cDenyFriendShip > KDenyOptions::DOS_OFF )	
	{
		START_LOG( cerr, L"���������ʴ� Ŀ�´�Ƽ�ɼ� ���°�!" )
			<< BUILD_LOGc( kDenyOptions.m_cDenyFriendShip )
			<< END_LOG;

		return false;
	}

	if( kDenyOptions.m_cDenyInviteGuild < 0  ||  kDenyOptions.m_cDenyInviteGuild > KDenyOptions::DOS_ONLY_FRIEND )
	{
		START_LOG( cerr, L"���������ʴ� Ŀ�´�Ƽ�ɼ� ���°�!" )
			<< BUILD_LOGc( kDenyOptions.m_cDenyInviteGuild )
			<< END_LOG;

		return false;
	}

	if( kDenyOptions.m_cDenyParty < 0  ||  kDenyOptions.m_cDenyParty > KDenyOptions::DOS_ONLY_FRIEND )
	{
		START_LOG( cerr, L"���������ʴ� Ŀ�´�Ƽ�ɼ� ���°�!" )
			<< BUILD_LOGc( kDenyOptions.m_cDenyParty )
			<< END_LOG;

		return false;
	}

	if( kDenyOptions.m_cDenyPersonalTrade < 0  ||  kDenyOptions.m_cDenyPersonalTrade > KDenyOptions::DOS_ONLY_FRIEND )
	{
		START_LOG( cerr, L"���������ʴ� Ŀ�´�Ƽ�ɼ� ���°�!" )
			<< BUILD_LOGc( kDenyOptions.m_cDenyPersonalTrade )
			<< END_LOG;

		return false;
	}

	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( kDenyOptions.m_cDenyRequestCouple < 0  ||  kDenyOptions.m_cDenyRequestCouple > KDenyOptions::DOS_ONLY_FRIEND )
	{
		START_LOG( cerr, L"���������ʴ� Ŀ�´�Ƽ�ɼ� ���°�!" )
			<< BUILD_LOGc( kDenyOptions.m_cDenyRequestCouple )
			<< END_LOG;

		return false;
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
    if ( kDenyOptions.m_cDenyInvitePracticePVP < 0 || kDenyOptions.m_cDenyInvitePracticePVP > KDenyOptions::DOS_ONLY_FRIEND ) 
    {
        START_LOG( cerr, L"���������ʴ� Ŀ�´�Ƽ�ɼ� ���°�!" )
            << BUILD_LOGc( kDenyOptions.m_cDenyInvitePracticePVP )
            << END_LOG;

        return false;
    }

	m_cDenyFriendShip	 = kDenyOptions.m_cDenyFriendShip;
	m_cDenyInviteGuild	 = kDenyOptions.m_cDenyInviteGuild;
	m_cDenyParty		 = kDenyOptions.m_cDenyParty;
	m_cDenyPersonalTrade = kDenyOptions.m_cDenyPersonalTrade;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	m_cDenyRequestCouple = kDenyOptions.m_cDenyRequestCouple;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
    m_cDenyInvitePractivePVP = kDenyOptions.m_cDenyInvitePracticePVP;
	return true;
}

void KUserGameOption::SetChatBlackList( const std::vector< KChatBlackListUnit >& vecBlackList )
{
	// �� ����Ʈ �ʱ�ȭ
	m_mapChatBlackList.clear();	

	std::vector< KChatBlackListUnit >::const_iterator vit;
	for( vit = vecBlackList.begin(); vit != vecBlackList.end(); ++vit )
	{
		m_mapChatBlackList.insert( std::make_pair( vit->m_iUnitUID, vit->m_wstrNickName ) );
	}
}

void KUserGameOption::AddChatBlackListUnit( UidType iUnitUID, std::wstring& wstrNickName )
{
	// ����Ʈ�� �߰�
	m_mapChatBlackList.insert( std::make_pair( iUnitUID, wstrNickName ) );
	return;
}

void KUserGameOption::DelChatBlackListUnit( UidType iUnitUID, std::wstring& wstrNickName )
{
	// ����Ʈ���� ����	
	std::map< UidType, std::wstring >::iterator mit = m_mapChatBlackList.find( iUnitUID );
	if( mit != m_mapChatBlackList.end() )
	{
		wstrNickName = mit->second;
		m_mapChatBlackList.erase( mit );
	}
	else
	{
		START_LOG( cerr, L"������Ʈ���� ������ ������ ã�� ���߽��ϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
	}
}

