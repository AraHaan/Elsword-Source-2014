#ifndef _KTDX_H_
#define _KTDX_H_

#ifndef LINE1
#define LINE1(x_) #x_
#endif // #ifndef LINE1
#ifndef LINE
#define LINE(x_) LINE1(x_)
#endif // #ifndef LINE

#ifndef TODO
#define TODO(msg_) message ( __FILE__ "(" LINE(__LINE__)  "): [TODO] " msg_ )
#endif // #ifndef TODO
#ifndef NOTE
#define NOTE(msg_) message ( __FILE__ "(" LINE(__LINE__)  "): [NOTE] " msg_ )
#endif // #ifndef NOTE


//////////////////////////////////////////////////////////////////////////
// 2009-06-01, dmlee, 
// �׻� ���ԵǾ�� �ϴ� macro�� _ALWAYS_ ���ʿ� �����ؼ� ���
// ���� ��ġ ������ _SERVICE_
// �����׼� ��ġ ������ _SERVICE_, _OPEN_TEST_
// �ؿ� ��ġ ������ _FOREIGN_
// �׽�Ʈ �ڵ带 ������ �系 ��ġ ������ _IN_HOUSE_
// �׽�ũ �ڵ带 ������ �系 ��ġ ������ _IN_HOUSE_SERVICE_READY_QA_
//////////////////////////////////////////////////////////////////////////





//#define NVPerfHUD
//#define PROFILE_BUILD // Uncomment this line to enable profiling


#define _ALWAYS_
//#define _SERVICE_MANUAL_LOGIN_ // ������ VC++���� ��������� ������ ��
//#define _SERVICE_
//#define _OPEN_TEST_
//#define _OPEN_TEST_2_
//#define _NEXON_KR_
//#define _GAMANIA_TW_
//#define _IN_HOUSE_
//#define _IN_HOUSE_SERVICE_READY_QA_

//#ifdef  TARGET_UNDEFINED
//    #define _NEXON_KR_
//    #define _IN_HOUSE_
//    #define _TEST_UNPACK_
//#endif  TARGET_UNDEFINED


#ifdef _USE_BOOST_CONST_STRING_

#define BOOST_CONST_CSTRING             boost::const_cstring
#define BOOST_CONST_STRING_CREF( x )    boost::cref( x )

#else

#define BOOST_CONST_CSTRING             std::string
#define BOOST_CONST_STRING_CREF( x )    ( x )

#endif

// ��Ű�� ��ȣȭ�� ������ !! - robobeg, 2014-01-21
#define _ENCRIPT_SCRIPT_

#if defined( _IN_HOUSE_SERVICE_READY_QA_ )
	#pragma NOTE( "ktdx.h �� _IN_HOUSE_SERVICE_READY_QA_ ����!" )
	#define NO_GAMEGUARD
	#define MASS_FILE_FIRST			// ���� ���Ϻ��� kom���Ͽ��� ���� ã��
    //#define _ENCRIPT_SCRIPT_

	// ������ ġƮ�� ��� �������̽�
	#define ITEM_CHEAT_POPUP_TEST
	// �������� �����ڴ� ĳ���� ���� �޸� �� �ְ�
	#define VILLAGE_FLASH_MAN_DEV_ONLY
	// ��ڿ� �ش� ī���� ������ �� �Ⱦƹ����� ���. �׽�Ʈ �뵵�� ���� ����. ������ ���� ����!!
	#define AUTH_AUTO_SELL_ITEM	 

	// ������ �α�
//	#define ADD_DAMAGE_LOG 	// �ؿ��� ����
	
	// �系 �׽�Ʈ�� ���� ����Ű �߰�
//	#define ADD_SHORT_CUT_KEY_IN_HOUSE 	// �ؿ��� ����

	// �� �ȱ�, �� ������
	#define AUTH_AUTO_SELL_ITEM
	#define JUNK_AVATAR
	// ��ڿ� ���������� �ڵ����� ���
//	#define AUTH_RANDOM_ITEM_AUTO_OPEN 	// �ؿ��� ����

	// cheat command �Է� shortcut key, '/' Ű�� ������ ���������� �Է��� ġƮ��ɾ �����
	#define CHEAT_COMMAND_SHORT_CUT_KEY_TEST

	//#define FIX_HIT_FLY_RIDING_PET_MOTION

	// �۾�����: ���� �����ִ� ġƮ ��� �߰�.
	#define ERASE_BUFF_CHEAT

	// �۾�����: �����Ʒ��忡�� ����Ű�� ���� ��ȯ �ϱ�
	#define SUMMON_MONSTER_BY_PRESS_KEY_AT_TRAINING_GAME

	//////////////////////////////////////////////////////////////////////////
	// �̸�: ������
	// ���̵�: hboh88
	// �۾����۳�¥: 2013-12-18
	// �۾�����: �系�������� ���ӱ� ���� �߻����� �� ������ �ַ����� ���̵��� �ӽ� �۾�.
	//		   : TODO: ��������δ� �ڱ��ڸ��� ���� �ٿ���� �������� �ٸ��� �� ��.
	//				   ���� �۾� �ʿ�
	#define FORCE_SERVER_GROUP_ID_SETTING_WHEN_ERROR
	//////////////////////////////////////////////////////////////////////////

#elif defined( _SERVICE_ ) || defined( _OPEN_TEST_ )
	#pragma NOTE( "ktdx.h �� _SERVICE_ ����!" )
	#ifndef _SERVICE_MANUAL_LOGIN_
		//#define _ENCRIPT_SCRIPT_
	#endif _SERVICE_MANUAL_LOGIN_
		
//	#define NO_GAMEGUARD	// �ؿ��� ����
    // #define CHECK_NP_HACKUSER	// �������� ���ؼ� ���Ӱ��� ����
	//#define BUG_TRAP				// X2Main.cpp �� �̰�
	#define MASS_FILE_FIRST			// ���� ���Ϻ��� kom���Ͽ��� ���� ã��

// ����� : [2009/8/4] //	�ٽǵ�
//	#define	HACK_SHIELD		// �ؿ��� ����
//{{ ����� : [2009/10/26] //	�ٽ��� LMP���
//	#define	HS_LMP			// �ؿ��� ����
//{{ ����� : [2010/5/24/] //	�ٽǵ� ���� ���� �˻� ����
//	#define HS_NO_ADMIN_NEED	

//	#define ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2	// �ٽǵ� ��� �߰�	// �ؿ��� ����
//	#define ADDITIONAL_HS							// �ٽǵ� ��� �߰�	// �ؿ��� ����
//	#define SEND_URL_TO_NEXON				// ��� ��ü�� ���� ���� �� ������ ���� �̷��������, �ű԰����� ���� �ߴ��� �ؽ��ʿ� URL���� // �ؿ��� ����
//	#define SEND_NEXON_WISE_LOG_BY_URL		// �ؽ� ���� ���� �� �ɸ��� ���� ���� ������ �α� ���� // �ؿ��� ����
	//#define AVATAR_IMAGE_TEST				// kimhc // Ȩ������ ������ �ƹ�Ÿ �̹��� ǥ������ ���� ĳ���� �̹��� ĸ�� ��� �׽�Ʈ

#ifdef _OPEN_TEST_
	// ������ ġƮ�� ��� �������̽�
//	#define ITEM_CHEAT_POPUP_TEST // �ؿ��� ����
//	#define ADD_SHORT_CUT_KEY_IN_HOUSE		/// �系 �׽�Ʈ�� ���� ����Ű �߰� // �ؿ��� ����

//	#define INACTIVATION_MINIMIZE_ALL_WINDOW_AT_START	// mauntain	/ ����ȯ / 2013-02-13 / X2.exe ����� ������ ��� ȭ�� �ּ�ȭ �Ǵ� ��� ���� ( ���� ���� ) // �ؿ��� ����

//	#define CHECK_INVALIDE_CASH_SHOP_ITEM // ������ // 2013-10-04 // ItemTemplet�� ���� �������� ������ �÷��� �� �̸� �˷��ֱ�

#endif	// _OPEN_TEST_
	

#elif defined( _FOREIGN_ )
	#pragma NOTE( "ktdx.h �� _FOREIGN_ ����!" )
    #define OPTIMIZED_P2P
	//#define _ENCRIPT_SCRIPT_
    #define NO_GAMEGUARD
	

#elif defined( _IN_HOUSE_ )
    #pragma NOTE( "ktdx.h �� _IN_HOUSE_ ����!" )
	#define NO_GAMEGUARD
    //#define CHECK_NP_HACKUSER // �系������ ������� ������� ���Ӱ��� ����
	//#define _USE_NEXON_MSG_INHOUSE // �系���� ���� �ؽ��޽��� ���� (�ظ��ؼ� ������� ����.)
	
	//#define	HACK_SHIELD

    //#define MASS_FILE_FIRST

#ifdef HACK_SHIELD
//	#define	HS_LMP									// �ؿ��� ����
//	#define ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2	// �ٽǵ� ��� �߰�	// �ؿ��� ����
//	#define ADDITIONAL_HS							// �ٽǵ� ��� �߰�	// �ؿ��� ����
#endif

#else 
	#pragma NOTE( "ktdx.h �� define Ȯ���ϱ�.!" )
	NOT GOOD.

#endif // _SERVICE_

//#if defined( NDEBUG )	// kimhc // 2010-06-28 // �系 �Ǵ� �����׼� ��������� ����
//#define	IGA_TEST	//kimhc // 2010-03-03 // ���� ���⵵ ����
//#endif	// #if defined( NDEBUG )

// ���̴� ����
//#define IN_TESTING_THEMIDA_BY_TOOL_TEAM


// Always ----------------------------------------------------------------------
#ifdef _ALWAYS_
#	include "Always.h"
#	include "AlwaysButConditionally.h"
#endif _ALWAYS_
// -----------------------------------------------------------------------------





// New_Village -----------------------------------------------------------------
#ifdef NEW_VILLAGE

#ifndef _SERVICE_
#define KHELP_SYSTEM_TEST			
//#define CHECK_OPENED_DEVICE
//#define MEMORY_LEAK_DETECTION_TEST	
#define NEW_VILLAGE_AUTO_SD_UNIT
#endif


// �� ���� ������ �׽�Ʈ
#define NEW_VILLAGE_RENDERING_TEST

//#define SQUARE_CAMERA_TRACKING_TEST		// ���忡�� ī�޶� ĳ���� ������ ���󰡰� �׽�Ʈ 



// ĳ���� ��׶��� �ε� �׽�Ʈ
/** skinned mesh�� background �ε� ���� �۾��� �׽�Ʈ�ϱ� �����ϴ�.
- jintaeks on 2008-12-12, 11:12
���忡 ������ �� X2WorldObjectSkinMesh.cpp ��ü�� �ε� �۾��� �Ϸ��ϴ�.
- jintaeks on 2008-12-13, 15:07*/
#define UNIT_BACKGROUND_LOADING_TEST // �׻� ������� �ϰ� ��ũ�� ����, robobeg, 2013-12-30

/** ���忡 �����ϰ� ���� �������� ������ �ٸ� �������� ���ҽ� �ε� �۾��� �����ϱ� �����ϴ�.
- jintaeks on 2008-12-13, 15:07*/
#define EQUIP_BACKGROUND_LOADING_TEST // �׻� ������� �ϰ� ��ũ�� ����, robobeg, 2013-12-30


// ������ ������ ���� ���� ��׶��� �ε� �����Ű��
#define NEW_VILLAGE_UNIT_BACKGROUND_LOADING_TEST // �׻� ������� �ϰ� ��ũ�� ����, robobeg, 2013-12-30


// ������ ������ fieldnpc ��׶��� �ε�
//#define NEW_VILLAGE_FIELD_NPC_BACKGROUND_LOADING_TEST


// uitextureData���� SetTexȣ�� �Ҷ� �ؽ��İ� �ٲ� ��쿡�� settexȣ���ϵ��� ����
#define EFFICIENT_SET_TEX_FOR_UI


#define UNIT_NEW_MOTION // ��� �и� (���� �̸��)

#define NPC_SOUND   // npc���ġ��...
#define OTHER_ROAD	// ������
#define UNIT_EMOTION
#define CHANGE_CHANNEL      // ä���̵�	

#endif NEW_VILLAGE


// -----------------------------------------------------------------------------
// ��������� ����ȭ �۾� : robobeg, 2013-11-08


#define X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK            // mass file manager �� �� ����, �׻� ������� �ϰ� �ڵ忡���� ��ũ�� ����, robobeg 2013-12-30
//#define X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO   // �⺻���� ������� ��
#define X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
#define	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
#define	X2OPTIMIZE_REFACTORING_RESOURCE_BACKGROUND_LOAD	// UnitLoader �� ȣȯ���� ������ �ϱ� ����
#define X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
#define	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
#define	X2OPTIMIZE_INFORM_FIRST_FRAME_OF_SIMULATION_LOOP
#define X2OPTIMIZE_SOUND_BACKGROUND_LOAD // ���� ���ҽ� ��׶��� �ε� ����, ���� IO �� ��׶��忡��, fmod ������ fmod �����忡��
#define	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER // �⺻���� ������� �ϰ� ��ũ�ο��� ����, robobeg, 2013-12-30
#define X2OPTIMIZE_SIMULATION_LAG_LIMIT
#define X2OPTIMIZE_ANIMATION_LOSSY_COMPRESSION
//#define X2OPTIMIZE_KFILESERIALIZER_MODIFIED   // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
//#define X2OPTIMIZE_NPC_LUASPACE_SHARING       // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08

//#define SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK    // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
#define SERV_KTDX_OPTIMIZE_UDP_PACKET_CHECKSUM
#define X2OPTIMIZE_ROBUST_NPC_FINAL_STATE
#define X2OPTIMIZE_ROBUST_NPC_ROTATION
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#define SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
#define SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#define X2OPTIMIZE_PARTICLE_SEQUENCE_DVB 	    // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
//#define  X2OPTIMIZE_PREVENT_ERRANEOUS_GAMEUNIT_PUSH // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
#define X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
//#define X2OPTIMIZE_NPC_NONHOST_SIMULATION_ANALYSIS
//#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
// ����� �ùķ��̼� ������带 �ּ�ȭ�Ѵ�.
#define X2OPTIMIZE_NPC_NONHOST_SIMULATION_REDUCE_OVERHEAD
//#define X2OPTIMIZE_PUSH_PASS_BUG_FIX					// robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
//#define X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC           // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
//#define X2OPTIMIZE_USER_USER_PUSH_PASS_SYNC_BUG_FIX   // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
// ��� ���� ������ ���� npc state �� ������ �ʰ����� �� ũ������ �߻��ϴ� ���� ���´�
//#define X2OPTIMIZE_NPC_STATE_INDEXING_CRASH_BUG_FIX   // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION
// ���� �ٸ� �ʵ峪 ������ �ִ� ��Ƽ������ ���ʿ��ϰ� udp peer �����ϴ� ���� ����
//#define X2OPTIMIZE_PARTY_UDP_CONNECTION_BUG_FIX       // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
// ����� ������ NPC ���� ��ų ���� ���� ��ǻ�Ϳ��� NPC �� �и��� ���� ����
//#define X2OPTIMIZE_USER_PASS_PUSH_UNIT_TIME_BUG_FIX   // robobeg : �׻� ����ǰ� �ϰ� ��ũ�δ� �ּ�ó����, 2013-11-08
#define X2OPTIMIZE_LINEMAP_LINEGROUP

// shader ��� ������Ʈ ����ȭ : hcsung, 2013-11-08
#define X2OPTIMIZE_SETSHADERCONSTANT
// NPC �׸��� �ø� : hcsung, 2013-11-08
#define X2OPTIMIZE_NPC_SHADOW_CULLING
// ���� ������Ʈ mesh subset �κ� �ø� : hcsung, 2013-11-08
#define X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
// NPC headbone frame ã�� ���� ���� : hcsung, 2013-11-08
#define X2OPTIMIZE_NPC_DOSCALEHEADBONE
// ���� ������Ʈ mesh subset, bounding sphere �� view frustum �ȿ� ������ ������ �κ��ø� �� �� : robobeg, 2013-12-03
#define X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET_SPEEDUP

// ���� �ɼ� : ĳ����,NPC �׸��� ǥ�� : hcsung, 2013-11-08
#define X2OPTIMIZE_UNIT_SHADOW_SHOW_BY_GAMEOPTION
// ���� �ɼ� : NPC ������ ���� ���� : hcsung, 2013-11-08
#define X2OPTIMIZE_NPC_DIELIGHT_SOUND_BY_GAMEOPTION
// ���� �ɼ� : damage effect ǥ�� : hcsung, 2013-11-08
#define X2OPTIMIZE_USER_DAMAGEEFFECT_SHOW_BY_GAMEOPTION
// ���� �ɼ� : ���ӿɼ� �ڵ� ���� : hcsung, 2013-11-08
#define X2OPTIMIZE_AUTOSETUP_GAMEOPTION
// damage effect �浹�˻� ����ȭ : hcsung, 2013-11-08
#define X2OPTIMIZE_DAMAGEEFFECT_COLLISION
// ��ƼŬ �ø� : hcsung, 2013-11-08
#define X2OPTIMIZE_CULLING_PARTICLE
// dynamic buffer �� ���� �ÿ��� lock ������带 ���δ� : hcsung : 2013-11-20
#define X2OPTIMIZE_DYNAMICVB_DISCARD


// ��ƼŬ, ����Ʈ handle ȭ : robobeg, 2013-11-08
#define X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
// ��ƼŬ, ����Ʈ �ùķ��̼��� ������ ������ 1�� �ϱ� : robobeg, 2013-11-08
#define X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION
// ������ �� ��Ÿ�ӿ� ���� ����Ʈ handle �� validity �� üũ�� �� �ֵ��� �� : robobeg : 2013-11-14
#define X2OPTIMIZE_HANDLE_VALIDITY_CHECK
// ���� PET ���ҽ� ��׶��� �ε� : robobeg : 2013-11-08
#define X2OPTIMIZE_VILLAGE_OR_SQUARE_PET_BACKGROUND_LOAD
// ȿ������ stl container �� ��ü : robobeg, 2013-11-08
#define X2OPTIMIZE_STL_CONTAINER_USAGE
// ���ʿ��� pointer ��� ���� : robobeg, 2013-11-08
#define X2OPTIMIZE_REMOVE_UNNECESSARY_PTR
// shared ptr -> intrusive ptr �� ��ü : robobeg, 2013-11-08
#define X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
// NPC heuristic update �� ���� ����ȭ : robobeg, 2013-11-08
#define X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
// ȣȯ�� ������ ���� ���� NPC ��ũ��Ʈ ���˵� ����
#define X2OPTIMIZE_SUPPORT_OLD_NPC_SCRIPT_FORMAT

// ���� �ɼ� ������ ���� ���� ���ϴ� ���� : hcsung, 2013-11-08
#define X2OPTIMIZE_MASS_FILE_FIRST_BUGFIX // �⺻���� ������� �ϰ� ��ũ�δ� �ּ�ó��, robobeg
// linemap ���� ���� ���� �� ����ȭ : robobeg, 2013-11-08
#define X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
// ara, chung ���� particle handle �� mesh ���� ����� �ϴ� ���� ���� : robobeg, 2013-11-08
#define X2OPTIMIZE_ARA_MESH_DESTROY_BUG_FIX
// ������ ���� �ɼ� �ٲ� �� ũ�����߻��� �� �ִ� ���� ���� : robobeg, 2013-11-08
#define X2OPTIMIZE_GAMEOPTION_BUGFIX
// attacker �� user/npc �� �� ���� ó�� ����� �� �Ǵ� ���� ���� : robobeg, 2013-11-08
#define X2OPTIMIZE_UNITTYPE_BUG_FIX
// X2ServerProtocol �ʿ��� multithread safety ������ ũ���� �߻��ϴ� ���� ���� : robobeg, 2013-11-08
#define X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
// ����Ʈ�� ��ź�������� ĵ���ϴ� �� �������� �ʴ� ���� ����, ����ȭ ���� �ƴ� : robobeg : 2013-11-13
#define X2OPTIMIZE_EFFECT_TO_UNIT_BUG_FIX
// �ʵ� ����Ʈ�� ������ �ް� �ٴϴ� ���� ����, ����ȭ ���� �ƴ� : robobeg : 2013-11-13
#define X2OPTIMIZE_CONFUSE_DIFFERENT_PARTICLE_SYSTEMS_BUG_FIX
// ����Ʈ�¿����� �������� NULL üũ ���� fix  : robobeg : 2013-11-20
#define X2OPTIMIZE_EFFECTSET_NULL_CHECK_BUG_FIX
// ��������� �ʵ����Խ� start state ����ȵǴ� ���� ����  : robobeg : 2013-11-20
#define X2OPTIMIZE_NPCNONHOST_START_STATE_BUG_FIX
// Ż���� Ÿ�� ���� ������ �� ���������� ���ϴ� ���� ����(��������) : robobeg, 2013-11-21
#define X2OPTIMIZE_WALL_TRAP_BUG_FIX
// ���� ����Ʈ �浹�˻�� ũ���� ���� ���� : robobeg, 2013-11-21
#define X2OPTIMIZE_LOCKONDATA_COLLISION_CHECK_CRASH_BUG_FIX
// dynamic buffer �������� �� ����Ÿ�� 2���� ���� ������ �� �ְ� �Ѵ�. ( circular array ���� ) : robobeg 2013-11-21
#define X2OPTIMIZE_DYNAMICVB_SUPPORT_SPLIT_DATA
// ��ƼŬ �ý��� loop �� �ܺη� �����Ű�� �߻��� �� �ִ� ���� : robobeg, 2013-12-03
#define X2OPTIMIZE_PARTICLE_AND_ETC_INFINITE_LOOP_BUG_FIX
// ��ƼŬ �ý��� ��� ���� �̺�Ʈ Ÿ�ֺ̹��� Ŀ�� �� �߻��ϴ� velocity ���� ���� ���� : robobeg, 2013-12-09
#define X2OPTIMIZE_PARTICLE_AND_ETC_SIMULATION_VELOCITY_ACCUM_BUG_FIX

// ��Ų�޽� ũ���� ���� ����, ���� ����. �� �̷� ���� �߻��ϴ°�? : robobeg, 2013-12-04
#define X2OPTIMIZE_SKIN_ANIM_MESH_CRASH_BUG_FIX
// CX2Eqip ũ���� ���� ����  : robobeg, 2013-12-04
#define X2OPTIMIZE_X2EQIP_CRASH_BUG_FIX
// CX2SlotItem ũ���� ���� ����  : robobeg, 2013-12-04
#define X2OPTIMIZE_X2SLOTITEM_CRASH_BUG_FIX
// CX2KTDGXSkinAnim ũ���� ���� ���� : robobeg, 2013-12-04
#define X2OPTIMIZE_KTDGXSKINANIM_CRAHSH_BUG_FIX
//

// TET, XET ��ó��, �⺻���� ����
#define X2OPTIMIZE_TET_XET_PREPROCESSING
// Dialog, Linemap �� LUA ���� ���������� ������� �ʵ��� ���� ����, �⺻���� ����
#define X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

// ���ѹ��� ����... ��Ű�� �ɼ��� : robobeg, 2014-01-17
#define X2OPTIMIZE_APPLY_UNPACK_HACK_PREVENTION

#ifdef  X2OPTIMIZE_APPLY_UNPACK_HACK_PREVENTION

    #define _USE_LUAJIT_
    #if  defined(_SERVICE_) || defined(_IN_HOUSE_SERVICE_READY_QA_)
        #define X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE    
    #endif // defined(_SERVICE_) || defined(_IN_HOUSE_SERVICE_READY_QA_)
    #if !defined(DEBUG) && !defined(_DEBUG)
        #define X2OPTIMIZE_ACCELERATE_SHARED_NPC_LUA_BY_LUAJIT
    #endif
    // �ֿ� ���� ��ȣȭ ��ȭ
    #define X2OPTIMIZE_ENFORCE_IMPORTANT_DATA_ENCRYPTION
    // �ֿ� ����, ��Ű¡�� ���� �� ��ȣȭ�ϴ� ������� ����
    #define X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS

#endif  X2OPTIMIZE_APPLY_UNPACK_HACK_PREVENTION

#if     defined(X2OPTIMIZE_TET_XET_PREPROCESSING) && defined(X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE)
#define X2OPTIMIZE_DISABLE_LUA_MULTITHREADING
#endif  //defined(X2OPTIMIZE_TET_XET_PREPROCESSING) && defined(X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE)

// 2���� ������ mass file �б� ����, �ؽ��Ŀ� TET ���� �ε� �ӵ� ����
#define X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES

// ��ŷ�� ����Ʈ üũ�� ��Ƽ������ ���ٿ� ���� ũ���� �����Ѵ�.
#define X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
// ����Ÿ�� ��Ʈ�� ���� �޸� ����
#define X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
// ��׶��� �ε� �����尡 �� �� ����������...
#define X2OPTIMIZE_COOPERATIVE_BACKGROUND_RESOURCE_LOADING
// �Ϲ����� thread ������ ����
#define X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
// COLLECTHACKINFO thread �� thread ������ ����
#define X2OPTIMIZE_COLLECTHACKINFO_MULTITHREAD_DAMAGE_HEAP_BUG_FIX
// CKTDXLog Ŭ���� ��ü ( g_CKTDXLog ) �� thread safety ����
#define X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
// KTDGLineMap buffer overrun ���� ����
#define X2OPTIMIZE_KTDGLINEMAP_BUFFER_OVERRUN_BUG_FIX
// SERVER TIME OUT �α� ��� ����, ������ ����� �Ǿ��� Ȯ��
//#define X2OPTIMIZE_IGNORE_SERVER_TIME_OUT_LOG
// ��Ű¡�� uncompress ���� üũ�Ѵ�. ���ʿ��� ũ������ ���̸�, Kom ���������� �� ���ؼ����� Ȱ��� �� �ִ�.
#define X2OPTIMIZE_ZLIB_UNCOMPRESS_ERROR_CHECK
// �ݵ� ���� KTDGDynamicVBManager ����ϵ��� ���� : XP �ӽſ��� ũ���� ����, robobeg - 2014-01-23
#define X2OPTIMIZE_BANDI_DYNAMICVB
// ���� �����Ȳ���� �״� ���� ����
#define X2OPTIMIZE_X2GAME_RELEASE_CRASH_BUG_FIX
// ��üũ ���μ��� �� ������Ʈ�ϸ鼭 ��Ƽ������ �̽��� �߻��ϴ� ũ���� ����
#define X2OPTIMIZE_PROCESS_LIST_MULTITHREAD_CRASH_BUG_FIX
// ������ �� �������� ����ſ��� ũ���� ���� ����
#define X2OPTIMIZE_SLIDE_SHOT_NPC_SELF_CRASH_BUG_FIX


#ifndef _SERVICE_

// ���� ���Ͽ� ���ؼ��� xor encrypt ������� �ʵ��� ����
#define X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
//#define X2OPTIMIZE_VERIFY_NPC_SCRIPT_CONSISTENCY

//#define X2OPTIMIZE_TCP_RELAY_TEST//TCP Relay �׽�Ʈ��
//#define X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
//#define X2OPTIMIZE_ONE_SIMUL_ONE_RENDER_TEST
//#define X2OPTIMIZE_RENDER_BOUNDING_SPHERE_TEST
//#define X2OPTIMIZE_DAMAGE_EFFECT_TEST
//#define X2OPTIMIZE_VIEWDISTANCE_TEST
//#define X2OPTIMIZE_DYNAMICVB_DISCARDCOUNT_LOG




//vtune ����
//#ifdef _VTUNE_
//#define MASS_FILE_FIRST
//#define X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE
//#endif//_VTUNE_

#endif//_SERVICE_


//
// ��������� ����ȭ �۾�
// -----------------------------------------------------------------------------

//
// -----------------------------------------------------------------------------
// OpenTest 2�� ���� ---------------------------------------------------------------------
// 
#ifdef _OPEN_TEST_2_ // ���� �׽�Ʈ 2�� ������ Define
	// ������ ġƮ�� ��� �������̽�
//	#define ITEM_CHEAT_POPUP_TEST // �ؿ��� ����

//	#include  "OpenTest2.h"		// Always.h ó�� �۵��մϴ�. // �ؿ��� ����
#endif // _OPEN_TEST_2_		




// -----------------------------------------------------------------------------

// InHouse ---------------------------------------------------------------------
#ifdef _IN_HOUSE_ 

 	#include "InHouseEtc.h"	// �系 �� �� ��Ÿ ��ɵ�....
 	#include "InHouse1.h"	// ������
 	#include "InHouse2.h"	// ����ö
 	#include "InHouse3.h"	// ����ȯ
 	#include "InHouse4.h"	// ������
 	#include "InHouse5.h"	// ������
	#include "InHouse6.h"	// ��â��

#ifdef WORLD_TOOL
	#undef ADD_PET_INVENTORY_BUTTON
	#undef PET_INVENTORY_BUG_FIX_01
	//#undef PET_AURA_SKILL
	#undef WINDOW_SYSTEM_BUTTON_FUNC
	//#undef COME_BACK_REWARD
	//#undef SERV_SOCKET_NEW
	#undef MACHINE_ID
	#undef SEASON3_MONSTER_2011_7
	//#undef NEW_HENIR_TEST
	#undef GRACE_OF_EL
	//#undef ELSWORD_WAY_OF_SWORD	
	//#undef UPGRADE_RAVEN
	//#undef SECRET_VELDER
	//#undef EVE_ELECTRA
	//#undef ADD_GAME_STAGE_DELETE_DAMAGEEFFECT
	#undef ADD_STATE_EXTRA_ATTACK
	//#undef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	//#undef SERV_ADD_ARME_BATTLE_MAGICIAN
	//#undef ELSWORD_WAY_OF_SWORD
	
	//#undef CHUNG_SECOND_CLASS_CHANGE
	//#undef CHUNG_FIRST_CLASS_CHANGE
	#undef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	//#undef X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
	#undef USE_FREE_TYPE
	// commonpacket.h ���Ͽ��� ���� ó���ؾ���
	//#undef SERV_PET_SYSTEM
	//#undef SERV_SHIP_DUNGEON
	//#undef SERV_WORLD_TRIGGER_RELOCATION
	//#undef SERV_TOONILAND_CHANNELING_EVENT
	//#undef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//#undef SERV_MACHINE_ID_BLOCK
	//#undef SERV_MACHINE_ID_DUPLICATE_CHECK
	//#undef SERV_MACHINE_ID_BLOCK_UPDATE
	//#undef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	#undef SERV_NEW_ITEM_SYSTEM_2013_05
	#undef X2OPTIMIZE_MASS_FILE_FIRST_BUGFIX
	#define BUG_TRAP

//////////////////////////////////////////////////////////////////////////
// �ٳ����󿡼� ��� �����ϵ��� ����ó��
//#define BANANA_EXCEPTION

#ifdef BANANA_EXCEPTION
	#define MASS_FILE_FIRST			// ���� ���Ϻ��� kom���Ͽ��� ���� ã��
#endif // BANANA_EXCEPTION
//////////////////////////////////////////////////////////////////////////
	

#endif

#ifdef X2TOOL
	#define SHOW_LINEMAP_FILE_NAME
	#define CHANGE_KEY_DEVICE	
	//#define ADD_POSTEFFECT

	
	#undef NO_P2P_NO_GAME	
	//#undef CHECK_KOM_FILE_ON_LOAD
	//#undef MORE_INFO_ABOUT_HACK
	//#undef CHECK_FROM_DLLMANAGER
	//#undef DLL_MANAGER
	//#undef CHECK_ALL_WINDOW
	//#undef CHECK_THREAD_WND
	//#undef CHECK_FOREGROUNDWND
	//#undef DESK_THREAD_WINDOW
	//#undef ADD_COLLECT_CLIENT_INFO
	//#undef COLLECTION_MODULE
	//#undef SYS_INFO
	#undef MACHINE_ID
#endif

#endif _IN_HOUSE_
// -----------------------------------------------------------------------------


#ifdef _NEXON_KR_

	#define NX_ENDING_FLASH					// ���� �� ������ ����

#endif _NEXON_KR_

// �ؿ��� �߰�
#define XOR_KEY0 0xc6f8aa02
#define XOR_KEY1 0x2647abdc
#define XOR_KEY2 0x9800bbef


#ifndef STRICT
#define STRICT
#endif

// If app hasn't chosen, set to work with Windows 98, Windows Me, Windows 2000, Windows XP and beyond
#ifndef WINVER
#define WINVER         0x0410
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410 
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0500 
#endif

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#pragma warning (disable: 4702)
#pragma warning (disable: 4995)
#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

#include <Winsock2.h>
#include <wininet.h>
#include <windows.h>


#include <assert.h>
#include <wchar.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <math.h>      
#include <limits.h>      
#include <stdio.h>
#include <time.h>
#include <process.h>
#include <io.h>
#include <shldisp.h>
#include <malloc.h>
//{{ robobeg : 2009-8-19
#include <atlbase.h> // for CComPtr and CComQIPtr
//}} robobeg : 2009-8-19

//////////////////////////////////////////////////////////////////////////
//���� �������� ���μ����� ã�Ƴ������� �Լ��� ��������
#pragma comment( lib, "Kernel32.lib" )
#include <TlHelp32.h>
//////////////////////////////////////////////////////////////////////////


#include "KSuperAssert.h"
#include "CrashRpt.h"

#ifndef DLL_BUILD		// 10.03.11 �¿� : DLL Build�� �ٸ� DLL�� ���� Dependancy ����
#include "KncFirewall.h"

// nProtect ���� ���� ������
#include "../X2LIB/X2SMTPMail.h"
#pragma comment( lib, "MailMIME.lib" )
#pragma comment( lib, "MailSMTP.lib" )
#endif

#ifdef PROFILE_BUILD
	#include "../Profiler/KProfileMan.h"
#else
	#define KTDXPROFILE()
	#define KTDXPROFILE2(name)
	#define KTDXPROFILE_BEGIN(name)
	#define KTDXPROFILE_END()
#endif

#include "../X2/KOGGamePerformanceCheckDefine.h"

// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
	#include <crtdbg.h>
#endif


//{{ seojt // 2008-10-15, 10:47
// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
// "d3d9.h"�� include�ϱ� ���� �� ��ũ�θ� Ȱ��ȭ���� �־�� �Ѵ�.
// "d3d9.h" ��Ŭ��� �Ŀ� �ִ� ���Ǹ� "d3d9.h"�� ��Ŭ����ϱ� ������
// �ű��. - jintaeks on 2008-10-15, 10:49
#if defined(DEBUG) || defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
		#define D3D_DEBUG_INFO
	#endif
#endif
//}} seojt // 2008-10-15, 10:47
// Direct3D includes
#include <d3d9.h>
#include <d3dx9.h>
#ifdef CONVERSION_VS
#	include <dxerr.h>
#else CONVERSION_VS
#	include <dxerr9.h>
#endif CONVERSION_VS
#include <dinput.h>
#include <mmsystem.h>
#include <mmreg.h>
#ifdef CONVERSION_VS
//#	include <dplay8.h>
#else CONVERSION_VS
#include <dplay8.h>
#include <ddraw.h>
#include <dsound.h>
#include <dshow.h>
#endif CONVERSION_VS


#ifndef STRSAFE_NO_DEPRECATE
#pragma deprecated("strncpy")
#pragma deprecated("wcsncpy")
#pragma deprecated("_tcsncpy")
#pragma deprecated("wcsncat")
#pragma deprecated("strncat")
#pragma deprecated("_tcsncat")
#endif

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#include "zlib.h"

#include "DXUT.h"
#include "DXUTmisc.h"
#include "DXUTenum.h"
#include "DXUTmesh.h"
#include "DXUTgui.h"
#include "DXUTsettingsDlg.h"



#include <vector>
using std::vector;
#include <stack>
using std::stack;
#include <queue>
using std::queue;
#include <list>
using std::list;
#include <map>
using std::map;
#include <hash_map>
using stdext::hash_map;
#include <string>
using std::wstring;
using std::string;
using std::wstringstream;
using std::stringstream;
#include <fstream>
#include <math.h>
#include <iostream>
using std::wcout;
using std::cout;
#include <set>
using std::set;
//{{ robobeg : 2008-10-10
#include <bitset>
#include    <deque>
#include    <hash_set>
#include    <hash_map>
#include	<boost/test/utils/foreach.hpp>
#include    <boost/foreach.hpp>
#include    <boost/static_assert.hpp>
#include    <boost/type_traits/is_base_of.hpp>
//}} robobeg : 2008-10-10

#include <sys/stat.h>
#include <boost/noncopyable.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/array.hpp>

#ifdef BOOST_SINGLETON_POOL_TEST
	#include <boost/pool/singleton_pool.hpp>
#endif BOOST_SINGLETON_POOL_TEST


#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scope_exit.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/intrusive_ptr.hpp>

#ifdef USE_FREE_TYPE
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftbitmap.h>
#pragma comment( lib, "freetype2411.lib" )
#endif USE_FREE_TYPE


#if defined(DEBUG) || defined(_DEBUG)

	// ASSERTwnx
	//
	#if !defined(ASSERT)
		#define ASSERT(exp)         _ASSERTE(exp)
	#endif // #if !defined(ASSERT)

	#ifndef V
		#define V(x)           { hr = x; if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
	#endif
	#ifndef V_RETURN
		#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
	#endif


    #if !defined(TRACE)
        inline void _tracew(const WCHAR* fmt_, ...)
        {
            WCHAR   out[4096];
            va_list argList;

            va_start( argList, fmt_ );
            //StringCchVPrintfW( out, ARRAY_SIZE( out ), fmt_, argList );
#ifdef	CONVERSION_VS
			::vswprintf_s( out, _countof(out), fmt_, argList );
#else	CONVERSION_VS
			::vswprintf( out, fmt_, argList );
#endif	CONVERSION_VS
            va_end( argList );
            ::OutputDebugStringW( out );
        }//_tracew()

        inline void _tracea(const char* pszFormat_, ...)
        {
            char    out[4096];
			ZeroMemory( out, 4096 );
            va_list argList;

            va_start( argList, pszFormat_ );
            //StringCchVPrintfA( out, ARRAY_SIZE( out ), pszFormat_, argList );
#ifdef	CONVERSION_VS
			::vsprintf_s( out, _countof(out), pszFormat_, argList );
#else	CONVERSION_VS
			::vsprintf( out, pszFormat_, argList );
#endif	CONVERSION_VS
            va_end( argList );
            ::OutputDebugStringA( out );
        }//_tracea()
        #if !defined(TRACEA)
        #define TRACEA              _tracea
        #endif // #if !defined(TRACEA)
        #define TRACEW              _tracew

    #ifdef UNICODE // 2005-5-2, 11:40:58
    #define TRACE               _tracew
    #else
    #define TRACE               _tracea
    #endif // #ifdef UNICODE // 2005-5-2, 11:40:58
    #endif // #if !defined(TRACE)

#else

	#if !defined(ASSERT)
		#define ASSERT(exp)
	#endif // #if !defined(ASSERT)

	#ifndef V
		#define V(x)           { hr = x; }
	#endif
	#ifndef V_RETURN
		#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
	#endif

    #if !defined(TRACE)
        //#define TRACE               1 ? (void)0 :
        /** The __noop intrinsic specifies that a function should be ignored and the argument list
            be parsed but no code be generated for the arguments. It is intended for use in global
            debug functions that take a variable number of arguments.
            - jintaeks on 2008-03-04, 10:00 */
        #define TRACE               __noop
        #if !defined(TRACEA)
        //#define TRACEA              1 ? (void)0 :
        #define TRACEA              __noop
        #endif // #if !defined(TRACEA)
        //#define TRACEW              1 ? (void)0 :
        #define TRACEW              __noop
    #endif // #if !defined(TRACE)

#endif

#ifdef  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#define IMPLEMENT_INTRUSIVE_PTR(CLASS) \
namespace boost { \
    inline void    intrusive_ptr_add_ref( CLASS* p ) { if ( p )    p->AddRef(); } \
    inline void    intrusive_ptr_release( CLASS* p ) { if ( p )    p->Release(); } \
}

#else   X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR

#define IMPLEMENT_INTRUSIVE_PTR(CLASS)

#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR


#ifdef  X2OPTIMIZE_STL_CONTAINER_USAGE
template <class Container>
inline typename Container::iterator EraseUnorderlyUsingSwap( Container& container, typename Container::iterator iter )
{
	typename Container::iterator iterLast = container.end();
	iterLast--;
	if ( iterLast == iter )
	{
		return	container.erase( iter );
	}//if
	swap( *iter, *iterLast );
	Container::size_type dist = iter - container.begin();
	container.erase( iterLast );
	return	container.begin() + dist;
}

#endif  X2OPTIMIZE_STL_CONTAINER_USAGE

//{{ seojt // 2008-10-15, 10:27
//------------------------------------------------------------------------------
// CRT debug macros
//
#ifndef INIT_CRTDEBUG
/** �޸� �Ҵ�� ���� ��û�� ���� ������ ��� �޸𸮸� �˻��ؼ�
    overwrite�� underwrite�� �߰��ϰ� ������ flag_��

        INIT_CRTDEBUG(_CRTDBG_CHECK_ALWAYS_DF);

    �� ����Ѵ�.
    �Ϲ�������
    
        INIT_CRTDEBUG(0)

    ó�� ����Ѵ�.
    - jintaeks on 2008-03-20, 10:36 */
#define INIT_CRTDEBUG(flag_)    ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | flag_)
#endif // #ifndef INIT_CRTDEBUG


/** @example    ASSERT_MEMORY()
	@code

    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        INIT_CRTDEBUG(0);

        int*    p = new int;
        delete p;

        *p = 3;             // using after deletion
        ASSERT_MEMORY();    // see and check call stack window
        delete p;
    }//main()

	@endcode
*/
// ASSERT_MEMORY()�� ����� �����ϱ� ���ؼ��� ���� INIT_CRTDEBUG()�� ȣ��� ���¿��� �Ѵ�.
#ifndef ASSERT_MEMORY
#define ASSERT_MEMORY()     ASSERT( ::_CrtCheckMemory() )
#endif // #ifndef ASSERT_MEMORY


/** @example    BREAK_ALLOC()
	@code

  //------------------------------------------------------------------------------
  //
  1) memory leak example:

    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        INIT_CRTDEBUG(0);

        int*    p = new int;

        *p = 3;
    }//main()

    // debug message:
    Detected memory leaks!
    Dumping objects ->
    {44} normal block at 0x00372DA8, 4 bytes long.
    Data: <    > 03 00 00 00
    Object dump complete.

    // @note    allocation number is {44}, so you can set break point at 44th alloc
    //          see below example


  //------------------------------------------------------------------------------
  //
  2) break on mal-alloc {44}:

    #ifdef PROJECT_PCH
    #define _TOSTR1(x)    #x
    #define _TOSTR(x)     _TOSTR1(x)
    #include _TOSTR(PROJECT_PCH)
    #endif // PROJECT_PCH
    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        BREAK_ALLOC( 44 ); // install break CRT point condition
        INIT_CRTDEBUG(0);

        int*    p = new int;

        *p = 3;
    }//main()

	@endcode
*/
// INIT_CRTDEBUG()�� ������ �޸� ��Ͽ� break point�� �Ǵ�.
#ifndef BREAK_ALLOC
#define BREAK_ALLOC(line_)   ::_CrtSetBreakAlloc( line_ )
#endif // #ifndef BREAK_ALLOC
//}} seojt // 2008-10-15, 10:27






#ifndef DMLEE_NOTE
	#ifdef DMLEE_ONLY
		#define DMLEE_NOTE(msg_) message ( __FILE__ "(" LINE(__LINE__)  "): [DMLEE_NOTE] " msg_ )
		#pragma DMLEE_NOTE( "comment out #define DMLEE_ONLY" )
	#else
		#define DMLEE_NOTE(msg_) message( "" )
	#endif DMLEE_ONLY
#endif DMLEE_NOTE



#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))

#ifndef SAFE_CALL
#define SAFE_CALL(p)			if(p) p
#endif    
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_CLOSE
#define SAFE_CLOSE(p)			{ if(p) { g_pKTDXApp->GetDeviceManager()->CloseDevice( (p)->GetDeviceID() );     (p)=NULL; } }
#endif

//{{ robobeg : 2008-01-05

#ifndef SAFE_FREE
#define SAFE_FREE(p)        { if(p) { ::free(p); (p)=NULL; } }
#endif // #ifndef SAFE_FREE

#ifndef SAFE_REALLOC
#define SAFE_REALLOC(p,s)   \
{   if((s)==0){ SAFE_FREE(p); }   \
else if (!(p))    { (p) = ::malloc(s); ASSERT(p); } \
else {  (p) = ::realloc((p),(s)); ASSERT(p); } }
#endif // #ifndef SAFE_REALLOC

#ifndef SAFE_TYPEDREALLOC
#define SAFE_TYPEDREALLOC(type,p,s)   \
{   if((s)==0){ SAFE_FREE(p); }   \
else if (!(p))    { (p) = (type*) ::malloc((s) * sizeof(type)); ASSERT(p); } \
else {  (p) = (type*) ::realloc((p), (s) * sizeof(type)); ASSERT(p); } }
#endif // #ifndef SAFE_TYPEDREALLOC

//}} robobeg : 2008-01-05

#ifndef SAFE_DELETE_DIALOG
//{{ robobeg : 2009-01-07
//#define SAFE_DELETE_DIALOG(p)	{ DialogLog("SAFE_DELETE_DIALOG"); if(p) { g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg( (p) );		 (p)=NULL; } }
//#define SAFE_DELETE_DIALOG(p)   { DialogLog("SAFE_DELETE_DIALOG"); if ( p ) { bool bRet = g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg(p); ASSERT( bRet ); (p)=NULL; } }
#define SAFE_DELETE_DIALOG(p)   { DialogLog("SAFE_DELETE_DIALOG"); if ( p != NULL ) { bool bRet = g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg(p); ASSERT( bRet ); (p)=NULL; } }
//}} robobeg : 2009-01-07
#endif

//{{ seojt // 2009-8-19, 17:21
#ifndef SAFE_DELETE_DIALOG_HANDLE
#define SAFE_DELETE_DIALOG_HANDLE(p)    { CKTDGUIDialog::Destroy(p.GetHandle()); p.Invalidate(); }
#endif // SAFE_DELETE_DIALOG_HANDLE
//}} seojt // 2009-8-19, 17:21

//{{ seojt // 2009-1-13, 15:44
#ifndef SAFE_DELETE_KTDGOBJECT
#define SAFE_DELETE_KTDGOBJECT(p)   { if(p) { CKTDGObject::DeleteKTDGObject(p); (p)=NULL;} }
#endif // SAFE_DELETE_KTDGOBJECT
//}} seojt // 2009-1-13, 15:44


//{{ seojt // 2009-1-13, 15:44
#ifndef SHARED_POINTER_CAST
#define SHARED_POINTER_CAST(TARGET_TYPE,sharedPointer)     boost::static_pointer_cast<TARGET_TYPE>( sharedPointer )
#endif // SHARED_POINTER_CAST()
//}} seojt // 2009-1-13, 15:44


//#ifndef SAFE_DELETE_XMESH_PLAYER_INSTANCE
//#define SAFE_DELETE_XMESH_PLAYER_INSTANCE(xmesh,p)	{ if(p) { (xmesh)->DestroyInstance( (p) );					 (p)=NULL; } }
//#endif
#ifndef CREATE_VECTOR3
#define CREATE_VECTOR3(v,a,b,c)	{ D3DXVECTOR3 (v); (v).x = (a); (v).y = (b); (v).z = (c); }
#endif  
#ifndef INIT_VECTOR3
#define INIT_VECTOR3(v,a,b,c)	{ (v).x = (a); (v).y = (b); (v).z = (c); }
#endif  
#ifndef CREATE_VECTOR2
#define CREATE_VECTOR2(v,a,b)	{ D3DXVECTOR2 (v); (v).x = (a); (v).y = (b); }
#endif  
#ifndef INIT_VECTOR2
#define INIT_VECTOR2(v,a,b)		{ (v).x = (a); (v).y = (b); }
#endif
#ifndef INIT_COLOR
#define INIT_COLOR(v,red,green,blue,alpha)		{ (v).r = (red); (v).g = (green); (v).b = (blue); (v).a = (alpha); }
#endif

#define SETVECTOR4(vector,_x,_y,_z,_w) {vector.x = _x; vector.y = _y; vector.z = _z; vector.w = _w;}
#define SETVECTOR3(vector,_x,_y,_z) {vector.x = _x; vector.y = _y; vector.z = _z;}
#define SETVECTOR2(vector,_x,_y) {vector.x = _x; vector.y = _y;}





static const D3DXVECTOR3 Y_AXIS( 0.f, 1.f, 0.f);





struct TimeLimited
{
	float m_fTimeLeft;

	virtual void OnTimeOutCallBack() {};

	virtual void OnFrameMove( double fTime, float fElapsedTime )
	{
		if( m_fTimeLeft > 0.f )
		{
			m_fTimeLeft -= fElapsedTime;

			if( m_fTimeLeft <= 0.f )
			{
				m_fTimeLeft = 0.f; 
				OnTimeOutCallBack();
			}
		}
	}


};

struct KTDGSphere
{
	D3DXVECTOR3	center;
	float		fRadius;
};

extern double g_NowTime;




#ifdef APPLY_THEMIDA
#	include "../x2lib/ThemidaSDK.h"
#endif


#include "EnumToString.h"
#include <KncSingleton.h>
#include "KTDXRandomNumbers.h"
#include "KTDXStringTable.h"
#include "KVersionControl.h"

#include "KSystemTimer.h"

#include "ktdxsimplehandleinterface.h"
#include "KTDXLog.h"
#include "debug.h"

#ifndef DLL_BUILD
#include "KncFirewall.h"
#endif 

#include "KTDXErrorDefine.h"
#include "KGCMassFileBufferManager.h"
#include "KGCMassFileManager.h"
#include "CRC_32.h"
#include "KTDXCommonFunc.h"
#include "KTDXRecyclingArray.h"
//{{ robobeg : 2013-12-12
//#include "../luaLib/KLuaManager.h"
//#include "../luaLib/KLuaBinder.h"
#include "../LuaCommon/lua_tinker.h"
#include "../LuaCommon/KLuaManager.h"
#include "../LuaCommon/KLuaBinder.h"
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
#include "KLuaManagerProxy.h"
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
//}} robobeg : 2013-12-12

#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
inline void TableBind( KLuaManager* pLuaManager, KLuabinder* pLuaBinder )
{
	bool	retVal = true;
	int		index = 1;
	string	buffer;

	retVal = pLuaManager->GetValueUtf8(index,buffer);
	while( retVal == true )
	{
		HRESULT hr = pLuaBinder->DoString( buffer.c_str() );
		if( hr != S_OK )
			return;

		retVal = pLuaManager->GetValueUtf8(index,buffer);
		index++;
	}
}
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


#include "delegate.hpp"

#include "KProtectRefTable.h"
#include "KProtectedType.h"
#include "KPCHWebBrowser.h"
#include "KBrowserWrapper.h"
#include "KTDXTimer.h"
#include "KTDXThread.h"

#ifdef CLIENT_COUNTRY_ID
// 2013.05.22 lygan_������ // �ε��׽þ� �������� ����� �뵵
#include "../X2Lib/OnlyGlobal/ID/KNMBrowserWrapper.h"
#endif //AUTH_CJ_ID

#include "KTDXDeviceHolder.h"
#include "KTDXStage.h"

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
#include "KTDGDynamicVBManager.h"
//#endif

//{{ dmlee 2008.05.14 checksum �˻縦 ���� �����忡�� �ϵ��� �ϴ� �ڵ� �׽�Ʈ ��
//#include "KStringUtil.h"
#include "CSLock.h"
#include "KJohnThread.h"

#ifdef CHECKSUM_THREAD_TEST
	#include "KFileVerifier.h"
#endif CHECKSUM_THREAD_TEST
//}} dmlee 2008.05.14 checksum �˻縦 ���� �����忡��

#ifdef KHELP_SYSTEM_TEST // 2008-10-23
#include "KHelpSystem.h"
#endif // KHELP_SYSTEM_TEST // 2008-10-23

//{{ robobeg : 2008-01-05
#include "KFileSerializer.h"
//}} robobeg : 2008-01-05

//{{ seojt // 2009-4-3, 11:53
/** CKTDGUIDialog���� crash �߻�����, handle�� ����ϴ� ������� �ٲٴ� �۾��� �����ϴ�.
    : jintaeks on 2009-04-03, 11:54
*/
#include "KJohnObjMan.h"
#include "KHandle.h"
//}} seojt // 2009-4-3, 11:53

#include "KHangulPPWSelector.h"
#include "MinMax.h"
#include <boost/shared_ptr.hpp>
#include <boost/scope_exit.hpp>

#ifdef CONVERSION_VS
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "libxml-mdd.lib" )
#else
#pragma comment( lib, "libxml-md.lib" )
#endif
#endif CONVERSION_VS



#include "KTDXCollision.h"


class CKTDGObject;
#ifndef X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR
typedef boost::weak_ptr<CKTDGObject>		CKTDGObjectWeakPtr; 
typedef boost::shared_ptr<CKTDGObject>		CKTDGObjectPtr; 
#endif  X2OPTIMIZE_REMOVE_UNNECESSARY_SHARED_PTR


//#include "KTDXXET.h"

#include "KSmartNull.h"
#include "KObserverPtr.h"

#if defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #ifdef MOVIE_TEST	 	// 13-11-11, ���� ���� ����, kimjh, MOVIE_TEST �� ��뿡 �ʿ��� Define �� MOVIE_TEST_BASE �� ����

#include "BandiVideoLibrary.h"
#include "BandiVideoTexture_DX9.h"
#include "BandiVideoFileReader.h"
#include "BandiVideoSound_DS.h"

#endif // defined(REFORM_ENTRY_POINT) && defined( MOVIE_TEST_BASE ) || defined( MOVIE_TEST ) 
// #endif //  MOVIE_TEST	// 13-11-11, ���� ���� ����, kimjh, MOVIE_TEST �� ��뿡 �ʿ��� Define �� MOVIE_TEST_BASE �� ����

#include "KTDXDevice.h"
#include "KTDXDeviceBaseTexture.h"
#ifdef  X2OPTIMIZE_TET_XET_PREPROCESSING
#include "KTDXDeviceTET_Preprocessing.h"
#include "KTDXDeviceXET_Preprocessing.h"
#endif  X2OPTIMIZE_TET_XET_PREPROCESSING
#include "KTDXDeviceTexture.h"
#include "KTDXDeviceXET.h"
#include "KTDXDeviceXMesh.h"
#include "KTDXDeviceXSkinMesh.h"

#include "KTDXDeviceRenderTargetTexture.h"
#include "KTDXDeviceSound.h"
#include "KTDXDeviceManager.h"

#include "KTDXEnumToLua.h"


#include "KTDGManager.h"
#include "KTDIManager.h"
#include "KTDSManager.h"
#ifdef CONVERSION_VS
#include "KTDNPacket.h"
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
#include "KTDNUDPStatisticData.h"
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
#include "KTDNUDP.h"

#else CONVERSION_VS
#include "KTDNManager.h"
#endif CONVERSION_VS

//{{ robobeg : 2008-01-05
#include "KTDXAnimationController.h"
//}} robobeg : 2008-01-05

#ifdef KTDGDEVICEFONT_SIZE_CACHE
#include "KExceptionSafeUtil.h"
#else
#if defined(CLIENT_COUNTRY_TH)
#include "KExceptionSafeUtil.h"
#endif 
#endif

//{{ 09-03-04.hoons. ���� ������� ���̺귯��
#ifdef TEST_SCREEN_CAPTURE
	#if defined(DEBUG) || defined(_DEBUG)
		#pragma comment( lib, "jpegD.lib" )
		#pragma comment( lib, "pngd.lib" )
		#pragma comment( lib, "cxImageD.lib" )
	#else
		#pragma comment( lib, "jpeg.lib" )
		#pragma comment( lib, "png.lib" )
		#pragma comment( lib, "CxImage.lib" )
	#endif
	#include "CXImage/ximage.h"
#endif
//}} hoons.


#include "KTDXApp.h"

extern CKTDXApp* g_pKTDXApp;

#endif
