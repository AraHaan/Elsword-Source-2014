#pragma once
#include "ServerDefine.h"
#include "KncUtil.h"
#include "ServerPacket.h"

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KNCSingleton.h>
#include <ToString.h>
#include <map>

class KEventDataRefreshManager
{
	DeclareSingleton( KEventDataRefreshManager );
	DeclToStringW;
	DeclDump;

public:
	KEventDataRefreshManager();
	~KEventDataRefreshManager();

	// function
	const EVENT_DATA*	GetEventData( IN int iScriptID ) const;
	void				UpdateEventData( void );
	// DB���� �̺�Ʈ ��Ŷ�� �ð��..
	void				SetEventData( IN const std::map< int, EVENT_DATA >& mapEventData );

private:
	std::map< int, EVENT_DATA >				m_mapEventData;
};

DefSingletonInline( KEventDataRefreshManager );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}