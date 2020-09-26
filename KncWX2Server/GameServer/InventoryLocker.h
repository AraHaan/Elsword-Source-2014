#pragma once

#include <vector>
#include <boost/timer.hpp>


//{{ 2012. 03. 29	������	Inventory Lock ���
#ifdef SERV_INVENTORY_LOCK


class KInventoryLocker
{
public:
	enum LOCK_TYPE
	{
		LT_INFINITE = 0,
		LT_TIME_OUT,

		LT_END,
	};

private:
	bool			m_bLocked;
	boost::timer	m_tTimer;
	double			m_fLockTime;
	LOCK_TYPE		m_eLockType;

public:
	KInventoryLocker()
	{
		Clear();
	}

	void Clear()
	{
		m_bLocked = false;
		m_fLockTime = 0.0;
		m_eLockType = LT_INFINITE;
	}

	bool IsLocked()
	{
		// ��� ���� �ʴٸ�!
		if( !m_bLocked )
		{
			return false;
		}

		// Ÿ�Ӿƿ� Ÿ���̸� Ÿ�̸� üũ!
		if( m_eLockType == LT_TIME_OUT )
		{
			// �ð��� �����ߴٸ� lock�� Ǯ��!
			if( m_tTimer.elapsed() > m_fLockTime )
			{
				m_bLocked = false;
				return false;
			}
		}

		// �װ͵� �ƴϸ� Lock!
		return true;
	}

	bool Lock( IN const LOCK_TYPE eLockType, IN const double fLockTime = 99999.0 )
	{
		if( IsLocked() )
		{
			return false;
		}

		// �ʱ�ȭ
		Clear();
		
		// ��� ó��!
		m_bLocked = true;
		m_eLockType = eLockType;
		if( eLockType == LT_TIME_OUT )
		{
			m_fLockTime = fLockTime;
			m_tTimer.restart();
		}
		return true;
	}

	void Unlock()
	{
		Clear();
	}
};


#endif SERV_INVENTORY_LOCK
//}}


