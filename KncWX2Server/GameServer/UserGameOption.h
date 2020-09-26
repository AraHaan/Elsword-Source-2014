#pragma once
#include "KncUidType.h"
#include <boost/timer.hpp>
#include <vector>
#include <map>
#include "CommonPacket.h"


//{{ 2007. 8. 9  ������  Ŀ�´�Ƽ�ɼ� Ŭ���� }}
struct KBlackListUnit;
class KUserGameOption
{
public:

	KUserGameOption(void);
	~KUserGameOption(void);
	
	// Deny State
	bool IsDenyFriendShip()							{ return ( m_cDenyFriendShip == KDenyOptions::DOS_ON ); }
	bool IsDenyParty()								{ return ( m_cDenyParty == KDenyOptions::DOS_ON ); }
	bool IsDenyTrade()								{ return ( m_cDenyPersonalTrade == KDenyOptions::DOS_ON ); }
	bool IsDenyInviteGuild()						{ return ( m_cDenyInviteGuild == KDenyOptions::DOS_ON ); }
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	bool IsDenyRequestCouple()						{ return ( m_cDenyRequestCouple == KDenyOptions::DOS_ON ); }
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	const KDenyOptions GetDenyOptions();
	bool SetDenyOptions( const KDenyOptions& kDenyOptions );
		
	// Chat Black List functions
	bool IsAddChatBlackListPossible()				{ return ( m_mapChatBlackList.size() < 30 ); }
	void SetChatBlackList( const std::vector< KChatBlackListUnit >& vecBlackList );
	void AddChatBlackListUnit( UidType iUnitUID, std::wstring& wstrNickName );
	void DelChatBlackListUnit( UidType iUnitUID, std::wstring& wstrNickName );
	bool IsChatBlackListUnit( UidType iUnitUID )	{ return ( m_mapChatBlackList.find( iUnitUID ) != m_mapChatBlackList.end() ); }

private:
	char								m_cDenyFriendShip;
	char								m_cDenyInviteGuild;
	char								m_cDenyParty;
	char								m_cDenyPersonalTrade;
	//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
	char								m_cDenyRequestCouple;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
    char                                m_cDenyInvitePractivePVP;
	//{{ 2008. 1. 30  ������  ä�� �� ����Ʈ
	std::map< UidType, std::wstring >	m_mapChatBlackList;
	//}}
};

