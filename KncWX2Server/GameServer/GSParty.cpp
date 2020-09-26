#include "GSParty.h"
#include "X2Data/XSLPartyManager.h"


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////


KGSParty::KGSParty( const KPartyInfo& kInfo )
{
	SetInfo( kInfo );
}

KGSParty::~KGSParty()
{
}

void KGSParty::SetInfo( IN const KPartyInfo& kInfo )
{
	m_kPartyInfo = kInfo;
}

void KGSParty::AddMember( IN const KPartyUserInfo& kUserInfo )
{
    if( IsMember( kUserInfo.m_iUnitUID ) )
	{
		START_LOG( cerr, L"�̹� ��Ƽ ����ε� �� ��Ƽ ����� ����Ϸ���!" )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		// ������ �����ϰ� ���� �������!
		DeleteMember( kUserInfo.m_iUnitUID );
	}

	// ��Ƽ ���� ���
	m_mapMember.insert( std::make_pair( kUserInfo.m_iUnitUID, kUserInfo ) );
}

void KGSParty::DeleteMember( IN UidType nCID )
{
	if( !IsMember( nCID ) )
	{
		START_LOG( cerr, L"��Ƽ ����� �ƴѵ� ������� �����Ϸ���!" )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( nCID )
			<< END_LOG;
	}

	m_mapMember.erase( nCID );
}

void KGSParty::GetInfo( OUT KPartyInfo& kInfo )
{
	kInfo = m_kPartyInfo;
	//{{ 2012. 02. 28	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kInfo.m_sPartyFever = SiCXSLPartyManager()->GetMaxFeverPointByNumMember( GetPartyFever(), GetNumMember() );
#else
	kInfo.m_ucPartyFever = CXSLPartyManager::GetMaxFeverPointByNumMember( GetPartyFever(), GetNumMember() );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
}

UidType KGSParty::GetHostUID()
{
	std::map< UidType, KPartyUserInfo >::iterator mit;

	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second.m_bHost == true )
		{
			return mit->second.m_iUnitUID;
		}
	}

	START_LOG( cerr, L"��Ƽ���� ����.? ������ ���� ����!" )
		<< BUILD_LOG( GetPartyUID() )
		<< END_LOG;
	
    return 0;
}

void KGSParty::GetListUserInfoList( OUT std::vector< KPartyListUserInfo >& vecUserInfo )
{
	vecUserInfo.clear();

	std::map< UidType, KPartyUserInfo >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KPartyListUserInfo kInfo;
		kInfo.m_iChannelID		= mit->second.m_iChannelID;
		kInfo.m_iUnitUID		= mit->second.m_iUnitUID;
		kInfo.m_wstrNickName	= mit->second.m_wstrNickName;
		kInfo.m_cUnitClass		= mit->second.m_cUnitClass;
		kInfo.m_ucLevel			= mit->second.m_ucLevel;
		kInfo.m_cState			= mit->second.m_cState;
		kInfo.m_iStateCode		= mit->second.m_iStateCode;
		kInfo.m_bHost			= mit->second.m_bHost;
		vecUserInfo.push_back( kInfo );
	}
}

float KGSParty::GetPartyFeverExpRate()
{
	return ( static_cast<float>( SiCXSLPartyManager()->GetMaxFeverPointByNumMember( GetPartyFever(), GetNumMember() ) ) * 0.00005f );
}

void KGSParty::UpdatePartyUserInfo( IN const KPartyUserInfo& kInfo )
{
	std::map< UidType, KPartyUserInfo >::iterator mit;
	mit = m_mapMember.find( kInfo.m_iUnitUID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cwarn, L"������ �����Ϸ��� ������ ����!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
        return;
	}

	// ������.
	mit->second = kInfo;
}

//{{ 2011. 03. 21  ��μ�	���� ���̵� ����
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
bool KGSParty::CheckPartyMinLevel( IN const int& iDungeonMinLevel )
{
	std::map< UidType, KPartyUserInfo >::iterator mit = m_mapMember.begin();
	for(  ; mit != m_mapMember.end() ; ++mit )
	{
		KPartyUserInfo& UserInfo = mit->second;

		if( static_cast<int>(UserInfo.m_ucLevel) < (iDungeonMinLevel - 2) )
		{
			return false;
		}
	}
	return true;
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_CHANNEL_PARTY
//////////////////////////////////////////////////////////////////////////


