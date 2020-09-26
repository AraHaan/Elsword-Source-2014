

#include "GameServer.h"
#include ".\GSGlobalMissionManager.h"
#include "Enum/Enum.h"
#include "socket/NetCommon.h"

//{{ 2012. 09. 03	��ȫ��	�۷ι� �̼� �Ŵ���
#ifdef SERV_GLOBAL_MISSION_MANAGER

ImplementSingleton( KGSGlobalMissionManager );

KGSGlobalMissionManager::KGSGlobalMissionManager(void)
{	
	m_iTickCount = 0;
}

KGSGlobalMissionManager::~KGSGlobalMissionManager(void)
{
}

void KGSGlobalMissionManager::Init()
{
	// �۷ι� ������ ���� �̺�Ʈ ���� ������ ���
	KEGB_GET_MISSION_INFO_REQ kPacketReq;
	kPacketReq.m_iServerUID = KBaseServer::GetKObj()->GetUID();
	SendToGlobalServer( EGB_GET_GLOBAL_MISSION_INFO_REQ, kPacketReq );
}

void KGSGlobalMissionManager::UpdateGlobalMissionInfo( IN const KEGB_GET_GLOBAL_MISSION_INFO_ACK& kPacketAck )
{
	switch( kPacketAck.m_iFlag )
	{
	case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_NONE:
		{
			START_LOG( cerr, L"�޾Ƽ��� �ȵǴ� Ÿ��" )
				<< BUILD_LOG( kPacketAck.m_iFlag )
				<< END_LOG;

			return;
		} break;
		case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_INIT:
		case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_UPDATE:
		case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_BEGIN:
		{
			m_mapGlobalMissionInfo = kPacketAck.m_mapGlobalMissionInfo;

			if( kPacketAck.m_iFlag == KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_BEGIN )
			{
				m_kTimer.restart();
			}
		}break;
	case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_SUCCESS:
	case KEGB_GET_GLOBAL_MISSION_INFO_ACK::FT_FAIL:
		{
			m_mapGlobalMissionInfo = kPacketAck.m_mapGlobalMissionInfo;
		} break;
	default:
		{
			START_LOG( cerr, L"�޾Ƽ��� �ȵǴ� Ÿ��" )
				<< BUILD_LOG( kPacketAck.m_iFlag )
				<< END_LOG;

			return;
		}break;
	}

	m_iTickCount++;

	START_LOG( clog, L"�۷ι� �̼� ���� ����" )
		<< BUILD_LOG( m_iTickCount )
		<< END_LOG;
}

void KGSGlobalMissionManager::GetGlobalMissionInfo( OUT KEGS_GLOBAL_MISSION_UPDATE_NOT &kOut )
{
	kOut.m_mapGlobalMissionInfo = m_mapGlobalMissionInfo;
}

void KGSGlobalMissionManager::UpdateGlobalMissionStartTime( IN std::wstring wstrGlobalMissionDay, IN std::wstring& wstrDate )
{
/*
	std::map<int, KGlobalMissionOpenInfo>::iterator mit = m_mapGlobalMissionOpenInfo.find(iGlobalMissionID);
	if(mit != m_mapGlobalMissionOpenInfo.end())
	{
		mit->second.m_wstrScriptStartTime = wstrDate;
	}
*/
}

int KGSGlobalMissionManager::GetGlobalMissionStartRemainTime()
{
/*
	CTime tCurrent = CTime::GetCurrentTime();

	if( m_tWorldMissionNextStartTime > tCurrent )	// ���� �̼��� ���Ŀ� �����Ѵ�
	{
		CTimeSpan gap = m_tWorldMissionNextStartTime - tCurrent;

		return (int)gap.GetTotalMinutes();
	}
	else if( m_iMissionTime == 0 )					// ���� �̼� ���Ŀ� ����
	{
		return -1;
	}
	else											// ���� �̼��� ���� ���̴�
	{
		return 0;
	}
*/
	return 0;
}
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03  ��ȫ��	�۷ι� �̼� �Ŵ���