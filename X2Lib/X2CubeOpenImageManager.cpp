#include "StdAfx.h"

#ifdef CUBE_OPEN_IMAGE_MANAGER

#include "X2CubeOpenImageManager.h"

CX2CubeOpenImageManager::CX2CubeOpenImageManager() :
	m_iNowCubeID( 0 )
	,m_bPlaying(false)
	,m_bStart(false)
	,m_bSoundPlaying(false)
	,m_bNowResultEventCubePlaying(false)
	,m_bShowLastImage(false)
{

}

CX2CubeOpenImageManager::~CX2CubeOpenImageManager()
{
	MapCubeOpenImageInfoItor mit = m_mapCubeOpenImageInfo.begin();
	while(mit != m_mapCubeOpenImageInfo.end())
	{
		SAFE_DELETE(mit->second);
		++mit;
	}
	//m_mapCubeOpenImageInfo.clear();
}

bool CX2CubeOpenImageManager::OpenScriptFile( const WCHAR* pFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pCubeOpenImageManager", this );

	return g_pKTDXApp->LoadLuaTinker( pFileName );
}


bool CX2CubeOpenImageManager::AddImageInfo_LUA()
{
	int					iID = 0;
	CubeOpenImageInfo*	pImageInfo = new CubeOpenImageInfo;
	std::vector<int>	vecResultID;	// ����� iD
	std::vector<std::wstring>	vecResultImageName; // ��� �ۿ� ���� �̹��� �̸�

	KLuaManager luaManager(g_pKTDXApp->GetLuaBinder()->GetLuaState());
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder());
#endif X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE_RETURN(	luaManager, "ID",			iID,			0,	return false; );

	if( luaManager.BeginTable( "m_setCubeID" ) == true )
	{
		int tableIndex = 1;
		int iCubeID = 0;
		while( luaManager.GetValue( tableIndex, iCubeID) == true )
		{
			pImageInfo->m_setCubeID.insert(iCubeID);
			tableIndex++;
		}

		luaManager.EndTable();
	}
	
	if( luaManager.BeginTable( "m_vecImageName" ) == true )
	{
		int tableIndex = 1;
		std::wstring wstrImageName;	// ������ �ѹ�
		while( luaManager.GetValue( tableIndex, wstrImageName) == true )// ����as
		{

			pImageInfo->m_vecImageName.push_back( wstrImageName );// ����
			tableIndex++;
		}

		luaManager.EndTable();
	}

	LUA_GET_VALUE(			luaManager, "m_wstrSoundName",		pImageInfo->m_wstrSoundName,		L"" );

	LUA_GET_VALUE(			luaManager, "m_fChangeTime",		pImageInfo->m_fChangeTime,			0.f );
	LUA_GET_VALUE(			luaManager, "m_bStringendo",		pImageInfo->m_bStringendo,			false );

	LUA_GET_VALUE(			luaManager, "m_bLoop",		pImageInfo->m_bLoop,			true );

	LUA_GET_VALUE(			luaManager, "m_iStrIndexOpen",			pImageInfo->m_iStrIndexOpen,				STR_ID_809);
	LUA_GET_VALUE(			luaManager, "m_iStrIndexNoKey",			pImageInfo->m_iStrIndexNoKey,				STR_ID_4834);
	LUA_GET_VALUE(			luaManager, "m_bShowResult",			pImageInfo->m_bShowResult,				true);
	LUA_GET_VALUE(			luaManager, "m_bShowResultCustom",			pImageInfo->m_bShowResultCustom,				false);

	LUA_GET_VALUE(			luaManager, "m_bHideBar",			pImageInfo->m_bHideBar,				false);
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
	LUA_GET_VALUE(			luaManager, "m_bImageFadeInOut",			pImageInfo->m_bImageFadeInOut,				false);
#endif //#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
	if( luaManager.BeginTable( "m_vecResultItemID" ) == true )
	{
		int tableIndex = 1;
		int iItemID = 0;
		while( luaManager.GetValue( tableIndex, iItemID) == true )
		{
			vecResultID.push_back( iItemID );
			tableIndex++;
		}

		luaManager.EndTable();
	}

	if( luaManager.BeginTable( "m_vecResultImageName" ) == true )
	{
		int tableIndex = 1;
		std::wstring wstrImageName;	// ������ �ѹ�
		while( luaManager.GetValue( tableIndex, wstrImageName) == true )
		{
			vecResultImageName.push_back( wstrImageName );
			tableIndex++;
		}

		luaManager.EndTable();
	}

	if(vecResultID.size() != 0 && (vecResultID.size() == vecResultImageName.size() ))
	{
		for(int i = 0; i < vecResultID.size(); ++i)
		{
			pImageInfo->m_mapResultImageInfo.insert(std::make_pair(vecResultID[i], vecResultImageName[i]));
		}
	}


	if(m_mapCubeOpenImageInfo.find(iID) != m_mapCubeOpenImageInfo.end())
	{
		ErrorLog(XEM_ERROR12);
		return false;
	}
	else
	{
		m_mapCubeOpenImageInfo.insert(std::make_pair(iID, pImageInfo));
	}

	return true;	
}

bool CX2CubeOpenImageManager::IsEventCube( const int iCubeID )
{
	m_setResultItemID.clear();
	m_bShowLastImage = false;

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if( GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
		return false;
	else
		return true;
}

bool	CX2CubeOpenImageManager::GetCubeImageName(IN float fProgressOpen, IN const int iCubeID, OUT std::wstring& wstrName)
{
	wstrName.clear();

	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		return false;
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return false;
	}

	if(pCubeImageInfo->m_vecImageName.size() == 0)
	{
		dbg::cerr << L"�̹��� ���� ������ ����ִ�!! ť��ID : " << iCubeID << dbg::endl;
		return false;
	}


	// �̹��� ��ü�ð��� ������ �׳� �̹��� ����
	int iImageNum = 0;
	if(pCubeImageInfo->m_fChangeTime == 0)
	{

	}
	else if( false == pCubeImageInfo->m_bLoop && true == m_bShowLastImage )
	{
		iImageNum =  pCubeImageInfo->m_vecImageName.size() - 1;
	}
	else
	{
		float fTime = pCubeImageInfo->m_fChangeTime;
		if(pCubeImageInfo->m_bStringendo == true) // ���� ������
		{
			fTime = (pCubeImageInfo->m_fChangeTime * (1.5f - fProgressOpen));
			if(fTime < 0.03f)
				fTime = 0.03f;
		}

		iImageNum = static_cast<int>(fProgressOpen / fTime ) % pCubeImageInfo->m_vecImageName.size();
		if( false == pCubeImageInfo->m_bLoop && iImageNum == pCubeImageInfo->m_vecImageName.size() - 1)
			m_bShowLastImage = true;
	}

	if(static_cast<int>(pCubeImageInfo->m_vecImageName.size()) <= iImageNum)
	{
		dbg::cerr << L"�ش� index�� �̹��� ������ ����!! ť��ID : " << iCubeID << L"Index : " << iImageNum << dbg::endl;
		return false;
	}
	
	wstrName = pCubeImageInfo->m_vecImageName[iImageNum];

	return true;
}

bool	CX2CubeOpenImageManager::GetSoundName(IN const int iCubeID, OUT std::wstring& wstrName)
{
	wstrName.clear();

	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		return false;
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return false;
	}

	if(pCubeImageInfo->m_wstrSoundName == L"")
	{
		return false;
	}
	else
	{
		wstrName = pCubeImageInfo->m_wstrSoundName;
		return true;
	}
}

bool	CX2CubeOpenImageManager::GetResultImageName(IN const int iCubeID, IN const int iResultItemID, OUT std::wstring& wstrName)
{
	wstrName.clear();

	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		return false;
	}

	if(iResultItemID <= 0)
	{
		dbg::cerr << L"��� ������ ID�� �߸��Ǿ���." << dbg::endl;
		return false;
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return false;
	}

	std::map<int, std::wstring>::const_iterator mit = pCubeImageInfo->m_mapResultImageInfo.find(iResultItemID);

	if(mit == pCubeImageInfo->m_mapResultImageInfo.end())
	{
		dbg::cerr << L"��� �� ������ �´� ID�� ����.!! �����ID : " << iResultItemID << dbg::endl;
		return false;
	}

	wstrName = mit->second;

	return true; 
}

const wchar_t*	CX2CubeOpenImageManager::GetEventCubeOpenString(IN const int iCubeID)
{
	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		return GET_STRING( STR_ID_809 );
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return GET_STRING( STR_ID_809 );
	}

	return GET_STRING( pCubeImageInfo->m_iStrIndexOpen );
}
const wchar_t*	CX2CubeOpenImageManager::GetEventCubeNoKeyString(IN const int iCubeID)
{
	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		// �⺻ ��Ʈ���� ��½�Ű��.
		return GET_STRING( STR_ID_4834 );
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return GET_STRING( STR_ID_4834 );
	}

	return GET_STRING( pCubeImageInfo->m_iStrIndexNoKey );
}

bool	CX2CubeOpenImageManager::IsShowResult(IN const int iCubeID)
{
	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		return true;
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return true;
	}

	return pCubeImageInfo->m_bShowResult;
}

bool	CX2CubeOpenImageManager::IsShowResultCustom(IN const int iCubeID)
{
	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		return true;
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return false;
	}

	/*
	if( pCubeImageInfo->m_bShowResult == false )
	{
		return false;
	}
	*/

	return pCubeImageInfo->m_bShowResultCustom;
}


bool	CX2CubeOpenImageManager::IsHideBarCube(IN const int iCubeID)
{
	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		return false;
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return false;
	}

	return pCubeImageInfo->m_bHideBar;
}
#ifdef RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
bool	CX2CubeOpenImageManager::IsFadeInOutImage(IN const int iCubeID)
{
	if(iCubeID <= 0)
	{
		dbg::cerr << L"ť�� ID�� ����" << dbg::endl;
		return false;
	}

	CubeOpenImageInfo* pCubeImageInfo = NULL;
	if(GetCubeImageInfo(iCubeID, pCubeImageInfo) == false)
	{
		return false;
	}

	return pCubeImageInfo->m_bImageFadeInOut;
}
#endif // RNW_EVENT_CUBE_IMAGE_FADE_IN_OUT
const bool		CX2CubeOpenImageManager::IsResultItemID(IN const int iItemID)
{
	if(m_setResultItemID.find(iItemID) != m_setResultItemID.end())
		return true;
	else
		return false;
}
//////////////////////////////////////////////////////////////////////////
// private

bool CX2CubeOpenImageManager::GetCubeImageInfo( IN const int iCubeID, OUT CubeOpenImageInfo* &pCubeImageInfo_ )
{
	if(m_mapCubeOpenImageInfo.size() > 0)
	{
		MapCubeOpenImageInfoItor mit = m_mapCubeOpenImageInfo.begin();

		while( mit != m_mapCubeOpenImageInfo.end())
		{
			CubeOpenImageInfo* pCubeImageInfo = mit->second;

			ASSERT(pCubeImageInfo);

			if(pCubeImageInfo != NULL)
			{
				if(pCubeImageInfo->m_setCubeID.find(iCubeID) != pCubeImageInfo->m_setCubeID.end())
				{
					pCubeImageInfo_ = pCubeImageInfo;
					return true;
				}
			}
			++mit;
		}
		dbg::cerr << L"ť�� ID�� �ش��ϴ� �̹��� ������ ����. ť��ID : " << iCubeID << dbg::endl;
		return false;
	}
	else
	{
		dbg::cerr << L"ť�� �̹��� ������ ����ִ�!! �Ľ̿��� ����? ť��ID : " << iCubeID << dbg::endl;
		return false;
	}

}

#endif CUBE_OPEN_IMAGE_MANAGER