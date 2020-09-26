#pragma once

#include "RefreshSingleton.h"
#include <map>
#include <vector>

typedef unsigned char	u_char;
#define IN
#define OUT


class CXSLGuildManager
{
	DeclareRefreshSingleton( CXSLGuildManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	struct KCharactorFactor
	{
		unsigned char	m_ucBeginLv;
		unsigned char	m_ucEndLv;
		double			m_fFactor;

		KCharactorFactor()
		{
			m_ucBeginLv = 0;
			m_ucEndLv = 0;
			m_fFactor = 0.0f;
		}
	};

	struct KGuildEXP
	{
		int				m_iNeedEXP;
		int				m_iTotalEXP;

		KGuildEXP()
		{
			m_iNeedEXP = 0;
			m_iTotalEXP = 0;
		}
	};	

public:
	static bool IsValidSkillPointFigure( IN const int iSkillPointFigure ){	return ( ( 0 <= iSkillPointFigure ) && ( iSkillPointFigure <= 128 ) ) ? true : false;	}

	CXSLGuildManager(void);
	~CXSLGuildManager(void);

	bool OpenScriptFile( const char* pFileName );

	// for lua
	void SetLimitGuildLevel_LUA( u_char ucGuildLevel );
	bool AddGuildEXP_LUA( u_char ucGuildLevel, int iNeedEXP, int iTotalEXP );
	bool SetCharLevelFactor_LUA( u_char ucBeginLv, u_char ucEndLv, double fFactor );
	bool SetGuildMemberGradeFactor_LUA( u_char ucGuildMemberGrade, double fFactor );
	bool SetPcBangFactor_LUA( double fFactor );
	//{{ 2009. 11. 16  ������	�����������Ʈ
	bool SetPvpMemberFactor_LUA( int iPvpMember, double fFactor );
	//}}

	// ��� ���� ����ġ
	int	GetRequireTotalExpbyGuildLevel( IN u_char ucGuildLevel ) const;
	int GetGuildLimitEXP() const;
	u_char CheckGuildLevelUp( IN u_char ucGuildLevel, IN int iGuildExp ) const;	

	// �� ����Ʈ ���
	int GetHonorPoint_Dungeon( IN u_char ucLevel, IN u_char ucGuildMemberGrade, IN int iDecreaseSpirit, IN bool bIsPcBang );
	int GetHonorPoint_Pvp( IN u_char ucLevel, IN u_char ucGuildMemberGrade, IN int iPvpMemberCount, IN bool bIsPcBang );

private:
    double GetCharacterLevelFactor( IN u_char ucLevel ) const;
	double GetMemberGradeFactor( IN u_char ucGuildMemberGrade ) const;
	double GetPvpMemberFactor( IN int iPvpMemberCount ) const;

private:
	std::map< u_char, KGuildEXP >	m_mapGuildEXPTable;
	std::vector< KCharactorFactor > m_vecCharFactor;
	std::map< u_char, double >		m_mapMemberGradeFactor;
	double							m_fPcBangFactor;
	//{{ 2009. 11. 16  ������	�����������Ʈ
	std::map< int, double >			m_mapPvpMemberFactor;
	//}}
	u_char							m_ucLimitLevel;
};

DefRefreshSingletonInline( CXSLGuildManager );


