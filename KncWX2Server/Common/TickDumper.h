#pragma once
#include <atltime.h>
#include <Thread/Locker.h>


#define DeclareTickDumper \
public: \
	void TickDump( const char* pClassName, std::wostream& stm ) { m_kTickDumper.Dump( pClassName, stm ); } \
protected: \
	KTickDumper			m_kTickDumper; \
public: \


#define BEGIN_CHECK_TICK_LATENCY			m_kTickDumper.Begin_CheckTickLatency()
#define END_CHECK_TICK_LATENCY				m_kTickDumper.End_CheckTickLatency()
#define TICK_DUMP( classname, stream )		m_kTickDumper.Dump( #classname, stream )


#pragma pack( push, 1 )


struct KTickDumpInfo
{
	DWORD			m_dwMinTick;
	std::wstring	m_wstrMinTickRegDate;
	DWORD			m_dwMaxTick;
	std::wstring	m_wstrMaxTickRegDate;
	__int64			m_iAverageTickCount;
	__int64			m_iTotalTickCount;
	__int64			m_iPassedTickCount;

	KTickDumpInfo()
	{
		m_dwMinTick			= 0;
		m_dwMaxTick			= 0;		
		m_iAverageTickCount = 0;
		m_iTotalTickCount	= 0;
		m_iPassedTickCount	= 0;
	}
};


class KTickDumper
{
public:
	enum ERR_CODE
	{
		EC_NONE = 0,
		EC_FIRST_CHECK,
		EC_OVER_MIN_TICK,
		EC_OVER_MAX_TICK,
		EC_WARNING_TICK,
	};

public:
	KTickDumper() : m_bFirstCheck( false )
	{
		Reset();
	}
	~KTickDumper()
	{
	}

	void GetDump( KTickDumpInfo& kInfo ) const
	{
		//KLocker lock( m_csDumper );

		kInfo.m_dwMinTick			= m_dwMinTick;
		kInfo.m_wstrMinTickRegDate	= ( std::wstring )( m_tMinRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );		
		kInfo.m_dwMaxTick			= m_dwMaxTick;
		kInfo.m_wstrMaxTickRegDate	= ( std::wstring )( m_tMaxRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );		
		kInfo.m_iTotalTickCount		= m_iTotalLatency;
		kInfo.m_iPassedTickCount	= m_iPassedTickCount;

		if( m_iPassedTickCount == 0 )
		{
			kInfo.m_iAverageTickCount = 0;
		}
		else
		{
			kInfo.m_iAverageTickCount = m_iTotalLatency / m_iPassedTickCount;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ���۰� ���� �����ؼ� Tick Time�� ���ϴ� �Լ�
	void Dump( const char* pClassName, std::wostream& stm ) const
	{
		//KLocker lock( m_csDumper );

		KTickDumpInfo kInfo;
		GetDump( kInfo );

		stm << L"-------------- " << pClassName << " Tick Dump --------------------" << std::endl;
		stm << L"MinTick : "		<< kInfo.m_dwMinTick << std::endl;
		stm << L"MinTickRegDate : " << kInfo.m_wstrMinTickRegDate << std::endl;
		stm << L"MaxTick : "		<< kInfo.m_dwMaxTick << std::endl;
		stm << L"MaxTickRegDate : " << kInfo.m_wstrMaxTickRegDate << std::endl;
		stm << L"AverageTick : "	<< kInfo.m_iAverageTickCount << std::endl;
		stm << L"TotalTickCount : " << kInfo.m_iTotalTickCount << std::endl;
		stm << L"PassedTickCount : "<< kInfo.m_iPassedTickCount << std::endl;
	}

	void SetWarningTickCount( DWORD dwTickCount )
	{
		//KLocker lock( m_csDumper );

		m_dwWarningTickCount = dwTickCount;
	}

	DWORD GetWarningTickCount() const
	{
		//KLocker lock( m_csDumper );

		return m_dwWarningTickCount;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// One Loop Tick Time�� ��� �Լ�
	ERR_CODE CheckOneTickLatency()
	{
		// ����ȭ
		//KLocker lock( m_csDumper );

		ERR_CODE eRet = EC_NONE;

		if( !m_bFirstCheck )
		{
			Reset();
			m_bFirstCheck = true;
			eRet = EC_FIRST_CHECK;
			return eRet;
		}

		const DWORD dwCurTickCount = ::GetTickCount();
		const DWORD dwDelayTick = dwCurTickCount - m_dwLastTick;

		// ��հ��� ���ϱ� ���� ������ ����
		++m_iPassedTickCount;
		m_iTotalLatency += static_cast<__int64>(dwDelayTick);

		// �ּҰ� ���ϱ�
		if( dwDelayTick < m_dwMinTick )
		{
			eRet = EC_OVER_MIN_TICK;

			m_dwMinTick = dwDelayTick;
			m_tMinRegTime = CTime::GetCurrentTime();
		}

		// �ִ밪 ���ϱ�
		if( dwDelayTick > m_dwMaxTick )
		{
			eRet = EC_OVER_MAX_TICK;

			m_dwMaxTick = dwDelayTick;
			m_tMaxRegTime = CTime::GetCurrentTime();

			if( m_dwWarningTickCount != 0  &&  dwDelayTick > m_dwWarningTickCount )
			{
				eRet = EC_WARNING_TICK;
			}
		}

		// ���� Tick�� ���� ( ���� Tick������ ������ ���ϱ� ����.. )
		m_dwLastTick = dwCurTickCount;

		return eRet;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���۰� ���� �����ؼ� Tick Time�� ���ϴ� �Լ�
	ERR_CODE Begin_CheckTickLatency()
	{
		// ����ȭ
		//KLocker lock( m_csDumper );

		if( !m_bFirstCheck )
		{
			Reset();
			m_bFirstCheck = true;
		}

		// ���� ƽ ī��Ʈ�� ��������!
		m_dwLastTick = ::GetTickCount();
		return EC_NONE;
	}

	ERR_CODE End_CheckTickLatency()
	{
		// ����ȭ
		//KLocker lock( m_csDumper );

		ERR_CODE eRet = EC_NONE;

		const DWORD dwCurTickCount = ::GetTickCount();
		const DWORD dwDelayTick = dwCurTickCount - m_dwLastTick;

		// ��հ��� ���ϱ� ���� ������ ����
		++m_iPassedTickCount;
		m_iTotalLatency += static_cast<__int64>(dwDelayTick);

		// �ּҰ� ���ϱ�
		if( dwDelayTick < m_dwMinTick )
		{
			eRet = EC_OVER_MIN_TICK;

			m_dwMinTick = dwDelayTick;
			m_tMinRegTime = CTime::GetCurrentTime();
		}

		// �ִ밪 ���ϱ�
		if( dwDelayTick > m_dwMaxTick )
		{
			eRet = EC_OVER_MAX_TICK;

			m_dwMaxTick = dwDelayTick;
			m_tMaxRegTime = CTime::GetCurrentTime();

			if( m_dwWarningTickCount != 0  &&  dwDelayTick > m_dwWarningTickCount )
			{
				eRet = EC_WARNING_TICK;
			}
		}

		return eRet;
	}

private:
	void Reset()
	{
		m_dwLastTick		 = ::GetTickCount();
		m_dwMinTick			 = UINT_MAX;
		m_dwMaxTick			 = 0;
		m_iTotalLatency		 = 0;
		m_iPassedTickCount	 = 0;
		m_dwWarningTickCount = 0;
		m_tMinRegTime		 = CTime();
		m_tMaxRegTime		 = CTime();
	}

private:
	DWORD						m_dwLastTick;
	DWORD						m_dwMinTick;	// Min	
	DWORD						m_dwMaxTick;	// Max
	CTime						m_tMinRegTime;	// Min
	CTime						m_tMaxRegTime;	// Max
	__int64						m_iTotalLatency;
	__int64						m_iPassedTickCount;
	DWORD						m_dwWarningTickCount;
	bool						m_bFirstCheck;
	//mutable KncCriticalSection	m_csDumper;
};



//////////////////////////////////////////////////////////////////////////
// ���� Ÿ�Ե鿡 ���� Tickī��Ʈ�� �����Ҽ� �ִ� Ŭ����
class KMultiTickDumper
{
public:
	enum ERR_CODE
	{
		EC_NONE = 0,
		EC_FIRST_CHECK,
		EC_OVER_MAX_TICK,
		EC_WARNING_TICK,
	};

	struct KTickInfo
	{
		DWORD						m_dwMaxTick;	// Max
		CTime						m_tMaxRegTime;	// Max
		__int64						m_iTotalLatency;
		__int64						m_iPassedTickCount;

		KTickInfo()
		{
			m_dwMaxTick = 0;			
			m_iTotalLatency = 0;
			m_iPassedTickCount = 0;
		}
	};

public:
	KMultiTickDumper()
	{
		Reset();
	}
	~KMultiTickDumper()
	{
	}

	void GetDump( const int iType, KTickDumpInfo& kInfo ) const
	{
		std::map< int, KTickInfo >::const_iterator mit;
		mit = m_mapTickInfo.find( iType );
		if( mit == m_mapTickInfo.end() )
			return;

		const KTickInfo& kTickInfo = mit->second;

		kInfo.m_dwMaxTick			= kTickInfo.m_dwMaxTick;
		kInfo.m_wstrMaxTickRegDate	= ( std::wstring )( kTickInfo.m_tMaxRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );		
		kInfo.m_iTotalTickCount		= kTickInfo.m_iTotalLatency;
		kInfo.m_iPassedTickCount	= kTickInfo.m_iPassedTickCount;

		if( kTickInfo.m_iPassedTickCount == 0 )
		{
			kInfo.m_iAverageTickCount = 0;
		}
		else
		{
			kInfo.m_iAverageTickCount = kTickInfo.m_iTotalLatency / kTickInfo.m_iPassedTickCount;
		}
	}
	
	ERR_CODE CheckOneTickLatency( const int iType, const DWORD dwDelayTick )
	{
		std::map< int, KTickInfo >::iterator mit;
		mit = m_mapTickInfo.find( iType );
		if( mit == m_mapTickInfo.end() )
		{
			KTickInfo kNewInfo;
			kNewInfo.m_dwMaxTick = dwDelayTick;
			kNewInfo.m_iPassedTickCount = 1;
			kNewInfo.m_iTotalLatency += dwDelayTick;
			kNewInfo.m_tMaxRegTime = CTime::GetCurrentTime();
			m_mapTickInfo.insert( std::make_pair( iType, kNewInfo ) );
            
			return EC_FIRST_CHECK;
		}

		KTickInfo& kTickInfo = mit->second;

		ERR_CODE eRet = EC_NONE;

		// ��հ��� ���ϱ� ���� ������ ����
		++kTickInfo.m_iPassedTickCount;
		kTickInfo.m_iTotalLatency += static_cast<__int64>(dwDelayTick);

		// �ִ밪 ���ϱ�
		if( dwDelayTick > kTickInfo.m_dwMaxTick )
		{
			eRet = EC_OVER_MAX_TICK;

			kTickInfo.m_dwMaxTick = dwDelayTick;
			kTickInfo.m_tMaxRegTime = CTime::GetCurrentTime();
		}

		return eRet;
	}

private:
	void Reset()
	{
		m_mapTickInfo.clear();
	}

private:
	std::map< int, KTickInfo >	m_mapTickInfo;
};


#pragma pack( pop )

