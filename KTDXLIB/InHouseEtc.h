//////////////////////////////////////////////////////////////////////////
// �̸�: �ֹ�ö
// �۾����۳�¥: 2013-01-04
// ��ġ��¥: 
// �۾�����: ���ӳ� ���� ��Ʈ���� �������Ϸ� ��� // '?' ��ư Ŭ�� �� ���� (����Ʈâ, ��ųâ, ĳ������ Īȣâ, �� â)
//#define PRINT_INGAMEINFO_TO_EXCEL
//////////////////////////////////////////////////////////////////////////

// 2013.4.10 ������ : �������� ������ �Ⱓ�� �������� ������� ��, ������ID�� ���� �������� ���� ���� ����
//#define ITEM_EXPIRE_USING_ITEM_UID

// 2013.4.10 ������ : �ʵ� �ʿ��� �̵��� �� ��ġ�� ��� �� ��ġ �ֺ����� �ǵ��ư��� ���� ����
//#define FIX_FIELD_MAP_SCROLLING

// 2013.4.9 ������ : �ʵ� �ʿ��� ������ �̸��� �̻��� ���� ǥ�õǴ� ���� ����
//#define FIX_PARTY_MEMBER_POSITION


#define FIELD_CHARACTER_SCREEN_SHOT					// �輮�� [2012.02.15] ĳ���� ��ũ���� ��� ����

// ���Ǳ�, ��������Ŀ ���� �޺�Ʈ�� ����
//#define COMBO_TREE_TOOLTIP


//#define RAVEN_WEAPON_TAKER // ���̺�1������ ���� ����Ŀ
// patch �� �� �������� üũ�� �˻����� �ʰ� ���� ������ ������ �����忡�� �˻��ϰ� �ϴ� �׽�Ʈ ver.xml, *.kom ���� �ʿ�.
// ��, vc���� �����ϸ� �ȵ�, Ŭ���̾�Ʈ �ٷ� ����
// fix!!! x2.exe�� �ٸ� ���Ϸ� ��ü�ϸ� nexon passport�� ����� ���� �����Ƿ� x2.exe�� �ٷ� �˻��ؾ��� ��
//#define CHECKSUM_THREAD_TEST

//#define DMLEE_ONLY	// ���� ���� ������ �뵵 �Դϴ�. 

// �޶�ٴ� damage effect test
//#define STICKY_DAMAGE_EFFECT_TEST



// ĳ���Ͱ� ���߸� ����Ʈ�� ���߰�
//#define STOP_UNIT_STOP_EFFECT_TEST



// ���� ���� �׽�Ʈ
//#define TRANSFORMER_TEST




// ������ �߻��� �� �ִ� �κе��� üũ�ؼ� Ȯ���� �� �ֵ��� �Ѵ�.
#define NO_MORE_ERROR_TEST


// �系���� �߻��� �÷��� �α�, ���� �α� ���� ���Ϸ� �޾Ƽ� ������ Ȯ���սô�~
//#define IN_HOUSE_PLAY_LOG_TEST


// ���θ� ������ �ϴ� ���� �׽�Ʈ
#define MARIO_LIKE_BLOCK_TEST


// �ڽ���ſ� ������ �⸧��ũ�� ��ü�ϴ� �׽�Ʈ
//#define FUEL_TANK_TEST

// �������ӿ��� �ƹ� ���̳� ���������� �ε��� �� �ְ�
#define FREE_SELECT_PVP_MAP



// ī�޶� ��ġ �׽�Ʈ, ���ӿ��� ������, �������� ���� ���� �ȵ�
//#define CAMERA_SCRIPTING_TEST

// ���� Ʈ����
//#define MONSTER_TROPHY

// �ٿ�� �޺� �׽�Ʈ
//#define BOUNDING_COMBO_TEST

// ���� ���߷� ���·� ���� �׽�Ʈ
//#define REVERSE_GRAVITY_TEST

// �̺� �� ������ ī�޶� ����������
//#define UP_SIDE_DOWN_CAMERA_TEST

// ���� ���� �� �Ͷ߸���
//#define OVERKILL_TEST

// ���� ���� �� ��ƼŬ ����
//#define MONSTER_DIE_PARTICLE_TEST



// ���� Ÿ��
#define CLIFF_CLIMBING_TEST


// ������ ġƮ�� ��� �������̽�
#ifndef ITEM_CHEAT_POPUP_TEST
	#define ITEM_CHEAT_POPUP_TEST
#endif //ITEM_CHEAT_POPUP_TEST


// �系���� ���� �׽�Ʈ�� ���� �� �� �ְ� 
#define MONSTER_TEST_DUNGEON



// ������ ī�޶� ��ġ, ���� �����ؼ� �÷��� �� �� �ְ� �ϴ� ��� 
#define IN_GAME_MANUAL_CAMERA_POSITION_TEST


// ĳ����, ���� ��ġ�� �߸� �Ǿ��� ��, ��ġ�� �����ϴ� ���
#define RESET_INVALID_UNIT_POSITION_TEST


// ��Ƽ� ������
//#define GRAPPLING_TEST


// �系���������� �α���â���� �޼��� �˾�
//#define IN_HOUSE_ONLY_NOTICE_MESSAGE_POPUP_TEST


// �����߿� ���� ����ϸ� �ش� ������ �ִ� ������ ����Ÿ ���丮�� �߰����ش�
#define DROP_FILE_TEST



// meshplayer���� move axis angle�� 0�� �ƴҶ� �߷°��� �߸� ����Ǵ� ���� �����ϱ�
//#define FIX_XMESH_PLAYER_GRAVITY_ERROR_TEST


// precompile header ����ϴ� �ҽ��ڵ� ���̱� �׽�Ʈ 
#define REDUCED_PRECOMPILED_HEADER_TEST






// p2p ��Ŷ ������ ���� ����
//#define OPTIMIZED_P2P


// ĳ���Ͱ� ���θ� ���̸� ����, #define OPTIMIZED_P2P�� comment out �ؾ� �մϴ�.
//#define LINKED_LINEMAP_JUMP_USER_TEST
#ifdef LINKED_LINEMAP_JUMP_USER_TEST
#undef OPTIMIZED_P2P
#endif LINKED_LINEMAP_JUMP_USER_TEST


// ���Ͱ� ���θ� ���̸� ����
//#define LINKED_LINEMAP_JUMP_MONSTER_TEST

// ���� ������ �㸮����
//#define RENA_SIEGE_MODE_ANGLE_TEST


// ĳ���� �Ӹ��� ī�޶� ���̱�
//#define HEAD_ATTACHED_CAMERA_TEST


// m_FrameDataFuture.stateParam.bLandConnect ��� �����ϱ� ���ؼ�, m_NPCFrameData.stateParam.bLandConnect ã�Ƽ� ����ϴ� �κ� �����ϰų�, ĳ���Ϳ����� �����ϰ� ���ʹ� �״�� ���ֵܵ� �ɵ�
//#define REMOVE_LAND_CONNECT_CONSTRAINTS


// ���� ���� �ö󰡱�
//#define STEP_ON_MONSTER_TEST

// ���� collsion box ��� ����
//#define STEP_ON_MONSTER_COLLISION_BOX_TEST



// FPS ȭ�� ���
//#define FPS_SHOOTING_TEST


// �� ������ �׽�Ʈ
//#define HEAD_INVERSE_KINEMATICS_TEST



// �������� �����ڴ� ĳ���� ���� �޸� �� �ְ�
#define VILLAGE_FLASH_MAN_DEV_ONLY










// ���� ������ ���� ȭ�� �����°� �Ϻ� ���̱� �׽�Ʈ 
#define FIRST_SCREEN_TEST

// ���ʿ��� �ý��� �޼��� �˾� ���̱� �׽�Ʈ
#define HIDE_SYSTEM_MESSAGE


// �������ӿ��� �ʻ�� �� �� �ð� ���ߴ°� ���̱� �׽�Ʈ
//#define SPECIAL_ATTACK_DELAY_TEST

// CHECK opened device list, ���ҽ����� �����ؼ� �� 200frame���� �����ִ� ���ҽ� ��� üũ
//#define CHECK_OPENED_DEVICE



// �����ִ� ���ҽ� ����� ���Ϸ� ���
#define NEW_CHECK_OPENED_DEVICE
//'H' Ű�� ������ ���� �����ִ� ���ҽ� ����� ���Ϸ� �������ݴϴ�. opened resource list.txt
//���������� H�� ������ ���� �ٲ� ���� ����� ����Ʈ ���ݴϴ�.  
//@@@@@@@@@@@@@@@@@@@@			<--- ���� ���� ���� 
//ref(1) size(1) 				<---- ������ 'H' Ű�� ������ ���� referece count�� device size
//---------released ----------   		<---- ������ 'H' Ű�� ������ ���� �����־����� ������ ���� device list





// ���� ���۽� �ε� �ִϸ��̼� skip
//#define SKIP_INTRO_ANIMATION

// ���� �߰� �ε��� �ִϸ��̼�
//#define LOADING_ANIMATION_TEST

// ���� ó�� ������ �� �ε� ���� �������
//#define FIRST_GAME_LOADING_THREAD_TEST	


//{{ seojt // 2008-10-23, 18:48
// ���� ���� �Լ��� Ŭ������ �ڵ����� ���̴� ����� �׽�Ʈ
// - jintaeks on 2008-10-23, 18:49
#define KHELP_SYSTEM_TEST
//}} seojt // 2008-10-23, 18:48

//#define MEMORY_LEAK_DETECTION_TEST







// damageeffect lua ���̺� �ѹ��� �� �ε��ؼ� �޸𸮿� �÷��α�, 
// ĳ���Ϳ� ���Ϳ� damage effect�� ���� ���� 
// ��������, �������ӿ��� �ʿ��� damage effect�� �ҷ������� ����
//#define DAMAGE_EFFECT_LOAD_TEST

// lua �׽�Ʈ2
//#define LUA_TEST_TEST
// lua �׽�Ʈ 
//#define LUA_TEST
// lua gameunit �׽�Ʈ 
//#define LUA_GAMEUNIT_TEST


// �α׸� ���� ���ܼ� ���׸� ��ƺ���~
//#define HEAVY_LOG_TEST




// ���̺� ���� �ʻ��
//#define RAVEN_AIR_SPECIAL_ATTACK_TEST




// ������ hand slash trace ���ֱ�
//#define OLD_HAND_SLASH_TRACE












// singleton_pool test
#define BOOST_SINGLETON_POOL_TEST


// metal slug
//#define SHOOTING_TEST

// ���� ���� ���� �����α� �����
//#define LOADREALFILE_ERROR_LOG

#ifdef LOADREALFILE_ERROR_LOG
#define MASS_FILE_FIRST
#endif 

// ���� ��ü�� ���� ���� ���Ϸ� ����ϱ�, ��ȹ�� �׽�Ʈ �뵵 
// �����: �ƹ������̳� �ѹ� ���� ���� ��µ�~
//#define EXTRACT_ALL_DUNGEON_NPC_INFO




// EVE ���� �������� �׽�Ʈ
//#define EVE_COMBO_TEST_TEST


// ������ �� �ִ� �ڵ� �κ� ��� ���ؼ� �α� ����� ��
#define LEAVE_LAST_ERROR_LOG_TEST




// ������ Ű���� �Է� ������ �� �ְ�
//#define USER_DEFINED_KEYBOARD_SETTING





// ���θ� �ٶ� �׽�Ʈ 
//#define LINEMAP_SLOW_WIND_TEST
//#define LINEMAP_FAST_WIND_TEST


// ���θʿ� �� ���� �簢�� ���� ������ �� �ְ�
//#define LINEMAP_RECT_TEST






// ���� �Ϸ� ����
//#define TODAY_RECORD_TEST


// ��� Ż�� �׽�Ʈ
//#define EQUIP_STRIP_TEST





// tag �׽�Ʈ
//#define TAG_MATCH_TEST



// ������
//#define PVP_BOSS_COMBAT_TEST









// ���� ������ �������� ����ϴ� ���� ���� �� ���Ƽ� �ڵ� ������
//#define ATTACK_DELAY_GAGE




// .kom ���� ������� �ʰ� Ŭ���̾�Ʈ �׽�Ʈ �� �� ���, vld.h�� ����ϰų�, ����׷� �����׼�, ���� ������ ������ �� ���
// .kom������ ������, ���� 5~10�ʸ� ��ٸ��� ������ �ſ� ������.
//#define WITHOUT_MASS_FILE_TEST





//���¿�

//KGCMassFileManager.cpp�� �ڵ� ���� �Ŵ��� �ý����� �����ؾ��� ����� ��..?
//#define INTRUDE_THREAD_LOADING		// ������ ���� ���ֽôϱ� �ʿ� ������
//#define DUNGEON_WORLD_THREAD_LOADING	// ������ ���� ���ֽôϱ� �ʿ� ������
//#define MAKE_GGOBONG_TEST				// ���� �츮 ������ ����� �� �ϴ� �׽�Ʈ.
//#define DASH_CAMERA_ANGLE_TEST		// ��� �ؼ� ���� �Ǹ� ī�޶� �ޱ� ���ݾ� ��ȭ
//#define MONSTER_BABO_TEST				// ���� �ݴ�� ���� �����ϰԲ� 



//#define THROW_WOODEN_PIECE_TEST				// ���� ���� ������ �κ�
//#define PROGRESSIVE_X_SKIN_MESH				// ���α׷��ú� x��Ų �޽� ����
//#define PROGRESSIVE_X_MESH					// ���α׷��ú� �޽� ����
//#define RIDE_MACHINE_GUN						// ��ũ �ö�Ÿ��
//#define FRAME_MOVE_CULL						// �־��� ���� ������ ���� �ٿ��� �θ��Բ�.
//#define UNIT_SYNC_PACKET_DUNGEON_FOR_TEST		// ������ ������ �����ؼ� ���� ������Ʈ ���� �κ�
//#define LEAP_TEST								// ���� ���� �ؼ� ����
//#define	RIDE_HORSE							// ���Ͱ� ���� �ö�Ÿ�°�
//#define BEE_FLY								// �� ���ư���




//#define SKILL_SHORT_CUT_KEY_TEST			// ��ų ����Ű �۾�

//#define X2VIEWER  // x2viewer ���� ������ 

// ���¿�	
//#define WORLD_TOOL
//#define EFFECT_TOOL					// ������ // 2013-02-20 // ���������� ����Ʈ�� ��� �и� �� ����Ʈ �� ���� �۾�
//#ifdef EFFECT_TOOL
#if defined( EFFECT_TOOL ) || defined( X2VIEWER )
	#define WORLD_TOOL
#endif

//#define HALLOWEEN_EVENT			// �ҷ��� �̺�Ʈ ���� ������ ���� �ڵ�
//#define NEW_YEAR_EVENT_DUNGEON				// ���� �̺�Ʈ ���� ������ ���� �ڵ�
#define AUTH_AUTO_SELL_ITEM					// ��ڿ� �ش� ī���� ������ �� �Ⱦƹ����� ���. �׽�Ʈ �뵵�� ���� ����. ������ ���� ����!!

//#define X2TOOL

// ��� ġ������ ������ �־ ����...
//#define SCRIPT_SELECT						// ��ũ��Ʈ ���� Ư�� ����/������ ON/OFF ���������ϰ� ��. �ش� ���� �����ϴ� �ڵ� ��κ� ����.

//#define NEW_MESSENGER

//{{ robobeg : 2008-10-18	

// ���̾� �׽�Ʈ ������带 ���̱� ����, ������� ���� ���� �Ʒ� ��ũ�θ� Ŀ��Ʈ �ƿ��Ѵ�.
//    #define KTDGMANAGER_USE_DISABLE_LAYER  
//}} robobeg : 2008-10-18


// hoons // 2009-09-04 // �ڵ� �޺� & �ڵ� �޸��� �׽�Ʈ
//#define TEST_MACRO_COMBO

//#define POST_QUANTITY_NEW				// ���� ���� UI ����

// cheat command �Է� shortcut key, '/' Ű�� ������ ���������� �Է��� ġƮ��ɾ �����
#define CHEAT_COMMAND_SHORT_CUT_KEY_TEST

// ���� �׽�Ʈ�� ���� ����Ʈ UI
#define CREATE_MONSTER_LIST_TEST


// ���� �۾��� ���� ���� ����Ʈ UI
#define MONSTER_STATE_LIST_TEST




// ĳ���� ��ġ�� ������ �̵������ִ� �Լ�, ����� �系�׽�Ʈ�����θ� ����ϰ� �־ ���񽺹����� ���� �ȵǰ� �и��մϴ�. 
#define ENABLE_SET_UNIT_POSITION_TEST

//{{ ����� : [2010/11/5/] //	���� ����Ʈ �ؽ�Ʈ�� �ε���������� ����Ҷ� ���
//#define EVENT_SCENE_TEXT_USED_INDEX
//}} ����� : [2010/11/5/] //	���� ����Ʈ �ؽ�Ʈ�� �ε���������� ����Ҷ� ���

//#define RENDER_SKIP_TEST

//#define DOWN_FRAME_TEST // ����/���� �������� 65 fps�� �׽�Ʈ �ϱ�����
//#define NUM_TEST_FRAME (65.f)


//#define FIX_BLUE_TEAM_MP_REGEN // ������ : ���� ����� ���� ȸ���� ���ʽ� ���� ���� ����
//#define EXTRA_DAMAGE_CUT_NEGATIVE_RATE // ������ : ����Ʈ�󵥹��� Ȯ���� ������ �� 0���� ó�� (ASSERT ����)
//#define SHOW_OPPONENT_MP_ONLY // ������ : ���� ��� ���� �Ʊ��� �߹ؿ��� MP�� ����� ���� ����
//#define RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING	// ������(�Ϲ�) : ��Ŀ���� ���� �� �Է� �ʱ�ȭ
//#define FIX_BACKJUMP_SHOT_FREEZE // ������ : ���� ���¿��� ���� �� ������� �� ���߿��� ���� ���� ����(hyperStateData.StateEndFuture ��� �Ǽ�)

//#define MAGIC_NEC_FIX // ������(�븸/ȫ��) : ������ ����� ����

#define PREVENT_SUBSTR_ERROR // 2011.09.15 ����ȣ substr error check
//#define BANDICAM_CAPTURE // 2011.9.16	����ȣ  �ݵ�ķ ��ũ���� ĸ�� ����
//#define BANDICAM_RECORDING // 2011.9.16 ����ȣ �ݵ�ķ �۾�
//#define FILDE_ONLY_CHARACTER //2011.11.03 ����ȣ X_x88 �ʵ忡�� ���� �ɸ��͸� ���Դϴ�
//#define POSTBOX_CANCEL_QUANTITY_ITEM_FIX	/// 11.09.26 �ӱԼ� ���� ������ ������ ÷�� ��� ���� ����

//#define FIX_SOUND_DEVICE						// 11.11.28 ������ ���� ����̽� ���� ���� ����
//#define REMOVE_DRAW_PRIMITIVE_UP_3				// 11.11.28 ������ DrawPrimitiveUp �Լ� ����
//#define KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT	// 11.11.28 ������ ��ü ��Ʈ���� ���� directx ��Ʈ ��� �ùķ��̼�
//#define UINPC_TO_SMARTPTR						// 11.11.28 ������ UI NPC ����Ʈ ����Ʈȭ
//#define CLEARTYPE_FONT							// 11.12.01 �ڱ��� Clear Type �׽�Ʈ
//#define DYNAMIC_VERTEX_BUFFER_OPT				// 11.12.12 ������ Vertext Buffer ����ȭ
//#define KTDGDEVICEFONT_BACKGROUND_PRELOADING	// 11.12.12 ������ Device Font Background Loading

// ������ : Ư�� ������ ���
//#define SPECIAL_USE_ITEM

// robobeg : ������ [2011.12.20] //
//#define KTDGDEVICEFONT_SIZE_CACHE

//#define SLASH_TRACE_BUG_FIX			// 11.12.27 ������ ������ ���� ���� ���� ���� ����
//#define REMOVE_DXUT_SAMPLE_CODE		// 11.12.27 ������
//#define DXUT_KTDXLIB_SAFE_ON_RESET	// 11.12.27 ������ 

// 2011.12.14 ������ : �޵����̼Ǵ޸��� ����
//#define FIX_MEDITATION_RUN

//#define DAMAGE_EFFECT_BUG_FIX

#define CAMILLA_SMASH					// �ؿ��� �߰� ����

// ������ : �����ϱ� �� ���� �ؽ� ĳ�÷��� �޸� ������ ���� ����
//#define ADD_COMMA_TO_CASH_ITEM

// �ӱԼ� : ��ƴ���� �ݰ� �������� ������ �ݿ� �ȵǴ� ���� ����
//#define COMPACT_COUNTER_FIX

// ������ : ���Ż���� �޸� Ǫ�þ�Ǫ�� ùŸ ������ ���� �ȵǴ� ���� ����
//#define  MEMO_FIX_PUSH_AND_PUSH

// ������ : ALLY NPC �� ������ Ÿ�� ��� ���� ����
//#define EVOKE_TARGETING_BUG_FIX

#define NOT_USE_UDP_CHECK_INHOUSE		/// ������ �系������ �۵� ���ϵ���!

