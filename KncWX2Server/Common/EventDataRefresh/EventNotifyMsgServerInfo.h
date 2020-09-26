#pragma once

#include "ServerDefine.h"
#include "KncUtil.h"
#include "KncUidType.h"

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KNCSingleton.h>
#include <ToString.h>

class KEventNotifyMsgServerInfo
{
	DeclareSingleton( KEventNotifyMsgServerInfo );
	DeclDump;
	DeclToStringW;

public:
	KEventNotifyMsgServerInfo();
	~KEventNotifyMsgServerInfo();

	// function
	void Init( void );
	bool IsNotifyMSGEvent( void ) const { return m_bNotifyMSGEvent; }
	void UpdateEventNotifyMsgServerInfo( void );
	// DB���� �̺�Ʈ ��Ŷ�� �ð��..
	void SetEventNotifyMsgServerInfoData( IN const std::vector< std::pair< int, int > >& vecEventNotifyMsgServerInfo );

private:
	//{{ 2008. 7. 11  ������	���� �̺�Ʈ
	UidType									m_iNotifyCnServerUID;
	bool									m_bNotifyMSGEvent;
	//}}
};

DefSingletonInline( KEventNotifyMsgServerInfo );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}