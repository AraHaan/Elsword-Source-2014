#pragma once

#include <ToString.h>
#include <KNCSingleton.h>
#include <Thread/Locker.h>
#include <boost/timer.hpp>
//#include "BaseServer.h"

#define MAX_PROFILE_SAMPLE	256
#define MAX_PATH			260

#define AUTO_PROFILE()					KAutoProfile kAutoProfile( __WFUNCTION__ );
#define AUTO_PROFILE_NAME( funcname )	KAutoProfile kAutoProfile( funcname );
#define BEGIN_PROFILE( funcName )		SiKProfiler()->Begin( funcName );
#define END_PROFILE( funcName )			SiKProfiler()->End( funcName );

struct PROFILE_SAMPLE_HISTORY
{
	bool			m_bValid;
	wchar_t			m_wszName[MAX_PATH];
	double			m_dAverage;
	double			m_dMinimun;
	double			m_dMaximum;

	unsigned int	m_uiProfileInstance;	// Ƚ�� ����
};

struct PROFILE_SAMPLE
{
	bool			m_bValid;
	unsigned int	m_uiProfileInstance;	// Ƚ��
	__int32			m_iOpenProfiles;
	wchar_t			m_wszName[MAX_PATH];
	double			m_dStartTime;
	double			m_dAccumulator;
	//DWORD			m_dChildrenSampleTime;
	//unsigned int	m_uiNumberOfParents;

	double			m_dwMinTime;			// �ּҰɸ��ð�
	double			m_dwMaxTime;			// �ִ�ɸ��ð�
	double			m_dwAvgTime;			// ��հɸ��ð�
};

class KProfiler
{
	DeclareSingleton( KProfiler );
	DeclToStringW;
	DeclDump;

public:
	KProfiler(void);
	~KProfiler(void);

	void Init();

	void DumpToLogFile();
	//void Tick();

	double GetExactTime();	

	void Begin( wchar_t* lpwName );
	void End( wchar_t* lpwName );

	void DumpProfileOutput( std::wostream& stm_ );
	bool StoreProfileInHistory( wchar_t* lpwName, const PROFILE_SAMPLE& oSample );
	bool GetProfileFromHistory( wchar_t* lpwName, unsigned int& uiHistoryIndex );

private:
	double					m_dFrequency;
	double					m_dEndProfile;
	double					m_dBeginProfile;

	mutable KncCriticalSection	m_csProfileSample;
	PROFILE_SAMPLE			m_oSample[MAX_PROFILE_SAMPLE];
	PROFILE_SAMPLE_HISTORY	m_oHistories[MAX_PROFILE_SAMPLE];

	boost::timer			m_kDumpTimer;
};

DefSingletonInline( KProfiler );

class KAutoProfile
{
public:
	KAutoProfile( wchar_t* lpwName ) { m_lpwName = lpwName; SiKProfiler()->Begin( m_lpwName ); }
	~KAutoProfile()					 { SiKProfiler()->End( m_lpwName ); }
private:
	wchar_t* m_lpwName;
};


