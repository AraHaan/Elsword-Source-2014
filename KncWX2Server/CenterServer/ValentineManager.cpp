#include "ValentineManager.h"

//{{ 2013. 02. 01  �̺�Ʈ �߷�Ÿ�� ���� - ��μ�
#ifdef SERV_EVENT_VALENTINE_DUNGEON


KValentineManager::KValentineManager(void)
{
	for( int index = 0 ; index < TEM_TOTAL_NUM; ++index )
	{
		m_bOnTimer[index] = false;
	}
}

KValentineManager::~KValentineManager(void)
{
}

void KValentineManager::Init()
{
	// Ÿ�̸� �ʱ�ȭ 
	for( int index = 0 ; index < TEM_TOTAL_NUM; ++index )
	{
		SetStartTimer( index, false );
	}
}

bool KValentineManager::SetStartTimer( IN int iEnum, IN bool bStart )
{
	// Ÿ�̸� enum Ȯ��
	if( iEnum < TEM_AFTER_DUNGEON_START || iEnum >= TEM_TOTAL_NUM )
	{
		return false;
	}

	m_kTimer[iEnum].restart();
	m_bOnTimer[iEnum] = bStart;

	if( iEnum == TEM_AFTER_DUNGEON_START )
	{
		m_kTimer[TEM_AFTER_DUNGEON_START_5_SEC].restart();
		m_bOnTimer[TEM_AFTER_DUNGEON_START_5_SEC] = bStart;
	}

	return true;
}

bool KValentineManager::IsOnTimer( IN int iEnum )
{
	if( iEnum < TEM_AFTER_DUNGEON_START || iEnum >= TEM_TOTAL_NUM )
	{
		return false;
	}

	return m_bOnTimer[iEnum];
}

void KValentineManager::CheckTimer( IN int iEnum, OUT int& iType )
{
	// Ÿ�̸� enum Ȯ��
	if( iEnum < TEM_AFTER_DUNGEON_START || iEnum >= TEM_TOTAL_NUM )
	{
		iType = TT_NONE;
		return;
	}

	if( IsOnTimer( iEnum ) == false )
	{
		iType = TT_NONE;
		return;
	}

	switch( iEnum )
	{
	case TEM_AFTER_DUNGEON_START:
		{
			if( m_kTimer[iEnum].elapsed() >= LDT_VALENTINE )	// �̺�Ʈ ���� ����
			{
				iType = TT_END_GAME;
			}
			else if( m_kTimer[TEM_AFTER_DUNGEON_START_5_SEC].elapsed() >= 5.f ) // �̺�Ʈ ���� �ð� ����ȭ
			{
				iType = TT_SYNC_TIME;
				m_kTimer[TEM_AFTER_DUNGEON_START_5_SEC].restart();
			}
		}
		break;
	}
}

int KValentineManager::GetRemainTime( IN int iEnum )
{
	// Ÿ�̸� enum Ȯ��
	if( iEnum < TEM_AFTER_DUNGEON_START || iEnum >= TEM_TOTAL_NUM )
	{
		return 0;
	}

	return static_cast<int>(m_kTimer[iEnum].elapsed());
}
#endif SERV_EVENT_VALENTINE_DUNGEON
//}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
void KValentineManager::CheckTimerForInt( IN int iEnum, OUT int& iType )
{
	// Ÿ�̸� enum Ȯ��
	if( iEnum < TEM_AFTER_DUNGEON_START || iEnum >= TEM_TOTAL_NUM )
	{
		iType = TT_NONE;
		return;
	}

	if( IsOnTimer( iEnum ) == false )
	{
		iType = TT_NONE;
		return;
	}

	switch( iEnum )
	{
	case TEM_AFTER_DUNGEON_START:
		{
			if( m_kTimer[TEM_AFTER_DUNGEON_START_5_SEC].elapsed() >= 5.f ) // �̺�Ʈ ���� �ð� ����ȭ
			{
				iType = TT_SYNC_TIME;
				m_kTimer[TEM_AFTER_DUNGEON_START_5_SEC].restart();
			}
		}
		break;
	}
}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT