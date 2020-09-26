#include ".\usersecuritymanager.h"
//{{ 2011. 09. 06	������	�ӽ� ID ��� ������Ʈ
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
	#include "Base64/base64.h"
#endif SERV_MACHINE_ID_BLOCK_UPDATE
//}}
//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	#include "Statistics.h"
#endif SERV_IMPOSSIBLE_SELL_ITEM
//}}


//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_BLOCK


KUserSecurityManager::KUserSecurityManager(void)
{
	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	m_iChannelRandomKey	= 0;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	m_iServerSN	= 0;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
	//{{ 2012. 04. 02	��μ�		sn ����͸� ��� �и�
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	m_TimerSNMonitoring.restart();
	m_iMonitoringFlag = SSM_NONE;
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}

	//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	m_iKickRandomTime = 0;
	m_cKickState = KT_NONE;
	m_tKickTimer.restart();
	m_iKickReason = KStatistics::eSIColDR_No_Reason;
#endif SERV_IMPOSSIBLE_SELL_ITEM
	//}}
}

KUserSecurityManager::~KUserSecurityManager(void)
{
}

const std::string KUserSecurityManager::GetStringMachineID() const
{
	std::string strResult = KncUtil::toNarrowString( m_wstrMachineID );
	return strResult;
}

void KUserSecurityManager::SetMachineID( IN const std::string& strMachineID )
{
    m_wstrMachineID = KncUtil::toWideString( strMachineID );
}

WORD KUserSecurityManager::GetHashFromMachineID( IN const std::string& strMachineID )
{
	if( strMachineID.size() < 16 )
	{
		return 0;
	}

	WORD wHash = *(WORD*)&strMachineID[14];
	return wHash;
}

WORD KUserSecurityManager::MakeHashFromMachineID( IN const std::string& strMachineID )
{
	if( strMachineID.size() < 16 )
	{
		return 0;
	}

	// �ؽ��� üũ!
	BYTE MachineId[16] = {0, };

	for( int i = 0; i < 16; ++i )
	{
		MachineId[i] = strMachineID[i];
	}

	// fills Mac Address, Serial Number.
	WORD wHash = 0;
	for (int i=0; i<6; ++i)
	{
		wHash += ((WORD*)MachineId)[i] * (i+3);
	}

	return wHash;
}

bool KUserSecurityManager::CheckValidMachineID( IN const std::string& strMachineID ) const
{
	// �ӽ�ID�� ����ִ��� �˻�!
	if( strMachineID.empty() == true )
		return false;

	// �ӽ�ID ���� �˻�
	if( strMachineID.size() < 16 )
		return false;

	//{{ 2011. 09. 06	������	�ӽ� ID ��� ������Ʈ
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
	//machine id ��ȿ�� �˻� üũ �ʿ�
	//���ڵ� ó�� �����ϸ� ��.. �� base64 decode�� �˻��ؾ� ��
	std::string strDecodingMachineID = base64_decode( strMachineID );

	if( GetHashFromMachineID( strDecodingMachineID ) != MakeHashFromMachineID( strDecodingMachineID ) )
		return false;
#endif SERV_MACHINE_ID_BLOCK_UPDATE
	//}}

    return true;
}

#endif SERV_MACHINE_ID_BLOCK
//}}

//{{ 2012. 04. 02	��μ�		sn ����͸� ��� �и�
#ifdef SERV_SERIAL_NUMBER_MORNITORING
bool KUserSecurityManager::CheckServerSN_Monitoring()
{
	if( m_iMonitoringFlag == SSM_MONITORING )
	{
		if( m_TimerSNMonitoring.elapsed() < 30 )
		{
			return false;
		}
		
		m_TimerSNMonitoring.restart();

		return true;
	}

	return false;
}
#endif SERV_SERIAL_NUMBER_MORNITORING
//}}

//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
void KUserSecurityManager::SetKickRandomTime( int iMinTime, int iRange )
{
	if( iRange < 1 )
	{
		iRange = 1;
	}

	m_iKickRandomTime = rand() % iRange + iMinTime;
	m_tKickTimer.restart();

	START_LOG( clog2, L"���� �ð� �ڿ� ����ǰ� ���� �ð� ����" )
		<< BUILD_LOG( m_iKickRandomTime )
		<< END_LOG;
}

void KUserSecurityManager::SetKickState( IN char cState )
{
	m_cKickState = cState;

	START_LOG( clog2, L"���� ���� Ÿ�� ����" )
		<< BUILD_LOGc( m_cKickState )
		<< END_LOG;
}

bool KUserSecurityManager::CheckIsKickUser()
{
	if( m_cKickState == KT_AFTER_RANDOM_TIME )
	{
		if( m_tKickTimer.elapsed() < m_iKickRandomTime )
		{
			return false;
		}

		m_tKickTimer.restart();
		m_cKickState = KT_NONE;

		return true;
	}

	return false;
}
#endif SERV_IMPOSSIBLE_SELL_ITEM
//}}
