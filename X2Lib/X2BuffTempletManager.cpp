/** @file : X2BuffTempletManager.h
    @breif : ���� ���ø��� �����ϴ� �Ŵ��� Ŭ����
*/

#pragma once

#include "StdAfx.h"
#include ".\X2BuffTempletManager.h"

CX2BuffTempletManager* CX2BuffTempletManager::pInstance = NULL;

/** @function : OpenScriptFile
	@brief : �������ø� ��ƽ�ũ��Ʈ �Ľ�
	@param : ������ϸ�
*/
void CX2BuffTempletManager::OpenScriptFileForTemplet()
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pBuffTempletManager", this );



	const wchar_t* wszScriptFileName = L"BattleFieldBuffTemplet.lua";

    if ( g_pKTDXApp->LoadLuaTinker( wszScriptFileName ) == false )
    {
		ASSERT( !"DoMemory doesn't work!" );
		ErrorLogMsg( XEM_ERROR2, wszScriptFileName );
    }
}


void CX2BuffTempletManager::OpenScriptFileForFactor()
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "g_pBuffTempletManager", this );

	const wchar_t* wszScriptFileName = L"BattleFieldBuffFactor.lua";

    if ( g_pKTDXApp->LoadLuaTinker( wszScriptFileName ) == false )
    {
		ASSERT( !"DoMemory doesn't work!" );
		ErrorLogMsg( XEM_ERROR2, wszScriptFileName );
    }
}

/** @function : AddBuffTemplet_LUA
	@brief : �������ø� �ν��Ͻ� ����, ��ũ��Ʈ �Ľ� �� map�� ����
*/
void CX2BuffTempletManager::AddBuffTemplet_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
	
	CX2BuffTempletPtr	ptrBuffTemplet = CX2BuffTemplet::CreateBuffTempletPtr();

	if ( NULL != ptrBuffTemplet )
	{
		if ( ptrBuffTemplet->ParsingScript( luaManager ) )
			m_mapBuffTempletPtr.insert( make_pair( ptrBuffTemplet->GetBuffTempletID(), ptrBuffTemplet ) );
	}
	else
	{
		DISPLAY_ERROR( L"new BuffTemplet didn't work" );
	}
}

/** @function : AddBuffFactor_LUA
	@brief : Ư�� ������ �����Ǵ� ���� �����Ǵ� ���� ���͸� ���ؼ� ����� ���� ����
*/
void CX2BuffTempletManager::AddBuffFactor_LUA()
{
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );

	UINT uiNum = 0;		/// BUFF_FACTOR_ID �� order�� �����
	LUA_GET_VALUE_RETURN( luaManager, "BUFF_FACTOR_ID", uiNum, 0, DISPLAY_ERROR( L"BUFF_FACTOR_ID doesn't exist" ) );
	
	if ( m_vecBuffFactorPtr.size() == uiNum - 1 )
	{
		if ( luaManager.BeginTable( "BUFF_FACTOR" ) )
		{
			CX2BuffFactorPtr ptrBuffFactor = CX2BuffFactor::CreateBuffFactorPtr();
			if ( NULL != ptrBuffFactor )
			{
				if ( ptrBuffFactor->ParsingBuffFactor( luaManager ) )
					m_vecBuffFactorPtr.push_back( ptrBuffFactor );
			}
			else
			{
				DISPLAY_ERROR( L"new BUFF_FACTOR didn't work" );
			}

			luaManager.EndTable();
		}
		else
		{
			DISPLAY_ERROR( L"BUFF_FACTOR table doesn't exist" );
		}
	}
	else
	{
		DISPLAY_ERROR( L"BUFF_FACTOR_ID invalid" );
	}
}

/** @function : GetBuffTempletPtr
	@brief : �������ø����̵�� ��Ī�Ǵ� �������ø��� ����
	@param : �������ø����̵�(eBuffTempletID)
	@return : �������̵�� ��Ī�Ǵ� ���ø�(CX2BuffTempletPtr)
*/
CX2BuffTempletPtr CX2BuffTempletManager::GetBuffTempletPtr( const BUFF_TEMPLET_ID eBuffTempletID_ ) const
{
	if ( !m_mapBuffTempletPtr.empty() )
	{
		map<BUFF_TEMPLET_ID, CX2BuffTempletPtr>::const_iterator mItr = m_mapBuffTempletPtr.find( eBuffTempletID_ );

		if ( m_mapBuffTempletPtr.end() != mItr )
			return mItr->second;
	}

	return CX2BuffTempletPtr();
}

/** @function : GetCombination
	@brief : �ش� ��Ұ� ��� ������ ��ҵ�� ���յǾ� �ִ����� �˾Ƴ��� �Լ�
	@param : �Ľ� ���� ��ũ��Ʈ�� ��ƸŴ���(luaManager_), ���յ��� ������ ����(vecType_)
	@return : �Ľ� ������ true, ���н� false
*/
/*static*/ bool CX2BuffTempletManager::GetCombination( KLuaManager& luaManager_, OUT vector<UINT>& vecType_ )
{
	if ( luaManager_.BeginTable( "COMBINATION" ) )
	{
		int		iIndex = 1;
		UINT	uiType = 0;

		while ( luaManager_.GetValue( iIndex, uiType ) )
		{
			vecType_.push_back( uiType );

			++iIndex;
		}

		luaManager_.EndTable();

		if ( vecType_.empty() )
		{
			return DISPLAY_ERROR( L"COMBINATION elements don't exist" );	/// ���Ͱ� ��������� �ĺ���̼��� �����Ǿ� ���� ���� ���̹Ƿ� �Ľ� ����
		}
		else
			return true;
	}
	else
		return DISPLAY_ERROR( L"COMBINATION doesn't exist" );	/// �ĺ���̼��� ���� �Ǿ� ���� �����Ƿ� �Ľ� ����
}

CX2BuffFactorPtr CX2BuffTempletManager::GetBuffFactorPtr( const UINT uiBuffFactorNumber_ ) const
{
	if ( uiBuffFactorNumber_ != 0 && uiBuffFactorNumber_ <= m_vecBuffFactorPtr.size() )	/// �۴ٷ� �� ���� ���� ������ �ε��� 0���� uiBuffFactorNumber 1�� �����ϱ� ����
	{
		return m_vecBuffFactorPtr[uiBuffFactorNumber_ - 1];
	}
	else
	{
		DISPLAY_ERROR( L"uiBuffFactorNumber_ invaild" );
		return CX2BuffFactorPtr();
	}
}

void CX2BuffTempletManager::GetBuffFactorPtrFromBuffFactorList( IN KLuaManager& luaManager_, OUT vector<CX2BuffFactorPtr>& vecBuffFactorPtr_, OUT vector<UINT>& vecBuffFactorID_)
{
	if ( luaManager_.BeginTable( "BUFF_FACTOR" ) )
	{
		int iIndex = 1;
		UINT uiBuffFactorId = 0;
		while( luaManager_.GetValue( iIndex, uiBuffFactorId ) == true )
		{
			if ( 0 != uiBuffFactorId )
			{
				CX2BuffFactorPtr ptrBuffFactor = GetBuffFactorPtr( uiBuffFactorId );
				if ( NULL != ptrBuffFactor )
				{
					vecBuffFactorPtr_.push_back( ptrBuffFactor );
					vecBuffFactorID_.push_back( uiBuffFactorId );
				}
			}
			else
				DISPLAY_ERROR( L"uiBuffFactorId Invalid" );

			++iIndex;
		}

		luaManager_.EndTable();
	}
}


