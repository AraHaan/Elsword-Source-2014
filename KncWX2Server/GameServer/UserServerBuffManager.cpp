#include "GSUser.h"
#include "BaseServer.h"
#include ".\UserServerBuffManager.h"
#include "X2Data/XSLBuffManager.h"
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLRoom.h"
#include "GSWorldMissionManager.h"


//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM


KUserServerBuffManager::KUserServerBuffManager(void)
{
	Clear();
}

KUserServerBuffManager::~KUserServerBuffManager(void)
{
}

void KUserServerBuffManager::Clear()
{
	m_iInsertTime = 0;
	m_mapBuff.clear();
}

void KUserServerBuffManager::Init( IN const KUnitBuffInfo& kUnitBuffInfo )
{
	m_mapBuff.clear();

	// �α��� ������ ���� �Ŵ����� �ִ� �ð��� �����Ѵ�.
	m_iInsertTime = kUnitBuffInfo.m_iInsertTime;

	// ���� ������ ��������
	m_mapBuff = kUnitBuffInfo.m_mapBuffInfo;
}

bool KUserServerBuffManager::ActivateBuff( IN KBuffInfo& kBuff, OUT std::vector< KBuffInfo >& vecActivatedBuff, IN const CTime tUseTime /* = 0*/, IN int iPeriod /* = 0*/ )
{
	//////////////////////////////////////////////////////////////////////////
	if( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
	{
		START_LOG( clog, L"[����üũ] ����� �� ��� ������ Ȱ��ȭ �˴ϴ�!" )
			<< BUILD_LOG( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
	}
	//////////////////////////////////////////////////////////////////////////

	// �����ϴ� �������� �˻�
	const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
	if( pBuffTemplet == NULL )
	{
		START_LOG( cerr, L"���� ���� �ʴ� �����Դϴ�!" )
			<< BUILD_LOG( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
			<< END_LOG;
		return false;
	}

	// �̹� �ߵ��Ǿ��ִ� �������� �˻�
	if( IsBuffActivated( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID ) == true )
	{
		return false;
	}

	if( tUseTime == 0 )
	{
		CTime tCurr = CTime::GetCurrentTime();
		kBuff.m_iBuffStartTime = tCurr.GetTime();
	}
	else
	{
		kBuff.m_iBuffStartTime = tUseTime.GetTime();
	}

	if( iPeriod == 0 )
	{
		if( pBuffTemplet->m_fBuffPeriodSecond == 0 )
		{
			kBuff.m_iBuffEndTime = kBuff.m_iBuffStartTime;
		}
		else
		{
			kBuff.m_iBuffEndTime = kBuff.m_iBuffStartTime + static_cast<__int64>(pBuffTemplet->m_fBuffPeriodSecond);
		}
	}
	else
	{
		kBuff.m_iBuffEndTime = kBuff.m_iBuffStartTime + static_cast<__int64>(iPeriod);
	}

	m_mapBuff.insert( std::make_pair( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, kBuff ) );

	vecActivatedBuff.push_back( kBuff );
	return true;
}

bool KUserServerBuffManager::DeactivateBuff( IN const int iBuffID, IN OUT std::vector< int >& vecDeleteBuff )
{
	//////////////////////////////////////////////////////////////////////////
	if( iBuffID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
	{
		START_LOG( clog, L"[����üũ] ����� �� ��� ������ ��Ȱ��ȭ �˴ϴ�!" )
			<< BUILD_LOG( iBuffID );
	}
	//////////////////////////////////////////////////////////////////////////
	

	if( IsBuffActivated( iBuffID ) == false )
	{
		START_LOG( cerr, L"�̹� ������ �����Դϴ�." )
			<< BUILD_LOG( iBuffID )
			<< END_LOG;
		return false;
	}

	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.find( iBuffID );
	if( mit != m_mapBuff.end() )
	{
		// ��������
		m_mapBuff.erase( mit );
		vecDeleteBuff.push_back( iBuffID );
	}
	else
	{
		START_LOG( cerr, L"�������� �ʴ� ���� �Դϴ�." )
			<< BUILD_LOG( iBuffID )
			<< END_LOG;
		return false;
	}
	
	return true;
}

//{{ 2013. 04. 15	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
bool KUserServerBuffManager::GetActivateBuff( IN const int iBuffID, OUT KBuffInfo& kBuff ) const
{
	std::map< int, KBuffInfo >::const_iterator it = m_mapBuff.find( iBuffID );
	
	if( it != m_mapBuff.end() )
	{
		kBuff = it->second;
		return true;
	}

	return false;
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

void KUserServerBuffManager::GetBuffList( OUT std::vector< KBuffInfo >& vecBuffInfo, IN const int iRoomType /*= CXSLRoom::RT_NONE*/ ) const
{
	vecBuffInfo.clear();

	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffInfo = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffInfo == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� ���� ID�� ���� ��?" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( iRoomType != CXSLRoom::RT_NONE )
		{
			if( pBuffInfo->CheckRoomType( iRoomType ) == false )
				continue;
		}

		vecBuffInfo.push_back( mit->second );
	}
}

void KUserServerBuffManager::GetBuffIDList( OUT std::vector< int >& vecBuffID, IN const int iRoomType /*= CXSLRoom::RT_NONE*/ ) const
{
	vecBuffID.clear();

	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffInfo = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffInfo == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� ���� ID�� ���� ��?" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( iRoomType != CXSLRoom::RT_NONE )
		{
			if( pBuffInfo->CheckRoomType( iRoomType ) == false )
				continue;
		}

		vecBuffID.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
	}
}

void KUserServerBuffManager::GetBuffFactorList( OUT std::vector< KBuffFactor >& vecBuffFactorID ) const
{
	vecBuffFactorID.clear();

	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffInfo = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffInfo == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� ���� ID�� ���� ��?" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		vecBuffFactorID.push_back( mit->second.m_kFactorInfo );
	}
}

void KUserServerBuffManager::GetBuffBonusRate( IN OUT KRoomUserInfo& kRoomUserInfo, IN const int iRoomType /*= CXSLRoom::RT_NONE*/ )
{
	if( m_mapBuff.empty() )
		return;

	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffInfo = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffInfo == NULL )
		{
			START_LOG( cerr, L"�������� �ʴ� ���� ID�� ���� ��?" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( iRoomType != CXSLRoom::RT_NONE )
		{
			if( pBuffInfo->CheckRoomType( iRoomType ) == false )
				continue;
		}

		// ���� �ɼ� ����
		BOOST_TEST_FOREACH( const int, iSocketOption, pBuffInfo->m_vecOption )
		{
			const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( iSocketOption );
			if( pSocketData == NULL )
			{
				START_LOG( cerr, L"���������ʴ� ���Ͽɼ��� ������ ���� �ɼ����� ���õǾ� �ֽ��ϴ�." )
					<< BUILD_LOG( pBuffInfo->m_eBuffID )
					<< BUILD_LOG( iSocketOption )
					<< END_LOG;
				continue;
			}

			if( pSocketData->m_fIncreaseExpPer > 0.0f )
			{
				kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_EXP, pSocketData->m_fIncreaseExpPer );
			}
			if( pSocketData->m_fIncreaseEDPer > 0.0f )
			{
				kRoomUserInfo.AddBonusRate( KRoomUserInfo::BT_GEV_ED, pSocketData->m_fIncreaseEDPer );
			}
		}
	}
}

void KUserServerBuffManager::CheckBuffPeriod( OUT std::vector< int >& vecDeactivatedBuff, IN const int& iRoomType /*= CXSLRoom::RT_NONE*/ )
{
	vecDeactivatedBuff.clear();

	CTime tCurr = CTime::GetCurrentTime();
	std::vector<int> vecDeleteBuff;
    // ����� ������ üũ�Ѵ�!
	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		if( mit->second.m_iBuffStartTime != mit->second.m_iBuffEndTime )
		{
			if( tCurr > mit->second.m_iBuffEndTime )
			{
				vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );

				//////////////////////////////////////////////////////////////////////////
				if( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
				{
					START_LOG( clog, L"[����üũ] ����� �� ��� ������ �ð� ���ѿ� �ɷȽ��ϴ�!" )
						<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
				}
				//////////////////////////////////////////////////////////////////////////
			}
		}
	}

	BOOST_TEST_FOREACH( int, iBuffID, vecDeleteBuff )
	{
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

void KUserServerBuffManager::OnLeaveField( IN KGSUserPtr spUser, OUT std::vector< int >& vecDeactivatedBuff )
{
	vecDeactivatedBuff.clear();

	std::vector< int > vecDeleteBuff;
	std::map< int, KBuffInfo >::const_iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		switch( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
		case CXSLBuffManager::BTI_BUFF_WARM_ENERGE_OF_EL:			// ������ ���� ���
			vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			break;
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// ���� ����
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
void KUserServerBuffManager::OnLeaveDungeonRoom( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivateBuffList, IN bool bBadAttitudeUser )
#else
void KUserServerBuffManager::OnLeaveDungeonRoom( IN KGSUserPtr spUser, std::vector< int >& vecDeactivateBuffList )
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}
{
	std::vector<int> vecDeleteBuff;

	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� �����Դϴ�!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		switch( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_1:		// ������ ����
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_2:
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_3:
			vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			break;

		case CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY:		// �������� ���� �޽��� ��Ż�ص� ������ �������� �ʴ´�.
			//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
			if( bBadAttitudeUser == true )		// �ҷ� ������ ���� �����̶��' �������� ���� �޽�' ������� ���� �ɾ� �ش�.
			{
				vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
			//}
			break;

		default:
			{
				if( pBuffTemplet->m_bEndBuffAtTheLeaveDungeonRoom == true )
				{
					vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );

					//////////////////////////////////////////////////////////////////////////
					if( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
					{
						START_LOG( clog, L"[����üũ] ����� �� ��� ������ ���� ��Ż�� �����˴ϴ�." )
							<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
					}
					//////////////////////////////////////////////////////////////////////////
				}
			}
			break;
		}
	}
	
	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// ���� ����
		DeactivateBuff( iBuffID, vecDeactivateBuffList );
	}

	//{{ 2013. 01. 09 ���� ���� �ý��� - ��μ�
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	if( bBadAttitudeUser == true )		// �ҷ� ������ ���� �����̶��' �������� ���� �޽�' ������� ���� �ɾ� �ش�.
	{
		KBuffInfo kBuff;
		kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY;

		// �����ϴ� �������� �˻�
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY );
		if( pBuffTemplet != NULL )
		{
			kBuff.m_kFactorInfo.m_iFactorID = pBuffTemplet->m_iFactorID;
			ActivateBuff( kBuff, vecActivateBuff );
		}
	}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
}

void KUserServerBuffManager::OnLeaveBattleField( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivatedBuff )
{
	vecDeactivatedBuff.clear();
	std::vector<int> vecDeleteBuff;
	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� �����Դϴ�!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		switch( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
		case CXSLBuffManager::BTI_BUFF_FIELD_DEFENCE:		// ��Ʋ�ʵ� �̺�Ʈ ����(��ġ +30%, ED +30%)
			{
				vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			}break;

		default:
			if( pBuffTemplet->m_bEndBuffAtTheLeaveBattleFieldRoom == true )
			{
				vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			}
			break;
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// ���� ����
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

void KUserServerBuffManager::OnEndDungeonGame( IN KGSUserPtr spUser, IN const bool bDungeonClearSuccess, IN const bool bStartedByAutoParty, OUT std::vector< int >& vecDeactivatedBuff )
{
	vecDeactivatedBuff.clear();
	std::vector< int > vecDeleteBuff;
	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� �����Դϴ�!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		switch( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
		{
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_1:	// ������ ����
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_2:
		case CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_3:
			{
				if( bStartedByAutoParty )
				{
					vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
				}				
			}			
			break;

		case CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY:	// �������� ���� �޽�
			{
				// �������� ���� �޽��� ����Ŭ��� �ؾ߸� �������.
				// ���� ���� ���δ� �߿����� �ʴ�
				if( bStartedByAutoParty )
				{
					vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
				}
			}
			break;

		default:
			{
				if( pBuffTemplet->m_bEndBuffAtTheEndDungeonGame == true )
				{
					vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );

					//////////////////////////////////////////////////////////////////////////
					if( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
					{
						START_LOG( clog, L"[����üũ] ����� �� ��� ������ ���� ����� �����˴ϴ�." )
							<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
					}
					//////////////////////////////////////////////////////////////////////////
				}
			}
			break;
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// ���� ����
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

void KUserServerBuffManager::OnUserDied( IN KGSUserPtr spUser, std::vector< int >& vecDeactivatedBuff )
{
	std::vector< int > vecDeleteBuff;

	std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin();
	for( ; mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� �����Դϴ�!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( pBuffTemplet->m_bEndBuffAtTheUserDied == true )
		{
			vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );

			//////////////////////////////////////////////////////////////////////////
			if( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == CXSLBuffManager::BTI_BUFF_GATE_DEFENCE )
			{
				START_LOG( clog, L"[����üũ] ����� �� ��� ������ ĳ���� ������� �����˴ϴ�." )
					<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// ���� ����
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}

#ifdef SERV_FIX_REVENGE_BUFF// �۾���¥: 2013-08-09	// �ڼ���
void KUserServerBuffManager::OnLeavePVPRoom( IN KGSUserPtr spUser, OUT std::vector< KBuffInfo >& vecActivateBuff, OUT std::vector< int >& vecDeactivatedBuff )
{
	std::vector<int> vecDeleteBuff;

	for( std::map< int, KBuffInfo >::iterator mit = m_mapBuff.begin(); mit != m_mapBuff.end() ; ++mit )
	{
		const CXSLBuffManager::BuffTemplet* pBuffTemplet = SiCXSLBuffManager()->GetBuffTemplet( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		if( pBuffTemplet == NULL )
		{
			START_LOG( cerr, L"���� ���� �ʴ� �����Դϴ�!" )
				<< BUILD_LOG( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
			continue;
		}

		if( pBuffTemplet->m_bEndBuffAtTheLeavePvpRoom == true )
		{
			vecDeleteBuff.push_back( mit->second.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID );
		}
	}

	BOOST_TEST_FOREACH( const int, iBuffID, vecDeleteBuff )
	{
		// ���� ����
		DeactivateBuff( iBuffID, vecDeactivatedBuff );
	}
}
#endif // SERV_FIX_REVENGE_BUFF

#endif SERV_SERVER_BUFF_SYSTEM
//}}


