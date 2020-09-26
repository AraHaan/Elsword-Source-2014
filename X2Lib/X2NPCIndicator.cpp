#include "StdAfx.h"
#ifdef QUEST_GUIDE
#include "X2NPCIndicator.h"

CX2NPCIndicator::CX2NPCIndicator(void):
m_iIndicatorCount(0),
m_hPortalQuestion(INVALID_PARTICLE_SEQUENCE_HANDLE),
m_fCheckDistance(0.f),
m_pDLGParticle(NULL),
m_bShow(false)
{	
	for( int i = 0; i < MAX_QUEST_INDICATOR_NUM; ++i )
	{
		m_hIndicator[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
		m_hIndicatorMark[i] = INVALID_PARTICLE_SEQUENCE_HANDLE;
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_hParticleIndicator[i] = INVALID_PARTICLE_HANDLE;
		m_hParticleIndicatorMark[i] = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		m_pIndicator[i] = NULL;
		m_pIndicatorMark[i] = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
	}
}

CX2NPCIndicator::~CX2NPCIndicator(void)
{
	Clear();
}

void CX2NPCIndicator::Clear()
{
	if( NULL != g_pData && NULL != g_pData->GetUIMajorParticle() )
	{
		for( int i = 0; i < m_iIndicatorCount; ++i )
		{
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hIndicator[i]);
			g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hIndicatorMark[i]);
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		    m_hParticleIndicator[i] = INVALID_PARTICLE_HANDLE;
		    m_hParticleIndicatorMark[i] = INVALID_PARTICLE_HANDLE;
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		    m_pIndicator[i] = NULL;
		    m_pIndicatorMark[i] = NULL;
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		}
		g_pData->GetUIMajorParticle()->DestroyInstanceHandle(m_hPortalQuestion);
	}

#ifdef PARTICLE_RENDER_BY_DIALOG
	if( NULL != m_pDLGParticle )
	{
		m_pDLGParticle->ClearParticleList();
	}
#endif //PARTICLE_RENDER_BY_DIALOG
	SAFE_DELETE_DIALOG(m_pDLGParticle);

	m_iIndicatorCount = 0;
	m_fCheckDistance = 0.f;
	memset(&m_GuideTargetInfo, 0, sizeof(m_GuideTargetInfo));
}
void CX2NPCIndicator::OnFrameMove()
{
	KTDXPROFILE();

	if( m_iIndicatorCount < 0 )
		return;

#ifdef DIALOG_SHOW_TOGGLE
	bool bHide = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetHideDialog();
	if( true == bHide )
	{
		SetShowAllParticle(false);
		return;
	}
#endif
	if( false == m_bShow )
	{
		return;
	}


	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			if( NULL != g_pTFieldGame->GetMyUnit() )
			{
				bool bMove = false;
				D3DXVECTOR3 vMyUserPos = g_pTFieldGame->GetMyUnit()->GetPos();
				vMyUserPos.y += 200;
				vMyUserPos.z += 320;
				for( int i = 0; i < m_iIndicatorCount; ++i )
				{
					float fNpcDistance = GetDistance( m_GuideTargetInfo[i].vTargetPos, vMyUserPos );

					if( 0 == i )
					{	//ù��° Ÿ�ٿ��� ������ üũ
						if( fabs(m_fCheckDistance-fNpcDistance) > 1.f )
						{
							bMove = true;
						}
						m_fCheckDistance = fNpcDistance;
					}

					SetRotateParticle( i,vMyUserPos, fNpcDistance, bMove);
				}
			}
		}break;
	case CX2Main::XS_BATTLE_FIELD:
		{
			if( NULL != g_pX2Game->GetMyUnit() )
			{
				bool bMove = false;
				D3DXVECTOR3 vMyUserPos = g_pX2Game->GetMyUnit()->GetPos();
				vMyUserPos.y += 200;
				vMyUserPos.z += 320;
				for( int i = 0; i < m_iIndicatorCount; ++i )
				{
					float fNpcDistance = GetDistance( m_GuideTargetInfo[i].vTargetPos, vMyUserPos );

					if( 0 == i )
					{	//ù��° Ÿ�ٿ��� ������ üũ
						if( fabs(m_fCheckDistance-fNpcDistance) > 1.f )
						{
							bMove = true;
						}
						m_fCheckDistance = fNpcDistance;
					}

					SetRotateParticle( i,vMyUserPos, fNpcDistance, bMove);
				}
			}
		}break;
	} 
	return;
}
void CX2NPCIndicator::SetRotateParticle(int iParticleIndex,const D3DXVECTOR3& vMyUserPos, float fDistance, bool bMove)
{
	if( iParticleIndex >= MAX_QUEST_INDICATOR_NUM )
		return;

	if( NULL == g_pData->GetUIMajorParticle() )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeqIndicator = NULL;
	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hIndicator[iParticleIndex] )
	{
		pSeqIndicator = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hIndicator[iParticleIndex] );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeqMark = NULL;
	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hIndicatorMark[iParticleIndex] )
	{
		pSeqMark = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hIndicatorMark[iParticleIndex] );
	}

	CKTDGParticleSystem::CParticleEventSequence* pSeqPortal = NULL;
	if( INVALID_PARTICLE_SEQUENCE_HANDLE != m_hPortalQuestion )
	{
		pSeqPortal = g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hPortalQuestion );
	}

	if( NULL != pSeqIndicator && NULL != pSeqMark )
	{
		float fScale = 1.f;
		if( fDistance <= 1000.f)
		{   //�Ÿ� 1000���� ������ �������� �ʱ�
			if( false == m_GuideTargetInfo[iParticleIndex].bIsNPC )
			{//��ó�� �ִ� ������ ��ġ ����.
				
				D3DXVECTOR2 vPos = g_pKTDXApp->GetProj3DPos(m_GuideTargetInfo[iParticleIndex].vTargetPos);
				D3DXVECTOR3	vecPortalPos(vPos.x, vPos.y, 0.f);
								
				if( NULL != pSeqPortal )
				{
					pSeqPortal->SetParticlePosition(vecPortalPos);
					pSeqPortal->SetShowObject( true );
				}
			}
			pSeqIndicator->SetShowObject(false);
			pSeqMark->SetShowObject(false);
			return;
		}
		else if( fDistance <= 2000.f )
		{   //NPC���� �Ÿ��� 1000~1500 ���� �� �� ������ �ٿ��ֱ�
			fScale = 1.f - ( ( 2000.f - fDistance ) * 0.0003f ); // 1��~0.7��
		}
		pSeqIndicator->SetScaleFactor(D3DXVECTOR3(fScale,fScale,1.f));
		pSeqMark->SetScaleFactor(D3DXVECTOR3(fScale,fScale,1.f));

		if( true == bMove )
		{//�������� ���� ���� ó�� �ϴ� ����

			D3DXVECTOR3 vDirVector = m_GuideTargetInfo[iParticleIndex].vTargetPos - vMyUserPos;
			D3DXVec3Normalize( &vDirVector, &vDirVector );

			//���̵� ��ġ�� ȭ�� �����ڸ��� ����
			D3DXVECTOR3 vGuidePos(0,0,0);
			vGuidePos.x = 698 * (vDirVector.x+1) - 197;
			if ( vGuidePos.x <= 37)			
			{
				vGuidePos.x = 37;
			}
			if( vGuidePos.x >= 987 )
			{
				vGuidePos.x = 987;
			}

			vGuidePos.y = (300 -  (vDirVector.y * 1100)) - vDirVector.z*500;
			if( vGuidePos.y <= 38	)
			{
				vGuidePos.y = 38;
			}
			if(vGuidePos.y >= 678 )
			{
				vGuidePos.y = 678;
			}

			//���̵� ��ƼŬ�� ȸ��
			D3DXVECTOR3 vRot(0.f,0.f, GetDirVecToDegree( vDirVector ));

			pSeqIndicator->SetAddRotate( vRot );
			pSeqIndicator->SetAxisAngle( vRot );

#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if ( CKTDGParticleSystem::CParticle* pIndicator = pSeqIndicator->GetParticle( m_hParticleIndicator[iParticleIndex] ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( NULL !=  m_hIndicator[iParticleIndex] )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{				
				pIndicator->SetRotate( vRot );
				pIndicator->SetAxisRotateDegree( vRot );
				pIndicator->SetPos( vGuidePos );			
			}
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
            if( CKTDGParticleSystem::CParticle* pIndicatorMark = pSeqMark->GetParticle( m_hParticleIndicatorMark[iParticleIndex] ) )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			if( NULL != m_pIndicatorMark[iParticleIndex] )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
			{
				pIndicatorMark->SetPos( vGuidePos );
			}
		}

		pSeqIndicator->SetShowObject(true);
		pSeqMark->SetShowObject(true);	
	}
}
void CX2NPCIndicator::ResetGuideTargetInfo()
{
	Clear();

#ifdef FIELD_BOSS_RAID
	if( true == g_pData->GetBattleFieldManager().GetIsBossRaidCurrentField() )
	{
		return;
	}
#endif // FIELD_BOSS_RAID

	CX2QuestManager* pQuestManager = NULL;
	
	if( NULL != g_pData )
	{
		pQuestManager = g_pData->GetQuestManager();
	}

	if( NULL == pQuestManager )
		return;
	
	int iIndex = 0;							// ����Ʈ���̵� �ε���
	GuideTarget GuideTargetTemp;			// ���̵� Ÿ�� ���� �ӽ� ����ü
	GuideTargetTemp.bIsNPC = false;			// NPC�� �ƴ� Ÿ��(����, ��������Ʈ)	

	map<int, D3DXVECTOR3>	mapVillageInfo;
	GetLinkInfo(mapVillageInfo); //�̵� �� �� �ִ� ����, �ʵ�� ����� ��Ż ��ġ
		

	//����Ʈ �ϷḦ ���� �湮�� �ʿ��� �ʵ� üũ
	if( MAX_QUEST_INDICATOR_NUM > iIndex )
	{
		map<int, bool> SetNeedToVisitFieldList;
		if( true == pQuestManager->GetNeedToVisitFieldList(SetNeedToVisitFieldList) ) //�湮 �ʿ��� village ���
		{
			for( map<int, bool>::iterator it = SetNeedToVisitFieldList.begin();
				it != SetNeedToVisitFieldList.end() && iIndex < MAX_QUEST_INDICATOR_NUM; ++it )
			{	
				map<int, D3DXVECTOR3>::iterator mitVillage = mapVillageInfo.find( it->first );
				if( mitVillage != mapVillageInfo.end() )
				{
					GuideTargetTemp.vTargetPos	= mitVillage->second;	//Ÿ����ġ
					GuideTargetTemp.bIsEpic		= it->second;	//������ ����
					m_GuideTargetInfo[iIndex] = GuideTargetTemp;		
					++iIndex;
				}
			}
		}
	}

	//����Ʈ �ϷḦ ���� �湮�� �ʿ��� ���� üũ
	if( MAX_QUEST_INDICATOR_NUM > iIndex )
	{
		map<int, bool> SetNeedToVisitVillageList;
		if( true == pQuestManager->GetNeedToVisitVillageList(SetNeedToVisitVillageList) ) //�湮 �ʿ��� village ���
		{
			for( map<int, bool>::iterator it = SetNeedToVisitVillageList.begin();
				it != SetNeedToVisitVillageList.end() && iIndex < MAX_QUEST_INDICATOR_NUM; ++it )
			{	
				map<int, D3DXVECTOR3>::iterator mitVillage = mapVillageInfo.find( it->first );
				if( mitVillage != mapVillageInfo.end() )
				{
					GuideTargetTemp.vTargetPos	= mitVillage->second;	//Ÿ����ġ
					GuideTargetTemp.bIsEpic		= it->second;	//������ ����
					m_GuideTargetInfo[iIndex] = GuideTargetTemp;		
					++iIndex;
				}
			}
		}
	}


	if( CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
	{
		//����Ʈ �ϷḦ ���� ��ȭ�� �ʿ��� NPC üũ
		if( MAX_QUEST_INDICATOR_NUM > iIndex )
		{
			GuideTargetTemp.bIsNPC = true;
			map<CX2UnitManager::NPC_UNIT_ID, bool> setNeedToTalkNPCList;
			if( true == pQuestManager->GetNeedToTalkNPCList(setNeedToTalkNPCList) )
			{
				const int iFieldNPCCount = g_pTFieldGame->GetFieldNPCCount() ;
				for( int i=0; i < iFieldNPCCount && iIndex < MAX_QUEST_INDICATOR_NUM ; ++i )
				{	
					CX2TFieldNpc* pNPC = g_pTFieldGame->GetFieldNPC(i);
					if( NULL == pNPC )
						continue;
					map<CX2UnitManager::NPC_UNIT_ID, bool>::iterator mit = setNeedToTalkNPCList.begin();
					for( ; mit != setNeedToTalkNPCList.end() && iIndex < MAX_QUEST_INDICATOR_NUM ; ++mit)
					{
						if( pNPC->GetNpcId() == mit->first )
						{	
							GuideTargetTemp.vTargetPos	= pNPC->m_vPos;	//Ÿ����ġ
							GuideTargetTemp.bIsEpic		= mit->second;	//������ ����
							m_GuideTargetInfo[iIndex] = GuideTargetTemp;				
							++iIndex;
						}
					}
				}	
			}
		}
	}

	m_iIndicatorCount = iIndex;
	CreateIndicatorParticle();

	
	if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
	{
		m_bShow = g_pData->GetUIManager()->GetUIQuestNew()->m_bUserShowQuickQuestDLG;
		SetShowAllParticle(m_bShow);
	}
}

void CX2NPCIndicator::CreateIndicatorParticle()
{
	if( NULL == g_pData->GetUIMajorParticle() )
		return;

	if( m_iIndicatorCount < 0 )
		return;

	if( m_iIndicatorCount > MAX_QUEST_INDICATOR_NUM )
		m_iIndicatorCount = MAX_QUEST_INDICATOR_NUM;

	for( int i = 0; i < m_iIndicatorCount; ++i )
	{
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hIndicator[i] )
		{	//ȭ��ǥ ��ƼŬ ����
			if( true == m_GuideTargetInfo[i].bIsEpic )
			{	//��������Ʈ�� �Ϲ� ����Ʈ�� ���� ����(����)
				m_hIndicator[i] = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"NPCTargetArrowEpic", 70, 450, 0);	
			}
			else
			{	//��������Ʈ�� �Ϲ� ����Ʈ�� ���� ����(���)
				m_hIndicator[i] = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"NPCTargetArrowNormal", 70, 450, 0);
			}

			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence(m_hIndicator[i]);
			if( NULL != pSeq )
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                if( INVALID_PARTICLE_HANDLE == m_hParticleIndicator[i] )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				if( NULL == m_pIndicator[i] )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    m_hParticleIndicator[i] = pSeq->CreateNewParticleHandle( D3DXVECTOR3(70.f,408.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					m_pIndicator[i] = pSeq->CreateNewParticle( D3DXVECTOR3(70.f,408.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				}

				if( NULL == m_pDLGParticle )
				{//��ƼŬ�� ���̾ ���� �ϱ� ���� ���̾�α� ����
					m_pDLGParticle = new CKTDGUIDialog( g_pMain->GetNowState(), NULL);
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGParticle );
					m_pDLGParticle->SetLayer(XDL_NORMAL_1);
					m_pDLGParticle->SetFront(true);
					m_pDLGParticle->SetShow(true);
				}
#ifdef PARTICLE_RENDER_BY_DIALOG
				if( NULL != m_pDLGParticle )
				{
					m_pDLGParticle->AddParticle(static_cast<CKTDGObject*>(pSeq));
				}
#endif //PARTICLE_RENDER_BY_DIALOG
			}
			
			if( false == m_GuideTargetInfo[i].bIsNPC 
				&& CX2Main::XS_VILLAGE_MAP == g_pMain->GetNowStateID() )
			{	//Ÿ���� NPC�� �ƴ� ��� ? ��ƼŬ �߰�
				if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hPortalQuestion )
				{
					m_hPortalQuestion = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"NpcCompleteQuest1", -200.0f, -200.0f, 0.0f);
					CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence(m_hPortalQuestion);
					if( NULL != pSeq )
					{
						pSeq->CreateNewParticle( D3DXVECTOR3(0,0,0) );
					}
				}
			}
		}
		
		if( INVALID_PARTICLE_SEQUENCE_HANDLE == m_hIndicatorMark[i] )
		{	//����Ʈ ��ũ ��ƼŬ ����
			m_hIndicatorMark[i] = g_pData->GetUIMajorParticle()->CreateSequenceHandle( NULL,  L"NPCTargetQuestion", 70, 450, 0);	
			CKTDGParticleSystem::CParticleEventSequence* pSeq = g_pData->GetUIMajorParticle()->GetInstanceSequence(m_hIndicatorMark[i]);
			if( NULL != pSeq )
			{
				pSeq->SetShowObject(false);
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                if ( INVALID_PARTICLE_HANDLE == m_hParticleIndicatorMark[i] )
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				if( NULL == m_pIndicatorMark[i] )
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    m_hParticleIndicatorMark[i] = pSeq->CreateNewParticleHandle( D3DXVECTOR3(70.f,408.0f,0.0f) );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					m_pIndicatorMark[i] = pSeq->CreateNewParticle( D3DXVECTOR3(70.f,408.0f,0.0f) );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				}

				if( NULL == m_pDLGParticle )
				{//��ƼŬ�� ���̾ ���� �ϱ� ���� ���̾�α� ����
					m_pDLGParticle = new CKTDGUIDialog( g_pMain->GetNowState(), NULL);
					g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGParticle );
					m_pDLGParticle->SetLayer(XDL_NORMAL_1);
					m_pDLGParticle->SetFront(true);
					m_pDLGParticle->SetShow(true);
				}
#ifdef PARTICLE_RENDER_BY_DIALOG
				if( NULL != m_pDLGParticle )
				{
					m_pDLGParticle->AddParticle(static_cast<CKTDGObject*>(pSeq));
				}
#endif //PARTICLE_RENDER_BY_DIALOG
			}
		}
	}
}

float CX2NPCIndicator::GetDirVecToDegree( D3DXVECTOR3 dirVec )
{
	D3DXVECTOR3 vZero(1, 0, 0) ; 
	float fRadian = acosf(D3DXVec3Dot(&dirVec,&vZero));
	float fDegree = D3DXToDegree(fRadian);

	return ( dirVec.y < 0 ) ? fDegree : -fDegree;
}

void CX2NPCIndicator::SetShowAllParticle( bool bVal )
{
	if( NULL == g_pData->GetUIMajorParticle() )
		return;

	CKTDGParticleSystem::CParticleEventSequence* pSeq;

	for( int i = 0; i < m_iIndicatorCount; ++i )
	{
		pSeq =  g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hIndicator[i] );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bVal );
		}
		pSeq =  g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hIndicatorMark[i] );
		if( NULL != pSeq )
		{
			pSeq->SetShowObject( bVal );
		}		
	}

	pSeq =  g_pData->GetUIMajorParticle()->GetInstanceSequence( m_hPortalQuestion );
	if( NULL != pSeq )
	{
		pSeq->SetShowObject( bVal );
	}
}

void CX2NPCIndicator::SetShow(bool bVal)
{
	if( bVal == m_bShow )
	{
		return;
	}

	if( true == bVal )
	{
		if( NULL != g_pData && NULL != g_pData->GetUIManager() && NULL != g_pData->GetUIManager()->GetUIQuestNew() )
		{//����Ʈ����̰� ���������� ������ false�� ����
			if( false == g_pData->GetUIManager()->GetUIQuestNew()->m_bUserShowQuickQuestDLG )
			{
				bVal = false;
			}
		}
	}
	SetShowAllParticle(bVal); 
	m_bShow = bVal; 
}

//�� �Լ��� ���;� �ϴ� ���� ���� ��ġ���� �̵� �� �� �ִ� ����, �ʵ� ID�� ����� ��Ż ��ġ.
bool CX2NPCIndicator::GetLinkInfo(map<int, D3DXVECTOR3>& mapVillageInfo_)
{
	mapVillageInfo_.clear();
	//�ϴ� �������� �ʵ�� ����� �� Ȯ������..

	switch ( g_pMain->GetNowStateID() )
	{
	case CX2Main::XS_VILLAGE_MAP:
		{
			vector<int> m_vecLinkedPos;
			g_pData->GetLocationManager()->GetVecLinkedPos(m_vecLinkedPos);	//���� ��ġ���� �̵� �� �� �ִ� ���� StartPosId	
			for( vector<int>::iterator it = m_vecLinkedPos.begin(); it != m_vecLinkedPos.end(); ++it )
			{
				SEnum::VILLAGE_MAP_ID eFieldID = g_pData->GetLocationManager()->GetMapId( *it );
				D3DXVECTOR3 vPos = g_pData->GetLocationManager()->GetStartPosLoc(*it);
				vPos.y += 320.f;

				mapVillageInfo_.insert( std::make_pair(eFieldID, vPos) );
			}
		} break;
	case CX2Main::XS_BATTLE_FIELD:
		{
			map<UINT, UINT> mapBattleFieldMovingInfo; //PLACE_ID_TO_MOVE, LINE_NUMBER
			g_pData->GetBattleFieldManager().GetBattleFieldPortalMovingInfo(mapBattleFieldMovingInfo);
			
			for( map<UINT, UINT>::iterator mit = mapBattleFieldMovingInfo.begin(); 
				mit != mapBattleFieldMovingInfo.end(); ++mit )
			{
				const CKTDGLineMap::LineData* pLineData = g_pX2Game->GetWorld()->GetLineMap()->GetLineData( mit->second );
				if ( NULL != pLineData )
				{
					D3DXVECTOR3 vPos = pLineData->startPos;
					vPos.y += 320.f;
					mapVillageInfo_.insert( std::make_pair(mit->first, vPos) );
				}
			}
		} break;
	}

	return true;
}
#endif //QUEST_GUIDE


