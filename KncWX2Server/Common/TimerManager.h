#pragma once
#include <boost/timer.hpp>

#pragma pack( push, 1 )

//{{ 2009. 3. 6  ������		Ÿ�̸ӸŴ���
class KTimerManager
{
public:
	KTimerManager()	: m_fAddTime( 0.0f ) {} 
	void restart()						{ m_kTimer.restart(); m_fAddTime = 0.0f; }
	double elapsed() const				{ return ( m_kTimer.elapsed() + m_fAddTime ); }

	void SetAddTime( double fTime )		{ m_fAddTime = fTime; }
	double GetAddTime()					{ return m_fAddTime; }

private:
	double		 m_fAddTime;
	boost::timer m_kTimer;
};
//}}

//{{ 2009. 7. 21  ������	���� Ÿ�̸�
class KDynamicTimer
{
public:
	KDynamicTimer() { m_vecTimer.clear(); }
	~KDynamicTimer() {}

	bool CreateTimer()
	{
		// �̹� Ÿ�̸Ӱ� �ִٸ� Ÿ�̸� ���� ����!
		if( !m_vecTimer.empty() ) return false;

		m_vecTimer.push_back( boost::timer() );
		return true;
	}

	bool KillTimer()
	{
		// ������ Ÿ�̸Ӱ� ���ٸ� ���� ����
		if( m_vecTimer.empty() ) return false;

		m_vecTimer.clear();
		return true;
	}

	bool IsExist() { return !m_vecTimer.empty(); }

	double elapsed() const
	{
		// Ÿ�̸Ӱ� ���ٸ� 0.0f�� ��ȯ
		std::vector< boost::timer >::const_iterator vit = m_vecTimer.begin();
		if( vit == m_vecTimer.end() )
			return 0.0f;
		
		return vit->elapsed();
	}

private:
	std::vector< boost::timer > m_vecTimer;
};
//}}

#pragma pack( pop )

