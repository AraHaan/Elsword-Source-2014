/**@file : X2BuffBehaviorTemplet.h
   @breif : ������ �ൿ�� �����ϴ� Templet ����
*/

#pragma once

class CX2BuffTemplet;
class CX2BuffBehaviorTemplet;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::intrusive_ptr<CX2BuffBehaviorTemplet> CX2BuffBehaviorTempletPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::shared_ptr<CX2BuffBehaviorTemplet> CX2BuffBehaviorTempletPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

/** @class : CX2BuffBehaviorTemplet
	@brief : ������ �ൿ ����� �����ϴ� Ŭ����
	@date : 2012/7/16/
*/
class CX2BuffBehaviorTemplet
{
public:
	static bool ParsingCombinationBehavior( KLuaManager& luaManager_, OUT vector<CX2BuffBehaviorTempletPtr>& vecBuffBehaviorTempletPtr_ );
	
	CX2BuffBehaviorTemplet( const CX2BuffBehaviorTemplet& rhs_ ) : m_eType( rhs_.m_eType ), m_bStart( rhs_.m_bStart )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	{}
	virtual ~CX2BuffBehaviorTemplet() {}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ ) {}
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ ) {}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ ) = NULL;
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const = NULL;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const = NULL;
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ ) {}
	
	bool SetFactorFromPacketTemplateMothod( const KBuffFactor& kBuffFactor_, CX2GameUnit* pGameUnit_ );
	bool DidStart() const { return m_bStart; }
	void SetStart(bool val) { m_bStart = val; }

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	// �ش� BBT Ÿ���� �޾ƿ;� �� ��찡 �־ �߰�, GetType �� ����
	BUFF_BEHAVIOR_TYPE GetBuffBehaviorType() const { return m_eType; }
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    void    AddRef()    {   ++m_uRefCount; }
    void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR


protected:
	CX2BuffBehaviorTemplet() : m_eType( BBT_NONE ), m_bStart( false )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR    
    {}
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    CX2BuffBehaviorTemplet& operator = ( const CX2BuffBehaviorTemplet& );
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

	BUFF_BEHAVIOR_TYPE GetType() const { return m_eType; }
	void SetType(BUFF_BEHAVIOR_TYPE val) { m_eType = val; }

	bool		ParsingBehaviorTemplateMethod( KLuaManager& luaManager_, const char* pszTableName_ );
	virtual bool ParsingBehavior( KLuaManager& luaManager_ ) { return true; }
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ ) {};

private:
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
    unsigned                                        m_uRefCount;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
	BUFF_BEHAVIOR_TYPE		m_eType;
	bool					m_bStart;
};

IMPLEMENT_INTRUSIVE_PTR( CX2BuffBehaviorTemplet );


/** @class : CX2BuffChangeNowHpPerSecondBehaviorTemplet
	@brief : �ʴ� HP�� ���� �Ǵ� �ൿ�� ���� �ϴ� Ŭ����
	@date : 2012/7/16/
*/
class CX2BuffChangeNowHpPerSecondBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,							/// �ʴ� HP�� ��ȭ�Ǵ� Ÿ��
		FO_MULTIPLIER_OR_VALUE,					/// ���� Ȥ�� ��ġ
		FO_PERCENT = FO_MULTIPLIER_OR_VALUE,	/// ��ġŸ���� �ƴ� ��쿡�� MULTIPLIER�ڸ��� %
		FO_RELATION_TYPE,						/// ����Ÿ��
		FO_ATTACK_VALUE,						/// ���ݷ�
		FO_END									/// enum ����
	};

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	enum SKILL_LEVEL
	{
		SL_VALUE = 30,
	};
#endif //UPGRADE_SKILL_SYSTEM_2013

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeNowHpPerSecondBehaviorTemplet ); }


#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffChangeNowHpPerSecondBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_fResultValue( 0.0f ),	m_delegateChangeHpByTypeFunc(), 
		m_delegateGetValueOrPercentPerSecondFunc(),
		m_eChangeType( BCT_SWAP_VALUE ), m_bDrawText( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	void SelectDelegate();
	void ChangeHpByValue( CX2GameUnit* pGameUnit_ );
	void ChangeHpByPercent( CX2GameUnit* pGameUnit_ );

	float GetFixValuePerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const;
	float GetValuePerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const;
	float GetPercentPerSecondByPercent( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const;
	void DrawText( CX2GameUnit* pGameUnit_, const float fDeltaValue_ );

private:
	typedef srutil::delegate1<void, CX2GameUnit*> DelegateChangeHpByType;
	typedef srutil::delegate2<float, const KBuffBehaviorFactor*, const float> DelegateGetValueOrPercentPerSecond;

	CKTDXCheckElapsedTime		m_CheckElapsedTimeForInterval;	/// �� �ʸ��� HP�� �����ؾ��ϴ����� �Ǵ��ϱ� ���� ���
	float						m_fResultValue;					/// �ʴ� ����� ��ġ �Ǵ� %(��ø�� ��� ����� �� ���� ������)
	DelegateChangeHpByType		m_delegateChangeHpByTypeFunc;	/// �ʴ� ���� ���� ����Ǵ��� %�� ���� ����Ǵ����� ���� �ٸ� ��� �Լ� ����
	DelegateGetValueOrPercentPerSecond m_delegateGetValueOrPercentPerSecondFunc;	/// �ʴ� ����� ��ġ �Ǵ� %�� ��� �Լ� ��������Ʈ
	BUFF_CHANGE_TYPE			m_eChangeType;				/// �ʴ� ��ȭ�Ǵ� ���� % ���� ������ ����
	bool						m_bDrawText;				/// ��ȭ�Ǵ� ���� ǥ���� ���ΰ�
};

/** @class : CX2BuffChangeNowMpPerSecondBehaviorTemplet
	@brief : �ʴ� Mp�� ���� �Ǵ� �ൿ�� ���� �ϴ� Ŭ����
	@date : 2012/7/16/
*/
class CX2BuffChangeNowMpPerSecondBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// �ʴ� Mp�� ��ȭ�Ǵ� Ÿ��
		FO_VALUE_OR_PERCENT,		/// ��ġ or %
		FO_END						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeNowMpPerSecondBehaviorTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );

protected:
	CX2BuffChangeNowMpPerSecondBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_CheckElapsedTimeForInterval( 0.0f ),
		m_fResultValue( 0.0f ), m_eChangeType( BCT_SWAP_VALUE ), m_bDrawText( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	void SelectDelegate();
	void ChangeMpByValue( CX2GameUnit* pGameUnit_ );
	void ChangeMpByPercent( CX2GameUnit* pGameUnit_ );

	float GetValueOrPercentPerSecond( const KBuffBehaviorFactor* pBehaviorFactor_, const float fRegist_ ) const;
	void DrawText( CX2GameUnit* pGameUnit_, const float fDeltaValue_ );

private:
	typedef srutil::delegate1<void, CX2GameUnit*> DelegateChangeMpByType;
	typedef srutil::delegate1<float, const float> DelegateGetValueOrPercentPerSecond;

	CKTDXCheckElapsedTime		m_CheckElapsedTimeForInterval;	/// �� �ʸ��� Mp�� �����ؾ��ϴ����� �Ǵ��ϱ� ���� ���
	float						m_fResultValue;					/// �ʴ� ����� ��ġ �Ǵ� %(��ø�� ��� ����� �� ���� ������)
	DelegateChangeMpByType		m_delegateChangeMpByTypeFunc;	/// �ʴ� ���� ���� ����Ǵ��� %�� ���� ����Ǵ����� ���� �ٸ� ��� �Լ� ����
	BUFF_CHANGE_TYPE			m_eChangeType;				/// �ʴ� ��ȭ�Ǵ� ���� % ���� ������ ����
	bool						m_bDrawText;				/// ��ȭ�Ǵ� ���� ǥ���� ���ΰ�
};

/** @class : CX2BuffReverseLeftRightBehaviorTemplet
	@brief : ���� �ð����� �¿츦 ���� ��Ű�� �ൿ Ŭ����
	@date : 2012/7/21/
*/
class CX2BuffReverseLeftRightBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffReverseLeftRightBehaviorTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffReverseLeftRightBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_CheckElapsedTimeForInterval( 0.0f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

	/// ��������ڴ� �������� ����(����� KBuffIdentity�� ���� ���� �� ���� ����)
private:
	CKTDXCheckElapsedTime		m_CheckElapsedTimeForInterval;	/// �� �ʸ��� �¿츦 ���� ���Ѿ� �ϴ���
};

/** @class : CX2BuffAttackImpossibleBehaviorTemplet
	@brief : �ش� ������ �ɷ��ִ� ���ȿ��� ����(�Ϲ�+��ų) �Ұ� �ൿ
	@date : 2012/7/22/
*/
class CX2BuffIdentityBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffIdentityBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffIdentityBehaviorTemplet() : CX2BuffBehaviorTemplet()
	{}
};

/** @class : CX2BuffChangeStatBehaviorTemplet
	@brief : �ش� ������ �ɷ��ִ� ���ȿ� ���� ����
	@date : 2012/7/23/
*/
class CX2BuffChangeStatBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// ��ȭ�Ǵ� Ÿ��
		FO_VALUE,					/// ��
		FO_RELATION_TYPE,			/// CHANGE_TYPE�� ������ ��� ������ �����ɰ�����
		FO_MULTIPLIER,				/// ����(CHANGE_TYPE�� REATION �϶� ���)
		FO_END						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeStatBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeStatBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_eRelationType( BRT_END ),
		m_fResultValue( 0.0f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	float GetValue( const KBuffBehaviorFactor* pBehaviorFactor_, CX2GameUnit* pGameUnit_, const float fRegist_ ) const;
	bool MustUseOptionData() const;

private:
	BUFF_CHANGE_TYPE			m_eChangeType;
	BUFF_RELATION_TYPE			m_eRelationType;
	float						m_fResultValue;
};

/** @class : CX2BuffCreateBuffFactortBehaviorTemplet
	@brief : �ش� ������ �ɷ��ִ� ���ȿ� BuffFactor ����
	@date : 2012/7/23/
*/
class CX2BuffCreateBuffFactortBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_BUFF_FACTOR_ID,
		FO_SKILL_ID,
		FO_END						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffCreateBuffFactortBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffCreateBuffFactortBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_uiBuffFactorId( 0 )
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		, m_uiSkillId( 0 )
#endif //UPGRADE_SKILL_SYSTEM_2013
	{}

private:
	UINT						m_uiBuffFactorId;
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
	UINT						m_uiSkillId;
#endif //UPGRADE_SKILL_SYSTEM_2013
};

/** @class : CX2BuffAbsorbEffectAttackBehaviorTemplet
	@brief : ����Ʈ ������ ����Ͽ� MP ȸ��
	@date : 2012/7/27/
*/
class CX2BuffAbsorbEffectAttackBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_ABSORB_VALUE,			/// �߻�ü �Ѱ��� ȸ����
		FO_END						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffAbsorbEffectAttackBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffAbsorbEffectAttackBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_fAbsorbValue( 0.0f )
	{}

private:
	float						m_fAbsorbValue;
};

// /** @class : CX2BuffChangeMpHitBehaviorTemplet
// 	@brief : Ÿ�ݽ� MP ȸ����
// 	@date : 2012/7/27/
// */
// class CX2BuffChangeMpHitBehaviorTemplet	: public CX2BuffBehaviorTemplet
// {
// public:
// 	enum FACTOR_ORDER
// 	{
// 		FO_MULTIPLIER,				/// ����
// 		FO_END						/// enum ����
// 	};
// 
// 	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeMpHitBehaviorTemplet ); }
// 
// 	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
// 	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
// 	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
// 	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
// 	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
// 
// protected:
// 	CX2BuffChangeMpHitBehaviorTemplet() : CX2BuffBehaviorTemplet(),
// 		m_fMultiplier( 1.0f )
// 	{}
// 
// private:
// 	float						m_fMultiplier;
// };
// 
// /** @class : CX2BuffChangeMpHittedBehaviorTemplet
// 	@brief : Ÿ�ݽ� MP ȸ����
// 	@date : 2012/7/27/
// */
// class CX2BuffChangeMpHittedBehaviorTemplet	: public CX2BuffBehaviorTemplet
// {
// public:
// 	enum FACTOR_ORDER
// 	{
// 		FO_MULTIPLIER,				/// ����
// 		FO_END						/// enum ����
// 	};
// 
// 	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeMpHittedBehaviorTemplet ); }
// 
// 	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
// 	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
// 	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
// 	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
// 	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
// 
// protected:
// 	CX2BuffChangeMpHittedBehaviorTemplet() : CX2BuffBehaviorTemplet(),
// 		m_fMultiplier( 1.0f ), m_eChangeType;
// 	{}
// 
// private:
// 	float						m_fMultiplier;
// 	BUFF_CHANGE_TYPE			m_eChangeType;
// };
// 
// /** @class : CX2BuffChangeHyperModeBehaviorTemplet
// 	@brief : �������� �����Ű�� �����ൿ (���������� �ִٴ� �����Ͽ� ����)
// 	@date : 2012/8/7/
// */
class CX2BuffChangeHyperModeBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_HYPER_MODE_TIME,			/// ���������ð�
		FO_HYPER_MODE_COUNT,		/// ����ΰ�(�������� ���� ���� ����)
		FO_END						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeHyperModeBehaviorTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeHyperModeBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_uiHyperModeCount( 0 ), m_fRemainTime( 0.0f )
	{}

private:
	UINT						m_uiHyperModeCount;	/// ����ΰ�
	float						m_fRemainTime;		/// ���� ���� �ð� �����ֱ��...
};

/** @class : CX2BuffResetSkillCoolTimeBehaviorTemplet
	@brief : ��ų�� ��Ÿ���� �ʱ�ȭ �����ִ� �ൿ
	@date : 2012/8/7/
*/
class CX2BuffResetSkillCoolTimeBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// ����ȯ
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// ��ȭ�Ǵ� Ÿ��
		FO_VALUE,					/// ��
		FO_END,						/// enum ����
	};
#endif // UPGRADE_SKILL_SYSTEM_2013

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffResetSkillCoolTimeBehaviorTemplet ); }

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	
protected:
	CX2BuffResetSkillCoolTimeBehaviorTemplet() : CX2BuffBehaviorTemplet()
		, m_eSkillTypeToResetCoolTime( 0 )
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// ����ȯ
		, m_eChangeType( BCT_FIX_VALUE )
		, m_fCoolTime( 0.f )
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
		, m_fCoolTimeRate ( 1.f ) 
		, m_bIsRelativeAllSkill ( false)
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	UINT				m_eSkillTypeToResetCoolTime;	/// ��Ÿ���� ������ ��ųŸ��
#ifdef UPGRADE_SKILL_SYSTEM_2013 /// ����ȯ
	BUFF_CHANGE_TYPE	m_eChangeType;					/// ���� ���� Ÿ��
	float				m_fCoolTime;					/// ������ ��Ÿ��
#endif // UPGRADE_SKILL_SYSTEM_2013

#ifdef ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�
	float				m_fCoolTimeRate;				// ������ ��Ÿ�� (����)
	bool				m_bIsRelativeAllSkill;			// ��ų Ÿ�԰��� ���� ���� ��� ���� ���� ��ų Ÿ�Կ� ���� ������ ��ĥ ���ΰ�?
#endif // ADDED_BBT_ALL_EQUIP_SKILL_SET_OR_RESET_COOLTIME					// ������ / 13-07-04 / ��� ���� ���� ��ų�� ��Ÿ�� �ִ�ġ�� �����ǰ� �ϴ� BBT �߰�

};

/** @class : CX2BuffChangeUnitScaleBehaviorTemplet
	@brief : ������ ũ�⸦ �����ϴ� �ൿ ������
	@date : 2012/8/7/
*/
class CX2BuffChangeUnitScaleBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeUnitScaleBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeUnitScaleBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_vScale( 0.0f, 0.0f, 0.0f ), m_bUseTimeToReturn( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	D3DXVECTOR3			m_vScale;
	bool				m_bUseTimeToReturn;
};

/** @class : CX2BuffCustomFunctionBehaviorTemplet
	@brief : �ϵ��ڵ��� ó���� CommonStateStart, CommonStateFrameMove, CommonStateEnd���� �����ϵ��� �ϴ� �ൿ ���ø�
	@date : 2012/8/7/
*/
class CX2BuffCustomFunctionBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffCustomFunctionBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffCustomFunctionBehaviorTemplet() : CX2BuffBehaviorTemplet(), m_kBuffBehaviorFactor()
	{}

private:
	KBuffBehaviorFactor		m_kBuffBehaviorFactor;
};

/** @class : CX2BuffEffectSetWithDamageBehaviorTemplet
	@brief : �������� �ִ� ����Ʈ�� �ൿ ���ø�(������ ����� ������Ʈ��PowerRate�� ������ ������ ����Ʈ���� ���� ��ų�� ������ PowerRate�� �ٸ� ��찡 �����Ƿ� �������� �ִ� EffectSet�� ��� PowerRate�� ����ȭ�ϴ� Behavior ���)
	@date : 2012/8/27/
*/
class CX2BuffEffectSetWithDamageBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffEffectSetWithDamageBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffEffectSetWithDamageBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_fPowerRate( 0.0f ), m_bNotUse( false )
	{
		m_vecHandleEffectSet.clear();
		m_vecWstrEffectSetName.clear();
	}
	
	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	void PlayEffectSet( CX2GameUnit* pGameUnit_ );

private:
	vector<CX2EffectSet::Handle>		m_vecHandleEffectSet;
	vector<wstring>						m_vecWstrEffectSetName;	/// ���� ����Ʈ���� ��������
	float								m_fPowerRate;
	bool								m_bNotUse;				/// ���ø��� EffectSet�� �����Ǿ� �־, Factor���� ����Ʈ���� ��� ���θ� ��� �����ߴ����� �����ϴ� ����
};

/** @class : CX2BuffStunBehaviorTemplet
	@brief : ���� ������Ʈ�� ������Ű�� ���ø�
	@date : 2012/8/29/
*/
class CX2BuffStunBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffStunBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffStunBehaviorTemplet() : CX2BuffBehaviorTemplet()
	{}	
};

/** @class : CX2BuffFinishOtherBuffDebuffBehaviorTemplet
	@brief : �ٸ� ����, ������� ���� ��Ű�� �ൿ
	@date : 2012/9/2/
*/
class CX2BuffFinishOtherBuffDebuffBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffFinishOtherBuffDebuffBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;

	/// �ʿ䰡 ���� �� ���Ƽ� ����
	//virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	
protected:
	CX2BuffFinishOtherBuffDebuffBehaviorTemplet() : CX2BuffBehaviorTemplet()
	{
		m_vecBuffTempletIDToBeFinished.clear();
	}
	
	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
	inline void EraseBuffTempletFromGameUnit( CX2GameUnit* pGameUnit_, const BUFF_TEMPLET_ID eExceptBuffTempletID_ );

private:
	vector<BUFF_TEMPLET_ID>		m_vecBuffTempletIDToBeFinished;
};

/** @class : CX2BuffFixStateBehaviorTemplet
	@brief : ���� ������Ʈ�� ������Ű�� ���ø�
	@date : 2012/9/4/
*/
class CX2BuffFreezeBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffFreezeBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

protected:
	CX2BuffFreezeBehaviorTemplet() : CX2BuffBehaviorTemplet()
		, m_fAnimCurrentTime( 0.0f ), m_bIsDown( false )
	{}	

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	float	m_fAnimCurrentTime;
	bool	m_bIsDown;				/// ���� ����� �ٿ� ������Ʈ�� �Ѿ ������
};

/** @class : CX2BuffCanPassUnitBehaviorTemplet
	@brief : ������ ����Ǵ� ���� ������ CanPassUnit�� ���� �ϴ� �ൿ ���ø�
	@date : 2012/9/5/
*/
class CX2BuffCanPassUnitBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffCanPassUnitBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_, float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	virtual void OnFrameMove( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

protected:
	CX2BuffCanPassUnitBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_bCanPassUnit( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	bool	m_bCanPassUnit;
};

/** @class : CX2BuffNeverMoveBehaviorTemplet
	@brief : ������ ����Ǵ� ���� ������ ���ڸ� ���� ��Ű�� �ൿ ������
	@date : 2012/9/5/
*/
class CX2BuffNeverMoveBehaviorTemplet: public CX2BuffBehaviorTemplet
{
public:

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffNeverMoveBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
#ifdef NEVER_MOVE_GRAVITY_APPLY
	CX2BuffNeverMoveBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_vPosition( 0.0f, 0.0f, 0.0f ), m_bIsGravity( false )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );
#else
	CX2BuffNeverMoveBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_vPosition( 0.0f, 0.0f, 0.0f )
	{}
#endif //NEVER_MOVE_GRAVITY_APPLY



private:
	D3DXVECTOR3		m_vPosition;
#ifdef NEVER_MOVE_GRAVITY_APPLY
	bool			m_bIsGravity;	//�߷��� �����Ұ��ΰ�. true�� �߷� ����, false�� ��ġ ����
#endif //NEVER_MOVE_GRAVITY_APPLY
};

/** @class : CX2BuffChangeAttackByTypeBehaviorTemplet
	@brief : ���� Ÿ�Կ� ���� �������� ������Ű�� �ൿ ������
	@date : 2012/11/03/
*/
class CX2BuffChangeAttackByTypeBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// ��ȭ�Ǵ� Ÿ��
		FO_VALUE,					/// ��
		FO_ATTACK_TYPE,				/// ���� Ÿ��
		FO_END,						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeAttackByTypeBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeAttackByTypeBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_fResultValue( 0.f ), m_eAttackType( CX2DamageManager::AT_NORMAL )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE				m_eChangeType;
	float							m_fResultValue;
	CX2DamageManager::ATTACK_TYPE	m_eAttackType;
};

/** @class : CX2BuffChangeEncahntAttackRateBehaviorTemplet
@brief : ���� �Ӽ� ���� �ߵ� Ȯ���� ������Ű�� �ൿ ������
@date : 2012/11/03/
*/
class CX2BuffChangeEncahntAttackRateBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// ��ȭ�Ǵ� Ÿ��
		FO_VALUE,					/// ��
		FO_END,						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeEncahntAttackRateBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeEncahntAttackRateBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_fRate( 0.f )
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// �⺻ ���� ��� �Ӽ� ��ȭ
		, m_eEnchantAttributeType ( BEAT_ALL )
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE				m_eChangeType;
	float							m_fRate;
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
	BUFF_ENCHANT_ATTRIBUTE_TYPE		m_eEnchantAttributeType;	// ��æƮ �Ӽ� Ÿ��
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
};

/** @class : CX2BuffChangeEncahntAttackRateBehaviorTemplet
@brief : ���� �Ӽ� ���� �ߵ� Ȯ���� ������Ű�� �ൿ ������
@date : 2012/11/03/
*/
class CX2BuffDoubleAttackBehaviorTemplet : public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// ��ȭ�Ǵ� Ÿ��
		FO_VALUE,					/// ��
		FO_END,						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffDoubleAttackBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffDoubleAttackBehaviorTemplet() : CX2BuffBehaviorTemplet(),
		m_eChangeType( BCT_SWAP_VALUE ), m_fRate( 0.f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE				m_eChangeType;
	float							m_fRate;
};

/** @class : CX2BuffChangeAttackSphereScaleBehaviorTemplet
	@brief : ���ùڽ��� ũ�⸦ �����ϴ� �ൿ ���ø�
	@date : 2013/03/13/
*/
class CX2BuffChangeAttackSphereScaleBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	struct AttackSphereNameAndScale
	{
		wstring		m_wstrAttackSphereName;
		float		m_fScale;

		AttackSphereNameAndScale()
			: m_fScale( 1.0f )
		{
			m_wstrAttackSphereName.clear();
		}
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeAttackSphereScaleBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeAttackSphereScaleBehaviorTemplet() : CX2BuffBehaviorTemplet()
	{
		m_vecAttackSphereNameAndScale.clear();
	}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	vector<AttackSphereNameAndScale>	m_vecAttackSphereNameAndScale;
};

/** @class : CX2BuffAddSkillLevelBehaviorTemplet
	@brief : ��ų������ Up �Ǵ� Down ��Ű�� �ൿ ���ø�
	@date : 2013/04/16/
*/
class CX2BuffAddSkillLevelBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_SKILL_LEVEL = 0,				///	��ȭ �� ��ų���� ��
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffAddSkillLevelBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffAddSkillLevelBehaviorTemplet() 
		: CX2BuffBehaviorTemplet(), m_iAddSkillLevel( 0 )
	{}

private:
	int	m_iAddSkillLevel;		/// ��� �Ǵ� ���� ��ų ��ų ����
};

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ

/** @class : CX2BuffReflectMagicBehaviorTemplet
	@brief : �ش� ������ �ɷ��ִ� ���ȿ��� ����(�Ϲ�+��ų) �Ұ� �ൿ
	@date : 2013/7/10/
*/
class CX2BuffReflectMagicBehaviorTemplet	: public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,				/// ��ȭ�Ǵ� Ÿ��
		FO_VALUE,					/// ��
		FO_END						/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffReflectMagicBehaviorTemplet ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffReflectMagicBehaviorTemplet() : CX2BuffBehaviorTemplet()
		, m_eChangeType( BCT_SWAP_VALUE )
		, m_fResultValue( 0.0f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE			m_eChangeType;
	float						m_fResultValue;
};

#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef FIX_SKILL_BALANCE_AISHA_LENA //JHKang
/** @class : CX2BuffChangeManaConsumeRate
	@brief : �ش� ������� �ɷ��ִ� ��ų��� �� ���� �Һ� ��ȭ
	@date : 2013/09/09/
*/
class CX2BuffChangeConsumeMpRate : public CX2BuffBehaviorTemplet
{
public:
	enum FACTOR_ORDER
	{
		FO_CHANGE_TYPE,		/// ��ȭ�Ǵ� Ÿ��
		FO_SKILL_ID,		/// ��ų ���̵�
		FO_VALUE,			/// ��
		FO_END				/// enum ����
	};

	static CX2BuffBehaviorTempletPtr	CreateBuffBehaviorTempletPtr() { return CX2BuffBehaviorTempletPtr( new CX2BuffChangeConsumeMpRate ); }

	virtual bool SetFactor( const CX2BuffFactor& BuffFactor_, CX2GameUnit* pGameUnit_, const CX2BuffTemplet* pBuffTemplet_ );
	virtual CX2BuffBehaviorTempletPtr GetClonePtr() const;
	virtual void GetFactor( OUT vector<KBuffBehaviorFactor>& vecFactors_ ) const;
	virtual void SetFactorFromPacket( const KBuffFactor& kBuffFactor_, const KBuffBehaviorFactor& kFactor_, CX2GameUnit* pGameUnit_ );
	virtual void DoFinish( CX2GameUnit* pGameUnit_, CX2BuffTemplet* pBuffTemplet_ );

protected:
	CX2BuffChangeConsumeMpRate() : CX2BuffBehaviorTemplet()
		, m_eChangeType( BCT_SWAP_VALUE ), m_fResultValue( 0.0f ), m_uiSkillId( 0 ), m_fInitMp( 0.f )
	{}

	virtual bool ParsingBehavior( KLuaManager& luaManager_ );

private:
	BUFF_CHANGE_TYPE	m_eChangeType;		/// ��ȭ�Ǵ� Ÿ��
	UINT				m_uiSkillId;		/// ����� ��ų ���̵�
	float				m_fResultValue;		/// ��
	float				m_fInitMp;			/// �ʱ� ���� ��뷮
};
#endif //FIX_SKILL_BALANCE_AISHA_LENA