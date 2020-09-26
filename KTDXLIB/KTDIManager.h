#pragma once

#ifdef KEY_MAPPING
// ���� ���ӿ��� ���Ǵ� ��� �׼�. Ű���� �ƴ϶� ��ɺ��� ������.
// Map ������ GameAction -> Key ���� ������ �Ǿ� �����Ƿ�, ���� Action�� �ϳ��� Key�� ������ �� �ִ�.
// �ݴ��, �ϳ��� Action�� ���� Key�� ������ ���� ����.
// Ű ���� ������ �׼��� �߰��ÿ��� enum GAME_ACTION�� �߰��ϰ�, ��ũ�η� GA_~~~ �� ����� ��� ��. (���� ��İ��� ȣȯ�� ����)
// SetDefaultMap�� Action Map���� �߰������ ��.
// ����ó�� Ű���忡 ���� �Ҵ��� ���� GetKeyboard()->GetKeyState( DIK_?? ) ���� ����ϸ� �ȴ�.

#ifdef SERV_KEY_MAPPING_INT
enum JOYSTICK_BUTTON
{
	JB_BUTTON0 = DIJOFS_BUTTON0,
	JB_BUTTON1,
	JB_BUTTON2,
	JB_BUTTON3,
	JB_BUTTON4,
	JB_BUTTON5,
	JB_BUTTON6,
	JB_BUTTON7,
	JB_BUTTON8,
	JB_BUTTON9,
	JB_BUTTON10,
	JB_BUTTON11,
	JB_BUTTON12,
	JB_BUTTON13,
	JB_BUTTON14,
	JB_BUTTON15,
	JB_BUTTON16,
	JB_BUTTON17,
	JB_BUTTON18,
	JB_BUTTON19,
	JB_BUTTON20,
	JB_BUTTON21,
	JB_BUTTON22,
	JB_BUTTON23,
	JB_BUTTON24,
	JB_BUTTON25,
	JB_BUTTON26,
	JB_BUTTON27,
	JB_BUTTON28,
	JB_BUTTON29,
	JB_BUTTON30,
	JB_BUTTON31,
	JB_UP,
	JB_LEFT,
	JB_RIGHT,
	JB_DOWN,
	JB_LEFT_TRIGGER,
	JB_RIGHT_TRIGGER,
};
#endif SERV_KEY_MAPPING_INT

enum GAME_ACTION
{

	GAME_ACTION_NONE = -1,

	GAMEACTION_ATTACK_FAST,		/// Ŀ�ǵ� ��
	GAMEACTION_ATTACK_STRONG,	/// Ŀ�ǵ� ��
	GAMEACTION_SKILL1,			/// ��ų ���� 1
	GAMEACTION_SKILL2,			/// ��ų ���� 2
	GAMEACTION_SKILL3,			/// ��ų ���� 3
	GAMEACTION_SKILL4,			/// ��ų ���� 4
	GAMEACTION_SKILL_EX1,		/// Ȯ�� ��ų ���� 1
	GAMEACTION_SKILL_EX2,		/// Ȯ�� ��ų ���� 2
	GAMEACTION_SKILL_EX3,		/// Ȯ�� ��ų ���� 3
	GAMEACTION_SKILL_EX4,		/// Ȯ�� ��ų ���� 4
	
	//GAMEACTION_SCREENSHOT_MODE,	/// ���� ��� ����
	GAMEACTION_SLOT_CHANGE,		/// ���� ü����
	GAMEACTION_PVE,				/// ���� ����	
	GAMEACTION_PVP,				/// ���� ����
	GAMEACTION_ZOOMIN,			/// Ȯ��
	GAMEACTION_ZOOMOUT,			/// ���
	GAMEACTION_QUEST,			/// ����Ʈ â
	GAMEACTION_MAP,				/// ���� ��, ���� ���൵
	GAMEACTION_COMMUNITY,		/// Ŀ�´�Ƽ â
	GAMEACTION_INFO,			/// ĳ���� ���� â
	GAMEACTION_INVENTORY,		/// ĳ���� ���� â
	GAMEACTION_OPTION,			/// �ý��� �ɼ�
	GAMEACTION_PET,				/// �� ���� â
	GAMEACTION_GUILD,			/// ��� â
	GAMEACTION_SKILL_TREE,		/// ��ų â
	GAMEACTION_NAME,			/// ���� �̸� ON/OFF
	GAMEACTION_FRIEND,			/// ģ�� â
	GAMEACTION_AWAKE,			/// ����
	
	GAMEACTION_QUICKSLOT1,		/// ţ ����1
	GAMEACTION_QUICKSLOT2,		/// ţ ����2
	GAMEACTION_QUICKSLOT3,		/// ţ ����3
	GAMEACTION_QUICKSLOT4,		/// ţ ����4
	GAMEACTION_QUICKSLOT5,		/// ţ ����5
	GAMEACTION_QUICKSLOT6,		/// ţ ����6

	GAMEACTION_UP,				/// ����
	GAMEACTION_DOWN,			/// �Ʒ���
	GAMEACTION_LEFT,			/// ����
	GAMEACTION_RIGHT,			/// ������
	
	GAMEACTION_SIT,				/// �ɱ�

#ifdef RIDING_SYSTEM
	GAMEACTION_RIDING,			/// Ÿ��
#endif //RIDING_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
	GAMEACTION_RELATION,			/// Ŀ��/�ַ� Ŀ�´�Ƽâ ����Ű (��ȥ�ý���)
#endif //ADDED_RELATIONSHIP_SYSTEM

//#ifdef SERV_ADD_WARP_BUTTON
	GAMEACTION_WARP,			/// �ں� �ͽ������� SERV_ADD_WARP_BUTTON
//#endif SERV_ADD_WARP_BUTTON

#ifdef SERV_KEY_MAPPING_INT
	GAMEACTION_RETURN,			/// ä��â�� �����ϰ�, ���� ���.
	GAMEACTION_ESCAPE,			/// ��� ���. Ű������ ESC�� �ٸ� ����, �� �׼��� SystemMenu�� LastMsgByEsc�� ȣ������ �ʴ´�.
	GAMEACTION_PARTYREADY,		/// F8
#endif // SERV_KEY_MAPPING_INT

	GAMEACTION_END,
	GAMEACTION_TOTAL,				// ��ü
};
#endif KEY_MAPPING

#include "KTDIDevice.h"
#ifdef  CHANGE_KEY_DEVICE
#include "KTDIDeviceWinKey.h"
#endif

//////////////////////////////////////////////////////////////////////////
// 09.10.21 ���¿� : #define KEY_MAPPING : DIManager ��� ��� ����.
// ������ GetKeyboard() �� ���� ���� ������ �����ϰ�, DIManager �ܺο����� DIManager ������ �����ϵ���.
// DIManager �ȿ����� Action Mapping�� ���ؼ� ������� �����Ѵ� : Ű ���� �����ϵ��� ����.
// ���� ��İ��� ȣȯ�� ���� �ڵ� �������� ��ũ�η� ȣ���ϰ� ����. #define KEY_MAPPING�� ���� ��� ��� ���θ� ���� �����ϴ�.
//////////////////////////////////////////////////////////////////////////

#ifdef KEY_MAPPING
// Mapping ���. �� ��ɿ� �ش��ϴ� GAMEACTIONS Enum�� Define�� �ּ���.
#define GET_KEY_STATE( action_id )				g_pKTDXApp->GetDIManager()->GetActionState( action_id )
#define GET_DOUBLEKEY_STATE( action_id )		g_pKTDXApp->GetDIManager()->GetDoubleActionState( action_id )
#define GET_DOUBLEKEYPURE_STATE( action_id )	g_pKTDXApp->GetDIManager()->GetDoubleActionPureState( action_id )
#define GET_PUREKEY_STATE( action_id )			g_pKTDXApp->GetDIManager()->GetPureActionState( action_id )
#define SET_KEYLOCK( action_id , bstate )		g_pKTDXApp->GetDIManager()->SetLock( action_id , bstate )
// Game Actions

#define GA_ATTACK_FAST			GAMEACTION_ATTACK_FAST
#define GA_ATTACK_STRONG		GAMEACTION_ATTACK_STRONG

#define GA_SKILL1				GAMEACTION_SKILL1
#define GA_SKILL2				GAMEACTION_SKILL2
#define GA_SKILL3				GAMEACTION_SKILL3
#define GA_SKILL4				GAMEACTION_SKILL4
#define GA_SKILL_EX1			GAMEACTION_SKILL_EX1
#define GA_SKILL_EX2			GAMEACTION_SKILL_EX2
#define GA_SKILL_EX3			GAMEACTION_SKILL_EX3
#define GA_SKILL_EX4			GAMEACTION_SKILL_EX4

//#define GA_SCREENSHOT_MODE		GAMEACTION_SCREENSHOT_MODE
#define GA_SLOT_CHANGE			GAMEACTION_SLOT_CHANGE
#define GA_PVE					GAMEACTION_PVE
#define GA_PVP					GAMEACTION_PVP
#define GA_ZOOMIN				GAMEACTION_ZOOMIN
#define GA_ZOOMOUT				GAMEACTION_ZOOMOUT
#define GA_QUEST				GAMEACTION_QUEST
#define GA_MAP					GAMEACTION_MAP
#define GA_COMMUNITY			GAMEACTION_COMMUNITY
#define GA_INFO					GAMEACTION_INFO
#define GA_INVENTORY			GAMEACTION_INVENTORY
#define GA_OPTION				GAMEACTION_OPTION
#define GA_PET					GAMEACTION_PET
#define GA_GUILD				GAMEACTION_GUILD
#define GA_SKILL_TREE			GAMEACTION_SKILL_TREE
#define GA_NAME					GAMEACTION_NAME
#define GA_FRIEND				GAMEACTION_FRIEND
#define GA_AWAKE				GAMEACTION_AWAKE

#define GA_QUICKSLOT1			GAMEACTION_QUICKSLOT1
#define GA_QUICKSLOT2			GAMEACTION_QUICKSLOT2
#define GA_QUICKSLOT3			GAMEACTION_QUICKSLOT3
#define GA_QUICKSLOT4			GAMEACTION_QUICKSLOT4
#define GA_QUICKSLOT5			GAMEACTION_QUICKSLOT5
#define GA_QUICKSLOT6			GAMEACTION_QUICKSLOT6

#define GA_UP					GAMEACTION_UP
#define GA_DOWN					GAMEACTION_DOWN
#define GA_LEFT					GAMEACTION_LEFT
#define GA_RIGHT				GAMEACTION_RIGHT

#define GA_SIT					GAMEACTION_SIT

#ifdef RIDING_SYSTEM
#define GA_RIDING				GAMEACTION_RIDING
#endif //RIDING_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
#define GA_RELATION				GAMEACTION_RELATION			/// Ŀ��/�ַ� Ŀ�´�Ƽâ ����Ű (��ȥ�ý���)
#endif //ADDED_RELATIONSHIP_SYSTEM

#define GA_WARP					GAMEACTION_WARP			// SERV_ADD_WARP_BUTTON

#ifdef SERV_KEY_MAPPING_INT
#define GA_RETURN				GAMEACTION_RETURN
#define GA_ESCAPE				GAMEACTION_ESCAPE
#define GA_PARTYREADY			GAMEACTION_PARTYREADY
#endif // SERV_KEY_MAPPING_INT


#ifdef KEY_MAPPING_UPGRADE

#define GA_RESURRECTION_PLAYER		GAMEACTION_RESURRECTION_PLAYER
#define GA_RESURRECTION_PARTY1		GAMEACTION_RESURRECTION_PARTY1
#define GA_RESURRECTION_PARTY2		GAMEACTION_RESURRECTION_PARTY2
#define GA_RESURRECTION_PARTY3		GAMEACTION_RESURRECTION_PARTY3
#define GA_RESURRECTION_CANCEL		GAMEACTION_RESURRECTION_CANCEL
#define GA_QUEST_WINDOW				GAMEACTION_QUEST_WINDOW
#define GA_INVENTORY_WINDOW			GAMEACTION_INVENTORY_WINDOW
#define GA_CHARACTER_WINDOW			GAMEACTION_CHARACTER_WINDOW
#define GA_COMMUNITY_WINDOW			GAMEACTION_COMMUNITY_WINDOW
#define GA_SKILL_WINDOW				GAMEACTION_SKILL_WINDOW
#define GA_PARTY_WINDOW				GAMEACTION_PARTY_WINDOW
#define GA_OPTION_WINDOW			GAMEACTION_OPTION_WINDOW
#define GA_WORLDMAP					GAMEACTION_WORLDMAP

#define GA_FRIEND_WINDOW			GAMEACTION_FRIEND_WINDOW
#define GA_NPC_NAME_SHOW			GAMEACTION_NPC_NAME_SHOW
#define GA_PAT_WINDOW				GAMEACTION_PAT_WINDOW
#define GA_GUILD_WINDOW				GAMEACTION_GUILD_WINDOW

#define GA_HELP						GAMEACTION_HELP
#define GA_FRIEND_PARTY_SHOW		GAMEACTION_FRIEND_PARTY_SHOW
#define GA_SD_ONOFF					GAMEACTION_SD_ONOFF
#define GA_DYNAMIC_CAMERA_ONOFF		GAMEACTION_DYNAMIC_CAMERA_ONOFF
#define GA_EFFECT_SOUND_ONOFF		GAMEACTION_EFFECT_SOUND_ONOFF
#define GA_BGM_SOUND_ONOFF			GAMEACTION_BGM_SOUND_ONOFF
#define GA_GUIDEBOOK_ONOFF			GAMEACTION_GUIDEBOOK_ONOFF
#define GA_SCREENSHOT				GAMEACTION_SCREENSHOT
//{{ 2011.9.16 ����ȣ �ݵ�ķ �۾�
#ifdef BANDICAM_RECORDING
#define GA_RECORD_VIDEO				GAMEACTION_RECORD_VIDEO
#define GA_PAUSE_VIDEO				GAMEACTION_PAUSE_VIDEO
#endif BANDICAM_RECORDING
//}}
#define GA_CHATTING_CHANGE			GAMEACTION_CHATTING_CHANGE
#define GA_CAMERA_EXTEND			GAMEACTION_CAMERA_EXTEND
#define GA_CAMERA_REDUCE			GAMEACTION_CAMERA_REDUCE

#endif KEY_MAPPING_UPGRADE

#else KEY_MAPPING

// ���� ���. �� ��ɿ� �ش��ϴ� DIK�� define �� �ּ���.
#define GET_KEY_STATE( action_id )				g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState( action_id )
#define GET_DOUBLEKEY_STATE( action_id )		g_pKTDXApp->GetDIManager()->Getkeyboard()->GetDoubleKeyState( action_id )
#define GET_DOUBLEKEYPURE_STATE( action_id )	g_pKTDXApp->GetDIManager()->Getkeyboard()->GetDoubleKeyPureState( action_id )
#define GET_PUREKEY_STATE( action_id )			g_pKTDXApp->GetDIManager()->Getkeyboard()->GetPureKeyState( action_id )
#define SET_KEYLOCK( action_id , bstate )		g_pKTDXApp->GetDIManager()->Getkeyboard()->SetLock( action_id , bstate )
// // Game Actions
#define GA_UP					DIK_UP
#define GA_DOWN					DIK_DOWN
#define GA_LEFT					DIK_LEFT
#define GA_RIGHT				DIK_RIGHT
#define GA_ATTACKZ				DIK_Z
#define GA_ATTACKX				DIK_X
#define GA_SPECIALA				DIK_A
#define GA_SPECIALS				DIK_S
#define GA_SPECIALD				DIK_D
#define GA_SPECIALC				DIK_C
#define GA_AWAKE				DIK_LCONTROL
#define GA_CHANGE_SKILLSLOT		DIK_SPACE

#define GA_RETURN				DIK_RETURN
#define GA_ESCAPE				DIK_ESCAPE
#define GA_PARTYREADY			DIK_F8

#define GA_QUICKSLOT1			DIK_1
#define GA_QUICKSLOT2			DIK_2
#define GA_QUICKSLOT3			DIK_3
#define GA_QUICKSLOT4			DIK_4
#define GA_QUICKSLOT5			DIK_5
#define GA_QUICKSLOT6			DIK_6

#endif KEY_MAPPING

class CKTDIManager : public CKTDXStage
{
#ifdef KEY_MAPPING
private:
	struct DoubleKey
	{
		bool	bFirstDown;
		bool	bKeyUp;
		float	fTimeOut;

		bool	bDoubleOK;
		bool	bDoubleKeyLock;

		void Init()
		{
			bFirstDown		= false;
			bKeyUp			= false;
			fTimeOut		= 0.0f;

			bDoubleOK		= false;
			bDoubleKeyLock	= false;
		}

		DoubleKey()
		{
			Init();
		}
	};
#endif KEY_MAPPING

	public:
		CKTDIManager( HWND hWnd, HINSTANCE hInst );
		virtual ~CKTDIManager();

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );

		void CreateKeyboard();
		void CreateMouse( BOOL windowed );
		void CreateJoystic();

		void DestroyKeyboard();
		void DestroyMouse();
		void DestroyJoystic();

		void SetEnable( bool bEnabled ){ m_bEnabled = bEnabled; }

#ifdef  CHANGE_KEY_DEVICE
		CKTDIDeviceWinKey* Getkeyboard(){ return m_KTDIKeyboard; }
#else
		CKTDIDevice* Getkeyboard(){ return m_KTDIKeyboard; }
#endif
		CKTDIDevice* GetMouse(){ return m_KTDIMouse; }
		CKTDIDevice* GetJoystic(){ return m_KTDIJoystic; }

#ifdef KEY_MAPPING
		void SetDefaultMap();

		BOOL  GetActionState( GAME_ACTION Action );
		BOOL  SetActionState( GAME_ACTION Action, BOOL State );
		BOOL  GetPureActionState( GAME_ACTION Action );
		BOOL  GetDoubleActionPureState( GAME_ACTION Action );
		BOOL  GetDoubleActionState( GAME_ACTION Action );
		void  SetDoubleActionReset( GAME_ACTION Action );

		void SetEnableJoyStick( bool bEnabled ){ m_bJoyEnable = bEnabled; }
		bool GetEnableJoyStick(){ return m_bJoyEnable; }

		BOOL GetLock( GAME_ACTION Action );
		BOOL SetLock( GAME_ACTION Action, BOOL State = TRUE );

		bool UpdateActionState( float fElapsedTime );
		void HandleAction(UINT nAction, bool bState, long lData = 0);

		void ApplyActionInput( GAME_ACTION Action, float fElapsedTime );

		bool ListenDeviceInput( unsigned char& ucoutUserInput, InputDevices eType );

#ifdef SERV_KEY_MAPPING_INT
		void SetKeyChanging( bool bKeyChanging );
		void SetGamePadDefaultMap();
#endif // SERV_KEY_MAPPING_INT

#endif KEY_MAPPING

#ifdef  CHANGE_KEY_DEVICE

		//void    MsgPeek_KeyMsgOnly( HACCEL hAccel = NULL );

		//bool    MsgPeek_QueuedKeyMsg( MSG& msg );

		void    MsgProc_KeyMsg(     HWND hWnd_
			, UINT uMsg_
			, WPARAM wParam_
			, LPARAM lParam_ );

		//bool    IsSkipKeyProcess() { return m_bSkipKeyProcess; }

#endif

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
		bool IsActivated();
		void ActivateApp();
		void DeactivateApp();
#endif

	private:
		bool		Init();
		void		UpdateData( float fElapsedTime );

		HWND			m_hWnd;
		HINSTANCE		m_hInst;
		IDirectInput8*	m_pDI;

#ifdef  CHANGE_KEY_DEVICE
		//std::deque<std::pair<MSG,WORD> >     m_dequeWinMsg;
		CKTDIDeviceWinKey*	m_KTDIKeyboard;
		//bool                m_bSkipKeyProcess;
		//bool                m_bInPeek;
#else
		CKTDIDevice*	m_KTDIKeyboard;
#endif
		CKTDIDevice*	m_KTDIMouse;
		CKTDIDevice*	m_KTDIJoystic;

		bool			m_bEnabled;

#ifdef KEY_MAPPING
		bool			m_State[GAMEACTION_END];
		bool			m_Locks[GAMEACTION_END];
		DoubleKey		m_DoubleKeyCheck[GAMEACTION_END];

		//{{100518 ������ �����е� Ȱ��/��Ȱ�� ����� ���Ͽ�
		bool			m_bJoyEnable;
		//}}100518 ������ �����е� Ȱ��/��Ȱ�� ����� ���Ͽ�
#ifdef SERV_KEY_MAPPING_INT
		bool			m_bKeyChanging;
#endif // SERV_KEY_MAPPING_INT
#endif KEY_MAPPING

#ifdef RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING
		bool			m_bActivated;
#endif
};
