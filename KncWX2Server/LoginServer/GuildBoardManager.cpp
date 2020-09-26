#include ".\guildBoardmanager.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"
#include "NetError.h"
#include "Enum/Enum.h"


#ifdef SERV_GUILD_AD


ImplementSingleton( KGuildBoardManager );

KGuildBoardManager::KGuildBoardManager(void)
{
	// lua
	lua_tinker::class_add<KGuildBoardManager>( g_pLua, "KGuildBoardManager" );
	lua_tinker::class_def<KGuildBoardManager>( g_pLua, "dump",					&KGuildBoardManager::Dump );

	lua_tinker::decl( g_pLua, "GuildBoardManager", this );
}

KGuildBoardManager::~KGuildBoardManager(void)
{
}

ImplToStringW( KGuildBoardManager )
{
	stm_ << L"------------------------------------------" << std::endl;

	stm_ << TOSTRINGW( m_mapGuildAd.size() );

	for( int i = 0; i < AST_MAX; ++i )
	{
		stm_ << TOSTRINGW( m_vecGuildAd[i].size() );
	}
	
	stm_ << TOSTRINGW( m_vecExpiredGuildAd.size() );
	stm_ << TOSTRINGW( m_mapApplyJoinGuildList.size() );

	return stm_;
}

void KGuildBoardManager::Init()
{
	// ����DB�� ���� ��� ���� ����Ʈ�� ����!
    SendToGameDB( DBE_GET_GUILD_BOARD_INFO_REQ );
}

void KGuildBoardManager::UpdateGuildBoardInfo( IN const KDBE_GET_GUILD_BOARD_INFO_ACK& kInfo )
{
	KDBE_DELETE_GUILD_AD_LIST_NOT kPacketNot;

	// 1. ��� ���� ���� ������Ʈ
	{
		std::vector< KGuildAdInfo >::const_iterator vit;
		for( vit = kInfo.m_vecGuildAdList.begin(); vit != kInfo.m_vecGuildAdList.end(); ++vit )
		{
			InsertGuildAd( *vit );
		}
	}

	// 2. ��� ���� ��û ���� ������Ʈ
	{
		std::vector< KApplyJoinGuildInfo >::const_iterator vitAJ = kInfo.m_vecApplyJoinGuildList.begin();
		for( ; vitAJ != kInfo.m_vecApplyJoinGuildList.end(); ++vitAJ )
		{
			if( InsertApplyJoinGuild( *vitAJ ) == false )
			{
				START_LOG( cerr, L"��� ���� ��û�� ���� ������Ʈ ����!" )
					<< BUILD_LOG( vitAJ->m_iApplyJoinGuildUID )
					<< BUILD_LOG( vitAJ->m_iUnitUID )
					<< END_LOG;

				//{{ 2010. 02. 10  ������	���� ���� ���� ���� �α׸� ����
				//kPacketNot.m_vecDeleteApply.push_back( KApplyDeleteInfo( vitAJ->m_iApplyJoinGuildUID, vitAJ->m_iUnitUID ) );
				//}}
				continue;
			}
		}
	}

	if( !kPacketNot.m_vecDeleteApply.empty() )
	{
		START_LOG( cerr, L"��� ���� ��� ���� ���� ó���� ���� ��û ����Ʈ!" )
			<< BUILD_LOG( kPacketNot.m_vecDeleteApply.size() );

        SendToGameDB( DBE_DELETE_GUILD_AD_LIST_NOT, kPacketNot );
	}
	
	// 3. ��� ���� ���� üũ
	CheckGuildAdDeleteTime( true );

	START_LOG( cout, L"��� �Խ��� ���� ������Ʈ �Ϸ�!" )
		<< BUILD_LOG( kInfo.m_vecGuildAdList.size() )
		<< BUILD_LOG( kInfo.m_vecApplyJoinGuildList.size() );
}

void KGuildBoardManager::Tick()
{
	// ��� ���� �Ⱓ ���� üũ
	CheckGuildAdPeriod();

	// ��� ���� ���� üũ
	CheckGuildAdDeleteTime();

	// ��� ���� ���� üũ
	CheckGuildAdHotGuildSort();
}

KGuildAdPtr KGuildBoardManager::GetGuildAd( IN int iGuildUID )
{
	std::map< int, KGuildAdPtr >::iterator mit = m_mapGuildAd.find( iGuildUID );
	if( mit == m_mapGuildAd.end() )
	{
		START_LOG( clog, L"��� ���� ���� ã�� ����!" )
			<< BUILD_LOG( iGuildUID );

		return KGuildAdPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"��� ������ �̻�.!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return KGuildAdPtr();
	}

	return mit->second;
}

bool KGuildBoardManager::IsExpiredGuildAd( IN int iGuildUID )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"��� ���� ������ �������� �ʽ��ϴ�." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

        return true;
	}

	return spGuildAd->IsExpired();
}

void KGuildBoardManager::InsertGuildAd( IN const KGuildAdInfo& kInfo )
{
	// 1. ������ �����ִ� ��� ���� ������ �ִ��� ã�ƺ���!
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_iGuildUID );
	if( spGuildAd == NULL )
	{
		// ������ ���� ����!
		spGuildAd = KGuildAdPtr( new KGuildAd( kInfo ) );

		m_mapGuildAd.insert( std::make_pair( kInfo.m_iGuildUID, spGuildAd ) );
	}
	else
	{
		// ������ ��������!
		spGuildAd->SetGuildAdInfo( kInfo );
	}

	// 2. �Ⱓ ����� �������� üũ�ؼ� ����Ǿ��ٸ� ����Ʈ���� ���� ����!
	if( spGuildAd->GetCTimeEndDate() < CTime::GetCurrentTime() )
	{
		START_LOG( clog, L"�Ⱓ ����� ����� �Ⱓ���� ����Ʈ�� �ֽ��ϴ�." )
			<< BUILD_LOG( spGuildAd->GetGuildUID() );

		ExpiredGuildAd( kInfo.m_iGuildUID );
	}
	else
	{
		// ���� ����Ʈ�� �߰�
		for( int iType = 0; iType < AST_MAX; ++iType )
		{
			std::vector< KGuildAdPtr >::iterator vit;
			for( vit = m_vecGuildAd[iType].begin(); vit != m_vecGuildAd[iType].end(); ++vit )
			{
				if( (*vit) == NULL )
					continue;

				if( (*vit)->GetGuildUID() == kInfo.m_iGuildUID )
				{
					START_LOG( cerr, L"guild vector�� �ߺ��� guild�� �ִ�!" )
						<< BUILD_LOG( kInfo.m_iGuildUID )
						<< END_LOG;

					// ����Ʈ�����͸� �������!
					(*vit) = spGuildAd;
					break;
				}
			}

			if( vit == m_vecGuildAd[iType].end() )
			{
				m_vecGuildAd[iType].push_back( spGuildAd );
			}
		}
	}

	// 3. ���� ����Ʈ ����
	SortGuildAd();
}

void KGuildBoardManager::DeleteGuildAd( IN int iGuildUID )
{
	// 1. map�����̳ʿ��� �����
	m_mapGuildAd.erase( iGuildUID );

	// 2. vector�����̳ʿ��� �����
	for( int iType = 0; iType < AST_MAX; ++iType )
	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecGuildAd[iType].begin(); vit != m_vecGuildAd[iType].end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == iGuildUID )
				break;
		}

		if( vit != m_vecGuildAd[iType].end() )
		{
			m_vecGuildAd[iType].erase( vit );
		}
	}

	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecExpiredGuildAd.begin(); vit != m_vecExpiredGuildAd.end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == iGuildUID )
				break;
		}

		if( vit != m_vecExpiredGuildAd.end() )
		{
			m_vecExpiredGuildAd.erase( vit );
		}
	}

	// 3. ���� ����Ʈ ����
	SortGuildAd();
}

void KGuildBoardManager::ExpiredGuildAd( IN int iGuildUID )
{
	// 1. ������Ʈ���� ����
	for( int iType = 0; iType < AST_MAX; ++iType )
	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecGuildAd[iType].begin(); vit != m_vecGuildAd[iType].end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == iGuildUID )
				break;
		}

		if( vit != m_vecGuildAd[iType].end() )
		{
			m_vecGuildAd[iType].erase( vit );
		}
	}

	// 2. �Ⱓ���� ����Ʈ�� �߰�
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�Ⱓ ����� ���� ������ �����ϴ�. �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;
		return;
	}

	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecExpiredGuildAd.begin(); vit != m_vecExpiredGuildAd.end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == iGuildUID )
			{
				START_LOG( cerr, L"�Ⱓ ����� ��� ���� ������ �ߺ��Ǹ� �ȵǴµ� �ߺ��Ȱ� �ֳ�?" )
					<< BUILD_LOG( iGuildUID )
					<< END_LOG;
				
				// ������.
				(*vit) = spGuildAd;
				return;
			}
		}

		if( vit == m_vecExpiredGuildAd.end() )
		{
			m_vecExpiredGuildAd.push_back( spGuildAd );
		}
	}

	// 3. ���� ����Ʈ ����
	SortGuildAd();
}

void KGuildBoardManager::UnexpiredGuildAd( IN const KGuildAdInfo& kInfo )
{
	// 1. ��� ���� ���� ������Ʈ
	UpdateGuildAdInfo( kInfo );

	{ // 2. �Ⱓ ����� ���� ����Ʈ���� ����.
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecExpiredGuildAd.begin(); vit != m_vecExpiredGuildAd.end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == kInfo.m_iGuildUID )
				break;
		}

		if( vit != m_vecExpiredGuildAd.end() )
		{
			m_vecExpiredGuildAd.erase( vit );
		}
	}

	// 3. ���� ����Ʈ�� �߰�
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�Ⱓ ����� ���� ������ �����ϴ�. �Ͼ���� �ȵǴ� ����!" )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;
		return;
	}
	
	for( int iType = 0; iType < AST_MAX; ++iType )
	{
		std::vector< KGuildAdPtr >::iterator vit;
		for( vit = m_vecGuildAd[iType].begin(); vit != m_vecGuildAd[iType].end(); ++vit )
		{
			if( (*vit) == NULL )
				continue;

			if( (*vit)->GetGuildUID() == kInfo.m_iGuildUID )
			{
				START_LOG( cerr, L"guild vector�� �ߺ��� guild�� �ִ�!" )
					<< BUILD_LOG( kInfo.m_iGuildUID )
					<< END_LOG;

				// ����Ʈ�����͸� �������!
				(*vit) = spGuildAd;
				break;
			}
		}

		if( vit == m_vecGuildAd[iType].end() )
		{
			m_vecGuildAd[iType].push_back( spGuildAd );
		}
	}

	// 4. ���� ����Ʈ ����
	SortGuildAd();
}

void KGuildBoardManager::CheckGuildAdPeriod( bool bForce /*= false*/ )
{
	// 1�и��� ��� ���� �Ⱓ ���� üũ
	if( m_kTimer[TM_CHECK_GUILD_AD_PERIOD].elapsed() < 60.0  &&  bForce == false )
		return;

	m_kTimer[TM_CHECK_GUILD_AD_PERIOD].restart();

	// ��� ���� �Ⱓ üũ	
	std::vector< int > vecEndGuildAd;

	std::vector< KGuildAdPtr >::const_iterator vit;
	for( vit = m_vecGuildAd[AST_MEMBER_COUNT].begin(); vit != m_vecGuildAd[AST_MEMBER_COUNT].end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->IsExpired() == true )
		{
			vecEndGuildAd.push_back( (*vit)->GetGuildUID() );
		}
	}

	// �Ⱓ�� ����Ǹ� �Խ��ǿ����� ����ó���Ѵ�. ( map���� �״�� ���� )
	std::vector< int >::const_iterator vitEG;
	for( vitEG = vecEndGuildAd.begin(); vitEG != vecEndGuildAd.end(); ++vitEG )
	{
		ExpiredGuildAd( *vitEG );
	}
}

void KGuildBoardManager::CheckGuildAdDeleteTime( bool bForce /*= false*/ )
{
	// 5�и��� ��� ���� ���� üũ
	if( m_kTimer[TM_CHECK_GUILD_AD_DELETE_TIME].elapsed() < 60.0 * 5.0  &&  bForce == false )
		return;

	m_kTimer[TM_CHECK_GUILD_AD_DELETE_TIME].restart();
	
	KDBE_DELETE_GUILD_AD_LIST_NOT kPacketNot;

	std::vector< KGuildAdPtr >::const_iterator vit;
	for( vit = m_vecExpiredGuildAd.begin(); vit != m_vecExpiredGuildAd.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->IsDeleteTime() == true )
		{
			// ���� ��� �ش� ��带 �ִ´�.
			kPacketNot.m_vecDeleteGuildAd.push_back( (*vit)->GetGuildUID() );
		}
	}
	
	std::vector< int >::const_iterator vitDG = kPacketNot.m_vecDeleteGuildAd.begin();
	for( ; vitDG != kPacketNot.m_vecDeleteGuildAd.end(); ++vitDG )
	{
		// ��� ���� ��û ����Ʈ ����
		DeleteApplyJoinGuildList( *vitDG, kPacketNot.m_vecDeleteApply );

		// ��� ���� ����
		DeleteGuildAd( *vitDG );
	}

	if( !kPacketNot.m_vecDeleteGuildAd.empty() )
	{
		SendToGameDB( DBE_DELETE_GUILD_AD_LIST_NOT, kPacketNot );
	}
}

void KGuildBoardManager::CheckGuildAdHotGuildSort()
{
	// 1�и��� ��� HOT����Ʈ ����
	if( m_kTimer[TM_SORT_GUILD_AD].elapsed() < 60.0 )
		return;

	m_kTimer[TM_SORT_GUILD_AD].restart();

	// ��� ���� ��� ī��Ʈ ����
	std::stable_sort( m_vecGuildAd[AST_HOT_GUILD].begin(),		m_vecGuildAd[AST_HOT_GUILD].end(),		&KGuildBoardManager::HotGuildSort );
}

void KGuildBoardManager::SortGuildAd()
{
	// ����Ʈ �ټ����� ��Ʈ�ؾ� �ϹǷ� �����ս��� ��������.

	std::stable_sort( m_vecGuildAd[AST_REG_DATE].begin(),		m_vecGuildAd[AST_REG_DATE].end(),		&KGuildBoardManager::RegDateSort );
	std::stable_sort( m_vecGuildAd[AST_NEW_GUILD].begin(),		m_vecGuildAd[AST_NEW_GUILD].end(),		&KGuildBoardManager::NewGuildSort );
	std::stable_sort( m_vecGuildAd[AST_HOT_GUILD].begin(),		m_vecGuildAd[AST_HOT_GUILD].end(),		&KGuildBoardManager::HotGuildSort );
	std::stable_sort( m_vecGuildAd[AST_MEMBER_COUNT].begin(),	m_vecGuildAd[AST_MEMBER_COUNT].end(),	&KGuildBoardManager::MemberCountSort );
	std::stable_sort( m_vecGuildAd[AST_GUILD_LEVEL].begin(),	m_vecGuildAd[AST_GUILD_LEVEL].end(),	&KGuildBoardManager::GuildLevelSort );
}

bool KGuildBoardManager::RegDateSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL������ �˻�
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. ��� ����
	if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
	{
		return true;
	}

	return false;    
}

bool KGuildBoardManager::MemberCountSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL������ �˻�
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. �ο��� ���� ��
	if( spFirst->GetGuildNumMember() > spSecond->GetGuildNumMember() )
	{
		return true;
	}
	else if( spFirst->GetGuildNumMember() == spSecond->GetGuildNumMember() )
	{
		// 2. �ο��� ���ٸ� ��� ����
		if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
		{
			return true;
		}
	}

	return false;
}

bool KGuildBoardManager::GuildLevelSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL������ �˻�
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. ��� ������ ������
	if( spFirst->GetGuildLevel() > spSecond->GetGuildLevel() )
	{
		return true;
	}
	else if( spFirst->GetGuildLevel() == spSecond->GetGuildLevel() )
	{
		// 2. ��� ������ ���ٸ� ��� ����
		if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
		{
			return true;
		}
	}

	return false;
}

bool KGuildBoardManager::NewGuildSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL������ �˻�
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. ��� ���� ������ ���� ���� ��
	if( spFirst->GetGuildFoundingDay() > spSecond->GetGuildFoundingDay() )
	{
		return true;
	}
	else if( spFirst->GetGuildFoundingDay() == spSecond->GetGuildFoundingDay() )
	{
		// 2. ���ٸ� ��� ����
		if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
		{
			return true;
		}
	}

	return false;
}

bool KGuildBoardManager::HotGuildSort( const KGuildAdPtr spFirst, const KGuildAdPtr spSecond )
{
	// NULL������ �˻�
	if( spFirst == NULL  ||  spSecond == NULL )
		return false;

	// 1. ��� �������� ������ ����
	if( spFirst->GetGuildEXPUpCount() > spSecond->GetGuildEXPUpCount() )
	{
		return true;
	}
	else if( spFirst->GetGuildEXPUpCount() == spSecond->GetGuildEXPUpCount() )
	{
		// 2. ���ٸ� ��� ����
		if( spFirst->GetCTimeRegDate() > spSecond->GetCTimeRegDate() )
		{
			return true;
		}
	}

	return false;
}

bool KGuildBoardManager::IsExistApplyJoinGuild( IN int iGuildUID, IN UidType iUnitUID )
{
	std::map< UidType, int >::const_iterator mit = m_mapApplyJoinGuildList.find( iUnitUID );
	if( mit == m_mapApplyJoinGuildList.end() )
	{
		// �߻� ������ ���� �Դϴ�.
		START_LOG( clog, L"���������ʴ� ���� ��û�Դϴ�." )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID );

		return false;
	}

	if( iGuildUID != mit->second )
	{
		START_LOG( cerr, L"���� ��û�� ��尡 �ٸ��ϴ�. ������ ���� ����!" )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� ���� ���� ��û�� �����Ϸ��� �ϳ�?" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	if( spGuildAd->IsExistMyApplyJoinGuild( iUnitUID ) == false )
	{
		START_LOG( cerr, L"��� ���� ���� ��û�� ��ϵǾ����� �ʴ�!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		// ���� ������ �̹Ƿ� ���� ó���մϴ�.
		DeleteApplyJoinGuild( iUnitUID );
		return false;
	}

	return true;
}

bool KGuildBoardManager::GetMyApplyJoinGuildUID( IN UidType iUnitUID, OUT int& iGuildUID )
{
	iGuildUID = 0;

	std::map< UidType, int >::const_iterator mit = m_mapApplyJoinGuildList.find( iUnitUID );
	if( mit == m_mapApplyJoinGuildList.end() )
	{
		// ������ �ƴϴ�.
		START_LOG( clog, L"���������ʴ� ���� ��û�Դϴ�." )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID );
		
		return false;
	}

	iGuildUID = mit->second;
	return true;
}

bool KGuildBoardManager::InsertApplyJoinGuild( IN const KApplyJoinGuildInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_iApplyJoinGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� ���� ���� ��û�� �Ϸ��� �ϳ�?" )
			<< BUILD_LOG( kInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
		
		return false;
	}

	// ��忡 ���� ��û�� ���
	spGuildAd->NewApplyJoin( kInfo );

	// ���� ��û�� ����Ʈ���� ���
	m_mapApplyJoinGuildList.insert( std::make_pair( kInfo.m_iUnitUID, kInfo.m_iApplyJoinGuildUID ) );
	return true;
}

bool KGuildBoardManager::DeleteApplyJoinGuild( IN UidType iUnitUID )
{
	std::map< UidType, int >::iterator mit = m_mapApplyJoinGuildList.find( iUnitUID );
	if( mit == m_mapApplyJoinGuildList.end() )
	{
		START_LOG( cerr, L"���� ��û������ ���µ� ���Խ�û�� ����� �մϴ�." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}
	
	KGuildAdPtr spGuildAd = GetGuildAd( mit->second );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� ���� ���� ��û�� �����Ϸ��� �ϳ�?" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( mit->second )
			<< END_LOG;

		return false;
	}

	// ���� ��û ���� ó��
	spGuildAd->DeleteApplyJoin( iUnitUID );
	m_mapApplyJoinGuildList.erase( mit );
	return true;
}

bool KGuildBoardManager::DeleteApplyJoinGuildList( IN int iGuildUID, OUT std::vector< KApplyDeleteInfo >& vecApplyDeleteList )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� �����Դϴ�." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	// ��� ���� ��û ����Ʈ ���
	spGuildAd->GetApplyJoinGuildList( vecApplyDeleteList );

	// ��� ���� ��û ����Ʈ ���� ó��
	std::vector< KApplyDeleteInfo >::const_iterator vit;
	for( vit = vecApplyDeleteList.begin(); vit != vecApplyDeleteList.end(); ++vit )
	{
		DeleteApplyJoinGuild( vit->m_iUnitUID );
	}

	return true;
}

void KGuildBoardManager::NewGuildAd( IN const KGuildAdInfo& kInfo )
{
	if( IsExistGuildAd( kInfo.m_iGuildUID ) == true )
	{
		START_LOG( cerr, L"�̹� ��ϵǾ��ִ� ��� ���� ����Ϸ� �߽��ϴ�." )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		// ���� ������ ����
		DeleteGuildAd( kInfo.m_iGuildUID );
	}
    
	// ��� ���� �߰�
    InsertGuildAd( kInfo );
}

bool KGuildBoardManager::ApplyJoinGuild( IN const KELG_APPLY_JOIN_GUILD_REQ& kInfo, OUT std::vector< KApplyDeleteInfo >& vecDeleteApply )
{
	vecDeleteApply.clear();

	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� ���� ���� ��û�� �Ϸ��� �ϳ�?" )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_BOARD_01 );
		return false;
	}

	// �Ⱓ�� �ٵ� �������� Ȯ��
	if( spGuildAd->IsExpired() == true )
	{
		SET_ERROR( ERR_GUILD_BOARD_13 );
		return false;
	}

	// ���� ��û�� ����Ʈ�� 100������ Ȯ��
	if( spGuildAd->GetApplyJoinNumber() >= KGuildAd::MAX_APPLY_LIST_COUNT )
	{
		SET_ERROR( ERR_GUILD_BOARD_16 );
		return false;
	}

	// ������ ���� ��û �س����� �ִ��� Ȯ��
	int iApplyJoinGuildUID = 0;
	if( GetMyApplyJoinGuildUID( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID, iApplyJoinGuildUID ) == true )
	{
		// ���� ���� ��û�� ���� ���� ����
		if( kInfo.m_bOldApplyDelete )
		{
            if( DeleteApplyJoinGuild( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID ) == false )
			{
				START_LOG( cerr, L"���� ���� ��û ���� ����!" )
					<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
					<< END_LOG;

				SET_ERROR( ERR_GUILD_BOARD_08 );
				return false;
			}

			// DB���� ���� ��� ����� ���� ���
			KApplyDeleteInfo kDeleteInfo;
			kDeleteInfo.m_iGuildUID = iApplyJoinGuildUID;
			kDeleteInfo.m_iUnitUID = kInfo.m_kApplyJoinGuildInfo.m_iUnitUID;
			vecDeleteApply.push_back( kDeleteInfo );
		}
		else
		{
			SET_ERROR( ERR_GUILD_BOARD_09 );
			return false;
		}
	}

	// ���� ��û
    if( InsertApplyJoinGuild( kInfo.m_kApplyJoinGuildInfo ) == false )
	{
		START_LOG( cerr, L"���� ��û ����!" )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< BUILD_LOG( kInfo.m_bOldApplyDelete )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_BOARD_08 );
		return false;
	}
	
	return true;
}

void KGuildBoardManager::RollbackApplyJoinGuild( IN const KDBE_APPLY_JOIN_GUILD_ACK& kInfo )
{
    if( DeleteApplyJoinGuild( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"��� ���� ��û �ѹ� ����!" )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< END_LOG;
	}
}

void KGuildBoardManager::UpdateApplyJoinGuildInfo( IN const KDBE_APPLY_JOIN_GUILD_ACK& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"��層�� �������� �ʽ��ϴ�. ������ ���¿���!" )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kInfo.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< END_LOG;		
		return;
	}

	// ���� ���� ������Ʈ
    spGuildAd->UpdateApplyJoinInfo( kInfo.m_kApplyJoinGuildInfo );
}

void KGuildBoardManager::GetGuildAdList( IN const KELG_GET_GUILD_AD_LIST_REQ& kReq, OUT KEGS_GET_GUILD_AD_LIST_ACK& kAck )
{
	// ������ ������ ����
	const u_int SORT_TYPE = kReq.m_cSortType;
	kAck.m_vecGuildAdList.clear();
	kAck.m_cSortType = kReq.m_cSortType;
	kAck.m_uiViewPage = kReq.m_uiViewPage;
	kAck.m_uiTotalPage = 1;

	if( SORT_TYPE < 0  ||  SORT_TYPE >= AST_MAX )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� ���� Ÿ���Դϴ�." )
			<< BUILD_LOGc( SORT_TYPE )
			<< END_LOG;
		return;
	}

	_JIF( kAck.m_uiViewPage > 0, return; );

	if( m_vecGuildAd[SORT_TYPE].empty() )
	{
		kAck.m_uiViewPage = 1;
		kAck.m_uiTotalPage = 1;
		return;
	}

	kAck.m_uiTotalPage = m_vecGuildAd[SORT_TYPE].size() / AE_PAGE_PER_COUNT;
	if( ( m_vecGuildAd[SORT_TYPE].size() % AE_PAGE_PER_COUNT ) > 0 )
	{
		++kAck.m_uiTotalPage;
	}

	u_int uiBeginIndex = ( kAck.m_uiViewPage * AE_PAGE_PER_COUNT ) - AE_PAGE_PER_COUNT;
	if( m_vecGuildAd[SORT_TYPE].size() <= uiBeginIndex )
	{
		kAck.m_uiViewPage = kAck.m_uiTotalPage; // ������ ������
		uiBeginIndex = ( kAck.m_uiViewPage * AE_PAGE_PER_COUNT ) - AE_PAGE_PER_COUNT;

		if( m_vecGuildAd[SORT_TYPE].size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"�Ͼ �� ���� ����!" )
				<< BUILD_LOG( kAck.m_uiViewPage )
				<< BUILD_LOG( kAck.m_uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecGuildAd[SORT_TYPE].size() )
				<< END_LOG;
			return;
		}
	}

	std::vector< KGuildAdPtr >::const_iterator vit = m_vecGuildAd[SORT_TYPE].begin();
	vit += uiBeginIndex;

	while( vit != m_vecGuildAd[SORT_TYPE].end() )
	{
		if( (*vit) == NULL )
		{
			START_LOG( cerr, L"��� ���� �����Ͱ� �̻��մϴ�." )
				<< END_LOG;
			break;
		}

		kAck.m_vecGuildAdList.push_back( (*vit)->GetGuildAdInfo() );

		if( kAck.m_vecGuildAdList.size() >= AE_PAGE_PER_COUNT )
			break;

		++vit;
	}
}

bool KGuildBoardManager::GetGuildAdEndDate( IN int iGuildUID, OUT std::wstring& wstrEndDate )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )	
	{
		return false;
	}
    
	wstrEndDate = spGuildAd->GetGuildAdEndDate();
	return true;
}

bool KGuildBoardManager::GetApplyJoinGuildList( IN const KELG_GET_APPLY_JOIN_GUILD_LIST_REQ& kReq, OUT KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK& kAck )
{
	KGuildAdPtr spGuildAd = GetGuildAd( kReq.m_iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� �����Դϴ�." )
			<< BUILD_LOG( kReq.m_iGuildUID )
			<< END_LOG;

		SET_ERROR( ERR_GUILD_BOARD_01 );
		return false;
	}

	spGuildAd->GetApplyJoinGuildList( kReq, kAck );
	return true;
}

bool KGuildBoardManager::MakeGuildMemberInfoForAcceptJoinGuild( IN int iGuildUID, IN UidType iUnitUID, OUT KGuildMemberInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� �����Դϴ�." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;
		
		return false;
	}

	KApplyJoinGuildInfo kApplyInfo;
	if( spGuildAd->GetApplyJoinGuildInfo( iUnitUID, kApplyInfo ) == false )
	{
		START_LOG( cerr, L"��� ���� ��û ���� ��� ����!" )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	// ��� ��� ���� �����
	kInfo.m_iUnitUID		  = kApplyInfo.m_iUnitUID;
	kInfo.m_wstrNickName	  = kApplyInfo.m_wstrNickName;
	kInfo.m_cUnitClass		  = kApplyInfo.m_cUnitClass;
	kInfo.m_ucLevel			  = kApplyInfo.m_ucLevel;
	kInfo.m_ucMemberShipGrade = SEnum::GUG_NEW_USER;	
	return true;
}

bool KGuildBoardManager::GetGuildSkillInfo( IN int iGuildUID, OUT KGuildSkillInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� �����Դϴ�." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}
	
	return spGuildAd->GetGuildSkillInfo( kInfo );
}

bool KGuildBoardManager::SetGuildSkillInfo( IN int iGuildUID, IN const KGuildSkillInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� �����Դϴ�." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuildAd->SetGuildSkillInfo( kInfo );
	return true;
}

bool KGuildBoardManager::DeleteGuildSkillInfo( IN int iGuildUID )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� �����Դϴ�." )
			<< BUILD_LOG( iGuildUID )
			<< END_LOG;

		return false;
	}

	spGuildAd->DeleteGuildSkillInfo();
	return true;
}

bool KGuildBoardManager::UpdateGuildAdInfo( IN const KGuildAdInfo& kInfo )
{
	KGuildAdPtr spGuildAd = GetGuildAd( kInfo.m_iGuildUID );
	if( spGuildAd == NULL )
	{
		START_LOG( cerr, L"�������� �ʴ� ��� �����Դϴ�." )
			<< BUILD_LOG( kInfo.m_iGuildUID )
			<< END_LOG;

		return false;
	}

	// ��� ���� ���� ����
    spGuildAd->SetGuildAdInfo( kInfo );
	return true;
}

void KGuildBoardManager::UpdateGuildEXP( IN int iGuildUID, IN int iGuildExp )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildEXP( iGuildExp );	
}

void KGuildBoardManager::UpdateGuildLevelResult( IN int iGuildUID, IN u_char ucGuildLevel )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildLevelResult( ucGuildLevel );

	// ��� ���� ������ ����
	std::stable_sort( m_vecGuildAd[AST_GUILD_LEVEL].begin(),	m_vecGuildAd[AST_GUILD_LEVEL].end(),	&KGuildBoardManager::GuildLevelSort );
}

void KGuildBoardManager::UpdateGuildMasterNickName( IN int iGuildUID, IN const std::wstring& wstrNickName )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildMasterNickName( wstrNickName );
}

void KGuildBoardManager::UpdateGuildCurMemberCount( IN int iGuildUID, IN u_short usMemberCount )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildCurMemberCount( usMemberCount );

	// ��� ���� ��� ī��Ʈ ����
	std::stable_sort( m_vecGuildAd[AST_MEMBER_COUNT].begin(),	m_vecGuildAd[AST_MEMBER_COUNT].end(),	&KGuildBoardManager::MemberCountSort );
}

//{{ 2012. 02. 22	�ڼ���	��� �̸� �����
#ifdef SERV_GUILD_CHANGE_NAME
void KGuildBoardManager::UpdateGuildName( IN const int& iGuildUID, IN const std::wstring& wstrGuildName )
{
	KGuildAdPtr spGuildAd = GetGuildAd( iGuildUID );
	if( spGuildAd == NULL )
	{
		return;
	}

	spGuildAd->UpdateGuildName( wstrGuildName );
}
#endif SERV_GUILD_CHANGE_NAME
//}}

void KGuildBoardManager::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}


#endif SERV_GUILD_AD