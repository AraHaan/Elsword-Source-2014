#include "SimLayer.h"
#include "GlobalSimLayer.h"
#include "ServerSNManager.h"
#include "Socket/NetCommon.h"
#include "Enum/Enum.h"
#include "NetError.h"


//////////////////////////////////////////////////////////////////////////
//{{ 2012. 03. 29	��μ�		�ߺ� sn ���� ����
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//////////////////////////////////////////////////////////////////////////

ImplementRefreshSingleton( KServerSNManager );

KServerSNManager::KServerSNManager(void)
{
}

KServerSNManager::~KServerSNManager(void)
{
}

ImplToStringW( KServerSNManager )
{
	stm_	<< L"----------[ ServerSNManager ]----------" << std::endl
			<< m_mapServerSNManager.size()
			;

	return stm_;
}

void KServerSNManager::RegServerSN( IN __int64& iServerSN, IN UidType& iUserUID, IN UidType& iGSUID, OUT int& iResult, OUT std::map<UidType, UidType>& mapOverlapUser )
{
	// ���� ������ ServerSN �� ����Ѵ�.
	iResult = NetError::NET_OK;

	// �ߺ� ���� ���̸�...
	std::map< __int64, std::map<UidType, UidType> >::iterator mit = m_mapServerSNManager.find( iServerSN );
	if( mit != m_mapServerSNManager.end() )
	{
		iResult = NetError::ERR_SN_OVERLAP_01;

		START_LOG( cerr, L"ServerSN�� �ߺ� ���� ���̴�." )
			<< BUILD_LOG( iServerSN )
			<< END_LOG;

		mit->second.insert( std::make_pair(iUserUID, iGSUID) );
		mapOverlapUser = mit->second;
		return;
	}

	std::map<UidType, UidType>	mapTemp;
	mapTemp.insert( std::make_pair(iUserUID,iGSUID) );
	m_mapServerSNManager.insert( std::make_pair( iServerSN, mapTemp ) );
}

void KServerSNManager::UnRegServerSN( IN __int64& iServerSN, IN UidType& iUserUID, OUT int& iResult )
{
	// ���� ���� ������ ServerSN �� �����Ѵ�.
	iResult = NetError::NET_OK;

	// �����Ϸ��µ� ������...
	std::map< __int64, std::map<UidType, UidType> >::iterator mit = m_mapServerSNManager.find( iServerSN );
	if( mit == m_mapServerSNManager.end() )
	{
		iResult = NetError::ERR_SN_OVERLAP_02;

		START_LOG( cerr, L"ServerSN �������� �ʴµ� �����Ϸ��Ѵ�." )
			<< BUILD_LOG( iServerSN )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;

		return;
	}

	if( mit->second.size() > 1 )
	{
		std::map<UidType, UidType>::iterator sit = mit->second.find( iUserUID );
		if( sit == mit->second.end() )
		{
			iResult = NetError::ERR_SN_OVERLAP_02;

			START_LOG( cerr, L"iUserUID�� �������� �ʴ´�." )
				<< BUILD_LOG( iServerSN )
				<< BUILD_LOG( iUserUID )
				<< END_LOG;
			return;
		}

		mit->second.erase( sit );
	}
	else
	{
		m_mapServerSNManager.erase( mit );
	}
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}
//////////////////////////////////////////////////////////////////////////
