#include "ChannelServer.h"
#include "CCUToolManager.h"


ImplementSingleton( KCCUToolManager );

KCCUToolManager::KCCUToolManager()
{	
}

KCCUToolManager::~KCCUToolManager()
{
}

ImplToStringW( KCCUToolManager )
{
	return stm_;
}

void KCCUToolManager::Tick()
{

}

const KCCUToolManager::CCUDateData* KCCUToolManager::GetCCUData( IN CCUDate iCCUDate )
{
	std::map< CCUDate, CCUDateData >::const_iterator mit = m_mapCCU.find( iCCUDate );
	if( mit == m_mapCCU.end() )
		return NULL;

	return &mit->second;
}

const KCCUToolManager::CCUDateData* KCCUToolManager::GetCCUData( IN CTime tCCUDate )
{
	CCUDate TodayCCUDate = tCCUDate.GetTime();
    return GetCCUData( TodayCCUDate );
}

bool KCCUToolManager::NewCCUData( IN UidType iServerID, IN CCUDate iCCUDate, IN std::vector< KCCUInfo > vecCCUInfo )
{
	CCUDateData mapCCUData;

	int iAfter = 0;
	int iBefore = mapCCUData.size();

	mapCCUData.insert( make_pair( iServerID, vecCCUInfo ) );
	
	iAfter = mapCCUData.size();

	if( iAfter == iBefore )
	{
		return false;
	}

	iBefore = iAfter = 0;

	iBefore = m_mapCCU.size();

	m_mapCCU.insert( make_pair( iCCUDate, mapCCUData ) );

	iAfter = m_mapCCU.size();

	if( iAfter == iBefore )
	{
		return false;
	}

	return true;
}

bool KCCUToolManager::SetCCUData( IN CCUDate iCCUDate, IN UidType iServerID, IN std::vector< KCCUInfo > vecCCUInfo, IN char cCCUType )
{
	std::map< CCUDate, CCUDateData >::iterator mapit = m_mapCCU.find( iCCUDate );
	if( mapit == m_mapCCU.end() )
	{
		//[������] �α� ����
		START_LOG( cerr, L"���� �����Ͱ� ���µ� ���� �Ϸ� �Ѵ�!" )
			<< BUILD_LOG( iServerID )
			<< BUILD_LOG( iCCUDate )
			<< END_LOG;

		return false;
	}

	KCCUToolManager::CCUDateData::iterator mit = mapit->second.find( iServerID );
	if( mit == mapit->second.end() )
	{
		mapit->second.insert( make_pair( iServerID, vecCCUInfo ) );
	}
	else 
	{
		if( mit->second.size() != vecCCUInfo.size() )
		{
			mit->second = vecCCUInfo;
		}
	}

	return true;
}

bool KCCUToolManager::GetExistCCU( IN const KE_TOOL_GET_CCU_INFO_REQ& kReq, OUT KE_TOOL_GET_CCU_INFO_ACK& kAck )
{
	CTime tCutTime;
	CTime tNowTime = CTime::GetCurrentTime();

	if( KncUtil::ConvertStringToCTime( kReq.m_wstrDate, tCutTime ) == false )
	{
		START_LOG( cerr, L"�ð� ���� ����!" )
			<< BUILD_LOG( kReq.m_wstrDate )
			<< BUILD_LOG( kReq.m_iServerUID )
			<< BUILD_LOG( kReq.m_cCCUType )
			<< END_LOG;
		return false;
	}

	kAck.m_cCCUType = kReq.m_cCCUType;
	kAck.m_iServerUID = kReq.m_iServerUID;	
	kAck.m_wstrDate = kReq.m_wstrDate;

	KCCUInfo tempInfo;

			
	const CCUDateData* pCCUData = GetCCUData( tCutTime.GetTime() );
	if( pCCUData == NULL )
	{
		return false;
	}

	CCUDateData::const_iterator mit;
	mit = pCCUData->find( kReq.m_iServerUID );
	if( mit != pCCUData->end() ) 
	{
		if( mit->second.size() > 0 )	// vector�� ����� 0 �ƴҶ� 
		{
			tempInfo = mit->second.back();
			kAck.m_vecCCUInfo = mit->second;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	
	switch( kReq.m_cCCUType )
	{
	case KE_TOOL_GET_CCU_INFO_REQ::CT_TODAY:
		{	
			if( tempInfo.m_iCCUTime + (60*1) <= tNowTime.GetTime() )	// DB�� ����� �ð� ������ 1�д��̴�.
			{
				// �� ���� ���ſ�
				return false;
			}
		}break;
	case KE_TOOL_GET_CCU_INFO_REQ::CT_YESTERDAY:
	case KE_TOOL_GET_CCU_INFO_REQ::CT_DATE:
		{
			CTime tCompTime = CTime( tCutTime.GetYear(), tCutTime.GetMonth(), tCutTime.GetDay(), 23, 59, 0 );

			if( tempInfo.m_iCCUTime < tCompTime.GetTime() )				// ������ �ð��� 11:59 �ΰ�?
			{
				// �� ���� ���ſ�
				return false;
			}
		}break;
	default:
		{
			START_LOG( cerr, L"�̻��� Ÿ���Դϴ�" )
				<< BUILD_LOGc( kReq.m_cCCUType )
				<< END_LOG;

			return false;
		}
	}

	return true;
}


