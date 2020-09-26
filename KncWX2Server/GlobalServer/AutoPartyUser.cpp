#include "AutoPartyUser.h"
#include "Enum/Enum.h"


//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


KAutoPartyUser::KAutoPartyUser( IN const KAutoPartyUserInfo& kInfo )
{
	m_kAutoPartyUserInfo = kInfo;
}

KAutoPartyUser::~KAutoPartyUser()
{
}

char KAutoPartyUser::GetUnitType() const
{
	const CXSLUnit::UNIT_CLASS eUnitClass = static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() );
	return static_cast<char>( CXSLUnit::GetUnitClassToUnitType( eUnitClass ) );
}

bool KAutoPartyUser::Accept()
{
	if( IsAccepted() == true )
	{
		START_LOG( cerr, L"�̹� ������ �����̴�!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return false;
	}

	StateTransition( KAutoPartyUserFSM::I_TO_ACCEPTED );
	return true;
}

bool KAutoPartyUser::IsAccepted()
{
	return ( GetStateID() == KAutoPartyUserFSM::S_ACCEPTED );
}

void KAutoPartyUser::UpdatePartyUserInfo( IN const KPartyUserInfo& kPartyUserInfo )
{
	if( IsEmptyPartyUserInfo() == false )
	{
		START_LOG( cerr, L"�̹� PartyUserInfo�� �ִµ� �� ������Ʈ �Ϸ� ���Գ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecPartyUserInfo.size() )
			<< END_LOG;
	}

	// PartyUserInfo �ʱ�ȭ
	ClearPartyUserInfo();

	// ������ PartyUserInfo�� ��������!
	m_vecPartyUserInfo.push_back( kPartyUserInfo );
}

bool KAutoPartyUser::GetPartyUserInfo( OUT KPartyUserInfo& kPartyUserInfo )
{
	if( IsEmptyPartyUserInfo() == true )
	{
		START_LOG( cerr, L"PartyUserInfo�� ����ִ�! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecPartyUserInfo.size() )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, m_vecPartyUserInfo )
	{
		kPartyUserInfo = kInfo;
		return true;
	}

	return false;
}

void KAutoPartyUser::UpdatePartyRoomUserInfo( IN const KPartyRoomUserInfo& kPartyRoomUserInfo )
{
	if( IsEmptyPartyRoomUserInfo() == false )
	{
		START_LOG( cerr, L"�̹� PartyRoomUserInfo�� �ִµ� �� ������Ʈ �Ϸ� ���Գ�? �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecPartyRoomUserInfo.size() )
			<< END_LOG;
	}

	// PartyRoomUserInfo �ʱ�ȭ
	ClearPartyRoomUserInfo();

	// ������ RoomUserInfo�� ��������!
	m_vecPartyRoomUserInfo.push_back( kPartyRoomUserInfo );
}

bool KAutoPartyUser::GetPartyRoomUserInfo( OUT KPartyRoomUserInfo& kPartyRoomUserInfo )
{
	if( IsEmptyPartyRoomUserInfo() == true )
	{
		START_LOG( cerr, L"PartyRoomUserInfo�� ����ִ�! �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecPartyRoomUserInfo.size() )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KPartyRoomUserInfo&, kInfo, m_vecPartyRoomUserInfo )
	{
		kPartyRoomUserInfo = kInfo;
		kPartyRoomUserInfo.m_bAutoPartyBonus = m_kAutoPartyUserInfo.m_bAutoPartyBonus;	// �ڵ� ��Ƽ ���ʽ�
		kPartyRoomUserInfo.m_kOldPartyInfo = m_kAutoPartyUserInfo.m_kOldPartyInfo;		// ���� ��Ƽ ����
		//{{ �ڵ� ��Ƽ�� ���� ���� �� ��Ī ��� �ð� �α� �߰� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		kPartyRoomUserInfo.m_iAutoPartyWaitTime = m_kAutoPartyUserInfo.m_iAutoPartyWaitTime;
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
		return true;
	}

	return false;
}

void KAutoPartyUser::SendToGSCharacter( unsigned short usEventID )
{
    SendToGSCharacter( usEventID, char() );
}

bool KAutoPartyUser::IsHost( const UidType iUnitUID )
{
	// host unit uid �� 0 �̸� ��Ƽ�� �ƴϱ� ������ ���û �ϵ��� �ؾ���
	if( m_kAutoPartyUserInfo.m_kOldPartyInfo.m_iOldHostUnitUID == 0 )
		return true;

	return m_kAutoPartyUserInfo.m_kOldPartyInfo.m_iOldHostUnitUID == iUnitUID;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}



