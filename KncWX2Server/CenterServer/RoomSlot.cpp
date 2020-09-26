#include "RoomSlot.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "X2Data/XSLRoom.h"
#include "Room/RoomSlotFSM.h"
#include "Room/RoomUserFSM.h"

KRoomSlot::KRoomSlot()
{
    InitSlot( 0 );
}

KRoomSlot::KRoomSlot( int iSlotID )
{
    InitSlot( iSlotID );
}

KRoomSlot::~KRoomSlot()
{
}

void KRoomSlot::AssignTeam( int iGameMode )
{
    switch( iGameMode )
    {
    case CXSLRoom::PGT_TEAM:
	case CXSLRoom::PGT_TEAM_DEATH:
        if( GetSlotID() / 4 == 0 )
        {
            SetTeam( CXSLRoom::TN_RED );
        }
        else
        {
            SetTeam( CXSLRoom::TN_BLUE );
        }
        break;
    case CXSLRoom::PGT_SURVIVAL:
        SetTeam( GetSlotID() );
        break;
    default:
        SetTeam( CXSLRoom::TN_RED );
        break;
    }
}

UidType KRoomSlot::GetCID()
{
    if( !IsOccupied() )
    {
        return 0;
    }

    return m_spRoomUser->GetCID();
}

void KRoomSlot::GetRoomSlotInfo( KRoomSlotInfo& kInfo )
{
    kInfo.m_Index		= GetSlotID();
    kInfo.m_SlotState	= GetStateID();
    kInfo.m_TeamNum		= GetTeam();

    if( IsOccupied() )
    {
        kInfo.m_SlotState += m_spRoomUser->GetStateID() - KRoomUserFSM::S_INIT;
        // RoomUser ���°� S_RESULT �� ��� Ŭ���̾�Ʈ�� ���� ���� ���¸� SS_PLAY�� �����.
        if( ( int )kInfo.m_SlotState > ( int )CXSLRoom::SS_PLAY )
        {
            kInfo.m_SlotState = CXSLRoom::SS_PLAY;
        }
        kInfo.m_bHost	= m_spRoomUser->IsHost();
        kInfo.m_bReady	= m_spRoomUser->IsReady();
		kInfo.m_bPitIn	= m_spRoomUser->IsPitIn();
        kInfo.m_bTrade  = m_spRoomUser->IsInTrade();

//#ifdef PVP_BOSS_COMBAT_TEST
//		kInfo.m_bIsBoss = m_spRoomUser->GetIsBoss();
//#endif PVP_BOSS_COMBAT_TEST

        m_spRoomUser->GetRoomUserInfo( kInfo.m_kRoomUserInfo );
    }
    else
    {
        kInfo.m_bHost	= false;
        kInfo.m_bReady	= false;
		kInfo.m_bPitIn	= false;
        kInfo.m_bTrade  = false;
        kInfo.m_kRoomUserInfo.m_nUnitUID = 0;
    }
}

bool KRoomSlot::IsOpened()
{
    if( GetStateID() == KRoomSlotFSM::S_INIT ||
        GetStateID() == KRoomSlotFSM::S_ASSIGNED )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool KRoomSlot::IsOccupied()
{
    if( GetStateID() == KRoomSlotFSM::S_ASSIGNED )
    {
        if( !m_spRoomUser )
        {
            START_LOG( cerr, L"���Կ� ����ִ� ������ ������ �̻�." )
                << BUILD_LOG( GetSlotID() )
                << BUILD_LOG( GetStateID() )
                << BUILD_LOG( GetStateIDString() )
                << END_LOG;

            ForceStateTransitionTo( KRoomSlotFSM::S_INIT );
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool KRoomSlot::Enter( KRoomUserPtr spRoomUser )
{
    if( !IsOpened() || IsOccupied() )
    {
        START_LOG( cerr, L"���Կ� ���� ����." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << END_LOG;

        return false;
    }

    if( !spRoomUser )
    {
        START_LOG( cerr, L"�� ���� ������ �̻�." )
            << END_LOG;

        return false;
    }

    m_spRoomUser = spRoomUser;
    m_spRoomUser->SetSlotID( GetSlotID() );
    m_spRoomUser->SetTeam( GetTeam() );
    StateTransition( KRoomSlotFSM::I_TO_ASSIGNED );

    return true;
}

bool KRoomSlot::Leave()
{
    if( !IsOccupied() )
    {
        START_LOG( cerr, L"�� ���Կ� ĳ���� ����." )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << END_LOG;

        return true;
    }

    StateTransition( KRoomSlotFSM::I_TO_INIT );
    m_spRoomUser.reset();

    return true;
}

bool KRoomSlot::Open()
{
    if( IsOpened() )
    {
        START_LOG( cerr, L"�̹� ���� �ִ�." )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << END_LOG;

        return true;
    }

    if( IsOccupied() )
    {
        START_LOG( cerr, L"������� ����ִ�." )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << END_LOG;

        return true;
    }

    StateTransition( KRoomSlotFSM::I_TO_INIT );

    return true;
}

bool KRoomSlot::Close()
{
    if( !IsOpened() )
    {
        START_LOG( cerr, L"�̹� �����ִµ�?" )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << END_LOG;

        return true;
    }

    if( IsOccupied() )
    {
        START_LOG( cerr, L"����� �ִٱ�." )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << END_LOG;

        return false;
    }

    StateTransition( KRoomSlotFSM::I_TO_CLOSED );

    return true;
}

bool KRoomSlot::ToggleOpenClose()
{
    switch( GetStateID() )
    {
    case KRoomSlotFSM::S_INIT:
        START_LOG( clog, L"���� ����." );
        StateTransition( KRoomSlotFSM::I_TO_CLOSED );
        break;
    case KRoomSlotFSM::S_CLOSED:
        START_LOG( clog, L"���� ����." );
        StateTransition( KRoomSlotFSM::I_TO_INIT );
        break;
    case KRoomSlotFSM::S_ASSIGNED:
        START_LOG( cerr, L"���Կ� ĳ���Ͱ� �ִµ� ���� ������ ��." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << BUILD_LOG( GetCID() )
            << END_LOG;
        return false;
    default:
        START_LOG( cerr, L"���°� �̻���." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << END_LOG;
        return false;
    }

    return true;
}

void KRoomSlot::ResetSlot()
{
    switch( GetStateID() )
    {
    case KRoomSlotFSM::S_INIT:
        break;
    case KRoomSlotFSM::S_CLOSED:
        StateTransition( KRoomSlotFSM::I_TO_INIT );
        break;
    case KRoomSlotFSM::S_ASSIGNED:
        START_LOG( cerr, L"���Կ� ĳ���Ͱ� �Ҵ�Ǿ� ����." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetSlotID() )
            << BUILD_LOG( GetCID() )
            << BUILD_LOG( GetTeam() )
            << END_LOG;
        StateTransition( KRoomSlotFSM::I_TO_INIT );
        break;
    default:
        START_LOG( cerr, L"���°� �̻���." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() )
            << BUILD_LOG( GetSlotID() )
            << END_LOG;
        ForceStateTransitionTo( KRoomSlotFSM::S_INIT );
        break;
    }

    m_spRoomUser.reset();
}

void KRoomSlot::InitSlot( int iSlotID )
{
    SetSlotID( iSlotID );
    AssignTeam( CXSLRoom::PGT_TEAM );
    m_spRoomUser.reset();
}