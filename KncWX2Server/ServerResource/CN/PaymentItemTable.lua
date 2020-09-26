﻿-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.



-- 지우자
--DropTable:SetNotifyDropItem( true )


DUNGEON_KINDS = 
{
	ALL_DUNGEON = 0,
	EXP_DUNGEON = 1,
	LEVEL_DUNGEON = 2,
}

--DropTable:AddPaymentDungeon( DUNGEON_ID["DI_EVENT_NIGHT_WATCHER"], { iItemID = 80002144, fRate = 100, iQuantity = 1 } )

--[[ 세팅 예시
DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],{ iItemID = 69010, fRate = 80} )   
모든 던전 클리어시 아이템(69010)을 80% 확률로 지급

DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],{ iItemID = 69011, fRate = 10.5, iQuestID = 10060 } )   
모든 던전 클리어시 소유한 퀘스트(10060)가 존재하면 아이템(69011)을 10.5% 확률로 지급

DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],{ iItemID = 69012, fRate = 30,  iMaxLv = 20  } ) 
모든 던전 클리어시 레벨이 20이하 이면 아이템(69012)을 30% 확률로 지급

DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],{ iItemID = 69013, fRate = 100,  iMinLv = 40  } )
모든 던전 클리어시 레벨이 40이상 이면 아이템(69013)을 100% 확률로 지급

DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],{ iItemID = 69014, fRate = 100, iMinLv = 40, iMaxLv = 50 } ) 
모든 던전 클리어시 레벨이 40이상, 50이하 이면 아이템(69014)을 100% 확률로 지급

DropTable:AddPaymentDungeon( DUNGEON_KINDS["EXP_DUNGEON"],{ iItemID = 69015, fRate = 100, iQuestID = 10060, iMinLv = 40 } )
모든 던전 클리어시 레벨이 40이상 이고 소유한 퀘스트(10060)가 존재하면 아이템(69015)을 100% 확률로 지급

DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],{ iItemID = 69016, fRate = 100, iQuestID = 10060, iMinLv = 20, iMaxLv = 50 } ) 
모든 던전 클리어시 레벨이 20이상, 50이하 이고 소유한 퀘스트(10060)가 존재하면 아이템(69016)을 100% 확률로 지급

DropTable:AddPaymentDungeon( DUNGEON_KINDS["EXP_DUNGEON"],{ iItemID = 69017, fRate = 50 } )
경험치를 획득한 던전을 클리어시 아이템(69017)을 50% 확률로 지급
(다른 경우 위와 동일 합니다.)

DropTable:AddPaymentDungeon( 30000, { iItemID = 69018, fRate = 80} )
엘의 나무(노말)던전을 클리어시 아이템(69018)을 80% 확률로 지급
(다른 경우 위와 동일 합니다.)
(던전 아이디의 경우 난이도 별로 스크립트 추가 하셔야 합니다.)

				"조건을 사용할 금지 던전ID( DUNGEON_KINDS 사용할 수 없음 )",					"조건"
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_EL_TREE_NORMAL"]			,{ iItemID = 135246, fRate = 45 } )

DUNGEON_ID["DI_RUBEN_EL_TREE_NORMAL"]에서 { iItemID = 135246, fRate = 45 } 조건이 설정되어 있더라도 무시한다.
( 기간 조건은 포함되지 않음 )
--]]


--PaymentItemTable.lua 추가 된 조건 만족 시 던전 결과창( Clear 일때만 )에 아이템이 추가로 지급 됩니다.
--                             필수         필수         필수          선택       선택    선택         선택
--   던전별 아이템셋팅     ( 던전종류,{ 지급 아이템ID, 지급 확률, 소유 퀘스트ID, MIN_LV, MAX_LV , 피씨방 유저 지급 확률} )
-----------------------------------------------------------------------------------------------------------
-- 추가 필수 조건
-- iGoingQuestID = 진행중인 퀘스트
-- 추가 선택 조건
-- iTitleID1, iTitleID2 = 칭호, eUnitType = 유닛 타입, bPartyPay = 조건 만족시 파티 전체 지급 여부
-- bComeback = 복귀유저(사용시 확인 필요), iEquippedItemID1, iEquippedItemID2 = 장착장비, iUseItemID = 사용 아이템, bMustParty = 파티여부
-- iConsumingItemID = 아이템 소모하고 획득, iAllyNPCID = 동료NPC
-----------------------------------------------------------------------------------------------------------


DropTable:AddPaymentDungeon( DUNGEON_KINDS["EXP_DUNGEON"],		{ iItemID = 135039, fRate = 100, iQuestID = 10146 } )  -- 경험치 받는 던전, 10146 퀘스트 보유 시, 135039 아이템을 100% 지급.
DropTable:AddPaymentDungeon( DUNGEON_KINDS["EXP_DUNGEON"],		{ iItemID = 135044, fRate = 100, iQuestID = 10156 } )  -- 경험치 받는 던전, 10156 퀘스트 보유 시, 135044 아이템을 100% 지급.
DropTable:AddPaymentDungeon( DUNGEON_KINDS["EXP_DUNGEON"],		{ iItemID = 160039, fRate = 100, iQuestID = 10165 } )

--DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],		{ iItemID = 69010, fRate = 100, iQuestID = 10060, iMinLv = 1, iMaxLv = 20 } )  -- 모든던전,1Lv~20Lv 사이 유저이고,10060 퀘스트를 보유 중이면,69010 아이템 100% 지급.
--DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],		{ iItemID = 69011, fRate = 100, iQuestID = 10060, iMinLv = 40, iMaxLv = 60 } )  -- 모든던전
--DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],		{ iItemID = 69012, fRate = 100, iQuestID = 10060 } )  -- 모든던전
--DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],		{ iItemID = 69013, fRate = 100,  iMaxLv = 20  } )  -- 모든던전
--DropTable:AddPaymentDungeon( DUNGEON_KINDS["ALL_DUNGEON"],		{ iItemID = 69014, fRate = 100,  iMinLv = 40  } )  -- 모든던전

--DropTable:AddPaymentDungeon( ALL_DUNGEON,		{ iItemID = 69010, fRate = 0, iQuestID = 10060, iMinLv = 1, iMaxLv = 20 } )  -- 모든던전
--DropTable:AddPaymentDungeon( ALL_DUNGEON,		{ iItemID = 69011, fRate = 0, iQuestID = 10060, iMinLv = 40, iMaxLv = 60 } )  -- 모든던전
--DropTable:AddPaymentDungeon( ALL_DUNGEON,		{ iItemID = 69012, fRate = 0, iQuestID = 10060 } )  -- 모든던전
--DropTable:AddPaymentDungeon( ALL_DUNGEON,		{ iItemID = 69013, fRate = 0, iMinLv = 1, iMaxLv = 20  } )  -- 모든던전
--DropTable:AddPaymentDungeon( ALL_DUNGEON,		{ iItemID = 69013, fRate = 0, iMinLv = 40, iMaxLv = 60  } )  -- 모든던전

--DropTable:AddPaymentDungeon( DUNGEON_KINDS["EXP_DUNGEON"],		{ iItemID = 69016, fRate = 100  } )  -- 경험치 받는 던전, 69016 아이템을 100% 지급.
--DropTable:AddPaymentDungeon( DUNGEON_KINDS["EXP_DUNGEON"],		{ iItemID = 69021, fRate = 100, iMaxLv = 20  } )  -- 경험치 받는 던전, 20lv 이하 일 경우, 69021 아이템을 100% 지급.

--DropTable:AddPaymentDungeon( 30000, { iItemID = 69017, fRate = 100, iMinLv = 1, iMaxLv = 20 } )	--엘의 나무(노말)던전 클리어 시, 1Lv~20Lv 사이 유저이면, 69017 아이템을 100% 지급.
--DropTable:AddPaymentDungeon( 30000, { iItemID = 69018, fRate = 100, iMinLv = 15, iMaxLv = 30 } )
--DropTable:AddPaymentDungeon( 30000, { iItemID = 69019, fRate = 100, iMinLv = 16, iMaxLv = 20 } )
--DropTable:AddPaymentDungeon( 30000, { iItemID = 69020, fRate = 100, iMinLv = 17, iMaxLv = 400 } )

--어둠의 문 클리어 시, 마룡의 비늘 조각 (마룡 악세 제조 재료), 암흑의 크리소(교환용) 아이템을 100% 지급. > 어둠의 문 시즌3
DropTable:AddPaymentDungeon(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_NORMAL"]		, { iItemID = 250001580, fRate = 100, iQuantity = 2 } )
--DropTable:AddPaymentDungeon(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_NORMAL"]		, { iItemID = 111383,	fRate = 100, iQuantity = 3, iMinLv = 35	} )
--DropTable:AddPaymentDungeon(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_NORMAL"]		, { iItemID = 78610,	fRate = 100, iQuantity = 1, iMinLv = 35	} )

--발렌타인 던전 이벤트
--DropTable:AddPaymentDungeon( DUNGEON_ID["DI_EVENT_VALENTINE_DUNGEON_INT"],		{ iItemID = 141000974, fRate = 100, iQuantity = 1  } )  

--[[
-- 2013년 크리스마스 이벤트
DropTable:AddPaymentDungeon(	DUNGEON_KINDS["LEVEL_DUNGEON"],		{ iItemID = 153000084, fRate = 100, iGoingQuestID = 140190 } )  -- 경험치 받는 던전, 140190 퀘스트 보유 시, 펭귄산타의 잃어버린 선물 아이템을 100% 지급.
DropTable:AddPaymentDungeon(	DUNGEON_KINDS["LEVEL_DUNGEON"],		{ iItemID = 153000084, fRate = 100, iGoingQuestID = 140200 } )  -- 경험치 받는 던전, 140200 퀘스트 보유 시, 펭귄산타의 잃어버린 선물 아이템을 100% 지급.
DropTable:AddPaymentDungeon(	DUNGEON_KINDS["LEVEL_DUNGEON"],		{ iItemID = 153000084, fRate = 100, iGoingQuestID = 140210 } )  -- 경험치 받는 던전, 140210 퀘스트 보유 시, 펭귄산타의 잃어버린 선물 아이템을 100% 지급.

DropTable:AddPaymentDungeon(	DUNGEON_KINDS["LEVEL_DUNGEON"],		{ iItemID = 153000081, fRate = 100, iConsumingItemID = 153000080, iAllyNPCID = 616, wstrBeginDate = '2013-12-18 00:00:00', wstrEndDate = '2013-12-27 00:00:01', } )  -- 경험치 받는 던전, 선물 보따리를 보유하고 펭귄 산타NPC를 소환 중, 이벤트 기간이 맞다면 신비의 얼음 큐브 아이템을 100% 지급.
DropTable:AddPaymentDungeon(	DUNGEON_KINDS["LEVEL_DUNGEON"],		{ iItemID = 153000081, fRate = 100, iConsumingItemID = 153000080, iAllyNPCID = 616, wstrBeginDate = '2013-11-18 00:00:00', wstrEndDate = '2013-12-18 00:00:01', } )  -- 위 테스트용
DropTable:AddPaymentDungeon(	DUNGEON_KINDS["LEVEL_DUNGEON"],		{ iItemID = 153000098, fRate = 100, iQuestID = 140220 } )  -- 만렙 인증 징표 (던전)

-- 아라 소선 이벤트
DropTable:AddPaymentDungeon( ALL_DUNGEON,		{ iItemID = 60007341, fRate = 100, iQuestID = 63750, wstrBeginDate = '2013-11-26 14:00:00', wstrEndDate = '2013-12-17 23:59:00'  } )  -- 모든던전 -- ES_JP 13.07.24 아라 이벤트
DropTable:AddPaymentDungeon( ALL_DUNGEON,		{ iItemID = 60007341, fRate = 100, iQuestID = 63750, wstrBeginDate = '2013-12-18 00:00:00', wstrEndDate = '2013-12-24 09:59:00'  } )  -- 모든던전 -- ES_JP 13.07.24 아라 이벤트
--]]
-- 아라 신 전직 전야 이벤트
--DropTable:AddPaymentDungeon( DUNGEON_KINDS["LEVEL_DUNGEON"],		{ iItemID = 85003832, fRate = 100, iQuestID = 85660, wstrBeginDate = '2014-01-06 00:00:00', wstrEndDate = '2014-01-15 09:59:00'  } )  




-- 코보 PC방 쿠폰 (2012-12-20 ~ 2013-02-28)
--DropTable:AddPaymentDungeon(	DUNGEON_KINDS["EXP_DUNGEON"],	{ iItemID = 135093, fPcBangUserRate = 100, wstrBeginDate = '2012-12-20 00:00:00', wstrEndDate = '2013-02-28 07:00:00', iQuantity = 1 } )	-- 코보 PC방 쿠폰

--[[
-- 현자의 주문서
DropTable:AddPaymentDungeon( DUNGEON_KINDS["LEVEL_DUNGEON"],{ iItemID = 160267, fRate = 100, iMaxLv = 45, wstrBeginDate = '2013-01-26 12:00:00', wstrEndDate = '2013-01-26 13:00:00' } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_EL_TREE_NORMAL"]			,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_NORMAL"]		,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_HARD"]			,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_EXPERT"]		,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_SWAMP_NORMAL"]				,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_SWAMP_HARD"]				,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_SWAMP_EXPERT"]				,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_ELDER_HENIR_SPACE"]				,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_NORMAL"]	,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_HARD"]		,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_EXPERT"]	,{ iItemID = 160267, fRate = 100, iMaxLv = 45 } )

-- 현자의 마법석
DropTable:AddPaymentDungeon( DUNGEON_KINDS["LEVEL_DUNGEON"],{ iItemID = 135184, fRate = 100, iMinLv = 46, wstrBeginDate = '2013-01-26 12:00:00', wstrEndDate = '2013-01-26 13:00:00' } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_EL_TREE_NORMAL"]			,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_NORMAL"]		,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_HARD"]			,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_RUIN_OF_ELF_EXPERT"]		,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_SWAMP_NORMAL"]				,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_SWAMP_HARD"]				,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_RUBEN_SWAMP_EXPERT"]				,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_ELDER_HENIR_SPACE"]				,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_NORMAL"]	,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_HARD"]		,{ iItemID = 135184, fRate = 100 } )
DropTable:AddUnPaymentCondition(	DUNGEON_ID["DI_DEFENCE_DUNGEON_ELDER_EXPERT"]	,{ iItemID = 135184, fRate = 100 } )

-- 강화 장비 드랍 이벤트
DropTable:SetEnchantItemDropEvent
{
	m_wstrBeginDate	= '2013-01-26 12:00:00',
	m_wstrEndDate	= '2013-01-26 13:00:00',
	
	--DEFAULT_SETTING_INFO =
	{
		iMinLevel = 1,
		iMaxLevel = 40,
		
		{	m_cEnchantLevel = 6,	m_fRate = 45,						},
		{	m_cEnchantLevel = 7,	m_fRate	= 30,						},
		{	m_cEnchantLevel = 8,	m_fRate	= 16,						},
		{	m_cEnchantLevel = 9,	m_fRate	= 6,						},
		{	m_cEnchantLevel = 10,	m_fRate	= 3,						},
	},
	
	{
		iMinLevel = 41,
		iMaxLevel = 55,
		
		{	m_cEnchantLevel = 6,	m_fRate	= 49.8,						},
		{	m_cEnchantLevel = 7,	m_fRate	= 33.2,						},
		{	m_cEnchantLevel = 8,	m_fRate	= 14,						},
		{	m_cEnchantLevel = 9,	m_fRate	= 2,						},
		{	m_cEnchantLevel = 10,	m_fRate	= 1,						},
	},
	
	{
		iMinLevel = 56,
		iMaxLevel = 65,
		
		{	m_cEnchantLevel = 6,	m_fRate	= 54,						},
		{	m_cEnchantLevel = 7,	m_fRate	= 36,						},
		{	m_cEnchantLevel = 8,	m_fRate	= 8.5,						},
		{	m_cEnchantLevel = 9,	m_fRate	= 1,						},
		{	m_cEnchantLevel = 10,	m_fRate	= 0.5,						},
	},
}
--]]
