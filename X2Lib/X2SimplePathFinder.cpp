#include "StdAfx.h"
#include ".\x2simplepathfinder.h"


CX2SimplePathFinder::CX2SimplePathFinder(void)
{
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
	m_pPrevLineGroup.Init();
	m_pCurrLineGroup.Init();
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	m_pPrevLineGroup = NULL;
	m_pCurrLineGroup = NULL;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	m_MoveState = MS_WAIT;

#ifdef AI_FLY
	m_bFlyTurn = false;		
#endif

	m_iDestLineDataIndex = -1;
}


CX2SimplePathFinder::~CX2SimplePathFinder(void)
{
}


//////////////////////////////////////////////////////////////////////////
// CSimplePathFinder::FollowTargetPath() -- �ڵ�� ������ �Ȱ����� ������ �뷫 �̷����� rule
// if( �������� ) 
//   ���������� ������ �ӵ��� �����δ�
// else
//   if( ������������ ����� line���� �ִ� ) �޷�~
//   else
//     if( ������������ ���� && ���� ���� �������� ) ���� ���� ����
//     else if( ������������ �Ʒ��� && ���� �Ʒ��� �������� ) ���� �Ʒ��� ����
//     else      
//       if( ���� ���� ���� )
//         if( �밢�� ���� ���� ) �밢�� ����
//         else ���� �ݴ�������� �ȱ�
//       else ��� �ȱ�
//
// todo: randomness �߰�
//       ��ã�� �� ã���� ��ġ��.
//       
//////////////////////////////////////////////////////////////////////////

void CX2SimplePathFinder::FollowTargetPath( /*float fElapsedTime,*/ const D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
											const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex, 
											const float fMaxJumpUp, const float fMaxJumpRight,
											IN CKTDGLineMap* pLineMap, const float fDestArriveGap, const float fDestLeaveGap, const bool bFootOnLine,
											const bool bStayOnCurrLineGroup, OUT bool& bTargetOnRight,
											const float fLineEndDetectRange 
#ifdef ADD_NPC_CONDITION_TABLE
											, IN bool bIfCannotFindMoveStateDoWait /*= false*/
#endif // ADD_NPC_CONDITION_TABLE
											)
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 
	// ���� �̹� const�� �Ǿ� �ִµ� �� �� const�� �ٽ� �޳�?�Ѥ�;
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 

	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
	
#ifdef UNIT_ROAD
	if( pFinalDestLineData != NULL )
	{
		switch ( pFinalDestLineData->lineType )
		{
		case CKTDGLineMap::LT_UNIT_ROAD:
		case CKTDGLineMap::LT_JUMP_UP_ABS:
		case CKTDGLineMap::LT_JUMP_UP_REL:
			m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
			return;
			break;

		default:
			break;
		}							
	}
#endif

	switch( m_MoveState )
	{
	case MS_JUMP:
	case MS_DOWN:
	case MS_JUMP_LEFT:
	case MS_JUMP_RIGHT:
	case MS_DOWN_LEFT:
	case MS_DOWN_RIGHT:
		{
			if( true == bFootOnLine )
				m_MoveState = MS_WAIT;
		} break;

	case MS_WAIT:
	case MS_LEFT:
	case MS_RIGHT:
	case MS_FORCE_LEFT:
	case MS_FORCE_RIGHT:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );

			// pCurrLineData�� NULL�ϰ�� ó����....????
			if(pCurrLineData == NULL)
				return;

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			float fFinalDestDistSq = D3DXVec3LengthSq( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			if( pFinalDestLineGroup == m_pCurrLineGroup ) // ���� line group
			{						
				if( fFinalDestDistSq < fDestArriveGap * fDestArriveGap ) // �������� ����
				{
					m_MoveState = MS_WAIT;
					return;
				}
				else
				{
					if( MS_WAIT == m_MoveState && fFinalDestDistSq < fDestLeaveGap * fDestLeaveGap )
					{
						return;
					}

					if( fIsRight > 0.f )
					{
						m_MoveState = MS_RIGHT;
					}
					else 
					{
						m_MoveState = MS_LEFT;
					}
					return;
				}		
			} 
			else // ���� line group�� �ƴϸ�
			{
				if( false == bStayOnCurrLineGroup )
				{
					if( fIsUp > 0.f ) // �������� ���� �ִ� ���
					{
						D3DXVECTOR3 vJumpPos = vCurrPos + vUpVec * fMaxJumpUp;
						m_iDestLineDataIndex = iCurrLineDataIndex;
						vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

						// FieldFix: �̰� ���ϴ� ���̷�...
						// ������ ������ �����ؼ� �� �ö���� ��츦 ���� ���� ���� �ڵ�
						const BOOL MAGIC_USE_COMPENSATION = true;
						if( true == MAGIC_USE_COMPENSATION )
						{
							const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( m_iDestLineDataIndex );

							const float MAGIC_LINE_RADIUS_EXT = LINE_RADIUS * 1.5f;
							if( pLineData->beforeLine < 0 )
							{
								if( GetDistance3Sq( pLineData->startPos, vJumpPos ) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
								{
									m_iDestLineDataIndex = -1;
								}
							}

							if( pLineData->nextLine < 0 )
							{
								if( GetDistance3Sq( pLineData->endPos, vJumpPos ) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
								{
									m_iDestLineDataIndex = -1;
								}
							}
						}

						if( -1 != m_iDestLineDataIndex && m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
							const CKTDGLineMap::LineData* pDestLineData	= pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
							if( pDestLineData != NULL )
#endif
							{
								switch ( pDestLineData->lineType )
								{
								case CKTDGLineMap::LT_UNIT_ROAD:
								case CKTDGLineMap::LT_JUMP_UP_ABS:
								case CKTDGLineMap::LT_JUMP_UP_REL:
									break;

								default:
									{
										if( false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( pDestLineGroup != m_pCurrLineGroup &&
											(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
										{
											// ���� ���� jump
											m_MoveState = MS_JUMP;
											m_vDestPos = vJumpPos;
											return;
										}
									} break;
								}							
							}

						}
					}
					else // �������� �Ʒ��� �ִ� ���
					{
						D3DXVECTOR3 vJumpPos = vCurrPos - vUpVec * LINE_RADIUS * 1.5f;
						m_iDestLineDataIndex = iCurrLineDataIndex;
						vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
							if( pDestLineData != NULL )
#endif
							{   
								switch ( pDestLineData->lineType )
								{
								case CKTDGLineMap::LT_UNIT_ROAD:
								case CKTDGLineMap::LT_JUMP_UP_ABS:
								case CKTDGLineMap::LT_JUMP_UP_REL:
									break;

								default:
									{
										if( false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState )
										{
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										    if( pDestLineGroup != m_pCurrLineGroup &&
											    (pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
										    {
											    // ������ ���� ���̰� ������ �ظ��ϸ� �������� �ʴ´�
											    //if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState || vJumpPos.y >= vFinalDestPos.y )

												    // ���� �Ʒ��� down
												m_MoveState = MS_DOWN;
												m_vDestPos = vJumpPos;
												return;
										    }
                                        }
									} break;
								}										
							}

						}
					}
				}

				// ���� �� �Ʒ��� jump �ϰų� down �� �� ������ �׳� �̵�
				if( MS_FORCE_LEFT != m_MoveState && (MS_FORCE_RIGHT == m_MoveState || fIsRight > 0.1f) ) // �������� ������ 
				{
					if( pCurrLineData->nextLine < 0 || pLineMap->GetLineData( pCurrLineData->nextLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineEnd = pCurrLineData->endPos - vCurrPos;
						const float fLineEndDist = D3DXVec3LengthSq( &vLineEnd );
						if( fLineEndDist < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // ���������� �� �� ������
						{	
							if( false == bStayOnCurrLineGroup )
							{
								if( vFinalDestPos.y < vCurrPos.y )
								{									
									// �׳� �ɾ�� ������ �� �ִ� ������ �ִ��� Ȯ��
									m_iDestLineDataIndex = iCurrLineDataIndex;
									D3DXVECTOR3 vDownPos( pCurrLineData->endPos + vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS );
									vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
									{
										const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

										if( 
                                            pDestLineData == NULL || 
                                            false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
										{													
											m_MoveState = MS_FORCE_RIGHT;
											return;
										}	
									}
								}

								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vJumpPos;
								for( int i=0; i<3; i++ )
								{
									vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
									vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
										break;
								}								

								if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( 
                                        pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										// �밢�� ������ jump
										m_MoveState = MS_JUMP_RIGHT;
										m_vDestPos = vJumpPos;
										return;
									}
								}
							}

							// ���������� �� ���� ���� jump�� �� �� ������ �������� �̵�
							{
#ifdef ADD_NPC_CONDITION_TABLE
								if( true == bIfCannotFindMoveStateDoWait )
								{
									m_MoveState = MS_WAIT;
									return;
								}
								else
#endif // ADD_NPC_CONDITION_TABLE
								{
									switch( m_MoveState )
									{									
									case MS_LEFT:
									case MS_FORCE_LEFT:
										{
											m_MoveState = MS_FORCE_RIGHT;
										} break;

									case MS_WAIT:
									case MS_RIGHT:
									case MS_FORCE_RIGHT:
										{
											m_MoveState = MS_FORCE_LEFT;
										} break;
									}
								}
							}
						}							
					}

					// can go right
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_RIGHT;
					}
					return;
				}
				else if( MS_FORCE_RIGHT != m_MoveState && (MS_FORCE_LEFT == m_MoveState || fIsRight < -0.1f) ) // �������� ����
				{
					if( pCurrLineData->beforeLine < 0 || pLineMap->GetLineData( pCurrLineData->beforeLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineStart = pCurrLineData->startPos - vCurrPos;
						float fLineStartDist = D3DXVec3Length( &vLineStart );
						if( fLineStartDist < MAGIC_COLLISION_BOUND ) // �������� �� �� ������
						{
							if( false == bStayOnCurrLineGroup )
							{
								if( vFinalDestPos.y < vCurrPos.y )
								{	
									// �׳� �ɾ�� ������ �� �ִ� ������ �ִ��� Ȯ��
									m_iDestLineDataIndex = iCurrLineDataIndex;
									D3DXVECTOR3 vDownPos;
									vDownPos = pCurrLineData->startPos - vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
									vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
									{
										const CKTDGLineMap::LineData*   pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

										if( 
                                            pDestLineData == NULL ||
                                            false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
										{
											m_MoveState = MS_FORCE_LEFT;
											return;
										}										
									}
								}

								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vJumpPos;
								for( int i=0; i<3; i++ )
								{
									vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);									
									vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

									if( m_iDestLineDataIndex != iCurrLineDataIndex )
										break;
								}

								if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( 
                                        pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										// �밢�� ���� jump
										m_MoveState = MS_JUMP_LEFT;
										m_vDestPos = vJumpPos;
										return;
									}
								}
							}

							// �������� �� ���� ���� jump�� �� �� ������ ���������� �̵�
							{
#ifdef ADD_NPC_CONDITION_TABLE
								if( true == bIfCannotFindMoveStateDoWait )
								{
									m_MoveState = MS_WAIT;
									return;
								}
								else
#endif // ADD_NPC_CONDITION_TABLE
								{
									switch( m_MoveState )
									{
									case MS_WAIT:
									case MS_LEFT:
									case MS_FORCE_LEFT:
										m_MoveState = MS_FORCE_RIGHT;
										break;

									case MS_RIGHT:
									case MS_FORCE_RIGHT:
										m_MoveState = MS_FORCE_LEFT;
										break;
									}
								}
							}
						}
					}

					// can go left
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_LEFT;
					}
					return;
				}
				else // �������� ���ʵ� �����ʵ� �ƴ� ���
				{
					// force move
					if( MS_LEFT == m_MoveState )
					{
						m_MoveState = MS_FORCE_LEFT;
					}
					else if( MS_RIGHT == m_MoveState )
					{
						m_MoveState = MS_FORCE_RIGHT;
					}
					else if( MS_WAIT == m_MoveState )
					{
						m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
					}
					return;
				}				
			} 
		}break;
		default:
			{
				if( true == bFootOnLine )
					m_MoveState = MS_WAIT;
			}
		break;
	} // end of switch( m_MoveState )
} // end of OnFrameMove()


void CX2SimplePathFinder::EscapeTargetPath( /*float fElapsedTime,*/ const D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
											const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex, 
											const float fMaxJumpUp, const float fMaxJumpRight,
											IN CKTDGLineMap* pLineMap, const float fDestArriveGap, /*const float fDestLeaveGap,*/ const bool bFootOnLine,
											/*bool bStayOnCurrLineGroup,*/ OUT bool& bTargetOnRight,
											const bool bIsRight,
											const float fLineEndDetectRange )
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 

	const CKTDGLineMap::LineData*			pFinalDestLineData	= pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID	    pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup*	pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	D3DXVECTOR3 vFinalDestDir	= vFinalDestPos - vCurrPos;
	float fFinalDestDist		= D3DXVec3Length( &vFinalDestDir );

	switch( m_MoveState )
	{
	case MS_JUMP:
	case MS_DOWN:
	case MS_JUMP_LEFT:
	case MS_JUMP_RIGHT:
	case MS_DOWN_LEFT:
	case MS_DOWN_RIGHT:
		{
			if( true == bFootOnLine )
				m_MoveState = MS_WAIT;
		} break;

	case MS_WAIT:
	case MS_LEFT:
	case MS_RIGHT:
	case MS_FORCE_LEFT:
	case MS_FORCE_RIGHT:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            if ( pCurrLineData == NULL )
                m_pCurrLineGroup.Init();
            else
                m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			if( pFinalDestLineGroup == m_pCurrLineGroup ) // ���� line group
			{
				if( bTargetOnRight == bIsRight && fFinalDestDist < fDestArriveGap ) // Ÿ�������� �̵��ϸ鼭 Ÿ�� ������ ������
				{
					// �����ؼ� �Ѿ��
					if( true == bIsRight )
					{
						m_MoveState = MS_JUMP_RIGHT;
					}
					else
					{
						m_MoveState = MS_JUMP_LEFT;
					}
					return;					
				}		
			}

			// ���� line group�� �ƴϰų�, ���� line group�̶� ������ ���� ������ 
			if( fIsRight > 0.f ) // Ÿ���� �����ʿ� �ְ�
			{
				if( pCurrLineData->beforeLine < 0 || pLineMap->GetLineData(pCurrLineData->beforeLine)->bEnable == false )
				{
					D3DXVECTOR3 vLineStart	= pCurrLineData->startPos - vCurrPos;
					float fLineStartDist	= D3DXVec3Length( &vLineStart );
					if( fLineStartDist < MAGIC_COLLISION_BOUND ) // �������� �� �� ������
					{
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

							//�밢�� ���� jump
							m_MoveState = MS_JUMP_LEFT;
							m_vDestPos = vJumpPos;
							return;
						}
						else // jump �ؼ� ���� ���� ������ �ݴ� �������� ���ư���
						{
							m_MoveState = MS_FORCE_RIGHT;
							return;
						}
					}
				}

				// �������� �� �� ������ 
				m_MoveState = MS_LEFT;
				return;
			}
			else // Ÿ���� ���ʿ� �ְ� 
			{
				if( pCurrLineData->nextLine < 0 || pLineMap->GetLineData(pCurrLineData->nextLine)->bEnable == false ) 
				{
					D3DXVECTOR3 vLineEnd	= pCurrLineData->endPos - vCurrPos;
					float fLineEndDist		= D3DXVec3Length( &vLineEnd );
					if( fLineEndDist < MAGIC_COLLISION_BOUND ) // ���������� �� �� ������
					{	
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}								

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

							// �밢�� ������ jump
							m_MoveState = MS_JUMP_RIGHT;
							m_vDestPos = vJumpPos;
							return;
						}
						else // jump �ؼ� ���� ���� ������ �ݴ� �������� ���ư���
						{
							m_MoveState = MS_FORCE_LEFT;
							return;
						}
					}
				}

				// ���������� �� �� ������ 
				m_MoveState = MS_RIGHT;
				return;
			}		
		} break;
#ifdef AI_FLY
	default:
		m_MoveState = MS_WAIT;
		break;
#endif
	} // end of switch( m_MoveState )
} // end of OnFrameMove()


#ifdef AI_FLY
// *FlyPos : ������ ������ ��ġ
// *TargetPos : ������ ��ġ
// *NpcPos : ���� ��Ʋ ��ġ//
// *Fly �̵��� ���̴� �ڵ����� �����ǵ��� �Ѵ�. (gunpc::PhysicProcess ���� �����ϵ����Ѵ�.)
//
//	1. ���������ȿ� �����ߴٸ� 
//		1. ���� ������ �Ѵٸ� Fly Up ��Ų��.
//		2. ���� ����� �Ѵٸ� Fly Down ��Ų��.
//		3. ���� ������ �ʿ䰡 ���ٸ� Wait
//	2. ���̻� ���� ���� �����̶��(���θ��� ��) �������� �ٲ۴�.
//	3. TargetPos�� Fly ��Ų��. (������������)

void CX2SimplePathFinder::FollowTargetPathFly( /*float fElapsedTime,*/ const D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
										   const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex,
										   const float fMaxJumpUp, const float fMaxJumpRight,
										   const float fDestArriveHeight, const float fDestArriveArea,
										   IN CKTDGLineMap* pLineMap, const float fDestArriveGap, /*const float fDestLeaveGap,*/ /*bool bFootOnLine,*/
										   /*bool bStayOnCurrLineGroup,*/ bool& bTargetOnRight,
										   const float fLineEndDetectRange )
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 

	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID   pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
	if( pFinalDestLineData != NULL )
	{
		switch ( pFinalDestLineData->lineType )
		{
		case CKTDGLineMap::LT_UNIT_ROAD:
		case CKTDGLineMap::LT_JUMP_UP_ABS:
		case CKTDGLineMap::LT_JUMP_UP_REL:
			m_MoveState = rand()%2 ? MS_FLY_LEFT_FRONT : MS_FLY_RIGHT_FRONT;
			return;
			break;

		default:
			break;
		}							
	}
#endif

	D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;	
	float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );

	D3DXVECTOR3 vFinalDestLandDir = vFinalDestDir;
	vFinalDestLandDir.y = 0.f;
	float fFinalDestLandDist = D3DXVec3Length( &vFinalDestLandDir );

	switch( m_MoveState )
	{
	case MS_WAIT:
	case MS_FLY_UP:
	case MS_FLY_DOWN:
	case MS_FLY_LEFT_FRONT:
	case MS_FLY_LEFT_FRONT_UP:
	case MS_FLY_LEFT_FRONT_DOWN:
	case MS_FLY_LEFT_BACK:
	case MS_FLY_LEFT_BACK_UP:
	case MS_FLY_LEFT_BACK_DOWN:
	case MS_FLY_RIGHT_FRONT:
	case MS_FLY_RIGHT_FRONT_UP:
	case MS_FLY_RIGHT_FRONT_DOWN:
	case MS_FLY_RIGHT_BACK:
	case MS_FLY_RIGHT_BACK_UP:
	case MS_FLY_RIGHT_BACK_DOWN:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );

			// pCurrLineData�� NULL�ϰ�� ó����....????
			if(pCurrLineData == NULL)
			{				
				m_MoveState = MS_WAIT;
				m_bFlyTurn = false;
				return;
			}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			int iLandLineIndex = -1;
			D3DXVECTOR3 vLandPos = pLineMap->GetLandPosition(vCurrPos, 2000.f, &iLandLineIndex);

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			//if( m_MoveState == MS_WAIT && fFinalDestDist < fDestLeaveGap )
			//{
			//	return;
			//}

			bool bFlyFront = true;
			if( fFinalDestLandDist < fDestArriveGap - fDestArriveArea )
			{
				bFlyFront = false;
			}
			if( fFinalDestLandDist > fDestArriveGap + fDestArriveArea )
			{					
				bFlyFront = true;
			}

			bool bFlyUp = false;
			bool bFlyDown = false;
			if( vCurrPos.y < vFinalDestPos.y + fDestArriveHeight - 10.f )
			{
				// �������� ���� �ִ� ���
				bFlyUp = true;
				bFlyDown = false;
			}
			else if( vCurrPos.y > vFinalDestPos.y + fDestArriveHeight + 10.f )
			{
				// �������� �Ʒ��� �ִ� ���
				bFlyUp = false;
				bFlyDown = true;
			}

			// �������� ������������ �����ߴ��� �˻��Ѵ�.
			if( fFinalDestLandDist >= fDestArriveGap - fDestArriveArea && fFinalDestLandDist <= fDestArriveGap + fDestArriveArea ) // �������� ����
			{
				// �������¿��� ���� ������ ������ �����Ѵ�.
				if(bFlyUp == true)
				{
					m_MoveState = MS_FLY_UP;
					m_bFlyTurn = false;		
					return;
				}
				else if(bFlyDown == true)
				{
					m_MoveState = MS_FLY_DOWN;
					m_bFlyTurn = false;		
					return;
				}

				// �� ������ �ʿ䰡 ���ٸ� ���¸� Wait�� �ٲ۴�.
				m_MoveState = MS_WAIT;
				m_bFlyTurn = false;				
				return;
			}
			else
			{
				// ���� �������� �������� �������Ƿ� �̵���Ų��.
				// ���� ���θ��� ���̶� ���̻� �̵��Ұ����� �������� �˻��Ѵ�.
				// �̵��Ұ��ϴٸ� ������ �ٲ㼭 �̵��Ѵ�.
				
				// Fly Front�� �̵����� Back���� �̵����� ����
				
				
				if(m_bFlyTurn == true)
				{
					if( ((m_MoveState == MS_FLY_LEFT_FRONT || m_MoveState == MS_FLY_LEFT_FRONT_UP || m_MoveState == MS_FLY_LEFT_FRONT_DOWN) &&
						 fIsRight > 0.3f) ||
						((m_MoveState == MS_FLY_RIGHT_FRONT || m_MoveState == MS_FLY_RIGHT_FRONT_UP || m_MoveState == MS_FLY_RIGHT_FRONT_DOWN) && 
						 fIsRight < -0.3f) )
					{
						m_bFlyTurn = false;
						m_MoveState = MS_WAIT;						
					}					
					return;
				}

				if( m_MoveState == MS_FLY_RIGHT_FRONT || m_MoveState == MS_FLY_LEFT_FRONT ||
					m_MoveState == MS_FLY_RIGHT_BACK || m_MoveState == MS_FLY_LEFT_BACK ||
					m_MoveState == MS_FLY_RIGHT_BACK_UP || m_MoveState == MS_FLY_LEFT_BACK_UP ||
					m_MoveState == MS_FLY_RIGHT_BACK_DOWN || m_MoveState == MS_FLY_LEFT_BACK_DOWN ) //|| m_MoveState == MS_WAIT)
				{
					//const float fMaxJumpUp = 282.f;
					//const float fMaxJumpRight = 94.f;

					if( fIsRight < 0.f )
					{
						// �������� ���ʿ� �ְ�, ��������� ������ �ڷ� �̵����̴�.
						bool bLeftLand, bLeftUpLand, bLeftDownLand;
						bLeftLand = true;
						bLeftUpLand = bLeftDownLand = false;

						// ���� ����
						if(pCurrLineData->nextLine < 0 || pLineMap->GetLineData( pCurrLineData->nextLine)->bEnable == false)
                           bLeftLand = false;

						// �Ʒ��� �������� �ִ� ����
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vDownPos;
						vDownPos = pCurrLineData->endPos + vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
						vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex )
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bLeftLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {													
										    bLeftDownLand = true;
									    }	
								    } break;
							    }		
                            }
						}

						// ���� �ö󰥼� �ִ� ����
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}								

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bLeftLand = false;
										    return;
									    }
    #ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
    #endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    // �밢�� ������ jump
										    bLeftUpLand = true;
									    }
								    } break;
							    }
                            }
						}


						if( bLeftLand == false && bLeftUpLand == false && bLeftDownLand == false ) //
						{
							m_MoveState = MS_FLY_LEFT_FRONT;
							m_bFlyTurn = true;							
							return;
						}
					}
					else
					{
						// �������� �����ʿ� �ְ�, ��������� ���� �ڷ� �̵����̴�.
						bool bRightLand, bRightUpLand, bRightDownLand;
						bRightLand = true;
						bRightUpLand = bRightDownLand = false;

						// ���� ����
						if(pCurrLineData->beforeLine < 0 || pLineMap->GetLineData( pCurrLineData->beforeLine)->bEnable == false)
							bRightLand = false;

						// �Ʒ��� �������� �ִ� ����
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vDownPos;
						vDownPos = pCurrLineData->startPos - vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
						vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex )
						{
							const CKTDGLineMap::LineData*   pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
							
                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bRightLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    bRightDownLand = true;
									    }										
								    } break;
							    }
                            }
						}

						// ���� �ö󰥼� �ִ� ����
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);									
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );							
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bRightLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;	
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    // �밢�� ���� jump
										    bRightUpLand = true;
									    }
								    } break;
							    }
                            }
						}

						if( bRightLand == false && bRightUpLand == false && bRightDownLand == false ) //
						{
							m_MoveState = MS_FLY_RIGHT_FRONT;
							m_bFlyTurn = true;							
							return;
						}
					}
				}


				// �̵������ϴٸ� ��~ �̵���Ų��.
				if( fIsRight > 0.f )
				{
					if(bFlyFront == true)
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_RIGHT_FRONT_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_RIGHT_FRONT_DOWN;
						else
							m_MoveState = MS_FLY_RIGHT_FRONT;
					}
					else
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_RIGHT_BACK_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_RIGHT_BACK_DOWN;
						else
							m_MoveState = MS_FLY_RIGHT_BACK;
					}
				}
				else
				{
					if(bFlyFront == true)
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_LEFT_FRONT_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_LEFT_FRONT_DOWN;
						else
							m_MoveState = MS_FLY_LEFT_FRONT;
					}
					else
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_LEFT_BACK_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_LEFT_BACK_DOWN;
						else
							m_MoveState = MS_FLY_LEFT_BACK;
					}
				}				
				return;
			}
		}
		break;
	default:
		m_MoveState = MS_WAIT;
		break;

	} // end of switch( m_MoveState )
	
} // end of OnFrameMove()
#endif


#ifdef SERV_PET_SYSTEM
void CX2SimplePathFinder::SimpleFollowTargetPath( float fElapsedTime, D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
										   const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex, 
										   const float fMaxJumpUp, const float fMaxJumpRight,
										   CKTDGLineMap* pLineMap, const float fDestArriveGap, const float fDestLeaveGap, bool bFootOnLine,
										   bool bStayOnCurrLineGroup, bool& bTargetOnRight,
										   const float fLineEndDetectRange )
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 

	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
	float fFinalDestDist = D3DXVec3LengthSq( &vFinalDestDir );


	switch( m_MoveState )
	{
	case MS_JUMP:
	case MS_DOWN:
	case MS_JUMP_LEFT:
	case MS_JUMP_RIGHT:
	case MS_DOWN_LEFT:
	case MS_DOWN_RIGHT:
		{
			if( true == bFootOnLine )
				m_MoveState = MS_WAIT;
		} break;

	case MS_WAIT:
	case MS_LEFT:
	case MS_RIGHT:
	case MS_FORCE_LEFT:
	case MS_FORCE_RIGHT:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );

			// pCurrLineData�� NULL�ϰ�� ó����....????
			if(pCurrLineData == NULL)
				return;

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			float fFinalDestDist = D3DXVec3LengthSq( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			if( pFinalDestLineGroup == m_pCurrLineGroup ) // ���� line group
			{						
				if( MS_WAIT == m_MoveState && fFinalDestDist < pow( fDestLeaveGap, 2 ) )
				{
					return;
				}

				if( fFinalDestDist < pow( fDestArriveGap, 2 ) ) // �������� ����
				{
					m_MoveState = MS_WAIT;
					return;
				}
				else
				{
					if( fIsRight > 0.f )
					{
						m_MoveState = MS_RIGHT;
					}
					else 
					{
						m_MoveState = MS_LEFT;
					}
					return;
				}		
			} 
			else // ���� line group�� �ƴϸ�
			{
				if( false == bStayOnCurrLineGroup )
				{
					if( fIsUp > 0.f ) // �������� ���� �ִ� ���
					{
						D3DXVECTOR3 vJumpPos = vCurrPos + vUpVec * fMaxJumpUp;
						m_iDestLineDataIndex = iCurrLineDataIndex;
						vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

						// ������ ������ �����ؼ� �� �ö���� ��츦 ���� ���� ���� �ڵ�
						const BOOL MAGIC_USE_COMPENSATION = true;
						if( true == MAGIC_USE_COMPENSATION )
						{
							const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( m_iDestLineDataIndex );

							const float MAGIC_LINE_RADIUS_EXT = LINE_RADIUS * 1.5f;
							if( pLineData->beforeLine < 0 )
							{
								D3DXVECTOR3 vDist = vJumpPos - pLineData->startPos;
								if( D3DXVec3LengthSq(&vDist) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
								{
									m_iDestLineDataIndex = -1;
								}
							}

							if( pLineData->nextLine < 0 )
							{
								D3DXVECTOR3 vDist = vJumpPos - pLineData->endPos;
								if( D3DXVec3LengthSq(&vDist) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
								{
									m_iDestLineDataIndex = -1;
								}
							}
						}

						if( -1 != m_iDestLineDataIndex && m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
							const CKTDGLineMap::LineData* pDestLineData	= pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
							if( pDestLineData != NULL )
#endif
							{
								switch ( pDestLineData->lineType )
								{
								case CKTDGLineMap::LT_UNIT_ROAD:
								case CKTDGLineMap::LT_JUMP_UP_ABS:
								case CKTDGLineMap::LT_JUMP_UP_REL:
									break;

								default:
									{
										if( false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( pDestLineGroup != m_pCurrLineGroup &&
											(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
										{
											// ���� ���� jump
											m_MoveState = MS_JUMP;
											m_vDestPos = vJumpPos;
											return;
										}
									} break;
								}
							}

						}
					}
					else // �������� �Ʒ��� �ִ� ���
					{
						D3DXVECTOR3 vJumpPos = vCurrPos - vUpVec * LINE_RADIUS * 1.5f;
						m_iDestLineDataIndex = iCurrLineDataIndex;
						vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
							if( pDestLineData != NULL )
#endif
							{   
								switch ( pDestLineData->lineType )
								{
								case CKTDGLineMap::LT_UNIT_ROAD:
								case CKTDGLineMap::LT_JUMP_UP_ABS:
								case CKTDGLineMap::LT_JUMP_UP_REL:
									break;

								default:
									{
										if( false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( pDestLineGroup != m_pCurrLineGroup &&
											(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
										{
											// ������ ���� ���̰� ������ �ظ��ϸ� �������� �ʴ´�
											//if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState || vJumpPos.y >= vFinalDestPos.y )
											if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState )
											{
												// ���� �Ʒ��� down
												m_MoveState = MS_DOWN;
												m_vDestPos = vJumpPos;
												return;
											}
										}
									} break;
								}
							}

						}
					}
				}

				// ���� �� �Ʒ��� jump �ϰų� down �� �� ������ �׳� �̵�
				if( MS_FORCE_LEFT != m_MoveState && (MS_FORCE_RIGHT == m_MoveState || fIsRight > 0.1f) ) // �������� ������ 
				{
					if( pCurrLineData->nextLine < 0 || pLineMap->GetLineData( pCurrLineData->nextLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineEnd = pCurrLineData->endPos - vCurrPos;
						float fLineEndDist = D3DXVec3LengthSq( &vLineEnd );
						if( fLineEndDist < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // ���������� �� �� ������
						{	
							if( false == bStayOnCurrLineGroup )
							{
								if( vFinalDestPos.y < vCurrPos.y )
								{									
									// �׳� �ɾ�� ������ �� �ִ� ������ �ִ��� Ȯ��
									m_iDestLineDataIndex = iCurrLineDataIndex;
									D3DXVECTOR3 vDownPos;
									vDownPos = pCurrLineData->endPos + vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
									vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
									{
										const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

										if( 
                                            pDestLineData == NULL ||
                                            false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
										{													
											m_MoveState = MS_FORCE_RIGHT;
											return;
										}										
									}
								}

								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vJumpPos;
								for( int i=0; i<3; i++ )
								{
									vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
									vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
										break;
								}								

								if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( 
                                            pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										// �밢�� ������ jump
										m_MoveState = MS_JUMP_RIGHT;
										m_vDestPos = vJumpPos;
										return;
									}
								}
							}

							// ���������� �� ���� ���� jump�� �� �� ������ �������� �̵�
							{
								switch( m_MoveState )
								{									
								case MS_LEFT:
								case MS_FORCE_LEFT:
									{
										m_MoveState = MS_FORCE_RIGHT;
									} break;

								case MS_WAIT:
								case MS_RIGHT:
								case MS_FORCE_RIGHT:
									{
										m_MoveState = MS_FORCE_LEFT;
									} break;
								}
							}
						}							
					}

					// can go right
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_RIGHT;
					}
					return;
				}
				else if( MS_FORCE_RIGHT != m_MoveState && (MS_FORCE_LEFT == m_MoveState || fIsRight < -0.1f) ) // �������� ����
				{
					if( pCurrLineData->beforeLine < 0 || pLineMap->GetLineData( pCurrLineData->beforeLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineStart = pCurrLineData->startPos - vCurrPos;
						float fLineStartDist = D3DXVec3LengthSq( &vLineStart );
						if( fLineStartDist < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // �������� �� �� ������
						{
							if( false == bStayOnCurrLineGroup )
							{
								if( vFinalDestPos.y < vCurrPos.y )
								{	
									// �׳� �ɾ�� ������ �� �ִ� ������ �ִ��� Ȯ��
									m_iDestLineDataIndex = iCurrLineDataIndex;
									D3DXVECTOR3 vDownPos;
									vDownPos = pCurrLineData->startPos - vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
									vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
									{
										const CKTDGLineMap::LineData*   pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( 
                                            pDestLineData == NULL ||
                                            false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
										{
											m_MoveState = MS_FORCE_LEFT;
											return;
										}										
									}
								}

								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vJumpPos;
								for( int i=0; i<3; i++ )
								{
									vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);									
									vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

									if( m_iDestLineDataIndex != iCurrLineDataIndex )
										break;
								}

								if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( 
                                        pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										// �밢�� ���� jump
										m_MoveState = MS_JUMP_LEFT;
										m_vDestPos = vJumpPos;
										return;
									}
								}
							}

							// �������� �� ���� ���� jump�� �� �� ������ ���������� �̵�
							{
								switch( m_MoveState )
								{
								case MS_WAIT:
								case MS_LEFT:
								case MS_FORCE_LEFT:
									m_MoveState = MS_FORCE_RIGHT;
									break;

								case MS_RIGHT:
								case MS_FORCE_RIGHT:
									m_MoveState = MS_FORCE_LEFT;
									break;
								}
							}
						}
					}

					// can go left
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_LEFT;
					}
					return;
				}
				else // �������� ���ʵ� �����ʵ� �ƴ� ���
				{
					// force move
					if( MS_LEFT == m_MoveState )
					{
						m_MoveState = MS_FORCE_LEFT;
					}
					else if( MS_RIGHT == m_MoveState )
					{
						m_MoveState = MS_FORCE_RIGHT;
					}
					else if( MS_WAIT == m_MoveState )
					{
						m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
					}
					return;
				}				
			} 
		}break;

	} // end of switch( m_MoveState )
} // end of OnFrameMove()

#ifdef AI_FLY
void CX2SimplePathFinder::SimpleFollowTargetPathFly( float fElapsedTime, D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
											  const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex,
											  const float fMaxJumpUp, const float fMaxJumpRight,
											  const float fDestArriveHeight, const float fDestArriveArea,
											  CKTDGLineMap* pLineMap, const float fDestArriveGap, const float fDestLeaveGap, bool bFootOnLine,
											  bool bStayOnCurrLineGroup, bool& bTargetOnRight,
											  const float fLineEndDetectRange )
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 

	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID   pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;	
	
	D3DXVECTOR3 vFinalDestLandDir = vFinalDestDir;
	vFinalDestLandDir.y = 0.f;
	float fFinalDestLandDist = D3DXVec3Length( &vFinalDestLandDir );

	switch( m_MoveState )
	{
	case MS_WAIT:
	case MS_FLY_UP:
	case MS_FLY_DOWN:
	case MS_FLY_LEFT_FRONT:
	case MS_FLY_LEFT_FRONT_UP:
	case MS_FLY_LEFT_FRONT_DOWN:
	case MS_FLY_LEFT_BACK:
	case MS_FLY_LEFT_BACK_UP:
	case MS_FLY_LEFT_BACK_DOWN:
	case MS_FLY_RIGHT_FRONT:
	case MS_FLY_RIGHT_FRONT_UP:
	case MS_FLY_RIGHT_FRONT_DOWN:
	case MS_FLY_RIGHT_BACK:
	case MS_FLY_RIGHT_BACK_UP:
	case MS_FLY_RIGHT_BACK_DOWN:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );

			// pCurrLineData�� NULL�ϰ�� ó����....????
			if(pCurrLineData == NULL)
			{				
				m_MoveState = MS_WAIT;
				m_bFlyTurn = false;
				return;
			}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( iCurrLineDataIndex );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			int iLandLineIndex = -1;
			D3DXVECTOR3 vLandPos = pLineMap->GetLandPosition(vCurrPos, 2000.f, &iLandLineIndex);

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;			
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			
			bool bFlyFront = true;
			if( fFinalDestLandDist < fDestArriveGap - fDestArriveArea )
			{
				bFlyFront = false;
			}
			if( fFinalDestLandDist > fDestArriveGap + fDestArriveArea )
			{					
				bFlyFront = true;
			}

			bool bFlyUp = false;
			bool bFlyDown = false;
			if( vCurrPos.y < vFinalDestPos.y + fDestArriveHeight - 10.f )
			{
				// �������� ���� �ִ� ���
				bFlyUp = true;
				bFlyDown = false;
			}
			else if( vCurrPos.y > vFinalDestPos.y + fDestArriveHeight + 10.f )
			{
				// �������� �Ʒ��� �ִ� ���
				bFlyUp = false;
				bFlyDown = true;
			}

			// �������� ������������ �����ߴ��� �˻��Ѵ�.
			if( fFinalDestLandDist >= fDestArriveGap - fDestArriveArea && fFinalDestLandDist <= fDestArriveGap + fDestArriveArea ) // �������� ����
			{
				// �������¿��� ���� ������ ������ �����Ѵ�.
				if(bFlyUp == true)
				{
					m_MoveState = MS_FLY_UP;
					m_bFlyTurn = false;		
					return;
				}
				else if(bFlyDown == true)
				{
					m_MoveState = MS_FLY_DOWN;
					m_bFlyTurn = false;		
					return;
				}

				// �� ������ �ʿ䰡 ���ٸ� ���¸� Wait�� �ٲ۴�.
				m_MoveState = MS_WAIT;
				m_bFlyTurn = false;				
				return;
			}
			else
			{
				// ���� �������� �������� �������Ƿ� �̵���Ų��.
				// ���� ���θ��� ���̶� ���̻� �̵��Ұ����� �������� �˻��Ѵ�.
				// �̵��Ұ��ϴٸ� ������ �ٲ㼭 �̵��Ѵ�.

				// Fly Front�� �̵����� Back���� �̵����� ����


				if(m_bFlyTurn == true)
				{
					if( ((m_MoveState == MS_FLY_LEFT_FRONT || m_MoveState == MS_FLY_LEFT_FRONT_UP || m_MoveState == MS_FLY_LEFT_FRONT_DOWN) &&
						fIsRight > 0.3f) ||
						((m_MoveState == MS_FLY_RIGHT_FRONT || m_MoveState == MS_FLY_RIGHT_FRONT_UP || m_MoveState == MS_FLY_RIGHT_FRONT_DOWN) && 
						fIsRight < -0.3f) )
					{
						m_bFlyTurn = false;
						m_MoveState = MS_WAIT;						
					}					
					return;
				}

				if( m_MoveState == MS_FLY_RIGHT_FRONT || m_MoveState == MS_FLY_LEFT_FRONT ||
					m_MoveState == MS_FLY_RIGHT_BACK || m_MoveState == MS_FLY_LEFT_BACK ||
					m_MoveState == MS_FLY_RIGHT_BACK_UP || m_MoveState == MS_FLY_LEFT_BACK_UP ||
					m_MoveState == MS_FLY_RIGHT_BACK_DOWN || m_MoveState == MS_FLY_LEFT_BACK_DOWN ) //|| m_MoveState == MS_WAIT)
				{
					//const float fMaxJumpUp = 282.f;
					//const float fMaxJumpRight = 94.f;

					if( fIsRight < 0.f )
					{
						// �������� ���ʿ� �ְ�, ��������� ������ �ڷ� �̵����̴�.
						bool bLeftLand, bLeftUpLand, bLeftDownLand;
						bLeftLand = true;
						bLeftUpLand = bLeftDownLand = false;

						// ���� ����
						if(pCurrLineData->nextLine < 0 || pLineMap->GetLineData( pCurrLineData->nextLine)->bEnable == false)
							bLeftLand = false;

						// �Ʒ��� �������� �ִ� ����
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vDownPos;
						vDownPos = pCurrLineData->endPos + vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
						vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex )
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bLeftLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
							            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {													
										    bLeftDownLand = true;
									    }	
								    } break;
							    }	
                            }
						}

						// ���� �ö󰥼� �ִ� ����
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}								

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bLeftLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
							            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    // �밢�� ������ jump
										    bLeftUpLand = true;
									    }
								    } break;
							    }
                            }
						}


						if( bLeftLand == false && bLeftUpLand == false && bLeftDownLand == false ) //
						{
							m_MoveState = MS_FLY_LEFT_FRONT;
							m_bFlyTurn = true;							
							return;
						}
					}
					else
					{
						// �������� �����ʿ� �ְ�, ��������� ���� �ڷ� �̵����̴�.
						bool bRightLand, bRightUpLand, bRightDownLand;
						bRightLand = true;
						bRightUpLand = bRightDownLand = false;

						// ���� ����
						if(pCurrLineData->beforeLine < 0 || pLineMap->GetLineData( pCurrLineData->beforeLine)->bEnable == false)
							bRightLand = false;

						// �Ʒ��� �������� �ִ� ����
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vDownPos;
						vDownPos = pCurrLineData->startPos - vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
						vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex )
						{
							const CKTDGLineMap::LineData*   pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bRightLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
							            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    bRightDownLand = true;
									    }										
								    } break;
							    }
                            }
						}

						// ���� �ö󰥼� �ִ� ����
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);									
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP					

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bRightLand = false;
										    return;
									    }

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
							            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    // �밢�� ���� jump
										    bRightUpLand = true;
									    }
								    } break;
							    }
                            }
						}

						if( bRightLand == false && bRightUpLand == false && bRightDownLand == false ) //
						{
							m_MoveState = MS_FLY_RIGHT_FRONT;
							m_bFlyTurn = true;							
							return;
						}
					}
				}


				// �̵������ϴٸ� ��~ �̵���Ų��.
				if( fIsRight > 0.f )
				{
					if(bFlyFront == true)
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_RIGHT_FRONT_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_RIGHT_FRONT_DOWN;
						else
							m_MoveState = MS_FLY_RIGHT_FRONT;
					}
					else
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_RIGHT_BACK_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_RIGHT_BACK_DOWN;
						else
							m_MoveState = MS_FLY_RIGHT_BACK;
					}
				}
				else
				{
					if(bFlyFront == true)
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_LEFT_FRONT_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_LEFT_FRONT_DOWN;
						else
							m_MoveState = MS_FLY_LEFT_FRONT;
					}
					else
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_LEFT_BACK_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_LEFT_BACK_DOWN;
						else
							m_MoveState = MS_FLY_LEFT_BACK;
					}
				}				
				return;
			}
		}
		break;
	default:
		m_MoveState = MS_WAIT;
		break;

	} // end of switch( m_MoveState )

} // end of OnFrameMove()
#endif
#endif // PET


void CX2SimplePathFinder::FollowTargetPathInBattleField( const CX2BattleFieldNpcAi* pBattleFieldNpcAi_, IN const CKTDGLineMap* pLineMap_, OUT bool& bTargetOnRight_ )
{
	KTDXPROFILE();

    int iFinalDestLineIndex = pBattleFieldNpcAi_->GetFinalDestLineIndex();
	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap_->GetLineData( iFinalDestLineIndex );
	
#ifdef UNIT_ROAD
	if( pFinalDestLineData != NULL )
	{
		switch ( pFinalDestLineData->lineType )
		{
		case CKTDGLineMap::LT_UNIT_ROAD:
		case CKTDGLineMap::LT_JUMP_UP_ABS:
		case CKTDGLineMap::LT_JUMP_UP_REL:
			m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
			return;
			break;

		default:
			break;
		}
	}
#endif

	switch( m_MoveState )
	{
	case MS_JUMP:
	case MS_DOWN:
	case MS_JUMP_LEFT:
	case MS_JUMP_RIGHT:
	case MS_DOWN_LEFT:
	case MS_DOWN_RIGHT:
		{
			if( true == pBattleFieldNpcAi_->GetFootOnLine() )
				m_MoveState = MS_WAIT;
		} break;

	case MS_WAIT:
	case MS_LEFT:
	case MS_RIGHT:
	case MS_FORCE_LEFT:
	case MS_FORCE_RIGHT:
		{
			const int iCurrLineDataIndex = pBattleFieldNpcAi_->GetLastTouchLineIndex();
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap_->GetLineData( iCurrLineDataIndex );

			if(pCurrLineData == NULL)
				return;
			
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap_->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			const D3DXVECTOR3& vFinalDestPos = pBattleFieldNpcAi_->GetFinalDest();
			const D3DXVECTOR3& vCurrPos = pBattleFieldNpcAi_->GetNowPosition();

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			const float fFinalDestDistSq = D3DXVec3LengthSq( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );

			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight_ = true;
			else
				bTargetOnRight_ = false;

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            CKTDGLineMap::LineGroupID pFinalDestLineGroup;
            if ( pFinalDestLineData != NULL )
                pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID; 
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			if( pFinalDestLineGroup == m_pCurrLineGroup ) // ���� line group
			{					
				const float fDestArriveGap = static_cast<float>( pBattleFieldNpcAi_->GetAIData()->chaseMoveData.destGap );

				if( fFinalDestDistSq < fDestArriveGap * fDestArriveGap ) // �������� ����
				{
					m_MoveState = MS_WAIT;
					return;
				}
				else
				{
					const float fDestLeaveGap = static_cast<float>( pBattleFieldNpcAi_->GetAIData()->chaseMoveData.moveGap );

					if( MS_WAIT == m_MoveState && fFinalDestDistSq < fDestLeaveGap * fDestLeaveGap )
					{
						return;
					}

					if( fIsRight > 0.f )
					{
						m_MoveState = MS_RIGHT;
					}
					else 
					{
						m_MoveState = MS_LEFT;
					}
					return;
				}		
			} 
			else // ���� line group�� �ƴϸ�
			{
				const D3DXVECTOR3 vUpVec(0,1,0);
				const float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );

				if( fIsUp > 0.f ) // �������� ���� �ִ� ���
				{
					m_iDestLineDataIndex = iCurrLineDataIndex;
					
					D3DXVECTOR3 vJumpPos = vCurrPos + vUpVec * pBattleFieldNpcAi_->GetMaxJumpUp();
					vJumpPos = pLineMap_->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

					const CKTDGLineMap::LineData* pLineData = pLineMap_->GetLineData( m_iDestLineDataIndex );

					const float MAGIC_LINE_RADIUS_EXT = LINE_RADIUS * 1.5f;
					if( pLineData->beforeLine < 0 )
					{
						if( GetDistance3Sq( pLineData->startPos, vJumpPos ) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
						{
							m_iDestLineDataIndex = -1;
						}
					}

					if( pLineData->nextLine < 0 )
					{
						if( GetDistance3Sq( pLineData->endPos, vJumpPos ) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
						{
							m_iDestLineDataIndex = -1;
						}
					}

					if( -1 != m_iDestLineDataIndex && m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
					{
						const CKTDGLineMap::LineData* pDestLineData	= pLineMap_->GetLineData( m_iDestLineDataIndex );
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
						CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
						if( pDestLineData != NULL )
#endif
						{
							switch ( pDestLineData->lineType )
							{
							case CKTDGLineMap::LT_UNIT_ROAD:
							case CKTDGLineMap::LT_JUMP_UP_ABS:
							case CKTDGLineMap::LT_JUMP_UP_REL:
								break;

							default:
								{
									if( false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( pDestLineGroup != m_pCurrLineGroup &&
										(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
									{
										// ���� ���� jump
										m_MoveState = MS_JUMP;
										m_vDestPos = vJumpPos;
										return;
									}
								} break;
							}
						}

					}
				}
				else // �������� �Ʒ��� �ִ� ���
				{
					m_iDestLineDataIndex = iCurrLineDataIndex;
					D3DXVECTOR3 vJumpPos = vCurrPos;
					vJumpPos -= vUpVec * LINE_RADIUS * 1.5f;
					vJumpPos = pLineMap_->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

					if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
					{
						const CKTDGLineMap::LineData* pDestLineData = pLineMap_->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
						CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
						if( pDestLineData != NULL )
#endif
						{   
							switch ( pDestLineData->lineType )
							{
							case CKTDGLineMap::LT_UNIT_ROAD:
							case CKTDGLineMap::LT_JUMP_UP_ABS:
							case CKTDGLineMap::LT_JUMP_UP_REL:
								break;

							default:
								{
									if( false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( pDestLineGroup != m_pCurrLineGroup &&
										(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
									{
										// ������ ���� ���̰� ������ �ظ��ϸ� �������� �ʴ´�
										//if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState || vJumpPos.y >= vFinalDestPos.y )
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState )
										{
											// ���� �Ʒ��� down
											m_MoveState = MS_DOWN;
											m_vDestPos = vJumpPos;
											return;
										}
									}
								} break;
							}
						}

					}
				}

				const float& MAGIC_COLLISION_BOUND = pBattleFieldNpcAi_->GetAIData()->chaseMoveData.m_fLineEndDetectRange;	// ���� ���� �����ߴ��� üũ�� �� ���Ǵ� ���� 

				// ���� �� �Ʒ��� jump �ϰų� down �� �� ������ �׳� �̵�
				if( MS_FORCE_LEFT != m_MoveState && (MS_FORCE_RIGHT == m_MoveState || fIsRight > 0.1f) ) // �������� ������ 
				{
					// ���� ���θʿ��� ���������� ���� �� ���� ���µ�
					if( pCurrLineData->nextLine < 0 || pLineMap_->GetLineData( pCurrLineData->nextLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineEnd = pCurrLineData->endPos - vCurrPos;
						const float fLineEndDist = D3DXVec3LengthSq( &vLineEnd );
						// ���� ���� ���� ������
						if( fLineEndDist < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // ���������� �� �� ������
						{	
							// �������� ���� ��ġ ���� �Ʒ���
							if( vFinalDestPos.y < vCurrPos.y )
							{									
								// �׳� �ɾ�� ������ �� �ִ� ������ �ִ��� Ȯ��
								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vDownPos = pCurrLineData->endPos;
								vDownPos += vRightVec * LINE_RADIUS;
								vDownPos -= vUpVec * LINE_RADIUS;
								vDownPos = pLineMap_->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );

								// �ɾ� �������� �Ǹ�
								if( m_iDestLineDataIndex != iCurrLineDataIndex )
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap_->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{													
										m_MoveState = MS_FORCE_RIGHT;
										return;
									}										
								}
							}

							m_iDestLineDataIndex = iCurrLineDataIndex;
							D3DXVECTOR3 vJumpPos;
							for( int i = 0; i < 3; i++ )
							{
								vJumpPos = vCurrPos;
								vJumpPos += vUpVec * pBattleFieldNpcAi_->GetMaxJumpUp();
								vJumpPos += vRightVec * pBattleFieldNpcAi_->GetMaxJumpRight() / pow(2.f, (float)i);
								vJumpPos = pLineMap_->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
								
								if( m_iDestLineDataIndex != iCurrLineDataIndex )
									break;
							}								

							if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
							{
								const CKTDGLineMap::LineData* pDestLineData = pLineMap_->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
								CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

								if( pDestLineData == NULL ||
                                    false == pDestLineData->m_bMonsterAllowed )
								{
									m_MoveState = MS_WAIT;
									return;
								}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
								if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
									pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
								{
									// �밢�� ������ jump
									m_MoveState = MS_JUMP_RIGHT;
									m_vDestPos = vJumpPos;
									return;
								}
							}

							// ���������� �� ���� ���� jump�� �� �� ������ �������� �̵�
							{
								switch( m_MoveState )
								{									
								case MS_LEFT:
								case MS_FORCE_LEFT:
									{
										m_MoveState = MS_FORCE_RIGHT;
									} break;

								case MS_WAIT:
								case MS_RIGHT:
								case MS_FORCE_RIGHT:
									{
										m_MoveState = MS_FORCE_LEFT;
									} break;
								}
							}
						}							
					}

					// can go right
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_RIGHT;
					}
					else
					{
						m_MoveState = MS_LEFT;
					}
					return;
				}
				else if( MS_FORCE_RIGHT != m_MoveState && (MS_FORCE_LEFT == m_MoveState || fIsRight < -0.1f) ) // �������� ����
				{
					if( pCurrLineData->beforeLine < 0 || pLineMap_->GetLineData( pCurrLineData->beforeLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineStart = pCurrLineData->startPos - vCurrPos;
						float fLineStartDist3Sq = D3DXVec3LengthSq( &vLineStart );
						if( fLineStartDist3Sq < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // �������� �� �� ������
						{
							if( vFinalDestPos.y < vCurrPos.y )
							{	
								// �׳� �ɾ�� ������ �� �ִ� ������ �ִ��� Ȯ��
								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vDownPos = pCurrLineData->startPos;
								vDownPos -= vRightVec * LINE_RADIUS;
								vDownPos -= vUpVec * LINE_RADIUS;
								vDownPos = pLineMap_->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );

								if( m_iDestLineDataIndex != iCurrLineDataIndex )
								{
									const CKTDGLineMap::LineData*   pDestLineData = pLineMap_->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
								    CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

								    if( pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										m_MoveState = MS_FORCE_LEFT;
										return;
									}										
								}
							}

							m_iDestLineDataIndex = iCurrLineDataIndex;

							D3DXVECTOR3 vJumpPos;
							for( int i = 0; i < 3; i++ )
							{
								vJumpPos = vCurrPos;
								vJumpPos += vUpVec * pBattleFieldNpcAi_->GetMaxJumpUp();
								vJumpPos -= vRightVec * pBattleFieldNpcAi_->GetMaxJumpRight() / pow(2.f, (float)i);									
								vJumpPos = pLineMap_->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

								if( m_iDestLineDataIndex != iCurrLineDataIndex )
									break;
							}

							if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump�ؼ� ���� ���� ������
							{
								const CKTDGLineMap::LineData* pDestLineData = pLineMap_->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
								CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

								if( pDestLineData == NULL ||
                                    false == pDestLineData->m_bMonsterAllowed )
								{
									m_MoveState = MS_WAIT;
									return;
								}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
								if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
									pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
								{
									// �밢�� ���� jump
									m_MoveState = MS_JUMP_LEFT;
									m_vDestPos = vJumpPos;
									return;
								}
							}

							// �������� �� ���� ���� jump�� �� �� ������ ���������� �̵�
							{
								switch( m_MoveState )
								{
								case MS_WAIT:
								case MS_LEFT:
								case MS_FORCE_LEFT:
									m_MoveState = MS_FORCE_RIGHT;
									break;

								case MS_RIGHT:
								case MS_FORCE_RIGHT:
									m_MoveState = MS_FORCE_LEFT;
									break;
								}
							}
						}
					}

					// can go left
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_LEFT;
					}
					else
					{
						m_MoveState = MS_RIGHT;
					}
					return;
				}
				else // �������� ���ʵ� �����ʵ� �ƴ� ���
				{
					// force move
					if( MS_LEFT == m_MoveState )
					{
						m_MoveState = MS_FORCE_LEFT;
					}
					else if( MS_RIGHT == m_MoveState )
					{
						m_MoveState = MS_FORCE_RIGHT;
					}
					else if( MS_WAIT == m_MoveState )
					{
						m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
					}
					return;
				}				
			} 
		}break;

	} // end of switch( m_MoveState )
}