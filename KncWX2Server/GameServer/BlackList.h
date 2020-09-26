#pragma once
#include "KncUidType.h"
#include <string>
#include <map>
#include <boost/timer.hpp>

#define IN
#define OUT
#define MAX_BLACKLIST_USER		20
#define ADD_LIMIT_TIME			2.0
#define DEL_LIMIT_TIME			2.0
#define OPT_UPDATE_LIMIT_TIME	2.0

//{{ 2007. 8. 9  ������  ������Ʈ Ŭ���� }}

class KBlackList
{
public:

	enum BlackListDenyState
	{
		BL_ON,
		BL_OFF,
		BL_ONLY_FRIEND
	};

	KBlackList(void);
	~KBlackList(void);
	
	// deny state
	char GetInsertFriendDeny()						{ return m_cInsertFriendDeny; }
	bool SetInsertFriendDeny( char stateVal );

	char GetManToManChatDeny()						{ return m_cManToManChatDeny; }
	void SetManToManChatDeny( char stateVal )		{ m_cManToManChatDeny = stateVal; }

	char GetPartyDeny()								{ return m_cPartyDeny; }
	void SetPartyDeny( char stateVal )				{ m_cPartyDeny = stateVal; }

	char GetPrivateDealDeny()						{ return m_cPrivateDealDeny; }
	void SetPrivateDealDeny( char stateVal )		{ m_cPrivateDealDeny = stateVal; }
	
	// check possible
	bool IsAddPossible();
	bool IsDelPossible();
	bool IsUpdateOptPossible();

	// black list
	void AddNewBlackListUser( IN UidType nUnitUID, IN std::wstring &wstrNickName );	 // �߰�
	bool DeleteBlackListUser( IN std::wstring &wstrNickName, OUT UidType &unitUid ); // ����
	
	void SetBlackList( IN std::map< UidType, std::wstring > &mapBlackList ); 		 // ������Ʈ ��� - KEGS_SELECT_UNIT_ACK �ڵ鷯���� ȣ��

	bool VerifyBlackListUser( IN UidType nUnitUID );								 // �� ����Ʈ �˻�
	bool VerifyBlackListUser( IN std::wstring wstrNickName );						 // �� ����Ʈ �˻�
	
private:	
	char								m_cInsertFriendDeny;
	char								m_cManToManChatDeny;
	char								m_cPartyDeny;
	char								m_cPrivateDealDeny;
	boost::timer						m_kDenyOptionTimer;

	std::map< UidType, std::wstring >	m_mapBlackListUid;
	std::map< std::wstring, UidType >	m_mapBlackListNick;
	boost::timer						m_kAddBlackListTimer;
	boost::timer						m_kDelBlackListTimer;
};

