/**@file : X2ComboAndSkillChainSystem.cpp
   @breif : �޺� �� ��ų ���� ü�� �ý����� ������ ����
*/

#pragma once

#include "StdAfx.h"
#include ".\X2ComboAndSkillChainSystem.h"

/** @function : 
	@brief : 
	@param : 
	@return : 
*/
bool CX2ComboAndSkillChainSystem::CreateComboAndSkillChain( KLuaManager& luaManager_ )
{
	int iChainIndex = 1;	/// ü���� ����
	while ( luaManager_.BeginTable( iChainIndex ) )
	{
		BOOST_SCOPE_EXIT( (&luaManager_) ) {
			luaManager_.EndTable();
		} BOOST_SCOPE_EXIT_END	/// iChainIndex

			CX2ComboAndSkillChainInfoListPtr ptrComboAndSkillChainInfoList
			= CX2ComboAndSkillChainInfoList::CreateComboAndSkillChainInfoTemplet();
		if ( NULL != ptrComboAndSkillChainInfoList )
		{
			ptrComboAndSkillChainInfoList->ParsingSecretSkillStateId( luaManager_ );
			ptrComboAndSkillChainInfoList->ParsingTriggerStateIdToSecretSkill( luaManager_ );
			ptrComboAndSkillChainInfoList->ParsingTriggerTimeToSecretSkill( luaManager_ );

			int iComboAndSkillIndex = 1;	/// �� ���� ü�� ���� �޺� �� ��ų ����
			while ( luaManager_.BeginTable( iComboAndSkillIndex ) )
			{
				BOOST_SCOPE_EXIT( (&luaManager_) ) {
					luaManager_.EndTable();
				} BOOST_SCOPE_EXIT_END	/// iComboAndSkillIndex

				int iStateIdChained = 0;
				LUA_GET_VALUE_RETURN( luaManager_, 1, iStateIdChained, 0, return DISPLAY_ERROR( L"StateID" )  );

				int  iSkillID = 0;
				LUA_GET_VALUE_RETURN( luaManager_, 2, iSkillID, 0, return DISPLAY_ERROR( L"SkillID" )  );

				COMBO_AND_SKILL_CHAIN_TYPE eComboAndSkillChainType = CASCT_NONE;
				LUA_GET_VALUE_RETURN_ENUM( luaManager_, 3, eComboAndSkillChainType, COMBO_AND_SKILL_CHAIN_TYPE, CASCT_NONE, return DISPLAY_ERROR( L"COMBO_AND_SKILL_CHAIN_TYPE" ) );					

				CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo
					= GetSkillChainInfoPtrByStateID( iStateIdChained );
				
				if ( NULL != ptrComboAndSkillChainInfo )
				{
					ptrComboAndSkillChainInfoList->PushBack( ptrComboAndSkillChainInfo );					
				}
				else
				{
					ptrComboAndSkillChainInfo = CX2ComboAndSkillChainInfo::CreateComboAndSkillChainInfoTemplet( iStateIdChained, iSkillID, eComboAndSkillChainType );

					if ( NULL != ptrComboAndSkillChainInfo )
					{
						ptrComboAndSkillChainInfoList->ConnectNextChainInfo( ptrComboAndSkillChainInfo );
						ptrComboAndSkillChainInfoList->PushBack( ptrComboAndSkillChainInfo );					
						m_vecComboAndSkillChainInfoPtrInSystem.push_back( ptrComboAndSkillChainInfo );					
					}
					else
					{
						return DISPLAY_ERROR( L"CreateComboAndSkillChainInfo" )
					}
				}

				iComboAndSkillIndex++;				
			}	/// while

			/// ���ǰ� �����Ǿ� ������ �ʿ� �ִ´�
			if ( 0 != ptrComboAndSkillChainInfoList->GetTriggerStateIdToSecretSkill() )
				m_vecComboAndSkillChainListPtr.push_back( ptrComboAndSkillChainInfoList );
						
			//m_vecComboAndSkillChainListPtr.push_back( ptrComboAndSkillChainInfoList );
		}
		else
		{
			return DISPLAY_ERROR( L"CreateComboAndSkillChainInfoTemplet" )
		}

		iChainIndex++;
	}

	return true;
}

void CX2ComboAndSkillChainSystem::InitSkillStateChained( CX2GUUser* pCX2GUUser_ )
{
	if ( NULL != pCX2GUUser_ )
	{
		BOOST_FOREACH( CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo, m_vecComboAndSkillChainInfoPtrInSystem )
		{
			if ( NULL != ptrComboAndSkillChainInfo )
				pCX2GUUser_->InitAquiredActiveSkillState( ptrComboAndSkillChainInfo->GetSkillID() );			
		}		
	}
}

CX2ComboAndSkillChainInfoPtr CX2ComboAndSkillChainSystem::GetSkillChainInfoPtrBySkillID( const int iSkillID_ ) const
{
	BOOST_FOREACH( CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo, m_vecComboAndSkillChainInfoPtrInSystem )
	{
		if ( NULL != ptrComboAndSkillChainInfo 
			 && CASCT_SKILL == ptrComboAndSkillChainInfo->GetComboAndSkillChainType() 
			 && iSkillID_ == ptrComboAndSkillChainInfo->GetSkillID() )
			 return ptrComboAndSkillChainInfo;			
	}

	return CX2ComboAndSkillChainInfoPtr();
}

CX2ComboAndSkillChainInfoPtr CX2ComboAndSkillChainSystem::GetSkillChainInfoPtrByStateID( const int iStateID_ ) const
{
	BOOST_FOREACH( CX2ComboAndSkillChainInfoPtr ptrComboAndSkillChainInfo, m_vecComboAndSkillChainInfoPtrInSystem )
	{
		if ( NULL != ptrComboAndSkillChainInfo 
			&& iStateID_ == ptrComboAndSkillChainInfo->GetStateIdChained() )
			return ptrComboAndSkillChainInfo;			
	}

	return CX2ComboAndSkillChainInfoPtr();
}

int CX2ComboAndSkillChainSystem::GetTriggerSecretSkillState( const CX2GUUser* pCX2GUUser_ )
{
	if ( !m_vecComboAndSkillChainListPtr.empty() )
	{
		BOOST_FOREACH( CX2ComboAndSkillChainInfoListPtr ptrComboAndSkillChainInfoList, m_vecComboAndSkillChainListPtr )
		{
			if ( NULL != ptrComboAndSkillChainInfoList )
			{
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // ����ȯ
				int iStateID = pCX2GUUser_->GetFutureStateID();		/// ���� ������Ʈ ���̵�

				/// ���� State ID�� ��ų ���� State ID�� �����Ǿ��� ��, �ش� ��ų�� �´� State ID�� ����
				/// ( ��ų ���� State ID�� �ƴ϶��, �״�� ��ȯ )
				pCX2GUUser_->GetStateIDBySecretSkillTriggerStateID( iStateID );

				if ( iStateID == ptrComboAndSkillChainInfoList->GetTriggerStateIdToSecretSkill()
					&& ptrComboAndSkillChainInfoList->CanTriggerToSecretSkill() )
#else // SERV_ARA_CHANGE_CLASS_SECOND
				if ( pCX2GUUser_->GetFutureStateID() == ptrComboAndSkillChainInfoList->GetTriggerStateIdToSecretSkill() 
					&& ptrComboAndSkillChainInfoList->CanTriggerToSecretSkill() )
#endif // SERV_ARA_CHANGE_CLASS_SECOND
				{
					const float fTriggerTime = ptrComboAndSkillChainInfoList->GetTriggerTimeToSecretSkill();

					if ( IsSamef( fTriggerTime, 100.0f ) && pCX2GUUser_->IsAnimFutureEnd() )
						return ptrComboAndSkillChainInfoList->GetSecretSkillStateId();
					else if ( pCX2GUUser_->GetNowTimeAnimFuture() > fTriggerTime )
						return ptrComboAndSkillChainInfoList->GetSecretSkillStateId();
				}
			}
		}		
	}

	return 0;
}
