#pragma once


#ifdef COMBO_TREE_UI_TEST

// ��ųƮ���� ������ �޺� ��� UI
class CX2ComboTree
{
public:
	enum COMBO_TREE_ICON
	{
		CTI_INVALID,

		// Ű���� �׸�
		CTI_Z,
		CTI_X,
		CTI_Z_CONTINUE,
		CTI_X_CONTINUE,
		CTI_LEFT,
		CTI_RIGHT,
		CTI_UP,
		CTI_DOWN,
		CTI_DASH,
		CTI_DASH_UP,
		CTI_LEFT_Z,
		CTI_LEFT_X,
		CTI_RIGHT_Z,
		CTI_RIGHT_X,
		CTI_UP_Z,
		CTI_UP_X,
		CTI_DOWN_Z,
		CTI_DOWN_X,


		// ���� �������� �ٿ�������� ǥ��
		CTI_UP_DECISION,
		CTI_DOWN_DECISION,



		// ���μ�
		CTI_LINE_RIGHT,
		CTI_LINE_UP_RIGHT,
		CTI_LINE_DOWN, 
		CTI_LINE_DOWN_RIGHT,
		CTI_LINE_DOUBLE_UP_RIGHT,
		CTI_LINE_DOUBLE_DOWN_RIGHT,
		


		// Į, �� �׸�
		CTI_ICON_JUMP,
		CTI_ICON_DOUBLE_JUMP,
		CTI_ICON_DASH,
		CTI_ICON_DASH_JUMP,
		CTI_ICON_SWORD,
		CTI_ICON_STAFF,
		CTI_ICON_FIST,
		CTI_ICON_KICK,
		CTI_ICON_GEAR,
		CTI_ICON_FIRE_BALL,
		CTI_ICON_ARROW,
		CTI_ICON_TRIPLE_ARROW,
		CTI_ICON_MAGIC_AREA,
		CTI_ICON_DARK_FIRE_BALL,
		CTI_ICON_ELECTRON_BALL,
		CTI_ICON_EXPLOSION,
		CTI_ICON_TRIANGLE,
		CTI_ICON_RUNE_AREA,
		CTI_ICON_SPARK,

		//{{ kimhc // 2010.12.20 // 2010-12-23 New Character CHUNG
		CTI_ICON_CANNON,
		CTI_ICON_CANNON_BALL,
		CTI_ICON_CANNON_BALL_CHARGE,
		//}} kimhc // 2010.12.20 //  2010-12-23 New Character CHUNG

		CTI_ICON_HANDGUN,	// kimhc // 2010.01.24 // û 1�� ����
		CTI_ICON_LANDING,
		CTI_BACKGROUND,

//#ifdef COMBO_TREE_TOOLTIP
		// Ű���� �׸�: ���ҵ� �˻��� �� - Ȱ��
		CTI_Z_BLUE,
		CTI_X_BLUE,
		CTI_Z_CONTINUE_BLUE,
		CTI_X_CONTINUE_BLUE,
		CTI_LEFT_Z_BLUE,
		CTI_LEFT_X_BLUE,
		CTI_RIGHT_Z_BLUE,
		CTI_RIGHT_X_BLUE,
		CTI_UP_Z_BLUE,
		CTI_UP_X_BLUE,
		CTI_DOWN_Z_BLUE,
		CTI_DOWN_X_BLUE,
		// Ű���� �׸�: ���ҵ� �˻��� �� - �ı�
		CTI_Z_RED,
		CTI_X_RED,
		CTI_Z_CONTINUE_RED,
		CTI_X_CONTINUE_RED,
		CTI_LEFT_Z_RED,
		CTI_LEFT_X_RED,
		CTI_RIGHT_Z_RED,
		CTI_RIGHT_X_RED,
		CTI_UP_Z_RED,
		CTI_UP_X_RED,
		CTI_DOWN_Z_RED,
		CTI_DOWN_X_RED,
//#endif COMBO_TREE_TOOLTIP

//#ifdef RAVEN_WEAPON_TAKER
		CTI_X_CONTINUE_OVERHEAT,
//#endif RAVEN_WEAPON_TAKER
//#ifdef EVE_ELECTRA
		CTI_ICON_BEAM,
		CTI_PUSH_Z,
//#endif EVE_ELECTRA
//#ifdef SERV_ARME_DIMENSION_WITCH
		CTI_ICON_BLACKHOLE,
//#endif
//#ifdef SERV_RENA_NIGHT_WATCHER
		CTI_ICON_ACCEL_DASH,
		CTI_ICON_EXPLOSIVE_ARROW,
//#endif SERV_RENA_NIGHT_WATCHER
//#ifdef SERV_RAVEN_VETERAN_COMMANDER
		CTI_PUSH_Z_OVERHEAT,
//#endif SERV_RAVEN_VETERAN_COMMANDER
		CTI_ICON_SIEGE,
		CTI_ICON_SIEGE_SHELLING,
//#ifdef ARA_CHARACTER_BASE
		CTI_ICON_SPEAR,
		CTI_PUSH_X,
//#endif

//#ifdef BALANCE_ELEMENTAL_MASTER_20130117
		CTI_LINE_DOWN_DOUBLE_RIGHT,			/// ���μ� �߰�
		CTI_LINE_THREEWAY_INTERSECTION,		/// ���μ� �߰�
		CTI_ICON_HURRICANE,					/// ������Ż ������ �㸮����
//#endif BALANCE_ELEMENTAL_MASTER_20130117

#ifdef ARA_CHANGE_CLASS_FIRST
		CTI_X_DRAGON_TOOTH,			/// ��� 1��
#endif

#ifdef SERV_ARA_CHANGE_CLASS_SECOND
		CTI_Z_WOLF_TOOTH,			/// ���� 1��
#endif //SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER
		CTI_ICON_DYNAMO_Z			= 92,
		CTI_ICON_DYNAMO_X			= 93,
		CTI_ICON_DYNAMO_ROLLING		= 94,
		CTI_ICON_DYNAMO_TELEPORT	= 95,
		CTI_ICON_FIST_NA			= 96,
		CTI_ICON_KICK_NA			= 97,
		CTI_RIGHT_Z_PURPLE			= 98,
		CTI_RIGHT_X_PURPLE			= 99,
		CTI_RIGHT_Z_PURPLE_CONTINUE	= 100,
		CTI_RIGHT_X_PURPLE_CHARGE	= 101,
		CTI_ICON_NA					= 102,
#endif //SERV_9TH_NEW_CHARACTER
	};



	enum COMBO_CATEGORY
	{
		CC_INVALID,
		CC_NORMAL,
		CC_JUMP,
		CC_DASH,
		CC_DASH_JUMP,
//#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		CC_SPECIAL,			/// Ư�� �޺� Ʈ��
//#endif //SERV_ADD_LUNATIC_PSYKER
	};




	struct ComboIcon
	{
		D3DXVECTOR2 m_vLeftTop;
		COMBO_TREE_ICON	m_eComboIcon;
		int m_iLayer;

		ComboIcon()
			: m_vLeftTop( 0, 0 )
			, m_eComboIcon( CTI_INVALID )
			, m_iLayer( 0 )
		{
		}

		ComboIcon( float x, float y, COMBO_TREE_ICON eComboIcon, int iLayer )
			: m_vLeftTop( x, y )
			, m_eComboIcon( eComboIcon )
			, m_iLayer( iLayer )
		{
		}


		bool operator < (const ComboIcon& rhs ) const
		{
			if( m_iLayer < rhs.m_iLayer )
				return true;

			return false;
		}
	};


	struct ComboSet
	{
		std::vector<ComboIcon> m_vecNormalCombo;
		std::vector<ComboIcon> m_vecJumpCombo;
		std::vector<ComboIcon> m_vecDashCombo;
		std::vector<ComboIcon> m_vecDashJumpCombo;
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		std::vector<ComboIcon> m_vecSpecialCombo;		/// Ư�� �޺� Ʈ��
#endif //SERV_ADD_LUNATIC_PSYKER

		void Init()
		{
			m_vecNormalCombo.resize(0);
			m_vecJumpCombo.resize(0);
			m_vecDashCombo.resize(0);
			m_vecDashJumpCombo.resize(0);
#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
			m_vecSpecialCombo.resize(0);
#endif //SERV_ADD_LUNATIC_PSYKER
		}
	};

	typedef std::map< CX2Unit::UNIT_CLASS, ComboSet > UnitComboMap;


private:
	struct UnitClassName
	{
		CX2Unit::UNIT_CLASS m_eUnitClass;
		//std::wstring m_Name;
        std::string m_Name;

		UnitClassName( CX2Unit::UNIT_CLASS eUnitClass, const std::string& strName )
		{
			m_eUnitClass = eUnitClass;
			m_Name = strName;
		}
	};




public:
	CX2ComboTree() {};
	~CX2ComboTree() {};

	const ComboSet* GetUnitComboSet( CX2Unit::UNIT_CLASS eUnitClass );

	int GetComboIconLayer( COMBO_TREE_ICON eIcon );

	void ParseComboSet( KLuaManager &luaManager, std::vector<ComboIcon>& vecComboSet );

	bool OpenScriptFile( const std::wstring& filename );




private:
	UnitComboMap m_mapUnitCombo;

};

#endif COMBO_TREE_UI_TEST