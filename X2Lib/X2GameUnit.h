#pragma once

#include "./X2SoundCloseManager.h"
#include "./X2RidingPet.h"

#define LINE_RADIUS 30.0f
#define EVENT_FLAG_LIST_NUM 10
#ifdef SERV_RAVEN_VETERAN_COMMANDER
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 35	/// ���̺� 2�� ���� ���׶� Ŀ�Ǵ� - ġ���
#else
#ifdef SERV_RENA_NIGHT_WATCHER
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 34	/// ���� 2�� ���� ����Ʈ ���� - ������ ȭ��
#else
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 33
#else
#ifdef EVE_ELECTRA
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 32
#else
#ifdef RAVEN_WEAPON_TAKER
#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 31
#else RAVEN_WEAPON_TAKER
	//{{ kimhc // 2011-03-31 // 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
	#ifdef CHUNG_SECOND_CLASS_CHANGE
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
		#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 30
#else
		#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 29
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE
	#else
		#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 28
	#endif
#else	CASH_SKILL_FOR_CHUNG_2011_04
	#define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER 26
#endif	CASH_SKILL_FOR_CHUNG_2011_04
	//}} kimhc // 2011-03-31 // 2011-04 �� ��ġ�� û ĳ�� ��ų
#endif RAVEN_WEAPON_TAKER
#endif EVE_ELECTRA
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN
#endif SERV_RENA_NIGHT_WATCHER
#endif SERV_RAVEN_VETERAN_COMMANDER
#define SET_CB_FUNC(classname,funcname) GUStateCBFunc::from_method<classname, &classname::funcname>(this)

typedef	srutil::delegate0<void>	DelegateProcess;
#define SET_DELEGATE_PROCESS(classname,funcname) DelegateProcess::from_method<classname, &classname::funcname>(this)

typedef	srutil::delegate1<void, const CX2DamageManager::DamageData&>	DelegateProcessWithDamageData;
#define SET_DELEGATE_PROCESS_WITH_DAMAGE_DATA(classname,funcname) DelegateProcessWithDamageData::from_method<classname, &classname::funcname>(this)

#define ERASE_BUFF_IDENTITY( vecBuffIdentity_, BuffIdentity_ ) \
{ \
	vector<CX2BuffIdentityPtr>::iterator vItr \
		= FindSameBuffIdentity<vector<CX2BuffIdentityPtr>::iterator, CX2BuffIdentityPtr>( vecBuffIdentity_, BuffIdentity_ ); \
	if ( vecBuffIdentity_.end() != vItr ) \
		vecBuffIdentity_.erase( vItr ); \
} \

class CX2GameUnit;
typedef KObserverPtr<CX2GameUnit>   CX2GameUnitoPtr;

class CX2UnitSlashTraceManager;
class CX2Game;

// CX2GameUnit �� ù��° parent class �� �ݵ�� CKTDGObject �� �� ��!
// lua_tinker �� ������ ���� Ŭ���� ��ӽ� ù��°�� �ƴ� parent class �� ���� ����� �õ��� ��� ũ���� �߻�
class CX2GameUnit : public CKTDGObject, public CKTDXDeviceHolder, public KObservable<CX2GameUnit>
{
	friend CX2OldGageManager;
	friend CX2EnchantItem;

	// ���� ����ü, ����ü
	public:

		typedef CKTDGParticleSystem::CParticleEventSequenceHandle ParticleEventSequenceHandle;

		enum GAME_UNIT_TYPE
		{
			GUT_USER	= 0,
			GUT_NPC,			
			//GUT_TRANSFORMER, 
		};

		enum GAME_UNIT_STATE_ID
		{
			GUSI_NONE = 0,
			GUSI_LOADING,
			GUSI_READY,
			GUSI_PLAY,
			GUSI_DIE,
			GUSI_END,
		};


		enum ENABLE_DASH_STATE
		{
			EDS_LEG_WOUND,
			EDS_MEDITATION,
			EDS_END,
		};

#ifdef SERV_SKILL_NOTE
		enum SPEED_FACTOR_ID
		{
			SFI_NONE		= 0,
			SFI_SPEED_1,	/// ���ε� ��Ŭ �ӵ����� �����
			SFI_SPEED_2,	/// ���� ���� �ӵ� ���� ����
			SFI_SPEED_3,	/// �� �ӵ����� ����
			SFI_SPEED_4,	/// ��Ƽ���̼� �޸� �̼� ���� ����
#ifdef NEW_SKILL_2010_11
			SFI_SPEED_5,	/// ���� ����Ŀ - ���̷�����, ��� �ӵ� ���� ���� ����
#endif NEW_SKILL_2010_11
#ifdef WIDE_BUFF_ANI_SPEED_UP
			SFI_SPEED_6,	/// ���� Ʈ������ ���� �ִϸ��̼� �ӵ� ���� ����
#endif
#ifdef NEW_HENIR_TEST
			SFI_SPEED_7,	/// ��ϸ� �ٶ��� �� - ��� �ӵ� ���� ���� 
#endif NEW_HENIR_TEST
#ifdef ELSWORD_WAY_OF_SWORD
			SFI_SPEED_8,	/// ���ҵ� Ȱ���� �˻� - ���� �ӵ� ���� ���� 
#endif ELSWORD_WAY_OF_SWORD
#ifdef UPGRADE_RAVEN
			SFI_SPEED_9,	// ���̺� ���� ����
#endif
#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
			SFI_SPEED_10,	// ���ǵ� ����������
#endif
#ifdef SPECIAL_USE_ITEM
			SFI_SPEED_11,	// ���� ��� ������, �ٶ� ����
#endif
#ifdef BALANCE_PATCH_20120329
			SFI_SPEED_13,	// �̺� Ŭ��ŷ �����
#endif
		};

		enum SPEED_FACTOR_SET_TYPE		// FACTOR��Ҹ� Set �� �� factor�� vector�� ��� ���
		{
			// ���ǵ� ���� ����ü�� ������ ���� �ʰ� Set �Լ��� ȣ���� ���� �������ڷ� �Ѱ��ִ� �뵵�� ���

			SFST_MULTY,					// vector���� ���� ���̵� ��Ұ� ������ �־ �Ǵ� Ÿ��
			SFST_ONLY,					// vector���� ���� ���̵� ��Ұ� �ִ� ��� ���� set �ϴ� factor�� �����ϴ� Ÿ��
			SFST_UPDATE,				// ������ vector ���� �ִ� ��Ҹ� ���� �����ִ� Ÿ�� (���� �� Ÿ������ Set ���� �ÿ� vector���� �ش� ��� �� ������ �߰� ��)
		};
#endif

#ifdef CHUNG_SECOND_CLASS_CHANGE
		enum PULSE_WEAPON_COLOR_ID
		{
			PWCI_NONE		= 0,
			PWCI_UNDEFINED,
			PWCI_CHUNG_LAND_DEMOLISHER,
		};
#endif CHUNG_SECOND_CLASS_CHANGE

//-------------------------------------------------------------------------------------------------------
		struct  InitDeviceData
		{
			std::vector<std::wstring>    m_vecstrReadyTexture;
			std::vector<std::wstring>    m_vecstrReadyXMesh;
			std::vector<std::wstring>    m_vecstrReadyXSkinMesh;
			std::vector<std::wstring>    m_vecstrReadySound;
			std::vector<std::wstring>	 m_vecstrReadyXET;
			std::vector<std::wstring>	 m_vecstrReadyGameMajorParticle;
			std::vector<std::wstring>	 m_vecstrReadyGameMinorParticle;

			void    AppendToDeviceList( CKTDXDeviceDataList& listInOut_ ) const;

		};//struct  InitDeviceData

		struct InputData
		{
			bool pureLeft;				/// ���� ���� ȭ��ǥ�� �ѹ� ��� ������ �ִ� ����
			bool oneLeft;				/// ���� ���� ȭ��ǥ�� �ѹ� ������ �� ����
			bool pureDoubleLeft;		/// ���� ���� ȭ��ǥ�� �ι� ������, ��� ������ �ִ� ����
			bool oneDoubleLeft;			/// ���� ���� ȭ��ǥ�� �ι� ������ �� ����

			bool pureRight;
			bool oneRight;
			bool pureDoubleRight;
			bool oneDoubleRight;

			bool pureUp;
			bool oneUp;
			bool pureDoubleUp;
			bool oneDoubleUp;

			bool pureDown;
			bool oneDown;
			bool pureDoubleDown;
			bool oneDoubleDown;

			bool pureZ;
			bool oneZ;
			bool pureDoubleZ;
			bool oneDoubleZ;

			bool pureX;
			bool oneX;
			bool pureDoubleX;
			bool oneDoubleX;

			bool pureC;
			bool oneC;
			bool pureDoubleC;
			bool oneDoubleC;

			bool pureA;
			bool oneA;
			bool pureDoubleA;
			bool oneDoubleA;

			bool pureS;
			bool oneS;
			bool pureDoubleS;
			bool oneDoubleS;

			bool pureD;
			bool oneD;
			bool pureDoubleD;
			bool oneDoubleD;

			bool pureF;
			bool oneF;
			bool pureDoubleF;
			bool oneDoubleF;

//{{������ // 2012-04-30 // ��ų���� ü���� ���� Ȯ�� ��ų ��� �� �� �ֵ��� ����
			bool pureQ;
			bool oneQ;
			bool pureDoubleQ;
			bool oneDoubleQ;

			bool pureW;
			bool oneW;
			bool pureDoubleW;
			bool oneDoubleW;

			bool pureE;
			bool oneE;
			bool pureDoubleE;
			bool oneDoubleE;

			bool pureR;
			bool oneR;
			bool pureDoubleR;
			bool oneDoubleR;
//}}������ // 2012-04-30 // ��ų���� ü���� ���� Ȯ�� ��ų ��� �� �� �ֵ��� ����

			bool pureLShift;
			bool oneLShift;
			bool pureDoubleLShift;
			bool oneDoubleLShift;

			bool pureLAlt;
			bool oneLAlt;
			bool pureDoubleLAlt;
			bool oneDoubleLAlt;

			bool pureSpace;
			bool oneSpace;
			bool pureDoubleSpace;
			bool oneDoubleSpace;

			bool pureLCtr;
			bool oneLCtr;
			bool pureDoubleLCtr;
			bool oneDoubleLCtr;

			InputData()
			{
				Init();
			}

			void Init()
			{
				pureLeft			= false;
				oneLeft				= false;
				pureDoubleLeft		= false;
				oneDoubleLeft		= false;

				pureRight			= false;
				oneRight			= false;
				pureDoubleRight		= false;
				oneDoubleRight		= false;

				pureUp				= false;
				oneUp				= false;
				pureDoubleUp		= false;
				oneDoubleUp			= false;

				pureDown			= false;
				oneDown				= false;
				pureDoubleDown		= false;
				oneDoubleDown		= false;

				pureZ				= false;
				oneZ				= false;
				pureDoubleZ			= false;
				oneDoubleZ			= false;

				pureX				= false;
				oneX				= false;
				pureDoubleX			= false;
				oneDoubleX			= false;

				pureC				= false;
				oneC				= false;
				pureDoubleC			= false;
				oneDoubleC			= false;

				pureA				= false;
				oneA				= false;
				pureDoubleA			= false;
				oneDoubleA			= false;

				pureS				= false;
				oneS				= false;
				pureDoubleS			= false;
				oneDoubleS			= false;

				pureD				= false;
				oneD				= false;
				pureDoubleD			= false;
				oneDoubleD			= false;


				pureF				= false;
				oneF				= false;
				pureDoubleF			= false;
				oneDoubleF			= false;

				//{{������ // 2012-04-30 // ��ų���� ü���� ���� Ȯ�� ��ų ��� �� �� �ֵ��� ����
				pureQ				= false;
				oneQ				= false;
				pureDoubleQ			= false;
				oneDoubleQ			= false;

				pureW				= false;
				oneW				= false;
				pureDoubleW			= false;
				oneDoubleW			= false;

				pureE				= false;
				oneE				= false;
				pureDoubleE			= false;
				oneDoubleE			= false;

				pureR				= false;
				oneR				= false;
				pureDoubleR			= false;
				oneDoubleR			= false;
				//}}������ // 2012-0= false;4-30 // ��ų���� ü���� ���� Ȯ�� ��ų ��� �� �� �ֵ��� ����


				pureLShift			= false;
				oneLShift			= false;
				pureDoubleLShift	= false;
				oneDoubleLShift		= false;

				pureLAlt			= false;
				oneLAlt				= false;
				pureDoubleLAlt		= false;
				oneDoubleLAlt		= false;

				pureSpace			= false;
				oneSpace			= false;
				pureDoubleSpace		= false;
				oneDoubleSpace		= false;

				pureLCtr			= false;
				oneLCtr				= false;
				pureDoubleLCtr		= false;
				oneDoubleLCtr		= false;
			}



			void Init_ZXArrowOnly()
			{
				pureLeft			= false;
				oneLeft				= false;
				pureDoubleLeft		= false;
				oneDoubleLeft		= false;

				pureRight			= false;
				oneRight			= false;
				pureDoubleRight		= false;
				oneDoubleRight		= false;

				pureUp				= false;
				oneUp				= false;
				pureDoubleUp		= false;
				oneDoubleUp			= false;

				pureDown			= false;
				oneDown				= false;
				pureDoubleDown		= false;
				oneDoubleDown		= false;

				pureZ				= false;
				oneZ				= false;
				pureDoubleZ			= false;
				oneDoubleZ			= false;

				pureX				= false;
				oneX				= false;
				pureDoubleX			= false;
				oneDoubleX			= false;


				//pureLShift			= false;
				//oneLShift			= false;
				//pureDoubleLShift	= false;
				//oneDoubleLShift		= false;

				//pureLAlt			= false;
				//oneLAlt				= false;
				//pureDoubleLAlt		= false;
				//oneDoubleLAlt		= false;

				//pureSpace			= false;
				//oneSpace			= false;
				//pureDoubleSpace		= false;
				//oneDoubleSpace		= false;

				//pureLCtr			= false;
				//oneLCtr				= false;
				//pureDoubleLCtr		= false;
				//oneDoubleLCtr		= false;

			}


			bool IsKeyPressed()
			{
				bool bPressed = false;

				UINT nSize = sizeof(InputData) / sizeof(bool);
				bool* pKey = (bool*)this;
				for( UINT i=0; i<nSize; i++ )
				{
					if( true == pKey[i] )
					{
						bPressed = true;
					}
				}
				return bPressed;
			}
		};

//-------------------------------------------------------------------------------------------------------
		
		/** @class : TIME_SPEED
			@brief : ������ �̵��ӵ�, m_PhysicParam.nowSpeed�� ���� ��ġ�� ���ϰų� �����ϱ� ���� ����ü
			@date  : 2010/11/11
		*/
		struct TIME_SPEED
		{
			D3DXVECTOR3 vSpeed;		/// x�� X��, y�� Y��, z�� ���ǵ尡 ����� �ִϸ��̼� Ÿ�� ����
			int iFlag;				/// 1�̸� m_PhysicParam.nowSpeed �� x, y�� += ������\n
									///	2�̸� = ������ �����ϰ��ϴ� �÷���

			TIME_SPEED() :
			vSpeed( D3DXVECTOR3(0.f, 0.f, 0.f) ),
			iFlag( 0 )
			{
			}
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            bool    operator < ( const TIME_SPEED& In_ ) const { return vSpeed.z < In_.vSpeed.z; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		};

//-------------------------------------------------------------------------------------------------------

#ifdef RAVEN_SECOND_CLASS_CHANGE
		struct TIME_PUSHPASS
		{
			float fTime;
			bool bCan;

			TIME_PUSHPASS() :
			fTime(0.f),
			bCan(false)
			{
			}
		};
#endif

//-------------------------------------------------------------------------------------------------------

		// m_fDuration �ð� ���� �ȱ�, �޸��� �ӵ��� m_fSpeedRate��ŭ ����
		struct SPEED_FACTOR
		{
#ifdef SERV_SKILL_NOTE
			SPEED_FACTOR_ID	m_eId;
#endif
			float	m_fDuration;
			float	m_fSpeedRate;

//{{ kimhc // 2010.4.28 // ��д��� �۾�(���� ���� ȿ��)
#ifdef SERV_SECRET_HELL
			CX2EffectSet::Handle	m_hEffectSet;
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.28 // ��д��� �۾�(���� ���� ȿ��)

			SPEED_FACTOR() :
#ifdef SERV_SKILL_NOTE
			m_eId( SFI_NONE ),
#endif
			m_fDuration( 0.f ),
			m_fSpeedRate( 0.f )

			//{{ kimhc // 2010.4.28 // ��д��� �۾�(���� ���� ȿ��)
#ifdef SERV_SECRET_HELL
			, m_hEffectSet( INVALID_EFFECTSET_HANDLE )
#endif SERV_SECRET_HELL
			//}} kimhc // 2010.4.28 // ��д��� �۾�(���� ���� ȿ��)
			{
			}
		};

//-------------------------------------------------------------------------------------------------------

#ifdef DUNGEON_ITEM
		struct BUFFER_FACTOR
		{
			float m_fBufferRate;
			float m_fBufferTime;

			BUFFER_FACTOR() :
			m_fBufferRate( 1.f ),
			m_fBufferTime( 0.f )
			{
			}
		};
#endif

//-------------------------------------------------------------------------------------------------------

#ifdef GRAPPLING_TEST
		struct GrapplingState
		{
			GAME_UNIT_TYPE m_eUnitTypeGrapplingMe;	 
			UidType m_UnitUIDGrapplingMe;					// ���� ���� ����
			GAME_UNIT_TYPE m_eUnitTypeGrappledByMe;	
			UidType m_UnitUIDGrappledByMe;					// ���� ���� ����

			D3DXVECTOR3 m_vGrapplingPosition;				// ���� ������� ���� ���� ���� ����ġ
			D3DXVECTOR3 m_vGrapplingRotation;
			CKTDXDeviceXSkinMesh::MultiAnimFrame* m_pFrameGrappling;

			D3DXVECTOR3 m_vGrappledPosition;				// ���� ������ �� ���� ���� ���� �����ġ
			D3DXVECTOR3 m_vGrappledRotation;				
			CKTDXDeviceXSkinMesh::MultiAnimFrame* m_pFrameGrappled;


			GrapplingState()
			: m_eUnitTypeGrapplingMe( GUT_USER )
			, m_UnitUIDGrapplingMe( -1 )
			, m_eUnitTypeGrappledByMe( GUT_USER )
			, m_UnitUIDGrappledByMe( -1 )
			, m_vGrapplingPosition( 0, 0, 0 )
			, m_vGrapplingRotation( 0, 0, 0 )
			, m_pFrameGrappling( NULL )
			, m_vGrappledPosition( 0, 0, 0 )
			, m_vGrappledRotation( 0, 0, 0 ) 
			, m_pFrameGrappled( NULL )
			{
			}


			void ResetGrapplingState( bool OnlyMe = false );
			void SetUnitGrapplingMe( GAME_UNIT_TYPE eGameUnitType, UidType unitUID );
			CX2GameUnit* GetUnitGrapplingMe();
			void SetUnitGrappledByMe( GAME_UNIT_TYPE eGameUnitType, UidType unitUID );
			CX2GameUnit* GetUnitGrappledByMe();

			void CalcGrapplingPosition();
			const D3DXVECTOR3& GetGrapplingPosition() const { return m_vGrapplingPosition; }
			const D3DXVECTOR3& GetGrapplingRotation() const { return m_vGrapplingRotation; }

			void CalcGrappledPosition();
			const D3DXVECTOR3& GetGrappledPosition() const { return m_vGrappledPosition; }
			const D3DXVECTOR3& GetGrappledRotation() const { return m_vGrappledRotation; }

		};
#endif GRAPPLING_TEST

//-------------------------------------------------------------------------------------------------------

		// note!! �� ����ü ������ Ŀ���� �ʿ��� �͸� �ʿ� �����ϴ� �������� �ٲٱ�
		struct ExtraDamagePack
		{
			CX2DamageManager::ExtraDamageData	m_Poison;
			CX2DamageManager::ExtraDamageData	m_Fire;
			CX2DamageManager::ExtraDamageData	m_Curse;
			CX2DamageManager::ExtraDamageData	m_Slow;
			CX2DamageManager::ExtraDamageData	m_Lightning;
			CX2DamageManager::ExtraDamageData	m_ReverseLeftRight;
			CX2DamageManager::ExtraDamageData	m_Cold;
			CX2DamageManager::ExtraDamageData	m_Y_Pressed;	// ����

#ifdef SKILL_30_TEST
			CX2DamageManager::ExtraDamageData	m_Frozen;				/// �����Ǽ� ���ߴ� ��
			CX2DamageManager::ExtraDamageData	m_LegWound;				/// ��ü ��ó
			CX2DamageManager::ExtraDamageData	m_WeaponBreak;			/// ���� �ı�
			CX2DamageManager::ExtraDamageData	m_ArmorBreak;			/// �� �ı�
			CX2DamageManager::ExtraDamageData	m_Entangled;			/// ���� ���ʱ� �ʻ��. �ٵ�� �� �����̰�
			CX2DamageManager::ExtraDamageData	m_Dull;					/// ���� �ο�ű. ������ ��ȭ
	
			CX2DamageManager::ExtraDamageData	m_TimeBomb;				/// ���̻� �����ʵ� ������ź
			CX2DamageManager::ExtraDamageData	m_Stun;					/// ���� ������ stun
#endif SKILL_30_TEST

			CX2DamageManager::ExtraDamageData	m_Aging;				/// ��ȭ


			CX2DamageManager::ExtraDamageData	m_EnchantBlaze;
			CX2DamageManager::ExtraDamageData	m_EnchantFrozen;
			CX2DamageManager::ExtraDamageData	m_EnchantPoison;
			CX2DamageManager::ExtraDamageData	m_EnchantShock;


			CX2DamageManager::ExtraDamageData	m_MagicDefenceDebuff;
			CX2DamageManager::ExtraDamageData	m_StigmaDebuff;
			
			CX2DamageManager::ExtraDamageData	m_Dryad;
#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.8] // �ε� ����Ʈ - ���� ���� - EDT_BLIND
			CX2DamageManager::ExtraDamageData	m_Blind;;
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST
			CX2DamageManager::ExtraDamageData	m_WaterHold;
#endif EDT_WATER_HOLD_TEST

#ifdef	CASH_SKILL_FOR_CHUNG_2011_04
			CX2DamageManager::ExtraDamageData	m_Panic;	/// ����ð��� ���� Ű���� ��Ʈ���� �¿� �����Ǵ� �ͽ�Ʈ�� ������
			CX2DamageManager::ExtraDamageData	m_Pain;		/// ���濡�� �ɷ��ִ� ExtraDamage�� ���� �����ϴ� Ƚ���� �޶����� ExtraDamage
#endif	CASH_SKILL_FOR_CHUNG_2011_04

#ifdef CHUNG_SECOND_CLASS_CHANGE
			CX2DamageManager::ExtraDamageData	m_LegShot;	/// �̵�, ���� �ӵ��� ���ҽ�Ų��.
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
			CX2DamageManager::ExtraDamageData	m_EarthQuake;/// �̵�, �ִϸ��̼� �ӵ��� ���ҽ�Ų��.
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef RAVEN_WEAPON_TAKER
			CX2DamageManager::ExtraDamageData	m_SmokeBlind;/// ȸ�ǰ���
#endif RAVEN_WEAPON_TAKER

#ifdef EVE_ELECTRA
			CX2DamageManager::ExtraDamageData	m_FlashBang;
#endif EVE_ELECTRA

#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
			CX2DamageManager::ExtraDamageData	m_MarkOfCommander;
#endif SERV_ADD_CHUNG_SHELLING_GUARDIAN

			// kimhc // 2010-04-05 // map���� �ٲٷ� ������ �����۾��� ���� ���� �־� ������ �ٲ�� �ҵ�
			// ���� �� ����ü ���� ExtraDamageData�� 30���� �̰��� ������ �߰��ϸ�
			// #define NUM_OF_EXTRA_DAMAGE_PACK_MEMBER �� ���� ���� ���������

#ifdef VELDER_SECRET_DUNGEON
			// ��������� �����ϹǷ� �ϴ� NUM_OF_EXTRA_DAMAGE_PACK_MEMBER �� �߰����� �ʴ´�.
			// �������� EDT�� ���� �� ���� �߰��ؾ���
			CX2DamageManager::ExtraDamageData	m_ScaleCriticalBuff;
			CX2DamageManager::ExtraDamageData	m_BerserkerBuff;
			CX2DamageManager::ExtraDamageData	m_ManaBuff;
			CX2DamageManager::ExtraDamageData	m_AttackAllTeam;
#endif
#ifdef UPGRADE_RAVEN
			CX2DamageManager::ExtraDamageData	m_RunJumpSpeed;
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
			CX2DamageManager::ExtraDamageData	m_StartOfDelayedFiring;		/// ���� 2�� ���� ����Ʈ ���� - ������ ��ȣź
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_RAVEN_VETERAN_COMMANDER
			CX2DamageManager::ExtraDamageData	m_Bleeding;					/// ���̺� 2�� ���� ���׶� Ŀ�Ǵ� - ġ���
#endif SERV_RAVEN_VETERAN_COMMANDER

			void Init();
#ifdef CHUNG_SECOND_CLASS_CHANGE
			void Init(CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageType);
#endif CHUNG_SECOND_CLASS_CHANGE
			bool Verify();
		};

//-------------------------------------------------------------------------------------------------------

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        struct  KAttackTimeProjSeqName
        {
            D3DXVECTOR2 m_v2AttackTime;
            wstring     m_wstrProjSeqName;

            KAttackTimeProjSeqName()
            : m_v2AttackTime( 0.f, 0.f ) {}
        };//struct  KAttackTimeProjSeqName

#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		/// ���� ���� �Ķ����
		struct StateParam
		{
			bool					bLandConnect;	/// ���� ��� �־�� �ϴ� ������Ʈ
			KProtectedType<bool>	bCanPushUnit;		
			KProtectedType<bool>	bCanPassUnit;		/// �ٸ� ���ֵ��� ����ϰų�, �ٸ� ���ֵ鿡 ���� ��� �� �� ����
			PROTECT_VECTOR2			invincibleTime;
			KProtectedType<bool>	bInvincible;
			KProtectedType<bool>	bSuperArmor;
			KProtectedType<float>	fRevengeStartTime;
			KProtectedType<float>	fRevengeEndTime;
			int						normalCamera;
			int						hitCamera;
			bool					bResetCamera;
			bool					bEventFlagList[EVENT_FLAG_LIST_NUM];
            bool                    bSuperArmorNotRed;		/// ���� �Ƹ��� �� ���������� ��Ÿ���� ����
            int                     fGuardDefence;			/// ���
			bool					bAttackState;			/// ���� ���°� ���ݻ�������, m_AttackTimeList�� empty�� �ƴϰų� �� ������ �����Ǿ� �ִٸ� ���ݻ�����
#ifdef SERV_EVE_BATTLE_SERAPH
			bool					bNoChangSpectro;		/// ��ų �߿��� ������ ������ �ʾƾ� �Ǵ� ��ų ����
#endif
			
			KProtectedType<float>	fReflexMagicStartTime;	/// 
			KProtectedType<float>	fReflexMagicEndTime;	/// 

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SUPER_ARMOR_TIME
			std::vector<D3DXVECTOR2> m_vecSuperArmorTime;
#endif

#ifdef ARA_CHARACTER_BASE
			USHORT					m_usSoundSet;		/// ���� Set ��ȣ ����
#endif

#ifdef ADD_CAN_PASS_ALLY_UNIT	//JHKang
			KProtectedType<bool>	bCanPassAllyUnit;	/// �ٸ� ���ֵ��� ����ϰų�, �ٸ� ���ֵ鿡 ���� ��� �� �� ����, ���� ���� ���
#endif //ADD_CAN_PASS_ALLY_UNIT
			bool					bAllowDirChange;
			PROTECT_VECTOR3			m_DefenceTime;
			D3DXVECTOR2				afterImage;
			std::map< int, D3DXVECTOR3 > m_mapAddSlashTrace;		// weapon index, slashtrace_data
			float					fSlashTraceTipWide;
			bool					bApplyAnimMove;
			bool					bFallDown;			
            bool                    bGuardFront;			/// �� ���
			bool					bGuardBack;				/// �� ���
            bool                    bDamageDown;			/// �� ������Ʈ���º��� �ٿ���������� ��Ÿ��
			bool					bNotUseLandConnect;
#ifdef SEASON3_MONSTER_2010_12
			D3DXVECTOR3				m_vReflectMagic;	// x: �ߵ����۽ð�, y:�����ð�, z:Ȯ���̳�, �յ��Ǵܵ��� ���� extra value(���������)
#endif
#ifdef SECRET_VELDER
			D3DXVECTOR2				m_vInvisibleTime;	/// Ư�� ���¿��� ���� �ð����� ����ȭ ����(������ ���� ���� ����)
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


			StateParam()
			{
				Init();
			}
			bool Verify()
			{
				if( bCanPushUnit.Verify()			== false 
					|| bCanPassUnit.Verify()		== false 
					|| invincibleTime.Verify()		== false 
					|| bInvincible.Verify()			== false
					|| bSuperArmor.Verify()			== false
					|| fRevengeStartTime.Verify()	== false
					|| fRevengeEndTime.Verify()		== false 
					|| fReflexMagicStartTime.Verify() == false
					|| fReflexMagicEndTime.Verify() == false )
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef ADD_CAN_PASS_ALLY_UNIT	//JHKang
					|| bCanPassAllyUnit.Verify()	== false
#endif //ADD_CAN_PASS_ALLY_UNIT
					|| m_DefenceTime.Verify()		== false
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				{
					return false;
				}
				return true;
			}
			StateParam& operator=( const StateParam& data )
			{
				bLandConnect			= data.bLandConnect;
				bCanPushUnit			= data.bCanPushUnit;
				bCanPassUnit			= data.bCanPassUnit;
				invincibleTime.m_fX		= data.invincibleTime.m_fX;
				invincibleTime.m_fY		= data.invincibleTime.m_fY;
				bInvincible				= data.bInvincible;
				bSuperArmor				= data.bSuperArmor;
				fRevengeStartTime		= data.fRevengeStartTime;
				fRevengeEndTime			= data.fRevengeEndTime;
				normalCamera			= data.normalCamera;
				hitCamera				= data.hitCamera;
				bResetCamera			= data.bResetCamera;
                bSuperArmorNotRed       = data.bSuperArmorNotRed;
                fGuardDefence           = data.fGuardDefence;
				bAttackState			= data.bAttackState;
#ifdef SERV_EVE_BATTLE_SERAPH
				bNoChangSpectro			= data.bNoChangSpectro;
#endif

				fReflexMagicStartTime	= data.fReflexMagicStartTime;
				fReflexMagicEndTime		= data.fReflexMagicEndTime;


				memcpy( bEventFlagList, data.bEventFlagList, sizeof(bool) * EVENT_FLAG_LIST_NUM );

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SUPER_ARMOR_TIME
				m_vecSuperArmorTime		= data.m_vecSuperArmorTime;
#endif

#ifdef ARA_CHARACTER_BASE
				m_usSoundSet			= data.m_usSoundSet;
#endif

#ifdef ADD_CAN_PASS_ALLY_UNIT	//JHKang
				bCanPassAllyUnit		= data.bCanPassAllyUnit;
#endif //ADD_CAN_PASS_ALLY_UNIT
				bAllowDirChange			= data.bAllowDirChange;
				m_DefenceTime.x			= data.m_DefenceTime.x;
				m_DefenceTime.y			= data.m_DefenceTime.y;
				m_DefenceTime.z			= data.m_DefenceTime.z;
				afterImage				= data.afterImage;
				m_mapAddSlashTrace		= data.m_mapAddSlashTrace;
				fSlashTraceTipWide		= data.fSlashTraceTipWide;
				bApplyAnimMove			= data.bApplyAnimMove;
				bFallDown				= data.bFallDown;
                bGuardFront             = data.bGuardFront;
				bGuardBack				= data.bGuardBack;
                bDamageDown             = data.bDamageDown;
				bNotUseLandConnect		= data.bNotUseLandConnect;
#ifdef SECRET_VELDER
				m_vInvisibleTime		= data.m_vInvisibleTime;
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


				return *this;
			}
			void Init()
			{
				bLandConnect			= true;
				bCanPushUnit			= true;
				bCanPassUnit			= false;
				bInvincible				= false;
				bSuperArmor				= false;
				fRevengeStartTime		= -1.0f;
				fRevengeEndTime			= -1.0f;
				normalCamera			= 0;
				hitCamera				= 0;
				bResetCamera			= true;
                bSuperArmorNotRed       = false;
                fGuardDefence           = 0;
				bAttackState			= false;
#ifdef SERV_EVE_BATTLE_SERAPH
				bNoChangSpectro			= false;
#endif

				fReflexMagicStartTime	= -1.f;
				fReflexMagicEndTime		= -1.f;

				ZeroMemory( bEventFlagList, sizeof(bool) * EVENT_FLAG_LIST_NUM );

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SUPER_ARMOR_TIME
				m_vecSuperArmorTime.clear();
#endif

#ifdef ARA_CHARACTER_BASE
				m_usSoundSet = 0;
#endif

#ifdef ADD_CAN_PASS_ALLY_UNIT	//JHKang
				bCanPassAllyUnit		= true;
#endif //ADD_CAN_PASS_ALLY_UNIT
				bAllowDirChange			= true;
				INIT_VECTOR2( afterImage, 0.0f, 0.0f );		
				m_mapAddSlashTrace.clear();
				fSlashTraceTipWide		= 4.0f;
				bApplyAnimMove			= true;
				bFallDown				= true;
                bGuardFront             = true;
				bGuardBack				= true;
                bDamageDown             = true;
				bNotUseLandConnect = false;
#ifdef SEASON3_MONSTER_2010_12
				m_vReflectMagic = D3DXVECTOR3(0.f, 0.f, 0.f);
#endif
#ifdef SECRET_VELDER
				INIT_VECTOR2( m_vInvisibleTime, 0.0f, 0.0f );
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


			}
		};

//-------------------------------------------------------------------------------------------------------

		struct UnitCondition	//��Ȳ�� ��Ÿ���� �Ķ����
		{
			char			nextState;
			char			nextSubState;
			float			fStateTime;			/// ������Ʈ �������� ���� ����� �ð�
			float			fStateTimeBack;		/// ������Ʈ �������� ���� ���� �����ӱ��� ����� �ð�
			bool			bTransitionChange;

			float			fUnitWidth;	
			float			fUnitHeight;	

			D3DXVECTOR3		dirDegree;
			D3DXVECTOR3		dirVector;

			float			fStopTime;
			float			fStop2Time;

			//bool			bFall;
			bool			bFootOnLine;

//{{ kimhc // 2010.7.7 // ���Ͱ� ������ ���θ�
#ifdef	LINE_MAP_CREATED_BY_MONSTER
			bool			bTempLineMap;
#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.7 // ���Ͱ� ������ ���θ�

//#ifdef STEP_ON_MONSTER_TEST
//			bool			bFootOnUnit;
//#endif STEP_ON_MONSTER_TEST
			D3DXVECTOR3		landPosition;

			bool			bHit;
			bool			bAttackerFront;

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
			bool			bFrameBufferPass;	/// ���� �������� m_ReceiveSyncDataList.size()�� 0�̸� true
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            union
            {
                KTWO_PARTS  stateChangeParts;
                char		stateChangeNum;
            };
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			char			stateChangeNum;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef WALL_JUMP_TEST
			bool			bFootOnWall;
#endif WALL_JUMP_TEST




#ifdef GRAPPLING_TEST
			GrapplingState m_GrapplingState;
#endif GRAPPLING_TEST



			UnitCondition()
			{
				Init();
			}
			void Init()
			{
				nextState					= -1;
				nextSubState				= -1;
				fStateTime					= 0.0f;
				fStateTimeBack				= 0.0f;
				bTransitionChange			= true;

				fUnitWidth					= 0.0f;
				fUnitHeight					= 0.0f;

				INIT_VECTOR3( dirDegree,	0,0,0 );
				INIT_VECTOR3( dirVector,	0,0,0 );

				fStopTime					= 0.0f;
				fStop2Time					= 0.0f;

				//bFall						= false;
				bFootOnLine					= true;

				//{{ kimhc // 2010.7.7 // ���Ͱ� ������ ���θ�
#ifdef	LINE_MAP_CREATED_BY_MONSTER
				bTempLineMap				= false;
#endif	LINE_MAP_CREATED_BY_MONSTER
				//}} kimhc // 2010.7.7 // ���Ͱ� ������ ���θ�

//#ifdef STEP_ON_MONSTER_TEST
//				bFootOnUnit					= false;
//#endif STEP_ON_MONSTER_TEST

				landPosition				= D3DXVECTOR3(0.0f,0.0f,0.0f);

				bHit						= false;
				bAttackerFront				= true;

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				bFrameBufferPass			= false;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
				stateChangeNum				= 0;

#ifdef WALL_JUMP_TEST
				bFootOnWall					= false;
#endif WALL_JUMP_TEST

			}
		};

//-------------------------------------------------------------------------------------------------------

		struct PhysicParam
		{
			D3DXVECTOR2		nowSpeed;		/// ���� ���ǵ�X,Y
			D3DXVECTOR2		passiveSpeed;	/// �� ���ǵ�� ��� �������ش�
			float			fDownAccel;		/// �ϰ� ���ӵ�

			float			fReloadAccel;	/// ���� �ӵ� ������

			float			fGAccel;		/// �߷� ���ӵ�
			float			fMaxGSpeed;		/// �ִ� �ϰ� �ӵ� Y

			float			fWalkSpeed;		/// �ȱ� �ӵ�
			float			fRunSpeed;		/// �޸��� �ӵ�
			float			fJumpSpeed;		/// �Ϲ� ���� ��¼ӵ�
			float			fDashJumpSpeed;	/// ��� ���� ��¼ӵ�

//			float			fSpeedFactor;	/// �ȱ�, �޸���, ����, ��� ���� �ӵ��� ��������. fix!! �ٵ� ���� ������ �̵��ӵ����� ����ǵ��� �Ǿ��ִ�??

// 			SPEED_FACTOR 	m_MoveSpeedFactor;	/// �����ð� ���� �ȱ�, �ٱ� �ӵ��� ����
// 			SPEED_FACTOR 	m_JumpSpeedFactor;	/// �����ð� ���� ����, ��� ���� �ӵ��� ����
// 
// #ifdef DUNGEON_ITEM
// 			SPEED_FACTOR	m_MoveSpeedByItem;
// 			SPEED_FACTOR	m_JumpSpeedByItem;
// #endif

// #ifdef SERV_SKILL_NOTE
// 			std::vector<SPEED_FACTOR> m_vecAnimSpeedFactor;
// #endif
// 			std::vector<SPEED_FACTOR> m_vecMoveSpeedFactor;
// 			std::vector<SPEED_FACTOR> m_vecJumpSpeedFactor;
			
			PhysicParam()
			{
				INIT_VECTOR2( nowSpeed, 0.0f, 0.0f );
				INIT_VECTOR2( passiveSpeed, -1.0f, -1.0f );
				fDownAccel		= 0.0f;

				fReloadAccel	= 0.0f;

				fGAccel			= 0.0f;
				fMaxGSpeed		= 0.0f;

				fWalkSpeed		= 0.0f;
				fRunSpeed		= 0.0f;
				fJumpSpeed		= 0.0f;
				fDashJumpSpeed	= 0.0f;
				
//				fSpeedFactor	= 1.f;

// #ifdef SERV_SKILL_NOTE
// 				m_vecAnimSpeedFactor.clear();				
// #endif
// 				m_vecMoveSpeedFactor.clear();
// 				m_vecJumpSpeedFactor.clear();
			}

			void OnFrameMove( double fTime, float fElapsedTime )
			{
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					m_MoveSpeedFactor.m_fDuration -= fElapsedTime;
// 				}
// 
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					m_JumpSpeedFactor.m_fDuration -= fElapsedTime;
// 				}
// #ifdef DUNGEON_ITEM
// 				if( m_MoveSpeedByItem.m_fDuration > 0.f )
// 				{
// 					m_MoveSpeedByItem.m_fDuration -= fElapsedTime;
// 				}
// 				if( m_JumpSpeedByItem.m_fDuration > 0.f )
// 				{
// 					m_JumpSpeedByItem.m_fDuration -= fElapsedTime;
// 				}
// #endif
			}

			float GetWalkSpeed() const
			{
				return fWalkSpeed;
// 				float fMoveSpeed = 1.f;
// 				if( m_MoveSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fMoveSpeed = m_MoveSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fMoveSpeed *= GetVecMoveSpeed();
// 
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fWalkSpeed * fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate * fMoveSpeed;
// 				}
// 				else
// 				{
// 					return fWalkSpeed * fSpeedFactor * fMoveSpeed;
// 				}
			}

			float GetDashSpeed() const
			{
				return fRunSpeed;
// #ifdef DUNGEON_ITEM				
// 				float fMoveSpeed = 1.f;
// 				if( m_MoveSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fMoveSpeed = m_MoveSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fMoveSpeed *= GetVecMoveSpeed();
// 
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fRunSpeed * fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate * fMoveSpeed;
// 				}
// 				else
// 				{
// 					return fRunSpeed * fSpeedFactor * fMoveSpeed;
// 				}
// #else
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fRunSpeed * fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					return fRunSpeed * fSpeedFactor;
// 				}
// #endif				
			}

			float GetJumpSpeed() const
			{
				return fJumpSpeed;
// #ifdef DUNGEON_ITEM		
// 				float fJumpSpeedTemp = 1.f;
// 				if( m_JumpSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fJumpSpeedTemp *= m_JumpSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fJumpSpeedTemp *= GetVecJumpSpeed();
// 
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fJumpSpeed * m_JumpSpeedFactor.m_fSpeedRate * fJumpSpeedTemp;
// 				}
// 				else
// 				{
// 					return fJumpSpeed * fJumpSpeedTemp;
// 				}
// #else
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fJumpSpeed * m_JumpSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					return fJumpSpeed;
// 				}
// #endif
				
			};

			float GetDashJumpSpeed() const
			{
				return fDashJumpSpeed;
// #ifdef DUNGEON_ITEM				
// 				float fJumpSpeedTemp = 1.f;
// 				if( m_JumpSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fJumpSpeedTemp *= m_JumpSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fJumpSpeedTemp *= GetVecJumpSpeed();
// 
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fDashJumpSpeed * m_JumpSpeedFactor.m_fSpeedRate * fJumpSpeedTemp;
// 				}
// 				else
// 				{
// 					return fDashJumpSpeed * fJumpSpeedTemp;
// 				}
// #else
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					return fDashJumpSpeed * m_JumpSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					return fDashJumpSpeed;
// 				}
// #endif				
			}

#ifdef SERV_SKILL_NOTE
			float GetAnimSpeed()
			{
				return 1.0f;
// 				float fAnimSpeed = 1.f;
// 				for(UINT i=0; i<m_vecAnimSpeedFactor.size(); ++i)
// 				{
// 					if(m_vecAnimSpeedFactor[i].m_fDuration > 0.f)
// 					{
// 						fAnimSpeed *= m_vecAnimSpeedFactor[i].m_fSpeedRate;
// 					}
// 				}
// 
// 				return fAnimSpeed;
			}
#endif

			float GetVecMoveSpeed()
			{
				return 1.0f;
// 				float fMoveSpeed = 1.f;
// 				for(UINT i=0; i<m_vecMoveSpeedFactor.size(); ++i)
// 				{
// 					if(m_vecMoveSpeedFactor[i].m_fDuration > 0.f)
// 					{
// 						fMoveSpeed *= m_vecMoveSpeedFactor[i].m_fSpeedRate;
// 					}
// 				}
// 
// 				return fMoveSpeed;
			}

			float GetVecJumpSpeed()
			{
				return 1.0f;
// 				float fJumpSpeed = 1.f;
// 				for(UINT i=0; i<m_vecJumpSpeedFactor.size(); ++i)
// 				{
// 					if(m_vecJumpSpeedFactor[i].m_fDuration > 0.f)
// 					{
// 						fJumpSpeed *= m_vecJumpSpeedFactor[i].m_fSpeedRate;
// 					}
// 				}
// 
// 				return fJumpSpeed;
			}

			float GetMoveSpeedPower()
			{
				return 1.0f;
// #ifdef DUNGEON_ITEM			
// 				float fMoveSpeed = 1.f;
// 				if( m_MoveSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fMoveSpeed = m_MoveSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fMoveSpeed *= GetVecMoveSpeed();
// 
// 				float fRetSpeed = 1.f;
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					fRetSpeed = fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate * fMoveSpeed;
// 				}
// 				else
// 				{
// 					fRetSpeed = fSpeedFactor * fMoveSpeed;
// 				}
// #else
// 				if( m_MoveSpeedFactor.m_fDuration > 0.f )
// 				{
// 					fRetSpeed = fSpeedFactor * m_MoveSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					fRetSpeed = fSpeedFactor;
// 				}
// #endif		
// 				if( fRetSpeed <= 0.f )
// 					fRetSpeed = 1.f;
// 
// 				return fRetSpeed;
			}

			float GetJumpSpeedPower()
			{
				return 1.0f;
// #ifdef DUNGEON_ITEM		
// 				float fJumpSpeedTemp = 1.f;
// 				if( m_JumpSpeedByItem.m_fDuration > 0.f)
// 				{					
// 					fJumpSpeedTemp *= m_JumpSpeedByItem.m_fSpeedRate;
// 				}
// 
// 				fJumpSpeedTemp *= GetVecJumpSpeed();
// 
// 				float fRetSpeed = 1.f;
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					fRetSpeed = m_JumpSpeedFactor.m_fSpeedRate * fJumpSpeedTemp;
// 				}
// 				else
// 				{
// 					fRetSpeed = fJumpSpeedTemp;
// 				}
// #else
// 				if( m_JumpSpeedFactor.m_fDuration > 0.f )
// 				{
// 					fRetSpeed = m_JumpSpeedFactor.m_fSpeedRate;
// 				}
// 				else
// 				{
// 					return 1.f;
// 				}
// #endif
// 				if( fRetSpeed <= 0.f )
// 					fRetSpeed = 1.f;
// 
// 				return fRetSpeed;			
			}
		};

//-------------------------------------------------------------------------------------------------------
		struct SoundPlayData 
		{
			float			m_SoundPlayTime;		/// ���� �ð�
			wstring			m_SoundPlayName;		/// ���ϸ�
			int				m_SoundPlayRate;		/// ����?
			bool			m_bOnlyIfMyUnit;		/// 
			float			m_fMaxSoundDistance;	/// ���� �ִ� �Ÿ�
			bool			m_b3DSound;

            SoundPlayData()
              : m_SoundPlayTime( -1.f )
              , m_SoundPlayName()
              , m_SoundPlayRate( 100 )
              , m_bOnlyIfMyUnit( false )
              , m_fMaxSoundDistance( -1.f )
              , m_b3DSound( true ) 
            {
            }
		};

//-------------------------------------------------------------------------------------------------------
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		struct ParticleEffectData
		{
			bool		m_bUse;
			bool		m_bMajor;
			float		m_fTime;
			wstring		m_Name;
			bool		m_bWeapon;
			wstring		m_Pos;
			bool		m_StateEndDelete;
			bool		m_bTrace;
			D3DXVECTOR3 m_vOffset;
			bool		m_bApplyUnitRotation;
			D3DXVECTOR3	m_vRotation;
            D3DXVECTOR3 m_vScale;
			bool		m_bUnitScale;
			
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;

			ParticleEffectData()
			{
				m_bUse				= false;
				m_bMajor			= false;
				m_fTime				= 0.0f;
				m_bWeapon			= false;
				m_StateEndDelete	= false;
				m_hSeq				= INVALID_PARTICLE_SEQUENCE_HANDLE;
				m_bTrace			= true;
				m_vOffset			= D3DXVECTOR3(0, 0, 0);
				m_bApplyUnitRotation = false;
				m_vRotation			= D3DXVECTOR3(0, 0, 0);
                m_vScale            = D3DXVECTOR3(1, 1, 1);
				m_bUnitScale		= false;

			}
			~ParticleEffectData();

#ifdef BOOST_SINGLETON_POOL_TEST
			void* operator new(size_t s);
			void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST

		};

//-------------------------------------------------------------------------------------------------------
		struct MeshPlayerEffectData
		{
			bool		m_bUse;
			bool		m_bMajor;
			float		m_fTime;
			wstring		m_Name;
			bool		m_bWeapon;
			wstring		m_Pos;
			bool		m_StateEndDelete;
			bool		m_Trace;

			CKTDGXMeshPlayer::CXMeshInstanceHandle m_hMeshInst;

			MeshPlayerEffectData()
			{
				m_bUse				= false;
				m_bMajor			= false;
				m_fTime				= 0.0f;
				m_bWeapon			= false;
				m_StateEndDelete	= false;
				m_Trace				= false;
				m_hMeshInst			= INVALID_MESH_INSTANCE_HANDLE;
			}
			~MeshPlayerEffectData();

#ifdef BOOST_SINGLETON_POOL_TEST
			void* operator new(size_t s);
			void operator delete(void *p);
#endif BOOST_SINGLETON_POOL_TEST


		};

//-------------------------------------------------------------------------------------------------------
		struct CameraCrashData
		{
			float		m_fTime;
			float		m_fGap;
			float		m_CrashTime;
			bool		m_bOnlyFocus;

			CameraCrashData()
			{
				m_fTime			= 0.0f;
				m_fGap			= 0.0f;
				m_CrashTime		= 0.0f;
				m_bOnlyFocus	= false;
			}
		};
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//-------------------------------------------------------------------------------------------------------
		struct LeapData
		{
			D3DXVECTOR3										m_StartPosToLeap;
			D3DXVECTOR3										m_ArrivalPos;
			D3DXVECTOR3										m_DirVecToLeap;
			float											m_fVerticalSpeedToLeap;
			float											m_fParallelSpeedToLeap;
			float											m_fHeightToLeap;
			int												m_DegreeToLeap;
			float											m_fTimeToLeap;
			float											m_fElapsedTimeToLeap;

			LeapData()
			{
				m_StartPosToLeap				= D3DXVECTOR3(0,0,0);
				m_ArrivalPos					= D3DXVECTOR3(0,0,0);
				m_DirVecToLeap					= D3DXVECTOR3(0,0,0);
				m_fVerticalSpeedToLeap			= 0.0f;
				m_fParallelSpeedToLeap			= 0.0f;
				m_fHeightToLeap					= 0.0f;
				m_DegreeToLeap					= 0;
				m_fTimeToLeap					= 0.0f;
				m_fElapsedTimeToLeap			= 0.0f;
			}
		};

//-------------------------------------------------------------------------------------------------------

		struct WeaponParticle
		{
			wstring		m_wstrBoneName;
			bool		m_bIsMajorParticle;
			CKTDGParticleSystem::CParticleEventSequenceHandle  m_hSeq;

			WeaponParticle();
			~WeaponParticle();
		};

//-------------------------------------------------------------------------------------------------------

//{{ robobeg : 2008-10-28
        struct Weapon : public CKTDGObject
//}} robobeg : 2008-10-28
		{
			//{{ kimhc // NPC�� ������ Z_ENABLE ���� // 2009-07-07
		private:
			bool					m_bZEnable;		// NPC ��ũ��Ʈ ������ �� WEAPON ���̺��� Z_ENABLE ���θ� ��� ����
			//}} kimhc // NPC�� ������ Z_ENABLE ���� // 2009-07-07

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			// û�� �ý��� �� ����ȭ ��忡�� ���⸦ ��ȯ�ϴ� ��ɰ� �����Ͽ� �߰���
			// �������� SlashTrace, EnchantData���� ������ m_puserWeaponEquip�� ���ο��� ����������
			// pItemTempletOrigin�� NULL �� �ƴ� ��쿡�� �̰����� ���������� �Ͽ�, Item������ itemTemplet����,
			// �޽� ������ m_pUserWeaponEquip���� ���� ������ ���� �ֵ��� ��
			const CX2Item::ItemTemplet*  m_pItemTempletOrigin;	/// SlashTrace, EnchantData� ���� ������ ������ ItemTemplet
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		public:

//{{ robobeg : 2008-10-28
			//CX2GameUnit*			m_pOwnerUnit;
//}} robobeg : 2008-10-28

			int						m_iWeaponIndex;
            /// ���������� ����ϴ� ��ü�̹Ƿ� smart pointer�� ������� �ʴ´�.
            /// - jintaeks on 2009-01-12, 17:36
			CKTDGXSkinAfterImage*	m_pAfterImage[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

			CX2EqipPtr				m_pUserWeaponEquip;		// guuser�� �̰��� ����ϰ�

			//{{ kimhc // NPC�� �����
            /// ���������� �����ϴ� ��ü�̹Ƿ� smart pointer�� ������� �ʴ´�.
            /// - jintaeks on 2009-01-12, 16:22
			CKTDGXSkinAnim*			m_pXSkinWeapon;			// gunpc�� �̰��� ����Ѵ�
			D3DXMATRIX*				m_pMatrixWeapon;
			D3DXVECTOR3				m_WeaponRotation;
			D3DXVECTOR3				m_WeaponScale;
			//}} kimhc // NPC�� �����

			
			// slash trace ���� ������
			bool					m_bUseSlashTrace;
			//wstring					m_SlashTraceTopBone;
			//wstring					m_SlashTraceBottomBone;
			CKTDGSlashTrace*		m_pSlashTrace[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
			CKTDGSlashTrace*		m_pSlashTraceTip[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
#ifdef ARA_CHARACTER_BASE
			CKTDGSlashTrace*		m_pSlashTraceTip2[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
#endif
#ifdef CUSTOM_SLASH_TRACE_TEXTURE
			CKTDGSlashTrace*		m_pSlashTraceCustomTexture[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];
#endif // CUSTOM_SLASH_TRACE_TEXTURE

			D3DXCOLOR				m_coSlashTrace;
			D3DXCOLOR				m_coSlashTraceHyper;
			D3DXCOLOR				m_coSlashTraceTip;
			D3DXCOLOR				m_coSlashTraceTipHyper;
			
			CKTDXDeviceXSkinMesh::MultiAnimFrame*	m_pFrame_TRACE_START[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];		// slash trace�� �ٴ� frame
			CKTDXDeviceXSkinMesh::MultiAnimFrame*	m_pFrame_TRACE_END[ CX2Item::MAX_MODEL_COUNT_A_ITEM ];

			vector<WeaponParticle*>					m_vecpWeaponParticle;

			CKTDGParticleSystem::CParticleEventSequenceHandle	m_ahEnchantWeapon[3];

//{{ robobeg : 2008-10-28
			//bool					m_bAlphaObject;
//}} robobeg : 2008-10-28
			bool                    m_bEnable;
			//vector<wstring>         m_vecAttackBoxName;

		private:
			//Weapon() { ASSERT( !"invalid" ); }
//{{ robobeg : 2008-10-28
			Weapon( CX2GameUnit& refInOwnerUnit_ );
//}} robobeg : 2008-10-28

//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			Weapon( CX2GameUnit& refInOwnerUnit_, 
                const CX2Item::ItemTemplet* pItemTempletOrigin_ 
                );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
			~Weapon();

			bool					m_bTraceColor;
			D3DXCOLOR				m_vTraceColor;
			D3DXCOLOR				m_vTraceTipColor;

			//{{ JHKang / 2011.03.23 / Blend �� ����
#ifdef ADD_WEAPON_BLEND_VALUE
			D3DBLEND	m_d3dSrcBlend;
			D3DBLEND	m_d3dDestBlend;
#endif

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// ���� ��ȭ ����Ʈ ��ü
			CKTDXDeviceXSkinMesh::MultiAnimFrame*				m_pFrame_ATTACK_LINE_END0;
			CKTDXDeviceXSkinMesh::MultiAnimFrame*				m_pFrame_ATTACK_LINE_START0_FRONT;

			CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqUpgradeWeapon;
			CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqUpgradeWeapon2;
			CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqUpgradeWeapon3;
			CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqUpgradeWeapon4;
#endif ADD_UPGRADE_WEAPON_PARTICLE
			//}}

		//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
		private:
	#ifdef ARA_CHARACTER_BASE
			void InitWeapon( CX2Unit::UNIT_TYPE eUnitType_ = CX2Unit::UT_NONE );
	#else 
			void InitWeapon();
	#endif //ARA_CHARACTER_BASE
		//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG

		public:
			static Weapon* CreateWeapon( CX2GameUnit& refInOwnerUnit_ ) { return new Weapon( refInOwnerUnit_ ); }
//{{ kimhc // 2010.12.14 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
			static Weapon* CreateWeapon( CX2GameUnit& refInOwnerUnit_, 
                const CX2Item::ItemTemplet* pItemTempletOrigin_ 
                )
			{
				return new Weapon( refInOwnerUnit_, pItemTempletOrigin_ );
			}
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.12.14 //  2010-12-23 New Character CHUNG
		


			bool IsTraceColor() { return m_bTraceColor; }
			void SetTraceColor(bool val, D3DXCOLOR vColor, D3DXCOLOR vTipColor) 
			{ 
				m_bTraceColor = val; 
				m_vTraceColor = vColor; 
				m_vTraceTipColor = vTipColor; 
			}
			

//{{ robobeg : 2008-10-28
            CX2GameUnit& GetOwnerUnit() { return *static_cast<CX2GameUnit*>( GetManagerObject() ); }
//}} robobeg : 2008-10-28


			// @iModelIndex: �� ��° �޽�����
			D3DXVECTOR3 GetBonePos( const WCHAR* pBoneName, int iModelIndex );

			//{{ kimhc // 2011.1.17 // ������ Bone�� ��Ʈ���� ���� Trace �� (chung �ڵ� ����)
			const D3DXMATRIX* GetCombineMatrix( const WCHAR* pBoneName, int iModelIndex ) const;
			//}} kimhc // 2011.1.17 // ������ Bone�� ��Ʈ���� ���� Trace �� (chung �ڵ� ����)

//{{ robobeg : 2008-10-28
			//void SetShow( bool bShow )
            virtual void NotifyShowObjectChanged();
//}} robobeg : 2008-10-28

			void SetEnable( bool bEnable );

			bool LoadDataForUser( KLuaManager& luaManager );
			bool LoadDataForNPC( KLuaManager& luaManager );

//{{ robobeg : 2008-10-28

			virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
            virtual RENDER_HINT     OnFrameRender_Prepare();
			//void OnFrameRenderForNPC();
            virtual void                OnFrameRender_Draw();
//}} robobeg : 2008-10-28

			
			void AddSlashTraceByType( int iType, D3DXVECTOR3 up, D3DXVECTOR3 down, D3DXVECTOR3 tipDown, int iModelIndex );

			void CommonStateEnd();
			void CommonFrameMoveForNPC( double fTime, float fElapsedTime );
			void CommonFrameMoveForUser( double fTime, float fElapsedTime );
		
			void CreateEnchantParticle( const WCHAR* pName1, const WCHAR* pName2, const WCHAR* pName3 );
			void InitEnchantParticleForSword();
			void InitEnchantParticleForBow();
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			static void AppendEnchantParticleForSwordToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
			static void AppendEnchantParticleForBowToDeviceList( CKTDXDeviceDataList& listInOut_, CX2DamageManager::EXTRA_DAMAGE_TYPE eDamageType_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD

			void EnchantParticleFrameMoveForSword();
			void EnchantParticleFrameMoveForBow();
			void EnchantParticleFrameMoveForEve();
#ifdef ARA_CHARACTER_BASE
			void EnchantParticleFrameMoveForAra();
#endif
#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
			void EnchantParticleFrameMoveForAdd();
#endif //SERV_9TH_NEW_CHARACTER


			void SetEnchantParticleShow(bool bShow);

			//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ��� ��ü
#ifdef REAL_TIME_ELSWORD
			void DeleteEnchantParticle();
#endif REAL_TIME_ELSWORD
			//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ��� ��ü

			//{{ kimhc // NPC�� ������ Z_ENABLE ���� // 2009-07-07
			bool GetZEnable() const;
			void SetZEnable( bool bEnable );
			//}} kimhc // NPC�� ������ Z_ENABLE ���� // 2009-07-07
			void UpdateSlashTraceTextureName();

#ifdef FIX_SLASH_TRACE
			void InitSlashTrace();
#endif

#ifdef ADD_UPGRADE_WEAPON_PARTICLE		/// ��ȭ ���⿡ ���� ����Ʈ ��� ó��
			void InitUpgradeWeaponParticle();		/// ��ȭ ���� ����Ʈ �ʱ�ȭ
			void CreateUpgradeWeaponParticle( const WCHAR* pName1, const WCHAR* pName2 = NULL, const WCHAR* pName3 = NULL, const WCHAR* pName4 = NULL );	/// ��ȭ ���� ����Ʈ ����
			void ProcessUpgradeWeaponParticle();	/// ��ȭ ���� ����Ʈ ����
			void DeleteUpgradeWeaponParticle();		/// ��ȭ ���� ����Ʈ ����
#endif ADD_UPGRADE_WEAPON_PARTICLE
		};

//-------------------------------------------------------------------------------------------------------

		struct TimedCriticalBuff
		{
			float m_fTimeLeft;
			float m_fDamageRate;
			float m_fEffectiveRate; // ũ��Ƽ�� �ߵ� Ȯ�� ����ġ

			TimedCriticalBuff()
			{
				Init();
			}

			void Init()
			{
				m_fTimeLeft = 0.f;
				m_fDamageRate = 0.f;
				m_fEffectiveRate = 0.f;
			}

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				if( m_fTimeLeft > 0.f )
				{
					m_fTimeLeft -= fElapsedTime;
					if( m_fTimeLeft <= 0.f )
					{
						m_fTimeLeft = 0.f;
						m_fDamageRate = 0.f;
						m_fEffectiveRate = 0.f;
					}
				}
			}
		};

//-------------------------------------------------------------------------------------------------------

		struct TimedMagicDefenceDebuff
		{
			float m_fEffectiveTime;
			float m_fMagicDefenceRate;
			float m_fEffectiveRate; // ũ��Ƽ�� ������ �ߵ��� Ȯ��

			TimedMagicDefenceDebuff()
			{
				Init();
			}

			void Init()
			{
				m_fEffectiveTime = 0.f;
				m_fMagicDefenceRate = 0.f;
				m_fEffectiveRate = 0.f;
			}

		};

//-------------------------------------------------------------------------------------------------------

		struct SkillRelatedData
		{
			float m_fForceDownValueRateOfRenaArrowAttack;		// ������ ȭ����� �����ٿ� ��ġ ����
			float m_fForceDownValueRateOfRangeAttack;			// �߻�ü ������ �����ٿ� ��ġ ����
			float m_fForceDownValueRateOfMeleeAttack;			// melee ������ �����ٿ� ��ġ ����
			float m_fSkillCoolTimeLeft_SI_P_COMMON_SOFTBODY;
			int m_iNoDownDamageAtStandUpAttackPercent;
			float m_fStatAtkRateOfSummoned;						// ��ȯ�� ������ ���ݷ� ����

		public: 
			SkillRelatedData()
			{
				Init();
			}

			void Init()
			{
				m_fForceDownValueRateOfRenaArrowAttack = 1.f;
				m_fForceDownValueRateOfRangeAttack = 1.f;
				m_fForceDownValueRateOfMeleeAttack = 1.f;
				m_fSkillCoolTimeLeft_SI_P_COMMON_SOFTBODY = 0.f;
				m_iNoDownDamageAtStandUpAttackPercent = 0;
				m_fStatAtkRateOfSummoned = 1.f;
			}

			float GetForceDownValueRateOfRenaArrowAttack() const { return m_fForceDownValueRateOfRenaArrowAttack; }
			void SetForceDownValueRateOfRenaArrowAttack( float fForceDownValueRateOfRenaArrowAttack_ ) 
			{ 
				m_fForceDownValueRateOfRenaArrowAttack = fForceDownValueRateOfRenaArrowAttack_; 

				ASSERT( 0.f < m_fForceDownValueRateOfRenaArrowAttack && 1.f >= m_fForceDownValueRateOfRenaArrowAttack );

				if ( 0.f >= m_fForceDownValueRateOfRenaArrowAttack || 1.f < m_fForceDownValueRateOfRenaArrowAttack )
					m_fForceDownValueRateOfRenaArrowAttack = 1.f;
			}

			// ������ : TODO : ó���� SetDamageDataFromLUA �ܺο� �־, �Ϻ� ó������ �ʴ� ���� ����.(DAMAGE_DATA_NEXT, �������� ������ ��..) ���� �ʿ�
			float GetForceDownValueRateOfRangeAttack() const { return m_fForceDownValueRateOfRangeAttack; }
			void SetForceDownValueRateOfRangeAttack(float fForceDownValueRateOfRangeAttack_) 
			{ 
				m_fForceDownValueRateOfRangeAttack = fForceDownValueRateOfRangeAttack_; 

				ASSERT( 0.f < m_fForceDownValueRateOfRangeAttack && 1.f >= m_fForceDownValueRateOfRangeAttack );

				if ( 0.f >= m_fForceDownValueRateOfRangeAttack || 1.f < m_fForceDownValueRateOfRangeAttack )
					m_fForceDownValueRateOfRangeAttack = 1.f;
			}

			float GetForceDownValueRateOfMeleeAttack() const { return m_fForceDownValueRateOfMeleeAttack; }
			void SetForceDownValueRateOfMeleeAttack(float fForceDownValueRateOfMeleeAttack_) 
			{ 
				m_fForceDownValueRateOfMeleeAttack = fForceDownValueRateOfMeleeAttack_; 

				ASSERT( 0.f < m_fForceDownValueRateOfMeleeAttack && 1.f >= m_fForceDownValueRateOfMeleeAttack );

				if ( 0.f >= m_fForceDownValueRateOfMeleeAttack || 1.f < m_fForceDownValueRateOfMeleeAttack )
					m_fForceDownValueRateOfMeleeAttack = 1.f;
			}

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				KTDXPROFILE();
			}
		};

//-------------------------------------------------------------------------------------------------------

		struct CommonState
		{
			int m_Wait;
			//int m_Walk;
			//int m_Dash;

			//int m_GrabFront;			// �տ� �ִ� ���� ���
			//int m_PickUp;				// ���� �ִ� ���� �ݱ�
#ifdef GRAPPLING_TEST
			int m_GrappledFront;
			int m_GrappledBack;
#endif // GRAPPLING_TEST
			int m_Thrown;
			//int m_ThrownForward;
			//int m_ThrownUpward;



#ifdef CLIFF_CLIMBING_TEST
			int m_ClimbWait;
			int m_ClimbFront;
			int m_ClimbUp;
			int m_ClimbUpFront;
			int m_ClimbDown;
			int m_ClimbDownFront;
#endif CLIFF_CLIMBING_TEST


			int m_DownDamageLandFront;	
			int m_DownDamageLandBack;	
			int m_DownDamageAir;
			int m_DamageAirDownLanding;


			int m_LinkJumpState;


//#ifdef TRANSFORMER_TEST
//			int m_Transformed; // �������� ����
//#endif TRANSFORMER_TEST



			CommonState()
			: m_Wait( 0 )
			//, m_GrabFront( 0 )
			//, m_PickUp( 0 )
#ifdef GRAPPLING_TEST
			, m_GrappledFront( 0 )
			, m_GrappledBack( 0 )
#endif // GRAPPLING_TEST
			, m_Thrown( 0 )
			//, m_ThrownForward( 0 )
			//, m_ThrownUpward( 0 )
#ifdef CLIFF_CLIMBING_TEST
			, m_ClimbWait( 0 )
			, m_ClimbFront( 0 )
			, m_ClimbUp( 0 )
			, m_ClimbUpFront( 0 )
			, m_ClimbDown( 0 )
			, m_ClimbDownFront( 0 )
#endif CLIFF_CLIMBING_TEST
			, m_DownDamageLandFront( 0 )
			, m_DownDamageLandBack( 0 )
			, m_DownDamageAir( 0 )
			, m_DamageAirDownLanding( 0 )
			, m_LinkJumpState( 0 )
//#ifdef TRANSFORMER_TEST
//			, m_Transformed( 0 )
//#endif TRANSFORMER_TEST

			{
			}
		};

//-------------------------------------------------------------------------------------------------------

#ifdef MODIFY_EFFECT_SET_DELETE
		enum DELETE_CONDITION_EFFECT_SET
		{
			DCES_NONE = 0,
			DCES_DAMAGE_REACT,
			DCES_STATE_END,
			DCES_DIE,
			DCES_CUSTOM_STATE,
		};

		struct EffectSetToPlay
		{
			std::wstring m_wstrEffectSetName;
			float		m_fStartAnimationTime;

#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��
			vector<DELETE_CONDITION_EFFECT_SET> m_vecDeleteCondition;
#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��
			DELETE_CONDITION_EFFECT_SET m_eDeleteCondition;
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��

#ifdef ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��
			EffectSetToPlay()
				: m_wstrEffectSetName( L"" )
				, m_fStartAnimationTime( 0.f )
			{
				m_vecDeleteCondition.push_back( DCES_NONE );
			}

			EffectSetToPlay( const wstring& name, float fTime )
			{
				m_wstrEffectSetName = name;
				m_fStartAnimationTime = fTime;
				m_vecDeleteCondition.push_back( DCES_NONE );
			}
			
			EffectSetToPlay( const wstring& name, float fTime, vector<DELETE_CONDITION_EFFECT_SET> & vecDeleteCondition )
			{
				m_wstrEffectSetName = name;
				m_fStartAnimationTime = fTime;
				m_vecDeleteCondition.clear();
				m_vecDeleteCondition.resize( vecDeleteCondition.size() );
				std::copy ( vecDeleteCondition.begin(), vecDeleteCondition.end(), m_vecDeleteCondition.begin() );
			}

#else // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��
			EffectSetToPlay()
				: m_wstrEffectSetName( L"" )
				, m_fStartAnimationTime( 0.f )
				, m_eDeleteCondition( DCES_NONE )
			{}
			EffectSetToPlay( const wstring& name, float fTime, DELETE_CONDITION_EFFECT_SET eDeleteCondition = DCES_NONE )
			{
				m_wstrEffectSetName = name;
				m_fStartAnimationTime = fTime;
				m_eDeleteCondition = eDeleteCondition;
			}
#endif // ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
            bool    operator < ( const EffectSetToPlay& rhs_ ) const { return m_fStartAnimationTime < rhs_.m_fStartAnimationTime; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		};
#else // MODIFY_EFFECT_SET_DELETE
	// MODIFY_EFFECT_SET_DELETE ������ ������ �� ���~
	struct EffectSetToPlay
	{
		std::wstring m_wstrEffectSetName;
		float		m_fStartAnimationTime;

		EffectSetToPlay()
			: m_wstrEffectSetName( L"" )
			, m_fStartAnimationTime( 0.f )
		{
		}

		EffectSetToPlay( const wstring& name, float fTime )
		{
			m_wstrEffectSetName = name;
			m_fStartAnimationTime = fTime;
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        bool    operator < ( const EffectSetToPlay& rhs_ ) const { return m_fStartAnimationTime < rhs_.m_fStartAnimationTime; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	};
	// ~ MODIFY_EFFECT_SET_DELETE ������ ������ �� ���
#endif //MODIFY_EFFECT_SET_DELETE


//-------------------------------------------------------------------------------------------------------
#ifdef DAMAGE_EFFECT_GROUP_DAMAGE			/// ������ ����Ʈ �׷� ������ ���� ����� ����ü

		struct DamageEffectGroupData;
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        typedef boost::intrusive_ptr<DamageEffectGroupData> DamageEffectGroupDataPtr;
#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		typedef boost::shared_ptr<DamageEffectGroupData> DamageEffectGroupDataPtr;
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		struct DamageEffectGroupData
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            : private boost::noncopyable
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
		{
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
        private:
            unsigned                                        m_uRefCount;
        public:
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			std::wstring				m_wstrDamageEffectName;		/// �׷� ������ �̸�
			UidType						m_uidAttackerUID;			/// ������ ���� ���̵�
			CX2GameUnit::GAME_UNIT_TYPE m_eAttackerGameUnitType;	/// ������ ���� ���� Ÿ��
			float						m_fHitGFap;					/// �ǰ� ���� ( �ǰ� �������� ������ ���� ���θ� �˻��ϹǷ�, �ǰ� ������ 
																	///				���� ������ ����Ʈ�� �׷� �������� ���۵��� �ʴ´�. )

			DamageEffectGroupData()
				: m_wstrDamageEffectName( L"" )
				, m_uidAttackerUID( -1 )
				, m_fHitGFap( 0.f )
				, m_eAttackerGameUnitType( CX2GameUnit::GUT_USER )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                , m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			{
			}

			DamageEffectGroupData( const wstring& wstrDamageEffectName, UidType uidAttackerUID, 
								   CX2GameUnit::GAME_UNIT_TYPE eAttackerGameUnitType, float fHitGFap )
#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
                : m_uRefCount(0)
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
			{
				m_wstrDamageEffectName	= wstrDamageEffectName;
				m_uidAttackerUID		= uidAttackerUID;
				m_fHitGFap				= fHitGFap;
				m_eAttackerGameUnitType	= eAttackerGameUnitType;
			}

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
            void    AddRef()    {   ++m_uRefCount; }
            void    Release()   { if ( (--m_uRefCount) == 0 )   delete this; }
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

		};

#endif DAMAGE_EFFECT_GROUP_DAMAGE
//-------------------------------------------------------------------------------------------------------

	protected:
		CX2GameUnit( int unitIndex, int teamNum, const WCHAR* pScriptFileName );
		virtual ~CX2GameUnit(void);

#ifdef RIDING_SYSTEM
		bool CanNotShowEnchantedWeaponEffectToRiding();
#endif //RIDING_SYSTEM

//-------------------------------------------------------------------------------------------------------

		// Public Function Start
	public:
		static void					InitDevice( InitDeviceData& OutDevice_, KLuaManager& luaManager_ );
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void					InitComponent( InitDeviceData& OutDevice_, KLuaManager& luaManager_ );
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		static void                 InitWeaponDataForNPC( InitDeviceData& OutDevice_, KLuaManager& luaManager_ );

		virtual void				Init( bool bUseTeamPos = true, int startPosIndex = -1 )		= NULL;
		virtual void				ReInit( bool bUseTeamPos = true, int startPosIndex = -1 );
		virtual void				InitEffect();
		virtual void				DeleteMinorParticle();
		void						DeleteGameUnitMajorParticle();
		void						DeleteGameUnitMinorParticle();


		virtual HRESULT				OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT				OnCameraMove();

//{{ robobeg : 2008-10-28
		//virtual HRESULT				OnFrameRender()												= NULL;
        virtual void                OnFrameRender_Draw()                                        = NULL;
//}} robobeg : 2008-10-28

		virtual HRESULT				OnResetDevice()												= NULL;
		virtual HRESULT				OnLostDevice()												= NULL;

		virtual UidType				GetUnitUID() const { return m_UnitUID; }
		//virtual CX2Unit*			GetUnit() const { return NULL; }
		virtual CKTDGXSkinAnimPtr	GetSkinAnimPtr() const = NULL;
		//virtual void              StateChange( int state, bool bDirectChange = false )		= NULL;
        virtual bool                StateChange( int state, bool bDirectChange = false )		= NULL;
		virtual void				DamageReact( CX2DamageManager::DamageData* pDamageData )	= NULL;
		virtual void				AttackResult()												= NULL;
		
		//virtual CX2DamageManager::Damage SetAttackDamage( CX2DamageManager::DamageData* pDamageData ); // obsolete!!
		//virtual float				SetDefenceDamage( CX2DamageManager::DamageData* pDamageData, CX2DamageManager::Damage damage );		// ���Լ� ���� �뵵�� ����� ������ �𸣰���

		virtual float				GetItemAttackPower( CX2GameUnit* pAttacker, CX2Item* pItem = NULL, CX2DamageManager::DAMAGE_TYPE eDamageType = CX2DamageManager::DT_MIX );
		virtual float				GetFinalDamage( CX2DamageManager::DamageData* pAttDamageData );

#ifdef ELSWORD_WAY_OF_SWORD
		// GetFinalDamage ��� �� �� ���Ǳ� Ÿ�Կ� ���� ������ ���� ���� �ֱ� ���� �Լ�
		virtual float GetFinalDamage_WayOfSword( CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_ ){ return 1.f; } // �⺻ �� 1
		virtual void SetCriticalPercent_WayOfSword( IN CX2DamageManager::WAY_OF_SWORD_TYPE eWayOfSwordType_,
													OUT float& fCriticalPercent_) {}
#endif //ELSWORD_WAY_OF_SWORD

		void InitEnableDash();
		virtual void SetEnableDash( bool bEnableDash,  ENABLE_DASH_STATE eState = EDS_END );
		bool GetEnableDash( ENABLE_DASH_STATE eState = EDS_END );

		bool GetEntangled() const { return m_bEntangled; }
		void SetEntangled( const bool val );

#ifdef DUNGEON_ITEM
		float GetTimeToReturnBasicScale() { return m_fTimeToReturnBasicScale; }
#endif

		float						CalcExtraAttackPower( CX2DamageManager::DamageData* pAttDamageData, CX2GameUnit* pAttacker, CX2Stat* pAttackerStat );
		float 						CalcLevelFactorForFinalDamage( CX2GameUnit* pAttacker );



		virtual D3DXVECTOR3			GetBonePos( const WCHAR* pBoneName ) const						= NULL;
		
#ifdef MODIFY_GET_BONE_POS
		// ������ // 2012-12-17 // ���� ���� ��쿡 ���� üũ�ϱ� ���� OUT ���ڸ� ����ϴ� �Լ� �߰�
		virtual bool				GetBonePos( const WCHAR* pBoneName, OUT D3DXVECTOR3& pos_ ) const = NULL;
#endif //MODIFY_GET_BONE_POS

//{{ kimhc // 2011.1.17 // ������ Bone�� ��Ʈ���� ���� Trace �� (chung �ڵ� ����)
		virtual const D3DXMATRIX*	GetCombineMatrixFromBoneName( const wstring& wstrBoneName )	const = NULL;
		const D3DXMATRIX* GetCombineMatrixFromWeaponBoneName( int iWeaponIndex, const wstring& wstrWeaponBoneName, int iModelIndex ) const;
//}} kimhc // 2011.1.17 // ������ Bone�� ��Ʈ���� ���� Trace �� (chung �ڵ� ����)

#ifdef RIDING_SYSTEM
		virtual const D3DXMATRIX*	GetCombineMatrixFromRidingPetBoneName( const wstring& wstrBoneName );
#endif //RIDING_SYSTEM

		//virtual D3DXVECTOR3			GetWeaponBonePos( const WCHAR* pBoneName );
		virtual void				Win()														= NULL;
		virtual void				Lose()														= NULL;
		virtual void				Draw()														= NULL;

//{{ robobeg : 2008-10-21
		//virtual void				SetShow( bool bShow );
		//bool						GetShow() { return m_bShow; }
        virtual void                NotifyShowObjectChanged();
//}} robobeg : 2008-10-21
		void						SetShadow( const WCHAR* pTexName, float fSize );

		virtual bool				IsFullHyperMode() const { return false; }		/// 3�����ΰ�?
		virtual void				SetFullHyperMode( const bool bFullHyperMode_ ) {}

		virtual void				SetHyperModeCount( int i )	= NULL;
		virtual int					GetHyperModeCount() const	= NULL;
		virtual int					GetLastStateHyperModeCount() const = NULL;

		float						GetRemainHyperModeTime() const 
		{ 
			return ( NULL != GetGageData() ? GetGageData()->GetHyperModeRemainTime() : 0.0f );
		}

		void						SetRemainHyperModeTime( float fRemainHyperModeTime_ ) 
		{ 
			if ( NULL != GetGageData() )
				GetGageData()->SetHyperModeRemainTime( fRemainHyperModeTime_ ); 			
		}

		bool						VerifyRemainHyperModeTime() const 
		{ 
			return ( NULL != GetGageData() ? GetGageData()->VerifyHyperModeRemainTime() : true ); 
		} 

		float						GetMaxHyperModeTime() const { return m_fMaxHyperModeTime; }
		void						SetMaxHyperModeTime( float fMaxHyperModeTime_ ) { m_fMaxHyperModeTime = fMaxHyperModeTime_; };

		virtual		float			GetHyperModeTime() const { return GetMaxHyperModeTime(); }
		
		float GetForceHyperModeTime() const { return m_fForceHyperModeTime; }
		void SetForceHyperModeTime(float val) { m_fForceHyperModeTime = val; }
	

		bool						VerifyMaxHyperModeTime() const { return m_fMaxHyperModeTime.Verify(); }

		virtual float				GetFinalDamageToBeChangedBySkill( float fFinalDamage_ ) { return fFinalDamage_; }
#ifdef UPGRADE_SKILL_SYSTEM_2013 //��â��
		virtual float				GetFinalDamageToBeChangedByActiveSkill( float fFinalDamage_ ) { return fFinalDamage_; }
#endif //UPGRADE_SKILL_SYSTEM_2013
		virtual void				ApplyHyperModeBuff();
		virtual void				HyperModeBuffEffectStart() = NULL;
		virtual void				HyperModeBuffEffectEnd();
		virtual void				HyperModeBuffEffectOnFrameMove();

		GAME_UNIT_TYPE				GetGameUnitType()	const			{ return m_GameUnitType; }
		void						SetUnitIndex( int unitIndex )		{ m_UnitIndex = unitIndex; }
		int							GetUnitIndex()						{ return m_UnitIndex; }
		int							GetTeam() const							{ return m_TeamNum; }
		void						SetTeamNum( int val )				{ m_TeamNum = val; } 
#ifdef	SERV_TRAPPING_RANGER_TEST
		void SetAllyTeam( CX2Room::TEAM_NUM eAllyTeam )					{ m_eAllyTeam = eAllyTeam; }
		CX2Room::TEAM_NUM GetAllyTeam()	const								{ return m_eAllyTeam; }
#endif	SERV_TRAPPING_RANGER_TEST

//#ifdef PVP_BOSS_COMBAT_TEST
//		int GetTeamNumOriginal() const { return m_TeamNumOriginal; }
//#endif PVP_BOSS_COMBAT_TEST

		virtual CX2GageData*	CreateGageData();
		virtual CX2GageUI*		CreateGageUI( const CX2GageData* pOwnerGageData_ );

		bool IsNullGageUI() const { return ( NULL == m_pGageUI ); }
		void SetShowGage( const bool bShow_ ) 
		{ 
			if ( !IsNullGageUI() )
				m_pGageUI->SetShow( bShow_ ); 
		}
		
		void SetHpTexture( const WCHAR* pwszSmallHpBar_ );
		void SetMpTexture( const WCHAR* pwszSmallMpBar_ );

		void RenderGageUI()
		{
			if ( !IsNullGageUI() )
				m_pGageUI->OnFrameRender();
		}

		D3DXVECTOR3					GetImpactPos()	const			{ return m_DamageData.impactPoint; }
        		
		const CX2GageData*			GetGageData() const				{ return m_pGageData; }
		CX2GageData*				GetGageData()					{ return m_pGageData; }
		
		float						GetNowHp()	const					{ return ( NULL != GetGageData() ? GetGageData()->GetNowHp() : 1 ); }
		float						GetMaxHp()	const					{ return GetGageData()->GetMaxHp(); }
		void						SetMaxHp( const float maxHP )		{ GetGageData()->SetMaxHp( maxHP ); }

		float						GetNowMp()	const					{ return GetGageData()->GetNowMp(); }
		float						GetMaxMp()	const					{ return GetGageData()->GetMaxMp(); }
		void						SetMaxMp( const float maxMP )		{ GetGageData()->SetMaxMp( maxMP ); }

		bool						VerifyNowHp() { return GetGageData()->VerifyNowHp(); }

		bool						GetEnableHPRelativeChangeRate() const { return GetGageData()->GetEnableHPRelativeChangeRate(); }
		void						SetEnableHPRelativeChangeRate( const bool bHpRelativeChangeRate_ ) { GetGageData()->SetEnableHPRelativeChangeRate( bHpRelativeChangeRate_ ); }

		bool						FlushMp( const float fFlushMp_ )	{ return GetGageData()->FlushMp( fFlushMp_ ); }
		bool						DamageFlushMp() { return GetGageData()->DamageFlushMp(); }
		bool						FlushForceDown() { return GetGageData()->FlushForceDown(); }
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
        bool						DamageFlushMp_CheckOnly() { return GetGageData()->DamageFlushMp_CheckOnly(); }
        bool						FlushForceDown_CheckOnly() { return GetGageData()->FlushForceDown_CheckOnly(); }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
		
		bool						FlushSoul( const float fFlushSoul_ ) { return GetGageData()->FlushSoul( fFlushSoul_ ); }

		void AddMPFactor( const float factor, const float time ) { GetGageData()->AddMPFactor( factor, time ); }
		void ClearMPFactor() { GetGageData()->ClearMPFactor(); }


		void SetChangeRateHp( const float changeRate ) { GetGageData()->SetChangeRateHp( changeRate ); }
		void SetChangeRateMp( const float changeRate ) { GetGageData()->SetChangeRateMp( changeRate ); }
		void SetChangeRateChargeMpForDetonation( const float changeRate ) { GetGageData()->SetChangeRateChargeMpForDetonation( changeRate ); }
		void SetNowChargeMpForDetonation( const float mpCharge ) { GetGageData()->SetNowChargeMpForDetonation( mpCharge ); }
		
		float GetChangeRateMp() { return GetGageData()->GetChangeRateMp(); }
				
		// TODO : UpHP() �Լ��� ��ġ��
		void UpHPIfAlive( const float hpIncrement, const float hpMinimum = 0.f ) 
		{
			if( GetNowHp() <= 0.f )
				return; 

			UpNowHp( hpIncrement, hpMinimum );
		}

		// TODO: rename UpHP() -> ChangeHPByDamage() or something 
		void UpNowHp( const float hpIncrement, const float hpMinimum = 0.f ) { GetGageData()->UpNowHp( hpIncrement, hpMinimum ); }
		void UpNowMp( const float mpIncrement ) { GetGageData()->UpNowMp( mpIncrement ); }
		void UpNowSoul( const float soulIncrement ) { GetGageData()->UpNowSoul( soulIncrement ); }

		void SetNowHp( float fHPNow );
		void SetNowMp( float fMPNow )			{ GetGageData()->SetNowMp( fMPNow ); }

		float GetMaxSoul() const { return GetGageData()->GetMaxSoul(); }
		void SetMaxSoul( const float maxSoul ) { GetGageData()->SetMaxSoul( maxSoul ); }

		float GetNowSoul() const { return GetGageData()->GetNowSoul(); }
		void SetNowSoul( const float soul ) { GetGageData()->SetNowSoul( soul ); }

		float GetNowDetonationGage() { return GetGageData()->GetNowChargeMpForDetonation(); }
		
		void SetNowForceDown( const float value ) { GetGageData()->SetNowForceDown( value ); }
		void SetMaxForceDown( const float value ) { GetGageData()->SetMaxForceDown(value); }
		
		void SetShowMPGage( bool bShow ) { ASSERT( !L"�����ʿ�!" ); }//GetGageManager()->SetShowMPGage( bShow ); }
		bool GetShowMpGage() { ASSERT( !L"�����ʿ�!" ); return true; } //{ return GetGageManager()->GetShowMPGage(); }

		void SetChangeRateByItem( const float fRate_, const float fTime_ )
		{ 
			GetGageData()->SetChangeRateByItem( fRate_ );
			GetGageData()->SetChangeRateTimeByItem( fTime_ );
		}

		void AddForceDownGage( const float fChangeOfNowForceDown_ ) { GetGageData()->UpNowForceDown( fChangeOfNowForceDown_ ); }

		const PhysicParam&			GetPhysicParam() const	{ return m_PhysicParam; }
		PhysicParam&				AccessPhysicParam()		{ return m_PhysicParam; }
		void						SetEnableAttackBox( const WCHAR* pName, bool bEnable );
		bool						SetEnableAttackBox( const CKTDXCollision::CollisionDataListSet& setCollisionDataList_, const WCHAR* pName_, const bool bEnable_ );

		void						SetEnableCollisionBox( const WCHAR* pName, bool bEnable );

		float GetSphereAttackBoxScale( const wstring& attackBoxName );
		void SetSphereAttackBoxScale( const wstring& attackBoxName, float fScale );


		const wstring&				GetHeadBoneName() const				{ return m_HeadBoneName; }
		virtual D3DXVECTOR3			GetHeadBonePos() const				{ return D3DXVECTOR3(0,0,0); }

		virtual void				SetStopTime( float fStopTime )		= NULL;
		virtual void				SetStop2Time( float fStopTime )		= NULL;

		virtual float				GetStopTime() = NULL;

#ifdef STOP_UNIT_STOP_EFFECT_TEST

		// @fRangeSq : �Ÿ��� �����Դϴ�
		void SetEffectStopTime( float fTime, float fRangeSq = 1000.f );

#endif STOP_UNIT_STOP_EFFECT_TEST

		virtual int					GetNowStateID() const = NULL;
		float GetForceInvincible() { return m_fForceInvincibleTime; }
		float GetShowInvincible() { return m_fShowInvincibleTime; }
		virtual void				SetForceInvincible( float fTime ){ m_fForceInvincibleTime = fTime; }
		virtual void				SetShowInvincible( float fTime ){ m_fShowInvincibleTime = fTime; }

		virtual const UnitCondition& GetUnitCondition( bool bFuture = false ) const	= NULL;
		virtual const StateParam&	GetStateparam( bool bFuture = false ) const	= NULL;

		virtual int					GetLastTouchLineIndex( bool bFuture = false ) const = NULL;


		virtual void SetPositionOnLine( const D3DXVECTOR3& vPosition, const int iLineIndex ) = NULL;


		virtual const D3DXVECTOR3&	GetLandPos( bool bFuture = false ) const	= NULL;
		virtual const D3DXVECTOR3&	GetPos( bool bFuture = false ) const = NULL;
		virtual float				GetXPos( bool bFuture = false )	const	= NULL;
		virtual float				GetYPos( bool bFuture = false )	const	= NULL;
		virtual float				GetZPos( bool bFuture = false ) const		= NULL;
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        virtual void BackupPosition() = 0;
        const D3DXVECTOR3&          GetPositionBackup() { if ( m_bPositionBackup == false ) BackupPosition(); return m_vPositionBackup; }
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual void                OnFrameMove_PostProcess() = NULL;
        float                       GetElapsedTime() const              { return m_fElapsedTime; }
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		virtual const D3DXVECTOR3&	GetRotateDegree() const				= NULL;
		virtual float				GetXRotateDegree() const			= NULL;
		virtual float				GetYRotateDegree() const					= NULL;
		virtual float				GetZRotateDegree() const					= NULL;

		virtual float				GetPlaySpeed() const				= NULL;

		D3DXVECTOR3			        GetZVector() const
		{
			D3DXVECTOR3 vDirVector = GetDirVector();
			if( vDirVector.y != 0.f )
			{
				vDirVector.y = vDirVector.z;
				vDirVector.z = vDirVector.x;
				vDirVector.x = -vDirVector.y;
				vDirVector.y = 0.f;
				D3DXVec3Normalize( &vDirVector, &vDirVector );
			}
			else
			{
				vDirVector.y = vDirVector.z;
				vDirVector.z = vDirVector.x;
				vDirVector.x = -vDirVector.y;
				vDirVector.y = 0.f;
			}
			return vDirVector;
		}

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual UINT    GetSizeOfAttackTimeList() const = NULL;
        virtual const D3DXVECTOR2* const GetAttackTimeByIndex( const UINT uiIndex_ ) const = NULL;
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		UINT GetSizeOfAttackTimeList() const { return m_AttackTimeList.size(); }
		const D3DXVECTOR2* const GetAttackTimeByIndex( const UINT uiIndex_ )
		{
			if ( uiIndex_ < m_AttackTimeList.size() )
				return &m_AttackTimeList[uiIndex_];
			else
				return NULL;
		}
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		virtual const D3DXVECTOR3&	GetDirVector() const						= NULL;
		virtual bool				GetIsRight() const						= NULL;
	
		virtual GAME_UNIT_STATE_ID	GetGameUnitState() const			= NULL;
		virtual bool				GetInvincible()	const				= NULL;
		virtual bool				GetIsFallDownState()				= NULL;

		virtual float				GetUnitWidth( bool bFuture ) const	= NULL;
		virtual float				GetUnitHeight( bool bFuture ) const	= NULL;

		bool						GetDyingStart() const { return m_bDyingStart; }

		virtual bool				IsMyUnit()	const		= NULL;
		virtual bool				IsLocalUnit() const		= NULL;
		virtual bool				IsFocusUnit() const		= NULL;
		virtual void				RenderName()		= NULL;

		void						DeleteComboManager() { SAFE_DELETE( m_pComboManager ); }
		CX2ComboManager*			GetComboManager() { return m_pComboManager; }
//{{ robobeg : 2013-11-04
		//CX2Stat*					GetStat() { return m_pStat; }
        const CX2Stat&				GetStat() const { return m_Stat; }
        CX2Stat&				    AccessStat() { return m_Stat; }
//}} robobeg : 2013-11-04
		void						MultiplyStat( float fFactor );

		bool						GetAttackedByUserUnit() { return m_bAttackedByUserUnit; }
		void						SetAttackedByUserUnit( bool bAttackedByUserUnit ) { m_bAttackedByUserUnit = bAttackedByUserUnit; }
		//UidType						GetAttackerUnitUID() { return m_AttackerUnitUID; }
		//void						SetAttackerUnitUID( UidType AttackerUnitUID ) { m_AttackerUnitUID = AttackerUnitUID; }

//#ifdef SERV_PET_SYSTEM
//		UidType						GetLastAttackUnitUID() { return m_LastAttackUnitUID; }
//		void						SetLastAttackUnitUID( UidType attackUnitUID ) { m_LastAttackUnitUID = attackUnitUID; }
//#endif
		CX2GameUnit*				GetGameUnitAttackedByMe() const { return ( null != m_optrGameUnitAttackedByMe ? m_optrGameUnitAttackedByMe.GetObservable() : NULL ); }
		void						SetGameUnitAttackedByMe( CX2GameUnit* pGameUnitAttackedByMe_ ) 
		{ 
			if ( NULL != pGameUnitAttackedByMe_ )
				m_optrGameUnitAttackedByMe = pGameUnitAttackedByMe_; 
		}
		void						ResetGameUnitAttackedByMe() { m_optrGameUnitAttackedByMe.Reset(); }

		CX2GameUnit*				GetGameUnitWhoAttackedMe() const { return ( null != m_optrGameUnitWhoAttackedMe ? m_optrGameUnitWhoAttackedMe.GetObservable() : NULL ); }
		void						SetGameUnitWhoAttackedMe( CX2GameUnit* pGameUnitWhoAttackedMe_ ) 
		{ 
			if ( NULL != pGameUnitWhoAttackedMe_ )
            {
#ifdef  X2OPTIMIZE_UNITTYPE_BUG_FIX
                m_bAttackedByUserUnit = pGameUnitWhoAttackedMe_->GetGameUnitType() == GUT_USER; 
#endif  X2OPTIMIZE_UNITTYPE_BUG_FIX
				m_optrGameUnitWhoAttackedMe = pGameUnitWhoAttackedMe_ ; 
            }//if
		}
		void						ResetGameUnitWhoAttackedMe() { m_optrGameUnitWhoAttackedMe.Reset(); }

		void						SetDamageData_LUA( const char* szTableNameUTF8 );

		CKTDXDeviceSound*			PlaySound( const WCHAR* pName, bool b3DSound = true, bool bLoop = false );
		void						PlaySound_LUA( const char* pName );
		void						StopSound_LUA( const char* pName );
		
		void						ClearHitUnitList_LUA()
		{ 
			m_DamageData.hitUnitList.resize(0); 
		}

		void						SetSpeedX( float fX ){ m_PhysicParam.nowSpeed.x = fX; }
		void						SetSpeedY( float fY ){ m_PhysicParam.nowSpeed.y = fY; }
		void						SetColor( D3DXCOLOR color ){ m_RenderParam.color = color; }
		D3DXCOLOR					GetColor() const { return m_RenderParam.color; }
		const CKTDGXRenderer::RenderParam& GetUnitRenderParam() const { return m_RenderParam; }

		D3DXVECTOR3					GetImpactPoint_LUA(){ return m_DamageData.impactPoint; }
		CX2GameUnit*				GetAttackerUnit_LUA(){ return ( null != m_DamageData.optrAttackerGameUnit ? m_DamageData.optrAttackerGameUnit.GetObservable() : NULL ); }

		virtual D3DXVECTOR3			GetCloneFramePosition( const WCHAR* boneName ) { return D3DXVECTOR3(0,0,0); }
		float						GetDieAfterTime(){ return m_fDieAfterTime; }
		
		virtual void				InitPosition( bool bUseTeamPos = true, int startPosIndex = -1 ) = NULL;
		bool						GetUpMPThisFrame(){ return m_bUpMPThisFrame; }
		void						SetUpMPThisFrame( bool bUpMPThisFrame ){ m_bUpMPThisFrame = bUpMPThisFrame; }

		float						GetChargeMpHitMeleeAttack() const { return m_AdditionalChargeMpHitMeleeAttack.GetResultValue(); }
		float						GetChargeMpHitRangeAttack() const { return m_AdditionalChargeMpHitRangeAttack.GetResultValue(); }

		float						GetChargeMpHitted() const { return m_AdditionalChargeMpHitted.GetResultValue(); }

		float						GetDistanceFrom( D3DXVECTOR3 vPos ) 
		{ 
			D3DXVECTOR3 vDist = vPos - GetPos();
			return D3DXVec3Length( &vDist ); 
		}
		float						GetXDistanceFrom( float fX ) { return fabs( fX - GetPos().x ); }
		void						SetUseWorldColor( bool bUseWorldColor ){ m_bUseWorldColor = bUseWorldColor; }


		bool						IsCanNotIntput() const;
		void						SetCanNotInputTime_LUA( float fCanNotInputTime ){ m_fCanNotInputTime = fCanNotInputTime; }
		void						SetCanNotInputTime_ZXArrow( float fCanNotInputTime ) { m_fCanNotInputTimeZXArrow = fCanNotInputTime; }
#ifdef MODIFY_DUNGEON_STAGING
		void						SetCanNotInput(bool bVal){m_bCanNotInput = bVal;}
#endif //MODIFY_DUNGEON_STAGING 



		void						SetDamageReducePercent( int damageReducePercent ) { m_DamageReducePercent = damageReducePercent; }
		int							GetDamageReducePercent() { return m_DamageReducePercent; }

        void                        SetDamageGaurdPercent( int damageGuardPercent ) { m_DamageGuardPercent = damageGuardPercent; }

#ifdef SERV_RENA_NIGHT_WATCHER
		void						SetExtraDamagePack( ExtraDamagePack *pExtraDamagePack );
#else  SERV_RENA_NIGHT_WATCHER
		void						SetExtraDamagePack( ExtraDamagePack *pExtraDamagePack ) { m_ExtraDamagePack = *pExtraDamagePack; }
#endif SERV_RENA_NIGHT_WATCHER

//{{ robobeg : 2013-11-04
		//ExtraDamagePack*			GetExtraDamagePack() { return &m_ExtraDamagePack; }
        const ExtraDamagePack&		GetExtraDamagePack() const { return m_ExtraDamagePack; }
        ExtraDamagePack&			AccessExtraDamagePack() { return m_ExtraDamagePack; }
//}} robobeg : 2013-11-04

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE //��â��
		//�ڽſ��� �ɸ� ���� ��æƮ Ÿ���� ��ȯ
		const CX2DamageManager::EXTRA_DAMAGE_TYPE		GetApplyWeaponExtraDamageType();
#endif //SERV_ELESIS_SECOND_CLASS_CHANGE

		void						AddOnStatReq( KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT statType, float fStatVal, float fTime );
		void						SetAddOnStat( KEGS_ADD_ON_STAT_REQ::ADD_ON_STAT statType, float fStatVal, float fTime );


		void									SetMajorMeshPlayer_LUA( int index, CKTDGXMeshPlayer::CXMeshInstance* pMeshPlayer );
		void									ClearMajorMeshPlayer_LUA( int index );
		CKTDGXMeshPlayer::CXMeshInstance*		GetMajorMeshPlayer_LUA( int index );
		CKTDGXMeshPlayer::CXMeshInstanceHandle	GetMajorMeshPlayerHandle( int index );

		void									SetMinorMeshPlayer_LUA( int index, CKTDGXMeshPlayer::CXMeshInstance* pMeshPlayer );
		void									ClearMinorMeshPlayer_LUA( int index );
		CKTDGXMeshPlayer::CXMeshInstance*		GetMinorMeshPlayer_LUA( int index );
		CKTDGXMeshPlayer::CXMeshInstanceHandle	GetMinorMeshPlayerHandle( int index );




		void								SetSubAttackList( CKTDGXSkinAnim* pXSkinAnim );
//{{ robobeg : 2013-11-04
		//CX2SocketItem::SocketData*		GetSocketData(){ return &m_SocketData; }
        const CX2SocketItem::SocketData&	GetSocketData() const { return m_SocketData; }
//}} robobeg : 2013-11-04
		CX2DamageManager::EXTRA_DAMAGE_TYPE GetEnchantExtraDamageType() { return m_eWeaponEnchantExtraDamageType; }		// fix!! rename GetWeaponEnchantExtraDamageType
		const CX2EnchantItem::EnchantData*	GetWeaponEnchantData( CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType ) const;
		
		
		virtual int							GetRandomInt( int iOffset = 0 ) = NULL;		// 0~99 ������ ���� ����
		virtual float						GetRandomFloat( int iOffset = 0 ) = NULL;	// 0~0.99999 ������ ���� ����

#ifdef NEW_SKILL_2010_11					// oasis907 : ����� [2010.11.8] // �ε� ����Ʈ - ���� ���� - EDT_BLIND
		virtual float						GetAttackRateByAbnormalityBlind();
#endif NEW_SKILL_2010_11

		virtual float						GetCriticalPercent( BYTE byDamageType, float fExtraCritical = 0.f ) { return 0.0f; }
		virtual float						GetCriticalValue( float fDefenderCritical = 0.f ) { return GetCriticalAttack(); }

		//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
		/// �ǰ��ڰ� ũ��Ƽ���� ���� ���� Ȯ��
		const float							GetCriticalValueFromDefender();
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
		//}} 2011-04 �� ��ġ�� û ĳ�� ��ų

		virtual void						SetGameStat( const KStat& kGameStat_ );
		virtual void						SetGameStat( const CX2Stat::Stat& Stat_ );

		float								GetMaxHpValue() const { return m_AdditionalMaxHp.GetResultValue(); }
		float								GetPhysicAttackValue() const { return m_AdditionalPhysicAttack.GetResultValue(); }
		float								GetMagicAttackValue() const { return m_AdditionalMagicAttack.GetResultValue(); }
		float								GetPhysicDefenceValue() const { return m_AdditionalPhysicDefence.GetResultValue(); }
		float								GetMagicDefenceValue() const { return m_AdditionalMagicDefence.GetResultValue(); }
		float								GetAnimSpeedValue() const { return m_AdditionalAnimSpeed.GetResultValue(); }
		
		float								GetWalkSpeed() const { return m_AdditionalWalkSpeed.GetResultValue() * m_fRunJumpSpeedRate; }
		float								GetDashSpeed() const { return m_AdditionalDashSpeed.GetResultValue() * m_fRunJumpSpeedRate; }
		float								GetJumpSpeed() const { return m_AdditionalJumpPower.GetResultValue(); }
		float								GetDashJumpSpeed() const { return m_AdditionalDashJumpPower.GetResultValue(); }
		
		float								GetAccuracyPercent() const { return m_AdditionalAccuracy.GetResultValue(); }
		float								GetAvoidancePercent() const { return m_AdditionalAvoidance.GetResultValue(); }
		float								GetCriticalRate() const { return m_AdditionalCriticalRate.GetResultValue(); }
		float								GetCriticalAttack() const { return m_AdditionalCriticalAttack.GetResultValue(); }
		float								GetAntiCritialRate() const { return m_AdditionalAntiCriticalRate.GetResultValue(); }
		float								GetCriticalDefence() const { return m_AdditionalCriticalDefence.GetResultValue(); }
		float								GetAdditionalAttack() const { return m_AdditionalChangeAdditionalAttack.GetResultValue(); }
#ifdef FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER
		void								SetAdditionalAttack (const float AdditionalAttackDamage_ ) { m_AdditionalChangeAdditionalAttack.SetBasicStatValue(AdditionalAttackDamage_); }
#endif // FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		float								GetHyperChargeSpeed() const { return m_AdditionalHyperCharge.GetResultValue(); }
#endif //SERV_ADD_LUNATIC_PSYKER

		float								GetAdditionalDefence() const { return m_AdditionalChangeAdditionalDefence.GetResultValue(); }
		float								GetAdditionalFinalDamageMultiplierMeleeAttack() const { return m_AdditionalFinalDamageMultiplierMeleeAttack.GetResultValue(); }
		float								GetAdditionalFinalDamageMultiplierRangeAttack() const { return m_AdditionalFinalDamageMultiplierRangeAttack.GetResultValue(); }
		float								GetAdditionalFinalDefenceMultiplier() const { return m_AdditionalFinalDefenceMultiplier.GetResultValue(); }
		float								GetAdditionalForceDownMultiplierMeleeAttack() const { return m_AddtionalForceDownMultiplierMeleeAttack.GetResultValue(); }
		float								GetAdditionalForceDownMultiplierRangeAttack() const { return m_AddtionalForceDownMultiplierRangeAttack.GetResultValue(); }
		
		/// AttactResult���� ����� Delegate���μ���
		void								InsertDelegateProcessToAttackResult( const DelegateProcessWithDamageData& delegateProcess_ ) { m_vecDelegateProcessInAttackResult.push_back( delegateProcess_ ); }
		void								DoDelegateProcessInAttackResult( const CX2DamageManager::DamageData& damageData_ ) { DoDelegateProcessWithDamageData( m_vecDelegateProcessInAttackResult, damageData_ ); }

		/// DamageReact���� ����� Delegate���μ���
		void								InsertDelegateProcessToDamageReact( const DelegateProcess& delegateProcess_ ) { m_vecDelegateProcessInDamageReact.push_back( delegateProcess_ ); }
		void								DoDelegateProcessInDamageReact() { DoDelegateProcess( m_vecDelegateProcessInDamageReact ); }

		/// DamageReact �ÿ��� ����Ǵ� ��������Ʈ ����(DamageData ���� ���)
		void								InsertDelegateProcessWithDamageDataToDamageReact ( const DelegateProcessWithDamageData& delegateProcess_ ) { m_vecDelegateProcessWithDamageDataInDamageReact.push_back( delegateProcess_ ); }
		void								DoDelegateProcessWithDamageDataInDamageReact ( const CX2DamageManager::DamageData& damageData_ ) { DoDelegateProcessWithDamageData( m_vecDelegateProcessWithDamageDataInDamageReact, damageData_ ); }
		
		/// CommonFrameMove���� ����� Delegate���μ���
		void								InsertDelegateProcessToCommonFrameMove( const DelegateProcess& delegateProcess_ ) { m_vecDelegateProcessInCommonFrameMove.push_back( delegateProcess_ ); }
		void								DoDelegateProcessInCommonFrameMove() { DoDelegateProcess( m_vecDelegateProcessInCommonFrameMove ); }

		void								ClearAllDelegateProcess()
		{
			m_vecDelegateProcessInAttackResult.clear();
			m_vecDelegateProcessInDamageReact.clear();
			m_vecDelegateProcessInCommonFrameMove.clear();
			m_vecDelegateProcessWithDamageDataInDamageReact.clear();
		}

		virtual void						ChangeModelDetail( int detailPercent ) = 0;

		float GetScale() const { return m_fScale; }

		const D3DXVECTOR2& GetOrgUnitSize() { return m_vOrgUnitSize; }


		virtual void						ShowSnatchCureEffect() {}

		virtual float						GetDamageUpPercentBySMA( int npcID ) { return 0; }
		virtual float						GetEvadeUpPerBySMA( int npcID ) { return 0; }

		int							GetWeaponCount() { return (int) m_vecpWeapon.size(); }
		D3DXVECTOR3					GetWeaponBoneRot( int iWeaponIndex, int iModelIndex );
		D3DXVECTOR3					GetWeaponBonePos( int iWeaponIndex, const WCHAR* pBoneName, int iModelIndex ) const;
		D3DXVECTOR3 				GetWeaponBonePos_LUA( int iWeaponIndex, const char* pBoneName );
		void                        SetWeaponEnable(UINT index, bool bEnable)
		{
			if( index < 0 || index >= m_vecpWeapon.size() )
				return;

			m_vecpWeapon[index]->SetEnable(bEnable);
		}

#ifdef RIDING_SYSTEM
		CX2RidingPetPtr GetRidingPet()
		{
			return m_ptrRidingPet;
		}

		virtual bool GetRidingOn() const { return false; }
#endif //RIDING_SYSTEM

		void AddWeaponAttackCollisionBoxForNPC( Weapon* pWeapon );
		void RemoveAllWeaponAttackCollisionBoxForNPC();

		void AttachWeaponParticle( int iWeaponIndex, bool bIsMajorParticle, const wstring& wstrBoneName, 
			CKTDGParticleSystem::CParticleEventSequenceHandle hSeq );
		void AttachWeaponParticle_LUA( int iWeaponIndex, bool bIsMajorParticle, char* pBoneName, 
			CKTDGParticleSystem::CParticleEventSequenceHandle hSeq );

		bool GetInvisibility() const { return m_bInvisibility; }
		bool GetAbsoluteInvisibility() const { return m_bAbsoluteInvisibility; }




		// dmlee 2009.5.9 �̺� ��Ȧ �ʻ�� ���� ���� ���� - ������ displace ��ġ�� �����ϵ��� �߾��µ�, �ð������� �ִ� ��쿡 ������������ ��ġ�� �����Ǵ� ������ �־ ���� assign �ϴ� ������ �����߽��ϴ�.
		void SetDisplaceUnitPosition( const D3DXVECTOR3& vDisplace )
		{
			m_vDisplaceUnitPosition = vDisplace;
		}
		void ClearDisplaceUnitPosition()
		{
			m_vDisplaceUnitPosition = D3DXVECTOR3( 0, 0, 0 );
		}

		void SetFixedUnitPosition(const D3DXVECTOR3& val) { m_vFixedUnitPosition = val; }
		void ClearFixedUnitPosition() { m_vFixedUnitPosition = D3DXVECTOR3(0, 0, 0); }



		CX2DamageManager::DamageData& GetDamageData() { return m_DamageData; }
		void SetExtraDamageDataTime( float fTime ) // ���� ���ʱ� Ʈ���� ���� ����ó�� �ڵ� 
		{
			if( CX2DamageManager::EDT_ENTANGLE == m_DamageData.m_ExtraDamage.m_ExtraDamageType )
			{
				m_DamageData.m_ExtraDamage.m_fTime = fTime;
			}
		}

		
		
		// �������
		void SetDoubleAttack(wstring wstrHitSound, float fDamage, D3DXVECTOR3 vimpactPoint);


		virtual float GetPowerRate() = NULL;



#ifdef GRAPPLING_TEST
		virtual void CalcGrapplingPosition( bool bFuture ) = NULL;
		virtual void ResetGrapplingState( bool bOnlyMe ) = NULL;

		virtual void SetUnitGrapplingMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID ) = NULL;
		virtual void SetUnitGrappledByMe( bool bFuture, GAME_UNIT_TYPE eGameUnitType, UidType unitUID ) = NULL;

		virtual CX2GameUnit* GetUnitGrapplingMe( bool bFuture ) = NULL;
		virtual CX2GameUnit* GetUnitGrappledByMe( bool bFuture ) = NULL;


		virtual bool IsGrappled( bool bFuture ) = NULL;

		
#endif GRAPPLING_TEST



//#ifdef STEP_ON_MONSTER_TEST
//		bool CanBeSteppedOn() const { return m_bCanBeSteppedOn; }
//		void SetCanBeSteppedOn(const bool val) { m_bCanBeSteppedOn = val; }
//#endif STEP_ON_MONSTER_TEST


//#ifdef STEP_ON_MONSTER_COLLISION_BOX_TEST
//		bool CanBeSteppedOnCollsionBox() const { return m_bCanBeSteppedOnCollsionBox; }
//		void SetCanBeSteppedOnCollsionBox(const bool val) { m_bCanBeSteppedOnCollsionBox = val; }
//#endif STEP_ON_MONSTER_COLLISION_BOX_TEST

		//{{ kimhc // �ǽð� ���ҵ� �� �ǽð� ��� ��ü
#ifdef REAL_TIME_ELSWORD
		void DeleteEnchantWeaponEffectUnitSpecific( int index );
#endif REAL_TIME_ELSWORD
		//}} kimhc // �ǽð� ���ҵ� �� �ǽð� ��� ��ü

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		virtual void LoadEffectSetFromScript( IN KLuaManager& luaManager );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		void MakeEffectSetBeDeletedWhenDie( const CX2EffectSet::Handle& hEffectSet_ );

		void DeleteEffectSetOnStateEnd();
		void DeleteEffectSetOnDamageReact();
		void DeleteEffectSetOnDie();
#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
		void DeleteEffectSetOnCustomState();
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE
		void PushEffectSetToDeleteOnDie( const CX2EffectSet::Handle& hEffectSet_ ) { m_vecEffectSetToDeleteOnDie.push_back(hEffectSet_); }

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void CommonFrameMove_EffectSet( float fNowAnimationTime );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		void AddEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect );
		void RemoveEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect );
        bool IsEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffectHandle hDamageEffect );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		void AddEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect );
		void RemoveEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect );
        bool IsEffectiveBlackHoleDamageEffect( CX2DamageEffect::CEffect* pDamageEffect );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		void ClearEffectiveBlackHoleDamageEffect();
		bool IsThereEffectiveBlackHoleDamageEffect();

		virtual int GetUnitLevel() const { return 0; }
		//virtual int GetUnitLevel() = 0;

#ifdef SKILL_30_TEST

		//const CX2GameUnit::SPEED_FACTOR& GetMoveSpeedFactor() const { return m_PhysicParam.m_MoveSpeedFactor; }
		void SetMoveSpeedFactor( float fSpeedRate, float fDuration )
		{ 
// 			m_PhysicParam.m_MoveSpeedFactor.m_fSpeedRate = fSpeedRate;
// 			m_PhysicParam.m_MoveSpeedFactor.m_fDuration = fDuration;
		};


		//const CX2GameUnit::SPEED_FACTOR& GetJumpSpeedFactor() const { return m_PhysicParam.m_JumpSpeedFactor; }
		void SetJumpSpeedFactor( float fSpeedRate, float fDuration )
		{ 
// 			m_PhysicParam.m_JumpSpeedFactor.m_fSpeedRate = fSpeedRate;
// 			m_PhysicParam.m_JumpSpeedFactor.m_fDuration = fDuration;
		};

#endif SKILL_30_TEST

#ifdef SERV_SKILL_NOTE
		void ClearAnimSpeedFactor();
		void SetAnimSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId = CX2GameUnit::SFI_NONE, SPEED_FACTOR_SET_TYPE eSetType = SFST_MULTY );
#endif

		void ClearMoveSpeedFactor();
		void ClearJumpSpeedFactor();
		void SetVecMoveSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId = CX2GameUnit::SFI_NONE, SPEED_FACTOR_SET_TYPE eSetType = SFST_MULTY );
		void SetVecJumpSpeedFactor( float fSpeedRate, float fDuration, SPEED_FACTOR_ID eId = CX2GameUnit::SFI_NONE, SPEED_FACTOR_SET_TYPE eSetType = SFST_MULTY );
		void RemoveVecMoveSpeedFactor( SPEED_FACTOR_ID eID );

		virtual bool CanApplyBuffToGameUnit() const;
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        virtual const std::vector<BUFF_TEMPLET_ID>& GetVecImmunityAtThisState() const = NULL;
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool ApplyBuffToGameUnitInDamageReact( const CX2DamageManager::DamageData* pDamageData_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		virtual bool SetBuffFactorToGameUnitByBuffFactorID( const UINT uiBuffFactorId_, IN const UINT uiLevel_ = 1 );
#else //UPGRADE_SKILL_SYSTEM_2013
		virtual bool SetBuffFactorToGameUnitByBuffFactorID( const UINT uiBuffFactorId_ );
#endif //UPGRADE_SKILL_SYSTEM_2013

#ifdef ADD_LIVE_CREATOR_FINALIZER
	#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		virtual bool SetBuffFactorToGameUnitByBuffFactorIDAndCreatorID( const UINT uiBuffFactorId_, const UidType iCreatorID_,
			const bool bIsUserID_ = true, IN const UINT uiLevel_ = 1  );
	#else //UPGRADE_SKILL_SYSTEM_2013
		virtual bool SetBuffFactorToGameUnitByBuffFactorIDAndCreatorID( const UINT uiBuffFactorId_, const UidType iCreatorID_, const bool bIsUserID_ = true );
	#endif //UPGRADE_SKILL_SYSTEM_2013
#endif ADD_LIVE_CREATOR_FINALIZER
		virtual bool SetBuffFactorToGameUnit( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const UINT uiIndex_ );
		virtual bool SetBuffFactorToGameUnit( const CX2BuffFactor& buffFactor_ );

#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		virtual bool SetBuffFactorToDamageData( const UINT uiBuffFactorId_, IN const UINT uiLevel_ = 1 );
#else //UPGRADE_SKILL_SYSTEM_2013
		virtual bool SetBuffFactorToDamageData( const UINT uiBuffFactorId_ );
#endif //UPGRADE_SKILL_SYSTEM_2013
		
		bool GetBuffFinalizerTempletPtrList( const BUFF_TEMPLET_ID eBuffTempletID_, OUT vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ );
		void ChangeBuffFinalizerTempletPtrList( const BUFF_TEMPLET_ID eBuffTempletID_, OUT vector<CX2BuffFinalizerTempletPtr>& vecFinalizerPtr_ );

		void PushNewBuffTempletToGameUnit( IN CX2BuffTempletPtr ptrBuffTemplet_, const CX2BuffFactor& buffFactor_, const UINT uiUniqueNum_ );
		void EraseBuffTempletFromGameUnit( const BUFF_TEMPLET_ID eBuffTempletID_ );
		void EraseBuffTempletFromGameUnitByIterator( vector<CX2BuffTempletPtr>::iterator vItr_ );
		void ReserveToFinishBuffTempletFromGameUnit( const BUFF_TEMPLET_ID eBuffTempletID_ );

		bool HaveThisBuff( const BUFF_TEMPLET_ID eBuffTempletID_ );

		UINT GetAppliedBuffCount( const bool bIsDeBuff_ ) const;

		/// �� ���ָ��� Ư�� ������ ���� �Ǵ� ���� �� �� �����ؾ� �ϴ� �Լ� ����(ex: �����ø���ũ���� �������̶�� �÷��� ��...)
		virtual void SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ ) {}
		virtual void UnSetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ ) {}

//{{ kimhc // 2010.4.2 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
		float	GetStatUpBufferBySocketItem() const;
		CX2SocketItem::STAT_UP_TYPE GetStatUpType() const { return m_StatUpType; }
		void	SetStatUpBufferBySocketItem( float fVal, float fTime )
		{
			m_StatUpBufferBySocketItem.m_fBufferRate = fVal;
			m_StatUpBufferBySocketItem.m_fBufferTime = fTime;
		}
		void	ProcessStatPercentUpBuffer( CX2GameUnit* pAttacker, OUT CX2Stat &AttackerStat, OUT CX2Stat &DefenderStat );
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.2 // ��д��� �۾�(��Ʈ������ȿ��)
		float GetGroggyTime() const { return m_fGroggyTime; }
		void SetGroggyTime(const float val) { m_fGroggyTime = val; }
		
		bool GetDrainMana() { return m_bHitedDrainMana; }



		float GetNoSkillTime() const { return m_fNoSkillTime; }
		void AddNoSkillTime(const float val) { m_fNoSkillTime += val; }


		const CX2GameUnit::SkillRelatedData& GetSkillRelatedData() const { return m_SkillRelatedData; }


#ifdef RESET_INVALID_UNIT_POSITION_TEST

		bool IsMisplaced() 
		{
			const float MAGIC_TIME = 3.f;
			if( true == m_bMisplaced && m_TimerMisplaced.elapsed() > MAGIC_TIME )
				return true;
			else
				return false;
		}

#endif RESET_INVALID_UNIT_POSITION_TEST


		bool IsImmuneToExtraDamage() const { return m_bImmuneToExtraDamage; }


		const CX2GameUnit::CommonState& GetCommonState() const { return m_CommonState; }



//#ifdef TRANSFORMER_TEST
//		bool GetTransformed() const { return m_bIsTransformed; }
//		void SetTransformed(const bool val) { m_bIsTransformed = val; }
//
//		UidType GetTransformerOwnerUID() const { return m_TransformerOwnerUID; }
//		void SetTransformerOwnerUID(const UidType val) { m_TransformerOwnerUID = val; }
//
//#endif TRANSFORMER_TEST

#ifdef RAVEN_SECOND_CLASS_CHANGE
		void ChangeDamageType(CX2DamageManager::DAMAGE_TYPE eDamageType, float fDamageRel)
		{
			m_bChangeDamageType = true;
			m_eDamageType = eDamageType;
			m_fDamageRel = fDamageRel;
		}
		void ResetDamageType()
		{
			m_bChangeDamageType = false;
			m_eDamageType = CX2DamageManager::DT_PHYSIC;
			m_fDamageRel = 1.f;
		}
		bool GetChangeDamageType(CX2DamageManager::DAMAGE_TYPE &eDamageType, float &fDamageRel)
		{
			eDamageType = m_eDamageType;
			fDamageRel = m_fDamageRel;
			return m_bChangeDamageType;
		}
		void ChangeDamageTimes(int iDamageTimes) { m_iDamageTimes = iDamageTimes; }
		void ResetDamageTimes() { m_iDamageTimes = -1; }
		int  GetDamageTimes() { return m_iDamageTimes; }
#endif
		
		/** @function SetShieldDamageReduce
			@brief ������ ������ ���� �ɷ�
		*/
        //{{ JHKang / ������ / 2010/12/07
#ifdef NEW_SKILL_2010_11
        void SetShieldDamageReduce( IN float fValue_ ) { m_fAtomicShieldDamageRate = fValue_; }
#endif NEW_SKILL_2010_11
        //}} JHKang / ������ / 2010/12/07

		void SetLockOnNearTarget_LUA( CX2DamageEffect::CEffect* pCEffect, int randomOffset = 0 );

#ifdef CHUNG_SECOND_CLASS_CHANGE
		// oasis907 : ����� [2011.6.16] û 2�� ���� - ���̾� �ȶ��
		void ProcessTransitDebuff( const CX2DamageManager::DamageData* pDamageData_, CX2GameUnit* pTargetUnit_ );
		bool GetEnableWeaponRenderEffect(){ return m_bEnableWeaponRenderEffect; }
		void SetEnableWeaponRenderEffect( bool _bVal );
		D3DXCOLOR GetWeaponRenderEffectColor(){ return m_WeaponRenderEffectColor; }
		void SetWeaponRenderEffectColor( const D3DXCOLOR& _Color){ m_WeaponRenderEffectColor = _Color; }

		void FadeInWeaponRenderEffect( const D3DXCOLOR& _StartColor, const D3DXCOLOR& _EndColor,  float _fChangeTime, float _fDuration);
		void PulseWeaponRenderEffect( const D3DXCOLOR& _StartColor, const D3DXCOLOR& _EndColor, float _fPeriod, float _fDuration, PULSE_WEAPON_COLOR_ID _ePulseWeaponColorID = PWCI_UNDEFINED);

		void FrameMoveWeaponRenderEffect();

#endif CHUNG_SECOND_CLASS_CHANGE

		//{{ kimhc // 2011-07-19 // �ɼǵ���Ÿ ��ġȭ �۾�
		// �ɼ� ��ġȭ�� ���� �߰� ������
#ifdef ELSWORD_SHEATH_KNIGHT
		virtual float			GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData )
#else
		virtual float			GetAdditionalAttackDamage( const CX2DamageManager::DamageData* pAttackDamageData ) const
#endif ELSWORD_SHEATH_KNIGHT
		{ return 0.0f; }
		//}} kimhc // 2011-07-19 // �ɼǵ���Ÿ ��ġȭ �۾�


#ifdef SERV_PVP_NEW_SYSTEM
		bool IsPvpBot() const { return m_bPvpBot; }
		virtual wstring GetUnitName() { return L""; }
#endif

#ifdef VELDER_SECRET_DUNGEON
		void SetRestoreScale() { m_fTimeToReturnBasicScale = 0.f; }
#endif
#ifdef HIT_PARTICLE_TRACE_UNIT
		void AddTraceUnitHitParticle(CKTDGParticleSystem::CParticleEventSequenceHandle hParticle_){ m_listSeqHitParticleTraceUnit.push_back(hParticle_); }
#endif HIT_PARTICLE_TRACE_UNIT

#ifdef SERV_ARME_DIMENSION_WITCH
		void SetHitCreateDamageEffectRate( float fVal ) { m_fHitCreateDamageEffectRate = fVal; }
		float GetHitCreateDamageEffectRate() { return m_fHitCreateDamageEffectRate; }
		void SetHitCreateDamageEffectPowerRate( float fVal ) { m_fHitCreateDamageEffectPowerRate = fVal; }
		float GetHitCreateDamageEffectPowerRate() { return m_fHitCreateDamageEffectPowerRate; }
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		void		 SetStartOfDelayedFiringEmoticon();								/// �̸�Ƽ�� ����
		void		 CheckDamageEffectType( CX2DamageEffect::CEffect* pCEffect );	/// ������ ����Ʈ Ÿ��
		CX2GameUnit* GetGameUnitByUnitIndex( int iUnitIndex );						/// �ε����� �ش��ϴ� ���� ��ȯ
		void		 ClearStartOfDelayedFiringData();								/// ������ ��ȣź ���� ����
		const vector<CX2DamageManager::StartOfDelayedFiringData*>& GetStartOfDelayedFiringData() const		/// ������ ��ȣź ���� ���� ��ȯ
		{ 
			return m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData; 
		}
		vector<CX2DamageManager::StartOfDelayedFiringData*>& AccessStartOfDelayedFiringData()		/// ������ ��ȣź ���� ���� ��ȯ
		{ 
			return m_ExtraDamagePack.m_StartOfDelayedFiring.m_vecStartOfDelayedFiringData; 
		}
		void		 CommonFrameMoveStateAbnormalityStartOfDelayedFiring();			/// ������ ��ȣź �ǽð� ó��
		void		 SetOverlapEDTEmoticon( CKTDGParticleSystem::CParticleEventSequenceHandle& hEmoticon, int iActiveEDTNum );		/// ��ø�� EDT�� ���� ��ø Ƚ�� �̸�Ƽ�� ����
#endif SERV_RENA_NIGHT_WATCHER

		void		DownCrashCamera( const float fSpeed_ = -100.0f, const float fAccel_ = 1500.0f ) const;
		void		UpCrashCamera( const float fSpeed_ = 100.0f, const float fAccel_ = -1500.0f ) const;
		void		UpDownCrashCamera( const float fGap_ = 10.0f, const float fTime_ = 0.2f ) const;
		void		UpDownCrashCameraNoReset( const float fGap_ = 10.0f, const float fTime_ = 0.2f ) const;
		void		LeftRightCrashCameraNoReset( const float fGap_ = 10.0f, const float fTime_ = 0.2f ) const;
		void		ShakeLeftRight( const float fGap_ = 10.f, const float fTime_ = 0.2f, const float fTimeGap_ = 0.05f ) const;

		virtual void	UpNowMpPerHitOthers( const float fHitAddMp_, const int iAddValueByTeam_, const float fRel_ );	// �������� ������ ��ȸ�� ó��
		void			UpNowMpPerHitOthersWithDamageEffect( const float fHitAddMp_, const int iAddValueByTeam_, const float fRel_ );	// ����Ʈ ���� ������ ��ȸ�� ó��

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		void	UpNowHpPerHitOthers( IN const float fHitAddHpPer_ );	/// ���� ������, �������� ü�� ȸ�� ó��
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef ADD_MEMO_1ST_CLASS //��â��
		void	UpNowHpPerHitbyAttackPower( IN const float fHitAddHpPer_ );	/// ���� ������, �������� ü�� ȸ�� ó��( �������� ������ ��տ� ��� )
#endif //ADD_MEMO_1ST_CLASS

 		/// �¿����
		void CreateAndInsertReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ );
		void ToggleReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ );
		void EraseReverseLeftRightByBuff( const KBuffIdentity& BuffIdentity_ );

		/// ������ġ ����
#ifdef NEVER_MOVE_GRAVITY_APPLY
		void CreateAndInsertNeverMoveByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vPos_, const bool vGravity );
#else
		void CreateAndInsertNeverMoveByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vPos_ );
#endif //NEVER_MOVE_GRAVITY_APPLY	
		void EraseNeverMoveByBuff( const KBuffIdentity& BuffIdentity_ );

		/// BuffIdentity�� ���� ���еǴ� ȿ���� ��������� �ʿ���� �͵�(��úҰ�, ���ݺҰ�, ���۾Ƹ� ��)
		void CreateAndInsertBuffIdentity( const BUFF_BEHAVIOR_TYPE eType_, const KBuffIdentity& BuffIdentity_ );
		void EraseBuffIdentity( const BUFF_BEHAVIOR_TYPE eType_, const KBuffIdentity& BuffIdentity_ );

		/// BuffIdentity�� ���� ���еǴ� ȿ�� �� StateStart, CommonFrameMove, StageEnd���� ó���ؾ��ϴ� �ϵ��ڵ��� ����
		void CreateAndInsertCustomFunctionByBuff( const KBuffIdentity& BuffIdentity_, const KBuffBehaviorFactor& kBehaviorFactor_ );
		void EraseCustomFunctionByBuff( const KBuffIdentity& BuffIdentity_ );
		void DoCustomFunctionByBuffInCommonStateStart();
		void DoCustomFunctionByBuffInCommonFrameMove();
		void DoCustomFunctionByBuffInCommonStateEnd();
		void DoCustomFunctionByBuffInDamageReact( IN CX2DamageManager::DamageData& damageData_ );

		/// BuffIdentity�� ���� ���еǴ� ȿ�� �� AfterImage ���� �Լ�
		void CreateAndInsertAfterImageByBuff( const KBuffIdentity& BuffIdentity_, const D3DXCOLOR& d3dxColor_ );
		void EraseAfterImageByBuff( const KBuffIdentity& BuffIdentity_ );
		void EnableAfterImage();
		void DisableAfterImage( const bool bForce_ = false );

		virtual bool IsSuperArmor() const = NULL;
		bool ShouldAttackAllTeam() const { return !m_vecAttackAllTeamPtr.empty(); }

		/// ������ ���� RenderParam ����
		void CreateAndInsertRenderParamByBuff( const KBuffIdentity& BuffIdentity_, const StBuffRenderParam& stBuffRenderParam_ );
		virtual void CreateAndInsertRenderParamByBuffUnitType( const KBuffIdentity& BuffIdentity_, const map<CX2Unit::UNIT_TYPE, StBuffRenderParamPtr>& mapStBuffRenderParamPtr_ ) = NULL;
		void EraseRenderParamByBuff( const KBuffIdentity& BuffIdentity_ );

		/// ������ �ɸ� ���, Ÿ�ݽ� Ÿ������ ��µ� ����Ʈ��(������ ��ũ��Ʈ���� �ڵ�������Ѿ���)
#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-09
		void CreateAndInsertEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCreateGap );
#else // SERV_NEW_DEFENCE_DUNGEON
		void CreateAndInsertEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_ );
#endif // SERV_NEW_DEFENCE_DUNGEON
		void EraseEffectSetImpactPointByBuff( const KBuffIdentity& BuffIdentity_ );
		void PlayEffectSetImpactPointByBuff();

		/// ������ �ɸ� ���, �ǰݽ� ��µ� ����Ʈ��(������ ��ũ��Ʈ���� �ڵ�������Ѿ���)
#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-09
		void CreateAndInsertEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCreateGap );
#else // SERV_NEW_DEFENCE_DUNGEON
		void CreateAndInsertEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_ );
#endif // SERV_NEW_DEFENCE_DUNGEON
		void EraseEffectSetHittedByBuff( const KBuffIdentity& BuffIdentity_ );
		void PlayEffectSetHittedByBuff();

		/// ������ ���� ũ�� ����
		void CreateAndInsertChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vScale_ );
		void EraseChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const bool bUseTimeToReturn_ );
		
#ifdef VERIFY_STAT_BY_BUFF
		virtual void TransformScale( const PROTECT_VECTOR3& vScale_ ) = NULL;
#else	// VERIFY_STAT_BY_BUFF
		virtual void TransformScale( const D3DXVECTOR3& vScale_ ) = NULL;
#endif // VERIFY_STAT_BY_BUFF
		
		void TransformScale_LUA( const float fScale_, const float fTime_ );
		void ProcessReturnFromScaleChange();
		bool IsChangedGameUnitScaleByBuff() const;	/// �����Ǿ� �ִ� �����ΰ�?
#ifdef POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE
		void UpdateChangeUnitScaleByBuff( const KBuffIdentity& BuffIdentity_, const D3DXVECTOR3& vScale_, const UINT uiAccumulationLimit_ );
		D3DXVECTOR3  GetChangeUnitScaleByBuff();
#endif POSSIBLE_ADD_CHANGE_SCALE_BUFF_VALUE

		/// ����
		void CreateAndInsertStunByBuff( const KBuffIdentity& BuffIdentity_ );
		void EraseStunByBuff( const KBuffIdentity& BuffIdentity_ );

		/// ����
		void CreateAndInsertFreezeByBuff( const KBuffIdentity& BuffIdentity_ );
		void EraseFreezeByBuff( const KBuffIdentity& BuffIdentity_ );

		/// ���Ⱥ���
		void InsertAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fValue_ );
		void UpdateAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_, const float fValue_ );
		void EraseAdditionalStat( const BUFF_BEHAVIOR_TYPE eBehaviorType_, const BUFF_CHANGE_TYPE eChangeType_, const KBuffIdentity& BuffIdentity_ );
		
		void CheckAndUpdateAdditionalStat();

		void InitAdditionalStatOptionValue();
		void InitAdditionalBasicStatValue();
		void InitAdditionalPassive();
		void InitAdditionalPassiveMultipleAndSum(CX2AdditionalStatByBuff& Additional_ );
		virtual void InitAddtionalDashAndJumpBasicStatValue();

		const vector<CX2BuffFactorPtr>&					GetBuffFactorList() const { return m_vecBuffFactorPtr; }
		void CreateAndInsertSkillAndSocketBuffFactorToList( CX2BuffFactorPtr ptrBuffFactor_ );

		/// �Ͻ����� BuffFactor
		const vector<CX2BuffFactorPtr>&					GetTemporaryBuffFactorList() const { return m_vecTemporaryBuffFactorPtr; }
#ifdef UPGRADE_SKILL_SYSTEM_2013 //JHKang
		void CreateAndInsertTemporaryBuffFactor( const CX2BuffFactor& BuffFactor_, const KBuffIdentity& BuffIdentity_, UINT uiLevel );
#else //UPGRADE_SKILL_SYSTEM_2013
		void CreateAndInsertTemporaryBuffFactor( const CX2BuffFactor& BuffFactor_, const KBuffIdentity& BuffIdentity_ );
#endif //UPGRADE_SKILL_SYSTEM_2013
		void EraseTemporaryBuffFactor( const KBuffIdentity& BuffIdentity_ );


		CKTDGXSkinAfterImage* GetAfterImage() const { return m_pAfterImage; }

		void SetBasicStatValueChargeMpHitMeleeAttack( const float fValue_ ) { m_AdditionalChargeMpHitMeleeAttack.SetBasicStatValue( fValue_ ); }
		void CheckAndUpdateAdditionalChargeMpHitMeleeAttack()		{ m_AdditionalChargeMpHitMeleeAttack.CheckAndUpdateResultValue(); }

		void SetBasicStatValueChargeMpHitRangeAttack( const float fValue_ ) { m_AdditionalChargeMpHitRangeAttack.SetBasicStatValue( fValue_ ); }
		void CheckAndUpdateAdditionalChargeMpHitRangeAttack()		{ m_AdditionalChargeMpHitRangeAttack.CheckAndUpdateResultValue(); }

		void SetBasicStatValueChargeMpHitted( const float fValue_ ) { m_AdditionalChargeMpHitted.SetBasicStatValue( fValue_ ); }
		void CheckAndUpdateAdditionalChargeMpHitted()	{ m_AdditionalChargeMpHitted.CheckAndUpdateResultValue(); }

		/// ����Ʈ�������
		void CreateAndInsertAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_, const float fAbsorbValue_ );
		void UpdateAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_, const float fAbsrobValue_ );
		void EraseAbsorbEffectAttack( const KBuffIdentity& BuffIdentity_ );

		bool EmptyAbsorbEffectAttackList() const { return m_vecAbsorbEffectAttackPtr.empty(); }
		bool AbsorbEffectAttackProcess( CX2DamageManager::DamageData* pDamageData_ );

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		virtual bool DidReceiveFirstSyncPacket() { return true; };		/// �ش� ���������� ù ��ũ��Ŷ�� �޾Ҵ��� ���θ� �˾ƺ��� �Լ�
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		virtual bool DidReceiveFirstSyncPacket() const = NULL;		/// �ش� ���������� ù ��ũ��Ŷ�� �޾Ҵ��� ���θ� �˾ƺ��� �Լ�
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		/// ���� Ÿ�Կ� ���� ������ ����
		void CreateAndInsertChangeAttackByType( const KBuffIdentity& BuffIdentity_, CX2DamageManager::ATTACK_TYPE eAttackType_, float fValue_ );
		void EraseChangeAttackByType( const KBuffIdentity& BuffIdentity_ );
		float GetAdditionalAttackByType( CX2DamageManager::ATTACK_TYPE eAttackType_ );

		/// ���� �Ӽ� ���� �ߵ� Ȯ�� ����
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��æƮ �Ӽ� Ÿ�԰� ���� Ÿ�� �߰�
		void CreateAndInsertChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_, float fRate_, BUFF_ENCHANT_ATTRIBUTE_TYPE eEnchantAttributeType_ = BEAT_ALL, BUFF_CHANGE_TYPE eBuffChangeType_ = BCT_PERCENT );
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		void CreateAndInsertChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_, float fRate_ );
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		
		
		
		void EraseChangeEnchantAttackRate( const KBuffIdentity& BuffIdentity_ );
#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// ��æƮ ���� Ȯ�� ��꿡 ���Ǵ� ����ü
		// �� ���� ��, �� ���� ��, ���� Ÿ�� ( ���� ü���� Ÿ�� ) �� ���� ����� 
		struct EnchantAttackRate
		{
			EnchantAttackRate () : fIncreaseAttackRate ( 0.f ), eBuffchangeType ( BCT_PERCENT ), fIncreaseAttackRateFixValue ( 0.f ) {}
			EnchantAttackRate ( float IncreaseAttackRate_ , float IncraseAttackRateFixValue_, BUFF_CHANGE_TYPE BuffchangeType_ ) : 
				fIncreaseAttackRate ( IncreaseAttackRate_ ),fIncreaseAttackRateFixValue ( IncraseAttackRateFixValue_ ), eBuffchangeType ( BuffchangeType_ ) {}

			float				fIncreaseAttackRate;				// �� ���� ��
			float				fIncreaseAttackRateFixValue;		// �� ���� ��
			BUFF_CHANGE_TYPE	eBuffchangeType;					// ���� ���� Ÿ��
		};
		EnchantAttackRate GetChangeEnchantAttackRate( CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageType );
#else // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		float GetChangeEnchantAttackRate();
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

		/// ���� ����
		void CreateAndInsertDoubleAttackByBuff( const KBuffIdentity& BuffIdentity_, float fRate_ );
		void EraseDoubleAttackByBuff( const KBuffIdentity& BuffIdentity_ );
		bool IsDoubleAttackByBuff();

		void SetKBuffFactorPacketToGameUnit( const vector<KBuffFactor>& vecBuffFactor_ );
		
		/// �ʵ�<->���� �̵� �� �ڽſ��� �ɸ� ������ GageManager�� �ӽ÷� ���� �ߴ� ���� �ٽ� GameUnit�� �����ϴ� �Լ�
		void SetKBuffFactorFromGageManagerToGameUnit();

		bool IsBuffTempletListEmpty() { return m_vecBuffTempletPtr.empty(); }	/// ���� �Ǵ� ������� �ɷȴ��� ���θ� �˾ƺ��� �Լ�

		virtual void AddUnitSlashData( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ )
		{}

		/// ����Ǿ�� �ϴ� UnitSlashTrace�� ������ ����
		void SetUnitSlashTraceConditionToDo( const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION eSlashTraceCondition_ ) { m_eUnitSlashTraceConditionToDo = eSlashTraceCondition_; }
		const CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION GetUnitSlashTraceConditionToDo() const { return m_eUnitSlashTraceConditionToDo; }

#ifdef UPGRADE_SKILL_SYSTEM_2013
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_, const float fCoolTime = 0.f, bool bForceSet = true, BUFF_CHANGE_TYPE eChangeType = BCT_RELATION_VALUE, const float fCoolTimeRate = 1.f, const bool bIsRelativeAllSkill = false ) {}
#else // UPGRADE_SKILL_SYSTEM_2013
		virtual void ResetLeftSkillCoolTimeBySkillType( const CX2SkillTree::SKILL_TYPE eSkillType_ ) {}
#endif // UPGRADE_SKILL_SYSTEM_2013

		void InsertPairSubAttackListSet( const CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshHandle_, const CKTDGXSkinAnim* pSkinAnim_ );
		void ErasePairSubAttackListSet( const CKTDGXMeshPlayer::CXMeshInstanceHandle hMeshHandle_ );
		bool GetSubAttackListGetFromPair( const UINT uiIndex_, OUT const CKTDXCollision::CollisionDataListSet** ppCollisionDataListSet_ );
		UINT GetSizeOfPairSubAttackListSet() const { return m_vecPairSubAttackListSet.size(); }

		bool GetNeedToUpdateWeaponEffect() const { return m_bNeedToUpdateWeaponEffect; }
		void SetNeedToUpdateWeaponEffect( bool bNeedToUpdateWeaponEffect_ ) { m_bNeedToUpdateWeaponEffect = bNeedToUpdateWeaponEffect_; }

		virtual void UpdateWeaponEnchantEffectAndExtraDamageType() {}
		virtual void GuildSkillProcess( const CX2SkillTree::SKILL_ID eSkillID_ ) {}

		void GetBuffFactor( OUT vector<KBuffFactor>& vecBuffFactor_ ) const;
		void GetBuffFactorOnlyDebuff( OUT vector<KBuffFactor>& vecBuffFactor_ ) const;
		bool IsApplyBuffByBuffTempletID( BUFF_TEMPLET_ID eBuffTempletID_ ) const;

		virtual bool GetShowOnMiniMap() const;
		virtual bool IsGateNpc() const { return false; }
		
		//���̺� �뺴�� ó�ż� �нú�(SI_P_RWT_BEHAVIOR_OF_MERCENARY) ȿ���� �����Ű�� ���� �߰��� �Լ�
		virtual float GetReduceDebuffDurationRate(BUFF_TEMPLET_ID eBuffTempletID_){ return 1.f; }

#ifdef SERV_NEW_DEFENCE_DUNGEON  // ���볯¥: 2013-03-23
		/// ��ũ��Ʈ ������ Ʈ���̽� �α� ����� ���ִ� �Լ�
		void WriteTraceLog_LUA( const char* wstrLog_ ) 
		{ 
			wstring wstrStateName = L"";
			ConvertUtf8ToWCHAR( wstrStateName, wstrLog_ );

			wstrStateName += L"\n";

			DXUTTRACE( wstrStateName.c_str() ); 
		}

		/// HP �� MP ����ġ ��� �Լ�
		void ShowIncreaseValue( D3DXVECTOR3 vecPos, bool IsHP, float fValue );
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		const vector<CX2ReflectMagicByBuffPtr>& GetVecReflectMagicByBuffPtr() const { return m_vecReflectMagicByBuffPtr; }
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_9TH_NEW_CHARACTER // ����ȯ
		virtual void		UpNowDPValueByHit( IN const CX2DamageManager::DamageData& DamageData_ ) {}	/// �ֵ��� ��, Ÿ�ݽ� DP ���� ó��
		virtual	void		UpNowDPValue( IN const float fDPValue_, IN const float fChangeValueRate_ = 1.f ) {}	/// �ֵ��� ��, DP ��ġ ���� �Լ�
		bool				IsRightTarget(D3DXVECTOR3 vPos);	/// Ư����ġ�� ���� ���������� �������� �Ǻ��Ѵ�. ( ������ ���� ����, CX2GameUnit���� �̵� )
#endif //SERV_9TH_NEW_CHARACTER

		// Public Function End

#ifdef MODIFY_SET_DAMAGE_DATA
		virtual	void SetDamageExceptionProcess( OUT CX2DamageManager::DamageData* pDamageData_ , OUT float& fPowerRate);
#endif //MODIFY_SET_DAMAGE_DATA

#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
		/// BuffIdentity�� ���� ���еǴ� ȿ�� �� ������ AfterImage ���� �Լ�
		void CreateAndInsertWeaponAfterImageByBuff( const KBuffIdentity& BuffIdentity_, const D3DXCOLOR& d3dxColor_ );
		void EraseWeaponAfterImageByBuff( const KBuffIdentity& BuffIdentity_ );
		void EnableWeaponAfterImage();
		void DisableWeaponAfterImage( const bool bForce_ = false );
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

#ifdef FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER
		// GUNPC �� GUUSER �� GetAdditionalAttackDamage �Լ��� ����ϰ� ���ʿ� ����
		// ����ϴ� �κи� ���� ��� ���� ����ϱ� ���� �ۼ�
		float CalcAdditionalAttackDamage ( const CX2DamageManager::DamageData* pAttackDamageData_ ) const;
#endif // FIXED_APPLYING_ADDITINAL_DAMAGE_FOR_SUMMON_MONSTER

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE		/// �׷� ������ ���� ���� ��ȯ
		const vector<DamageEffectGroupDataPtr>& GetDamageEffectGroupData() const { return m_vecDamageEffectGroupDataPtr; }
        vector<DamageEffectGroupDataPtr>& AccessDamageEffectGroupData() { return m_vecDamageEffectGroupDataPtr; }

		void ProcessGroupDamageData( float fElapsedTime_ );	/// �׷� ������ ���� ó��
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE
		float GetAdditionalAttackPowerRateByType( CX2DamageManager::DamageData* pDamageData_ );		/// Ư�� ���ݿ� ���� ������ ���� ��ȯ �Լ� ( AT_SPECIAL, AT_NORMAL )
#endif ADD_SUMMON_NPC_SPECIAL_ACTOVE_POWER_RATE

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-14
		/// �Ϲ� ���� Ÿ�ݽ� ����Ʈ �� �����ϴ� ���÷��̾� Ÿ��
		void CreateAndInsertEffectSetNormalAttackByBuff( const KBuffIdentity& BuffIdentity_, const WCHAR* pwszEffectSetName_, const WCHAR* pwszSoundName_, const float fCreateGap );
		void EraseEffectSetNormalAttackByBuff( const KBuffIdentity& BuffIdentity_ );
		void PlayEffectSetNormalAttackByBuff( CX2DamageManager::ATTACK_TYPE eAttackType );
#endif // SERV_NEW_DEFENCE_DUNGEON

		/// ������ ���� ��ų������ �����ϴ� ó��
		void CreateAndInsertAddSkillLevelByBuff( const KBuffIdentity& BuffIdentity_, const int iAddSkillLevel_ );
		void EraseAddSkillLevelByBuff( const KBuffIdentity& BuffIdentity_ );
		int GetAddSkillLevelByBuff() const;

		virtual void	UpdatePassiveAndActiveSkillState() {}

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		void CreateAndInsertReflectMagicByBuff( const KBuffIdentity& BuffIdentity_, float fValue_ );
		void EraseReflectMagicByBuff( const KBuffIdentity& BuffIdentity_ );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef WRITE_SCRIPT_LOG_AT_CONSOLE_WINDOW  // ����ȯ
		/// ��ũ��Ʈ���� �ܼ�â�� �α׸� ���� �� �ִ� ��� �߰�
		void WriteLogByConsoleWindow_LUA( const char* wstrLog_ );
#endif // WRITE_SCRIPT_LOG_AT_CONSOLE_WINDOW

#ifdef NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE
		// ������ ����Ʈ�κ��� �ҷ����� Callback �Լ�
		virtual void DieDamageEffectProc( const CX2DamageManager::DamageData& pDamageData_ ){}
#endif // NOTIFY_TO_OWNER_UNIT_WHEN_DAMAGE_EFFECT_DIE

//-------------------------------------------------------------------------------------------------------

	public:

//{{ robobeg : 2008-11-04

        const CKTDXCollision::CollisionDataListSet&           GetCollisionListSet() const { return m_CollisionListSet; }
        const CKTDXCollision::CollisionDataListSet&           GetAttackListSet() const { return m_AttackListSet; }
        const CKTDXCollision::CollisionDataListSet&           GetSubAttackListSet() const { return m_SubAttackListSet; }
		
				
		bool GetUnderWater() const { return m_bUnderWaterHead; }
		void ChangeUnderWater(bool bNewStatus)
				{
			if( m_bCheckChangeUnderWater == true )
			{
				if( m_bUnderWaterHead != bNewStatus )
					m_bChangeUnderWater = true;
				else
					m_bChangeUnderWater = false;
			}
			else
			{
				m_bCheckChangeUnderWater = true;
				m_bChangeUnderWater = false;
			}

			m_bUnderWaterHead = bNewStatus;
		}

		virtual float	GetEnchantResist( CX2EnchantItem::ENCHANT_TYPE enchantType );
		
		virtual void		ResetPhysicParam(bool bPreserveNowSpeed = false );
		virtual void		ApplyRenderParamOnEmptyRenderParamByBuff( CKTDGXRenderer::RenderParam* pRenderParam_ );
		virtual void DoProcessWhenKillEnemy( const CX2GameUnit* pDefender_ ) {}
		virtual void		ApplyWorldLightColor( OUT CKTDGXRenderer::RenderParam* pRenderParam_ ){}

//-------------------------------------------------------------------------------------------------------
	protected:
		virtual void		Verify();
		
#ifdef VERIFY_STAT_BY_BUFF
		bool		VerifyStatByBuff();
		template<typename T>
		bool VerifyVector( IN vector<T>& vector_ )
		{
			BOOST_FOREACH( T val, vector_ )
			{
				if ( NULL != val && false == val->Verify() )
					return false;
			}

			return true;
		}
#endif // VERIFY_STAT_BY_BUFF

		virtual void		InitDevice();
		virtual void		InitSystem();
		virtual void		InitStat();
		virtual void		InitPhysic();
		virtual void		InitComponent();
		virtual	void		DyingStart();
		virtual void		InitEtc() {};

		virtual void		SetSmallGage();
		void PhysicProcessSyncLine( const CKTDGLineMap::LineData* pLineData, D3DXVECTOR3& position );
		void CalcLastPositionValueOnAnimatedLine( const CKTDGLineMap::LineData* pLineData, D3DXVECTOR3& position );


		__forceinline void PlaySequenceByTriggerCount( CKTDGParticleSystem::CParticleEventSequence*  pParticleEventSequence, const D3DXVECTOR3& pos, float minEmitRate, float maxEmitRate, int triggerCount )
		{
			if ( pParticleEventSequence == NULL )
				return;

			pParticleEventSequence->SetPosition( pos );
			pParticleEventSequence->SetLandPosition( pos.y );	
			pParticleEventSequence->SetEmitRate( minEmitRate, maxEmitRate );
			pParticleEventSequence->SetTriggerCount( triggerCount );
		}

		__forceinline void PlaySequenceByTriggerCount( CKTDGParticleSystem::CParticleEventSequence*  pParticleEventSequence, float fPosX, float fPosY, float fPosZ, float minEmitRate, float maxEmitRate, int triggerCount )
		{
			if ( pParticleEventSequence == NULL )
				return;

			D3DXVECTOR3 pos = D3DXVECTOR3( fPosX, fPosY, fPosZ );
			pParticleEventSequence->SetPosition( pos );
			pParticleEventSequence->SetLandPosition( pos.y );	
			pParticleEventSequence->SetEmitRate( minEmitRate, maxEmitRate );
			pParticleEventSequence->SetTriggerCount( triggerCount );
		}

		__forceinline void PlaySequenceByTriggerTime( CKTDGParticleSystem::CParticleEventSequence*  pParticleEventSequence, const D3DXVECTOR3& pos, float minEmitRate, float maxEmitRate, float fTriggerTime )
		{
			if ( pParticleEventSequence == NULL )
				return;

			pParticleEventSequence->SetPosition( pos );
			pParticleEventSequence->SetLandPosition( pos.y );	
			pParticleEventSequence->SetEmitRate( minEmitRate, maxEmitRate );
			pParticleEventSequence->SetTriggerTime( fTriggerTime );
		}

		__forceinline void PlaySequenceByTriggerTime( CKTDGParticleSystem::CParticleEventSequence*  pParticleEventSequence, float fPosX, float fPosY, float fPosZ, float minEmitRate, float maxEmitRate, float fTriggerTime )
		{
			if ( pParticleEventSequence == NULL )
				return;

			D3DXVECTOR3 pos = D3DXVECTOR3( fPosX, fPosY, fPosZ );
			pParticleEventSequence->SetPosition( pos );
			pParticleEventSequence->SetLandPosition( pos.y );	
			pParticleEventSequence->SetEmitRate( minEmitRate, maxEmitRate );
			pParticleEventSequence->SetTriggerTime( fTriggerTime );
		}

		
		virtual float	GetBasicReducedDamagePercent() { return 0.0f; }

		virtual float 	GetDamageUpByAMADS() { return 0.f; }
		
		virtual void	GetDamageReduceByResistExtraDamage( CX2DamageManager::EXTRA_DAMAGE_TYPE damageType, float& fFinalDamage ) {}
		
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE		
		//{{ kimhc // 2010.11.3 // �Ž�ų �۾�
#ifdef	NEW_SKILL_2010_11
		// �� ������Ʈ���� Immune�Ǿ���� ExtraDamage�� ��ũ��Ʈ���� �о��
		void LoadImmunityAtThisStateFromScript( IN KLuaManager& luaManager_ );
#endif	NEW_SKILL_2010_11
		//}} kimhc // 2010.11.3 // �Ž�ų �۾�
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		bool ProcessEnchantExtraDamage( CX2DamageManager::DamageData* pDamageData, 
			CX2DamageManager::EXTRA_DAMAGE_TYPE basicExtraDamageType, CX2DamageManager::EXTRA_DAMAGE_TYPE miscExtraDamageType, float fRandomValue );

		virtual bool	ProcessEnchantBlaze( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_BLAZE );
		virtual bool	ProcessEnchantFrozen( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_FROZEN );
		virtual bool	ProcessEnchantPoison( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_POISON );
		virtual bool	ProcessEnchantPiercing( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_PIERCING );
		virtual bool	ProcessEnchantShock( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_SHOCK );
		virtual bool	ProcessEnchantSnatch( CX2DamageManager::DamageData* pDamageData, 
											CX2DamageManager::EXTRA_DAMAGE_TYPE damageType = CX2DamageManager::EDT_ENCHANT_SNATCH );

		virtual	void	PostProcessEnchantDamage( OUT CX2DamageManager::DamageData* pDamageData_ );

		virtual CX2EnchantItem::ENCHANT_TYPE CX2GameUnit::GetEnchantTypeFromExtraDamageType( CX2DamageManager::EXTRA_DAMAGE_TYPE edtType );
		virtual void	ApplyEnchantResist( CX2EnchantItem::ENCHANT_TYPE enchantType, float& fDamage );
		virtual bool	IsImmuneToEnchant( CX2EnchantItem::ENCHANT_TYPE enchantType );
#ifdef PET_AURA_SKILL
		virtual float	GetEnchantAttack( CX2DamageManager::EXTRA_DAMAGE_TYPE extraDamageType );
#endif
#ifdef ENCHANT_ACCUMULATION_RATE_MODIFIER
		float EnchantAccumulationRateModifier( CX2DamageManager::EXTRA_DAMAGE_TYPE basicExtraDamageType, CX2DamageManager::EXTRA_DAMAGE_TYPE miscExtraDamageType, float fRate );
#endif ENCHANT_ACCUMULATION_RATE_MODIFIER
		

#ifdef SKILL_30_TEST
		void CommonFrameMoveStateAbnormality();
		virtual void CommonFrameMoveStateAbnormalityFrozen() = NULL;
		void CommonFrameMoveStateAbnormalityEntangled();
		void CommonFrameMoveStateAbnormalityTimeBomb();
#endif SKILL_30_TEST
		void CommonFrameMoveStateAbnormalityAging();
		void CommonFrameMoveStateAbnormalityStigmaDebuff();
#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.8] // �ε� ����Ʈ - ���� ���� - EDT_BLIND
		void CommonFrameMoveStateAbnormalityBlind();
#endif NEW_SKILL_2010_11

#ifdef EVE_ELECTRA
		virtual void CommonFrameMoveStateAbnormalityFlashBang() = NULL;
#endif EVE_ELECTRA

#ifdef EDT_WATER_HOLD_TEST
		virtual void CommonFrameMoveStateAbnormalityWaterHold() = NULL;
#endif EDT_WATER_HOLD_TEST

		

		//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
		void CommonFrameMoveStateAbnormalityPanic();
		void CommonFrameMoveStateAbnormalityPain();
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
		//}} 2011-04 �� ��ġ�� û ĳ�� ��ų

#ifdef CHUNG_SECOND_CLASS_CHANGE
		void CommonFrameMoveStateAbnormalityLegShot();
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
		void CommonFrameMoveStateAbnormalityEarthQuake();
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

#ifdef SERV_SKILL_NOTE
		void CommonFrameMoveAnimSpeedFactor();
		void CommonFrameMoveAnimSpeedFactorEffect(SPEED_FACTOR speedFactor);
		void CommonFrameMovePoisonDeBuff();
		void CommonFrameMoveRevengeDefence();
#endif

		void CommonFrameMoveMoveSpeedFactor();
		void CommonFrameMoveJumpSpeedFactor();
		void CommonFrameMoveMoveSpeedFactorEffect(SPEED_FACTOR speedFactor);		
		void CommonFrameMoveJumpSpeedFactorEffect(SPEED_FACTOR speedFactor);

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void ParseAnimNameFromList( wstring &animName, int iRandRate );
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool IsDamagedReaction( const CX2DamageManager::DamageData* pDamageData );


#ifdef SERV_SKILL_NOTE
		float GetDefenceMemoTime() { return m_fRevengeDefenceTime; }
		void  SetDefenceMemoTime(float fVal) { m_fRevengeDefenceTime = fVal; }
		float GetEnduranceAttackTime() { return m_fEnduranceAttackTime; }
		void  SetEnduranceAttackTime(float fVal) { m_fEnduranceAttackTime = fVal; }
		float GetPoisonDeBuff() { return m_fPoisonDeBuff; }
		void  SetPoisonDeBuff(float fVal) { m_fPoisonDeBuff = fVal; }
#endif



		virtual bool DoScaleHeadBone() { return true; }	/// FieldFix: OnFrameMove���� ������ �� ������

//{{ kimhc // 2010.6.16 
// 1. GameUnit::Weapon::SetEnchantParticleShow() �� GUUser::SetShowEnchantedWeaponEffectAtHand()��
// ���� ���� ��찡 �����Ƿ� �ѵ� ����
// 2. NotifyShowObjectChanged()������ ���� ���� ȣ���ϵ��� �Ǿ��ִ°��� �Ѱ������� ȣ���ϵ��� ����
// 3. virtual�� ���� GameUnit������ GameUnit::Weapon::SetEnchantParticleShow()�� ȣ���ϰ�
// GUUser������ SetShowEnchantedWeaponEffectAtHand()�� ȣ���ϵ��� ����
#ifdef	ENCHANT_BUG_TEST
		virtual void SetShowEnchantWeaponParticle( bool bShow );
#endif	ENCHANT_BUG_TEST
//}} kimhc // 2010.6.16 

		//{{ kimhc // 2010.11.1 // �ε峪��Ʈ - ���� ����
		
		static	bool GreaterExtraDamageTime( CX2DamageManager::ExtraDamageData* pFirst, CX2DamageManager::ExtraDamageData *pSecond );
		void CureExtraDamage( int iNumOfExtraDamageToBeCure_ = 1 );

		//{{ 2011-04 �� ��ġ�� û �޸� ��弦
#ifdef	CHUNG_MEMO_01				
		bool GetHittedExtraDamageHeadShot() const { return m_bHittedExtraDamageHeadShot; }
		void SetHittedExtraDamageHeadShot( bool bHittedExtraDamageHeadShot_ ) { m_bHittedExtraDamageHeadShot = bHittedExtraDamageHeadShot_; }
		void ApplyHeadShotDamage( float* pfFinalDamage_ );
#endif	CHUNG_MEMO_01				
		//}} 2011-04 �� ��ġ�� û �޸� ��弦

#ifdef NEW_HENIR_TEST

#ifndef ADD_HENIR_BUFF		/// ������ ����Ǹ鼭 ������ �ʰ� �� ����
		float GetTimeHenirBuffLight(){ return m_fTimeHenirBuff_Light; }
#endif ADD_HENIR_BUFF

#endif NEW_HENIR_TEST
	
		void UpdateVisibilitySmallGageAndName();

		void DoDelegateProcess( const vector<DelegateProcess>& vecDelegateProcess_ );
		void DoDelegateProcessWithDamageData( const vector<DelegateProcessWithDamageData>& vecDelegateProcess_, const CX2DamageManager::DamageData& damageData_ );

		void AddToDamagedMap( const CX2GameUnit* pAttackerGameUnit_, const float fFinalDamage_ );
		UidType GetGameUnitUIDMaxDamage() const;

public:
		virtual void	AnimStop() = 0;
		virtual void	AnimPlay() = 0;
		virtual void SetCanPassUnit( const bool bFlag_ ) = NULL;
		bool GetShowSmallGageAndName() const { return m_bShowSmallGageAndName; }
		void SetShowSmallGageAndName( bool bShowSmallGageAndName_ ) { m_bShowSmallGageAndName = bShowSmallGageAndName_; }

#ifdef ELSWORD_WAY_OF_SWORD
		void CureExtraDamage( CX2DamageManager::EXTRA_DAMAGE_TYPE eExtraDamageTypeToCure_ );
#endif ELSWORD_WAY_OF_SWORD

		UCHAR GetHittedCount() const { return m_ucHittedCount; }
		void SetHittedCount(UCHAR val) { m_ucHittedCount = val; }

		UCHAR GetHitCount() const { return m_ucHitCount; }
		void SetHitCount(UCHAR val) { m_ucHitCount = val; }

		UCHAR GetNumOfDeBuff() const { return m_ucNumOfDeBuff; }
		void SetNumOfDeBuff(UCHAR val) { m_ucNumOfDeBuff = val; }

		UCHAR GetNumOfBuff() const { return m_ucNumOfBuff; }
		void SetNumOfBuff(UCHAR val) { m_ucNumOfBuff = val; }

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		DWORD	GetFrameMoveCountFuture() const { return m_dwFrameMoveCountFuture; }
//		void	SetFrameMoveCountFuture(DWORD val) { m_dwFrameMoveCountFuture = val; }
//
//		DWORD	GetFrameMoveCountNow() const { return m_dwFrameMoveCountNow; }
//		void	SetFrameMoveCountNow(DWORD val) { m_dwFrameMoveCountNow = val; }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
        void	OnFrameMoveBuff( float fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		void	OnFrameMoveBuff();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		virtual void	ApplyRenderParam( CKTDGXRenderer::RenderParam* pRenderParam_ );
		void	FinishAndClearAllBuff();

		UINT	GetUnitSlashTraceDataSize() const
		{
			if ( NULL != m_pUnitSlashTraceManager )
				m_pUnitSlashTraceManager->GetUnitSlashTraceDataSize();
			else
				return 0;
		}

		void	DoUnitSlashTraceInCommonStateStart();
		void	AddSlashTraceTiming();
#ifdef BALANCE_CODE_NEMESIS_20121213
		virtual void ModifyFinalDamageByGameUnitType( OUT CX2DamageManager::DAMAGE_TYPE& damageType_, OUT float& fAttackerPhysicDamage_, OUT float& fAttackerMagicDamage_, OUT float& fForceDownValue_ ) {}
#else
		virtual void ModifyFinalDamageByGameUnitType( OUT CX2DamageManager::DAMAGE_TYPE& damageType_, OUT float& fAttackerPhysicDamage_, OUT float& fAttackerMagicDamage_ ) {}
#endif //BALANCE_CODE_NEMESIS_20121213

		bool	ApplyDrainHpBuff()
		{
			const float fDrainHpRate = m_AdditionalDrainHpNormalAttack.GetResultValue();
			if( fDrainHpRate > 0.f && m_DamageData.attackType != CX2DamageManager::AT_SPECIAL )
			{
				m_DamageData.m_fDrainHpRate = fDrainHpRate;
				return true;
			}

			return false;
		}

#ifdef BALANCE_BLADE_MASTER_20130117		/// ü�� ���ġ ��ȯ �Լ�
		float	GetDrainHpBuffValue( CX2DamageManager::DamageData* pDamageData_ )
		{
			const float fDrainHpRate = m_AdditionalDrainHpNormalAttack.GetResultValue();

			if( NULL != pDamageData_ && fDrainHpRate > 0.f && pDamageData_->attackType != CX2DamageManager::AT_SPECIAL )
			{
				pDamageData_->m_fDrainHpRate = fDrainHpRate;
				return fDrainHpRate;
			}

			return 0.f;
		}
#endif BALANCE_BLADE_MASTER_20130117

		virtual void SetStateToNowAnimation( const CKTDGXSkinAnim::XSKIN_ANIM_STATE eAnimState_ ) = NULL;
		
		virtual float GetNowAnimationTime() const = NULL;
		virtual void SetNowAnimationTime( const float fAnimCurrentTime_ ) = NULL;

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		void SetToDamagedMap ( const UidType uidType_, const float Damage_ );		// �ش� ���ֿ� ����� ���� ����, Uid �� �˻�
		void AddToDamagedMap ( const UidType uidType_, const float Damage_ );		// �ش� ���ֿ� ����� ���� ����, Uid �� �˻�
		const map<UidType,float>& GetDamagedMap() const { return m_DamagedMap; }			// DamagedMap �� ��ȯ
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef MODIFY_EFFECT_SET_DELETE
#ifdef  ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��
		void AddEffectSetDeleteCondition( CX2EffectSet::Handle hEffectSetHandle_, const vector<DELETE_CONDITION_EFFECT_SET> & eDeleteCondition );
#else   ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��
        void AddEffectSetDeleteCondition( CX2EffectSet::Handle hEffectSetHandle_, DELETE_CONDITION_EFFECT_SET eDeleteCondition );
#endif  ADD_TO_EFFECT_SET_LIST_EX_DELETE_CONDITION	  // ������, EFFECT_SET_LIST_EX ���̺��� ���� ���� �߰� Ȯ��
#endif //MODIFY_EFFECT_SET_DELETE 
		
		virtual void DamageDataChangeProcess(){}
//#ifdef INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
		//virtual void ParsingBuffFactorID( KLuaManager& luaManager_ ){}
		//virtual void CommonFrameMove_InsertBuffFactor(){}
//#endif // INSERT_BUFF_FACTOR_BY_SCPRIPT_STATE
#ifdef FIELD_BOSS_RAID
		virtual bool GetIsBosRaidNPC() const { return false; }
		virtual bool GetIsMiddleBosRaidNPC() const { return false; }
		virtual bool GetIsImmuneDebuff() const { return false; }
		virtual bool GetIsImmuneAllEDT() const { return false; }
#endif // FIELD_BOSS_RAID

#ifdef SERV_9TH_NEW_CHARACTER		
		virtual float	GetChangeDamageRateByAttackerSkill( const CX2DamageManager::DamageData* pDamageData_ ) const { return 1.f; }
		const D3DXCOLOR GetFlickerColorByBuff( IN const D3DXCOLOR& coRenderColor_, IN const float fFlickerGap_ );
#endif // SERV_9TH_NEW_CHARACTER

		// Public Member Start
	public:
		vector<UidType>							m_KillUserUnitList;		/// ų�� ������ �ε�.. PVP������ ����
		vector<int>								m_KillNPCList;			/// ��� ����

		vector<UidType>							m_MaxDamageKillUserUnitList;	/// �ǹ̴� ���� ���� �� �߿� �ڽ��� �ִ� �������� �� �� ������.. �߸� ���̰� �ִ� ��...
		vector<int>								m_MaxDamageKillNPCList;	/// ��� ����
		
		vector<CX2GameUnit*>					m_HitUnitList;		/// kimhc // �ʿ� ������ �𸣰���
		int										m_iDieCount;		// ������ //���� �÷��� �߿� die ī��Ʈ�� Ȯ���ϱ� ���� �߰�
		// Public Member End

		//-------------------------------------------------------------------------------------------------------
	protected:

		CKTDXCollision::CollisionDataListSet            m_CollisionListSet;	/// �̰� �ǰݹڽ� �ڰ�..
		CKTDXCollision::CollisionDataListSet            m_AttackListSet;	/// �̰� Ÿ�� �ڽ� �ڰ�..
		CKTDXCollision::CollisionDataListSet            m_SubAttackListSet;	/// �̰�..�Ƹ��ٵ� ���̵� ��...?

		typedef std::pair<CKTDGXMeshPlayer::CXMeshInstanceHandle, CKTDXCollision::CollisionDataListSet> PairSubAttackListSet;
		vector<PairSubAttackListSet>					m_vecPairSubAttackListSet;	/// first���� Mesh�� �ڵ�, second���� CollisionDataListSet

#ifndef X2OPTIMIZE_NPC_NONHOST_SIMULATION
		float m_fCommonDamageChangeTime; // ��ũ��Ʈ�� DamageData ���� ���� 
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
		//}} robobeg : 2008-11-04
	
//-------------------------------------------------------------------------------------------------------

private:
		KProtectedType<float>							m_fMaxHyperModeTime;	/// �ִ� ���� �ð�
		//KProtectedType<float>							m_fRemainHyperModeTime;	/// �����ִ� ���� �ð�

//-------------------------------------------------------------------------------------------------------
	
protected:

		GAME_UNIT_TYPE									m_GameUnitType;		/// User���� NPC������ ���� �ϴ� Ÿ��
		int												m_UnitIndex;		/// NPC�� ��� m_NPCUnitList���� unit�� �����ϴ� ��ġ�� index(����ִ°�), User�� ��� slotIndex �̴�.
		UidType											m_UnitUID;			/// Unique ID
		KProtectedType<int>								m_TeamNum;			/// Team �з� �ѹ���, ���������� ���� CX2Room::TN_RED �̴�.
//#ifdef PVP_BOSS_COMBAT_TEST
//		int												m_TeamNumOriginal;		
//#endif PVP_BOSS_COMBAT_TEST
#ifdef	SERV_TRAPPING_RANGER_TEST
		CX2Room::TEAM_NUM								m_eAllyTeam;
#endif	SERV_TRAPPING_RANGER_TEST

		wstring											m_ScriptFileName;	/// �ش� Unit�� Script ���ϸ� char* �̾ �� ��.. 
		
		float											m_fTime;			/// OnFrameMove�� ���� �Ѱܹ��� fTime ���� ����
		float											m_fElapsedTime;		/// �߱����������� �ʿ� ���� �ߴµ�... Ktdxapp�� �ִ°� �׳� ����ϸ� �Ǵϱ�...

		KLuaManager										m_LuaManager;		/// ��ƸŴ���, �����ڸ���Ʈ���� �ʱ�ȭ ��Ų�� �� �ѵ�..

		//PhysicParam										m_ScriptedPhysicParam;		/// ��ũ��Ʈ�� ������ ���� physic param
		PhysicParam										m_OrgPhysicParam;	/// Original�� ����� ���̶�� �� Original �ΰ�..?
		PhysicParam										m_PhysicParam;		/// ������ PhysicParam... �� ������ ����.../

		CX2DamageManager::DamageData					m_DamageData;		/// User �Ǵ� NPC ��ũ��Ʈ�� �ִ� DamageData
		
		float											m_fRevengeAddDamage;	/// �ιݰݿ� �ʿ��� ������ �� ������ (DamageReact�� ��� �����ϴ��Ŀ� ���� ���ҵ忡�Ը� �־ �� ��
		int												m_DamageReducePercent;	/// �������� ���ҽ����ִ� Percent��, ����� û ����, ���̺� ���ϵ�����, PVP ����� SPIKA �� ��� �ϰ� ����
        int                                             m_DamageGuardPercent;	/// ����� NPC������ ����ϴ� ������ ���ҿ�




#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
		int			m_iLinkJumpTargetLineIndex;
		D3DXVECTOR3 m_vLinkJumpTarget;
		D3DXVECTOR3 m_vLinkJumpSpeed;
		D3DXVECTOR3 m_vLinkJumpAcceleration;	// �߷°��ӵ��� ������ ���ӵ�
		float		m_fLinkJumpTime;
		float		m_fElapsedTimeAfterLastLinkJump;
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST


		
		CX2OldGageManager								m_GageManager;
		CX2GageData*									m_pGageData;		/// HP, MP, ĳ���� �̹��� ���� ������ ������ �Ӹ��� �ƴ϶�, �� ������ �ƴ� ��쿡�� ��Ƽ�� ������, NPC���� ��쿡�� NPC ������ ���� ������ ����(?) ������....
		CX2GageUI*										m_pGageUI;

		CKTDGXRenderer::RenderParam						m_RenderParam;	/// �ܰ���, alpha ���� ���� �����ϴ� RenderParam
		//vector<CKTDGXRenderer::RenderParamPtr>			m_vecRenderParamPtr;

		CKTDGPlanShadow*								m_pUnitShadow;	/// ������ �׸���.. �������δ� ��ũƮ���� ���� (��ü�� shadow.dds)


		/// ���� ���� ����Ʈ �κ��� �� ������ ���� ���� ������..
		/// kimhc // �� �ڵ���� ���� ���������� �Űܺ���...
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHyperBoostRFoot;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHyperBoostLFoot;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHyperBoostRArm;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hHyperBoostLArm;

		
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hQuestionMark;	/// NPC�� ���, ������ ���ҵ� �ܿ��� ������� �ʴ´�.
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hExclamationMark;	/// NPC�� ���, ������ ���ҵ��� �ݰݿ� ������ �� ���
		
		wstring												m_HeadBoneName;		/// �̰� NPC ���׸� �־ �� �� ����..

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		vector<D3DXVECTOR2>								m_AttackTimeList;		/// ���� Ÿ��, ������ üũ�� �����ϴ� AnimationTime
		vector<wstring>									m_AttackProjSeqName;	/// ���ݽ� ǥ�� �Ǵ� �Ǽ��� Particle�� �̰����� ��Ÿ���� ������.. ����� ���� ������... NPC���� �������� �ʳ�..

		/// Stop �κ��� �ʵ忡�� ��� ����...
		vector<D3DXVECTOR3>								m_StopAllList;
		vector<D3DXVECTOR2>								m_StopOtherList;
		vector<D3DXVECTOR2>								m_StopMyList;
		vector<D3DXVECTOR3>								m_Stop2AllList;
		vector<D3DXVECTOR2>								m_Stop2OtherList;
		vector<D3DXVECTOR2>								m_Stop2MyList;

		vector<SoundPlayData>							m_vecSoundPlayData;		/// Sound ����Ÿ
		vector<SoundPlayData>							m_vecSoundPlayLoop;		/// NPC ���߿��� ���� ����� �۸��� ���� ����� POT ������ ���� ��� ��
		vector<CKTDXDeviceSound*>						m_vecLoopSound;		/// �� ������Ʈ�� �ִ� LOOP_SOUND�� NPC�� �װų�, STOP_LOOP_SOUND�� true �� ������Ʈ�� ������ �� �����
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		CX2ComboManager*								m_pComboManager;	/// �ڱ� ������ ������ �Ǵ� ���� ��.. �ٸ� ������ NPC �� ����.. �����ϴ°�..
//{{ robobeg : 2013-11-04
		//CX2Stat*										m_pStat;			/// ������ ������ �� ������ ���� �Ǵ� �� �˾Ҵµ�.. �ƴϿ���.. g_pData->GetMyUser()->GetUnitByUID()�� m_pUnit�ε� �̰��� m_pUnit->GetUnitData()->m_GameStat �� m_Stat�� �����
        CX2Stat                                         m_Stat;
//}} robobeg : 2013-11-04

		float											m_DamageLightTime;	/// Ÿ���� �ϰų� �ǰ��� ������ ��� �ش� �ð� ���� World�� Light ��ġ�� ���� ��Ų��
		bool											m_bAttackedByUserUnit;	/// �ڽ��� Ÿ���� ������ ��������.. (�׾��� ���� ���� ��ü�� �������� �Ǵ��� �� ���) ��� ���ϴ� ����� ����..?
		//UidType											m_AttackerUnitUID;		/// �ڽ��� Ÿ���� ������ UID

#ifdef SERV_PET_SYSTEM
		//UidType											m_LastAttackUnitUID;	/// �ڽ��� ���������� Ÿ���� ����
#endif

		CX2GameUnitoPtr									m_optrGameUnitAttackedByMe;		/// �ڽſ��� Ÿ�� ���� ����
		CX2GameUnitoPtr									m_optrGameUnitWhoAttackedMe;	/// �ڽ��� Ÿ�� �� ����

		wstring											m_ShadowFileName;		/// �׸��ڷ� ����� ���� ��, �׷��� �̰��� ��� ������ �־�� �� ������ ����
		float											m_fShadowSize;			/// �׸����� ������ �ε� �ѵ�... �̰͵� ��� ������ �־�� �� ������ ����
		float											m_fDieAfterTime;		/// ����� ���������� ����ϴ� ������ �ٲپ� ���� ��??? HP < 0 �̸� DungeonGame�� �ٷ� ���...??
		bool											m_bDyingStart;			/// �״� ������Ʈ�� ���� �ߴ���.. �׷��� NPC���״� �ʿ������� �𸣳� User���״�...??


		// �������� �Ʒ� ���� �κе��� (state invincle �ƴ�) User���Ը� ���Ǿ��µ� PVP_BOT�� ���鼭 ����Ǿ���... �����°� ���� ������...?
		KProtectedType<float>							m_fForceInvincibleTime;	/// ���� �ð�, �� ������Ʈ�� INVINCIBLE Ű���� ���� �켱 �ϴ�

		KProtectedType<float>							m_fShowInvincibleTime;	/// ���� �ð����� ���� �������� �����ִ� �ð�

		float											m_fShowInvincibleSwapTime; /// ���� ���� �ð����� ����� �Ϲ� ������ �����̴µ� �� Ÿ�̹� (�뷫 0.05��)

		bool											m_bShowInvincibleWhite;	/// m_fShowInvincibleTime > 0 �� ���� �Ͽ� m_fShowInvincibleSwapTime�� <= 0 �� �� �� ���� ��� �����ش�.

		bool											m_bUpMPThisFrame;	/// �� ������ ������ �ι� MP�� UP���� �ʵ��� �Ϸ��� �ϴ� �� ������... �̰� �ʿ� �Ѱ�...? CommonFrameMove�� false�� �������شٴ°�.... �������ӿ� �θ��� ���� ��쿡�� �ι��� MP UP�� ����Ǵ°� �´°� ������... ������ ���� �� ����. (DamageEffect������...?)
		float											m_fCanNotAttackTime;	/// �� ������Ʈ�� EventProcess���� ���� ���õ� ��ư �Է��� ���� �ʴ� ������ ����µ� ���. �̰� ����.. ������ �ʿ��ϴ�.
		float											m_fCanNotJumpTime;	/// Jump ���� Ű�Է��� ���� �ʴ� �ð�. �̰͵� ������ �ʿ��ϴ�.
		float											m_fCanNotInputTime;	/// ��� Ű �Է��� ���� �ʴ� �ð�. �̰͵� ������ ��� ��
#ifdef MODIFY_DUNGEON_STAGING
		bool											m_bCanNotInput;
#endif //MODIFY_DUNGEON_STAGING
		float											m_fCanNotInputTimeZXArrow;	/// ������ ��� ���� ������ �� ���� 0 �̻��̸� ZXArrow ���� �� Ű�� init ��Ŵ (Portal, Bungee ��� ����ϴ� ��)

		CX2DamageManager::HITTED_TYPE					m_HittedType;	/// �� ���� ���� Ÿ�� ������ ���� ���尡 �޶�����
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
//{{ kimhc // 2011.1.21 // û 1�� ����
#ifdef	CHUNG_FIRST_CLASS_CHANGE
		CX2DamageManager::HITTED_TYPE					m_eHittedTypeAtState;	// ������Ʈ �� HittedType ����, �̰͵� �ϴ��� û�� ��� �ϴ� �ɷ� �˰� �ֱ� �ѵ�...
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.21 // û 1�� ����
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		int												m_FlyCount;		/// ����̳� �������� �����ϴ� ȸ����, �� ���� Ŭ���� �ߴ� ���̰� �۾��� ��RT_UP, RT_FLY, RT_DRAG_UP ���ݿ� ���� ���� ���� �� m_FlyCount�� ����)

		map<UidType,float>								m_DamagedMap;	/// �ڽſ��� �������� �� ������ UID�� �������� �����ϰ�, ���� ���� �������� �� ������ UID�� ������ ����
		
		float											m_fScale;	/// ������ �������� �����ϱ� ���� ���� �� ������.. NPC�� ��� ��...
		
		bool											m_bUseWorldColor;	/// û�� ���� �� BEE_BOMB(?) �� ��� ��... ���� true�� Ư���� ��� RenderParam ���� �ٲܶ� false�� ���

		vector<CX2BuffTempletPtr>						m_vecBuffTempletPtr;		/// ������ ������ �� �ִ� Ű�� �ش� ���ֿ��� �ɸ� ����(map���� ���� ���� ������ find ���ٴ� loop�� �� ��� ����)
		
		/// �׻� ������ �Ŵ� �������
		
		/// Ÿ�ݽ� ������ �Ŵ� �������
		/// �ǰݽ� ������ �Ŵ� �������
		/// ȸ�ǽ� ������ �Ŵ� �������
		/// ������� �ɸ��� ������ �Ŵ� �������
		
		/// Ÿ�ݽ� �ڽ����� �Ŵ� �������
		/// �ǰݽ� �ڽ����� �Ŵ� �������
		/// ȸ�ǽ� �ڽ����� �Ŵ� �������
		/// ������� �ɸ��� �ڽ����� �Ŵ� �������

		vector<CX2BuffFactorPtr>						m_vecBuffFactorPtr;				/// ������ �ִ°�(���, ��, Īȣ � ���ؼ� �׻����� �ִ� ��)
		vector<CX2BuffFactorPtr>						m_vecTemporaryBuffFactorPtr;	/// ������ �ִ°�(����� ���� �Ͻ����� ��)

		//vector<CX2BuffFactorPtr>		/// ������ �ִ°ǵ� �Ͻ����ΰ�
		//vector<CX2BuffFactorPtr>		/// ������ �ִ°ǵ� �Ͻ����ΰ�

// 		vector<CX2BuffFactorPtr>						m_vecEquipBuffFactorToOtherPtr;			/// ��� ���ؼ� ������ �ɼ��ִ� �������
// 		vector<CX2BuffFactorPtr>						m_vecTitleBuffFactorToOtherPtr;			/// Īȣ�� ���ؼ� ������ �ɼ��ִ� �������
// 		vector<CX2BuffFactorPtr>						m_vecPetBuffFactorToOtherPtr;			/// �꿡 ���ؼ� ������ �ɼ��ִ� �������
// 		vector<CX2BuffFactorPtr>						m_vecBuffTempletBuffFactorToOtherPtr;	/// �ڽ����� �ɸ� ������ ���ؼ� ������ �ɼ��ִ� �������
// 
// 		vector<CX2BuffFactorPtr>						m_vecEquipBuffFactorToMePtr;			/// ��� ���ؼ� �ڽ����� �ɼ��ִ� �������
// 		vector<CX2BuffFactorPtr>						m_vecTitleBuffFactorToMePtr;			/// Īȣ�� ���ؼ� �ڽ����� �ɼ��ִ� �������
// 		vector<CX2BuffFactorPtr>						m_vecPetBuffFactorToMePtr;				/// �꿡 ���ؼ� �ڽ����� �ɼ��ִ� �������
		
		ExtraDamagePack									m_ExtraDamagePack;	/// ExtraDamage ����ü.. ������..
		CX2SocketItem::SocketData						m_SocketData;		/// ���� �ɼ�, NPC�� �̰� �� ������ �ִ�..
		map< CX2EnchantItem::ENCHANT_TYPE, int >		m_mapDefenceEnchantData;	/// ������ EnchantData
#ifdef PET_AURA_SKILL
		map< CX2EnchantItem::ENCHANT_TYPE, float >		m_mapAttackEnchantData;		/// �ϴ� NPC���Դ� �ʿ� ���� ���̴µ�.. ��..
#endif


		std::vector< Weapon* >							m_vecpWeapon;	/// ���� ��ü ����Ʈ


		//Minor
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPoisonBody;	/// m_ExtraDamagePack�� ������ �ɷ��� �� ������ ��ƼŬ �ڵ�... ������ �ɸ��� �ɸ��� ������ ������ �־�� �ϳ�..
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqFireBody1;	/// m_ExtraDamagePack�� Fire �Ǵ� Blaze�� �ɷ��� �� ������ ��ƼŬ �ڵ� (Minor)
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqFireBody2;	/// m_ExtraDamagePack�� Fire �Ǵ� Blaze�� �ɷ��� �� ������ ��ƼŬ �ڵ�	(Minor)

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCurseBody;	/// ����
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSlowBodyRight;	/// ���ο� �ɷ��� �� ���� �ٸ��� �ٴ°�...
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqSlowBodyLeft;	/// ���ο� �ɷ��� �� ���� �ٸ��� �ٴ°�...
				
		// ���ϴ� clod�� EnchantFrozen�� �ɷ��� ��..
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold1;	
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold2;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold1_Right;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold2_Right;
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqCold_Head;

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPepperBody1;	/// û�����
		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqPepperBody2;	/// û�����

		CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqStun;		/// ���� (���� �����ε�?)

		// �ٵ�.. ����ġ�� ���� NPC�� �ֳ�...?
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqHPUp;	/// ����ġ
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqMPUp;	/// ����ġ


		

		//////////////////////////////////////////////////////////////////////////////
		// ����Ʈ�� ����� �������Ը� ���͵� �Ǵ°��� �����ϴ°� ���?
		//////////////////////////////////////////////////////////////////////////////

		// Major
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Stun;	/// ��þƮ����(��ȭ??)
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Exclamation;	/// ��þƮ ��ø 1��
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Exclamation2;	/// ��þƮ ��ø 2�� �̻�
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Exclamation3;	/// ��þƮ ��ø 3�� �̻�

		/// ������ ������ ----------------------------------------------------------------
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Blaze_Second;
		/// -------------------------------------------------------------------------------
		
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Frozen_Second;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Poison_Second;
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_Stun_Second;
		
		// �����̻� �̸�Ƽ��
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqBleeding;	/// �̸��� ���긮�� ��
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSkeletonHead;		/// ������ź �ذ�
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqStunAbnormality;	/// ���� �ɷ��� ����µ�... �Ķ��� �˱۹���.. ����Ÿ�� ���� ���� ������ �� ������ ��
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqAging;			/// ��ȭ�� �� ǥ��
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hStigma;	/// ���� ��ũ
		CX2EffectSet::Handle								m_hEffSetDryad;	/// ���� ������ ����� �����ٱ� ����Ʈ
#ifdef SERV_SKILL_NOTE
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hRevengeDefence;	/// ���ҵ� �ݰ� �޸� ���� �� ������...
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hPoisonDeBuff;	/// �� ������ �̹����� �������� ������ �� (m_ExtraDamagePack.m_Poison, �ٸ� �������ϰ� ���� �ٸ��ž�?)
#endif
#ifdef NEW_SKILL_2010_11 // oasis907 : ����� [2010.11.8] // �ε� ����Ʈ - ���� ���� - EDT_BLIND
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqBlind;	/// �γ� ���彺�迡 ���� ����� �̹���...(��ȸ�� ���� ����� �μ��� �̹���??)
#endif NEW_SKILL_2010_11

#ifdef EDT_WATER_HOLD_TEST// lucidash : ����� [2010.1.4] 
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWaterHold1;	/// ���� ����? ����� ����Ʈ
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqWaterHold2;	/// ���� ����� ����Ʈ������ ���� ��...
#endif EDT_WATER_HOLD_TEST

		//{{ 2011-04 �� ��ġ�� û ĳ�� ��ų
#ifdef	CASH_SKILL_FOR_CHUNG_2011_04				
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqPanic;	/// �¿찡 ��� �ٱʹ� ȥ��
#endif	CASH_SKILL_FOR_CHUNG_2011_04				
		//}} 2011-04 �� ��ġ�� û ĳ�� ��ų

#ifdef CHUNG_SECOND_CLASS_CHANGE
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqLegShot;	/// ������ ǥ��..
#endif
#ifdef HAMEL_NEW_SETOPTION_EXTRADAMAGE
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEarthQuakeRight;	/// �̵��ӵ��� �ִϸ��̼��� �Ѵ� ������ �ϴ� ȿ���� ����Ʈ �ڵ�
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEarthQuakeLeft;	/// �̵��ӵ��� �ִϸ��̼��� �Ѵ� ������ �ϴ� ȿ���� ����Ʈ �ڵ�
#endif HAMEL_NEW_SETOPTION_EXTRADAMAGE

		// �����̻� ����Ʈ��
		CX2EffectSet::Handle m_hEffSetStateAbnormalityFrozen;	/// m_ExtraDamagePack.m_Frozen �� �ɷ��� ��.....

#ifdef RAVEN_WEAPON_TAKER
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqSmokeBlind;
#endif RAVEN_WEAPON_TAKER

#ifdef GUIDE_QUEST_MONSTER
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqQuestMonster; // ����Ʈ ���� ǥ�� ����Ʈ
#endif //GUIDE_QUEST_MONSTER

		CX2DamageManager::EXTRA_DAMAGE_TYPE				m_eWeaponEnchantExtraDamageType;	/// ������ ��þƮ �Ӽ�
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMajorMeshPlayerList[10];	/// �̰� ��ƿ��� Ư�� ���� ��� ���ε�.. �������� ���ֱ�� �����...
		CKTDGXMeshPlayer::CXMeshInstanceHandle			m_hMinorMeshPlayerList[10]; /// �̰� ��ƿ��� Ư�� ���� ��� ���ε�.. �������� ���ֱ�� �����...


#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		vector<ParticleEffectData*>						m_ParticleEffData;	/// NPC�� ��� �ϴ� ��
		vector<MeshPlayerEffectData*>					m_MeshPlayerEffData;	/// NPC�� ����ϴ� ��ó�� ���̳� ���� ��� �ϴ� ���� ���� ��
		vector<CameraCrashData*>						m_CameraCrashData;	/// NPC�� ��� �ϴ� ��
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


		bool											m_bIgnoreLineSpeed;			/// ���ο� ������ ���ǵ带 �������� ���� (�⺻ false, pos += dirvector * speed * elapsedTime)

#ifdef FOG_WORLD
        bool                                            m_bFog;		/// �ɼ��� HIGH�� ���¿��� true�� fog ���, NPC�� ��� ��
#endif
		
#ifdef VERIFY_STAT_BY_BUFF
		PROTECT_VECTOR3									m_vBasicScale;	/// ������ �⺻ �������� ��Ÿ������ �� ���� �ѵ�...					
		PROTECT_VECTOR3									m_vTransScale;	/// ������ ������ ũ��
#else	// VERIFY_STAT_BY_BUFF
		D3DXVECTOR3										m_vBasicScale;	/// ������ �⺻ �������� ��Ÿ������ �� ���� �ѵ�...					
		D3DXVECTOR3										m_vTransScale;	/// ������ ������ ũ��
#endif // VERIFY_STAT_BY_BUFF

		D3DXVECTOR2										m_vOrgUnitSize;

		float											m_fBasicBoneScale;

		float											m_fTimeDirectToReturnBasicScale;	/// �� ���� ���� m_vTransScale�� ���ݾ� Ŀ���ٰ� 0���ϰ� �Ǵ� ���� ���� ũ��� ���ƿ�
		float											m_fTimeToReturnBasicScale;			/// ���� ũ��� ���ư� �� ���� �ɸ��� �ð�..(���ӽð�)

		bool											m_bCheckWidthEnlargeSize;		/// �̰͵� ������ ������ ���õ� ����
		bool											m_bCheckHeightEnlargeSize;		/// �̰͵� ������ ������ ���õ� ���� ... �ٵ� false�� �Ѵ� ��Ű�µ�.. (TransformScale����) true�� if, else�� �ϳ��ۿ� ���� �Ƚ�Ű��...?
		
#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		bool											m_bDisableGravity;				/// ����ü �̰� ��... ���� �ִ���... User�� ��� ��...�ڵ峻���� �߷� ���� �����ϵ��� �� �� ����մϴ�
		bool											m_bDisableGravityInScript;		/// ĳ����, ���� ��ũ��Ʈ���� �߷� ���� �����ϵ��� �� �� ����մϴ� ��� ������ �̰͵� User�� ��� �ϴ±�...
		D3DXVECTOR2										m_vDisableGravityInScriptTime;	/// �߷� ���� �ð� (X�� ���۽ð�, Y�� ����ð�)
		D3DXVECTOR2										m_vIgnoreLineTime;				/// �� �ִϸ��̼� �ð� ������ �������� �߿��� ���� üũ�� �����Ѵ�. ��, ������ ��� ���� �ʴ´�. (�����ϰ�, ��ƿ������ ����ϴ� ��) NPC�� ��� ���̳�..
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

		

		bool											m_bOneTimeInit;	/// ������

		bool											m_bWaitingForRebirthAfterBungee;	/// �������θ����� ���� ��� �̰��� true�� ��, true�� �Ǹ� m_timerRebirthAfterBunge�� üũ
		CKTDXTimer										m_timerRebirthAfterBungee;	/// �������θʿ��� �ٽ� �ǵ��� �ö� ���� �ɸ��� �ð�


		float											m_fGroggyTime;				/// ���� �������� �׷α� �ð��� �ִ� ���, �׷α⿡ �ɸ��� �׷α� ����(�ǰ� ���� �Ҹ�Ǽ� ������ϴ�...)�� ���� ��, ���� ����Ʈ�� �����ֳ�..?


		bool											m_bInvisibility;			/// ���� �Ⱥ��̴� ��������, ����� �̺길 ����ȭ ����� �����. �̸�, ��ǥ�� ��ũ �������� ������ �Ǳ� ������ ���������� ���⿡ �� (�Űܾ� ��..)
		bool											m_bAbsoluteInvisibility;	/// ���� �� �ϰ� �� ���ΰ�...m_bInvisibility == true�� ���¿��� �̸�, ��ǥ��, after_image, slash_trace, outline, unit_shadow, Īȣ ��� ������ �ȵǴ� ��������


		D3DXVECTOR3										m_vDisplaceUnitPosition;	/// �̺��� ��Ȧ �ʻ��� ���� ĳ���� �ܺο��� ĳ���� ��ġ�� �Ű��� �� ���Ǵ� ����, �� ����ŭ ĳ���� ��ġ�� �̵��ȴ�.
		D3DXVECTOR3										m_vFixedUnitPosition;		/// ������(��Ȧ �ʻ�� ����. ��Ȧ�� �����Ÿ��̻� ��������� ĳ���� ��ġ�� �����ع�����.)


		CKTDXTimer										m_TimerRandomSeedSync;		/// RandomSeed ���� �ٲٱ� ���� Ÿ�̸�... �ٵ� GUUser���� �� ���� �Ǿ� �ֳ�... �����ؾ���
		static const double								RANDOM_SEED_RESET_TIME;		/// RandomSeed ���� �ٲٴ� �ֱ�


//#ifdef LINEMAP_FAST_WIND_TEST
//		bool											m_bEnableUpsideWind;	
//		bool											m_bWaitInTheAir;		/// ���߿��� �����ð� ������ wait���·� �ٲ��
//		float											m_fTimeInTheAir;		/// ���߿��� �� �ð���ŭ ������ wait ���·� �ٲ��
//#endif LINEMAP_FAST_WIND_TEST

#ifndef  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		bool											m_bDoubleAttack;		/// ������� ��������
#endif  X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
		wstring											m_wstrHitSound;			/// ������ü����� ������ hit ����
		float											m_fFinalDamage;			/// ������ü����� ������ ������
		D3DXVECTOR3										m_vImpactPoint;			/// ������ü����� ������ Ÿ����ġ
		float											m_fDoubleAttackTime;	/// ������� ǥ�� �ð� ��

		//{{ 2011-04 �� ��ġ�� û �޸� ��弦
#ifdef	CHUNG_MEMO_01				
		bool											m_bHittedExtraDamageHeadShot;	/// ���� ��������� ��.. ������.. EDT_HEAD_SHOT�� �¾�����
#endif	CHUNG_MEMO_01				
		//}} 2011-04 �� ��ġ�� û �޸� ��弦

		std::vector< CX2EffectSet::Handle > m_vecEffectSetToDeleteOnDamageReact;	/// ������ ����Ʈ�� ���� �Ǹ� ���� �Ǿ� �ϴ� ����Ʈ ���� �ڵ�
		std::vector< CX2EffectSet::Handle > m_vecEffectSetToDeleteOnStateEnd;	/// ����Ʈ�� �ߵ� ���� ���� ���°� ������ ���� �Ǿ� �ϴ� ����Ʈ ���� �ڵ�
		std::vector< CX2EffectSet::Handle > m_vecEffectSetToDeleteOnDie;	/// ������ ���� �Ǿ� �ϴ� ����Ʈ ���� �ڵ�
#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
		std::vector< CX2EffectSet::Handle > m_vecEffectSetToDeleteOnCustomState;	/// Ư�� ������Ʈ�� ����Ǹ� ���� �Ǿ� �ϴ� ������Ʈ�� �ڵ�
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		std::vector< EffectSetToPlay > m_vecEffectSetToPlay;	/// �÷��� �Ǿ� �ϴ� ����Ʈ ��

		bool m_bDeleteEffectSetOnStateEnd;	/// m_vecEffectSetToPlay�� �ִ� �� ���� m_vecEffectSetToDeleteOnStateEnd�� �־�� �ϴ��� ����
		bool m_bDeleteEffectSetOnDamageReact;	/// m_vecEffectSetToPlay�� �ִ� �� ���� m_vecEffectSetToDeleteOnDamageReact�� �־�� �ϴ��� ����
		bool m_bDeleteEffectSetOnDie;	/// m_vecEffectSetToPlay�� �ִ� �� ���� m_vecEffectSetToDeleteOnDie�� �־�� �ϴ��� ����
#ifdef DELETE_EFFECTSET_ON_CUSTOM_STATE
		bool m_bDeleteEffectSetOnCustomState;	/// m_vecEffectSetToPlay�� �ִ� �� ���� m_vecEffectSetToPlay�� �־�� �ϴ��� ����
#endif DELETE_EFFECTSET_ON_CUSTOM_STATE
#ifdef CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT
		bool m_bIsCustomStateDeleteEffectOnDamageReact;
#endif // CUSTOM_DELETE_EFFECT_ON_DAMAGE_REACT

		bool m_bHyperEffectSet;	/// ����Ʈ �¿� HYPER_NAME�� �����ϰ�, �̰��� ���� �ϸ� HYPER_NAME���� ������ ����Ʈ�� ����
		float m_fEffectSetLifeTime;	/// ����Ʈ ���� �÷��� �ð�...
#ifdef ADDITIONAL_MEMO
		int	m_iMemoId;
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE


#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        std::set<CX2DamageEffect::CEffectHandle>		    m_setEffectiveBlackHoleDamageEffect;	/// ��ȦData�� ���� �ִ� ����Ʈ�� �������ų� ���� ��쿡 �̰��� �����Ͱ� insert �ȴ�.	
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		std::set<CX2DamageEffect::CEffect*>		m_setEffectiveBlackHoleDamageEffect;	/// ��ȦData�� ���� �ִ� ����Ʈ�� �������ų� ���� ��쿡 �̰��� �����Ͱ� insert �ȴ�.	
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE


		bool m_abEnableDash[EDS_END];	/// dash�� �������� �ƴ���
		bool m_bEntangled;	/// ���ʱۿ� �ɷȴ��� �ƴ���?
		D3DXVECTOR3 m_vEntangledPosition;		/// ���ʱ� ��ġ...��... ��ũ�� ���߱� ���� �ʿ��� �ɱ�..?
		bool m_bHitedDrainMana;		/// ������ ����� ���ݿ� �¾Ҵ���...



#ifdef CLOSE_SOUND_TEST
		CX2SoundCloseManager m_SoundCloseManager;
#endif CLOSE_SOUND_TEST

#ifdef DUNGEON_ITEM
		BUFFER_FACTOR	m_AttackBuffer;		/// ���ݷ� ����
		BUFFER_FACTOR	m_DefenceBuffer;	/// ���� ����
		BUFFER_FACTOR	m_PhysicAttackBuffer;	/// ���� ���ݷ� ����
		BUFFER_FACTOR	m_MagicAttackBuffer;	/// ���� ���ݷ� ����
		BUFFER_FACTOR	m_PhysicDefenceBuffer;	/// ���� ���� ����
		BUFFER_FACTOR	m_MagicDefenceBuffer;	/// ���� ���� ����
#endif

		float			m_fNoSkillTime; /// �������׸� �־ �ɵ�... ��ų�� ����� �� ���� �ð�

//{{ kimhc // 2010.4.2 // ��д��� �۾�(��Ʈ������ȿ��)
#ifdef SERV_SECRET_HELL
		/// �̷��� ����� �ִٴ�.. ���� ��..
		BUFFER_FACTOR					m_StatUpBufferBySocketItem;		/// �������Ǽ��Ͽɼǿ� ���� ����ȿ��
		CX2SocketItem::STAT_UP_TYPE		m_StatUpType;					/// � ������ ���� �� ���ΰ�?

		// �̰͵�.. �迭�� �ϸ� ���� ����..
		// �����̻� ���Ÿ� ���� ����� ���� ������ ExtraDamageData�� �ּҸ� ��� �ִ� ����
		// �� �迭�� ���� �����̻� �ð������� �����Ͽ� �����̻��� ������(time�� 0���� ����)
		CX2DamageManager::ExtraDamageData*		m_pExtraDamageDataList[NUM_OF_EXTRA_DAMAGE_PACK_MEMBER];
#endif SERV_SECRET_HELL
//}} kimhc // 2010.4.2 // ��д��� �۾�(��Ʈ������ȿ��)


#ifdef UNIT_SLASH_TRACE_MANAGER_TEST
		CX2UnitSlashTraceManager* m_pUnitSlashTraceManager;	/// ������� ���� ������ TRACE
#endif UNIT_SLASH_TRACE_MANAGER_TEST





//#ifdef HEAD_INVERSE_KINEMATICS_TEST
//		bool m_bEnableHeadIK;
//		float m_fHeadIKAngleChangeRate;		// ȸ�������� ���������� ����ȴ�. 
//		float m_fHeadIKAngleMax;
//		float m_fHeadIKAngle;
//		float m_fHeadIKRadius;
//#endif HEAD_INVERSE_KINEMATICS_TEST



//#ifdef STEP_ON_MONSTER_TEST
//		bool m_bCanBeSteppedOn; // �� �������� �ٸ� ������ �ö� �� �ִ�.
//#endif STEP_ON_MONSTER_TEST


//#ifdef STEP_ON_MONSTER_COLLISION_BOX_TEST
//		bool m_bCanBeSteppedOnCollsionBox; // �� ������ �浹�ڽ����� �ٸ� ������ �ö� �� �ִ�.
//#endif STEP_ON_MONSTER_COLLISION_BOX_TEST





//#ifdef HEAD_ATTACHED_CAMERA_TEST
//		D3DXVECTOR3 m_vBoneAttachedCameraOffset;
//#endif HEAD_ATTACHED_CAMERA_TEST



		SkillRelatedData m_SkillRelatedData;	/// ��� ��ų ������ ���ε�... ������ ���


		float m_fLastPositionValueOnAnimatedLine;	/// ���� frame�� �����̴� �������� �־��� ��쿡 ��ġ��
		D3DXVECTOR3 m_vLastPositionOnAnimatedLine;	/// ���� frame�� �����̴� �������� �־��� ��쿡 ��ġ��

		

		CommonState m_CommonState;		/// �� ���ֵ��� �⺻���� ����ID ����



#ifdef RESET_INVALID_UNIT_POSITION_TEST
		bool m_bMisplaced;
		CKTDXTimer m_TimerMisplaced;
#endif RESET_INVALID_UNIT_POSITION_TEST



#ifdef OVERKILL_TEST
		bool m_bOverKilled;
#endif OVERKILL_TEST



#ifdef REVERSE_GRAVITY_TEST
		float m_fReverseGravitySpeedDeltaY;		/// ������ �׷���Ƽ ���� ������
		D3DXVECTOR3 m_vReverseGravityRotateSpeedDelta; /// ������ �׷���Ƽ�� ���� �ִϸ��̼� ������Ʈ ����// oasis907 : ����� [2011.6.29]
#endif REVERSE_GRAVITY_TEST



#ifdef BOUNDING_COMBO_TEST
		bool m_bDamageBoundingReserved;
		float m_fDamageBoundingSpeed;
#endif BOUNDING_COMBO_TEST

		bool m_bImmuneToExtraDamage;		/// ��� extra damage �� ����, ���Ϳ��� �����Ǿ�����



//#ifdef TRANSFORMER_TEST
//
//		bool m_bIsTransformed;
//		UidType m_TransformerOwnerUID;
//		
//#endif TRANSFORMER_TEST

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		std::vector<TIME_SPEED>         m_vecSpeedFactor;	/// ������ �����, ��ũ��Ʈ �󿡼� Ư�� �ð��뿡 ���ǵ带 �ٶ� ���
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef RAVEN_SECOND_CLASS_CHANGE
		// ���̺츸 ���.. ���ҵ� �ڵ�
		bool							m_bChangeDamageType;	/// ������ Ÿ���� ���� �Ǿ�����..
		CX2DamageManager::DAMAGE_TYPE	m_eDamageType;	/// � ������ Ÿ������ ���� �Ǿ�����..
		float							m_fDamageRel;	/// ����� ����
		int								m_iDamageTimes;	/// �̰�.. �ϼ����� ����ϴ� �� ������..
#endif

#ifdef DAMAGE_HISTORY
		bool	m_bDamageHistory;	/// �ٹ�����.. NPC�� ���
		float	*m_fDamageHistory;	/// ������ ��� ���ص� ���ٵ�.. NPC�� �������� ���������� ���⿡ ���ϰ�, ���� �� �� ���� <= 0 �̸� ��ŷ ������ �Ǵ�
#endif

//{{ kimhc // 2010.2.17 //
#ifdef	APPLY_MOTION_OFFSET
		bool	m_bApplyMotionOffset;	/// ��ǿ� ���� �̵����� �ʵ��� �ϴ� �÷��׷�... ������ ���
#endif	APPLY_MOTION_OFFSET
//}} kimhc // 2010.2.17 //

#ifndef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef SKILL_BALANCE_PATCH
		bool	m_bCanPassUnit;		// �ٸ� ������ �н� �� �� �ִ��� �ΰ� ������.. ������ CommonStateStartFuture�� ��� �Ľ� �κ��� ������ �� �κ��� �����ص� �� ��..
#endif
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef SERV_SKILL_NOTE
		float m_fRevengeDefenceTime;	/// ���ҵ� �ݰ� �ε�.. ���ҵ常 ���
		float m_fEnduranceAttackTime;	/// ������� �ΰ�?
		float m_fPoisonDeBuff;	/// ������ �ɸ�
#endif
		//{{ JHKang / ������ / 2010/11/26 / ����� ���带 ���� ���� ���� ��ġ
#ifdef NEW_SKILL_2010_11
		float m_fAtomicShieldDamageRate;		/// ����� ���� ĳ���� ������ ���� ����

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
		// kimhc // 2010-12-08	
		std::vector<BUFF_TEMPLET_ID>	m_vecImmunityAtThisState;	/// ������ ������Ʈ������ Immune �Ǿ���� ExtraDamage ����.. �ٵ� ���⿡ ���� �������׸� ���� �ǵ��� �س���... ����!!�ؾ���
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#endif NEW_SKILL_2010_11
		//}} JHKang / ������ / 2010/11/26 / ����� ���带 ���� ���� ���� ��ġ

		bool	m_bForceChagneColor;	/// ������ Į�� ���� ������� �ϴ°�? (�� ���θ�)
		bool	m_bCheckChangeUnderWater;	/// ���� ���� ������ üũ�ؾ��ϴ� ��Ȳ����..
		bool	m_bChangeUnderWater;	/// �� ���� true�� ǳ�� �Ҹ��� ������� Ƣ�µ�... �׸���� �ٽ� false�� �ٲ���.. (��, ���� ���� �������� true)
		bool	m_bUnderWaterHead;		/// �� ���θʿ� ǳ�� �ߴ���... ���� ������ pRenderParam->color -= m_cLineUnitColor ���� �� ������ �Ѵ�.
		D3DXCOLOR m_cLineUnitColor;		/// ���θʿ� ���ؼ� ���� �Ǿ� �ϴ� ���� ��
		//{{ JHKang / ������ / 2011/01/13 / ��ü ���� Ÿ�� ���� Ȯ��(������ ���� Ÿ���� ����)
#ifdef SEASON3_MONSTER_2010_12
		CKTDGXRenderer::RENDER_TYPE	m_BasicRenderType;	/// ������ ������Ʈ ���� RenderpParam �� ����... �ٵ� �׷��� ������ ��� �ǵ��� ����?? NPC�� ���
#endif SEASON3_MONSTER_2010_12
		//}} JHKang / ������ / 2011/01/13

		bool m_bScaleEffectSetByNpc;	/// NPC�� ���, ������ Scale�� ���� EffectSet�� Scale�� ���� �� ������

#ifdef BALANCE_PATCH_20110303
		float	m_fMaxDownForce;	/// �� �����ӿ��� ���� �ִ� �����ٿ� ��ġ.. �̰��� �����ٿ� �������� ���� ��
#endif

		float	m_fExtraBigHead;	/// BIG_HEAD �ͽ�Ʈ�� �������� �޾Ҵ���... �̰� �����Ѱ� OnFrameMove���� �����ս��� ���� �Դ±�...

#ifdef CHUNG_SECOND_CLASS_CHANGE
		// oasis907 : ����� [2011.6.20] û 2�� ���� - ���̾� �ȶ��
		float	m_fAntiCriticalRate;	/// �ش� �� ��ŭ Ÿ�� ���� �� ũ��Ƽ�� Ȯ���� ���ҽ�Ŵ
		float	m_fDebuffTransitCompleteTimeLeft;	/// �ڽ��� ExtraDamage�� �ٷ� ������ �ʰ�, ������������ ���ĸ� ���� (�ٸ� ���͵� ExtraDamage�� ������ ��ȸ�� �ش�) ���ֱ� ���� �����̸� �ش�.
		vector<KBuffFactor>	m_vecKDebuffFactorListToTransit;		/// ������ ������� Factor ���

		bool	m_bEnableWeaponRenderEffect;	/// ���� ���� ����Ʈ�� ����ϰ� �ִ���..
		float	m_fFadeTimeLeft;				/// �� ���� ���̵尡 ���� �� ���ΰ�?
		float	m_fFadeChangeTime;				/// ���� �������� �ٲ�� ��
		float	m_fFadeChangeTimeLeft;			/// ���� �������� �ٲ�� ���� ���Ͽ� ���� ��ȭ�ǰ� ���� �ð� ��.. m_fFadeChangeTimeLeft = m_fFadeChangeTime - elpasedTime �ε�

		float	m_fPulseDuration;				/// ���ӽð�
		float	m_fPulsePeriod;					/// �󸶰�������..
		float	m_fPulseTimeLeft;				/// ���� �ð��ε�...
	
		PULSE_WEAPON_COLOR_ID m_eNowPulseWeaponColorID;		/// �޽��������� ���̵�

		D3DXCOLOR	m_WeaponRenderEffectColorStart;		/// ��ȭ ���� ���� Į��
		D3DXCOLOR	m_WeaponRenderEffectColorEnd;		///	��ȭ ������ ������ Į��
		D3DXCOLOR	m_WeaponRenderEffectColor;	/// ���� Į��
#endif CHUNG_SECOND_CLASS_CHANGE


#ifdef SERV_PVP_NEW_SYSTEM
		bool		m_bPvpBot;		/// ���� NPC ���� �ƴ���... NPC�� ����..
#endif

#ifdef NEW_HENIR_TEST

#ifndef ADD_HENIR_BUFF		/// ������ ����Ǹ鼭 ������ �ʰ� �� ����
		float		m_fTimeHenirBuff_Fire;		/// ��ϸ� �ð� fire ���� ���� �ð�
		float		m_fTimeHenirBuff_Water;		/// ��ϸ� �ð� water ���� ���� �ð�
		float		m_fTimeHenirBuff_Nature;	/// ��ϸ� �ð� nature ���� ���� �ð�
		float		m_fTimeHenirBuff_Wind;		/// ��ϸ� �ð� wind ���� ���� �ð�
		float		m_fTimeHenirBuff_Light;		/// ��ϸ� �ð� light ���� ���� �ð�
#endif ADD_HENIR_BUFF

#endif NEW_HENIR_TEST

//{{ robobeg : 2013-06-12
// CX2GUNPC �� �Űܰ�
//#ifdef SECRET_VELDER
//		KProtectedType<bool>	m_bGlobalSuperArmor;		/// m_fGlobalTimeSuperArmor �ð����� ������ ���۾Ƹ�..
//		KProtectedType<float>	m_fGlobalTimeSuperArmor;	/// �۷ι� ���۾Ƹ� �ð�
//#endif
//}} robobeg : 2013-06-12

#ifdef HIT_PARTICLE_TRACE_UNIT
#ifdef  X2OPTIMIZE_STL_CONTAINER_USAGE
        typedef std::vector<CKTDGParticleSystem::CParticleEventSequenceHandle>  SeqHitParticleTraceUnitContainer;
#else   X2OPTIMIZE_STL_CONTAINER_USAGE
        typedef std::list<CKTDGParticleSystem::CParticleEventSequenceHandle>    SeqHitParticleTraceUnitContainer;
#endif  X2OPTIMIZE_STL_CONTAINER_USAGE
		SeqHitParticleTraceUnitContainer    m_listSeqHitParticleTraceUnit;
#endif HIT_PARTICLE_TRACE_UNIT

#ifdef SPECIAL_USE_ITEM
		float		m_fSpecialItemBuff_Wind;
#endif


#ifdef SERV_ARME_DIMENSION_WITCH
		float		m_fHitCreateDamageEffectRate;
		float		m_fHitCreateDamageEffectPowerRate;
#endif

#ifdef SERV_RENA_NIGHT_WATCHER
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEnchant_ArrowOfExplosion;		/// ������ ȭ�� �̸�Ƽ�� �ڵ�
		CKTDGParticleSystem::CParticleEventSequenceHandle	m_hStartOfDelayedFiringOverlap;		/// ������ ��ȣź ��ø�� ��� �̸�Ƽ��
		CX2EffectSet::Handle								m_hActiveStartOfDelayedFiringEDT;	/// ������ ��ȣź ������ Ȯ�� ����Ʈ
#endif SERV_RENA_NIGHT_WATCHER

#ifdef SERV_CHUNG_TACTICAL_TROOPER
		float		m_fTacticalFieldDecreaseDamageValue;		/// ��Ƽ�� �ʵ忡 ���� ���ҵǴ� ������ ��ġ
#endif SERV_CHUNG_TACTICAL_TROOPER


		bool		m_bNeedToUpdateWeaponEffect;		/// ����ȭ ���, ������ ����ũ��� ���� ���߰ų�, �ٽ� �Ϲݸ��� ������ �� ���⿡ ���� ����Ʈ�� �ѹ����� �����ϱ� ���� �÷���
		bool		m_bShowSmallGageAndName;				/// SmallGage��Name�� ������ �ϴ��� ����(�Ÿ� �� ī�޶�� ĳ������ ���� ���� ���ؼ� �Ǵ�)

		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_Head;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_R_Foot;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_L_Foot;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_R_UpperArm;
		CKTDXDeviceXSkinMesh::MultiAnimFrame*			m_pFrame_Bip01_L_UpperArm;

		float		m_fForceHyperModeTime;			/// ���� ������� �ߵ��ÿ� ������ �����ð�
	
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		DWORD		m_dwFrameMoveCountNow;			/// CommonFrameMove ����ȸ��(X2GameUnit�� �Ҹ�Ǳ� ������ ��� ����)
//		DWORD		m_dwFrameMoveCountFuture;		/// CommonFrameMoveFuture ����ȸ��(X2GameUnit�� �Ҹ�Ǳ� ������ ��� ����)
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		UCHAR		m_ucHittedCount;				/// �ǰ� ���� Ƚ��
		UCHAR		m_ucHitCount;					/// Ÿ�� �� Ƚ��

		UCHAR		m_ucNumOfDeBuff;				/// ����� ����
		UCHAR		m_ucNumOfBuff;					/// ���� ����
		CX2UnitSlashTraceManager::SLASH_TRACE_CONDITION		m_eUnitSlashTraceConditionToDo;		/// ����Ǿ� �ϴ� UnitSlashTrace ����

		float							m_fRunJumpSpeedRate;			/// ���� �Ӽ� ��æƮ ������� ������ �ӽ� ���� (�Ӽ� ��æƮ�� ������ ���� �ϸ� ���� ����)

		vector<CX2ReverseLefRightPtr>	m_vecReverseLeftRightPtr;		/// �¿���� ȿ��
		vector<CX2BuffIdentityPtr>		m_vecDownImpossiblePtr;			/// �ϴ��̵��Ұ� ȿ��
		vector<CX2BuffIdentityPtr>		m_vecDashImpossiblePtr;			/// ��úҰ� ȿ��
		vector<CX2BuffIdentityPtr>		m_vecAttackImpossiblePtr;		/// ���ݺҰ� ȿ��
		vector<CX2BuffIdentityPtr>		m_vecSkillImpossiblePtr;		/// ��ų���ݺҰ� ȿ��
		vector<CX2BuffIdentityPtr>		m_vecZXImpossiblePtr;			/// ZX���ݺҰ� ȿ��
		vector<CX2BuffIdentityPtr>		m_vecSuperArmorPtr;				/// ���۾Ƹ� ȿ��
		vector<CX2NeverMoveByBuffPtr>	m_vecNeverMovePtr;				/// ������ġ�� ���� ȿ��

		CX2AdditionalStatByBuff			m_AdditionalMaxHp;				/// �ִ�Hp����ȿ��
		CX2AdditionalStatByBuff			m_AdditionalPhysicAttack;		/// �������ݷº���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalMagicAttack;		/// �������ݷº���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalPhysicDefence;		/// �������º���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalMagicDefence;		/// �������º���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalAnimSpeed;			/// ���ۼӵ�����ȿ��
		
		CX2AdditionalStatByBuff			m_AdditionalWalkSpeed;			/// �ȱ�ӵ�����ȿ��
		CX2AdditionalStatByBuff			m_AdditionalDashSpeed;			/// ��üӵ�����ȿ��

		CX2AdditionalStatByBuff			m_AdditionalJumpPower;			/// �����º���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalDashJumpPower;		/// ��������º���ȿ��
		
		CX2AdditionalStatByBuff			m_AdditionalAccuracy;			/// ���ߺ���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalAvoidance;				/// ȸ�Ǻ���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalCriticalRate;		/// ũ��Ƽ�ü���Ȯ������ȿ��
		CX2AdditionalStatByBuff			m_AdditionalCriticalAttack;		/// ũ��Ƽ�ü����ð��ݷº���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalAntiCriticalRate;	/// ũ��Ƽ�ù��Ȯ������ȿ��
		CX2AdditionalStatByBuff			m_AdditionalCriticalDefence;	/// ũ��Ƽ���ǰݽù��º���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalChangeAdditionalAttack;		/// �߰�����������ȿ��
		CX2AdditionalStatByBuff			m_AdditionalChangeAdditionalDefence;	/// ���������Һ���ȿ��
		
		vector<CX2StatFactorByBuffPtr>	m_vecAbsorbEffectAttackPtr;	/// ����Ʈ�������
		
		CX2AdditionalStatByBuff			m_AdditionalChargeMpHitMeleeAttack;		/// ���� Ÿ�ݽ� MPȸ���� ����
		CX2AdditionalStatByBuff			m_AdditionalChargeMpHitRangeAttack;		/// ���Ÿ� Ÿ�ݽ� MPȸ���� ����

		CX2AdditionalStatByBuff			m_AdditionalChargeMpHitted;		/// �ǰݽ� MPȸ���� ����

		CX2AdditionalStatByBuff			m_AdditionalFinalDamageMultiplierMeleeAttack;	/// �������� �ִ� ������ ����
		CX2AdditionalStatByBuff			m_AdditionalFinalDamageMultiplierRangeAttack;	/// ���Ÿ�(DAMAGE_EFFECT)���� �ִ� ������ ����
		CX2AdditionalStatByBuff			m_AdditionalFinalDefenceMultiplier;	/// �޴� ������ ����

		CX2AdditionalStatByBuff			m_AddtionalForceDownMultiplierMeleeAttack;	/// ���� ���ݿ� ���� ForceDown ���� ����
		CX2AdditionalStatByBuff			m_AddtionalForceDownMultiplierRangeAttack;	/// ���Ÿ� ���ݿ� ���� ForceDown ���� ����
		
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeBlazeDefence;	/// �Ӽ����׺���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeWaterDefence;	/// �Ӽ����׺���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeNatureDefence;	/// �Ӽ����׺���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeWindDefence;		/// �Ӽ����׺���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeLightDefence;	/// �Ӽ����׺���ȿ��
		CX2AdditionalStatByBuff			m_AdditionalChangeAttributeDarkDefence;		/// �Ӽ����׺���ȿ��

		CX2AdditionalStatByBuff			m_AdditionalDrainHpNormalAttack;			/// ����

		CX2AdditionalStatByBuff			m_AdditionalMaxMp;							/// �ִ�Mp����ȿ��

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		CX2AdditionalStatByBuff			m_AdditionalHyperCharge;					/// ���� ���� �ӵ�
#endif //SERV_ADD_LUNATIC_PSYKER

		vector<CX2StunPtr>				m_vecStunPtr;		/// ���� ȿ��
		vector<CX2FreezePtr>			m_vecFreezePtr;		/// ���� ȿ��
		vector<CX2BuffIdentityPtr>		m_vecAttackAllTeamPtr;	/// ����� ���� ȿ��

		vector<CX2ChangeAttackByTypePtr>		m_AdditionalAttackByType;				/// Ÿ�Ժ����ݷº���ȿ��

		vector<CX2ChangeEnchantAttackRatePtr>	m_AdditionalChangeEnchantAttackRate;	/// ���ϼӼ����ݹߵ�Ȯ��ȿ��

		vector<CX2DoubleAttackPtr>				m_vecDoubleAttack;						/// ���� ����

		/// ������ ���� ����� �� �ִ� RenderParam �� ��, ��ũ��Ʈ �Ǵ� ���� � ���ؼ� ����Ǵ� �⺻ RenderParam��
		/// RenderParam�� �����Ű�� ���� ����Ʈ�� empty �̸�, �� ������ ���ƿ´�
		StBuffRenderParam				m_stOriginBuffRenderParam;
		vector<CX2RenderParamByBuffPtr>	m_vecRenderParamByBuffPtr;	/// ������ ���� ����� RenderParam ����Ʈ��

		vector<DelegateProcessWithDamageData>			m_vecDelegateProcessInAttackResult;		/// AttackResult ���� �����ÿ��� ����Ǵ� ��������Ʈ ����
		vector<DelegateProcess>							m_vecDelegateProcessInDamageReact;		/// DamageReact �ÿ��� ����Ǵ� ��������Ʈ ����
		vector<DelegateProcess>							m_vecDelegateProcessInCommonFrameMove;	/// CommonFrameMove ���� ����Ǵ� ��������Ʈ ����
		vector<DelegateProcessWithDamageData>			m_vecDelegateProcessWithDamageDataInDamageReact;		/// DamageReact �ÿ��� ����Ǵ� ��������Ʈ ����(DamageData ���� ���)

		vector<CX2EffectSetImpactPointByBuffPtr>	m_vecEffectSetImpactPointByBuffPtr;	/// Ÿ�ݽ� Ÿ������Ʈ�� ��µǾ��ϴ� ����Ʈ�¸���Ʈ
		vector<CX2EffectSetImpactPointByBuffPtr>	m_vecEffectSetHittedByBuffPtr;		/// �ǰݽ� ��µǾ��ϴ� ����Ʈ�¸���Ʈ

		vector<CX2ChangeUnitScaleByBuffPtr>			m_vecChangeUnitScaleByBuffPtr;		/// ������ ���� ������ ũ�� ����
		DelegateProcess	m_delegateProcessReturnFromScaleChange;			/// Y_PRESS���� �ﷷ�̴� ȿ���� �����ϸ鼭 ���� �����Ϸ� ���ƿ��� ó��

		vector<CX2CustomFunctionByBuffPtr>			m_vecCustomFunctionByBuffPtr;		/// ������ ���� ȿ�� �� StateStart, CommonFrameMove, StageEnd���� ó���ؾ��ϴ� �ϵ��ڵ��� ����
		vector<CX2AfterImageByBuffPtr>				m_vecAfterImageByBuff;				/// Element�� AfterImage�� �����, Element�� �ִٸ� AfterImage�� Enable �Ǿ���
		CKTDGXSkinAfterImage*						m_pAfterImage;
#ifdef ADD_DISPLAYER_WEAPON_AFTER_IMAGE
		vector<CX2WeaponAfterImageByBuffPtr>		m_vecWeaponAfterImageByBuff;		/// Element�� ������ AfterImage�� �����, Element�� �ִٸ� AfterImage�� Enable �Ǿ���
#endif ADD_DISPLAYER_WEAPON_AFTER_IMAGE

#ifdef DAMAGE_EFFECT_GROUP_DAMAGE
		vector<DamageEffectGroupDataPtr>			m_vecDamageEffectGroupDataPtr;		/// �׷� ������ ���� ���� ����
#endif DAMAGE_EFFECT_GROUP_DAMAGE

#ifdef SERV_NEW_DEFENCE_DUNGEON // ���볯¥: 2013-04-14
		vector<CX2EffectSetImpactPointByBuffPtr>	m_vecEffectSetNormalAttackByBuffPtr;	/// �Ϲ� ���� Ÿ�ݽ� ��µǾ� �ϴ� ����Ʈ�¸���Ʈ
		vector<CX2AddSkillLevelByBuffPtr>			m_vecAddSkillLevelByBuffPtr;			/// ������ ���� ������ ��ų���� ����Ʈ�� �� ����Ʈ�� ���� SUM �ؼ� ��� �ؾ� ��
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
		vector<CX2ReflectMagicByBuffPtr>			m_vecReflectMagicByBuffPtr;				/// ������ ���� ���� �ݻ� ȿ��
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����
		// Z ���� �Ұ�, X ���� �Ұ� ���� Ÿ�� �߰�
		vector<CX2BuffIdentityPtr>		m_vecZImpossiblePtr;			/// Z ���ݺҰ� ȿ��
		vector<CX2BuffIdentityPtr>		m_vecXImpossiblePtr;			/// X ���ݺҰ� ȿ��
#endif // BALANCE_PATCH_20131107					// ������ / 13-10-16, 2013�� �Ĺݱ� �뷱�� ����

#ifdef RIDING_SYSTEM
		CX2RidingPetPtr		m_ptrRidingPet;		/// Ż �� ����Ʈ
		bool				m_bPassDash;		/// �н� �뽬���� ����, ���İ� ó����
#endif //RIDING_SYSTEM

#ifndef X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
#ifdef MODIFY_EFFECT_SET_DELETE
		map< UINT, float > m_mapStateInsertBuffFactor;	// Ư�� ������Ʈ�� ���� �ð��� �Ǹ� �ڱ� �ڽſ��� �Ŵ� �������� ����Ʈ
#endif // MODIFY_EFFECT_SET_DELETE
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
        bool                                        m_bPositionBackup;
        D3DXVECTOR3                                 m_vPositionBackup;
        int                                         m_iPositionBackup_LineIndex;
        bool                                        m_bPositionBackup_FootOnLine;

#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

#ifdef SERV_ADD_LUNATIC_PSYKER // ����ȯ
		float										m_fFlickerElapsedTime;	/// ���� BDT_RENDER_PARAM �� color�� ��¦�̴� ������ ����ϴ� �ð� ��
#endif //SERV_ADD_LUNATIC_PSYKER

#ifdef SERV_DROP_FOR_FINISHER_ONLY
		UidType										m_iFinisherUID;
#endif SERV_DROP_FOR_FINISHER_ONLY
};

IMPLEMENT_INTRUSIVE_PTR( CX2GameUnit::DamageEffectGroupData );