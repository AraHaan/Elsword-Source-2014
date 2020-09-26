#pragma once

#include "ServerDefine.h"

#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-10-14	// �ڼ���
#include "RefreshSingleton.h"
#include <set>

class CXSLFieldBossData
{
	DeclareRefreshSingleton( CXSLFieldBossData );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

	typedef	std::map< float, std::map< int, byte > >	TYPE_REWARD_DATA;

public:
	void	SetData_LUA( void );

	bool	DoesFieldAffectTotalDangerousValue( IN int iVillageMapID ) const	{	return m_setPortalLocation.find( iVillageMapID ) != m_setPortalLocation.end();	}
	bool	GetRandomPortalLocationID( OUT int& iPortalLocationID ) const;
	
	byte	GetTotalDangerousValue( void ) const	{	return m_byteTotalDangerousValue;	}
	int		GetFieldBossCoolTime_M( void ) const	{	return m_iFieldBossCoolTime_M;	}
	int		GetFieldBossPortalHoldingTime_M( void ) const	{	return m_iPortalHoldingTime_M;	}
	int		GetFieldBossFieldHoldingTime_M( void ) const	{	return m_iFieldHoldingTime_M;	}
	byte	GetFieldBossEndGameFieldHoldingTime_S( void ) const	{	return m_byteEndGameFieldHoldingTime_S;	}
	USHORT	GetFieldBossEndGameFieldHoldingTimeForce_S( void ) const	{	return ( m_byteEndGameFieldHoldingTime_S + m_byteEndGameFieldHoldingTimeForce_S );	}
	byte	GetIntrudeRestrictionTime_S( void ) const	{	return m_byteIntrudeRestrictionTime_S;	}
	byte	GetMaximumExpandedSocketCount( void ) const	{	return m_byteMaximumExpandedSocketCount;	}
	float	GetLimiteContribution( void ) const	{	return m_fLimiteContribution;	}
	float	GetMaximumContribution( void ) const	{	return m_fMaximumContribution;	}

	void	GetPrecedenceRewardData( IN const size_t iRank, IN const size_t iWholeSize, OUT std::map<int, byte>& mapRewardData ) const;

	bool	IsSocketExpandItemForWeapon( IN const int iItemID ) const	{	return m_setSocketExpandItemForWeapon.find( iItemID ) != m_setSocketExpandItemForWeapon.end();	}
	bool	IsSocketExpandItemForDefence( IN const int iItemID ) const	{	return m_setSocketExpandItemForDefence.find( iItemID ) != m_setSocketExpandItemForDefence.end();	}

private:
	CXSLFieldBossData( void );
	~CXSLFieldBossData( void );

	template<typename T1, typename T2>	bool	_SetData( OUT T1& typeValue, IN const T2 typeMinValue, IN const T2 typeSetValue, IN const T2 typeMaxValue ) const;
	void	_CheckData( void ) const;
	bool	_PrecedenceAndRewardDataParsing( IN KLuaManager& luaManager );
	bool	_RewardParsing( IN KLuaManager& luaManager, OUT std::map<int, byte>& mapReward ) const;

private:
	std::set<int>		m_setSocketExpandItemForWeapon;
	std::set<int>		m_setSocketExpandItemForDefence;
	std::set<int>		m_setPortalLocation;		// ���� �ʵ�� �̵��ϱ� ���� ��Ż ���� ��ġ ����Ʈ ( VillageMapID )
	TYPE_REWARD_DATA	m_mapRewardData;
	
	int		m_iFieldBossCoolTime_M;					// �ʵ� ���� �� ���� ���� �ð�
	int		m_iPortalHoldingTime_M;					// ���� �ʵ�� �̵��ϱ� ���� ��Ż�� ���� �ð�
	int		m_iFieldHoldingTime_M;					// ���� �ʵ� ���� ���� �� ���� �ð�

	float	m_fLimiteContribution;
	float	m_fMaximumContribution;
	
	byte	m_byteTotalDangerousValue;
	byte	m_byteEndGameFieldHoldingTime_S;		// ���� ���� ���� �� ���� �ʵ� ���� �ð�
	byte	m_byteEndGameFieldHoldingTimeForce_S;	// ���� �ʵ� ���� �ð� ���� ���� ��������� ���� �ð�
	byte	m_byteIntrudeRestrictionTime_S;			// ���� �ð��� �� �ð� �̸��� ��� ���� �Ұ��� �ȴ�.

	byte	m_byteMaximumExpandedSocketCount;		// �ִ� Ȯ�� ������ ���� ��
};

template<typename T1, typename T2>
bool CXSLFieldBossData::_SetData( OUT T1& typeValue, IN const T2 typeMinValue, IN const T2 typeSetValue, IN const T2 typeMaxValue ) const
{
	if( ( typeSetValue < typeMinValue )
		|| ( typeMaxValue < typeSetValue )
		)
	{
		START_LOG( cerr, L"������ �� �ִ� ������ ���� �ƴմϴ�.")
			<< BUILD_LOG( typeMinValue )
			<< BUILD_LOG( typeSetValue )
			<< BUILD_LOG( typeMaxValue )
			<< END_LOG;
		return false;
	}

	typeValue = static_cast<T1>( typeSetValue );

	return true;
}

DefRefreshSingletonInline( CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS
