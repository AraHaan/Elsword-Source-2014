#pragma once 

#include <string>
#include <vector>
#include <map>
#include "Event.h"

#define VARIFY_ENABLE( return_code ) \
	if( IsEnable() == false ) \
{ \
	return_code; \
} \

class KGameEvent
{
public:
	enum GAME_EVENT_TYPE
	{
		GET_NONE				= 0,

		GET_QUEST				= 1,	// [1]  ����Ʈ �̺�Ʈ
		GET_ITEM_DROP			= 2,	// [2]  ���� ������ ��ӷ� �̺�Ʈ
		GET_REWARD				= 3,	// [3]  ����ġ&ED&AP&RP �̺�Ʈ
		GET_MONSTER				= 4,	// [4]  ���� ���� �̺�Ʈ
		GET_TOURNAMENT			= 5,	// [5]  ������ȸ�� ����üũ �̺�Ʈ
		GET_SPIRIT				= 6,	// [6]  �ټ��� �̺�Ʈ
        GET_CONNECT_TIME		= 7,	// [7]  ���� �ð� �̺�Ʈ
		GET_SECRET_DUNGEON		= 8,	// [8]  ��� ���� �̺�Ʈ
		GET_NOTIFY_MSG			= 9,	// [9]  �������� �̺�Ʈ
		GET_CUMULATIVE_TIME		= 10,	// [10] ���� �ð� �̺�Ʈ
		GET_PURCHASE_LIMITED	= 11,	// [11] �ؿ� ���� ���� ����
		//{{ 2011. 08. 12   ��μ�      ��ϸ� ���� 
//#ifdef SERV_NEW_HENIR_TEST
		GET_HENIR_REWARD		= 12,	// [12] ��ϸ� ���� ���� ȸ��
//#endif SERV_NEW_HENIR_TEST
		//}}
//#ifdef SERV_OPTION_EVENT_SYSTEM
		GET_OPTION				= 13,	// [13] ���� ���� �ɼ� ����
//#endif SERV_OPTION_EVENT_SYSTEM

//#ifdef SERV_ADVERTISEMENT_EVENT
		GET_ADVERTISEMENT_EVENT	= 14,	// [14] URL �ΰ��� ����
//#endif SERV_ADVERTISEMENT_EVENT

//#ifdef SERV_NEW_EVENT_TYPES
		GET_DUNGEON				= 15,	// [15] ����
		GET_MAX_LEVEL			= 16,	// [16] ����
		GET_CONTENTS			= 17,	// [17] ������ ����
//#endif SERV_NEW_EVENT_TYPES

//#ifdef SERV_CODE_EVENT
		GET_CODE				= 18,	// [18] �ڵ�on/off
//#endif SERV_CODE_EVENT
//{{ 2013. 03. 18	 ����Ʈ ��� Ȯ�� ���� �̺�Ʈ - ��μ�
//#ifdef SERV_QUEST_ITEM_DROP_EVENT
		GET_QUEST_ITEM_DROP		= 19,	// [19] ����Ʈ ������ ��ӷ� �̺�Ʈ
//#endif SERV_QUEST_ITEM_DROP_EVENT
//}
//#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// ���볯¥: 2013-04-11
		GET_DEFENSE_DUNGEON		= 20,	// [20] ����� �� ���� Ȯ�� �̺�Ʈ
//#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

//#ifdef SERV_LEVEL_UP_EVENT// �۾���¥: 2013-06-24	// �ڼ���
		GET_LEVEL_UP			= 21,	// [21] ���� �� �̺�Ʈ
//#endif // SERV_LEVEL_UP_EVENT

//#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-09	// �ڼ���
		GET_JUMPING_CHARACTER	= 22,	// [22] ���� ĳ���� �̺�Ʈ
//#endif // SERV_JUMPING_CHARACTER

		GET_MAX,
	};

public:
	KGameEvent();
	virtual ~KGameEvent();

	void			SetUID( int iUID )				{ m_iUID = iUID; }
	int				GetUID() const					{ return m_iUID; }

	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
	void			SetScriptID( int iScriptID )	{ m_iScriptID = iScriptID; }
	int				GetScriptID() const				{ return m_iScriptID; }
	//}}

	void			SetType( GAME_EVENT_TYPE eType ){ m_eType = eType; }
	GAME_EVENT_TYPE	GetType() const					{ return m_eType; }

	void			SetEnable( bool bEnable )		{ m_bEnable = bEnable; }
	bool			IsEnable() const				{ return m_bEnable; }

#ifdef SERV_ALLOW_EVENT_ERASE
	void			SetOn( bool bOn )				{ m_bOn = bOn; }
	bool			IsOn() const					{ return m_bOn; }
#endif SERV_ALLOW_EVENT_ERASE

	void			SetEventName( std::wstring wstrEventName ){ m_wstrEventName = wstrEventName; }
	const std::wstring&	GetEventName() const		{ return m_wstrEventName; }

	void			SetBeginDate( CTime& ctBeginDate ){ m_BeginDate = ctBeginDate; }
	void			SetEndDate( CTime& ctEndDate ){ m_EndDate = ctEndDate; }
	const CTime&	GetBeginDate() const			{ return m_BeginDate; }
	const CTime&	GetEndDate() const				{ return m_EndDate; }

protected:
	int					m_iUID;
	//{{ 2010. 11. 15	������	�̺�Ʈ ��ũ��Ʈ �ǽð� ��ġ
	int					m_iScriptID;
	//}}
	GAME_EVENT_TYPE		m_eType;
	bool				m_bEnable;

	std::wstring		m_wstrEventName;

	CTime				m_BeginDate;
	CTime				m_EndDate;	

#ifdef SERV_ADVERTISEMENT_EVENT
	std::wstring		m_wstrAdvertisementURL;
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_ALLOW_EVENT_ERASE
	bool				m_bOn;
#endif SERV_ALLOW_EVENT_ERASE
};



