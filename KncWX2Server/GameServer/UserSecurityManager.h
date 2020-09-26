#pragma once
#include "Event.h"
#include "CommonPacket.h"

//{{ 2012. 04. 02	��μ�		sn ����͸� ��� �и�
#if defined(SERV_SERIAL_NUMBER_MORNITORING) || defined(SERV_IMPOSSIBLE_SELL_ITEM)  // ���� ������ �ؿ��� �߰�
	#include <boost/timer.hpp>
#endif /* SERV_SERIAL_NUMBER_MORNITORING or SERV_IMPOSSIBLE_SELL_ITEM */
//}}

//{{ 2011. 08. 17	������	�ӽ�ID �ߺ� ���� ����
#ifdef SERV_MACHINE_ID_BLOCK


class KUserSecurityManager
{
public:
	KUserSecurityManager(void);
	~KUserSecurityManager(void);	

	// ����
	const std::wstring GetWStringMachineID() const { return m_wstrMachineID; }
	const std::string GetStringMachineID() const;
	void SetMachineID( IN const std::string& strMachineID );	

	// �˻�
	bool CheckValidMachineID( IN const std::string& strMachineID ) const;

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	int GetChannelRandomKey()						{ return m_iChannelRandomKey; }
	void SetChannelRandomKey( IN int& iKey )		{ m_iChannelRandomKey = iKey; }
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	void SetServerSN( IN __int64& iServerSN )		{ m_iServerSN = iServerSN; }
	void GetServerSN( OUT __int64& iServerSN )		{ iServerSN = m_iServerSN; }
	//{{ 2012. 04. 02	��μ�		sn ����͸� ��� �и�
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	enum SERVER_SN_MONITORING
	{
		SSM_NONE		= 0,
		SSM_MONITORING	= 1,
		SSM_BLOCK		= 2,
	};

	void SetServerSN_Flag( IN int& iMonitoringFlag )		{ m_iMonitoringFlag = iMonitoringFlag; }
	bool CheckServerSN_Monitoring();
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
	
	//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	enum KICK_TYPE
	{
		KT_NONE					= 0,
		KT_AFTER_RANDOM_TIME	= 1,
	};

	void SetKickRandomTime( int iMinTime, int iRange );
	void SetKickState( IN char cState );
	bool CheckIsKickUser();
	int	 GetKickReason()					{ return m_iKickReason; }
	void SetKickReason( int iReason )		{ m_iKickReason = iReason; }
#endif SERV_IMPOSSIBLE_SELL_ITEM
	//}}


private:
	static WORD GetHashFromMachineID( IN const std::string& strMachineID );
	static WORD MakeHashFromMachineID( IN const std::string& strMachineID );

private:
	std::wstring					m_wstrMachineID;

	//{{ 2011. 09. 14  ��μ�	��ŷ ���μ��� ��� ���� - ���� �ε� ���� �ܰ�
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	int								m_iChannelRandomKey;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{  2012. 1. 16	��μ�		���Ӽ������� Server SN �˻�
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	__int64							m_iServerSN;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
	//{{ 2012. 04. 02	��μ�		sn ����͸� ��� �и�
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	boost::timer					m_TimerSNMonitoring;
	int								m_iMonitoringFlag;
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}
	//{{ 2012. 05. 23	��μ�       �Ǹ� �Ұ� ������ �Ǹ� �õ� ��ŷ ������ �Ǵ�
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
	int								m_iKickRandomTime;
	char							m_cKickState;
	boost::timer					m_tKickTimer;
	int								m_iKickReason;
#endif SERV_IMPOSSIBLE_SELL_ITEM
	//}}
};


#endif SERV_MACHINE_ID_BLOCK
//}}