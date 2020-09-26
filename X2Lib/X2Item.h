#pragma once

// dmlee 2008.11.29 
// �ϳ��� �����ۿ� 2���� �޽ø� ���� �� �ְ� �ڵ� ����
// ����������� ��� �ϳ��� �޽ø� ���� �� �ִ� ������ �ڵ� ����, ���߿� ���� �ؾ���




#define MAX_SPECIAL_ABILITY_NUM     3
#define MAX_ENCHANT_CARD_NUM    10



class CX2Item
{
public:

#include    "X2Item_Preprocessing.inl"

		enum SPECIAL_SKILL_TYPE
		{
#ifdef SPECIAL_USE_ITEM
			SST_WOODEN_SPEAR = 1,
			SST_LIGHT_SPEAR,
			SST_FIRE_SPHERE,
			SST_WATER_SPHERE,
			SST_NATURE_SPHERE,
			SST_WIND_SPHERE,
			SST_LIGHT_SPHERE,
			SST_DARK_SPHERE,

			SST_CHILDRENSDAY_SHPERE_01,	/// �г��� ���ź
			SST_CHILDRENSDAY_SHPERE_02,	/// ������ ���ź
			SST_CHILDRENSDAY_SHPERE_03,	/// ����� ���ź
			SST_CHILDRENSDAY_SHPERE_04,	/// �ູ�� ���ź

			SST_ANGER_SPHERE,		// ����� ������
#else SPECIAL_USE_ITEM
			SST_THROW_WOODEN_PIECE = 1,
			SST_MAGIC_SPEAR,
			SST_ICE_CRYSTAL,
			SST_FIRE_CRYSTAL,
			SST_MAGIC_BULLET,
			SST_POISON_POCKET,
#endif SPECIAL_USE_ITEM
//#ifdef EVENT_CHINA_THROW_ITEM
			SST_CHINA_EVENT_SPEAR = 1010,
//#endif //EVENT_CHINA_THROW_ITEM
//#ifdef EVENT_ICICLE_THROW_ITEM
			SST_ICICLE_SPEAR = 1011,
//#endif EVENT_ICICLE_THROW_ITEM
#ifdef SERV_RELATIONSHIP_EVENT_INT
			SST_LOVE_LV1,
			SST_LOVE_LV2,
#endif SERV_RELATIONSHIP_EVENT_INT
//#ifdef EVENT_FIRE_CRACKER_THROW_ITEM
			SST_FIRE_CRACKER = 1021,
			SST_GOOD_FIRE_CRACKER = 1022,
//#endif //EVENT_FIRE_CRACKER_THROW_ITEM
		};

        enum SPECIAL_SKILL_MOTION_TYPE
        {
            SSMT_THROW = 1,        //������ ����
            SSMT_INSTALL,        //��ġ�ϴ� ����
            SSMT_DIRECT_USE,    //�ٷ� ����ϴ� ����( �ٶ� ���� )
        };

        enum SUPPLEMENT_ITEM_TYPE
        {
            SIT_PLUS_SUCCESS_RATE = 0,
            SIT_NOT_BROKEN,
        };        


#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef USER_WEAPON_PARTICLE_SCRIPT_TEST
        struct AttachedParticleData
        {
            wstring wstrParticleName;
            wstring wstrBoneName;
        };
#endif USER_WEAPON_PARTICLE_SCRIPT_TEST



#ifdef SUB_EQUIP_TEST
        struct AttachedMeshData
        {
            wstring wstrMeshName;
            wstring wstrBoneName;
#ifdef ATTACH_SKINMESH
            bool    bSkinMesh;
            bool    bJiggle;
#endif
        };
#endif SUB_EQUIP_TEST


        struct SpecialAbility
        {
            SPECIAL_ABILITY_TYPE           m_Type;
            int                            m_CoolTime;
            int                            m_Value1;
            int                            m_Value2;
            int                            m_Value3;
#ifdef SUMMON_MONSTER_CARD_SYSTEM
            wstring                        m_StringValue1;        //Group���� �� ������ �޾� �� �� �ִ� ����
#endif SUMMON_MONSTER_CARD_SYSTEM

            SpecialAbility()
            {
                m_Type            = SAT_NONE;
                m_CoolTime        = 0;
                m_Value1        = 0;
                m_Value2        = 0;
                m_Value3        = 0;
#ifdef SUMMON_MONSTER_CARD_SYSTEM
                m_StringValue1    = L"Group 0";
#endif SUMMON_MONSTER_CARD_SYSTEM
            }

            SPECIAL_ABILITY_TYPE        GetType() const { return m_Type; }
#ifdef  SUMMON_MONSTER_CARD_SYSTEM
            const wchar_t*  GetStringValue1() const     { return m_StringValue1.c_str(); }
#endif  SUMMON_MONSTER_CARD_SYSTEM
        };

        struct ItemTemplet : public CX2ItemTemplet_Base
        {
            USE_TYPE                    m_UseType;                    // ��� ��� ����������� �ƴ���, ��������� ������� �����ϴ���
            USE_CONDITION                m_UseCondition;                // ��� ����
            int                            m_UseLevel;                    // ��� ���� ����
            CX2Unit::UNIT_TYPE            m_UnitType;                    // ��� unittype
            CX2Unit::UNIT_CLASS            m_UnitClass;                // ��� unit Ŭ����


            int                            m_ItemID;                    //������ ID
            std::wstring                m_Name;                        //������ �̸�
            std::wstring                m_Description;                //������ ����
            std::wstring                m_DescriptionInShop;        //������ ������ ���������� ���̴� ����



            wstring                        m_AttachFrameName[ MAX_MODEL_COUNT_A_ITEM ];    // ��� ���� �ٴ°�
            D3DXVECTOR3                    m_FrameOffsetElsword;        //�������� ������(���ҵ�)
            D3DXVECTOR3                    m_FrameOffsetLena;            //�������� ������(����)
            D3DXVECTOR3                    m_FrameOffsetAisha;            //�������� ������(���̻�)
            D3DXVECTOR3                    m_FrameOffsetRaven;            //�������� ������(���̺�)

            D3DXVECTOR3                    m_FrameOffsetEve;            //�������� ������(�̺�)

            //{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef    NEW_CHARACTER_CHUNG
            D3DXVECTOR3                    m_FrameOffsetChung;            // �������� ������(û)
#endif    NEW_CHARACTER_CHUNG
        //}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
            D3DXVECTOR3                    m_FrameOffsetAra;            /// �������� ������(�ƶ�)
#endif




            std::wstring                m_ModelName[ MAX_MODEL_COUNT_A_ITEM ];    //������ ���� _Hyper �� ���δ�
            std::wstring                m_TextureChangeXETName;        //������ ���� _Hyper �� ���δ�
            std::wstring                m_AniXETName;                //������ ���� _Hyper �� ���δ�
            std::wstring                m_AniName;                    //������ ���� _Hyper �� ���δ�
            std::wstring                m_ShopImage;
            std::wstring                m_DropViewer;                //��ӽ� ����� ��ƼŬ �ý��� �̸�



            ITEM_TYPE                    m_ItemType;
            ITEM_GRADE                    m_ItemGrade;                //������ ���
            bool                        m_bFashion;                    //�м� ������
            bool                        m_bVested;                    //�ͼ� ������(�絵�Ҵ�/�ǸźҴ�)            
            bool                        m_bCanEnchant;                //��ȭ���� �������ΰ�
            bool                        m_bCanUseInventory;            //�κ��丮���� ��밡���� �������ΰ� (������ Ŭ���� ��� ���Ǵ� �����۸�, ����Ŭ�ι�, 

            //{{ ����� : [2009/7/29] //    ���� �߰�
            bool                        m_bNoEquip;                    //�����Ҽ� ���� ����ΰ�(��ȯ�� ���)            
            //}} ����� : [2009/7/29] //    ���� �߰�

            //{{ 2010. 01. 05  ������    PC�� �����̾�
            bool                        m_bIsPcBang;                //PC�� ���� ������
            //}}
            //{{ 2011. 10. 14    ������    ��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
            int                            m_iItemLevel;                // ������ ���� ����
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
            //}}

            //{{ 080721.hoons.
            D3DXVECTOR3                    m_RanderScale[CX2Unit::UT_END];        //�������� ĳ���ͺ� ������.
            D3DXVECTOR3                    m_RanderRotate[CX2Unit::UT_END];    //�������� ĳ���ͺ� ȸ��.
            //}}

            bool                        m_bCanHyperMode;            //������ ��� ���� �������ΰ�            

            PERIOD_TYPE                    m_PeriodType;                //������ ����Ⱓ
            int                            m_Endurance;                //�ʱ⳻����
            CMinMax<int>                m_EnduranceDamage;            //1���������÷��̴系���� ���ҷ�
            //{{ 2007. 11. 16  ������  
            float                        m_RepairED;                    //������ 1 ������ ED
            int                            m_RepairVP;                    //������ 1 ������ VP
            //}}
            int                            m_Quantity;                    //1��������

            SHOP_PRICE_TYPE                m_PriceType;                //����Ÿ��
            int                            m_Price;                    //����
            int                            m_PricePvPPoint;            //���Ŵ�������Ʈ





#ifdef COMMON_ITEM_TEMPLET_TEST
            map<CX2Unit::UNIT_TYPE, wstring>    m_mapCommonItemModelName;    // UC_ANYONE�� ��쿡�� ���, ����� ���� ������ ��쿡��
            map<CX2Unit::UNIT_TYPE, wstring>    m_mapCommonItemXETName;
#endif COMMON_ITEM_TEMPLET_TEST

#ifdef BUFF_TEMPLET_SYSTEM
            int                            m_Buff_ID;                    //�����ۿ� ������ ���� Ÿ��
#endif BUFF_TEMPLET_SYSTEM

            CX2Unit::EQIP_POSITION        m_EqipPosition;                //���� ��ġ
            CX2Stat::Stat                m_Stat;                        //������ ����

            vector<CX2BuffFactorPtr>    m_vecBuffFactorPtr;            /// �߻� ��ų ����/����� ��� ����
            std::vector<SpecialAbility>    m_SpecialAbilityList;
            int                            m_CoolTime;
            int                            m_SetID;

            vector<int>                m_vecSocketOption;                    // �� short�� �Ǿ�������?

#ifdef USER_WEAPON_PARTICLE_SCRIPT_TEST
            vector< AttachedParticleData >        m_vecAttachedParticleData;   // note!! �ϴ� ���⿡�� �پ �������� ������.
#endif USER_WEAPON_PARTICLE_SCRIPT_TEST


#ifdef SUB_EQUIP_TEST
            vector< AttachedMeshData >            m_vecAttachedMeshData;   // note!! �ϴ� ���⿡�� �پ �������� ������.
#endif SUB_EQUIP_TEST



#ifdef ITEM_SLASH_TRACE_COLOR_TEST
            D3DXCOLOR                m_coSlashTrace;
            D3DXCOLOR                m_coSlashTraceHyper;
            D3DXCOLOR                m_coSlashTraceTip;
            D3DXCOLOR                m_coSlashTraceTipHyper;
#endif ITEM_SLASH_TRACE_COLOR_TEST

            //{{ kimhc // 2009-08-19 // ������ ����
#ifdef    SEAL_ITEM
            BYTE                    m_ucMaxSealCount;        // �ִ�� ������ �� �ִ� Ƚ��
#endif    SEAL_ITEM
            //}} kimhc // 2009-08-19 // ������ ����

            int                    m_iMaxAttribEnchantCount;
#ifdef PVP_SEASON2_SOCKET
            int                    m_iAttributeLevel;    /// ���� ���� �Ӽ� ����
#endif

            //{{ kimhc // 2009-10-19 // �ִ� MP ���� �� �߰�
            float                m_fAddMaxMP;
            //}} kimhc // 2009-10-19 // �ִ� MP ���� �� �߰�

//#ifdef DUNGEON_ITEM
//            SPECIAL_ITEM_TYPE    m_SpecialItemType;
//            wstring                m_wstrDamageEffectName;
//#endif

            //{{ 2010. 03. 22  ������    ����� ��Ʈ
#ifdef SERV_SKILL_NOTE            
            std::wstring        m_DescriptionInSkillNote;
#endif SERV_SKILL_NOTE
            //}}

#ifdef SERV_PVP_NEW_SYSTEM
            CX2PVPEmblem::PVP_RANK    m_BuyPvpRankCondition;
#endif

            CX2UnitManager::NPC_UNIT_ID    m_iSummonNpcID;
#ifdef HIDE_SET_DESCRIPTION
			bool				m_bHideSetDesc;
#endif HIDE_SET_DESCRIPTION

            ItemTemplet()
            {
                m_UseType                = UT_NONE;
                m_UseCondition            = UC_ANYONE; 
                m_UseLevel                = 0;                // ����
                m_UnitType                = CX2Unit::UT_NONE;    // ����
                m_UnitClass                = CX2Unit::UC_NONE;    // Ŭ����

                m_ItemID                = -1;                //������ ID

                m_ItemType                = IT_NONE;
                m_bFashion                = false;            //�м� ������
                m_bVested                = false;            //�ͼ� ������(�絵�Ҵ�/�ǸźҴ�)                
                m_bCanEnchant            = false;            //��ȭ���� �������ΰ�
                m_bCanUseInventory        = false;            //�κ��丮���� ��밡���� �������ΰ�
                //{{ ����� : [2009/7/29] //    ���� �߰�
                m_bNoEquip                = false;            //�����Ҽ� ���� ����ΰ�(��ȯ�� ���)            
                //}} ����� : [2009/7/29] //    ���� �߰�
                //{{ 2010. 01. 05  ������    PC�� �����̾�
                m_bIsPcBang                = false;            //PC�� ���� ������
                //}}
                //{{ 2011. 10. 14    ������    ��Ʋ�ʵ� ������ ���� �ý���
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
                m_iItemLevel            = 0;                // ������ ���� ����
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
                //}}

                m_ItemGrade                = IG_NONE;

                m_FrameOffsetElsword    = D3DXVECTOR3( 0,0,0 );
                m_FrameOffsetLena        = D3DXVECTOR3( 0,0,0 );
                m_FrameOffsetAisha        = D3DXVECTOR3( 0,0,0 );
                m_FrameOffsetRaven        = D3DXVECTOR3( 0,0,0 );
                m_FrameOffsetEve        = D3DXVECTOR3( 0,0,0 );
                //{{ kimhc // 2010.11.18 // 2010-12-23 New Character CHUNG
#ifdef    NEW_CHARACTER_CHUNG
                m_FrameOffsetChung        = D3DXVECTOR3( 0,0,0 );
#endif    NEW_CHARACTER_CHUNG
                //}} kimhc // 2010.11.18 //  2010-12-23 New Character CHUNG
#ifdef ARA_CHARACTER_BASE
                m_FrameOffsetAra        = D3DXVECTOR3( 0,0,0 );
#endif


                for( int i = CX2Unit::UT_NONE; i < CX2Unit::UT_END; ++i )
                {
                    m_RanderScale[i] = D3DXVECTOR3(1.0f,1.0f,1.0f);
                    m_RanderRotate[i] = D3DXVECTOR3(0.0f,0.0f,0.0f);
                }

                m_bCanHyperMode            = false;            //������ ��� ���� �������ΰ�

                m_PeriodType            = PT_INFINITY;        //������ ����Ⱓ
                m_Endurance                = -1;                //�ʱ⳻����
                m_EnduranceDamage        = CMinMax<int>(0,0);        //1���������÷��̴系���� ���ҷ�
                //{{ 2007. 11. 16  ������  
                m_RepairED                = 0.0f;                //������ 1 ������ ED
                m_RepairVP                = 0;                //������ 1 ������ VP
                //}}
                m_Quantity                = 0;                //1��������

                m_PriceType                = SPT_NONE;            //����Ÿ��
                m_Price                    = 0;                //����
                m_PricePvPPoint            = 0;                //���Ŵ�������Ʈ

                m_EqipPosition            = CX2Unit::EP_NONE;    //���� ��ġ
                m_CoolTime                = 0;
                m_SetID                    = 0;



#ifdef ITEM_SLASH_TRACE_COLOR_TEST
                m_coSlashTrace                = 0xcc5555ff;
                m_coSlashTraceHyper         = 0xccff3333;
                m_coSlashTraceTip            = 0xff5555ff;
                m_coSlashTraceTipHyper        = 0xffff3333;
#endif ITEM_SLASH_TRACE_COLOR_TEST

                //{{ kimhc // 2009-08-19 // ������ ����
#ifdef    SEAL_ITEM
                m_ucMaxSealCount            = 0;    // �⺻ 0, 0�̸� ���� �� �� ����                
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-19 // ������ ����

                m_iMaxAttribEnchantCount    = 0;

#ifdef PVP_SEASON2_SOCKET
                m_iAttributeLevel    = 0;
#endif

                //{{ kimhc // 2009-10-19 // �ִ� MP ���� �� �߰�
                m_fAddMaxMP                    = 0.0f;
                //}} kimhc // 2009-10-19 // �ִ� MP ���� �� �߰�

//#ifdef DUNGEON_ITEM
//                m_SpecialItemType = SIT_NONE;
//                m_wstrDamageEffectName = L"";
//#endif

                //{{ 2010. 03. 22  ������    ����� ��Ʈ
#ifdef SERV_SKILL_NOTE                
                m_DescriptionInSkillNote = L"";
#endif SERV_SKILL_NOTE
                //}}

#ifdef SERV_PVP_NEW_SYSTEM
                m_BuyPvpRankCondition = CX2PVPEmblem::PVPRANK_NONE;
#endif

                m_iSummonNpcID = CX2UnitManager::NUI_NONE;
#ifdef BUFF_TEMPLET_SYSTEM
                m_Buff_ID = 0;
#endif BUFF_TEMPLET_SYSTEM
#ifdef HIDE_SET_DESCRIPTION
				m_bHideSetDesc = false;
#endif HIDE_SET_DESCRIPTION
            }

            int                GetSocketSlotNum() const;
            const wchar_t*     GetNameColor_() const;
            const wchar_t*     GetFullName_() const;    //������ ��޿� ���� Īȣ ���� �̸� : �߰��� �߽����� ��

            int                 GetItemID() const                   { return m_ItemID; }

			bool    GetFashion() const                              { return m_bFashion; }
			bool    GetVested() const                               { return m_bVested; }
            bool    GetCanEnchant() const                           { return m_bCanEnchant; }
            bool    GetCanUseInventory() const                      { return m_bCanUseInventory; }
            bool    GetNoEquip() const                              { return m_bNoEquip; }
            bool    GetIsPcBang() const                             { return m_bIsPcBang; }
            bool    GetCanHyperMode() const                         { return m_bCanHyperMode; }
            USE_TYPE GetUseType() const                             { return m_UseType; }
            USE_CONDITION GetUseCondition() const                   { return m_UseCondition; }
            ITEM_TYPE GetItemType() const                           { return m_ItemType; }
            ITEM_GRADE GetItemGrade() const                         { return m_ItemGrade; }
            PERIOD_TYPE GetPeriodType() const                       { return m_PeriodType; }
            SHOP_PRICE_TYPE GetShopPriceType() const                { return m_PriceType; }

#ifdef  SERV_BATTLE_FIELD_ITEM_LEVEL
            int     GetItemLevel() const                            { return m_iItemLevel; }
#endif  SERV_BATTLE_FIELD_ITEM_LEVEL
            int     GetQuantity() const                             { return m_Quantity; }
            float   GetRepairED() const                             { return m_RepairED; }
            int     GetRepairVP() const                             { return m_RepairVP; }
            int     GetPrice() const                                { return m_Price; }
            int     GetPricePvPPoint() const                        { return m_PricePvPPoint; }

            int     GetSetID() const                                { return m_SetID; }
            //{{ kimhc // 2009-10-19 // �ִ� MP ���� �� �߰�
            float   GetAddMaxMP() const                             { return m_fAddMaxMP; }
            //}} kimhc // 2009-10-19 // �ִ� MP ���� �� �߰�

            int     GetEndurance() const                            { return m_Endurance; }
            int     GetEnduranceDamageMin() const                   { return m_EnduranceDamage.m_Min; }
            int     GetEnduranceDamageMax() const                   { return m_EnduranceDamage.m_Max; }
            CX2UnitManager::NPC_UNIT_ID GetSummonNpcID() const      { return m_iSummonNpcID; }
#ifdef  SERV_PVP_NEW_SYSTEM
            CX2PVPEmblem::PVP_RANK GetBuyPvpRankCondition() const   { return m_BuyPvpRankCondition; }
#endif  SERV_PVP_NEW_SYSTEM
            int     GetUseLevel() const                             { return m_UseLevel; }
            CX2Unit::UNIT_TYPE GetUnitType() const                  { return m_UnitType; }
            CX2Unit::UNIT_CLASS GetUnitClass() const                { return m_UnitClass; }
#ifdef  BUFF_TEMPLET_SYSTEM
            int     GetBuffID() const                               { return m_Buff_ID; }
#endif  BUFF_TEMPLET_SYSTEM
            CX2Unit::EQIP_POSITION GetEqipPosition() const          { return m_EqipPosition; }
            int     GetCoolTime() const                             { return m_CoolTime; }
#ifdef  SEAL_ITEM
            BYTE    GetMaxSealCount() const                         { return m_ucMaxSealCount; }
#endif  SEAL_ITEM
            int     GetMaxAttribEnchantCount() const                { return m_iMaxAttribEnchantCount; }
#ifdef  PVP_SEASON2_SOCKET
            int     GetAttributeLevel() const                       { return m_iAttributeLevel; }
#endif  PVP_SEASON2_SOCKET

            const wchar_t*  GetName() const     { return m_Name.c_str(); }
            const wchar_t*  GetFullName() const { return GetName(); }    //������ ��޿� ���� Īȣ ���� �̸� : �߰��� �߽����� ��
            const wchar_t*  GetDescription() const  { return m_Description.c_str(); }
            const wchar_t*  GetDescriptionInShop() const    { return m_DescriptionInShop.c_str(); }
            const wchar_t*  GetAttachFrameName( unsigned i ) const  { ASSERT( i < MAX_MODEL_COUNT_A_ITEM ); return m_AttachFrameName[i].c_str(); }
            const wchar_t*  GetModelName( unsigned i ) const        { ASSERT( i < MAX_MODEL_COUNT_A_ITEM ); return m_ModelName[i].c_str(); }
            const wchar_t*  GetTextureChangeXETName() const { return m_TextureChangeXETName.c_str(); }
            const wchar_t*  GetAniXETName() const   { return m_AniXETName.c_str(); }
            const wchar_t*  GetAniName() const      { return m_AniName.c_str(); }
            const wchar_t*  GetShopImage() const    { return m_ShopImage.c_str(); }
            const wchar_t*  GetDropViewer() const   { return m_DropViewer.c_str(); }
            const wchar_t*  GetDescriptionInSkillNote() const   { return m_DescriptionInSkillNote.c_str(); }
            const CX2Stat::Stat&  GetStat() const   { return m_Stat; }
            unsigned        GetNumBuffFactorPtr() const { return m_vecBuffFactorPtr.size(); }
            CX2BuffFactorPtr GetBuffFactorPtr( unsigned i ) const   { ASSERT( i < m_vecBuffFactorPtr.size() ); return m_vecBuffFactorPtr[i]; }
            unsigned        GetNumSpecialAbility() const    { return m_SpecialAbilityList.size(); }
            const SpecialAbility&    GetSpecialAbility( unsigned i ) const { ASSERT( i < m_SpecialAbilityList.size() ); return  m_SpecialAbilityList[i]; }
            unsigned        GetNumSocketOption() const  { return m_vecSocketOption.size(); }
            int             GetSocketOption( unsigned i ) const { ASSERT( i < m_vecSocketOption.size() ); return m_vecSocketOption[i]; }

            unsigned        GetNumAttachedParticleData() const { return m_vecAttachedParticleData.size(); }
            const AttachedParticleData&   GetAttachedParticleData( unsigned i ) const { ASSERT( i < m_vecAttachedParticleData.size() ); return m_vecAttachedParticleData[i]; }
            unsigned        GetNumAttachedMeshData() const  { return m_vecAttachedMeshData.size(); }
            const AttachedMeshData& GetAttachedMeshData( unsigned i ) const { ASSERT( i < m_vecAttachedMeshData.size() ); return m_vecAttachedMeshData[i]; }
            const wchar_t*  GetCommonItemModelName( CX2Unit::UNIT_TYPE eUnitType ) const
            {
                std::map<CX2Unit::UNIT_TYPE, wstring>::const_iterator iter = m_mapCommonItemModelName.find( eUnitType );
                if ( iter != m_mapCommonItemModelName.end() )
                    return iter->second.c_str();
                return L"";
            }
            const wchar_t*  GetCommonItemXETName( CX2Unit::UNIT_TYPE eUnitType ) const
            {
                std::map<CX2Unit::UNIT_TYPE, wstring>::const_iterator iter = m_mapCommonItemXETName.find( eUnitType );
                if ( iter != m_mapCommonItemXETName.end() )
                    return iter->second.c_str();
                return L"";
            }
            //void            GetSlashTraceColors( D3DXCOLOR& dwOut_, D3DXCOLOR& dwOutTip_, D3DXCOLOR& dwOutHyper_, D3DXCOLOR& dwOutHyperTip_ ) const;

            class LessID 
            {
            public:
                bool operator() ( const ItemTemplet*& lhs, const ItemTemplet*& rhs )
                {
                    if( lhs->m_ItemID < rhs->m_ItemID )
                        return true;
                    else
                        return false;
                }
            };

            class LessName
            {
            public:
                bool operator() ( const ItemTemplet*& lhs, const ItemTemplet*& rhs )
                {
                    if( lhs->m_Name.compare( rhs->m_Name ) < 0 )
                        return true;
                    else 
                        return false;
                }
            };

        };

#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

        struct ItemData : public CX2ItemData_Base
        {

            UidType				m_ItemUID;           //UID
            int					m_ItemID;

            PERIOD_TYPE			m_PeriodType;
            int					m_Endurance;         //������
            int					m_Quantity;          //����

            int					m_EnchantLevel;

            CX2EnchantItem::ItemEnchantedAttribute    m_EnchantedAttribute;

            int					m_Period;            //���� ¥�� �Ⱓ�� �������ΰ�?
            std::wstring		m_wstrExpirationDate;//��¥

            //{{ kimhc // 2009-08-20 // ������ ����
#ifdef    SEAL_ITEM
            bool				m_bIsSealed;         // ���ε� �����ΰ�?
            BYTE				m_ucTimesToBeSealed; // ���ε� Ƚ��
#endif    SEAL_ITEM
            //}} kimhc // 2009-08-20 // ������ ����

			vector<int>			m_SocketOption;      
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			std::vector< int >	m_vecRandomSocket;	 // �⺻ �̰��� ������ ���� ����
			bool				m_bIsEvaluation;		 // ������ ���� ����
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT
			BYTE				m_byAddedSocketSlot;	// �߰� ���� ���� ��
#endif // ADD_SOCKET_SLOT

        private:

            void    Init()
            {
                m_ItemUID                = 0;                //UID
                m_ItemID                = 0;

                m_PeriodType            = PT_INFINITY;
                m_Endurance                = -1;                //������
                m_Quantity                = 0;                //����

                m_EnchantLevel            = 0;
                m_EnchantedAttribute.Init();

                //m_EnchantedAttribute.m_aEnchantedType[0]         = 0;
                //m_EnchantedAttribute.m_aEnchantedType[1]         = 0;
                //m_EnchantedAttribute.m_aEnchantedType[2]         = 0;

                m_Period                = 0;
                m_wstrExpirationDate    = L"";

                //{{ kimhc // 2009-08-20 // ������ ����
#ifdef    SEAL_ITEM
                m_bIsSealed                = false;                    // ���ε� �����ΰ�?
                m_ucTimesToBeSealed        = 0;                        // ���ε� Ƚ��
#endif    SEAL_ITEM

                m_SocketOption.resize(0);
                //}} kimhc // 2009-08-20 // ������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
                m_vecRandomSocket.resize( 0 );
				m_bIsEvaluation	= true;	// ������ ���� ����
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = 0;	// �߰� ���� ���� ��
#endif // ADD_SOCKET_SLOT
            }

            void    Init( const KInventoryItemInfo& data )
            {
                m_ItemUID               = data.m_iItemUID;                    //UID
                m_ItemID                = data.m_kItemInfo.m_iItemID;

                m_PeriodType            = (CX2Item::PERIOD_TYPE) data.m_kItemInfo.m_cUsageType;
                m_Endurance             = static_cast<int>(data.m_kItemInfo.m_sEndurance);                //������
                m_Quantity              = data.m_kItemInfo.m_iQuantity;                    //����

                m_EnchantLevel          = (int)data.m_kItemInfo.m_cEnchantLevel;
                m_EnchantedAttribute.m_aEnchantedType[0] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0;
                m_EnchantedAttribute.m_aEnchantedType[1] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1;
                m_EnchantedAttribute.m_aEnchantedType[2] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2;

                m_Period                = (int)data.m_kItemInfo.m_sPeriod;
                m_wstrExpirationDate    = data.m_kItemInfo.m_wstrExpirationDate;

                //{{ kimhc // 2009-08-26 // ������ ����
#ifdef    SEAL_ITEM
                m_bIsSealed                = data.m_kItemInfo.IsSealedItem();    // ���ε� �����ΰ�?
                m_ucTimesToBeSealed        = data.m_kItemInfo.GetSealCount();    // ���ε� Ƚ��
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-26 // ������ ����

                m_SocketOption          = data.m_kItemInfo.m_vecItemSocket;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_vecRandomSocket		= data.m_kItemInfo.m_vecRandomSocket;
				// m_cItemState�� �̰��� ���°� �ƴ϶�� ���� �� ������ ����
				m_bIsEvaluation			= (KItemInfo::IS_NOT_EVALUATED != data.m_kItemInfo.m_cItemState);
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = data.m_kItemInfo.m_byteExpandedSocketNum;	// �߰� ���� ���� ��
#endif // ADD_SOCKET_SLOT
            }

            void    Init( const KItemInfo& data )
            {
                m_ItemUID               = -1;                    //UID
                m_ItemID                = data.m_iItemID;

                m_PeriodType            = (CX2Item::PERIOD_TYPE) data.m_cUsageType;
                m_Endurance             = static_cast<int>(data.m_sEndurance);                //������
                m_Quantity              = data.m_iQuantity;                    //����

                m_EnchantLevel          = (int)data.m_cEnchantLevel;
                m_EnchantedAttribute.m_aEnchantedType[0] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant0;
                m_EnchantedAttribute.m_aEnchantedType[1] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant1;
                m_EnchantedAttribute.m_aEnchantedType[2] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant2;

                m_Period                = (int)data.m_sPeriod;
                m_wstrExpirationDate    = data.m_wstrExpirationDate;


                //{{ kimhc // 2009-08-26 // ������ ����
#ifdef    SEAL_ITEM
                m_bIsSealed                = data.IsSealedItem();            // ���ε� �����ΰ�?
                m_ucTimesToBeSealed        = data.GetSealCount();            // ���ε� Ƚ��
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-26 // ������ ����

				m_SocketOption          = data.m_vecItemSocket;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_vecRandomSocket		= data.m_vecRandomSocket;
				// m_cItemState�� �̰��� ���°� �ƴ϶�� ���� �� ������ ����
				m_bIsEvaluation			= (KItemInfo::IS_NOT_EVALUATED != data.m_cItemState);
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = data.m_byteExpandedSocketNum;	// �߰� ���� ���� ��
#endif // ADD_SOCKET_SLOT
            }

            void    Init( const KPostItemInfo& data )
            {
                m_ItemUID               = -1;                    //UID
                m_ItemID                = data.m_iScriptIndex;

                m_PeriodType            = PT_INFINITY;
                m_Endurance                = -1;                //������
                m_Quantity              = data.m_iQuantity;                    //����

                m_EnchantLevel          = (int)data.m_cEnchantLevel;
                m_EnchantedAttribute.m_aEnchantedType[0] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant0;
                m_EnchantedAttribute.m_aEnchantedType[1] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant1;
                m_EnchantedAttribute.m_aEnchantedType[2] = (CX2EnchantItem::ENCHANT_TYPE) data.m_kAttribEnchantInfo.m_cAttribEnchant2;

                m_Period                = 0;
                m_wstrExpirationDate.resize( 0 );

                //{{ kimhc // 2009-08-26 // ������ ����
#ifdef    SEAL_ITEM
                m_bIsSealed                = data.IsSealedItem();            // ���ε� �����ΰ�?
                m_ucTimesToBeSealed        = data.GetSealCount();            // ���ε� Ƚ��
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-26 // ������ ����

				m_SocketOption          = data.m_vecItemSocket;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_vecRandomSocket		= data.m_vecRandomSocket;
				// m_cItemState�� �̰��� ���°� �ƴ϶�� ���� �� ������ ����
				m_bIsEvaluation			= (KItemInfo::IS_NOT_EVALUATED != data.m_cItemState);
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = data.m_byteExpandedSocketNum;	// �߰� ���� ���� ��
#endif // ADD_SOCKET_SLOT
            }

        public:

            ItemData()
            {
                Init();
            }

            ItemData( const KInventoryItemInfo& data )
            {
                Init( data );
            }

            ItemData( const KItemInfo& data )
            {
                Init( data );
            }
//{{ robobeg : 2013-11-04
            bool    Initialize( const KPostItemInfo& data );
//}} robobeg : 2013-11-04

            ItemData& operator=( const KInventoryItemInfo& data )
            {
                m_ItemUID               = data.m_iItemUID;                    //UID
                m_ItemID                = data.m_kItemInfo.m_iItemID;

                m_PeriodType            = static_cast<CX2Item::PERIOD_TYPE>( data.m_kItemInfo.m_cUsageType );
                m_Endurance             = static_cast<int>(data.m_kItemInfo.m_sEndurance);                //������
                m_Quantity              = data.m_kItemInfo.m_iQuantity;                    //����

                m_EnchantLevel          = static_cast<int>( data.m_kItemInfo.m_cEnchantLevel );
				m_SocketOption          = data.m_kItemInfo.m_vecItemSocket;
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				m_vecRandomSocket		= data.m_kItemInfo.m_vecRandomSocket;
				// m_cItemState�� �̰��� ���°� �ƴ϶�� ���� �� ������ ����
				m_bIsEvaluation			= (KItemInfo::IS_NOT_EVALUATED != data.m_kItemInfo.m_cItemState);
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

                m_Period                = static_cast<int>( data.m_kItemInfo.m_sPeriod );
                m_wstrExpirationDate    = data.m_kItemInfo.m_wstrExpirationDate;

                m_EnchantedAttribute.m_aEnchantedType[0] = static_cast<CX2EnchantItem::ENCHANT_TYPE>( data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 );
                m_EnchantedAttribute.m_aEnchantedType[1] = static_cast<CX2EnchantItem::ENCHANT_TYPE>( data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 );
                m_EnchantedAttribute.m_aEnchantedType[2] = static_cast<CX2EnchantItem::ENCHANT_TYPE>( data.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 );

                //{{ kimhc // 2009-08-26 // ������ ����
#ifdef    SEAL_ITEM
                m_bIsSealed                = data.m_kItemInfo.IsSealedItem();    // ���ε� �����ΰ�?
                m_ucTimesToBeSealed        = data.m_kItemInfo.GetSealCount();    // ���ε� Ƚ��
#endif    SEAL_ITEM
                //}} kimhc // 2009-08-26 // ������ ����

#ifdef ADD_SOCKET_SLOT
				m_byAddedSocketSlot = data.m_kItemInfo.m_byteExpandedSocketNum;	// �߰� ���� ���� ��
#endif // ADD_SOCKET_SLOT 
                return *this;
            }



            int GetAttribEnchantedCount() const
            {
                int iCount = 0;

                for( int i = 0; i<ARRAY_SIZE( m_EnchantedAttribute.m_aEnchantedType ) ; i++ )
                {
                    if( m_EnchantedAttribute.m_aEnchantedType[i] > 0 )
                    {
                        ++iCount;
                    }
                }

                return iCount;
            }

        };

    public:
        CX2Item( int itemID );
//{{ robobeg : 2013-11-04
        //CX2Item( ItemData* pItemData, CX2Unit* pOwnerUnit );
        CX2Item( const ItemData& kItemData, CX2Unit* pOwnerUnit );
//}} robobeg : 2013-11-04
        ~CX2Item(void);

//{{ robobeg : 2013-11-04
        //ItemData*        GetItemData()            { return m_pItemData; }
        const ItemData&    GetItemData() const      { return m_kItemData; }
        ItemData&          AccessItemData()         { return m_kItemData; }
//}} robobeg : 2013-11-04
        const ItemTemplet*  GetItemTemplet() const    { return m_pItemTemplet; }
		int					GetItemEnchantLevel() const { return m_kItemData.m_EnchantLevel; }

        UidType            GetUID() const            { return m_kItemData.m_ItemUID; }
        void            SetEqip( bool bEqip )    { m_bEqip = bEqip; }
        bool            GetEqip()                { return m_bEqip; }

        CX2Unit*        GetOwnerUnit()            { return m_pOwnerUnit; }

        //float            GetRepairPriceDiscountRate();
        int                GetEDToRepair();
        int                GetVPToRepair();

        int                GetEDToSell();                //������ �ǸŰ�

        int                GetEDToSocketPush();        //���� �� ������
        int                GetEDToSocketRemove();

        int                GetSocketOptionNum() const;        //����� �ɼ� ���� ( ���� ������ ���� )

#ifdef ITEM_RECOVERY_TEST
        bool            IsDisabled();
#endif


        CX2Stat::Stat   GetStat( bool bIncludeSocketOption = false );
        bool            GetEnchantStat( OUT CX2Stat::Stat& stat_ ) const;
		int             GetIEchantedItemLevel() const;    /// ��ȭ�� �����Ͽ� ������ ������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		CX2Stat::Stat   GetStat( IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_ );
		bool            GetEnchantStat( OUT CX2Stat::Stat& stat_, IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_ ) const;
		int             GetIEchantedItemLevel(IN CX2Unit::UNIT_TYPE eUnitType_, IN CX2Unit::UNIT_CLASS eUnitClass_) const;    /// ��ȭ�� �����Ͽ� ������ ������ ����

		// ���� ���� ���. �⺻���� true
        bool			GetIsEvaluation() const { return m_kItemData.m_bIsEvaluation; }
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

        //+0 ����ĭ ��
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
		wstring            GetFullName( IN const bool bAddRandomSocketName_ = false );
#else // SERV_UPGRADE_TRADE_SYSTEM
		wstring            GetFullName();
#endif // SERV_UPGRADE_TRADE_SYSTEM

#ifdef ADD_SOCKET_SLOT
		BYTE			GetbyAddedSocketSlot() const;
#endif // ADD_SOCKET_SLOT 

        //static ItemData* CreateItemData( const KPostItemInfo& kPostItemInfo );

    public:
        static const int        WEAPON_LEVEL_WEIGHT = 4;            /// ������ ��� ������ ���� �� ����ϴ� ���� �������� ���� ����ġ
        static const int        AVERAGE_ITEM_LEVEL_DIVISOR = 8;        /// ������ ��� ������ ���� �� ����ϴ� ����

    private:
        static const int        STANDARD_VALUE_PHYSIC_ATTACK = 120;    /// ������ 1���� ���� ����ġ (�̸�ŭ�� �����ؾ� 1������ ����)
        static const int        STANDARD_VALUE_MAGIC_ATTACK = 120;    /// ������ 1���� ���� ����ġ    (�̸�ŭ�� �����ؾ� 1������ ����)
        static const int        STANDARD_VALUE_PHYSIC_DEFENCE = 30;/// ������ 1���� ���� ����ġ    (�̸�ŭ�� �����ؾ� 1������ ����)
        static const int        STANDARD_VALUE_MAGIC_DEFENCE = 30;    /// ������ 1���� ���� ����ġ    (�̸�ŭ�� �����ؾ� 1������ ����)
        static const int        STANDARD_VALUE_BASE_HP = 3000;        /// ������ 1���� HP ����ġ    (�̸�ŭ�� �����ؾ� 1������ ����)

        //ItemData*        m_pItemData;
        ItemData        m_kItemData;
        const ItemTemplet*    m_pItemTemplet;
        CX2Unit*        m_pOwnerUnit;

        bool            m_bEqip;
};
