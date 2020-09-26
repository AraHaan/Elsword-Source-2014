#pragma once
/**	@file	: X2ViewerUI.h
	@desc	: UI ����
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
	@date	JHKang on 2013-11-20,
		- ���� ��ȯ ����
*/

class CX2ViewerMain;

#define RP_SIZE_X	600
#define RP_SIZE_Y	500

extern bool		g_bShowUI;

#pragma region CXViewerUI
/** @class : CX2ViewerUI
	@brief : UI ����
*/
class CX2ViewerUI : public CX2ViewerObject
{
	public:
		#pragma region MESH_SEL
		enum MESH_SEL
		{
			MS_NONE = 0,	/// ����
			MS_SKIN_MESH,	/// ��Ų �޽�
			MS_MESH,		/// �޽�
			MS_END,			/// ����ü�� ��
		};
		#pragma endregion enum

		#pragma region UI_ID
		enum UI_ID
		{
			UI_NONE	= 0,	/// ����

			#pragma region Base UI
			UI_STATIC_FILE,		/// Text "X/YFile"
			UI_BUT_OPEN,		/// ���� ���� ��ư
			UI_BUT_RESET,		/// �缳�� ��ư

			UI_STATIC_UI,		/// Text "UI"
			UI_BUT_UI_INIT,		/// UI �ʱ�ȭ
			UI_BUT_UI_HIDE,		/// UI �����
			
			UI_STATIC_VIEW,		/// Text "View"
			UI_CHECK_GRID,		/// ���� �� �� ǥ��
			UI_CHECK_WIREFRAME,	/// ���̾� ���������� ����

			UI_STATIC_LIGHT,		/// Text "Light"
			UI_STATIC_LIGHT_POS,	/// Text "LightPos"
			UI_STATIC_LIGHT_POS_X,	/// Text "X"
			UI_STATIC_LIGHT_POS_Y,	/// Text "Y"
			UI_STATIC_LIGHT_POS_Z,	/// Text "Z"
			UI_EDIT_LIGHT_POS_X,	/// EditBox X
			UI_EDIT_LIGHT_POS_Y,	/// EditBox Y
			UI_EDIT_LIGHT_POS_Z,	/// EditBox Z
			UI_CHECK_LIGHT_ONOFF,	/// Light Mehs On / Off

			UI_STATIC_CAMERAMODE,		/// Text "Camera Mode"
			UI_RADIO_CAMERA_NORMAL,		/// Normal Mode
			UI_RADIO_CAMERA_NAVIGATION,	/// Navigation Mode
			UI_BUT_CAMERA_RESET,		/// �ʱ�ȭ
			UI_STATIC_CAMERA_EYE_POS_X,		/// Text "X"
			UI_STATIC_CAMERA_EYE_POS_Y,		/// Text "Y"
			UI_STATIC_CAMERA_EYE_POS_Z,		/// Text "Z"
			UI_EDIT_CAMERA_EYE_POS_X,		/// EditBox X
			UI_EDIT_CAMERA_EYE_POS_Y,		/// EditBox Y
			UI_EDIT_CAMERA_EYE_POS_Z,		/// EditBox Z
			UI_STATIC_CAMERA_LOOK_POS_X,		/// Text "X"
			UI_STATIC_CAMERA_LOOK_POS_Y,		/// Text "Y"
			UI_STATIC_CAMERA_LOOK_POS_Z,		/// Text "Z"
			UI_EDIT_CAMERA_LOOK_POS_X,		/// EditBox X
			UI_EDIT_CAMERA_LOOK_POS_Y,		/// EditBox Y
			UI_EDIT_CAMERA_LOOK_POS_Z,		/// EditBox Z

			UI_STATIC_MODEL,	/// Text "Model List"
			UI_EDIT_MODEL,		/// EditBox Motion Name
			UI_LIST_MODEL,		/// ListBox Model

			UI_STATIC_BGSET,	/// Text "B.G Color"
			UI_STATIC_BG_A,		/// Text "A"
			UI_STATIC_BG_R,		/// Text "R"
			UI_STATIC_BG_G,		/// Text "G"
			UI_STATIC_BG_B,		/// Text "B"
			UI_EDIT_BG_A,		/// EditBox Alpha	( 0 ~ 255 )
			UI_EDIT_BG_R,		/// EditBox Red		( 0 ~ 255 )
			UI_EDIT_BG_G,		/// EditBox Green	( 0 ~ 255 )
			UI_EDIT_BG_B,		/// EditBox Blue	( 0 ~ 255 )
			#pragma endregion �⺻ UI

			#pragma region SkinMesh UI
			UI_STATIC_SCALE,	/// Text "Scale" X����
			UI_STATIC_SCALE_X,	/// Text "X"
			UI_STATIC_SCALE_Y,	/// Text "Y"
			UI_STATIC_SCALE_Z,	/// Text "Z"
			UI_EDIT_SCALE_X,	/// EditBox X
			UI_EDIT_SCALE_Y,	/// EditBox Y
			UI_EDIT_SCALE_Z,	/// EditBox Z

			UI_STATIC_POSITION,		/// Text "Position" X����
			UI_STATIC_POSITION_X,	/// Text "X"
			UI_STATIC_POSITION_Y,	/// Text "Y"
			UI_STATIC_POSITION_Z,	/// Text "Z"
			UI_EDIT_POSITION_X,		/// EditBox X
			UI_EDIT_POSITION_Y,		/// EditBox Y
			UI_EDIT_POSITION_Z,		/// EditBox Z

			UI_STATIC_BONE,			/// Text "Bone List"
			UI_EDIT_BONE,			/// Current Bone Name
			UI_LIST_BONE,			/// Bone ListBox
			UI_CHECK_BONE_MODIFY,	/// ������ �� Bone�� ����

			UI_STATIC_MODIFY_BONE,	/// Text "Modify Bone List"
			UI_EDIT_MODIFY_BONE,	/// Current Bone Name
			UI_LIST_MODIFY_BONE,	/// Modify Bone ListBox

			UI_STATIC_DEFAULT_BONE,	/// Text "Default Setting"
			UI_COMBO_DEFAULT_BONE,	/// �⺻ ���� ��ȯ ��

			UI_BUT_OPEN_BONE,	/// ����� ���� ���� ����
			UI_BUT_SAVE_BONE,	/// ����� ���� ���� ����
			
			UI_CHECK_MOTION_ONOFF,	/// Motion Model �Ѱ� ����
			UI_CHECK_BOUNDING,		/// �ٿ�� �ڽ� ���̱�
			UI_CHECK_ATTACK_BOX,	/// ���� �ڽ� ���̱�
			UI_BUT_RENDER_PARAM,	/// Rendering Parameter ��ȭ����
			#pragma endregion ��� ���� UI

			#pragma region Bone UI
			UI_STATIC_BONE_SCALE,		/// Text "ũ��"
			UI_STATIC_BONE_SCALE_X,		/// Text "X"
			UI_STATIC_BONE_SCALE_Y,		/// Text "Y"
			UI_STATIC_BONE_SCALE_Z,		/// Text "Z"
			UI_EDIT_BONE_SCALE_X,		/// EditBox X
			UI_EDIT_BONE_SCALE_Y,		/// EditBox Y
			UI_EDIT_BONE_SCALE_Z,		/// EditBox Z
			UI_STATIC_BONE_POS_Y,		/// Text "��ġ Y"
			UI_EDIT_BONE_POS_Y,			/// EditBox Y
			#pragma endregion ���õ� Bone ũ�� ����

			#pragma region Equip UI
			UI_STATIC_EQUIP,			/// Text "Equip List"
			UI_LIST_EQUIP,				/// Equip ListBox
			UI_BUT_CLEAR_EQUIP_LIST,	/// Clear Equip List Button
			UI_BUT_OPEN_EQUIP,			/// Open Equip Button

			UI_STATIC_WEAPON,			/// Text "Weapon List"
			UI_LIST_WEAPON,				/// Weapon ListBox
			UI_BUT_CLEAR_WEAPON_LIST,	/// Clear Weapon List Button
			UI_BUT_OPEN_WEAPON,			/// Open Weapon Button

			UI_STATIC_ACCESSORY,			/// Text "Accessory List"
			UI_LIST_ACCESSORY,				/// Accessory ListBox
			UI_BUT_CLEAR_ACCESSORY_LIST,	/// Clear Weapon List Button
			UI_BUT_OPEN_ACCESSORY,			/// Open Weapon Button

			UI_LIST_ATTACH_BONES,			/// Accessory ListBox

			UI_STATIC_SCENE,	/// Text "Scene"
			UI_BUT_SCENE_OPEN,	/// ��� ������ �ҷ���
			UI_BUT_SCENE_SAVE,	/// ���� ����� ���Ϸ� ����
			#pragma endregion ��� ���� UI

			#pragma region Weapon UI
			UI_STATIC_WEAPON_BONE_NAME,		/// Text "���� ��ġ"
			UI_EDIT_WEAPON_BONE_NAME,		/// EditBox Bone Name
			UI_STATIC_WEAPON_BONE_ROT_X,	/// Text "ȸ�� X"
			UI_STATIC_WEAPON_BONE_ROT_Y,	/// Text "ȸ�� Y"
			UI_STATIC_WEAPON_BONE_ROT_Z,	/// Text "ȸ�� Z"
			UI_EDIT_WEAPON_BONE_ROT_X,		/// EditBox X
			UI_EDIT_WEAPON_BONE_ROT_Y,		/// EditBox Y
			UI_EDIT_WEAPON_BONE_ROT_Z,		/// EditBox Z
			#pragma endregion ���õ� ������ ��ġ �� ȸ�� �� ����

			#pragma region Accessory UI
			UI_STATIC_ACCESSORY_BONE_NAME,	/// Text "���� ��ġ"
			UI_EDIT_ACCESSORY_BONE_NAME,	/// EditBox Bone Name
			UI_STATIC_ACCESSORY_X,			/// Text "X"
			UI_STATIC_ACCESSORY_Y,			/// Text "Y"
			UI_STATIC_ACCESSORY_Z,			/// Text "Z"
			UI_STATIC_ACCESSORY_MOVE,		/// Text "�̵�"
			UI_STATIC_ACCESSORY_ROT,		/// Text "ȸ��"
			UI_STATIC_ACCESSORY_SCALE,		/// Text "ũ��"
			UI_EDIT_ACCESSORY_MOVE_X,		/// EditBox X
			UI_EDIT_ACCESSORY_MOVE_Y,		/// EditBox Y
			UI_EDIT_ACCESSORY_MOVE_Z,		/// EditBox Z
			UI_EDIT_ACCESSORY_ROT_X,		/// EditBox X
			UI_EDIT_ACCESSORY_ROT_Y,		/// EditBox Y
			UI_EDIT_ACCESSORY_ROT_Z,		/// EditBox Z
			UI_EDIT_ACCESSORY_SCALE_X,		/// EditBox X
			UI_EDIT_ACCESSORY_SCALE_Y,		/// EditBox Y
			UI_EDIT_ACCESSORY_SCALE_Z,		/// EditBox Z
			#pragma endregion ���õ� Accessory �޽� ����

			#pragma region Animation UI
			UI_BUT_ANIM_PLAY_PAUSE,			/// ���/�Ͻ�����
			UI_BUT_ANIM_STOP,				/// ����
			UI_BUT_ANIM_PREV_FRAME,			/// ���� ������
			UI_BUT_ANIM_NEXT_FRAME,			/// ���� ������
			UI_STATIC_ANIM_FRAME,			/// Text "Frame : % / %, ���� �ð� / ��ü �ð�"
			UI_EDIT_ANIM_FRAME_TIME_INC,	/// �� ������ �̵� �ð�
			UI_STATIC_ANIM_SPEED,			/// Text "speed: %"
			UI_SLIDE_ANIM_SPEED,			/// �ӵ� ���� Slidebar
			UI_COMBO_PLAY_TYPE,				/// ComboBox
			UI_SLIDE_ANIM_FRAME,			/// �ִϸ��̼� ������ ǥ��

			UI_STATIC_ANIMATION,		/// Text "Animation List"
			UI_LIST_ANIMATION,			/// Animation ListBox
			UI_STATIC_ANIM_NUM,			/// ����
			UI_EDIT_ANIM_NAME,			/// ���� ���õ� �ִϸ��̼� �̸�
			UI_BUT_ANIM_NAME_CHANGE,	/// �ִϸ��̼� �̸� ����
			#pragma endregion �ִϸ��̼� ���

			#pragma region Render Parameter UI
			UI_STATIC_RENDERTYPE,	/// Text "RENDER TYPE"
			UI_COMBO_RENDERTYPE,	/// ComboBox Render type

			UI_STATIC_CARTOON_TEX_TYPE,	/// Text "CARTOON TEXTURE TYPE"
			UI_COMBO_CARTOON_TEX_TYPE,	/// ComboBox Texture Type

			UI_STATIC_CULLMODE,	/// Text "CULLMODE"
			UI_COMBO_CULLMODE,	/// ComboBox cull mode

			UI_STATIC_SRCBLEND,	/// Text "SRC BLEND"
			UI_COMBO_SRCBLEND,	/// ComboBox src blencd

			UI_STATIC_DESTBLEND,	/// Text "DEST BLEND"
			UI_COMBO_DESTBLEND,		/// ComboBox dest blend

			UI_CHECK_ALPHA_BLEND,	/// Check Alpah Blend
			UI_CHECK_ZENABLE,		/// Check ZEnable
			UI_CHECK_ZWRITABLE,		/// Check Zwritable

			UI_EDIT_OUTLINE_WIDE,	///	TextBox wide

			UI_STATIC_OUTLINE_COLOR,	/// Text "OUTLINE COLOR"
			UI_STATIC_OUTLINE_COLOR_A,	/// Text "A"
			UI_STATIC_OUTLINE_COLOR_R,	/// Text "R"
			UI_STATIC_OUTLINE_COLOR_G,	/// Text "G"
			UI_STATIC_OUTLINE_COLOR_B,	/// Text "B"
			UI_EDIT_OUTLINE_COLOR_A,	/// EditBox A
			UI_EDIT_OUTLINE_COLOR_R,	/// EditBox R
			UI_EDIT_OUTLINE_COLOR_G,	/// EditBox G
			UI_EDIT_OUTLINE_COLOR_B,	/// EditBox B

			UI_STATIC_COLOR,	/// Text "COLOR" Light Color
			UI_STATIC_COLOR_A,	/// Text "A"
			UI_STATIC_COLOR_R,	/// Text "R"
			UI_STATIC_COLOR_G,	/// Text "G"
			UI_STATIC_COLOR_B,	/// Text "B"
			UI_EDIT_COLOR_A,	/// EditBox A
			UI_EDIT_COLOR_R,	/// EditBox R
			UI_EDIT_COLOR_G,	/// EditBox G
			UI_EDIT_COLOR_B,	/// EditBox B

			UI_STATIC_LIGHTFLOW_WIDE,	/// Text "LIGHT FLOW WIDE(��)"
			UI_EDIT_LIGHTFLOW_WIDE,		/// EditBox wide

			UI_STATIC_LIGHTFLOW_IMPACT,				/// Text "LIGHT FLOW IMPACT"
			UI_STATIC_LIGHTFLOW_IMPACT_MIN,			/// Text "Min"
			UI_STATIC_LIGHTFLOW_IMPACT_MAX,			/// Text "Max"
			UI_STATIC_LIGHTFLOW_IMPACT_ANIMTIME,	/// Text "AnimTime"
			UI_EDIT_LIGHTFLOW_IMPACT_MIN,			/// EditBox Min
			UI_EDIT_LIGHTFLOW_IMPACT_MAX,			/// EditBox Max
			UI_EDIT_LIGHTFLOW_IMPACT_ANIMTIME,		/// EditBox AnimTime

			UI_STATIC_LIGHTFLOW_POINT,		/// Text "LIGHT FLOW POINT"
			UI_STATIC_LIGHTFLOW_POINT_X,	/// Text "X"
			UI_STATIC_LIGHTFLOW_POINT_Y,	/// Text "Y"
			UI_STATIC_LIGHTFLOW_POINT_Z,	/// Text "Z"
			UI_EDIT_LIGHTFLOW_POINT_X,		/// EditBox X
			UI_EDIT_LIGHTFLOW_POINT_Y,		/// EditBox Y
			UI_EDIT_LIGHTFLOW_POINT_Z,		/// EditBox Z

			UI_STATIC_TEXOFFSET_STAGE0,				/// Text "TEXTURE OFFSET STAGE 0"
			UI_STATIC_TEXOFFSET_STAGE0_X,			/// Text "X"
			UI_STATIC_TEXOFFSET_STAGE0_Y,			/// Text "Y"
			UI_STATIC_TEXOFFSET_STAGE0_MIN,			/// Text "Min"
			UI_STATIC_TEXOFFSET_STAGE0_MAX,			/// Text "Max"
			UI_STATIC_TEXOFFSET_STAGE0_ANIMTIME,	/// Text "Anim Time"
			UI_EDIT_TEXOFFSET_STAGE0_MIN_X,			/// EditBox Min X
			UI_EDIT_TEXOFFSET_STAGE0_MIN_Y,			/// EditBox Min Y
			UI_EDIT_TEXOFFSET_STAGE0_MAX_X,			/// EditBox Max X
			UI_EDIT_TEXOFFSET_STAGE0_MAX_Y,			/// EditBox Max Y
			UI_EDIT_TEXOFFSET_STAGE0_ANIMTIME,		/// EditBox Anim Time

			UI_STATIC_TEXOFFSET_STAGE1,				/// Text "TEXTURE OFFSET STAGE 1"
			UI_STATIC_TEXOFFSET_STAGE1_X,			/// Text "X"
			UI_STATIC_TEXOFFSET_STAGE1_Y,			/// Text "Y"
			UI_STATIC_TEXOFFSET_STAGE1_MIN,			/// Text "Min"
			UI_STATIC_TEXOFFSET_STAGE1_MAX,			/// Text "Max"
			UI_STATIC_TEXOFFSET_STAGE1_ANIMTIME,	/// Text "Anim Time"
			UI_EDIT_TEXOFFSET_STAGE1_MIN_X,			/// EditBox Min X
			UI_EDIT_TEXOFFSET_STAGE1_MIN_Y,			/// EditBox Min Y
			UI_EDIT_TEXOFFSET_STAGE1_MAX_X,			/// EditBox Max X
			UI_EDIT_TEXOFFSET_STAGE1_MAX_Y,			/// EditBox Max Y
			UI_EDIT_TEXOFFSET_STAGE1_ANIMTIME,		/// EditBox Anim Time

			UI_STATIC_TEXOFFSET_STAGE2,				/// Text "TEXTURE OFFSET STAGE 2"
			UI_STATIC_TEXOFFSET_STAGE2_X,			/// Text "X"
			UI_STATIC_TEXOFFSET_STAGE2_Y,			/// Text "Y"
			UI_STATIC_TEXOFFSET_STAGE2_MIN,			/// Text "Min"
			UI_STATIC_TEXOFFSET_STAGE2_MAX,			/// Text "Max"
			UI_STATIC_TEXOFFSET_STAGE2_ANIMTIME,	/// Text "Anim Time"
			UI_EDIT_TEXOFFSET_STAGE2_MIN_X,			/// EditBox Min X
			UI_EDIT_TEXOFFSET_STAGE2_MIN_Y,			/// EditBox Min Y
			UI_EDIT_TEXOFFSET_STAGE2_MAX_X,			/// EditBox Max X
			UI_EDIT_TEXOFFSET_STAGE2_MAX_Y,			/// EditBox Max Y
			UI_EDIT_TEXOFFSET_STAGE2_ANIMTIME,		/// EditBox Anim Time

			UI_BUT_PARAM_OK,		/// ���� ��ư
			UI_BUT_PARAM_CANCEL,	/// â�ݱ� ��ư
			#pragma endregion ������ �Ķ���� ���
									
			UI_STATIC_MESH_SCALE,	/// Text "Scale" Y����
			UI_STATIC_MESH_SCALE_X,	/// Text "X"
			UI_STATIC_MESH_SCALE_Y,	/// Text "Y"
			UI_STATIC_MESH_SCALE_Z,	/// Text "Z"
			UI_EDIT_MESH_SCALE_X,	/// EditBox X
			UI_EDIT_MESH_SCALE_Y,	/// EditBox Y
			UI_EDIT_MESH_SCALE_Z,	/// EditBox Z

			UI_STATIC_MESH_LIGHT_POS,	/// Text "LightPos"
			UI_STATIC_MESH_LIGHT_POS_X,	/// Text "X"
			UI_STATIC_MESH_LIGHT_POS_Y,	/// Text "Y"
			UI_STATIC_MESH_LIGHT_POS_Z,	/// Text "Z"
			UI_EDIT_MESH_LIGHT_POS_X,	/// EditBox X
			UI_EDIT_MESH_LIGHT_POS_Y,	/// EditBox Y
			UI_EDIT_MESH_LIGHT_POS_Z,	/// EditBox Z
			UI_CHECK_MESH_LIGHT_ONOFF,	/// Light On/Off Button

			UI_BUT_MESH_RENDER_PARAM,	/// ������ �Ķ���� ��ư

			// Particle Basic
			/*UI_LIST_PARTICLE_LIST,
			UI_BUT_PARTICLE_DELETE,
			
			UI_STATIC_PARTICLE_TIME,
			UI_EDIT_PARTICLE_TIME,

			UI_BUT_PARTICLE_BONESET,
			UI_BUT_PARTICLE_BONECLEAR,
			UI_CHECK_PARTICLE_TRACE,

			UI_STATIC_PARTICLE_OFFSET,
			UI_EDIT_PARTICLE_OFFSET_X,
			UI_EDIT_PARTICLE_OFFSET_Y,
			UI_EDIT_PARTICLE_OFFSET_Z,
			UI_CHECK_PARTICLE_LANDPOS,

			UI_STATIC_PARTICLE_ROT,
			UI_EDIT_PARTICLE_ROT_X,
			UI_EDIT_PARTICLE_ROT_Y,
			UI_EDIT_PARTICLE_ROT_Z,
			UI_CHECK_PARTICLE_APPUNITROT, 

			UI_BUT_PARTICLE_SAVESEQUENCE,

			UI_BUT_PARTICLE_PARTICLEEDITOR,
			// Particle Basic--

			// Particle Editor
			UI_LIST_PARTICLE_EDITOR_MYPARTICLE,
			UI_BUT_PARTICLE_EDITOR_MYPARTICLE_LOAD,
			UI_BUT_PARTICLE_EDITOR_MYPARTICLE_DELETE,
			UI_BUT_PARTICLE_EDITOR_MYPARTICLE_SAVE,

			UI_LIST_PARTICLE_EDITOR_PARTICLETEMPLET,
			UI_BUT_PARTICLE_EDITOR_PARTICLETEMPLET_COPY,
			UI_BUT_PARTICLE_EDITOR_PARTICLETEMPLET_RELOAD,

			UI_STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE,
			UI_LIST_PARTICLE_EDITOR_EMITTERATTRIBUTE,
			UI_STATIC_PARTICLE_EDITOR_EMITTERATTRIBUTE_VALUE,
			UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_SINGLE,
			UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_X,
			UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Y,
			UI_EDIT_PARTICLE_EDITOR_EMITTERATTRIBUTE_Z,
			UI_COMBO_PARTICLE_EDITOR_EMITTERATTRIBUTE,
			UI_BUT_PARTICLE_EDITOR_EMITTERATTRIBUTE_APPLY,
			
			UI_LIST_PARTICLE_EDITOR_EVENT,			//
			UI_BUT_PARTICLE_EDITOR_EVENT_DELETE,

			UI_STATIC_PARTICLE_EDITOR_EVENT_TYPE,
			UI_LIST_PARTICLE_EDITOR_EVENT_TYPE,		//
			UI_STATIC_PARTICLE_EDITOR_EVENT_TIME,
			UI_CHECK_PARTICLE_EDITOR_EVENT_FADE,
			UI_STATIC_PARTICLE_EDITOR_EVENT_FROM,
			UI_STATIC_PARTICLE_EDITOR_EVENT_TO,
			UI_EDIT_PARTICLE_EDITOR_EVENT_TIME1,
			UI_EDIT_PARTICLE_EDITOR_EVENT_TIME2,
			UI_STATIC_PARTICLE_EDITOR_EVENT_VALUE,
			UI_EDIT_PARTICLE_EDITOR_EVENT_SINGLE,
			UI_EDIT_PARTICLE_EDITOR_EVENT_X,
			UI_EDIT_PARTICLE_EDITOR_EVENT_Y,
			UI_EDIT_PARTICLE_EDITOR_EVENT_Z,
			UI_COMBO_PARTICLE_EDITOR_EVENT,
			UI_EDIT_PARTICLE_EDITOR_EVENT_R,
			UI_EDIT_PARTICLE_EDITOR_EVENT_G,
			UI_EDIT_PARTICLE_EDITOR_EVENT_B,
			UI_EDIT_PARTICLE_EDITOR_EVENT_A,
			UI_BUT_PARTICLE_EDITOR_EVENT_APPLY,			
			UI_BUT_PARTICLE_EDITOR_EVENT_NEW,
			
			UI_LIST_PARTICLE_EDITOR_MODEL_LIST,
			UI_BUT_PARTICLE_EDITOR_MODEL_ADD,
			UI_BUT_PARTICLE_EDITOR_MODEL_DELETE,

			UI_STATIC_PARTICLE_EDITOR_PLAY_TYPE,
			UI_COMBO_PARTICLE_EDITOR_PLAY_TYPE,
			UI_BUT_PARTICLE_EDITOR_PLAY,

			UI_STATIC_PARTICLE_EDITOR_SPEED,
			UI_SLIDE_PARTICLE_EDITOR_SPEED,
			UI_STATIC_PARTICLE_EDITOR_TIME,

			UI_BUT_PARTICLE_EDITOR_EXIT,
			// Particle Editor--
			*/
		};
		#pragma endregion enum 

        VecMultiAnimFrame m_vecFrameNameList;
		VecMultiAnimFrame m_vecModifyFrameNameListBase;

	public:
		CX2ViewerUI(CX2ViewerMain* pMain);
		virtual ~CX2ViewerUI(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual HRESULT OnResetDevice();
		virtual HRESULT OnLostDevice();

		void ToggleBaseVisible()
		{
			m_BaseUI.SetVisible( !m_BaseUI.GetVisible() );
		}

		void ToggleSkinMeshVisible()
		{
			m_SkinMeshUI.SetVisible( !m_SkinMeshUI.GetVisible() );
		}

		void ToggleEquipVisible()
		{
			m_EquipUI.SetVisible( !m_EquipUI.GetVisible() );
		}

		void ToggleAnimationVisible()
		{
			m_AnimationUI.SetVisible( !m_AnimationUI.GetVisible() );
		}

		void ToggleRenderParamVisible()
		{
			m_RenderParamUI.SetVisible( !m_RenderParamUI.GetVisible() );
		}

		void SetSkinMeshOnOff( bool bOnOff )
		{
			m_SkinMeshUI.SetVisible( bOnOff );
			m_bSkinMeshUI = bOnOff;
		}
		void SetBoneOnOff( bool bOnOff )
		{
			m_BoneUI.SetVisible( bOnOff );
		}
		void SetEquipUIOnOff( bool bOnOff )
		{
			m_EquipUI.SetVisible( bOnOff );
		}
		void SetWeaponOnOff( bool bOnOff )
		{
			m_WeaponUI.SetVisible( bOnOff );
		}
		void SetAccessoryOnOff( bool bOnOff )
		{
			m_AccessoryUI.SetVisible( bOnOff );
		}
		void SetAnimationOnOff( bool bOnOff )
		{
			m_AnimationUI.SetVisible( bOnOff );
		}
		void SetMeshOnOff( bool bOnOff )
		{
			m_Mesh.SetVisible( bOnOff );
		}
		
		void SetRenderParamOnOff( bool bIsParam )
		{ 
			m_RenderParamUI.SetVisible( bIsParam );
			m_Param.SetParamDlg( &m_RenderParamUI ); 
		}

		void SetEffectButOnOff( bool bIsOnOff )
		{
			//m_BaseUI.GetButton( UI_BUT_EFFECT_SET )->SetVisible( bIsOnOff );
		}
#ifndef UPDATE_X2VIEWER_2013 //JHKang
		void	SetParticleButOnOff( bool bIsOnOff ) { m_ParticleBasic.SetVisible( bIsOnOff ); }

		void	SetParticleEditButOnOff( bool bIsOnOff ) { m_ParticleEditor.SetVisible(bIsOnOff); }
#endif //UPDATE_X2VIEWER_2013		

		void DropFile( std::wstring fileName,std::wstring dir );

		float	GetAnimTimeInc() const { return m_fAnimTimeInc; }
		void	SetAnimTimeInc(float val) { m_fAnimTimeInc = val; }

        CKTDXDeviceXSkinMesh::MultiAnimFrame *GetFrameList(int inx) { return m_vecFrameNameList[inx]; }

		void Init();
		void InitBaseUI();
		void InitSkinMeshUI();
		void InitBoneUI();
		void InitEquipUI();
		void InitWeaponUI();
		void InitAccessoryUI();
		void InitAnimationUI();
		void InitRenderParamUI();
		void InitCharacerStringMap();

		void ResetBaseUI( KLuaManager* luaManager );
		void ResetSkinMeshUI( KLuaManager* luaManager );
		void ResetBoneUI( KLuaManager* luaManager );
		void ResetEquipUI( KLuaManager* luaManager );
		void ResetWeaponUI( KLuaManager* luaManager );
		void ResetAccessoryUI( KLuaManager* luaManager );
		void ResetAnimationUI( KLuaManager* luaManager );
		void ResetRenderParamUI( KLuaManager* luaManager );

		void SetShow( bool bValue_ = false );
		void GetMotionFileList( wstring strParamPath_ );

		void OpenEquip();
		void OpenWeapon();
		void OpenAccessory();
		bool OpenScriptFile( IN const WCHAR* pFileName_, KLuabinder* pKLuabinder );

		bool AddEquip();
		bool AddWeapon();
		bool AddAccessory();

		void SaveScene();
		void SaveBoneFile();

	private:
		static	void CALLBACK OnGUIBaseEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
		static	void CALLBACK OnGUISkinMeshEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
		static	void CALLBACK OnGUIBoneEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
		static	void CALLBACK OnGUIEquipEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
		static	void CALLBACK OnGUIWeaponEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
		static	void CALLBACK OnGUIAccessoryEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
		static	void CALLBACK OnGUIAnimationEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );

		static	void CALLBACK OnGUIMeshEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
		static	void CALLBACK OnGUIRPEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
#ifndef UPDATE_X2VIEWER_2013 //JHKang
		static	void CALLBACK OnGUIParticleEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
		static	void CALLBACK OnGUIParticleEditorEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
#endif //UPDATE_X2VIEWER_2013
		
		void DrawAnimFrame();

#ifndef UPDATE_X2VIEWER_2013 //JHKang
		void DrawParticleTime();
#endif //UPDATE_X2VIEWER_2013

		void InitAnimSpeed();

		// ��ƼŬ ������ ����
		void InitParticleEditor();	// ��ƼŬ ������ �ʱ�ȭ ���� �ڵ�. ������ �ؼ� ���� ��.
		void RefreshEventList();

		float ChangeEditBoxValueToWheel( UI_ID eValue_, CDXUTDialog* pDialog, float fUpValue_ = 1.0f );

	private:
		static	CX2ViewerUI*	ms_pSelfInstance;

		CDXUTDialog		m_BaseUI;			/// Base Dialog
		CDXUTDialog		m_SkinMeshUI;		/// SkinMesh Dialog
		CDXUTDialog		m_BoneUI;			/// SkinMesh Bone Modify Dialog
		CDXUTDialog		m_EquipUI;			/// Equip, Weapon, Accessory Dialog
		CDXUTDialog		m_WeaponUI;			/// Weapon Dialog
		CDXUTDialog		m_AccessoryUI;		/// Accessory Dialog
		CDXUTDialog		m_AnimationUI;		/// Animation Dialog
		CDXUTDialog		m_RenderParamUI;		/// Rendering Parameter Dialog

		CDXUTDialog		m_Mesh;				/// Mesh Dialog
		
		#ifndef UPDATE_X2VIEWER_2013 //JHKang
		CDXUTDialog		m_ParticleBasic;	// �𵨺� ��忡�� ǥ�õǴ� ��ƼŬ ���� UI��
		CDXUTDialog		m_ParticleEditor;	// ��ƼŬ ���� ��� UI��
		#endif //UPDATE_X2VIEWER_2013

		CX2ViewerParam	m_Param;	/// ������ �Ķ���� Ŭ����
		CX2ViewerFileOS	m_FileOS;	/// ���� ����� Ŭ����

		bool			m_bIsInit;		/// �ʱ�ȭ ����
		float			m_fElapsedTime;	/// �帥 �ð�
		CX2ViewerMain*	m_pMain;		/// Main ��ü ������

		MESH_SEL		m_MeshSel;		/// �޽� ����

		std::vector<CDXUTDialog*>	m_vecDialog;	/// Dialog Vector

		float			m_fAnimTimeInc;	/// ������ ������

		std::vector<wstring>	m_strMotionFileList;
		bool	m_bSkinMeshUI;
		bool	m_bModifyBone;
		bool	m_bWeaponUI;
		bool	m_bAccessoryUI;

		WCHAR	m_wstrRootDir[256];

		MapCharacterID m_mapCharacterIDStr;
};
#pragma endregion class