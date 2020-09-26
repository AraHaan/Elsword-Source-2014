#include ".\guildad.h"
#include "NetError.h"
#include "Enum/Enum.h"


#ifdef SERV_GUILD_AD


KGuildAd::KGuildAd( const KGuildAdInfo& kGuildAdInfo ) :
m_iInitGuildExp( 0 )
{
	SetGuildAdInfo( kGuildAdInfo );
}

KGuildAd::~KGuildAd(void)
{
}

void KGuildAd::SetGuildAdInfo( IN const KGuildAdInfo& kGuildAdInfo )
{
	// ��� ���� ���� ������Ʈ
	m_kGuildAdInfo = kGuildAdInfo;

	// ��� ���� ��ϵǴ¼����� ��� ����ġ�� ���� ( �����ϰ� ���� ����ġ�� ���� ����� Hot Guild ����Ʈ�� ���� )
	m_iInitGuildExp = kGuildAdInfo.m_iGuildEXP;

	//{{ 2010. 02. 04  ������	��� ���� ���� ����
	// ��� â���� ��¥ ����
	LIF( KncUtil::ConvertStringToCTime( kGuildAdInfo.m_wstrFoundingDay, m_tFoundingDay ) );
	//}}

	// ��� ���� ��� ��¥ ����
	LIF( KncUtil::ConvertStringToCTime( kGuildAdInfo.m_wstrAdRegDate, m_tAdRegDate ) );

	// ��� ���� ���� ��¥ ����
	LIF( KncUtil::ConvertStringToCTime( kGuildAdInfo.m_wstrAdEndDate, m_tAdEndDate ) );
}

void KGuildAd::InsertApplyJoin( IN const KApplyJoinGuildInfo& kInfo )
{
	// ��� ���� ��û ��ü ����
	KApplyJoinGuildInfoPtr spApplyInfo( new KApplyJoinGuildInfo( kInfo ) );

	// RegDate����
	CTime tRegDate;
	KncUtil::ConvertStringToCTime( kInfo.m_wstrRegDate, tRegDate );
	spApplyInfo->m_tRegDate = static_cast<__int64>(tRegDate.GetTime());

	// map�����̳ʿ� �ֱ�
	m_mapApplyJoinGuild.insert( std::make_pair( kInfo.m_iUnitUID, spApplyInfo ) );

	// vector�����̳ʿ� �ֱ�
	std::vector< KApplyJoinGuildInfoPtr >::iterator vit;
	for( vit = m_vecApplyJoinGuild.begin(); vit != m_vecApplyJoinGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->m_iUnitUID == kInfo.m_iUnitUID )
		{
			START_LOG( cerr, L"apply vector�� �ߺ��� ������ �ִ�!" )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;

			// ����Ʈ�����͸� �������!
			(*vit) = spApplyInfo;
			break;
		}
	}

	if( vit == m_vecApplyJoinGuild.end() )
	{
		m_vecApplyJoinGuild.push_back( spApplyInfo );
	}

	// ���� �ѹ� ������!
	SortApplyJoinList();
}

void KGuildAd::DeleteApplyJoin( IN UidType iUnitUID )
{
	// map�����̳ʿ��� �����
	m_mapApplyJoinGuild.erase( iUnitUID );

	// vector�����̳ʿ��� �����
	std::vector< KApplyJoinGuildInfoPtr >::iterator vit;
	for( vit = m_vecApplyJoinGuild.begin(); vit != m_vecApplyJoinGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->m_iUnitUID == iUnitUID )
			break;
	}

	if( vit != m_vecApplyJoinGuild.end() )
	{
		m_vecApplyJoinGuild.erase( vit );
	}

	// ���� �ѹ� ������!
	SortApplyJoinList();
}

void KGuildAd::UpdateApplyJoinInfo( IN const KApplyJoinGuildInfo& kInfo )
{
	KApplyJoinGuildInfoPtr spApplyJoinGuildInfo = GetApplyJoinGuildInfo( kInfo.m_iUnitUID );
	if( spApplyJoinGuildInfo == NULL )
	{
		START_LOG( cerr, L"��� ���� ��û�� ������ ����!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return;
	}
    
	// ���� ��û�� ���� ������Ʈ	
	spApplyJoinGuildInfo->m_cUnitClass		= kInfo.m_cUnitClass;
	spApplyJoinGuildInfo->m_ucLevel			= kInfo.m_ucLevel;
	spApplyJoinGuildInfo->m_wstrRegDate		= kInfo.m_wstrRegDate;	

	// RegDate����
	CTime tRegDate;
	KncUtil::ConvertStringToCTime( kInfo.m_wstrRegDate, tRegDate );
	spApplyJoinGuildInfo->m_tRegDate = static_cast<__int64>(tRegDate.GetTime());
}

void KGuildAd::SortApplyJoinList()
{
	// [����] ��ü�� �����ϱ� ���ؼ��� std::stable_sort()�Լ��� �����.

	std::stable_sort( m_vecApplyJoinGuild.begin(), m_vecApplyJoinGuild.end(), &KGuildAd::QuickOrder );
}

bool KGuildAd::QuickOrder( const KApplyJoinGuildInfoPtr spFirst, const KApplyJoinGuildInfoPtr spSecond )
{
	// NULL������ �˻�
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// ��ϼ����� ������
	if( spFirst->m_tRegDate > spSecond->m_tRegDate )
	{
		return true;
	}	

	return false;
}

KApplyJoinGuildInfoPtr KGuildAd::GetApplyJoinGuildInfo( IN UidType iUnitUID )
{
	std::map< UidType, KApplyJoinGuildInfoPtr >::iterator mit = m_mapApplyJoinGuild.find( iUnitUID );
	if( mit == m_mapApplyJoinGuild.end() )
	{
		START_LOG( cerr, L"��� ���� ��û ���� ã�� ����!" )
			<< BUILD_LOG( iUnitUID );

		return KApplyJoinGuildInfoPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��� ���� ��û ������ �̻�.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KApplyJoinGuildInfoPtr();
	}

	return mit->second;
}

bool KGuildAd::GetApplyJoinGuildInfo( IN UidType iUnitUID, OUT KApplyJoinGuildInfo& kInfo )
{
	KApplyJoinGuildInfoPtr spApplyJoinGuildInfo = GetApplyJoinGuildInfo( iUnitUID );
	if( spApplyJoinGuildInfo == NULL )
	{
		START_LOG( cerr, L"��� ���� ��û�� ������ ����!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

    kInfo = *spApplyJoinGuildInfo;
	return true;
}

bool KGuildAd::IsExpired()
{
	CTime tCurTime = CTime::GetCurrentTime();

	if( GetCTimeEndDate() < tCurTime )
		return true;

	return false;
}

bool KGuildAd::IsDeleteTime()
{
	CTime tCurTime = CTime::GetCurrentTime();

	// �Ⱓ ����� ���ķ� �������� �������� üũ
	CTimeSpan tPassedTime = tCurTime - GetCTimeEndDate();
	if( tPassedTime.GetDays() > 7 )
		return true;

	return false;
}

void KGuildAd::NewApplyJoin( IN const KApplyJoinGuildInfo& kInfo )
{
	// �̹� ���Ե� �������� üũ
	if( IsExistMyApplyJoinGuild( kInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"�̹� ���Ե� ���� �Դϴ�!" )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		// ���� ������ ���� ó��
		DeleteApplyJoin( kInfo.m_iUnitUID );
	}

	// ���� ��û
    InsertApplyJoin( kInfo );	
}

void KGuildAd::GetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq, OUT KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK& kAck )
{
	// ������ ������ ����	
	kAck.m_vecApplyJoinGuildList.clear();	
	kAck.m_wstrGuildName = m_kGuildAdInfo.m_wstrGuildName;
	kAck.m_uiViewPage = kReq.m_uiViewPage;
	kAck.m_uiTotalPage = 1;	

	_JIF( kAck.m_uiViewPage > 0, return; );

	if( m_vecApplyJoinGuild.empty() )
	{
		kAck.m_uiViewPage = 1;
		kAck.m_uiTotalPage = 1;
		return;
	}

	kAck.m_uiTotalPage = m_vecApplyJoinGuild.size() / APPLY_JOIN_PAGE_PER_COUNT;
	if( ( m_vecApplyJoinGuild.size() % APPLY_JOIN_PAGE_PER_COUNT ) > 0 )
	{
		++kAck.m_uiTotalPage;
	}

	u_int uiBeginIndex = ( kAck.m_uiViewPage * APPLY_JOIN_PAGE_PER_COUNT ) - APPLY_JOIN_PAGE_PER_COUNT;
	if( m_vecApplyJoinGuild.size() <= uiBeginIndex )
	{
		kAck.m_uiViewPage = kAck.m_uiTotalPage; // ������ ������
		uiBeginIndex = ( kAck.m_uiViewPage * APPLY_JOIN_PAGE_PER_COUNT ) - APPLY_JOIN_PAGE_PER_COUNT;

		if( m_vecApplyJoinGuild.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( kAck.m_uiViewPage )
				<< BUILD_LOG( kAck.m_uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecApplyJoinGuild.size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KApplyJoinGuildInfoPtr >::const_iterator vit = m_vecApplyJoinGuild.begin();
	vit += uiBeginIndex;

	while( vit != m_vecApplyJoinGuild.end() )
	{
		if( (*vit) == NULL )
		{
			START_LOG( cerr, L"��� ���� ��û ���� �����Ͱ� �̻��մϴ�." )
				<< END_LOG;
			break;
		}

		kAck.m_vecApplyJoinGuildList.push_back( *(*vit) );

		if( kAck.m_vecApplyJoinGuildList.size() >= APPLY_JOIN_PAGE_PER_COUNT )
			break;

		++vit;
	}
}

void KGuildAd::GetApplyJoinGuildList( OUT std::vector< KApplyDeleteInfo >& vecApplyDelete )
{
	std::vector< KApplyJoinGuildInfoPtr >::const_iterator vit;
	for( vit = m_vecApplyJoinGuild.begin(); vit != m_vecApplyJoinGuild.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

        KApplyDeleteInfo kInfo;
		kInfo.m_iGuildUID = GetGuildUID();
		kInfo.m_iUnitUID = (*vit)->m_iUnitUID;
		vecApplyDelete.push_back( kInfo );
	}
}

//////////////////////////////////////////////////////////////////////////
// guild skill manager class
KGuildAd::GuildSkillInfoManager::GuildSkillInfoManager() :
m_bInitGuildSkillInfo( false )
{
}

KGuildAd::GuildSkillInfoManager::~GuildSkillInfoManager()
{
}

bool KGuildAd::GuildSkillInfoManager::GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo )
{
    if( !m_bInitGuildSkillInfo )
		return false;

	kInfo = m_kGuildSkillInfo;
	return true;
}

void KGuildAd::GuildSkillInfoManager::SetGuildSkillInfo( IN const KGuildSkillInfo& kInfo )
{
	m_bInitGuildSkillInfo = true;
    m_kGuildSkillInfo = kInfo;	
}

//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
void KGuildAd::UpdateGuildName( IN const std::wstring& wstrGuildName )
{
	m_kGuildAdInfo.m_wstrGuildName = wstrGuildName;
}
#endif SERV_GUILD_CHANGE_NAME
//}}

#endif SERV_GUILD_AD