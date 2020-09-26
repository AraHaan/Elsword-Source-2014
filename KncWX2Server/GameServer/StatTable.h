#pragma once

#include <vector>
#include <ToString.h>
#include <KNCSingleton.h>
#include <KncUtil.h>    // IN, OUT, windows.h...
#include "X2Data/XSLUnit.h"

class KStatTable
{
	DeclareSingleton( KStatTable );

public:
	struct KUnitStatInfo
	{
        KUnitStatInfo();    // default constructor

		//����
		USHORT	m_usAtkPhysic;
		USHORT	m_usAtkMagic;

		//���
		USHORT	m_usDefPhysic;
		USHORT	m_usDefMagic;

		//{{ 2008. 4. 11  ������  
		UINT	m_uiHP;
		//}}
	};


public:
	virtual ~KStatTable();
	DeclToStringW;
	DeclDump;

	bool OpenScriptFile( const char* pFileName );
	bool GetUnitStat( IN char cUnitClass, IN u_char unitLevel, OUT KUnitStatInfo& sUnitStat );
    void SetUnitStat( IN int nUnitClass, IN u_char unitLevel );

    // 070605. florist. ���� �̷��� ���� �ʾƵ� ������ ���� �˻縦 Ȯ���� �ϱ� ���� �̸� ������ �Է¹޴´�.
    void ReserveMemory( IN int nUnitClass, IN u_char ucMaxLevel );

protected:
	KStatTable();

	void RegisterLuaBind();

	//{{ 2008. 2. 18  ������  1�� �������� ���� �ڷ��� ����
	std::map< int, std::vector< KUnitStatInfo > > m_mapStat;
	//}}
};

DefSingletonInline( KStatTable );
