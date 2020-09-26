#include ".\Profiler.h"

#include <KncLua.h>
#include <lua_tinker.h>
#include <dbg/dbg.hpp>
#include "lua/KLuaManager.h"
#include "BaseServer.h"


ImplementSingleton( KProfiler );

KProfiler::KProfiler(void) : 
m_dFrequency( 0.0f ),
m_dEndProfile( 0.0f ),
m_dBeginProfile( 0.0f )
{
	lua_tinker::class_add<KProfiler>( g_pLua, "KProfiler" );
	lua_tinker::class_def<KProfiler>( g_pLua, "dump",						&KProfiler::Dump );

	lua_tinker::decl( g_pLua, "Profiler", this );

	// �ʱ�ȭ
	Init();
}

KProfiler::~KProfiler(void)
{
}

ImplToStringW( KProfiler )
{
    SiKProfiler()->DumpProfileOutput( stm_ );
	return stm_;
}

void KProfiler::Init()
{
	LARGE_INTEGER CurrentFrequency;

	// cpu�� clock speed�� Ȯ���Ѵ�
	QueryPerformanceFrequency( &CurrentFrequency );

	m_dFrequency = static_cast<double>( CurrentFrequency.QuadPart );
	m_dBeginProfile = GetExactTime();
}

//void KProfiler::Tick()
//{
//	// 3�п� �ѹ� ����
//	if( m_kDumpTimer.elapsed() < 180 )
//		return;
//
//	m_kDumpTimer.restart();
//
//	// ���� ����!
//	Dump();
//}

double KProfiler::GetExactTime()
{
	LARGE_INTEGER CurrentTime;

	// ������ ��Ȯ�� �ð�
	QueryPerformanceCounter( &CurrentTime );

	return ( (double) CurrentTime.QuadPart ) / m_dFrequency;
}

void KProfiler::Begin( wchar_t* lpwName )
{
	KLocker kLock( m_csProfileSample );

	unsigned int i = 0;

	// 1. �ش� �̸��� �´� ����� ã��
	while( i < MAX_PROFILE_SAMPLE  &&  m_oSample[i].m_bValid == true )
	{
		// 1-1. ����� ã������ ���۽ð� ����
		if( wcscmp( m_oSample[i].m_wszName, lpwName ) == 0 )
		{
			m_oSample[i].m_iOpenProfiles++;		// �������� ���� ī��Ʈ
			m_oSample[i].m_uiProfileInstance++; // �������� Ƚ��

			m_oSample[i].m_dStartTime = GetExactTime();
			return;
		}

		++i;
	}

	// 2. �ִ�ũ�� �Ѿ�� ����
    if( i >= MAX_PROFILE_SAMPLE )
	{
		START_LOG( cerr, L"������ ���ڶ�!" )
			<< BUILD_LOG( i ) 
			<< END_LOG;
		return;
	}

	// 3. ó�� ��Ͻ� �ʱ�ȭ
#ifdef _CONVERT_VS_2010
	wcscpy_s( m_oSample[i].m_wszName, lpwName );
#else
	wcscpy( m_oSample[i].m_wszName, lpwName );
#endif _CONVERT_VS_2010

	m_oSample[i].m_bValid = true;
	m_oSample[i].m_iOpenProfiles = 1;
	m_oSample[i].m_uiProfileInstance = 1;
	m_oSample[i].m_dAccumulator = 0.0f;
	m_oSample[i].m_dStartTime = GetExactTime();
	//m_oSample[i].m_dChildrenSampleTime = 0;
	m_oSample[i].m_dwMinTime = 100000.0f;
	m_oSample[i].m_dwMaxTime = 0.0f;
	m_oSample[i].m_dwAvgTime = 0.0f;
}

void KProfiler::End( wchar_t* lpwName )
{
	KLocker kLock( m_csProfileSample );

	unsigned int i = 0;
	//unsigned int NumOfParents = 0;
	
	while( i < MAX_PROFILE_SAMPLE  &&  m_oSample[i].m_bValid == true )
	{
		// 1. ����� ã������ �ð� ���
		if( wcscmp( m_oSample[i].m_wszName, lpwName ) == 0 )
		{
			//unsigned int iInner = 0;
			//int iParent = -1;
			
			double dElapsedTime = ( GetExactTime() - m_oSample[i].m_dStartTime );

			m_oSample[i].m_iOpenProfiles--; // �������� ���� ī��Ʈ ����

			// ���� ���ÿ� ���� ����� end���� ���� ������ ã��!
			//while( m_oSample[iInner].m_bValid == true )
			//{
			//	if( m_oSample[iInner].m_iOpenProfiles > 0 )
			//	{
			//		++NumOfParents;

			//		if( iParent < 0 )
			//			iParent = iInner;
			//		else if( m_oSample[iInner].m_dStartTime >= m_oSample[iParent].m_dStartTime )
			//			iParent = iInner;
			//	}

			//	++iInner;
			//}

			//m_oSample[i].m_uiNumberOfParents = NumOfParents;			

			//if( iParent >= 0 )
			//{
			//	// ���� ������ �ɸ��ð����� ���� ������ �ɸ� �ð��� ���� ���� ����
   //             m_oSample[iParent].m_dChildrenSampleTime += dElapsedTime;
			//}

			// �ɸ� �ð� ���
			m_oSample[i].m_dAccumulator += dElapsedTime;

			// min
			if( m_oSample[i].m_dwMinTime > dElapsedTime )
				m_oSample[i].m_dwMinTime = dElapsedTime;

			// max
			if( m_oSample[i].m_dwMaxTime < dElapsedTime )
				m_oSample[i].m_dwMaxTime = dElapsedTime;

			// avg
			if( m_oSample[i].m_uiProfileInstance > 0 )
			{
				m_oSample[i].m_dwAvgTime = ( m_oSample[i].m_dAccumulator / m_oSample[i].m_uiProfileInstance );
			}
			else
			{
				START_LOG( cerr, L"profile instance�� 0�̴�? begin�� �ȵǾ���?" )
					<< BUILD_LOG( lpwName )
					<< END_LOG;
			}
			return;
		}
		++i;
	}

	START_LOG( cerr, L"end�� ã�� ���ߴ�!?" )
		<< BUILD_LOG( lpwName )
		<< END_LOG;
}

void KProfiler::DumpProfileOutput( std::wostream& stm_ )
{
	KLocker kLock( m_csProfileSample );

	////////////////////////////////////////////////////////////////////////// end ���Ȱ� ������ ������ end
	// ����ȸ	

	//int ir = MAX_PROFILE_SAMPLE;

	//while( ir >= 0 )
	//{
	//	if( m_oSample[ir].m_bValid )
	//	{
	//		if( m_oSample[ir].m_iOpenProfiles > 0 )
	//		{
	//			End( m_oSample[ir].m_wszName );
	//		}
	//	}
	//	
	//	--ir;
	//}

	//////////////////////////////////////////////////////////////////////////

    double dwPreviousEndProfile = m_dEndProfile;
	m_dEndProfile = GetExactTime();

    double dwTotalProfile = m_dEndProfile - m_dBeginProfile;

	////////////////////////////////////////////////////////////////////////// Sample
	unsigned int i = 0;

	stm_ << L" Total Sample Time : " << ( m_dEndProfile - dwPreviousEndProfile ) << std::endl;
	stm_ << L"    Avg    :    Min    :    Max    :    Num    :    Time    : Profile Name" << std::endl;
	stm_ << L"----------------------------------------------------------------------" << std::endl;

    std::vector< int > vecOpened;
	while( i < MAX_PROFILE_SAMPLE  &&  m_oSample[i].m_bValid == true )
	{
        if( m_oSample[i].m_iOpenProfiles > 0 )
        {
            vecOpened.push_back( i );
            End( m_oSample[i].m_wszName );
        }
		//unsigned int Indent = 0;

		wchar_t Line[256] = {0};
		wchar_t Name[256] = {0};
		wchar_t IndentedName[256] = {0};

		wchar_t Average[16] = {0};
		wchar_t Minimum[16] = {0};
		wchar_t Maximum[16] = {0};
		wchar_t Number[16] = {0};
		wchar_t Total[16] = {0};

		// ���
#ifdef _CONVERT_VS_2010
		_snwprintf_s( Average, 16, L"%2.5f", m_oSample[i].m_dwAvgTime );
		_snwprintf_s( Minimum, 16, L"%2.5f", m_oSample[i].m_dwMinTime );
		_snwprintf_s( Maximum, 16, L"%2.5f", m_oSample[i].m_dwMaxTime );
		_snwprintf_s( Number, 16, L"%9d", m_oSample[i].m_uiProfileInstance );
		_snwprintf_s( Total, 16, L"%2.5f", m_oSample[i].m_dAccumulator );

		wcsncpy_s( IndentedName, m_oSample[i].m_wszName, 256 );

		//for( Indent = 0; Indent < m_oSample[i].m_uiNumberOfParents; ++Indent )
		//{
		//	_snwprintf( Name, 256, L"  %s", IndentedName );
		//	wcsncpy( IndentedName, Name, 256 );
		//}

		_snwprintf_s( Line, 256, L"%9s : %9s : %9s : %9s : %9s : %s\n", Average, Minimum, Maximum, Number, Total, IndentedName );
#else
_snwprintf( Average, 16, L"%2.5f", m_oSample[i].m_dwAvgTime );
		_snwprintf( Minimum, 16, L"%2.5f", m_oSample[i].m_dwMinTime );
		_snwprintf( Maximum, 16, L"%2.5f", m_oSample[i].m_dwMaxTime );
		_snwprintf( Number, 16, L"%9d", m_oSample[i].m_uiProfileInstance );
		_snwprintf( Total, 16, L"%2.5f", m_oSample[i].m_dAccumulator );

		wcsncpy( IndentedName, m_oSample[i].m_wszName, 256 );

		//for( Indent = 0; Indent < m_oSample[i].m_uiNumberOfParents; ++Indent )
		//{
		//	_snwprintf( Name, 256, L"  %s", IndentedName );
		//	wcsncpy( IndentedName, Name, 256 );
		//}

		_snwprintf( Line, 256, L"%9s : %9s : %9s : %9s : %9s : %s\n", Average, Minimum, Maximum, Number, Total, IndentedName );
#endif _CONVERT_VS_2010
		stm_ << Line;

		++i;
	}

	stm_ << L"----------------------------------------------------------------------" << std::endl;
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////// History
	i = 0;


	stm_ << std::endl << L" Total History Time : " << dwTotalProfile << std::endl;
	stm_ << L"    Avg    :    Min    :    Max    :    Num    : Profile Name" << std::endl;
	stm_ << L"----------------------------------------------------------------------" << std::endl;

	while( i < MAX_PROFILE_SAMPLE  &&  m_oSample[i].m_bValid == true )
	{
		//unsigned int Indent = 0;

		wchar_t Line[256] = {0};
		wchar_t Name[256] = {0};
		wchar_t IndentedName[256] = {0};

		wchar_t Average[16] = {0};
		wchar_t Minimum[16] = {0};
		wchar_t Maximum[16] = {0};
		wchar_t Number[16] = {0};

		// 1. �ɸ��ð������� �����丮�� ���� �� �����Ѵ�
		if( !StoreProfileInHistory( m_oSample[i].m_wszName, m_oSample[i] ) )
		{
			START_LOG( cerr, L"���� ����!" )
				<< BUILD_LOG( i )
				<< END_LOG;
			continue;
		}

		// 2. �����丮�� ���� ������ �ð������� ���´�
		unsigned int uiHistoryIndex = 0;
		if( !GetProfileFromHistory( m_oSample[i].m_wszName, uiHistoryIndex ) )
		{
			START_LOG( cerr, L"�ε��� ��� ����!" )
				<< BUILD_LOG( i )
				<< END_LOG;
			continue;
		}

		// 3. ���
#ifdef _CONVERT_VS_2010
		_snwprintf_s( Average, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dAverage );
		_snwprintf_s( Minimum, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dMinimun );
		_snwprintf_s( Maximum, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dMaximum );
		_snwprintf_s( Number, 16, L"%9d", m_oHistories[uiHistoryIndex].m_uiProfileInstance );

		wcsncpy_s( IndentedName, m_oHistories[uiHistoryIndex].m_wszName, 256 );

		//for( Indent = 0; Indent < m_oSample[i].m_uiNumberOfParents; ++Indent )
		//{
		//	_snwprintf( Name, 256, L"  %s", IndentedName );
		//	wcsncpy( IndentedName, Name, 256 );
		//}

		_snwprintf_s( Line, 256, L"%9s : %9s : %9s : %9s : %s\n", Average, Minimum, Maximum, Number, IndentedName );
#else
		_snwprintf( Average, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dAverage );
		_snwprintf( Minimum, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dMinimun );
		_snwprintf( Maximum, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dMaximum );
		_snwprintf( Number, 16, L"%9d", m_oHistories[uiHistoryIndex].m_uiProfileInstance );

		wcsncpy( IndentedName, m_oHistories[uiHistoryIndex].m_wszName, 256 );

		//for( Indent = 0; Indent < m_oSample[i].m_uiNumberOfParents; ++Indent )
		//{
		//	_snwprintf( Name, 256, L"  %s", IndentedName );
		//	wcsncpy( IndentedName, Name, 256 );
		//}

		_snwprintf( Line, 256, L"%9s : %9s : %9s : %9s : %s\n", Average, Minimum, Maximum, Number, IndentedName );
#endif _CONVERT_VS_2010
		stm_ << Line;

		++i;
	}

	stm_ << L"----------------------------------------------------------------------" << std::endl;
	//////////////////////////////////////////////////////////////////////////

    std::vector< int >::iterator vit;
    for( vit = vecOpened.begin(); vit != vecOpened.end(); vit++ )
    {
        Begin( m_oSample[*vit].m_wszName );
    }

	// ������ ������ ����Ҵ� �ʱ�ȭ
    for( i = 0; i < MAX_PROFILE_SAMPLE; ++i )
	{
		m_oSample[i].m_bValid = false;
	}
}

void KProfiler::DumpToLogFile()
{
	KLocker kLock( m_csProfileSample );

	////////////////////////////////////////////////////////////////////////// end ���Ȱ� ������ ������ end
	// ����ȸ	

	//int ir = MAX_PROFILE_SAMPLE;

	//while( ir >= 0 )
	//{
	//	if( m_oSample[ir].m_bValid )
	//	{
	//		if( m_oSample[ir].m_iOpenProfiles > 0 )
	//		{
	//			End( m_oSample[ir].m_wszName );
	//		}
	//	}
	//	
	//	--ir;
	//}

	//////////////////////////////////////////////////////////////////////////

	double dwPreviousEndProfile = m_dEndProfile;
	m_dEndProfile = GetExactTime();

	double dwTotalProfile = m_dEndProfile - m_dBeginProfile;

	////////////////////////////////////////////////////////////////////////// Sample
	unsigned int i = 0;

	dbg::cout << L" Total Sample Time : " << ( m_dEndProfile - dwPreviousEndProfile ) << dbg::endl;
	dbg::cout << L"    Avg    :    Min    :    Max    :    Num    :    Time    : Profile Name" << dbg::endl;
	dbg::cout << L"----------------------------------------------------------------------" << dbg::endl;

	std::vector< int > vecOpened;
	while( i < MAX_PROFILE_SAMPLE  &&  m_oSample[i].m_bValid == true )
	{
		if( m_oSample[i].m_iOpenProfiles > 0 )
		{
			vecOpened.push_back( i );
			End( m_oSample[i].m_wszName );
		}
		//unsigned int Indent = 0;

		wchar_t Line[256] = {0};
		wchar_t Name[256] = {0};
		wchar_t IndentedName[256] = {0};

		wchar_t Average[16] = {0};
		wchar_t Minimum[16] = {0};
		wchar_t Maximum[16] = {0};
		wchar_t Number[16] = {0};
		wchar_t Total[16] = {0};

		// ���
#ifdef _CONVERT_VS_2010
		_snwprintf_s( Average, 16, L"%2.5f", m_oSample[i].m_dwAvgTime );
		_snwprintf_s( Minimum, 16, L"%2.5f", m_oSample[i].m_dwMinTime );
		_snwprintf_s( Maximum, 16, L"%2.5f", m_oSample[i].m_dwMaxTime );
		_snwprintf_s( Number, 16, L"%9d", m_oSample[i].m_uiProfileInstance );
		_snwprintf_s( Total, 16, L"%2.5f", m_oSample[i].m_dAccumulator );

		wcsncpy_s( IndentedName, m_oSample[i].m_wszName, 256 );

		//for( Indent = 0; Indent < m_oSample[i].m_uiNumberOfParents; ++Indent )
		//{
		//	_snwprintf( Name, 256, L"  %s", IndentedName );
		//	wcsncpy( IndentedName, Name, 256 );
		//}

		_snwprintf_s( Line, 256, L"%9s : %9s : %9s : %9s : %9s : %s\n", Average, Minimum, Maximum, Number, Total, IndentedName );
#else
		_snwprintf( Average, 16, L"%2.5f", m_oSample[i].m_dwAvgTime );
		_snwprintf( Minimum, 16, L"%2.5f", m_oSample[i].m_dwMinTime );
		_snwprintf( Maximum, 16, L"%2.5f", m_oSample[i].m_dwMaxTime );
		_snwprintf( Number, 16, L"%9d", m_oSample[i].m_uiProfileInstance );
		_snwprintf( Total, 16, L"%2.5f", m_oSample[i].m_dAccumulator );

		wcsncpy( IndentedName, m_oSample[i].m_wszName, 256 );

		//for( Indent = 0; Indent < m_oSample[i].m_uiNumberOfParents; ++Indent )
		//{
		//	_snwprintf( Name, 256, L"  %s", IndentedName );
		//	wcsncpy( IndentedName, Name, 256 );
		//}

		_snwprintf( Line, 256, L"%9s : %9s : %9s : %9s : %9s : %s\n", Average, Minimum, Maximum, Number, Total, IndentedName );
#endif _CONVERT_VS_2010
		dbg::cout << Line;

		++i;
	}

	dbg::cout << L"----------------------------------------------------------------------" << dbg::endl;
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////// History
	i = 0;


	dbg::cout << dbg::endl << L" Total History Time : " << dwTotalProfile << dbg::endl;
	dbg::cout << L"    Avg    :    Min    :    Max    :    Num    : Profile Name" << dbg::endl;
	dbg::cout << L"----------------------------------------------------------------------" << dbg::endl;

	while( i < MAX_PROFILE_SAMPLE  &&  m_oSample[i].m_bValid == true )
	{
		//unsigned int Indent = 0;

		wchar_t Line[256] = {0};
		wchar_t Name[256] = {0};
		wchar_t IndentedName[256] = {0};

		wchar_t Average[16] = {0};
		wchar_t Minimum[16] = {0};
		wchar_t Maximum[16] = {0};
		wchar_t Number[16] = {0};

		// 1. �ɸ��ð������� �����丮�� ���� �� �����Ѵ�
		if( !StoreProfileInHistory( m_oSample[i].m_wszName, m_oSample[i] ) )
		{
			START_LOG( cerr, L"���� ����!" )
				<< BUILD_LOG( i )
				<< END_LOG;
			continue;
		}

		// 2. �����丮�� ���� ������ �ð������� ���´�
		unsigned int uiHistoryIndex = 0;
		if( !GetProfileFromHistory( m_oSample[i].m_wszName, uiHistoryIndex ) )
		{
			START_LOG( cerr, L"�ε��� ��� ����!" )
				<< BUILD_LOG( i )
				<< END_LOG;
			continue;
		}

		// 3. ���
#ifdef _CONVERT_VS_2010
		_snwprintf_s( Average, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dAverage );
		_snwprintf_s( Minimum, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dMinimun );
		_snwprintf_s( Maximum, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dMaximum );
		_snwprintf_s( Number, 16, L"%9d", m_oHistories[uiHistoryIndex].m_uiProfileInstance );

		wcsncpy_s( IndentedName, m_oHistories[uiHistoryIndex].m_wszName, 256 );

		//for( Indent = 0; Indent < m_oSample[i].m_uiNumberOfParents; ++Indent )
		//{
		//	_snwprintf( Name, 256, L"  %s", IndentedName );
		//	wcsncpy( IndentedName, Name, 256 );
		//}

		_snwprintf_s( Line, 256, L"%9s : %9s : %9s : %9s : %s\n", Average, Minimum, Maximum, Number, IndentedName );
#else
		_snwprintf( Average, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dAverage );
		_snwprintf( Minimum, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dMinimun );
		_snwprintf( Maximum, 16, L"%2.5f", m_oHistories[uiHistoryIndex].m_dMaximum );
		_snwprintf( Number, 16, L"%9d", m_oHistories[uiHistoryIndex].m_uiProfileInstance );

		wcsncpy( IndentedName, m_oHistories[uiHistoryIndex].m_wszName, 256 );

		//for( Indent = 0; Indent < m_oSample[i].m_uiNumberOfParents; ++Indent )
		//{
		//	_snwprintf( Name, 256, L"  %s", IndentedName );
		//	wcsncpy( IndentedName, Name, 256 );
		//}

		_snwprintf( Line, 256, L"%9s : %9s : %9s : %9s : %s\n", Average, Minimum, Maximum, Number, IndentedName );
#endif _CONVERT_VS_2010
		dbg::cout << Line;

		++i;
	}

	dbg::cout << L"----------------------------------------------------------------------" << dbg::endl;
	//////////////////////////////////////////////////////////////////////////

	std::vector< int >::iterator vit;
	for( vit = vecOpened.begin(); vit != vecOpened.end(); vit++ )
	{
		Begin( m_oSample[*vit].m_wszName );
	}

	// ������ ������ ����Ҵ� �ʱ�ȭ
	for( i = 0; i < MAX_PROFILE_SAMPLE; ++i )
	{
		m_oSample[i].m_bValid = false;
	}
}

bool KProfiler::StoreProfileInHistory( wchar_t* lpwName, const PROFILE_SAMPLE& oSample )
{
	unsigned int i = 0;

	while( i < MAX_PROFILE_SAMPLE  &&  m_oHistories[i].m_bValid == true )
	{
		if( wcscmp( m_oHistories[i].m_wszName, lpwName ) == 0 )
		{
			unsigned int uiTotalProfileInstance = m_oHistories[i].m_uiProfileInstance + oSample.m_uiProfileInstance;			

			m_oHistories[i].m_dAverage = ( ( ( m_oHistories[i].m_dAverage * (double)m_oHistories[i].m_uiProfileInstance ) + (double)oSample.m_dAccumulator ) / (double)uiTotalProfileInstance );

			if( m_oHistories[i].m_dMinimun > oSample.m_dwMinTime )
				m_oHistories[i].m_dMinimun = oSample.m_dwMinTime;

			if( m_oHistories[i].m_dMaximum < oSample.m_dwMaxTime )
				m_oHistories[i].m_dMaximum = oSample.m_dwMaxTime;
			
			m_oHistories[i].m_uiProfileInstance = uiTotalProfileInstance; // Ƚ�� ����			
			return true;
		}

		++i;
	}

	if( i < MAX_PROFILE_SAMPLE )
	{
#ifdef _CONVERT_VS_2010
		wcsncpy_s( m_oHistories[i].m_wszName, lpwName, MAX_PATH );
#else
		wcsncpy( m_oHistories[i].m_wszName, lpwName, MAX_PATH );
#endif _CONVERT_VS_2010

		m_oHistories[i].m_bValid = true;
		m_oHistories[i].m_dAverage = oSample.m_dwAvgTime;
		m_oHistories[i].m_dMinimun = oSample.m_dwMinTime;
		m_oHistories[i].m_dMaximum = oSample.m_dwMaxTime;
		m_oHistories[i].m_uiProfileInstance = oSample.m_uiProfileInstance;
		return true;
	}

	START_LOG( cerr, L"���� ���� �����丮�� ����!" )
		<< BUILD_LOG( lpwName )
		<< BUILD_LOG( i )
		<< END_LOG;
	return false;
}

bool KProfiler::GetProfileFromHistory( wchar_t* lpwName, unsigned int& uiHistoryIndex )
{
	unsigned int i = 0;

	while( i < MAX_PROFILE_SAMPLE  &&  m_oHistories[i].m_bValid == true )
	{
		if( wcscmp( m_oHistories[i].m_wszName, lpwName ) == 0 )
		{
			uiHistoryIndex = i;
			return true;
		}
		++i;
	}

    return false;
}



