#include "StdAfx.h"
#include "X2SkillNoteManager.h"

#ifdef REFORM_SKILL_NOTE_UI


CX2SkillNoteManager* CX2SkillNoteManager::m_pManager = NULL;
/*static */CX2SkillNoteManager* CX2SkillNoteManager::GetInstance()
{
	if( NULL == m_pManager)
		m_pManager = new CX2SkillNoteManager;

	return m_pManager;
}

/*static*/ void CX2SkillNoteManager::ResetSkillNoteManager()
{
	DestroyInstance();
}

/*static*/ void CX2SkillNoteManager::DestroyInstance()
{
	SAFE_DELETE( m_pManager );
}

CX2SkillNoteManager::CX2SkillNoteManager():
m_eUnitClass(CX2Unit::UC_NONE)
{
}

CX2SkillNoteManager::~CX2SkillNoteManager()
{
}
void CX2SkillNoteManager::AddSkillNoteTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	CX2Unit::UNIT_CLASS eUnitClass;
	LUA_GET_VALUE_ENUM( luaManager, "UNIT_CLASS", eUnitClass, CX2Unit::UNIT_CLASS, CX2Unit::UC_NONE );

	ASSERT( CX2Unit::UC_NONE != eUnitClass );
	if( eUnitClass == CX2Unit::UC_NONE )
		return;

	vector<pair<int, int>> vecMemoList;
	if( luaManager.BeginTable( "MEMO_LIST" ) == true )
	{
		int iTableIndex = 1;
		while( true == luaManager.BeginTable( iTableIndex++  ) )
		{
			int iMemoItemID = -1;
			int iSkillID = -1;
			luaManager.GetValue( 1, iMemoItemID );
			luaManager.GetValue( 2, iSkillID );

			if( -1 != iMemoItemID &&
				-1 != iSkillID )
			{
				vecMemoList.push_back( std::make_pair(iMemoItemID,iSkillID));
			}
			else
			{
				ASSERT(!"SkillNoteTemplet Setting Error");
			}
			luaManager.EndTable();
		}
		luaManager.EndTable();
	}

	if( false == vecMemoList.empty() )
		m_mapSkillNoteTemplet.insert( std::make_pair( eUnitClass, vecMemoList ));
	return;
}
bool CX2SkillNoteManager::OpenScriptFile()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, false );

	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pSkillNoteManager", this );
	if( false == g_pKTDXApp->LoadLuaTinker( L"SkillNoteTemplet.lua" ) )
	{
		ErrorLogMsg( XEM_ERROR147, L"SkillNoteTemplet.lua" );
		return false;
	}

	return true;
}
/** @function : ResetMemoList
	@brief : ���õ� Ŭ������ �°� �޸� ����Ʈ ����
			1. �޸� ����Ʈ UI ����
			2. ���� Ŭ������ ��� ������ �޸� ��� ������ ���
			3. ���� ���ؿ� �°� �޸� ����Ʈ ����
*/
void CX2SkillNoteManager::ResetMemoList()
{
	if( CX2Unit::UC_NONE == m_eUnitClass )
		return;

	if( NULL != g_pData &&
		NULL != g_pData->GetUIManager() &&
		NULL != g_pData->GetUIManager()->GetUISkillNote() )
		g_pData->GetUIManager()->GetUISkillNote()->ResetMemoListDLG();

	m_vecUsableMemoListIfIhave.clear();


	// #1 ���� Ŭ������ �����ؾ� �� ��ü Ŭ���� ���
	vector<CX2Unit::UNIT_CLASS> vecUnitClass;
	vecUnitClass.push_back( m_eUnitClass );
	// ���� Ŭ���� ���
	g_pData->GetUnitManager()->GetLowRankUnitClass(vecUnitClass, m_eUnitClass );

	/*����~2�� ����
		vecUnitClass.size() 3 : 2�������� ����.
		vecUnitClass.size() 2 : 1�������� ����.
		vecUnitClass.size() 1 : 0�������� ����.*/		
	char cClassLine = static_cast<char>(vecUnitClass.size()-1);

	// #2 m_mapMemoList��, ���� Ŭ������ ��� �� �� �ִ� �޸� �߰�
	BOOST_FOREACH( CX2Unit::UNIT_CLASS eUnitClass, vecUnitClass )
	{
		map< CX2Unit::UNIT_CLASS, vector<pair<int, int>> >::const_iterator mit =  m_mapSkillNoteTemplet.find(eUnitClass);
		if( mit != m_mapSkillNoteTemplet.end() )
		{
			typedef pair<int, int> MemoList;
			BOOST_FOREACH( const MemoList& pairMemoList,  mit->second )
			{
				MemoData sMemoData;
				sMemoData.iMemoItemID	= pairMemoList.first;
				sMemoData.iSkillID		= pairMemoList.second;
				sMemoData.cClassLine	= cClassLine;
				sMemoData.iMemoItemNum	= -1;

				m_vecUsableMemoListIfIhave.push_back(sMemoData);
			}
		}
		--cClassLine;
	}

	// #3 �������� �� ���� ���� ��� ���� �� ��� ����
	std::sort(m_vecUsableMemoListIfIhave.begin(), m_vecUsableMemoListIfIhave.end(), MemoListSort());


	// #4 ���� ���� ���� �޸� ���� ������ �� ����
	{
		vector<MemoData>::iterator it = m_vecUsableMemoListIfIhave.begin();
		for( ; it != m_vecUsableMemoListIfIhave.end(); ++ it )
		{
			if( 0 >= it->iMemoItemNum )
				break;
		}

		std::sort(it, m_vecUsableMemoListIfIhave.end(), MemoListClassSort());
	}
}
/** @function : UpdateMemoPossession
	@brief : ���� ���� ����
*/
void CX2SkillNoteManager::UpdateMemoPossession()
{
	if( NULL != g_pData &&
		NULL != g_pData->GetMyUser() &&
		NULL != g_pData->GetMyUser()->GetSelectUnit() )
	{
		const CX2Inventory& pInventory = g_pData->GetMyUser()->GetSelectUnit()->GetInventory();
		BOOST_FOREACH( MemoData& sMemoData , m_vecUsableMemoListIfIhave)
		{
			const int iItemNum = pInventory.GetNumItemByTID( sMemoData.iMemoItemID);
			sMemoData.iMemoItemNum = iItemNum;
		}
	}
}
/** @function : UpdateMemoPossession
	@brief : �޸� ����Ʈ �������ο� ���� ����.
			 (�������� ���� �� ���� �ؾ� �Ѵ�.)
*/
void CX2SkillNoteManager::UpdateSortMemoList()
{
	std::sort(m_vecUsableMemoListIfIhave.begin(), m_vecUsableMemoListIfIhave.end(), MemoListSort());

	// #4 ���� ���� ���� �޸� ���� ������ �� ����
	{
		vector<MemoData>::iterator it = m_vecUsableMemoListIfIhave.begin();
		for( ; it != m_vecUsableMemoListIfIhave.end(); ++ it )
		{
			if( 0 >= it->iMemoItemNum )
				break;
		}

		std::sort(it, m_vecUsableMemoListIfIhave.end(), MemoListClassSort());
	}
}
/** @function : GetSkillIDByMemoItemID
	@brief : �޸� ������ ID�� �ش��ϴ� ��ųID ���
			 bIsMySkill_�� false�� ��� ��ȸ �ð��� ���� �ɸ���.
*/
int CX2SkillNoteManager::GetSkillIDByMemoItemID( int iMemoItemID_ , bool bIsMySkill_ /*= true */) const
{
	if( true == bIsMySkill_ )
	{
		BOOST_FOREACH( const MemoData& sMemoData , m_vecUsableMemoListIfIhave)
		{
			if( sMemoData.iMemoItemID == iMemoItemID_ )
				return sMemoData.iSkillID;
		}
	}
	else
	{
		map< CX2Unit::UNIT_CLASS, vector<pair<int, int>> >::const_iterator it = m_mapSkillNoteTemplet.begin();
		for( ; it != m_mapSkillNoteTemplet.end(); ++it )
		{
			typedef pair<int, int> MemoIDAndSkillID;

			BOOST_FOREACH( const MemoIDAndSkillID& sMemoIDAndSkillID, it->second)
			{
				if( sMemoIDAndSkillID.first == iMemoItemID_ )
					return sMemoIDAndSkillID.second;
			}
			
		}
	}
	return 0;
}
#endif // REFORM_SKILL_NOTE_UI
