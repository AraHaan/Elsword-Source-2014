#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <set>
#include <vector>
#include <map>
#include <boost/timer.hpp>
#include "KncSend.h"

//{{ 2011. 09. 26  ��μ�	�ӽ� ID üũ - ���� �ε� ���� �ܰ�
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING

class KMachineBlockManager
{
	DeclareSingleton( KMachineBlockManager );
	DeclDump;
	DeclToStringW;

	enum ENUM_TIMER
	{		
		TM_RELEASE_TICK = 0,

		TM_TOTAL_NUM,
	};

	enum BLOCK_TYPE
	{
		BT_CONNECT_BLOCK = 0,
		BT_TRADE_BLOCK,
		BT_IP_LOGIN_BLOCK,	// �ؿ������� ��������
		BT_IP_TRADE_BLOCK,	// �ؿ������� ��������
		BT_CHAT_BLOCK,		// �ؿ������� ��������
		//{{ 2011. 08. 09	������	�ӽ�ID �����
//#ifdef SERV_MACHINE_ID_BLOCK
		BT_MACHINE_ID_BLOCK,// �ӽ�ID ��
//#endif SERV_MACHINE_ID_BLOCK
		//}}

		BT_MAX,
	};

public:
	KMachineBlockManager(void);
	~KMachineBlockManager(void);

	void Tick();

	void Init();
	void UpdateMachineBlockList( IN const KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK& kPacket, OUT std::vector< std::string >& vecBlockMachineIDList );
	
	void GetReleaseTick( OUT std::map< int, int >& mapReleaseTick );

	bool IsBlockedMachineID( IN const std::string& strMachineID ) const	{ return ( m_setMachineIDBlockList.find( strMachineID ) != m_setMachineIDBlockList.end() ); }

protected:
	template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );
	void SendToAccountDB( unsigned short usEventID );

private:	

	std::set< std::string >		m_setMachineIDBlockList;

	boost::timer				m_kTimer[TM_TOTAL_NUM];
	int							m_iReleaseTick[BT_MAX];
};

DefSingletonInline( KMachineBlockManager );

template < class T >
void KMachineBlockManager::SendToAccountDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_CHANNEL_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CHANNEL_ACCOUNT_DB, 0, NULL, usEventID, data );
}

#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}
