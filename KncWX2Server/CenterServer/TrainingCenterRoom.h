#pragma once

#include "DungeonRoom.h"

SmartPointer( KTrainingCenterRoom );

//�Ʒü� ���� ������ ����� �״�� ������
// ���ӽ��� / ���ó�� �κи��� �����Ͽ�
// �����÷��̾� ����ó���� ���� �����Ѵ�.

class KTrainingCenterRoom : public KDungeonRoom
{
	NiDeclareRTTI;
	DeclToStringW;

public:
	KTrainingCenterRoom();
	virtual ~KTrainingCenterRoom();

	// override.
	//virtual void Tick();

protected:
	// Get Set
	virtual void SetRoomInfo( const KTCInfo& kInfo );

	// override.
	virtual void ProcessNativeEvent( const KEventPtr& spEvent );
	virtual void StartGame();
	//virtual void StartPlay();
	//virtual void EndPlay();
	//virtual void EndGame();
	//virtual bool CheckIfPlayEnd();
	//virtual int  DecideWinTeam();
	//virtual int  GetPlayMode();
	//virtual void HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS );
	virtual	void SendRoomListInfo( IN const int iCode ){}	//�ƷüҴ� �渮��Ʈ�� ���⶧���� ������ �ȵǰ� �Ѵ�.
	virtual float GetRemainingPlayTime(){ return KRoom::GetRemainingPlayTime(); }

	DECL_ON_FUNC( ERM_OPEN_TC_ROOM_REQ );
   _DECL_ON_FUNC( ERM_END_TC_GAME_REQ, KEGS_END_TC_GAME_REQ );
   _DECL_ON_FUNC( ERM_SET_TC_REMAINING_TIME_REQ, KEGS_SET_TC_REMAINING_TIME_REQ );
   //{{ 2007. 10. 23  ������  
   _DECL_ON_FUNC( ERM_LEAVE_TC_ROOM_REQ, KERM_LEAVE_ROOM_REQ );
   //}}

protected:
	int			m_iTCID;
};




