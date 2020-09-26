#include "SimLayer.h"
#include "GlobalSimLayer.h"
#include "BaseServer.h"
#include "GlobalMissionManager.h"
#include "Socket/NetCommon.h"
#include "Enum/Enum.h"
#include "GlobalServerCCUManager.h"

//////////////////////////////////////////////////////////////////////////
//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER
//////////////////////////////////////////////////////////////////////////

ImplementRefreshSingleton( KGlobalMissionManager );

KGlobalMissionManager::KGlobalMissionManager(void)
{
	Init();
}

KGlobalMissionManager::~KGlobalMissionManager(void)
{
}

void KGlobalMissionManager::Init()
{
	m_bGlobalMissionOn = false;
	m_bIsFirstTick = true;
	
	m_kTimerMissionStart.restart();
	m_kTimerEnableCheck.restart();
	m_kTimerUpdate.restart();

	m_mapGlobalMissionInfo.clear();
}

ImplToStringW( KGlobalMissionManager )
{
	stm_	<< L"----------[ Global Mission Manager ]----------" << std::endl
			<< TOSTRINGW( m_kTimerMissionStart.elapsed() )
			;

	KEGB_GET_GLOBAL_MISSION_INFO_ACK kPacket;
	SiKGlobalMissionManager()->GetGlobalMissionInfo( KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_INIT, kPacket );
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_GLOBAL_MISSION_INFO_NOT, kPacket );

	return stm_;
}

ImplementLuaScriptParser( KGlobalMissionManager )
{
	lua_tinker::class_add<KGlobalMissionManager>( GetLuaState(), "KGlobalMissionManager" );

	lua_tinker::class_def<KGlobalMissionManager>( GetLuaState(), "Reset",							&KGlobalMissionManager::Reset_LUA );

	lua_tinker::class_def<KGlobalMissionManager>( GetLuaState(), "SetMissionSuccess",				&KGlobalMissionManager::SetGlobalMissionSuccess_LUA );

	lua_tinker::class_def<KGlobalMissionManager>( GetLuaState(), "dump",							&KGlobalMissionManager::Dump );

	lua_tinker::class_def<KGlobalMissionManager>( GetLuaState(), "SetGlobalMissionOn",				&KGlobalMissionManager::SetGlobalMissionOn_LUA );

	lua_tinker::class_def<KGlobalMissionManager>( GetLuaState(), "GlobalMissionDump",				&KGlobalMissionManager::GlobalMissionDump_LUA );
	lua_tinker::decl( GetLuaState(), "GlobalMissionManager", this );
}

void KGlobalMissionManager::Tick()
{
	//////////////////////////////////////////////////////////////////////////
	// 10�� ���� Enableüũ
	if( m_kTimerEnableCheck.elapsed() < 10.0f )
		return;

	m_kTimerEnableCheck.restart();

	// �۷ι� ������ ó�� ������ GlobalMissionManager�� ù ��° ƽ�� ������ �� LogDB�� �۷ι� �̼� ���� ��û ��Ŷ�� ����
	if( m_bIsFirstTick == true && m_kTimerUpdate.elapsed() >= 10.0f )
	{
		// DB�� ����� �۷ι� �̼� ���� ��������
		KDBE_GET_GLOBAL_MISSION_INFO_REQ kReq;
		SendToLogDB( DBE_GET_GLOBAL_MISSION_INFO_REQ, kReq );

		m_bIsFirstTick = false;
		return;
	}

	// LogDB�κ��� �۷ι� �̼� ������ �޾����� �� ���Ӽ����� ��ε�ĳ����
	if( m_bReceiveGlobalMissionInfo == true && m_kTimerUpdate.elapsed() >= 10.0f)
	{
		if(SetTodayMissionTime() == true)
		{
			BroadCastGlobalMissionStartTime();
		}

		if(m_iNextGlobalMissionStartRemainTime != 0)
		{
			CTime tCurrentTime = CTime::GetCurrentTime();
			std::wstring wstrGlobalMissionBeginDate = tCurrentTime.Format(_T("%Y-%m-%d"));
			wstrGlobalMissionBeginDate += L" 00:00:00";

			BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_INIT, wstrGlobalMissionBeginDate );
		}
		
		m_bReceiveGlobalMissionInfo = false;
		return;
	}

	//	���Ӽ����� ���� ������Ʈ
	if( m_kTimerUpdate.elapsed() >= 10.0f )
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		std::wstring wstrGlobalMissionBeginDate = tCurrentTime.Format(_T("%Y-%m-%d"));
		wstrGlobalMissionBeginDate += L" 00:00:00";

		// ������ ������Ʈ �ϱ� ���ؼ��� ������ �������� ������ �Ѵ�.		
		BroadCastGlobalMissionStartTime();
		BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_UPDATE, wstrGlobalMissionBeginDate );
		m_kTimerUpdate.restart();
	}

	if( m_bGlobalMissionOn == true )		// EndMission() �� �Ҹ��� ���� �̺�Ʈ�� ����
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		
		if( tCurrentTime.GetHour() == 0 && tCurrentTime.GetMinute() == 0 && tCurrentTime.GetMinute() <= 1 )	// 00:00 �� ������ -> �������̴�
		{
			if(SetTodayMissionTime() == true)
			{
				BroadCastGlobalMissionStartTime();
			}			
			
			return;
		}
	}
	
	if( m_bIsFirstTick == false )
	{
		CheckGlobalMissionProcess();
	}
}

void KGlobalMissionManager::InitMissionData()
{

}

void KGlobalMissionManager::Reset_LUA()
{
	m_kTimerMissionStart.restart();
	
	START_LOG( cout, L"�۷ι� �̼� ���� ����" )
		<< END_LOG;
}

int KGlobalMissionManager::IncreaseGlobalMissionClearCount( std::wstring& wstrGlobalMissionDay, int iNowCount )
{
	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.find( wstrGlobalMissionDay );
	if( mit == m_mapGlobalMissionInfo.end() )
	{
		START_LOG( cerr, L"�������� �ʴ� �۷ι� �̼��̴�." )
			<< BUILD_LOG( wstrGlobalMissionDay )
			<< BUILD_LOG( iNowCount )
			<< END_LOG;

		return 0;
	}
	
	if(mit->second.m_bActive != true)
	{
		START_LOG( cerr, L"�̹� ���� �۷ι� �̼��̴�." )
			<< BUILD_LOG( wstrGlobalMissionDay )
			<< BUILD_LOG( iNowCount )
			<< END_LOG;

		return mit->second.m_iNowCount;
	}

	if( iNowCount <= 0 )
	{
		START_LOG( cerr, L"Ŭ���� ī��Ʈ ���� �̻��ϴ�." )
			<< BUILD_LOG( wstrGlobalMissionDay )
			<< BUILD_LOG( iNowCount )
			<< END_LOG;

		return mit->second.m_iNowCount;
	}

	mit->second.m_iNowCount += iNowCount;

	return mit->second.m_iNowCount;
}

void KGlobalMissionManager::GetGlobalMissionInfo( IN const int& iType, OUT KEGB_GET_GLOBAL_MISSION_INFO_ACK &kPacketAck )
{
	kPacketAck.m_iFlag = iType;
	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.begin();

	for(; mit != m_mapGlobalMissionInfo.end(); ++mit)
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		CTime tTodayStartTime(tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0);
		CTime tTodayEndTime(tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 23, 59, 59);

		CTime tGlobalMissionStartTime;
		KncUtil::ConvertStringToCTime(mit->first, tGlobalMissionStartTime);

		if(tTodayStartTime <= tGlobalMissionStartTime && tGlobalMissionStartTime < tTodayEndTime )
		{
			kPacketAck.m_mapGlobalMissionInfo.insert(std::make_pair(mit->first, mit->second));
		}
	}
}

void KGlobalMissionManager::BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FLAG_TYPE eType, std::wstring wstrGlobalMissionDay )
{
	KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT	kPacket;
	
	GetGlobalMissionInfo( eType, kPacket );
	
	switch(eType)
	{
	case KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_SUCCESS:
	case KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_FAIL:
		kPacket.m_wstrGlobalMissionDay = wstrGlobalMissionDay;
	}
	
	// ��� ���Ӽ����� ��ε� ĳ����
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_GLOBAL_MISSION_INFO_NOT, kPacket );
}
void KGlobalMissionManager::BroadCastGlobalMissionStartTime()	// ���ʿ��� �Լ� �ε�? ���� �Ϸᶧ���� ������ ������ �������� (�ۼ��Ͻ� : 2013.12.13 ��ȫ��)
{
	KEGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT kPacket;
	
	int iGlobalMissionID = 0;
	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.begin();
	for( ; mit != m_mapGlobalMissionInfo.end(); ++mit )
	{
		kPacket.m_mapGlobalMissionStartTime.insert(make_pair( iGlobalMissionID, mit->first ));
		iGlobalMissionID++;
	}

	// ��� ���Ӽ����� ��ε� ĳ����
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, EGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT, kPacket );
}

void KGlobalMissionManager::StartMission( std::wstring wstrGlobalMissionDay, bool IsReStart /* false */ )
{
	//	�̺�Ʈ ����
	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.find(wstrGlobalMissionDay);
	if(mit != m_mapGlobalMissionInfo.end())
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		mit->second.m_wstrMissionStartTime = tCurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		mit->second.m_bActive = true;
	}

	BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_BEGIN, wstrGlobalMissionDay );
	
	START_LOG( cout2, L"�۷ι� �̼� ����!" )
		<< BUILD_LOG( wstrGlobalMissionDay )
		<< END_LOG;
}

void KGlobalMissionManager::EndMission( std::wstring wstrGlobalMissionDay )
{
	m_bIsFirstTick = true;

	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.find( wstrGlobalMissionDay );
	if(mit != m_mapGlobalMissionInfo.end())
	{
		KGlobalMissionInfo* pkGlobalMissionInfo = &(mit->second);
		pkGlobalMissionInfo->m_iRemainSecond = 0;
		pkGlobalMissionInfo->m_bActive = false;
		bool bSuccess = (pkGlobalMissionInfo->m_iNowCount >= pkGlobalMissionInfo->m_iClearCount);
		
		//	�̺�Ʈ ����
		if( bSuccess )	//	����
		{
			START_LOG( cout2, L"�۷ι� �̼� ����!" )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second.m_iMissionType )
				<< BUILD_LOG( mit->second.m_iClearCondition )
				<< BUILD_LOG( mit->second.m_iClearCount )
				<< BUILD_LOG( mit->second.m_iNowCount )
				<< END_LOG;

			BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_SUCCESS, wstrGlobalMissionDay );
		}
		else
		{
			START_LOG( cout2, L"�۷ι� �̼� ����!" )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second.m_iMissionType)
				<< BUILD_LOG( mit->second.m_iClearCondition )
				<< BUILD_LOG( mit->second.m_iClearCount )
				<< BUILD_LOG( mit->second.m_iNowCount )
				<< END_LOG;

			BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_FAIL, wstrGlobalMissionDay );
		}

		WriteGlobalMissionInfo();
		//DB���� �����췯�� ���� �ϰ� ���� ���� ����
		//WriteGlobalMissionRewardInfo(pkGlobalMissionInfo, bSuccess); 
	}
}

// ġƮ��
void KGlobalMissionManager::SetGlobalMissionSuccess_LUA( bool bSuccess, std::wstring wstrGlobalMissionDay )
{
	if( bSuccess )	//	����
	{
		START_LOG( cout2, L"�۷ι� �̼� ���� ���·� ���� ����!" )
			<< END_LOG;

		BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_SUCCESS, wstrGlobalMissionDay );
	}
	else
	{
		START_LOG( cout2, L"�۷ι� �̼� ���� ���·� ���� ����!" )
			<< END_LOG;

		BroadCastGlobalMission( KEGB_UPDATE_GLOBAL_MISSION_INFO_NOT::FT_FAIL, wstrGlobalMissionDay );
	}		
}

void KGlobalMissionManager::GlobalMissionDump_LUA()
{
	START_LOG( cout, L"GlobalMission Check" )
		<< END_LOG;
}
bool KGlobalMissionManager::SetTodayMissionTime()
{
	CTime tTodayTime = CTime::GetCurrentTime();
	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.begin();
	m_iNextGlobalMissionStartRemainTime = 24 * 60 * 60;
	bool bSetMission = false;

	for( ; mit != m_mapGlobalMissionInfo.end(); ++mit )
	{
		CTime tGlobalMissionStartTime;
		KncUtil::ConvertStringToCTime(mit->first, tGlobalMissionStartTime);	// DB�� ��ϵ� �����ͻ� ���� �ð�

		CTimeSpan tTimeGap = tGlobalMissionStartTime - tTodayTime;

		if( tTimeGap.GetTotalSeconds() > 3600 )
		{
			m_kTimerMissionStart.restart();

			int iNextGlobalMissionStartRemainTime = tTimeGap.GetTotalSeconds(); 
			if(m_iNextGlobalMissionStartRemainTime > iNextGlobalMissionStartRemainTime)
			{
				m_iNextGlobalMissionStartRemainTime = iNextGlobalMissionStartRemainTime;
			}
				
			CTime tTodayStartTime = CTime( tTodayTime.GetYear(), tTodayTime.GetMonth(), tTodayTime.GetDay(), tGlobalMissionStartTime.GetHour(), tGlobalMissionStartTime.GetMinute(), 0 );
			mit->second.m_wstrMissionStartTime = tTodayStartTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			mit->second.m_iRemainSecond = mit->second.m_iDurationMinute * 60;

			START_LOG( clog, L"�۷ι� �̼� ����1" )
				<< BUILD_LOG(mit->first)
				<< BUILD_LOG(mit->second.m_wstrMissionStartTime)
				<< BUILD_LOG(mit->second.m_iDurationMinute)
				<< BUILD_LOG(mit->second.m_iRemainSecond)
				<< END_LOG;
			
			bSetMission = true;

			continue;
		}
		else if( tTimeGap.GetTotalSeconds() > 0 )
		{
			m_kTimerMissionStart.restart();

			int iNextGlobalMissionStartRemainTime = tTimeGap.GetTotalSeconds(); 
			if(m_iNextGlobalMissionStartRemainTime > iNextGlobalMissionStartRemainTime)
			{
				m_iNextGlobalMissionStartRemainTime = iNextGlobalMissionStartRemainTime;
			}

			CTime tTodayStartTime = CTime( tTodayTime.GetYear(), tTodayTime.GetMonth(), tTodayTime.GetDay(), tGlobalMissionStartTime.GetHour(), tGlobalMissionStartTime.GetMinute(), 0 );
			mit->second.m_wstrMissionStartTime = tTodayStartTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			mit->second.m_iRemainSecond = mit->second.m_iDurationMinute * 60;

			START_LOG( clog, L"�۷ι� �̼� ����2" )
				<< BUILD_LOG(mit->first)
				<< BUILD_LOG(mit->second.m_wstrMissionStartTime)
				<< BUILD_LOG(mit->second.m_iDurationMinute)
				<< BUILD_LOG(mit->second.m_iRemainSecond)
				<< END_LOG;

			bSetMission = true;

			continue;
		}

		if( tTimeGap.GetTotalMinutes() + mit->second.m_iDurationMinute > 0 )		// ���� : �̺�Ʈ ���� ������ ����۵� ��� �̺�Ʈ �ð� �ʱ�ȭ
		{
			// ��� ���� �ʱ�ȭ
			m_iNextGlobalMissionStartRemainTime = 0;
			m_kTimerMissionStart.restart();

			CTime tTodayMissionTime = CTime( tTodayTime.GetYear(), tTodayTime.GetMonth(), tTodayTime.GetDay(), tGlobalMissionStartTime.GetHour(), tGlobalMissionStartTime.GetMinute(), 0 );

			// �̼� ���� �ð�
			CTimeSpan gap = tTodayTime - tTodayMissionTime;
			mit->second.m_iRemainSecond =  static_cast<int>((mit->second.m_iDurationMinute * 60) - gap.GetTotalSeconds() );

			START_LOG( clog, L"�۷ι� �̼� ����3" )
				<< BUILD_LOG(mit->first)
				<< BUILD_LOG(mit->second.m_wstrMissionStartTime)
				<< BUILD_LOG(mit->second.m_iDurationMinute)
				<< BUILD_LOG(mit->second.m_iRemainSecond)
				<< END_LOG;

			bSetMission = true;

			continue;
		}

		mit->second.m_bActive = false;
		mit->second.m_iRemainSecond = 0;
	}

	return bSetMission;
}

void KGlobalMissionManager::CheckGlobalMissionProcess()
{
	if( m_bGlobalMissionOn == true )
	{
		std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.begin();
		for( ; mit != m_mapGlobalMissionInfo.end() ; ++mit )
		{
			if( mit->second.m_bActive == false )
			{
				if( mit->second.m_iRemainSecond <= 0 )
					continue;

				if( static_cast<int>(m_kTimerMissionStart.elapsed()) < m_iNextGlobalMissionStartRemainTime)
					continue;

				CTime tCurrentTime = CTime::GetCurrentTime();
				CTime tGlobalMissionStartTime;
				KncUtil::ConvertStringToCTime(mit->first, tGlobalMissionStartTime);	// DB�� �۷ι� �̼� ���� �ð�
				
				CTimeSpan gap = tCurrentTime - tGlobalMissionStartTime;
				if(gap.GetTotalSeconds() < 0)
					continue;

				StartMission( mit->first, false );
			}
			else
			{
				if( mit->second.m_iRemainSecond > GetCorrectMissionTime( mit->first ) )	// ���� �۷ι� �̼��� �ȳ������� ����.
					continue;

				EndMission( mit->first );
			}
		}
	}
}

int KGlobalMissionManager::GetCorrectMissionTime( std::wstring wstrGlobalMissonDay )
{
	// ���� �ð��� ���� ������ ������ ��Ȯ�� �̼� �ð��� �����Ѵ�.
	CTime tToDayStartTime;
	CTime tCurrentTime = CTime::GetCurrentTime();
	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.find( wstrGlobalMissonDay );
	if(mit != m_mapGlobalMissionInfo.end())
	{
		KncUtil::ConvertStringToCTime(mit->second.m_wstrMissionStartTime, tToDayStartTime);
	}

	CTimeSpan tTimeDiff = tCurrentTime - tToDayStartTime;
	int iGap = tTimeDiff.GetTotalSeconds();
	if( iGap < 0 )
	{
		START_LOG( clog, L"���� �̼��� �������� �ʾҽ��ϴ�." )
			<< BUILD_LOG(mit->second.m_iDurationMinute)
			<< BUILD_LOG(iGap)
			<< END_LOG;

		return 0;
	}

	return iGap;
}

void KGlobalMissionManager::WriteGlobalMissionInfo()
{
	KDBE_UPDATE_GLOBAL_MISSION_INFO_REQ kPacket;
	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.begin();
	
	for( ; mit != m_mapGlobalMissionInfo.end() ; ++mit )
	{
		if( mit->second.m_bActive )
		{
			kPacket.m_wstrGlobalMissionDay = mit->first;
			kPacket.m_iNowCount = mit->second.m_iNowCount;
			SendToLogDB( DBE_UPDATE_GLOBAL_MISSION_INFO_REQ, kPacket );

			return ;
		}
	}
}

void KGlobalMissionManager::SetInitMissionInfo( KDBE_GET_GLOBAL_MISSION_INFO_ACK kPacket )
{
	CTime tCurrentTime = CTime::GetCurrentTime();								
	std::wstring wstrGlobalMissionBeginDate = tCurrentTime.Format(_T("%Y-%m-%d"));
	wstrGlobalMissionBeginDate += L" 00:00:00";

	m_mapGlobalMissionInfo = kPacket.m_mapGlobalMissionInfo;

	std::map< std::wstring, KGlobalMissionInfo >::iterator mit = m_mapGlobalMissionInfo.find(wstrGlobalMissionBeginDate);
	if(mit != m_mapGlobalMissionInfo.end())
	{
		m_bGlobalMissionOn = true;
	}
	else
	{
		m_bGlobalMissionOn = false;
	}

	m_bReceiveGlobalMissionInfo = true;
}
/*
void KGlobalMissionManager::WriteGlobalMissionRewardInfo( KGlobalMissionInfo* pkGlobalMissionInfo, bool bSuccess )
{
	CTime tScriptStartTime;
	CTime tNextDayStartTime, tNextDayEndTime;
	KncUtil::ConvertStringToCTime(pkGlobalMissionInfo->m_wstrScriptStartTime, tScriptStartTime);
	
	if(bSuccess)
	{
		std::wstring wstrEventName = L"Invasion Defense [ Success ]";
		//KGlobalMissionReward pSuccessReward = pkGlobalMissionInfo->m_kGlobalMissionReward[0];
		// �׽�Ʈ��
		//tNextDayStartTime = tScriptStartTime + mit->second.m_iDurationTime * 60 + 60;
		//tNextDayEndTime = tNextDayStartTime + 10 * 60;
		// ���̺��
		tNextDayStartTime = tScriptStartTime + pkGlobalMissionInfo->m_iDurationMinute * 60 + 60;		// �۷ι� �̼��� ������ 1���ĺ���
		tNextDayEndTime = tNextDayStartTime + (23 * 60) * 60 + (59 * 60);
		//tNextDayEndTime = tNextDayStartTime + pSuccessReward.m_iRewardDurationTime * 60;
		std::wstring wstrNextDayStartTime = tNextDayStartTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		std::wstring wstrNextDayEndTime = tNextDayEndTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		//if(pSuccessReward.m_iRewardType == 1)						// ���� Ÿ���� �����̺�Ʈ�� ���
		//{
			KDBE_INSERT_CONNECT_EVENT_REQ kPacket;
			kPacket.m_wstrEventName = wstrEventName;
			kPacket.m_iEventType = 7;
			kPacket.m_wstrNextDayStartTime = wstrNextDayStartTime;
			kPacket.m_wstrNextDayEndTime = wstrNextDayEndTime;
			kPacket.m_iScriptID = 10124;
		//	kPacket.m_iScriptID = pSuccessReward.m_iScriptID;

			SendToLogDB( DBE_INSERT_CONNECT_EVENT_REQ, kPacket );
		//}
		//else
		//{
		//
		//}		
	}
	else
	{
		std::wstring wstrEventName = L"Invasion Defense [ Fail ]";
		//KGlobalMissionReward pFailReward = pkGlobalMissionInfo->m_kGlobalMissionReward[1];
		// �׽�Ʈ��
		//tNextDayStartTime = tScriptStartTime + mit->second.m_iDurationTime * 60 + 60;
		//tNextDayEndTime = tNextDayStartTime + 10 * 60;
		// ���̺��
		tNextDayStartTime = tScriptStartTime + pkGlobalMissionInfo->m_iDurationMinute * 60 + 60;		// �۷ι� �̼��� ������ 1���ĺ���
		tNextDayEndTime = tNextDayStartTime + (23 * 60) * 60 + (59 * 60);
		//tNextDayEndTime = tNextDayStartTime + pFailReward.m_iRewardDurationTime * 60;
		std::wstring wstrNextDayStartTime = tNextDayStartTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		std::wstring wstrNextDayEndTime = tNextDayEndTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		//if(pFailReward.m_iRewardType == 1)
		//{
			KDBE_INSERT_CONNECT_EVENT_REQ kPacket;
			kPacket.m_wstrEventName = wstrEventName;
			kPacket.m_iEventType = 7;
			kPacket.m_wstrNextDayStartTime = wstrNextDayStartTime;
			kPacket.m_wstrNextDayEndTime = wstrNextDayEndTime;
			kPacket.m_iScriptID = 10122;
		//	kPacket.m_iScriptID = pFailReward.m_iScriptID;

			SendToLogDB( DBE_INSERT_CONNECT_EVENT_REQ, kPacket );
		//}
		//else
		//{
		//
		//}		
	}
}
*/
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
//////////////////////////////////////////////////////////////////////////
