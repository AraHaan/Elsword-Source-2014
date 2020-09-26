#include "StdAfx.h"
#include ".\x2pvpgameteam.h"

CX2PVPGameTeam::CX2PVPGameTeam(void)
{
}

CX2PVPGameTeam::~CX2PVPGameTeam(void)
{
	/*
	Release() ���� ����
	*/
}

/** @function : Release
	@brief : X2Game�� �Ҹ��ڿ� ������ �͵��� �̰����� ������
	(�̷��� ���� ����� �� ������ X2Game �Ҹ��ڰ� X2GameUnit ���� �Ҹ��� ��
	X2GameUnit ���� �Ҹ��� ���� �Ǵ� ȣ��Ǵ� �Լ� �鿡 ���ο��� �ٽ� X2Game�� �����ϴ� ���
	�� ���Ƽ� �� ã�Ƴ��� ����⿡ �̷��� Release�� ��������ϴ�.
	X2Game�� �Ҹ�Ǵ� ���� ���� Pvp, Dungeon, BattleField�� ������Ʈ�� �Ҹ�� �� �̹Ƿ�
	�ش� ������Ʈ�� �Ҹ��ڿ��� Release�� ȣ�� �ϵ��� �Ͽ����ϴ�.
	�� ���� ����� ������ ���� �˷��ּ��� - 2012-11-11 (����ö)
*/
/*virtual*/ void CX2PVPGameTeam::Release()
{
	CX2PVPGame::Release();
}

HRESULT CX2PVPGameTeam::OnFrameMove( double fTime, float fElapsedTime )
{
	CX2PVPGame::OnFrameMove( fTime, fElapsedTime );

#ifndef SERV_PVP_NEW_SYSTEM
	int redTeamVPMax	= 0;
	int blueTeamVPMax	= 0;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GUUser* pCX2GUUser = m_UserUnitList[i];
		if( pCX2GUUser != NULL )
		{
			if( NULL != pCX2GUUser->GetUnit() )
			{
				if( pCX2GUUser->GetTeam() == CX2Room::TN_RED )
				{
					redTeamVPMax += pCX2GUUser->GetUnit()->GetUnitData().m_VSPointMax;
				}
				else if( pCX2GUUser->GetTeam() == CX2Room::TN_BLUE )
				{
					blueTeamVPMax += pCX2GUUser->GetUnit()->GetUnitData().m_VSPointMax;
				}
			}
			else
			{
				DeleteUserUnitByUID( pCX2GUUser->GetUnitUID() );
				ErrorLog( XEM_ERROR113 );
			}
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	int VPMaxGap = redTeamVPMax - blueTeamVPMax;
	if( VPMaxGap > 0 )
	{
		m_TeamVPMaxAdvRed	= 0;
		m_TeamVPMaxAdvBlue	= VPMaxGap;		
	}
	else
	{
		m_TeamVPMaxAdvRed	= -VPMaxGap;
		m_TeamVPMaxAdvBlue	= 0;		
	}
#endif

	return S_OK;
}

bool CX2PVPGameTeam::DetermineLastKill()
{
	if( m_bLastKillCheck == true )
		return false;

	int remainRedTeam = 0;
	int remainBlueTeam = 0;

	int dyingRedTeam = 0;
	int dyingBlueTeam = 0;

	CX2GameUnit* pLastUnit = NULL;
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::EnterCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
	{
		CX2GameUnit* pCX2GameUnit = m_UserUnitList[i];

		if ( pCX2GameUnit == NULL )
		{
			continue;
		}

		if( pCX2GameUnit->GetTeam() == CX2Room::TN_RED
			&& pCX2GameUnit->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
		{
			remainRedTeam++;
			if( pCX2GameUnit->GetNowHp() <= 0.0f )
			{
				pLastUnit = pCX2GameUnit;
				dyingRedTeam++;
			}
		}
		else if( pCX2GameUnit->GetTeam() == CX2Room::TN_BLUE
			&& pCX2GameUnit->GetGameUnitState() != CX2GameUnit::GUSI_DIE )
		{
			remainBlueTeam++;
			if( pCX2GameUnit->GetNowHp() <= 0.0f )
			{
				pLastUnit = pCX2GameUnit;
				dyingBlueTeam++;
			}
		}
	}
#ifndef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
	::LeaveCriticalSection( &m_csGameIntruder );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

	if( (remainRedTeam == 1 && dyingRedTeam == 1)
		|| (remainBlueTeam == 1 && dyingBlueTeam == 1) )
	{
		m_fLastKillWaitTime -= m_fElapsedTime;		
	}

	if( m_fLastKillWaitTime <= 0.0f )
	{
		if( pLastUnit != NULL && GetMyUnit() != NULL && pLastUnit == GetMyUnit() )
		{
			((CX2GUUser*)pLastUnit)->SendFrameData();
		}

		m_bLastKillCheck = true;
		g_pKTDXApp->StopFrame( 80 );
		g_pKTDXApp->GetDGManager()->ClearScreen( 5 );
		g_pKTDXApp->SlowFrame( 0.2f, 1.5f );

		SetTimerFocusUnit( pLastUnit, 1.0f );

		return true;
	}
	return false;
}



