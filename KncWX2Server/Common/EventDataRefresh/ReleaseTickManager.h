#pragma once
#include "ServerDefine.h"
#include "KncUtil.h"

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KNCSingleton.h>
#include <ToString.h>
#include <boost/timer.hpp>
#include <map>

class KReleaseTickManager
{
	DeclareSingleton( KReleaseTickManager );
	DeclDump;
	DeclToStringW;

public:
	KReleaseTickManager();
	~KReleaseTickManager();

	// function
	void	Init( void );
	void	Tick( void );
	void	SendReleaseTickListReq( void );
	
	// DB���� �̺�Ʈ ��Ŷ�� �ð��..
	void	SetReleaseTickData( IN const std::map< int, int >& mapReleaseTick );

private:
	std::map< int, int >	m_mapReleaseTick;
	boost::timer			m_TimerRefresh;		// ���� �ð����� ����
};

DefSingletonInline( KReleaseTickManager );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
