#pragma once

/** @class : CBattleFieldPortalMovingInfo
@brief : �ʵ忡�� ���� �Ǵ� ��Ż�� �̵� ���� Ŭ����
@date  : 2011/10/25
*/

class CBattleFieldPortalMovingInfo
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:
	// ������: �⺻Type�� Initialize�� �־��ֽð�, ����� ���� Ÿ���� ��� �ʱ�ȭ ����Ʈ���� �ʱ�ȭ ���ּ���
	CBattleFieldPortalMovingInfo() 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    { _OnCreate(); 
    }
	
	void Initialize() {
		_OnCreate();	// �⺻Type �ʱ�ȭ
		// ���� ���ʹ� �⺻Type�� �ƴ� �� �ʱ�ȭ	
	}

	bool ParsingScriptFile( IN KLuaManager& luaManager_ );	// CBattleFieldPortalMoveInfo�� �ʿ��� ������ �о���δ�

	UINT GetLineNumber() const { return m_uiLineNumber; }
	void SetLineNumber( const UINT uiLineNumber_ ) { m_uiLineNumber = uiLineNumber_; }

	UINT GetPlaceIdToMove() const { return m_uiPlaceIdToMove; }
	void SetPlaceIdToMove( const UINT uiPlaceIdToMove_ ) { m_uiPlaceIdToMove = uiPlaceIdToMove_; }

	USHORT GetPositionIndexToMove() const { return m_usPositionIndexToMove; }
	void SetPositionIndexToMove( const USHORT usPositionIndexToMove_ ) { m_usPositionIndexToMove = usPositionIndexToMove_; }

	USHORT GetPortalMoveType() const { return m_usPortalMoveType; }
	void SetPortalMoveType( const USHORT usPortalMoveType_ ) { m_usPortalMoveType = usPortalMoveType_; }


#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:	// private �Լ��� �տ��� _�� �ٿ� �־���

	/// �����ڿ��� �⺻Type�� �������� �ʱ�ȭ ��Ű�� ���� �뵵�� ���
	void _OnCreate() {
		m_uiLineNumber				= 0;
		m_uiPlaceIdToMove			= 0;
		m_usPositionIndexToMove		= 0;
		m_usPortalMoveType			= 0;
#ifdef QUEST_GUIDE
		m_uiFieldID					= 0;
#endif //QUEST_GUIDE
	}

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	UINT	m_uiLineNumber;				/// �ʵ��� ���θʿ��� �� ��Ż�� ��ġ�� �ʵ� ���� ���� ��ȣ �� �ε���
	UINT	m_uiPlaceIdToMove;			/// �̵��� ����� ID (VillageMapID or BattleFieldID)

	USHORT	m_usPositionIndexToMove;	/// �̵��� ����� Postion Index�� ���� ������ ���� DLG_Map_Village.lua�� �ִ� StartPosId, �ʵ��� ��쿡�� StartPos(?) �� �ɵ�

	USHORT	m_usPortalMoveType;			/// ��Ż�� �̵� Ÿ�� (CX2BattleFieldManager::PORTAL_MOVE_TYPE, ��Ż->����, ��Ż->��Ż)
#ifdef QUEST_GUIDE
	UINT	m_uiFieldID;				/// BATTLE_FIELD_PORTAL_MOVING_INFO�� ������ �ִ� �ʵ��� ID
#endif //QUEST_GUIDE
};

IMPLEMENT_INTRUSIVE_PTR( CBattleFieldPortalMovingInfo );

/** @class : CBattleFieldRiskInfo
@brief : �ʵ��� ���赵 ���� Ŭ������ �ʵ��� ���赵 ��ġ�� ���� � ���Ͱ� ���������� ���� ������ ������ ����
@date  : 2011/10/25
*/
class CBattleFieldRiskInfo
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
public:
	// true, false �Ӽ��� ������ Ÿ���� �÷���, std::bitset���� ����
	enum FLAG_RISK_INFO {
		FRI_SHOW_BOSS_NAME = 0,				/// ���� �̸��� ���� �� ������ ����
		FRI_DEVIDE_BOSS_HP_GAUGE_TO_LINES,	/// ������ HP �������� ���� �ٷ� ���� ������ ����
		FRI_FLAG_MAX,						/// �� ENUM�� MAX ��(bitset�� ũ��� ���)
	};

	// ������: �⺻Type�� Initialize�� �־��ֽð�, ����� ���� Ÿ���� ��� �ʱ�ȭ ����Ʈ���� �ʱ�ȭ ���ּ���
	CBattleFieldRiskInfo() 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    { _OnCreate(); }

	bool ParsingScriptFile( IN KLuaManager& luaManager_ );	// CBattleFieldRiskInfo�� �ʿ��� ������ �о���δ�

	void Initialize() {
		_OnCreate();	// �⺻Type �ʱ�ȭ
		// ���� ���ʹ� �⺻Type�� �ƴ� �� �ʱ�ȭ	
		m_bitsetFlagRiskInfo.reset();
	}

	UINT GetMonsterIdToBeSpawned() const { return m_uiMonsterIdToBeSpawned; }
	void SetMonsterIdToBeSpawned( const UINT uiMonsterIdToBeSpawned_ ) { m_uiMonsterIdToBeSpawned = uiMonsterIdToBeSpawned_; }

	USHORT GetRiskValue() const { return m_usRiskValue; }
	void SetRiskValue( const USHORT usRiskValue_ ) { m_usRiskValue = usRiskValue_; }

	bool GetBitSetFlag( const FLAG_RISK_INFO eFlagRiskInfo_ ) { m_bitsetFlagRiskInfo.test( eFlagRiskInfo_ ); }
	void SetBitSetFlag( const FLAG_RISK_INFO eFlagRiskInfo_, const bool bFlag_ ) {
		m_bitsetFlagRiskInfo.set( eFlagRiskInfo_, bFlag_ );
	}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:

	/// �����ڿ��� �⺻Type�� �������� �ʱ�ȭ ��Ű�� ���� �뵵�� ���
	void _OnCreate() {
		m_uiMonsterIdToBeSpawned	= 0;
		m_usRiskValue				= 0;
	}

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	UINT			m_uiMonsterIdToBeSpawned;			/// ���赵 ��ġ�� ���� ������ ����
	USHORT			m_usRiskValue;						/// ���赵 ��ġ
	
	std::bitset<FRI_FLAG_MAX>	m_bitsetFlagRiskInfo;	/// ���赵 �������� ���̴� Flag�� BitSet
};

IMPLEMENT_INTRUSIVE_PTR( CBattleFieldRiskInfo );

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS

class CBattleFieldMiddleBossInfo		// �ʵ� �߰� ������ ���� ������ ��� �ִ� Ŭ����, BattleFieldData.lua ���� �Ľ�
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{
	public :
		CBattleFieldMiddleBossInfo ()  
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        { _OnCreate(); }

		UINT GetMonsterSpawnID() const { return m_uiMonsterSpawnID; }
		void SetMonsterSpawnID(UINT val) { m_uiMonsterSpawnID = val; }


		USHORT GetRiskValue() const { return m_usRiskValue; }
		void SetRiskValue(USHORT val) { m_usRiskValue = val; }

		bool GetIsSplitBossGage() const { return m_bIsSplitBossGage; }
		void SetIsSplitBossGage(bool val) { m_bIsSplitBossGage = val; }

		bool GetIsShowBossGage() const { return m_bIsShowBossGage; }
		void SetIsShowBossGage(bool val) { m_bIsShowBossGage = val; }

		USHORT GetSpawnRate() const { return m_usSpawnRate; }
		void SetSpawnRate(USHORT val) { m_usSpawnRate = val; }

		USHORT GetMonsterSpawnMany() const { return m_usMonsterSpawnMany; }
		void SetMonsterSpawnMany(USHORT val) { m_usMonsterSpawnMany = val; }

		wstring GetMainMonsterName() const { return m_wstrMainMonsterName; }
		void SetMainMonsterName(wstring val) { m_wstrMainMonsterName = val; }

		bool ParsingScriptFile ( IN KLuaManager& luaManager_ );	

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        void    AddRef()    {   ++m_uRefCount; }
        void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	private : 

		void _OnCreate()
		{
			m_uiMonsterSpawnID = 0;
			m_usSpawnRate = 0;
			m_usRiskValue = 0;
			m_bIsShowBossGage = 0;
			m_bIsSplitBossGage = 0;
			m_usMonsterSpawnMany = 0;
		}

		void Initialize ()
		{
			_OnCreate();
			m_wstrMainMonsterName.clear();	
			m_iBossGroupStringIndex = 0;
			m_vecMonsterSpawnGroupID.clear();
		}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        unsigned            m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		USHORT				m_usMonsterSpawnMany;
		UINT				m_uiMonsterSpawnID;		
		std::vector<UINT>	m_vecMonsterSpawnGroupID;		
		USHORT				m_usSpawnRate;		
		USHORT				m_usRiskValue;
		bool				m_bIsShowBossGage;
		bool				m_bIsSplitBossGage;
		int					m_iBossGroupStringIndex;	// ���� �׷��� ��Ÿ���� String Index, Parsing �� ����
		wstring				m_wstrMainMonsterName;		// ���� �׷� �̸�
};

IMPLEMENT_INTRUSIVE_PTR( CBattleFieldMiddleBossInfo );

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS



/** @class : CBattleFieldData
@brief : �ʵ忡 ���� ������ ��� ���� Ŭ����
@date  : 2011/10/25
*/

class CBattleFieldData
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
{

public:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	typedef boost::intrusive_ptr<CBattleFieldPortalMovingInfo> CBattleFieldPortalMovingInfoPtr;
	typedef boost::intrusive_ptr<CBattleFieldRiskInfo> CBattleFieldRiskInfoPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	typedef boost::shared_ptr<CBattleFieldPortalMovingInfo> CBattleFieldPortalMovingInfoPtr;
	typedef boost::shared_ptr<CBattleFieldRiskInfo> CBattleFieldRiskInfoPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	typedef boost::shared_ptr<CBattleFieldMiddleBossInfo> CBattleFieldMiddleBossInfoPtr;
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

	CBattleFieldData() 
		: m_wstrBattleFieldName(), m_wstrBattleFieldTextureName(), m_uiReturnVillageId( 0 ) 
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {
		_OnCreate();
	}

	void Initialize() {
		_OnCreate();// �⺻Type �ʱ�ȭ
		// ���� ���ʹ� �⺻Type�� �ƴ� �� �ʱ�ȭ	
		m_wstrBattleFieldName.clear();
		m_wstrBattleFieldTextureName.clear();
		m_vecBattleFieldPortalMovingInfoPtr.resize(0);
		m_vecBattleFieldRiskInfoPtr.resize(0);
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		m_vecBattleFieldMiddleBossInfoPtr.resize(0);	
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS
	}

	bool ParsingScriptFile( IN KLuaManager& luaManager_ );	// CBattleFieldData�� �ʿ��� ������ �о���δ�

	UINT GetBattleFieldId() const { return m_uiBattleFieldId; }
	void SetBattleFieldId( const UINT uiBattleFieldId_ ) { m_uiBattleFieldId = uiBattleFieldId_; }

	UINT GetWorldId() const { return m_uiWorldId; }
	void SetWorldId( const UINT uiWorldId_ ) { m_uiWorldId = uiWorldId_; }

	UINT GetStandardMonsterLevel() const { return m_uiStandardMonsterLevel; }
	void SetStandardMonsterLevel( const UINT uiStandardMonsterLevel_ ) { m_uiStandardMonsterLevel = uiStandardMonsterLevel_; }

	USHORT GetMaxNumberOfMonsterInThisBattleField() const { 
		return m_usMaxNumberOfMonsterInThisBattleField; }
	void SetMaxNumberOfMonsterInThisBattleField( USHORT usMaxNumberOfMonsterInThisBattleField_ ) {
		m_usMaxNumberOfMonsterInThisBattleField = usMaxNumberOfMonsterInThisBattleField_; }

	const WCHAR* GetBattleFieldName() const { return m_wstrBattleFieldName.c_str(); }
	void SetBattleFieldName( const WCHAR* wszBattleFieldName_ ) { m_wstrBattleFieldName = wszBattleFieldName_; }

	const WCHAR* GetBattleFieldTextureName() const { 
		return ( m_wstrBattleFieldName.empty() ? NULL : m_wstrBattleFieldTextureName.c_str() ); }
	void SetBattleFieldTextureName( const WCHAR* wszBattleFieldTextureName_ ) {
		m_wstrBattleFieldTextureName = wszBattleFieldTextureName_; }

	const WCHAR* GetBattleFieldTextureKey() const { 
		return ( m_wstrBattleFieldTextureKey.empty() ? NULL : m_wstrBattleFieldTextureKey.c_str() ); }
	void SetBattleFieldTextureKey( const WCHAR* wszBattleFieldTextureKey_ ) {
		m_wstrBattleFieldTextureKey = wszBattleFieldTextureKey_; }

	UINT GetReturnVillageId() const { return m_uiReturnVillageId; }
	void SetReturnVillageId( const UINT uiReturnVillageId_ ) { m_uiReturnVillageId = uiReturnVillageId_; }

	CBattleFieldPortalMovingInfoPtr GetBattleFieldPortalMovingInfoPtrByLineMapIndex( const UINT uiLineMapIndex_ );
#ifdef QUEST_GUIDE
	bool GetBattleFieldPortalMovingInfo( OUT map<UINT, UINT>& mapBattleFieldMovingInfo_);
#endif //QUEST_GUIDE

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	wstring GetBattleFieldMiddleBossInfoGroupName ( int iSpawnGroup_ )
	{
		std::vector<CBattleFieldMiddleBossInfoPtr>::iterator it = m_vecBattleFieldMiddleBossInfoPtr.begin();
		for ( ; it != m_vecBattleFieldMiddleBossInfoPtr.end(); ++it )
		{
			if ( (*it) != NULL )  
			{
				if ( (*it)->GetMonsterSpawnID() == iSpawnGroup_ )		// ��ȯ�� �׷� ��� �˻� ���� �ش� �׷��� �̸��� ���ٸ�
				{
					return (*it)->GetMainMonsterName();					// �ش� �׷� ���� ��ȯ�Ѵ�.
				}
			}
		}		
		return L"NoData";
	}
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	bool GetIsBossRaidField() const { return m_bIsBossRaidField; }
	const USHORT GetRaidFieldPortalLineIndex() const { return m_usRaidFieldPortalPositionIndex; }
#endif // FIELD_BOSS_RAID

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

private:

	void _OnCreate() {
		m_uiBattleFieldId = 0;
		m_uiWorldId = 0;
		m_uiStandardMonsterLevel = 0;
		m_usMaxNumberOfMonsterInThisBattleField = 0;
#ifdef FIELD_BOSS_RAID
		m_bIsBossRaidField = false;
		m_usRaidFieldPortalPositionIndex = 0;
#endif // FIELD_BOSS_RAID
	}

	// CBattleFieldPortalMovingInfo�� shared_ptr ����
	CBattleFieldPortalMovingInfoPtr CreateBattleFieldPortalMovingInfoPtr() { 
		return CBattleFieldPortalMovingInfoPtr( new CBattleFieldPortalMovingInfo() ); }

	// CBattleFieldRiskInfo�� shared_ptr ����
	CBattleFieldRiskInfoPtr CreateBattleFieldRiskInfoPtr() { 
		return CBattleFieldRiskInfoPtr( new CBattleFieldRiskInfo() ); }

	bool ParsingBattleFieldMovingInfo( IN KLuaManager& luaManager_ );	// BattleFieldPortalMovingInfo �Ľ�
	bool ParsingBattleFieldRiskInfo( IN KLuaManager& luaManager_ );	// BattleFieldRiskInfo �Ľ�

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CBattleFieldMiddleBossInfoPtr CreateBattleFieldMiddleBossInfoPtr() 
	{ 
		return CBattleFieldMiddleBossInfoPtr( new CBattleFieldMiddleBossInfo() ); 
	}

	void ParsingBattleFieldMiddleBossInfo ( IN KLuaManager& luaManager_ );	// BATTLE_FIELD_MIDDLE_BOSS_INFO �Ľ�

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	UINT		m_uiBattleFieldId;				/// ���� �ʵ��� ID, ������ UINT�� ������ BattleField_ID ���� ����
	UINT		m_uiWorldId;				/// ���� �ʵ尡 ����ϴ� CX2World::WORLD_ID enum��
	UINT		m_uiStandardMonsterLevel;	/// ���� �ʵ忡�� ������ ���͵��� ���� ���� (�ϴ� ������ �ֱ�� ������.. Ŭ���̾�Ʈ���� ����ϴ����� �ΰ� ����)
	UINT		m_uiReturnVillageId;		/// ������ �� �ʵ忡�� �װ� ��Ȱ���� ���� ��� ���ư��Ե� ����ID
	
	USHORT		m_usMaxNumberOfMonsterInThisBattleField;	/// ���� �ʵ忡�� �ִ�� ���� �� �ִ� ���� ��
	
	// �̸��� �ʿ����� ������ ����
	wstring		m_wstrBattleFieldName;			/// �ʵ� �� (�������� �� �ٸ��� ������ wstring ��ü ���)
	
	wstring		m_wstrBattleFieldTextureName;		/// �ʵ� ���� ��Ÿ���� �ؽ��� ��
	wstring		m_wstrBattleFieldTextureKey;		/// �ʵ� ���� ��Ÿ���� �ؽ��� Ű
	
	std::vector<CBattleFieldPortalMovingInfoPtr> m_vecBattleFieldPortalMovingInfoPtr;	/// BattleFieldPortalMovingInfo�� shared_ptr�� ���� vector
	std::vector<CBattleFieldRiskInfoPtr> m_vecBattleFieldRiskInfoPtr;	/// BattleFieldRiskInfo�� shared_ptr�� ���� vector

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	std::vector<CBattleFieldMiddleBossInfoPtr> m_vecBattleFieldMiddleBossInfoPtr;	/// BattleFieldMiddleBossInfo�� shared_ptr�� ���� vector
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	bool		m_bIsBossRaidField;
	USHORT		m_usRaidFieldPortalPositionIndex;	// ���� ���̵� �ʵ�� �̵��ϴ� ��Ż ������ �ε���
#endif // FIELD_BOSS_RAID

};

IMPLEMENT_INTRUSIVE_PTR( CBattleFieldData );


/** @class : CX2BattleFieldManager
@brief : ���ҵ� �ʵ��� Data�� ��Ʒ� ���� �Ľ��ϰ�, �ش� BattleFieldData�� �����ϴ� Ŭ����
@date  : 2011/10/25
*/
class CX2BattleFieldManager {

public:

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    typedef boost::intrusive_ptr<CBattleFieldData> CBattleFieldDataPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	typedef boost::shared_ptr<CBattleFieldData> CBattleFieldDataPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	enum PORTAL_MOVE_TYPE {
		PMT_INVALID = 0,
		PMT_MOVE_TO_VILLAGE,				/// �ʵ� -> ������ �̵��ϴ� ��Ż
		PMT_MOVE_TO_BATTLE_FIELD,			/// �ʵ� -> �ʵ�� �̵��ϴ� ��Ż
		PMT_MOVE_TO_RAID_FIELD,				/// �ʵ� -> ���̵� �ʵ�� �̵��ϴ� ��Ż
		PMT_END,
	};

public:

	struct BATTLE_FIELD_POSITION_INFO {
		UINT		m_uiBattleFieldIdWhereIam;		/// ���� �ڽ��� �ִ� ���� �ʵ� ID (�ʵ� �̵� �� ���� �Ѵ�.)
		USHORT		m_usBattleFieldPositionIndexWhereIShouldBe;		/// �ʵ尣 �Ǵ� �ٸ� �������� �ʵ�� �̵� �ÿ� �ڽ��� �־�� �� �ʵ峻�� Position �ε���
		USHORT		m_usBattleFieldPostionValue;	/// PositionIndex�� ������ ���θʵ����Ϳ��� �� % ��ġ�� �ش��ϴ����� ���� ����
		bool		m_bStartPosition;	/// ���θ��� Start���� �ΰ�? 
		// (m_bStartPosition �� true�̸� m_usBattleFieldPostionValue�� ���ǹ��ϰ�, m_usBattleFieldPositionIndexWhereIShould�� LineMapData�� Index�� �ƴ϶� StartPos Index��)

		BATTLE_FIELD_POSITION_INFO() : m_uiBattleFieldIdWhereIam( 0 ), m_usBattleFieldPositionIndexWhereIShouldBe( 0 ),
			m_usBattleFieldPostionValue( 0 ), m_bStartPosition( false )
		{}
	};

	CX2BattleFieldManager() : m_BattleFieldPositionInfo()
	{}

	void OpenScriptFile( const wchar_t* wszScriptFileName_ );
	void AddBattleFieldData_LUA();
	void SetMonsterRespawnFactorByUserCount_LUA();		// dummy
	void SetMonsterRespawnTimeMinMax_LUA();				// dummy
	void SetBattleFieldFactor_LUA();					// dummy

	void ReOpenScriptFile( const wchar_t* wszScriptFileName_ );

	// CBattleFieldData�� shared_ptr ����
	CBattleFieldDataPtr CreateBattleFieldDataPtr() { return CBattleFieldDataPtr( new CBattleFieldData() ); }	

	// ���� �ڽ��� �ִ� ���� �ʵ� ID (�ʵ� �̵� �� ���� �Ѵ�.)
	UINT GetBattleFieldIdWhereIam() const { return m_BattleFieldPositionInfo.m_uiBattleFieldIdWhereIam; }
	void SetBattleFieldIdWhereIam( const UINT uiBattleFieldIdWhereIam_  ) { m_BattleFieldPositionInfo.m_uiBattleFieldIdWhereIam = uiBattleFieldIdWhereIam_; }

	// �ʵ尣 �Ǵ� �ٸ� �������� �ʵ�� �̵� �ÿ� �ڽ��� �־�� �� �ʵ峻�� Position �ε���
	USHORT GetBattleFieldPositionIndexWhereIShouldBe() const { return m_BattleFieldPositionInfo.m_usBattleFieldPositionIndexWhereIShouldBe; }
	void SetBattleFieldPositionIndexWhereIShouldBe( const USHORT usBattleFieldPositionIndexWhereIShouldBe_ ) 
	{ 
		m_BattleFieldPositionInfo.m_usBattleFieldPositionIndexWhereIShouldBe = usBattleFieldPositionIndexWhereIShouldBe_; 
	}

	USHORT GetBattleFieldPositionValue() const { return m_BattleFieldPositionInfo.m_usBattleFieldPostionValue; }
	void SetBattleFieldPositionValue( const USHORT usBattleFieldPositionValue_ ) { m_BattleFieldPositionInfo.m_usBattleFieldPostionValue = usBattleFieldPositionValue_; }

	bool GetNowBattleFieldPositionInfoStartPosition() const { return m_BattleFieldPositionInfo.m_bStartPosition; }
	void SetNowBattleFieldPositionInfoStartPosition( const bool bNowFieldPositionInfoStartPosition_ ) { m_BattleFieldPositionInfo.m_bStartPosition = bNowFieldPositionInfoStartPosition_; }

	UINT GetWorldIdByBattleFieldId( const UINT uiBattleFieldId_ ) const;
	CBattleFieldData::CBattleFieldPortalMovingInfoPtr GetPortalMovingInfoByLineMapIndexInNowBattleField( const int iLineMapIndex_ ) const;

	UINT GetReturnVillageId( UINT uiBattleFieldId_ = -1 ) const;

	bool GetBattleFieldTextureNameAndKey( IN const UINT uiBattleFieldId_, OUT const WCHAR** pTextureName_, OUT const WCHAR** pKeyName_ );

#ifdef REFORM_QUEST
	const WCHAR* GetBattleFieldNameByBattleFieldId( const UINT uiBattleFieldId_ ) const;
	bool		 GetBattleFieldPortalMovingInfo( OUT map<UINT, UINT>& mapBattleFieldMovingInfo_);
#endif //REFORM_QUEST

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	wstring GetBattleFieldBossDataGroupName ( UINT uiBattleFieldId_, int iBattleFieldBossGroupId_ );	// Field ID �� Boss Group ID �� �޾Ƽ� ���� �׷� ���� ��ȯ��

#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef FIELD_BOSS_RAID
	bool		GetIsBossRaidFieldByFieldID( const UINT uiBattleFieldID_ ) const;
	bool		GetIsBossRaidCurrentField() const;
	const USHORT GetRaidFieldPortalLineByFieldID( const UINT uiBattleFieldID_ ) const;
#endif // FIELD_BOSS_RAID

private:
	typedef std::map<UINT, CBattleFieldDataPtr> BattleFieldDataPtrMap;
	typedef std::pair<UINT, CBattleFieldDataPtr> BattleFieldDataPtrPair;

	BattleFieldDataPtrMap	m_mapBattleFieldDataPtr;	/// key: BattleField_ID, value: CBattleFieldDataPtr

	BATTLE_FIELD_POSITION_INFO	m_BattleFieldPositionInfo;
};