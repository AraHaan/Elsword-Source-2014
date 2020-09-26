#pragma once

#include "FSM/support_FSM.h"
#include "AutoPartyUserFSM.h"
#include "Event.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "X2Data/XSLUnit.h"


//{{ 2012. 02. 07	������	��Ʋ�ʵ� �ý���
#ifdef SERV_BATTLE_FIELD_SYSTEM


#define KAutoPartyUserPtr KAutoPartyUser*
#undef KAutoPartyUserPtr
SmartPointer( KAutoPartyUser );


class KAutoPartyUser
{
	DeclareFSM_MT;
public:
	enum TIMER_ENUM
	{
		TE_AUTO_PARTY_COMPLETE = 0,

		TE_MAX,
	};

public:
	KAutoPartyUser( IN const KAutoPartyUserInfo& kInfo );
    virtual ~KAutoPartyUser();
	
	UidType		GetCharUID() const							{ return m_kAutoPartyUserInfo.m_iUnitUID; }
	const std::wstring& GetNickName() const					{ return m_kAutoPartyUserInfo.m_wstrNickName; }
	UidType		GetGSUID() const							{ return m_kAutoPartyUserInfo.m_iGSUID; }
	const KAutoPartyUserInfo& GetAutoPartyUserInfo() const	{ return m_kAutoPartyUserInfo; }
	UidType		GetOldPartyUID() const						{ return m_kAutoPartyUserInfo.m_kOldPartyInfo.m_iOldPartyUID; }
	u_char		GetLevel() const							{ return m_kAutoPartyUserInfo.m_ucLevel; }
	char		GetUnitClass() const						{ return m_kAutoPartyUserInfo.m_cUnitClass; }
	char		GetUnitType() const;
	const KOldPartyInfo& GetOldPartyInfo() const			{ return m_kAutoPartyUserInfo.m_kOldPartyInfo; }
	double		GetAutoPartyCompleteSeconds() const			{ return m_tTimer[TE_AUTO_PARTY_COMPLETE].elapsed(); }

	//{{ �ڵ� ��Ƽ�� ���� ���� �� ��Ī ��� �ð� �α� �߰� - ��μ�
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	int			GetAutoPartyWaitTime() const				{ return m_kAutoPartyUserInfo.m_iAutoPartyWaitTime; }
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	//}
	void		SetAutoPartyBonus( IN const bool bAutoPartyBonus )	{ m_kAutoPartyUserInfo.m_bAutoPartyBonus = bAutoPartyBonus; }
	
	bool		Accept();
	bool		IsAccepted();

	// PartyUserInfo
	void		ClearPartyUserInfo()				{ m_vecPartyUserInfo.clear(); }	
	void		UpdatePartyUserInfo( IN const KPartyUserInfo& kPartyUserInfo );
	bool		GetPartyUserInfo( OUT KPartyUserInfo& kPartyUserInfo );
	bool		IsEmptyPartyUserInfo() const		{ return m_vecPartyUserInfo.empty(); }

	// PartyRoomUserInfo
	void		ClearPartyRoomUserInfo()			{ m_vecPartyRoomUserInfo.clear(); }	
	void		UpdatePartyRoomUserInfo( IN const KPartyRoomUserInfo& kPartyRoomUserInfo );
	bool		GetPartyRoomUserInfo( OUT KPartyRoomUserInfo& kPartyRoomUserInfo );
	bool		IsEmptyPartyRoomUserInfo() const	{ return m_vecPartyRoomUserInfo.empty(); }

	bool		IsHost( const UidType iUnitUID );

public:
	template < class T > void SendToGSCharacter( unsigned short usEventID, const T& data );
	void SendToGSCharacter( unsigned short usEventID );

private:
    KAutoPartyUserInfo					m_kAutoPartyUserInfo;
	std::vector< KPartyUserInfo >		m_vecPartyUserInfo;
	std::vector< KPartyRoomUserInfo >	m_vecPartyRoomUserInfo;
	// ���� ������ �־�� ���� ������?

	boost::timer					m_tTimer[TE_MAX];
};

template < class T >
void KAutoPartyUser::SendToGSCharacter( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetGSUID(), -1 };
	KncSend( PI_GLOBAL_AUTO_PARTY, KBaseServer::GetKObj()->GetUID(), PI_GS_CHARACTER, GetCharUID(), anTrace, usEventID, data );
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}


