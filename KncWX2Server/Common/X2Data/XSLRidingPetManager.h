#pragma once
#include "ServerDefine.h"

#ifdef	SERV_RIDING_PET_SYSTM// ���볯¥: 2013-04-21
#include "RefreshSingleton.h"

class CXSLRidingPetManager
{
	DeclareRefreshSingleton( CXSLRidingPetManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum RIDING_PET_UNIT_ID
	{
		RPUI_NONE				= 0,
		RPUI_NASOD_MOBI			= 1,	/// ���ҵ� ���
		RPUI_KOUCIKA			= 2,	/// �ڿ��ī
		RPUI_ANCIENT_PPORU		= 3,	/// ���μ�Ʈ �Ƿ�
		RPUI_SCORPION_TYPE_R	= 4,	/// ���� �����ǿ� TYPE-R
		RPUI_ICE_HAMELING		= 5,	/// ���̽� �ϸḵ
		RPUI_DARK_HAMELING		= 6,	/// ��ũ �ϸḵ 
		RPUI_RED_HAMELING		= 7,	/// ���� �ϸḵ 
		
		// ��ȹ�� ����� �ɷ�ġ�� �ؿ����� �ݿ����� �ʱ� ���� �߰� �� ID ~
		RPUI_NASOD_MOBI_INT     = 8,    /// ����� ���� ���� ���RT
		RPUI_KOUCIKA_INT        = 9,    /// ����� �ٶ��� �ڿ��ī
		RPUI_ANCIENT_PPORU_INT  = 10,   /// ����� ���̼�Ʈ ���̵� �Ƿ�
		// ~ ��ȹ�� ����� �ɷ�ġ�� �ؿ����� �ݿ����� �ʱ� ���� �߰� �� ID

		RPUI_OBERON_CUSTOM_650	= 11,	///	�̺� Ŀ����-������650
		RPUI_ANCIENT_PPORU_NAVER = 12,	///	���̹� ä�θ� �̺�Ʈ ���̼�Ʈ �Ƿ�

		RPUI_OBERON_CUSTOM_650_BLACK_SKULL = 13,	/// �̺� Ŀ���� - �� ����
		RPUI_OBERON_CUSTOM_650_ANGEL_HEART = 14,	/// �̺� Ŀ���� - ���� ��Ʈ

		RPUI_END,
	};

	enum RIDING_PET_ENUM
	{
		RPE_MAX_PET_COUNT	= 30,
		RPE_PAGE_PER_COUNT	= 6,	// ������ ���� ��� ���� ������ �������� ���Ѵ�.
	};

	struct RidingPetTemplet
	{
		RIDING_PET_UNIT_ID	m_iRidingPetID;
		USHORT				m_usMaxStamina;
		float				m_fStaminaRecoveryRate;
	};

public:
	CXSLRidingPetManager(void);
	~CXSLRidingPetManager(void);

	// for lua
	bool	AddRidingPetTemplet_LUA();
	bool	AddRidingPetCreateItemInfo_LUA( int iItemID, USHORT usRidingPetID, short sPeriod );
	void	SetEnableStamina_LUA( float fEnableStamina ){	m_fEnableStamina = fEnableStamina;	}
	float	GetEnableStamina( void ) const{	return m_fEnableStamina;	}
	
	// function
	const RidingPetTemplet*	GetRidingPetTemplet( IN const USHORT usRidingPetID ) const;

	bool	IsExistRidingPetID( IN const USHORT usiRidingPetID ) const{	return ( m_mapRidingPetTemplet.find( static_cast<RIDING_PET_UNIT_ID>( usiRidingPetID ) ) != m_mapRidingPetTemplet.end() );	}
	void	GetRidingPetID( IN const int iItemID, OUT USHORT& usCreatePetID, OUT short& sPeriod ) const;
	
	float	GetRecoveryTimePerOne( void );
	
	// static

private:
	std::map< RIDING_PET_UNIT_ID, RidingPetTemplet >		m_mapRidingPetTemplet;			// ���̵� �� ������
	std::map< int, std::pair<RIDING_PET_UNIT_ID, short> >	m_mapRidingPetCreateItem;		// ���̵� �� ���� ������ ����Ʈ
	float													m_fEnableStamina;				// ���� ���¹̳� ��ġ�� ���� ��� ���� ���� ����
};

DefRefreshSingletonInline( CXSLRidingPetManager );
#endif	// SERV_RIDING_PET_SYSTM