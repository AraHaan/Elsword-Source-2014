#include "SimLayer.h"
#include "GlobalSimLayer.h"
#include "BaseServer.h"
#include "WorldMissionManager.h"
#include "Socket/NetCommon.h"
#include "Enum/Enum.h"
#include "GlobalServerCCUManager.h"
//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "X2Data//XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
#ifdef SERV_INSERT_GLOBAL_SERVER
//////////////////////////////////////////////////////////////////////////

ImplementRefreshSingleton( KWorldMissionManager );

//{{ 2013. 04. 13	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
KWorldMissionManager::KWorldMissionManager(void)
{
	Init();
}

KWorldMissionManager::~KWorldMissionManager(void)
{
}

void KWorldMissionManager::Init()
{
	m_bWorldMissionOn = true;
	m_bIsFirstTick = true;

	m_bActive = false;
	m_bWorldBuffTime = false;

	m_iStartedCrystalCount		= 0;
	m_iProtectedCrystalCount	= 0;

	m_iTotalCCU = 0;

	SetMissionTime( 0 );

	m_kTimerDOT.restart();
	m_kTimerEnableCheck.restart();
	m_kTimerUpdate.restart();
	m_kTimerWorldBuff.restart();

	m_mapDefenseOpenTime.clear();
	m_tToDayMissionTime = CTime::GetCurrentTime();

	m_iWorldBuffID		= CXSLBuffManager::BTI_NONE;
	m_iIndividualBuffID	= CXSLBuffManager::BTI_NONE;

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	m_kEarthQuakeLottery.Clear();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}
}

ImplToStringW( KWorldMissionManager )
{
	stm_	<< L"----------[ World Mission Manager ]----------" << std::endl
		<< TOSTRINGW( m_bActive )
		<< BUILD_LOG( m_iTotalCCU )
		<< BUILD_LOG( m_iMissionTime )
		<< BUILD_LOG( m_iNextDOT )
		<< BUILD_LOG( m_usWorldBuffDurationTime )
		<< BUILD_LOG( m_iStartedCrystalCount )
		<< BUILD_LOG( m_iProtectedCrystalCount )
		<< TOSTRINGW( m_kTimerDOT.elapsed() )
		;

	KEGB_GET_MISSION_INFO_ACK kPacket;
	SiKWorldMissionManager()->GetWorldMissionInfo( KEGB_GET_MISSION_INFO_ACK::FT_INIT, kPacket );
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_MISSION_INFO_NOT, kPacket );

	return stm_;
}

ImplementLuaScriptParser( KWorldMissionManager )
{

	lua_tinker::class_add<KWorldMissionManager>( GetLuaState(), "KWorldMissionManager" );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "dump",								&KWorldMissionManager::Dump );

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
	//#ifdef SERV_INSERT_GLOBAL_SERVER
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetDefenseOpenTime",				&KWorldMissionManager::SetDefenseOpenTime_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetWorldBuffDurationTime",			&KWorldMissionManager::SetWorldBuffDurationTime_LUA );

	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "WorldMissionDump",					&KWorldMissionManager::WorldMissionDump_LUA );
	//#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetRandomEarthQuakeTime",			&KWorldMissionManager::SetRandomEarthQuakeTime_LUA );
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	//{{ 2013. 04. 24	�ڼ���	����� �� ����
#ifdef SERV_NEW_DEFENCE_DUNGEON
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetReduceTerm",					&KWorldMissionManager::SetReduceTerm_LUA );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	lua_tinker::decl( GetLuaState(), "WorldMissionManager", this );
}

void KWorldMissionManager::Tick()
{
	//////////////////////////////////////////////////////////////////////////
	// 10�� ���� Enableüũ
	if( m_kTimerEnableCheck.elapsed() < 1.0f )
	{
		return;
	}
	m_kTimerEnableCheck.restart();

	//	ó�� ����
	if( m_bIsFirstTick == true && m_kTimerUpdate.elapsed() >= 10.0f)
	{
		//	�ʱ� ���ð��� �⺻ �ð����� �����Ѵ�
		if( SetTodayMissionTime() == true )
		{
			BroadCastMissionStartTime();
		}

		if( m_iNextDOT != 0 )
		{
			BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_INIT );
		}
		m_bIsFirstTick = false;
		return;
	}

	if( IsActive() == true )
	{
		// ũ����Ż�� ���ҵǴ� ī��Ʈ ����
		if( GetReduceTerm() < m_kTimerForReduceTheProtectedCrystalCount.elapsed() )
		{
			IncreaseProtectedCrystalCount( m_iReduceCount );
			m_kTimerForReduceTheProtectedCrystalCount.restart();
		}
	}

	//	���Ӽ����� ���� ������Ʈ
	if( m_kTimerUpdate.elapsed() >= 10.0f )
	{
		// ������ ������Ʈ �ϱ� ���ؼ��� ������ �������� ������ �Ѵ�.		
		BroadCastMissionStartTime();
		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_UPDATE );

		BuffProcess();
		m_kTimerUpdate.restart();
	}

	if( m_bWorldMissionOn == true )		// EndMission() �� �Ҹ��� ���� �̺�Ʈ�� ����
	{
		CTime tCurrentTiem = CTime::GetCurrentTime();

		if( tCurrentTiem.GetHour() == 0 && tCurrentTiem.GetMinute() == 0 &&  tCurrentTiem.GetMinute() <= 1 )	// 00:00 �� ������ -> �������̴�
		{
			if( SetTodayMissionTime() == true )
			{
				BroadCastMissionStartTime();

			}
			else
			{
				START_LOG( clog, L"���� �� ���潺 ������ ����?!" )
					<< END_LOG;
			}
			return;
		}
	}

	if( m_bIsFirstTick == false )
	{
		CheckWorldMissionProcess();
	}
}

void KWorldMissionManager::InitMissionData( void )
{
	// ����ϰ� �ʱ�ȭ �ϱ� ���� �Լ�
	m_iTotalCCU = 0;
	m_iTotalCCU = SiKGlobalServerCCUManager()->GetNowTotalCCU(); // ����

	m_iStartedCrystalCount		= lua_tinker::call<int>( GetLuaState(), "COMPUTE_MIN_MAX_CRYSTAL_COUNT", m_iTotalCCU );
	m_iProtectedCrystalCount	= 0;
	m_iReduceCount				= lua_tinker::call<int>( GetLuaState(), "COMPUTE_REDUCE_CRYSTAL_COUNT", m_iStartedCrystalCount, m_tToDayMissionTime.GetHour() );

	m_kTimerForReduceTheProtectedCrystalCount.restart();
	
	BuffProcess();

	START_LOG( cout, L"����̼� �ʱ�ȭ" )
		<< BUILD_LOG( m_iStartedCrystalCount )
		<< BUILD_LOG( m_iProtectedCrystalCount )
		<< BUILD_LOG( m_iMissionTime )
		<< BUILD_LOG( m_iTotalCCU )
		<< BUILD_LOG( m_usWorldBuffDurationTime )
		<< BUILD_LOG( m_fReduceTerm )
		<< BUILD_LOG( m_iReduceCount )
		<< END_LOG;
}

void KWorldMissionManager::IncreaseProtectedCrystalCount( IN const int iProtectedCrystalCount )
{ 
	if( IsActive() == false )
	{
		return;
	}

	int iTemp = m_iProtectedCrystalCount + iProtectedCrystalCount;

	// over/under flow�� �˻��Ѵ�.
	if( abs( m_iStartedCrystalCount ) < abs( iTemp ) )
	{
		iTemp = m_iStartedCrystalCount;

		if( iProtectedCrystalCount < 0 )
		{
			iTemp = ~iTemp + 1;
		}
	}

	m_iProtectedCrystalCount = iTemp;

	START_LOG( clog, L"��ȣ�� ���� ���� ���� ��û" )
		<< BUILD_LOG( iProtectedCrystalCount )
		<< BUILD_LOG( m_iProtectedCrystalCount )
		<<END_LOG;
}

void KWorldMissionManager::ResetProtectedCrystalCount( void )
{
	if( IsActive() == false )
	{
		return;
	}

	m_iProtectedCrystalCount = 0;
}

void KWorldMissionManager::GetWorldMissionInfo( IN const int& iType, OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck ) const
{
	kPacketAck.m_bActive				= IsActive();
	kPacketAck.m_iWorldCrystalCount		= m_iProtectedCrystalCount;
	kPacketAck.m_iWorldMaxCrystalCount	= m_iStartedCrystalCount;
	kPacketAck.m_iMissionTime			= m_iMissionTime;

	if( iType == KEGB_UPDATE_MISSION_INFO_NOT::FT_INIT )
	{
		kPacketAck.m_mapDefenseOpenTime = m_mapDefenseOpenTime;
	}
	kPacketAck.m_iFlag = iType;
	kPacketAck.m_usWorldBuffDurationTime= m_usWorldBuffDurationTime;
}

void KWorldMissionManager::BroadCastMission( IN const KEGB_UPDATE_MISSION_INFO_NOT::FLAG_TYPE eType )
{
	KEGB_UPDATE_MISSION_INFO_NOT	kPacket;

	GetWorldMissionInfo( eType, kPacket );

	// ��� ���Ӽ����� ��ε� ĳ����
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_MISSION_INFO_NOT, kPacket );
}

void KWorldMissionManager::BroadCastWorldBuff( IN const int _skill )
{
	KEGB_UPDATE_WORLD_BUFF_NOT	kPacket;

	kPacket.m_bActive					= IsActive();
	kPacket.m_usSuccessWorldBuff		= static_cast<u_short>( _skill );
	kPacket.m_usWorldBuffDurationTime	= m_usWorldBuffDurationTime;

	// ��� ���Ӽ����� ��ε� ĳ����
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_WORLD_BUFF_NOT, kPacket );
}

void KWorldMissionManager::BroadCastMissionStartTime( void )
{
	KEGB_UPDATE_MISSION_START_TIME_NOT kPacket;
	kPacket.m_wsrtStartTime = m_tToDayMissionTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	// ��� ���Ӽ����� ��ε� ĳ����
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_MISSION_START_TIME_NOT, kPacket );
}

void KWorldMissionManager::StartMission( IN const bool IsReStart /*= false */)
{
	//	�̺�Ʈ ����
	m_bActive = true;
	m_tToDayStartTime = CTime::GetCurrentTime();

	if( IsReStart == false )
	{
		InitMissionData();
	}
	else
	{
		SetMissionData();
	}

	BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_BEGIN );

	START_LOG( cout2, L"����̼� ����!" )
		<< END_LOG;
}

void KWorldMissionManager::EndMission( void )
{
	m_bActive = false;
	m_bWorldBuffTime = ( GetProtectedCrystalCount() < 0 );
	m_kTimerWorldBuff.restart();
	m_bIsFirstTick = true;
	SetMissionTime( 0 );

	bool bSuccess = ( 0 <= GetProtectedCrystalCount() );

	//	�̺�Ʈ ����
	if( bSuccess )	//	����
	{
		START_LOG( cout2, L"����̼� ����!" )
			<< BUILD_LOG( m_iProtectedCrystalCount )
			<< END_LOG;

		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_SUCCESS );
		WriteWorldMissionCrystalCount( true );
	}
	else
	{
		START_LOG( cout2, L"����̼� ����!" )
			<< BUILD_LOG( m_iProtectedCrystalCount )
			<< END_LOG;

		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_FAIL );
		WriteWorldMissionCrystalCount( true );
	}
}

// ġƮ��
void KWorldMissionManager::SetMissionSuccess_LUA(bool bSuccess)
{
	if( bSuccess )	//	����
	{
		START_LOG( cout2, L"����̼� ���� ���·� ���� ����!" )
			<< END_LOG;

		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_SUCCESS );
	}
	else
	{
		START_LOG( cout2, L"����̼� ���� ���·� ���� ����!" )
			<< END_LOG;

		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_FAIL );
	}		
}

void KWorldMissionManager::WorldMissionDump_LUA()
{
	m_iTotalCCU = SiKGlobalServerCCUManager()->GetNowTotalCCU();

	START_LOG( cout, L"WorldMission ���� üũ" )
		<< BUILD_LOG( m_iTotalCCU )
		<< BUILD_LOG( m_iMissionTime )
		<< BUILD_LOG( m_iNextDOT )
		<< BUILD_LOG( m_usWorldBuffDurationTime )
		<< BUILD_LOG( m_iStartedCrystalCount )
		<< BUILD_LOG( m_iProtectedCrystalCount )
		<< END_LOG;
}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
//#ifdef SERV_INSERT_GLOBAL_SERVER
void KWorldMissionManager::SetDefenseOpenTime_LUA( int iDayOfWeek, int iStartHour, int iStartMin, int iDurationTime )
{
	if( (iDayOfWeek < 1  || iDayOfWeek > 7  ) ||		// ��~�� ���̰� �ƴҶ� ����üũ
		(iStartHour < 0  || iStartHour > 23 ) ||		// �ð� 0~23 ���̰� �ƴҶ�
		(iStartMin  < 0  || iStartMin  > 59 ) ||		// �� 0~59 ���̰� �ƴҶ�
		(iDurationTime < 1) )							// �����ð��� 1 ���� ������ 
	{
		START_LOG( cerr, L"���潺 ���� ���� ���� ���� �̻��մϴ�." )
			<< BUILD_LOG( iDayOfWeek )
			<< BUILD_LOG( iStartHour )
			<< BUILD_LOG( iStartMin )
			<< BUILD_LOG( iDurationTime )
			<< END_LOG;
		return;
	}

	KDefenseOpenTime _info;
	_info.m_iStartHour = iStartHour;
	_info.m_iStartMinute = iStartMin;
	_info.m_iDurationTime = iDurationTime;

	std::map< int, std::vector< KDefenseOpenTime > >::iterator mit = m_mapDefenseOpenTime.find( iDayOfWeek );
	if( mit != m_mapDefenseOpenTime.end() )
	{
		std::vector< KDefenseOpenTime >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end() ; ++vit )
		{
			if( vit->m_iStartHour == iStartHour && vit->m_iStartMinute == iStartMin )	// �ߺ� �Ǵ� ������ ����
			{
				START_LOG( cerr, L"���潺 ���� ���� �ð��� �ߺ� �Ǿ����ϴ�." )
					<< BUILD_LOG( iDayOfWeek )
					<< BUILD_LOG( iStartHour )
					<< BUILD_LOG( iStartMin )
					<< BUILD_LOG( iDurationTime )
					<< END_LOG;
				return;
			}
		}

		mit->second.push_back( _info );
	}
	else
	{
		std::vector< KDefenseOpenTime > vecTemp;
		vecTemp.push_back( _info );
		m_mapDefenseOpenTime.insert( std::make_pair( iDayOfWeek, vecTemp) );
	}
	START_LOG( clog, L"���潺 ���� ���� �ð� �߰��� �Ϸ� �Ǿ����ϴ�." )
		<< BUILD_LOG( iDayOfWeek )
		<< BUILD_LOG( iStartHour )
		<< BUILD_LOG( iStartMin )
		<< BUILD_LOG( iDurationTime )
		<< END_LOG;
}

bool KWorldMissionManager::SetTodayMissionTime( void )
{
	// ���Ϻ� ���� �ð� ���
	m_tToDayMissionTime = CTime::GetCurrentTime();
	std::map< int, std::vector< KDefenseOpenTime > >::iterator mit = m_mapDefenseOpenTime.find( m_tToDayMissionTime.GetDayOfWeek() );
	if( mit == m_mapDefenseOpenTime.end() )
	{
		START_LOG( cerr, L"���潺 ���� �ð� ���� ���� ���� ���� �ʽ��ϴ�." );
		return false;
	}

	// ���� �ð�
	std::vector< KDefenseOpenTime > vecTime = mit->second;
	std::vector< KDefenseOpenTime >::iterator vit = vecTime.begin();
	for( ; vit != vecTime.end() ; ++vit )
	{
		if( vit->m_iStartHour > m_tToDayMissionTime.GetHour() )
		{
			m_kTimerDOT.restart();
			m_iNextDOT = (((vit->m_iStartHour - m_tToDayMissionTime.GetHour() ) * 60 ) + ( vit->m_iStartMinute - m_tToDayMissionTime.GetMinute() ) ) * 60 - ( m_tToDayMissionTime.GetSecond()) ; 
			SetMissionTime( vit->m_iDurationTime * 60 );

			m_tToDayMissionTime = CTime( m_tToDayMissionTime.GetYear(), m_tToDayMissionTime.GetMonth(), m_tToDayMissionTime.GetDay(), vit->m_iStartHour, vit->m_iStartMinute, 0 );

			START_LOG( cout, L"���潺 ���� �ð� ���� �Ϸ�!!" )
				<< BUILD_LOG(vit->m_iStartHour)
				<< BUILD_LOG(vit->m_iStartMinute)
				<< BUILD_LOG(m_iNextDOT)
				<< BUILD_LOG(m_iMissionTime);
			return true;
		}
		else if( vit->m_iStartHour == m_tToDayMissionTime.GetHour() )
		{
			if( vit->m_iStartMinute >= m_tToDayMissionTime.GetMinute() )
			{
				m_kTimerDOT.restart();
				m_iNextDOT = ( vit->m_iStartMinute - m_tToDayMissionTime.GetMinute() ) * 60 - ( m_tToDayMissionTime.GetSecond());
				SetMissionTime( vit->m_iDurationTime * 60 );

				m_tToDayMissionTime = CTime( m_tToDayMissionTime.GetYear(), m_tToDayMissionTime.GetMonth(), m_tToDayMissionTime.GetDay(), vit->m_iStartHour, vit->m_iStartMinute, 0 );

				START_LOG( cout, L"���潺 ���� �ð� ���� �Ϸ�!!" )
					<< BUILD_LOG(vit->m_iStartHour)
					<< BUILD_LOG(vit->m_iStartMinute)
					<< BUILD_LOG(m_iNextDOT)
					<< BUILD_LOG(m_iMissionTime);
				return true;
			}
		}

		int iStartTimeTotalMin = (vit->m_iStartHour * 60) + vit->m_iStartMinute + vit->m_iDurationTime;
		int iNowTimeTotalMin = m_tToDayMissionTime.GetHour() * 60 + m_tToDayMissionTime.GetMinute();

		if( iStartTimeTotalMin > iNowTimeTotalMin )		// ����- �̺�Ʈ ���� ������ ���ȴ� �ٽ� ������ ��� �̺�Ʈ �ð� �ʱ�ȭ
		{
			// ��� ���� �ʱ�ȭ
			m_iNextDOT = 0;
			m_kTimerDOT.restart();

			// ���� ������ ���� �ð�
			CTime tCurTime = CTime::GetCurrentTime();			
			m_tToDayMissionTime = CTime( m_tToDayMissionTime.GetYear(), m_tToDayMissionTime.GetMonth(), m_tToDayMissionTime.GetDay(), vit->m_iStartHour, vit->m_iStartMinute, 0 );

			// �̼� ���� �ð�
			CTimeSpan gap = tCurTime - m_tToDayMissionTime;
			const int iMissionTime =  static_cast<int>((vit->m_iDurationTime * 60) - gap.GetTotalSeconds());
			SetMissionTime( iMissionTime );

			// DB�� ����� ���� ��������
			m_bWorldMissionOn = false;                          // �ٸ� �۾��� ���� �ϵ��� �Ѵ�.
			KDBE_GET_DEFENCE_CRYSTAL_REQ KReq;
			KReq.m_wsrtStartTime = m_tToDayMissionTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			SendToLogDB( DBE_GET_DEFENCE_CRYSTAL_REQ, KReq );

			START_LOG( cout, L"���潺 ���� �ð� ���� �Ϸ�!!" )
				<< BUILD_LOG(vit->m_iStartHour)
				<< BUILD_LOG(vit->m_iStartMinute)
				<< BUILD_LOG(m_iNextDOT)
				<< BUILD_LOG(m_iMissionTime)
				<< END_LOG;
			return true;
		}
	}

	m_bActive = false;
	m_iNextDOT = 0;
	SetMissionTime( 0 );
	START_LOG( clog, L"���� ���潺 ���� �ð� �������ϴ�." );

	return false;
}

void KWorldMissionManager::CheckWorldMissionProcess( void )
{
	if( m_bWorldMissionOn == true && m_bWorldBuffTime == false )
	{
		if( m_bActive == false )
		{
			if( m_iMissionTime <= 0 )
				return;

			if( (int)m_kTimerDOT.elapsed() < m_iNextDOT )
				return;

			StartMission();
		}
		else
		{
			if( m_iMissionTime > GetCorrectMissionTime() )
				return;

			EndMission();
		}
	}

	if( m_bWorldBuffTime && m_kTimerWorldBuff.elapsed() >= m_usWorldBuffDurationTime ) //   ���� �ð� ���� // ���� Ÿ���̰� ���ӽð��� ������
	{
		m_bWorldBuffTime = false;
		m_kTimerWorldBuff.restart();	// �ð� �ʱ�ȭ
		m_iProtectedCrystalCount = 0;
	}
}

int KWorldMissionManager::GetCorrectMissionTime( void ) const
{
	// ���� �ð��� ���� ������ ������ ��Ȯ�� �̼� �ð��� �����Ѵ�.
	CTime CurrentTime = CTime::GetCurrentTime();

	int iGap = ((CurrentTime.GetHour() - m_tToDayStartTime.GetHour()) * 60 * 60) + ((CurrentTime.GetMinute() - m_tToDayStartTime.GetMinute()) * 60) + (CurrentTime.GetSecond() - m_tToDayStartTime.GetSecond());

	if( iGap < 0 )
	{
		START_LOG( clog, L"���� �̼��� �������� �ʾҽ��ϴ�." )
			<< BUILD_LOG(m_iMissionTime)
			<< BUILD_LOG(iGap)
			<< END_LOG;
		return 0;
	}

	return iGap;
}

void KWorldMissionManager::WriteWorldMissionCrystalCount( IN const bool bforce /*= false */)
{
	if( m_bActive  ||  bforce )
	{
		//	Log DB�� �������
		KDBE_INCREASE_DEFENCE_CRYSTAL_REQ PacketReq;
		PacketReq.m_iTargetCrystal			= m_iStartedCrystalCount;
		PacketReq.m_iProtectedCrystalCount	= m_iProtectedCrystalCount;
		PacketReq.m_wsrtStartTime			= m_tToDayMissionTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		SendToLogDB( DBE_INCREASE_DEFENCE_CRYSTAL_REQ, PacketReq );
	}
}

void KWorldMissionManager::SetInitMissionInfo( IN const KDBE_GET_DEFENCE_CRYSTAL_ACK& KPacket )
{
	if( KncUtil::ConvertStringToCTime( KPacket.m_wsrtStartTime, m_tToDayMissionTime ) == false )
	{
		START_LOG( cerr, L"���� �̼� ���� �ð��� �̻��մϴ�. ��ȯ ����!" )
			<< BUILD_LOG( KPacket.m_wsrtStartTime )
			<< END_LOG;
		return;
	}

	m_iStartedCrystalCount		= KPacket.m_iTargetCrystal;
	m_iProtectedCrystalCount	= KPacket.m_iProtectedCrystalCount;
	m_bWorldMissionOn			= true;

	StartMission( true );
}

void KWorldMissionManager::SetMissionData( void )
{
	// ������ ����� ���� �� ����
	m_iTotalCCU = 0;
	m_iTotalCCU = SiKGlobalServerCCUManager()->GetNowTotalCCU(); // ����

	m_iReduceCount = lua_tinker::call<int>( GetLuaState(), "COMPUTE_REDUCE_CRYSTAL_COUNT", m_iStartedCrystalCount, m_tToDayMissionTime.GetHour() );

	m_kTimerForReduceTheProtectedCrystalCount.restart();

	BuffProcess();

	START_LOG( cout, L"����̼� �ʱ�ȭ" )
		<< BUILD_LOG( m_iStartedCrystalCount )
		<< BUILD_LOG( m_iProtectedCrystalCount )
		<< BUILD_LOG( m_iReduceCount )
		<< BUILD_LOG( m_iMissionTime )
		<< BUILD_LOG( m_iTotalCCU )
		<< BUILD_LOG( m_usWorldBuffDurationTime )
		<< END_LOG;
}

void KWorldMissionManager::BroadCastIndividualBuffLevel( IN const int& iIndividualBuffLevel ) const
{
	// ��� ���Ӽ����� ��ε� ĳ����
	KEGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT kPacket;
	UidType anTrace[2] = { 0, -1 };

	kPacket.m_iTheGateOfDarknessIndividualBuffLevel	= iIndividualBuffLevel;
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT, kPacket );
}
int KWorldMissionManager::CheckWorldBuff( void ) const
{
	if( m_bWorldBuffTime == false )
	{
		return CXSLBuffManager::BTI_NONE;
	}

	if( IsActive() == true )
	{
		return CXSLBuffManager::BTI_NONE;
	}

	if( 0 <= m_iProtectedCrystalCount )
	{
		return CXSLBuffManager::BTI_NONE;
	}

	// ���Ϻ� ���� �ð� ���
	CTime tCurrent = CTime::GetCurrentTime();
	std::map< int, std::vector< KDefenseOpenTime > >::const_iterator mit = m_mapDefenseOpenTime.find( tCurrent.GetDayOfWeek() );
	if( mit == m_mapDefenseOpenTime.end() )
	{
		START_LOG( cerr, L"���潺 ���� �ð� ���� ���� ���� ���� �ʽ��ϴ�." );
		return CXSLBuffManager::BTI_NONE;
	}
#ifdef SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#else //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	const int iBuffID = CXSLBuffManager::BTI_BUFF_2013_DEFENSE_DEBUFF;

	// ������ ���� �ǰ� �ִ� �ð��ΰ��� �˾ƺ���
	const std::vector< KDefenseOpenTime > vecTime = mit->second;
	// �ش� ���Ͽ� ����� ���� �ð����� ��ȸ�ϸ鼭, ���� �ð��뿡 �´� ������ �ִ��� �˻��Ѵ�.
	for( std::vector< KDefenseOpenTime >::const_reverse_iterator rit = vecTime.rbegin(); rit != vecTime.rend(); ++rit )
	{
		const KDefenseOpenTime& kDefenseOpenTime = *rit;
		CTime tEndTime = CTime( tCurrent.GetYear(), tCurrent.GetMonth(), tCurrent.GetDay(), kDefenseOpenTime.m_iStartHour, kDefenseOpenTime.m_iStartMinute, 0 ) + CTimeSpan( 0,0, kDefenseOpenTime.m_iDurationTime, 0);
		CTime tBuffEndTime = tEndTime + CTimeSpan( 0, 0, 0, static_cast<int>( m_usWorldBuffDurationTime ) );

		std::wstring wstrEndTime = tEndTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		std::wstring wstrBuffEndTime = tBuffEndTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		if( ( tEndTime <= tCurrent ) && ( tCurrent < tBuffEndTime ) )
		{
			return iBuffID;
		}
	}
#endif //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	return CXSLBuffManager::BTI_NONE;
}

int KWorldMissionManager::CheckIndividualBuff( void ) const
{
#ifdef SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#else //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	if( IsActive() == true )
	{
		return CXSLBuffManager::BTI_BUFF_2013_DEFENSE_FAKE_BUFF;
	}
#endif //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF

	if( m_iProtectedCrystalCount < 0 )
	{
		return CXSLBuffManager::BTI_NONE;
	}

	CTime tCurrentTime = CTime::GetCurrentTime();

	// ���Ϻ� ���� �ð� ���
	std::map< int, std::vector< KDefenseOpenTime > >::const_iterator mit = m_mapDefenseOpenTime.find( tCurrentTime.GetDayOfWeek() );

	// �ش� ���Ͽ� ����� ���� �ð� ���� ���� �˻�.
	if( mit == m_mapDefenseOpenTime.end() )
	{
		return CXSLBuffManager::BTI_NONE;
	}
#ifdef SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#else //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	const int iBuffID = CXSLBuffManager::BTI_BUFF_2013_DEFENSE_BUFF;

	// ���� �ð�
	const std::vector< KDefenseOpenTime >& vecTime = mit->second;

	// �ش� ���Ͽ� ����� ���� �ð����� ��ȸ�ϸ鼭, ���� �ð��뿡 �´� ������ �ִ��� �˻��Ѵ�.
	for( std::vector< KDefenseOpenTime >::const_reverse_iterator rit = vecTime.rbegin(); rit != vecTime.rend(); ++rit )
	{
		const KDefenseOpenTime& kDefenseOpenTime = *rit;
		CTime tBuffStartTime	= CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0 ) + CTimeSpan( 0,  kDefenseOpenTime.m_iStartHour, kDefenseOpenTime.m_iStartMinute + kDefenseOpenTime.m_iDurationTime, 0 );
		CTime tBuffEndTime		= tBuffStartTime + CTimeSpan( 0, 0, 0, static_cast<int>( m_usWorldBuffDurationTime ) );

		if( ( tBuffStartTime <= tCurrentTime ) && ( tCurrentTime < tBuffEndTime ) )
		{
			return iBuffID;
		}
	}
#endif //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	return CXSLBuffManager::BTI_NONE;
}

void KWorldMissionManager::BuffProcess( void )
{
	int iBuff = CheckWorldBuff();
	if( m_iWorldBuffID != iBuff )
	{
		m_iWorldBuffID = iBuff;
		BroadCastWorldBuff( m_iWorldBuffID );
	}

	iBuff = CheckIndividualBuff();
	if( m_iIndividualBuffID != iBuff )
	{
		m_iIndividualBuffID = iBuff;
		BroadCastIndividualBuffLevel( m_iIndividualBuffID );
	}
}

//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
void KWorldMissionManager::SetRandomEarthQuakeTime_LUA( int iSec, float fRate )
{
	m_kEarthQuakeLottery.AddCaseIntegerCast( iSec, fRate );
}

int KWorldMissionManager::GetRandomEarthQuakeTime()
{
	int iSec = m_kEarthQuakeLottery.Decision();

	if( iSec <= 0 )
	{
		START_LOG( cerr, L"������ �˷��ִ� �ð��� �ʹ� �����ų� �̻��ϴ�." )
			<< BUILD_LOG( iSec )
			<< END_LOG;

		iSec = 3600;
	}

	return iSec;
}
#endif SERV_EARTH_QUAKE_EFFECT
//}}
#else
KWorldMissionManager::KWorldMissionManager(void)
{
	Init();
}

KWorldMissionManager::~KWorldMissionManager(void)
{
}

void KWorldMissionManager::Init()
{
	m_bWorldMissionOn = false;
	m_bIsFirstTick = true;

	m_bActive = false;
	m_bWorldBuffTime = false;

	m_fNormalPlayRate		= 0.0f;
	m_fHardPlayRate			= 0.0f;
	m_fExpertPlayRate		= 0.0f;

	m_fNormalSuccessRate	= 0.0f;
	m_fHardSuccessRate		= 0.0f;
	m_fExpertSuccessRate	= 0.0f;

	m_cNormalBasicCrystalCount	= 0;
	m_cHardBasicCrystalCount	= 0;
	m_cExpertBasicCrystalCount	= 0;

	m_iCrystalTargetCount = 0;
	m_iProtectedCrystalCount = 0;
	m_fCountPerWorldCrystal = 1.0f;
	m_iWorldCrystalCount = 0;
	m_usSuccessWorldBuff = 0;

	m_iTotalCCU = 0;

	SetMissionTime( 0 );
	
	m_kTimerDOT.restart();
	m_kTimerEnableCheck.restart();
	m_kTimerUpdate.restart();
	m_kTimerWorldBuff.restart();

	m_mapDefenseOpenTime.clear();
	m_tToDayMissionTime = CTime::GetCurrentTime();

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	m_kEarthQuakeLottery.Clear();
#endif SERV_EARTH_QUAKE_EFFECT
	//}}
}

ImplToStringW( KWorldMissionManager )
{
	stm_	<< L"----------[ World Mission Manager ]----------" << std::endl
			<< TOSTRINGW( m_bActive )
			<< BUILD_LOG( m_iTotalCCU )
			<< BUILD_LOG( m_iMissionTime )
			<< BUILD_LOG( m_iNextDOT )
			<< BUILD_LOG( m_usWorldBuffDurationTime )
			<< BUILD_LOG( m_usWorldBuffDurationTime )
			<< BUILD_LOG( m_iCrystalTargetCount )
			<< BUILD_LOG( m_iProtectedCrystalCount )
			<< BUILD_LOG( m_fCountPerWorldCrystal )
			<< BUILD_LOG( m_iWorldCrystalCount )
			<< BUILD_LOG( m_fNormalPlayRate )
			<< BUILD_LOG( m_fHardPlayRate )
			<< BUILD_LOG( m_fExpertPlayRate )
			<< BUILD_LOG( m_fNormalSuccessRate )
			<< BUILD_LOG( m_fHardSuccessRate )
			<< BUILD_LOG( m_fExpertSuccessRate )
			<< TOSTRINGW( m_kTimerDOT.elapsed() )
			;

	//{{ 2012. 04. 24	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	KEGB_GET_MISSION_INFO_ACK kPacket;
	SiKWorldMissionManager()->GetWorldMissionInfo( KEGB_GET_MISSION_INFO_ACK::FT_INIT, kPacket );
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_MISSION_INFO_NOT, kPacket );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	return stm_;
}

ImplementLuaScriptParser( KWorldMissionManager )
{

	lua_tinker::class_add<KWorldMissionManager>( GetLuaState(), "KWorldMissionManager" );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetDungeonPlayTime",				&KWorldMissionManager::SetDungeonPlayTime_LUA );

	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "Reset",							&KWorldMissionManager::Reset_LUA );

	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetMissionSuccess",				&KWorldMissionManager::SetMissionSuccess_LUA );

	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "dump",								&KWorldMissionManager::Dump );

	//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
//#ifdef SERV_INSERT_GLOBAL_SERVER
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetNormalPlayRate",				&KWorldMissionManager::SetNormalPlayRate_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetHardPlayRate",					&KWorldMissionManager::SetHardPlayRate_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetExpertPlayRate",				&KWorldMissionManager::SetExpertPlayRate_LUA );

	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetNormalSuccessRate",				&KWorldMissionManager::SetNormalSuccessRate_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetHardSuccessRate",				&KWorldMissionManager::SetHardSuccessRate_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetExpertSuccessRate",				&KWorldMissionManager::SetExpertSuccessRate_LUA );

	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetNormalBasicCrystalCount",		&KWorldMissionManager::SetNormalBasicCrystalCount_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetHardBasicCrystalCount",			&KWorldMissionManager::SetHardBasicCrystalCount_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetExpertBasicCrystalCount",		&KWorldMissionManager::SetExpertBasicCrystalCount_LUA );

	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetDefenseOpenTime",				&KWorldMissionManager::SetDefenseOpenTime_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetWorldMissionOn",				&KWorldMissionManager::SetWorldMissionOn_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetWorldBuffDurationTime",			&KWorldMissionManager::SetWorldBuffDurationTime_LUA );

	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "BuffCheat",						&KWorldMissionManager::BuffCheat_LUA );
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "WorldMissionDump",					&KWorldMissionManager::WorldMissionDump_LUA );
//#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�

	//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
	lua_tinker::class_def<KWorldMissionManager>( GetLuaState(), "SetRandomEarthQuakeTime",					&KWorldMissionManager::SetRandomEarthQuakeTime_LUA );
#endif SERV_EARTH_QUAKE_EFFECT
	//}}

	lua_tinker::decl( GetLuaState(), "WorldMissionManager", this );
}

void KWorldMissionManager::Tick()
{
	//////////////////////////////////////////////////////////////////////////
	// 10�� ���� Enableüũ
	if( m_kTimerEnableCheck.elapsed() < 1.0f )
		return;

	m_kTimerEnableCheck.restart();

	//	ó�� ����
	if( m_bIsFirstTick == true && m_kTimerUpdate.elapsed() >= 10.0f)
	{
		//	�ʱ� ���ð��� �⺻ �ð����� �����Ѵ�
		if( SetTodayMissionTime() == true )
		{
			BroadCastMissionStartTime();
		}
		if( m_iNextDOT != 0 )
		{
			BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_INIT );
		}
		m_bIsFirstTick = false;
		return;
	}

	//	���Ӽ����� ���� ������Ʈ
	if( m_kTimerUpdate.elapsed() >= 10.0f )
	{
		// ������ ������Ʈ �ϱ� ���ؼ��� ������ �������� ������ �Ѵ�.		
		BroadCastMissionStartTime();
		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_UPDATE );
		m_kTimerUpdate.restart();
	}

	if( m_bWorldMissionOn == true )		// EndMission() �� �Ҹ��� ���� �̺�Ʈ�� ����
	{
		CTime tCurrentTiem = CTime::GetCurrentTime();
		
		if( tCurrentTiem.GetHour() == 0 && tCurrentTiem.GetMinute() == 0 &&  tCurrentTiem.GetMinute() <= 1 )	// 00:00 �� ������ -> �������̴�
		{
			if( SetTodayMissionTime() == true )
			{
				BroadCastMissionStartTime();
			
			}
			else
			{
				START_LOG( clog, L"���� �� ���潺 ������ ����?!" )
					<< END_LOG;
			}
			return;
		}
	}
	
	if( m_bIsFirstTick == false )
	{
		CheckWorldMissionProcess();
	}
}

void KWorldMissionManager::InitMissionData()
{
	// ����ϰ� �ʱ�ȭ �ϱ� ���� �Լ�
	m_iTotalCCU = 0;
	m_iTotalCCU = SiKGlobalServerCCUManager()->GetNowTotalCCU(); // ����

	//{{ 2012. 03. 03	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	int iNormalCount	= static_cast<int>((( m_iTotalCCU * m_fNormalPlayRate)	/ 1 ) * m_fNormalSuccessRate	);
	int iHardCount		= static_cast<int>((( m_iTotalCCU * m_fHardPlayRate)	/ 3 ) * m_fHardSuccessRate		);
	int iExpertCount	= static_cast<int>((( m_iTotalCCU * m_fExpertPlayRate)	/ 4 ) * m_fExpertSuccessRate	);
#else
	int iNormalCount = static_cast<int>((( m_iTotalCCU * m_fNormalPlayRate) / 1 ) * m_fNormalSuccessRate * m_cNormalBasicCrystalCount );
	int iHardCount = static_cast<int>((( m_iTotalCCU * m_fHardPlayRate) / 3 ) * m_fHardSuccessRate * m_cHardBasicCrystalCount );
	int iExpertCount = static_cast<int>((( m_iTotalCCU * m_fExpertPlayRate) / 4 ) * m_fExpertSuccessRate * m_cExpertBasicCrystalCount );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	m_iCrystalTargetCount = iNormalCount + iHardCount + iExpertCount;

	if( m_iCrystalTargetCount < WBSC_STEP_THREE )
	{
		m_iCrystalTargetCount = 15;
		//m_iCrystalTargetCount = 1500;
	}

	m_fCountPerWorldCrystal = m_iCrystalTargetCount / static_cast<float>( WBSC_STEP_THREE );
	m_iProtectedCrystalCount = 0;
	m_iWorldCrystalCount = 0;
	m_usSuccessWorldBuff = 0;

	START_LOG( cout, L"����̼� �ʱ�ȭ" )
		<< BUILD_LOG(m_iCrystalTargetCount)
		<< BUILD_LOG(m_iProtectedCrystalCount)
		<< BUILD_LOG(m_fCountPerWorldCrystal)
		<< BUILD_LOG(m_iWorldCrystalCount)
		<< BUILD_LOG(m_iMissionTime)
		<< BUILD_LOG(m_iDungeonPlayTime)
		<< BUILD_LOG(m_iTotalCCU)
		<< BUILD_LOG(m_fNormalPlayRate)
		<< BUILD_LOG(m_fHardPlayRate)
		<< BUILD_LOG(m_fExpertPlayRate)
		<< BUILD_LOG(m_usSuccessWorldBuff)
		<< BUILD_LOG(m_usWorldBuffDurationTime)
		<< END_LOG;
}

void KWorldMissionManager::Reset_LUA()
{
	m_kTimerDOT.restart();
	
	START_LOG( cout, L"���� �̼� ������ ����ó���߽��ϴ�." )
		<< END_LOG;
}

int KWorldMissionManager::IncreaseProtectedCrystalCount( int ProtectedCrystalCount )
{ 
	if( ProtectedCrystalCount <= 0)
	{
		START_LOG( cerr, L"��ȣ�� ������ ���� �̻��ϴ�." )
			<< BUILD_LOG( ProtectedCrystalCount )
			<< END_LOG;

		return m_iProtectedCrystalCount;
	}

	if( IsActive() == true )
	{
		m_iProtectedCrystalCount += ProtectedCrystalCount;
	}

	//{{ 2012. 04. 2	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	int iPrevRewardBuffLevel = 0;
	int iAfterRewardBuffLevel = 0;

	int iPrevWorldCrystalCount = m_iWorldCrystalCount;

	if( DEFENSE_REWARD_BUFF_LEVEL4_NEEDS <= m_iWorldCrystalCount )
	{
		iPrevRewardBuffLevel = 4;
	}
	else if( DEFENSE_REWARD_BUFF_LEVEL3_NEEDS <= m_iWorldCrystalCount )
	{
		iPrevRewardBuffLevel = 3;
	}
	else if( DEFENSE_REWARD_BUFF_LEVEL2_NEEDS <= m_iWorldCrystalCount )
	{
		iPrevRewardBuffLevel = 2;
	}
	else if ( DEFENSE_REWARD_BUFF_LEVEL1_NEEDS <= m_iWorldCrystalCount )
	{
		iPrevRewardBuffLevel = 1;
	}

	CheckWorldCrystalCount();	// ���� ũ����Ż ���� ���� : ���� ��ų�� ����ϴ��� �˻�

	if( DEFENSE_REWARD_BUFF_LEVEL4_NEEDS <= m_iWorldCrystalCount )
	{
		iAfterRewardBuffLevel = 4;
	}
	else if( DEFENSE_REWARD_BUFF_LEVEL3_NEEDS <= m_iWorldCrystalCount )
	{
		iAfterRewardBuffLevel = 3;
	}
	else if( DEFENSE_REWARD_BUFF_LEVEL2_NEEDS <= m_iWorldCrystalCount )
	{
		iAfterRewardBuffLevel = 2;
	}
	else if ( DEFENSE_REWARD_BUFF_LEVEL1_NEEDS <= m_iWorldCrystalCount )
	{
		iAfterRewardBuffLevel = 1;
	}

	if( 0 < ( iAfterRewardBuffLevel - iPrevRewardBuffLevel ) )
	{
		BroadCastIndividualBuffLevel( iAfterRewardBuffLevel );
	}

	if( iPrevWorldCrystalCount < GATE_OF_THE_DARKNESS_HARD )
	{
		if( GATE_OF_THE_DARKNESS_EXPERT <= m_iWorldCrystalCount )
		{
			// ��� ���Ӽ����� ��ε� ĳ����
			KEGS_NOTIFY_MSG_NOT kPacket;
			kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_THE_GATE_OF_DARKNESS;
			kPacket.m_Count = 1;
			kPacket.m_wstrMSG = L"������ ���ݿ� ���������� �����մϴ�.";

			UidType anTrace[2] = { 0, -1 };
			KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, ECN_NOTIFY_MSG_NOT, kPacket );
			BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_UPDATE );
		}
		else if( GATE_OF_THE_DARKNESS_HARD <= m_iWorldCrystalCount )
		{
			// ��� ���Ӽ����� ��ε� ĳ����
			KEGS_NOTIFY_MSG_NOT kPacket;
			kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_THE_GATE_OF_DARKNESS;
			kPacket.m_Count = 1;
			kPacket.m_wstrMSG = L"������ ������ ���� �ż����ϴ�.";

			UidType anTrace[2] = { 0, -1 };
			KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, ECN_NOTIFY_MSG_NOT, kPacket );
			BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_UPDATE );
		}
	}
	else if( iPrevWorldCrystalCount < GATE_OF_THE_DARKNESS_EXPERT )
	{
		if( GATE_OF_THE_DARKNESS_EXPERT <= m_iWorldCrystalCount )
		{
			// ��� ���Ӽ����� ��ε� ĳ����
			KEGS_NOTIFY_MSG_NOT kPacket;
			kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_THE_GATE_OF_DARKNESS;
			kPacket.m_Count = 1;
			kPacket.m_wstrMSG = L"������ ���ݿ� ���������� �����մϴ�.";

			UidType anTrace[2] = { 0, -1 };
			KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, ECN_NOTIFY_MSG_NOT, kPacket );
			BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_UPDATE );
		}
	}
#else
	CheckWorldCrystalCount();	// ���� ũ����Ż ���� ���� : ���� ��ų�� ����ϴ��� �˻�
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	return m_iProtectedCrystalCount; 
}

//{{ 2012. 04. 24	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
void KWorldMissionManager::ResetProtectedCrystalCount( void )
{
	if( IsActive() == false )
	{
		return;
	}

	//{{ 2012. 04. 29	�ڼ���	����� �� ����
	int iPrevRewardBuffLevel = 0;

	if( DEFENSE_REWARD_BUFF_LEVEL4_NEEDS <= m_iWorldCrystalCount )
	{
		iPrevRewardBuffLevel = 4;
	}
	else if( DEFENSE_REWARD_BUFF_LEVEL3_NEEDS <= m_iWorldCrystalCount )
	{
		iPrevRewardBuffLevel = 3;
	}
	else if( DEFENSE_REWARD_BUFF_LEVEL2_NEEDS <= m_iWorldCrystalCount )
	{
		iPrevRewardBuffLevel = 2;
	}
	else if ( DEFENSE_REWARD_BUFF_LEVEL1_NEEDS <= m_iWorldCrystalCount )
	{
		iPrevRewardBuffLevel = 1;
	}

	m_iProtectedCrystalCount = 0;
	m_iWorldCrystalCount = 0;

	int _buff = CXSLBuffManager::BTI_NONE;

#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF
#else //SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF
	if( m_iWorldCrystalCount >= WBSC_STEP_ONE && m_iWorldCrystalCount < WBSCL_STEP_TWO && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS ) // 1�ܰ� ���� ���� �ߵ� ����
	{
		_buff = CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS;
	}
	else if( m_iWorldCrystalCount >= WBSCL_STEP_TWO && m_iWorldCrystalCount < WBSC_STEP_THREE && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON ) // 2�ܰ� ���� ���� �ߵ� ����
	{
		_buff = CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON;
	}
	else if( m_iWorldCrystalCount >= WBSC_STEP_THREE && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL ) // 3�ܰ� ���� ���� �ߵ� ����
	{
		_buff = CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL;
	}

	if( _buff >= CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS )
	{
		BroadCastWorldBuff( _buff , true );
	}
#endif //SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF

	if( iPrevRewardBuffLevel != 1 )
	{
		BroadCastIndividualBuffLevel( 1 );
	}
}

void KWorldMissionManager::GetWorldMissionInfo( IN const int& iType, OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck )
#else
void KWorldMissionManager::GetWorldMissionInfo( OUT KEGB_GET_MISSION_INFO_ACK &kPacketAck )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}
{
	kPacketAck.m_bActive = IsActive();
	kPacketAck.m_iMissionTime = m_iMissionTime;
	kPacketAck.m_iDungeonPlayTime = m_iDungeonPlayTime;
	kPacketAck.m_iWorldCrystalCount = GetWorldCrystalCount();

	//{{ 2012. 04. 24	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	if( iType == KEGB_UPDATE_MISSION_INFO_NOT::FT_INIT )
	{
		kPacketAck.m_mapDefenseOpenTime = m_mapDefenseOpenTime;
	}
	kPacketAck.m_iFlag = iType;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
}

void KWorldMissionManager::BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FLAG_TYPE eType )
{
	KEGB_UPDATE_MISSION_INFO_NOT	kPacket;
	
	//{{ 2012. 04. 24	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	GetWorldMissionInfo( eType, kPacket );
#else
	GetWorldMissionInfo( kPacket );
	kPacket.m_iFlag = eType;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	
	// ��� ���Ӽ����� ��ε� ĳ����
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_MISSION_INFO_NOT, kPacket );
}

void KWorldMissionManager::BroadCastWorldBuff( int _skill, bool bIsEnd /* false */ )
{
	KEGB_UPDATE_WORLD_BUFF_NOT	kPacket;

	if( _skill != CXSLBuffManager::BTI_NONE 
		&& _skill < CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS 
		&&  _skill > CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL )
	{
		START_LOG( cerr, L"���� ��ų ������ �̻��մϴ�." )
			<< BUILD_LOG(_skill)
			<< END_LOG;
		return;
	}
	
	m_usSuccessWorldBuff = static_cast<u_short>(_skill);

	kPacket.m_bActive = true;
	kPacket.m_usSuccessWorldBuff = m_usSuccessWorldBuff;
	kPacket.m_usWorldBuffDurationTime = m_usWorldBuffDurationTime;

	if( bIsEnd == true )	// ���� �̼� ����
	{
		kPacket.m_bActive = false;
	}
	
	// ��� ���Ӽ����� ��ε� ĳ����
	UidType anTrace[2] = { 0, -1 };

	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_WORLD_BUFF_NOT, kPacket );
}

void KWorldMissionManager::BroadCastMissionStartTime()
{
	KEGB_UPDATE_MISSION_START_TIME_NOT kPacket;
	kPacket.m_wsrtStartTime = m_tToDayMissionTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	// ��� ���Ӽ����� ��ε� ĳ����
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_MISSION_START_TIME_NOT, kPacket );
}

void KWorldMissionManager::StartMission( bool IsReStart /* false */ )
{
	//	�̺�Ʈ ����
	m_bActive = true;
	m_tToDayStartTime = CTime::GetCurrentTime();

	//{{ 2012. 05. 03	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	m_bRewardBuff = true;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	if( IsReStart == false )
	{
		InitMissionData();
		
		//{{ 2012. 04. 26	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		BroadCastIndividualBuffLevel( 0 );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
	}
	else
	{
		SetMissionData();
		
		//{{ 2012. 04. 26	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		BroadCastIndividualBuffLevel();
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
	}
	
	BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_BEGIN );

	BroadCastWorldBuff( m_usSuccessWorldBuff );

	START_LOG( cout2, L"����̼� ����!" )
		<< END_LOG;
}

void KWorldMissionManager::EndMission()
{
	m_bActive = false;
	m_bWorldBuffTime = (GetWorldCrystalCount() >= WBSC_STEP_ONE) ? true : false;
	m_kTimerWorldBuff.restart();
	m_bIsFirstTick = true;
	SetMissionTime( 0 );

	bool bSuccess = (m_iProtectedCrystalCount >= m_iCrystalTargetCount);

	//	�̺�Ʈ ����
	if( bSuccess )	//	����
	{
		START_LOG( cout2, L"����̼� ����!" )
			<< BUILD_LOG( m_iCrystalTargetCount )
			<< BUILD_LOG( m_iProtectedCrystalCount )
			<< END_LOG;

		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_SUCCESS );
		CheckWorldCrystalCount( true );
		WriteWorldMissionCrystalCount( true );
	}
	else
	{
		START_LOG( cout2, L"����̼� ����!" )
			<< BUILD_LOG( m_iCrystalTargetCount )
			<< BUILD_LOG( m_iProtectedCrystalCount )
			<< END_LOG;

		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_FAIL );
		CheckWorldCrystalCount( true );
		WriteWorldMissionCrystalCount( true );
	}
}

// ġƮ��
void KWorldMissionManager::SetMissionSuccess_LUA(bool bSuccess)
{
	if( bSuccess )	//	����
	{
		START_LOG( cout2, L"����̼� ���� ���·� ���� ����!" )
			<< END_LOG;

		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_SUCCESS );
	}
	else
	{
		START_LOG( cout2, L"����̼� ���� ���·� ���� ����!" )
			<< END_LOG;

		BroadCastMission( KEGB_UPDATE_MISSION_INFO_NOT::FT_FAIL );
	}		
}

void KWorldMissionManager::BuffCheat_LUA( int iBuffType, bool bForce )
{
	START_LOG( cout, L"���� ���� ġƮ ����մϴ�!" )
		<< BUILD_LOG( iBuffType )
		<< BUILD_LOG( bForce );

	BroadCastWorldBuff( iBuffType, bForce );
}

void KWorldMissionManager::WorldMissionDump_LUA()
{
	m_iTotalCCU = SiKGlobalServerCCUManager()->GetNowTotalCCU();

	START_LOG( cout, L"WorldMission ���� üũ" )
		<< BUILD_LOG( m_iTotalCCU )
		<< BUILD_LOG( m_iMissionTime )
		<< BUILD_LOG( m_iNextDOT )
		<< BUILD_LOG( m_usWorldBuffDurationTime )
		<< BUILD_LOG( m_usWorldBuffDurationTime )
		<< BUILD_LOG( m_iCrystalTargetCount )
		<< BUILD_LOG( m_iProtectedCrystalCount )
		<< BUILD_LOG( m_fCountPerWorldCrystal )
		<< BUILD_LOG( m_iWorldCrystalCount )
		<< BUILD_LOG( m_fNormalPlayRate )
		<< BUILD_LOG( m_fHardPlayRate )
		<< BUILD_LOG( m_fExpertPlayRate )
		<< BUILD_LOG( m_fNormalSuccessRate )
		<< BUILD_LOG( m_fHardSuccessRate )
		<< BUILD_LOG( m_fExpertSuccessRate )
		<< END_LOG;
}

//{{ 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
//#ifdef SERV_INSERT_GLOBAL_SERVER
void KWorldMissionManager::SetDefenseOpenTime_LUA( int iDayOfWeek, int iStartHour, int iStartMin, int iDurationTime )
{
	if( (iDayOfWeek < 1  || iDayOfWeek > 7  ) ||		// ��~�� ���̰� �ƴҶ� ����üũ
		(iStartHour < 0  || iStartHour > 23 ) ||		// �ð� 0~23 ���̰� �ƴҶ�
		(iStartMin  < 0  || iStartMin  > 59 ) ||		// �� 0~59 ���̰� �ƴҶ�
		(iDurationTime < 1) )							// �����ð��� 1 ���� ������ 
	{
		START_LOG( cerr, L"���潺 ���� ���� ���� ���� �̻��մϴ�." )
			<< BUILD_LOG( iDayOfWeek )
			<< BUILD_LOG( iStartHour )
			<< BUILD_LOG( iStartMin )
			<< BUILD_LOG( iDurationTime )
			<< END_LOG;
		return;
	}

	KDefenseOpenTime _info;
	_info.m_iStartHour = iStartHour;
	_info.m_iStartMinute = iStartMin;
	_info.m_iDurationTime = iDurationTime;

	std::map< int, std::vector< KDefenseOpenTime > >::iterator mit = m_mapDefenseOpenTime.find( iDayOfWeek );
	if( mit != m_mapDefenseOpenTime.end() )
	{
		std::vector< KDefenseOpenTime >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end() ; ++vit )
		{
			if( vit->m_iStartHour == iStartHour && vit->m_iStartMinute == iStartMin )	// �ߺ� �Ǵ� ������ ����
			{
				START_LOG( cerr, L"���潺 ���� ���� �ð��� �ߺ� �Ǿ����ϴ�." )
					<< BUILD_LOG( iDayOfWeek )
					<< BUILD_LOG( iStartHour )
					<< BUILD_LOG( iStartMin )
					<< BUILD_LOG( iDurationTime )
					<< END_LOG;
				return;
			}
		}
	
		mit->second.push_back( _info );
	}
	else
	{
		std::vector< KDefenseOpenTime > vecTemp;
		vecTemp.push_back( _info );
		m_mapDefenseOpenTime.insert( std::make_pair( iDayOfWeek, vecTemp) );
	}
	START_LOG( clog, L"���潺 ���� ���� �ð� �߰��� �Ϸ� �Ǿ����ϴ�." )
		<< BUILD_LOG( iDayOfWeek )
		<< BUILD_LOG( iStartHour )
		<< BUILD_LOG( iStartMin )
		<< BUILD_LOG( iDurationTime )
		<< END_LOG;
}

bool KWorldMissionManager::SetTodayMissionTime()
{
	// ���Ϻ� ���� �ð� ���
	m_tToDayMissionTime = CTime::GetCurrentTime();
	std::map< int, std::vector< KDefenseOpenTime > >::iterator mit = m_mapDefenseOpenTime.find( m_tToDayMissionTime.GetDayOfWeek() );
	if( mit == m_mapDefenseOpenTime.end() )
	{
		START_LOG( cerr, L"���潺 ���� �ð� ���� ���� ���� ���� �ʽ��ϴ�." );
		return false;
	}

	// ������ ���� �ǰ� �ִ� �ð��ΰ��� �˾ƺ���
	//{{ 2012. 05. 03	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	{
		const std::vector< KDefenseOpenTime > vecTime = mit->second;
		// �ش� ���Ͽ� ����� ���� �ð����� ��ȸ�ϸ鼭, ���� �ð��뿡 �´� ������ �ִ��� �˻��Ѵ�.
		for( std::vector< KDefenseOpenTime >::const_reverse_iterator rit = vecTime.rbegin(); rit != vecTime.rend(); ++rit )
		{
			const KDefenseOpenTime& kDefenseOpenTime = *rit;
			CTime tEndTime = CTime( m_tToDayMissionTime.GetYear(), m_tToDayMissionTime.GetMonth(), m_tToDayMissionTime.GetDay(), kDefenseOpenTime.m_iStartHour, kDefenseOpenTime.m_iStartMinute + kDefenseOpenTime.m_iDurationTime, 0 );
			CTime tBuffEndTime = tEndTime + CTimeSpan( 0, 1, 0, 0 );

			std::wstring wstrEndTime = tEndTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			std::wstring wstrBuffEndTime = tBuffEndTime.Format(_T("%Y-%m-%d %H:%M:%S"));

			if( tEndTime <= m_tToDayMissionTime && tBuffEndTime > m_tToDayMissionTime )
			{
				m_bRewardBuff = true;
				break;;
			}
		}
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	// ���� �ð�
	std::vector< KDefenseOpenTime > vecTime = mit->second;
	std::vector< KDefenseOpenTime >::iterator vit = vecTime.begin();
	for( ; vit != vecTime.end() ; ++vit )
	{
		if( vit->m_iStartHour > m_tToDayMissionTime.GetHour() )
		{
			m_kTimerDOT.restart();
			m_iNextDOT = (((vit->m_iStartHour - m_tToDayMissionTime.GetHour() ) * 60 ) + ( vit->m_iStartMinute - m_tToDayMissionTime.GetMinute() ) ) * 60 - ( m_tToDayMissionTime.GetSecond()) ; 
			SetMissionTime( vit->m_iDurationTime * 60 );

			m_tToDayMissionTime = CTime( m_tToDayMissionTime.GetYear(), m_tToDayMissionTime.GetMonth(), m_tToDayMissionTime.GetDay(), vit->m_iStartHour, vit->m_iStartMinute, 0 );

			START_LOG( cout, L"���潺 ���� �ð� ���� �Ϸ�!!" )
				<< BUILD_LOG(vit->m_iStartHour)
				<< BUILD_LOG(vit->m_iStartMinute)
				<< BUILD_LOG(m_iNextDOT)
				<< BUILD_LOG(m_iMissionTime);
			return true;
		}
		else if( vit->m_iStartHour == m_tToDayMissionTime.GetHour() )
		{
			if( vit->m_iStartMinute >= m_tToDayMissionTime.GetMinute() )
			{
				m_kTimerDOT.restart();
				m_iNextDOT = ( vit->m_iStartMinute - m_tToDayMissionTime.GetMinute() ) * 60 - ( m_tToDayMissionTime.GetSecond());
				SetMissionTime( vit->m_iDurationTime * 60 );

				m_tToDayMissionTime = CTime( m_tToDayMissionTime.GetYear(), m_tToDayMissionTime.GetMonth(), m_tToDayMissionTime.GetDay(), vit->m_iStartHour, vit->m_iStartMinute, 0 );

				START_LOG( cout, L"���潺 ���� �ð� ���� �Ϸ�!!" )
					<< BUILD_LOG(vit->m_iStartHour)
					<< BUILD_LOG(vit->m_iStartMinute)
					<< BUILD_LOG(m_iNextDOT)
					<< BUILD_LOG(m_iMissionTime);
				return true;
			}
		}

		int iStartTimeTotalMin = (vit->m_iStartHour * 60) + vit->m_iStartMinute + vit->m_iDurationTime;
		int iNowTimeTotalMin = m_tToDayMissionTime.GetHour() * 60 + m_tToDayMissionTime.GetMinute();

		if( iStartTimeTotalMin > iNowTimeTotalMin )		// ����- �̺�Ʈ ���� ������ ���ȴ� �ٽ� ������ ��� �̺�Ʈ �ð� �ʱ�ȭ
		{
			// ��� ���� �ʱ�ȭ
			m_iNextDOT = 0;
			m_kTimerDOT.restart();

			// ���� ������ ���� �ð�
			CTime tCurTime = CTime::GetCurrentTime();			
			m_tToDayMissionTime = CTime( m_tToDayMissionTime.GetYear(), m_tToDayMissionTime.GetMonth(), m_tToDayMissionTime.GetDay(), vit->m_iStartHour, vit->m_iStartMinute, 0 );

			// �̼� ���� �ð�
			CTimeSpan gap = tCurTime - m_tToDayMissionTime;
			const int iMissionTime =  static_cast<int>((vit->m_iDurationTime * 60) - gap.GetTotalSeconds());
			SetMissionTime( iMissionTime );
			
			// DB�� ����� ���� ��������
			m_bWorldMissionOn = false;                          // �ٸ� �۾��� ���� �ϵ��� �Ѵ�.
			KDBE_GET_DEFENCE_CRYSTAL_REQ KReq;
			KReq.m_wsrtStartTime = m_tToDayMissionTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			SendToLogDB( DBE_GET_DEFENCE_CRYSTAL_REQ, KReq );
			
			START_LOG( cout, L"���潺 ���� �ð� ���� �Ϸ�!!" )
				<< BUILD_LOG(vit->m_iStartHour)
				<< BUILD_LOG(vit->m_iStartMinute)
				<< BUILD_LOG(m_iNextDOT)
				<< BUILD_LOG(m_iMissionTime)
				<< END_LOG;
			return true;
		}
	}

	m_bActive = false;
	m_iNextDOT = 0;
	SetMissionTime( 0 );
	START_LOG( clog, L"���� ���潺 ���� �ð� �������ϴ�." );

	return false;
}

void KWorldMissionManager::CheckWorldMissionProcess()
{
	//{{ 2012. 03. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	IndividualBuffEndProcess();
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	if( m_bWorldMissionOn == true && m_bWorldBuffTime == false )
	{
		if( m_bActive == false )
		{
			if( m_iMissionTime <= 0 )
				return;

			if( (int)m_kTimerDOT.elapsed() < m_iNextDOT )
				return;
			
			StartMission();
		}
		else
		{
			if( m_iMissionTime > GetCorrectMissionTime() )
				return;

			EndMission();
		}
	}

	if( m_bWorldBuffTime && m_kTimerWorldBuff.elapsed() >= m_usWorldBuffDurationTime ) //   ���� �ð� ���� // ���� Ÿ���̰� ���ӽð��� ������
	{
		m_bWorldBuffTime = false;
		m_usSuccessWorldBuff = CXSLBuffManager::BTI_NONE;	// ���� ���� �ʱ�ȭ
		m_kTimerWorldBuff.restart();	// �ð� �ʱ�ȭ
		m_iProtectedCrystalCount = 0;
		m_iWorldCrystalCount = 0;
		BroadCastWorldBuff( static_cast<int>(m_usSuccessWorldBuff), true );
	}
}

void KWorldMissionManager::CheckWorldCrystalCount( bool flag /* false */ )
{
	int _buff = CXSLBuffManager::BTI_NONE;
	
	if( flag == true ) // ���� �̼��� ���� �� ����
	{
		_buff = m_usSuccessWorldBuff;
	}

	// 0������ �˻�
	if( m_fCountPerWorldCrystal == 0 )
	{
		START_LOG( cerr, L"���� ũ����Ż �ϳ��� �������� 0�̴�! ����� �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( m_fCountPerWorldCrystal )
			<< END_LOG;

		m_fCountPerWorldCrystal = 1.0f;
	}

	if( (m_iWorldCrystalCount + 1) * m_fCountPerWorldCrystal <= m_iProtectedCrystalCount )
	{
		m_iWorldCrystalCount = static_cast<int>(m_iProtectedCrystalCount / m_fCountPerWorldCrystal);
	
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF
#else //SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF
		if( m_iWorldCrystalCount >= WBSC_STEP_ONE && m_iWorldCrystalCount < WBSCL_STEP_TWO && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS ) // 1�ܰ� ���� ���� �ߵ� ����
		{
			_buff = CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS;
		}
		else if( m_iWorldCrystalCount >= WBSCL_STEP_TWO && m_iWorldCrystalCount < WBSC_STEP_THREE && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON ) // 2�ܰ� ���� ���� �ߵ� ����
		{
			_buff = CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON;
		}
		else if( m_iWorldCrystalCount >= WBSC_STEP_THREE && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL ) // 3�ܰ� ���� ���� �ߵ� ����
		{
			_buff = CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL;
		}
		
		if( _buff >= CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS )
		{
			BroadCastWorldBuff( _buff , flag );
		}
#endif //SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF
	}
}

int KWorldMissionManager::GetCorrectMissionTime()
{
	// ���� �ð��� ���� ������ ������ ��Ȯ�� �̼� �ð��� �����Ѵ�.
	CTime CurrentTime = CTime::GetCurrentTime();

	int iGap = ((CurrentTime.GetHour() - m_tToDayStartTime.GetHour()) * 60 * 60) + ((CurrentTime.GetMinute() - m_tToDayStartTime.GetMinute()) * 60) + (CurrentTime.GetSecond() - m_tToDayStartTime.GetSecond());
	
	if( iGap < 0 )
	{
		START_LOG( clog, L"���� �̼��� �������� �ʾҽ��ϴ�." )
			<< BUILD_LOG(m_iMissionTime)
			<< BUILD_LOG(iGap)
			<< END_LOG;
		return 0;
	}

	return iGap;
}

int KWorldMissionManager::GetWorldCrystalCount()
{ 
	if( m_iWorldCrystalCount > WBSC_STEP_THREE )	// ������ �ִ� ���� ���� ��� �ִ� ���� ����(client UI's crystal)
	{
		m_iWorldCrystalCount = WBSC_STEP_THREE;
	}

	START_LOG( clog, L"���� ũ����Ż ���� Ȯ��" )
		<< BUILD_LOG(m_iWorldCrystalCount)
		<< BUILD_LOG(m_iProtectedCrystalCount)
		<< BUILD_LOG(m_fCountPerWorldCrystal)
		<< END_LOG;

	return m_iWorldCrystalCount; 
}

void KWorldMissionManager::WriteWorldMissionCrystalCount( bool bforce /*false*/ )
{
	if( m_bActive  ||  bforce )
	{
		//	Log DB�� �������
		KDBE_INCREASE_DEFENCE_CRYSTAL_REQ PacketReq;

		PacketReq.m_iTargetCrystal			= m_iCrystalTargetCount;
		PacketReq.m_iProtectedCrystalCount	= m_iProtectedCrystalCount;
		PacketReq.m_wsrtStartTime			= m_tToDayMissionTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		SendToLogDB( DBE_INCREASE_DEFENCE_CRYSTAL_REQ, PacketReq );
	}
}

void KWorldMissionManager::SetInitMissionInfo( KDBE_GET_DEFENCE_CRYSTAL_ACK KPacket )
{
	if( KncUtil::ConvertStringToCTime( KPacket.m_wsrtStartTime, m_tToDayMissionTime ) == false )
	{
		START_LOG( cerr, L"���� �̼� ���� �ð��� �̻��մϴ�. ��ȯ ����!" )
			<< BUILD_LOG( KPacket.m_wsrtStartTime )
			<< END_LOG;
		return;
	}

//	StartMission();

	// 0������ �˻�
	if( m_fCountPerWorldCrystal == 0 )
	{
		START_LOG( cerr, L"���� ũ����Ż �ϳ��� �������� 0�̴�! ����� �Ͼ�� �ȵǴ� ����!" )
			<< BUILD_LOG( m_fCountPerWorldCrystal )
			<< END_LOG;

		m_fCountPerWorldCrystal = 1.0f;
	}

	m_iProtectedCrystalCount = KPacket.m_iProtectedCrystalCount;
	m_iCrystalTargetCount = KPacket.m_iTargetCrystal;

	if( m_iCrystalTargetCount < WBSC_STEP_THREE ) // ���� ó�� Ȥ�� DB�� �߸� �������� ���� �����ϱ�
	{
		m_iCrystalTargetCount = 15;
		//m_iCrystalTargetCount = 1500;
	}
	m_fCountPerWorldCrystal = m_iCrystalTargetCount / static_cast<float>(WBSC_STEP_THREE);
	m_iWorldCrystalCount = static_cast<int>(m_iProtectedCrystalCount / m_fCountPerWorldCrystal);
	m_usSuccessWorldBuff = 0;

#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF
#else //SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF
	if( m_iWorldCrystalCount >= WBSC_STEP_ONE && m_iWorldCrystalCount < WBSCL_STEP_TWO && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS ) // 1�ܰ� ���� ���� �ߵ� ����
	{
		m_usSuccessWorldBuff = CXSLBuffManager::BTI_BUFF_THANKS_OF_RESIDENTS;
	}
	else if( m_iWorldCrystalCount >= WBSCL_STEP_TWO && m_iWorldCrystalCount < WBSC_STEP_THREE && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON ) // 2�ܰ� ���� ���� �ߵ� ����
	{
		m_usSuccessWorldBuff = CXSLBuffManager::BTI_BUFF_BREATH_OF_DRAGON;
	}
	else if( m_iWorldCrystalCount >= WBSC_STEP_THREE && m_usSuccessWorldBuff < CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL ) // 3�ܰ� ���� ���� �ߵ� ����
	{
		m_usSuccessWorldBuff = CXSLBuffManager::BTI_BUFF_BLESSING_OF_EL;
	}
#endif //SERV_REFORM_THE_GATE_OF_DARKNESS_NO_WORLD_BUFF

	m_bWorldMissionOn = true;

	StartMission( true );	
}

void KWorldMissionManager::SetMissionTime( int iTime )
{
	m_iMissionTime = iTime;
}

void KWorldMissionManager::SetMissionData()
{
	// ������ ����� ���� �� ����
	m_iTotalCCU = 0;
	m_iTotalCCU = SiKGlobalServerCCUManager()->GetNowTotalCCU(); // ����

	//{{ 2012. 04. 25	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	int iNormalCount	= static_cast<int>((( m_iTotalCCU * m_fNormalPlayRate)	/ 1 ) * m_fNormalSuccessRate	);
	int iHardCount		= static_cast<int>((( m_iTotalCCU * m_fHardPlayRate)	/ 3 ) * m_fHardSuccessRate		);
	int iExpertCount	= static_cast<int>((( m_iTotalCCU * m_fExpertPlayRate)	/ 4 ) * m_fExpertSuccessRate	);
#else
	int iNormalCount = static_cast<int>((( m_iTotalCCU * m_fNormalPlayRate) / 1 ) * m_fNormalSuccessRate * m_cNormalBasicCrystalCount );
	int iHardCount = static_cast<int>((( m_iTotalCCU * m_fHardPlayRate) / 3 ) * m_fHardSuccessRate * m_cHardBasicCrystalCount );
	int iExpertCount = static_cast<int>((( m_iTotalCCU * m_fExpertPlayRate) / 4 ) * m_fExpertSuccessRate * m_cExpertBasicCrystalCount );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	// ���� �۾����� �����Ǽ� �߰���.
	m_iCrystalTargetCount = iNormalCount + iHardCount + iExpertCount;

	if( m_iCrystalTargetCount < WBSC_STEP_THREE )
	{
		m_iCrystalTargetCount = 15;
		//m_iCrystalTargetCount = 1500;
	}

	CheckWorldCrystalCount();

	START_LOG( cout, L"����̼� �ʱ�ȭ" )
		<< BUILD_LOG(m_iCrystalTargetCount)
		<< BUILD_LOG(m_iProtectedCrystalCount)
		<< BUILD_LOG(m_fCountPerWorldCrystal)
		<< BUILD_LOG(m_iWorldCrystalCount)
		<< BUILD_LOG(m_iMissionTime)
		<< BUILD_LOG(m_iDungeonPlayTime)
		<< BUILD_LOG(m_iTotalCCU)
		<< BUILD_LOG(m_fNormalPlayRate)
		<< BUILD_LOG(m_fHardPlayRate)
		<< BUILD_LOG(m_fExpertPlayRate)
		<< BUILD_LOG(m_usSuccessWorldBuff)
		<< BUILD_LOG(m_usWorldBuffDurationTime)
		<< END_LOG;
}

//{{ 2012. 03. 23	�ڼ���	����� �� ����
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
void KWorldMissionManager::BroadCastIndividualBuffLevel( void ) const
{
	if( DEFENSE_REWARD_BUFF_LEVEL4_NEEDS <= m_iWorldCrystalCount )
	{
		BroadCastIndividualBuffLevel( 4 );
	}
	else if( DEFENSE_REWARD_BUFF_LEVEL3_NEEDS <= m_iWorldCrystalCount )
	{
		BroadCastIndividualBuffLevel( 3 );
	}
	else if( DEFENSE_REWARD_BUFF_LEVEL2_NEEDS <= m_iWorldCrystalCount )
	{
		BroadCastIndividualBuffLevel( 2 );
	}
	else if ( DEFENSE_REWARD_BUFF_LEVEL1_NEEDS <= m_iWorldCrystalCount )
	{
		BroadCastIndividualBuffLevel( 1 );
	}
}
void KWorldMissionManager::BroadCastIndividualBuffLevel( IN const int& iIndividualBuffLevel ) const
{
	// ��� ���Ӽ����� ��ε� ĳ����
	KEGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT kPacket;
	UidType anTrace[2] = { 0, -1 };

	kPacket.m_iTheGateOfDarknessIndividualBuffLevel = iIndividualBuffLevel;
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT, kPacket );
}
void KWorldMissionManager::IndividualBuffEndProcess( void )
{
	if( ( m_bRewardBuff == true ) && ( m_bActive == false ) )
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		CTime tNextStartTime = CTime( 2030 , 12 , 31, 23, 59, 0 ); // �׳� �̷��� �ð�����...
		// ���Ϻ� ���� �ð� ���
		std::map< int, std::vector< KDefenseOpenTime > >::const_iterator mit = m_mapDefenseOpenTime.find( tCurrentTime.GetDayOfWeek() );

		// �ش� ���Ͽ� ����� ���� �ð� ���� ���� �˻�.
		if( mit == m_mapDefenseOpenTime.end() )
		{
			return;
		}

		// ���� �ð�
		const std::vector< KDefenseOpenTime >& vecTime = mit->second;

		// �ش� ���Ͽ� ����� ���� �ð����� ��ȸ�ϸ鼭, ���� �ð��뿡 �´� ������ �ִ��� �˻��Ѵ�.
		for( std::vector< KDefenseOpenTime >::const_reverse_iterator rit = vecTime.rbegin(); rit != vecTime.rend(); ++rit )
		{
			const KDefenseOpenTime& kDefenseOpenTime = *rit;
			CTime tStartTime = CTime( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), kDefenseOpenTime.m_iStartHour, kDefenseOpenTime.m_iStartMinute, 0 );
			CTime tEndTime = tStartTime + CTimeSpan( 0, m_usWorldBuffDurationTime, kDefenseOpenTime.m_iDurationTime, 0 );

			if( tEndTime <= tCurrentTime && tNextStartTime > tCurrentTime )
			{
				m_bRewardBuff = false;
				BroadCastIndividualBuffLevel( 0 );
				return;
			}

			tNextStartTime = tStartTime;
		}
	}
}

#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ ������ ���� - ���� ȿ�� - ��μ�
#ifdef SERV_EARTH_QUAKE_EFFECT
void KWorldMissionManager::SetRandomEarthQuakeTime_LUA( int iSec, float fRate )
{
	m_kEarthQuakeLottery.AddCaseIntegerCast( iSec, fRate );
}

int KWorldMissionManager::GetRandomEarthQuakeTime()
{
	int iSec = m_kEarthQuakeLottery.Decision();

	if( iSec <= 0 )
	{
		START_LOG( cerr, L"������ �˷��ִ� �ð��� �ʹ� �����ų� �̻��ϴ�." )
			<< BUILD_LOG( iSec )
			<< END_LOG;

		iSec = 3600;
	}

	return iSec;
}
#endif SERV_EARTH_QUAKE_EFFECT
//}}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  ��μ�  �۷ι� ���� �߰�
//////////////////////////////////////////////////////////////////////////
