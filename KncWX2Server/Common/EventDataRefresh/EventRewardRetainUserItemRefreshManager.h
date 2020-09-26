#pragma once
#include "ServerDefine.h"
#include "KncUtil.h"

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB

//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>
#include <vector>
#include "CommonPacket.h"

class KEventRewardRetainUserItemRefreshManager
{
	DeclareSingleton( KEventRewardRetainUserItemRefreshManager );
	DeclToStringW;
	DeclDump;

public:
	KEventRewardRetainUserItemRefreshManager(void);
	~KEventRewardRetainUserItemRefreshManager(void);

	void GetOutUserRetainingRewardItemInfo( std::map< int, KRewardItem >& mapRewardItem );
	bool GetOutUserRetainingSelectItemInfo( IN int& iSlot, OUT KRewardItem& kInfo );
	bool GetOutUserRetainingSelectItemInfo( IN int& iItemID, OUT int& iQuantity );

	void UpdateRewardRetainUserItem( void );
	void SetRewardRetainUserItem( IN const std::map< int, KRewardItem >& mapRewardItem );

private:
	std::map< int, KRewardItem > m_mapRewardItem;
};

DefSingletonInline( KEventRewardRetainUserItemRefreshManager );

#endif SERV_NEXON_SESSION_PROTOCOL
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}