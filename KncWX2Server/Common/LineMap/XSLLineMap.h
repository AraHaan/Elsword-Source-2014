#pragma once

#define D3DFVF_LINE_MAP_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

inline float GetDistance( const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2 )
{
	D3DXVECTOR3 temp = pos2 - pos1;
	return D3DXVec3Length( &temp );
}

class CXSLLineMap
{
	public:
		struct LINE_MAP_VERTEX
		{
			D3DXVECTOR3	pos;
			DWORD		color;
		};

		enum LINE_TYPE
		{
			LT_NORMAL = 0,
			LT_WALL,
			LT_POTAL,
			LT_HOLE,
			LT_JUMP_UP_REL,
			LT_JUMP_UP_ABS,
			LT_BUNGEE,
#ifdef MONSTER_ROAD
            LT_MONSTER_ROAD,
#endif
#ifdef UNIT_ROAD
            LT_UNIT_ROAD,
#endif
#ifdef OTHER_ROAD
			LT_OTHER_ROAD,
#endif
			LT_CLIFF,
		};

		enum LINE_DUST_TYPE
		{
			LDT_DEFALT_DUST,
			LDT_WATER_SPLASH,
		};


		enum LINE_ANIM_TYPE
		{
			LAT_NONE,
			LAT_BACK_AND_FORTH,
			LAT_VIKING,
			LAT_CIRCLE,
			LAT_MOVE_IF_STEPPED_ON,
			//LAT_ROTATE_IF_STEPPED_ON,
			//LAT_SEESAW_IF_STEPPED_ON,
		};


		enum LINE_CAMERA_TYPE 
		{
			LCT_INVALID = 0,
			LCT_ABSOULTE_POSITION,
			LCT_RELATIE_POSITION,
		};

#ifdef WORLD_TRIGGER
		enum TRIGGER_MESH_ACTION_TYPE
		{
			TAT_MESH_NONE = 0,
			TAT_MESH_CHANGE_ANIM,
			TAT_MESH_CHANGE_SPEED,
		};
		enum TRIGGER_LINE_ACTION_TYPE
		{
			TAT_LINE_NONE = 0,
			TAT_LINE_CHANGE_ENABLE,			
			TAT_LINE_CHANGE_SPEED,
			TAT_LINE_CHANGE_JUMPSPEED,
			TAT_LINE_CHANGE_CAMERA,
			TAT_LINE_CHANGE_CANPASS,
			TAT_LINE_CHANGE_TELEPORT,

		};
		enum TRIGGER_ETC_ACTION_TYPE
		{
			TAT_ETC_NONE = 0,
			TAT_ETC_PLAY_SOUND,
			TAT_ETC_DURATION_TIME,
			TAT_ETC_CAMERA,
			TAT_ETC_CAMERA_SHAKE,
			TAT_ETC_ACTIVE_LAYER,
		};

		struct TRIGGER_DATA_MESH
		{
			TRIGGER_MESH_ACTION_TYPE	m_iMeshTriggerAction;			
			wstring			m_wstrMeshName;
			float			m_fTriggerTime;
			wstring			m_wstrName;
			int				m_iPlayType;			
			float			m_fAnimSpeed;
			bool			m_bDone;

			TRIGGER_DATA_MESH()
			{
				m_iMeshTriggerAction	= TAT_MESH_NONE;
				m_fTriggerTime			= 0.f;
				m_wstrName				= L"";
				m_iPlayType				= 1;				
				m_wstrMeshName			= L"";
				m_fAnimSpeed			= 1.f;
				m_bDone					= false;
			}
		};
		struct TRIGGER_DATA_LINE
		{
			TRIGGER_LINE_ACTION_TYPE	m_iLineTriggerAction;
			int				m_iTriggerLineIndex;
			float			m_fTriggerTime;
			bool			m_bEnable;
			float			m_fSpeed;
			D3DXVECTOR3		m_vJumpSpeed;
			int				m_iCamera;
			bool			m_bDone;

			TRIGGER_DATA_LINE()
			{
				m_iLineTriggerAction	= TAT_LINE_NONE;
				m_iTriggerLineIndex		= -1;
				m_fTriggerTime			= 0.f;
				m_bEnable				= true;
				m_fSpeed				= 0.f;
				m_vJumpSpeed			= D3DXVECTOR3(0.f, 0.f, 0.f);
				m_iCamera				= -1;
				m_bDone					= false;
			}
		};
		struct TRIGGER_DATA_ETC
		{
			TRIGGER_ETC_ACTION_TYPE	m_iEtcTriggerAction;
			wstring			m_wstrActionName;
			float			m_fTriggerTime;
			int				m_iDummyValue01;
			float			m_fDummyValue01;
			bool			m_bDone;

			TRIGGER_DATA_ETC()
			{
				m_iEtcTriggerAction = TAT_ETC_NONE;
				m_wstrActionName	= L"";
				m_fTriggerTime		= 0.f;
				m_iDummyValue01		= -1;
				m_fDummyValue01		= 0.f;
				m_bDone					= false;
			}
		};
		struct TRIGGER_DATA
		{			
			float	m_fElapsedTime;

			std::vector<TRIGGER_DATA_MESH>	m_vecTriggerMesh;
			std::vector<TRIGGER_DATA_LINE>	m_vecTriggerLine;
			std::vector<TRIGGER_DATA_ETC>	m_vecTriggerEtc;
		
			TRIGGER_DATA()
			{
				m_fElapsedTime = 0.f;

				m_vecTriggerMesh.clear();
				m_vecTriggerLine.clear();
				m_vecTriggerEtc.clear();
			}
		};
		struct TRIGGER_SEGMENT
		{
			int		m_iTriggerId;			
			int		m_iTriggerDataIndex;
			bool	m_bLoop;
			std::vector<TRIGGER_DATA>	m_vecTriggerData;

			TRIGGER_SEGMENT()
			{
				m_iTriggerId = -1;
				m_iTriggerDataIndex = 0;

				m_vecTriggerData.clear();
			}
		};
		struct LINEMAP_TRIGGER
		{
			std::vector<int>				m_vecNowTrigger;
			std::vector<TRIGGER_SEGMENT>	m_vecTriggerSeg;
		};

		struct TriggerLess
		{
			bool operator() (const TRIGGER_SEGMENT& a, const TRIGGER_SEGMENT& b)
			{
				return a.m_iTriggerId < b.m_iTriggerId;
			}
		};
#endif

		struct LineAnimData;
		typedef std::map< int, LineAnimData* > LineAnimDataMap;					// line anim index, line anim data

		struct LineData
		{
			int					lineIndex;		// line ��ȣ

			LINE_TYPE			lineType;		//���� ����
			bool				bEnable;		//���� �����ϴ°�

			LINE_DUST_TYPE		eDustType;		//�پ�ٴ� �� ����� ���� ��ƼŬ ����
			int					potalGroup;		//�� �׷��� ��Ż���� �̵� ����
			float				fStopFactor;	//�������
			bool				bCanDown;		//�� ������ ������ �� �ִ°�

			int					beforeLine;
			int					nextLine;

			D3DXVECTOR3			startPos;
			D3DXVECTOR3			endPos;
			D3DXVECTOR3			dirVector;
			D3DXVECTOR3			dirDegree;
			D3DXVECTOR3			dirRadian;
			float				fLength;

			int					lineSet;
			float				m_fSpeed;
			D3DXVECTOR3			m_vJumpSpeed;

			int					m_iTeleportLineIndex;

#ifdef WALL_JUMP_TEST
			bool				m_bEnableWallJump;
#endif WALL_JUMP_TEST

			bool				m_bMonsterAllowed;		// ���Ͱ� �� ���θ��� �ν��ϰ� ���� �� �ִ���
			bool				m_bCanPass;				// jump up, jump down �ؼ� �� ���θ��� ��� �� �� �ִ���

			int					m_iCameraIndex;
#ifdef OTHER_ROAD
			bool				m_bOtherRoadUp;
#endif
			LineAnimData*		m_pLineAnimData;


//#ifdef LINEMAP_SLOW_WIND_TEST
//			D3DXVECTOR2			m_vWindSpeed;
//			float				m_fWindRange;	// m_vWindSpeed�� ����Ǵ� ��������
//#endif LINEMAP_SLOW_WIND_TEST
			

//#ifdef LINEMAP_FAST_WIND_TEST
//			float m_fUpsideWindAccelaration;
//			float m_fUpsideWindRange;			// m_fUpsideWindAccelaration�� ����Ǵ� ���� ����
//#endif LINEMAP_FAST_WIND_TEST



#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
			std::vector< int > m_vecLinkedLineIndex;
			int m_iLineDepth;
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST


			float m_fCliffHeight;

			bool	m_bUnderWater;		// ���߶��θʿ���
			float	m_fWaterHeight;		// ���߳���(������� ����Y��ġ)
			float	m_fBuoyancy;		// �η°�
			float	m_fResistance;		// ���װ�
			float	m_fAnimSpeed;		// �ӵ���
			float	m_fVarianceOxyen;	// ��Һ�����
			D3DXCOLOR m_cLineColor;		// ���ο� ���� ����Į��
			bool	m_bForceChangeColor; // �ش� ���θʿ� ��ġ�ҽ� ������ ���ֻ��� ����
#ifdef ATTACH_MESH_LINE
			bool		m_bMoveLine;
			D3DXVECTOR3 m_vPivotPos;	// ���� �߽ɰ�
			D3DXVECTOR3 m_vOffsetPos;	
#endif
#ifdef WORLD_TRIGGER
			int			m_iTriggerId;
			bool		m_bFootOnLine;
#endif

#ifdef WORLD_TOOL
			D3DXVECTOR3		m_vOriJumpSpeed;
			float			m_fOriSpeed;
			int				m_iOriTeleportLineIndex;
			int				m_iOriCameraIndex;			
			bool			bOriEnable;
			bool			m_bOriCanPass;

			bool			m_bSelected;
#endif

			LineData()
			{
				Init();
			}

			void Init()
			{
				lineIndex		= -1;
				lineType		= LT_NORMAL;
				bEnable			= true;		

				potalGroup		= -1;		
				fStopFactor		= 1.0f;		
				bCanDown		= true;
				eDustType		= LDT_DEFALT_DUST;

				startPos		= D3DXVECTOR3(0,0,0);
				endPos			= D3DXVECTOR3(0,0,0);
				dirVector		= D3DXVECTOR3(0,0,0);
				dirDegree		= D3DXVECTOR3(0,0,0);
				dirRadian		= D3DXVECTOR3(0,0,0);
				fLength			= 0.0f;

				beforeLine		= -1;
				nextLine		= -1;

				lineSet			= -1;
				m_fSpeed		= 0.0f;
				m_vJumpSpeed	= D3DXVECTOR3(0,0,0);

				m_iTeleportLineIndex = -1; 

#ifdef WALL_JUMP_TEST
				m_bEnableWallJump = false;
#endif WALL_JUMP_TEST

				m_bMonsterAllowed	= true;
				m_bCanPass			= true;

				m_iCameraIndex		= -1;

				m_pLineAnimData		= NULL;

#ifdef OTHER_ROAD
				m_bOtherRoadUp		= false;
#endif


//#ifdef LINEMAP_SLOW_WIND_TEST
//				m_vWindSpeed		= D3DXVECTOR2( 0, 0 );
//				m_fWindRange		= 0.f;
//#endif LINEMAP_SLOW_WIND_TEST
//				
				
//#ifdef LINEMAP_FAST_WIND_TEST
//				m_fUpsideWindAccelaration	= 0.f;
//				m_fUpsideWindRange			= 0.f;
//#endif LINEMAP_FAST_WIND_TEST
//				



#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
				m_iLineDepth = 0;
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST


				m_fCliffHeight = 0;

//{{ kimhc // 2010.7.6 // ���Ͱ� ������ ���θ�
#ifdef	LINE_MAP_CREATED_BY_MONSTER
				m_bTempLineMap	= false;
#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.6 // ���Ͱ� ������ ���θ�

				m_bUnderWater = false;
				m_fWaterHeight	= 0.f;
				m_fBuoyancy	= 0.f;
				m_fResistance = 0.f;
				m_fAnimSpeed = 1.f;
				m_fVarianceOxyen = 0.f;
				m_bForceChangeColor = false;
#ifdef ATTACH_MESH_LINE
				m_bMoveLine = false;
				m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
				m_vOffsetPos = D3DXVECTOR3(0.f, 0.f, 0.f);
#endif

#ifdef WORLD_TOOL
				m_bSelected = false;

				m_vOriJumpSpeed = D3DXVECTOR3(0.f, 0.f, 0.f);
				m_fOriSpeed = 0.f;
				m_iOriTeleportLineIndex = -1;
				m_iOriCameraIndex = -1;
				
				m_bOriCanPass	= true;
				bOriEnable		= true;

#endif
#ifdef WORLD_TRIGGER
				m_iTriggerId = -1;
				m_bFootOnLine = true;
#endif
			}




#ifdef LINKED_LINEMAP_JUMP_MONSTER_TEST
			int GetRandomLinkedLineIndex( int iRandomSeed = 0 );
#endif LINKED_LINEMAP_JUMP_MONSTER_TEST

		}; // LineData




		struct LineAnimData
		{
			int							m_iLineAnimIndex;
			bool						m_bEnabled;
			LINE_ANIM_TYPE				m_eAnimType;
			bool						m_bMoveIfSteppedOn;
			D3DXVECTOR3					m_vPivotPosition;
			D3DXVECTOR3					m_vChildPivotPosition;
			D3DXVECTOR3					m_vChildPivotPositionDisplace;		// child pivot position - old child pivot position

			CXSLLineMap::LineData* 	m_pLineData;
			

			std::vector< int >	m_vecForwardStepOnLineIndex;		// �� ���;ȿ� �ִ� ������ ������ animdata�� ������ �ִ� ������ ������ ���� �����̰� �Ǵ� ������ �ε���
			std::vector< int >	m_vecBackwardStepOnLineIndex;		// �� ���;ȿ� �ִ� ������ ������ animdata�� ������ �ִ� ������ ������ ���� �����̰� �Ǵ� ������ �ε���

			std::vector<LineAnimData*>	m_vecpChildAnimData;		
			int							m_iParentAnimDataIndex;		// lua script ���� data parsing �� ���� ����Ѵ�.

		public: 
			LineAnimData( LINE_ANIM_TYPE eAnimType );
			virtual void OnFrameMove( double fTime, float fElapsedTime );
		};

		

		struct LineAnimBackAndForth : LineAnimData
		{
			D3DXVECTOR3			m_vMovingDirection;
			float				m_fMovingSpeed;
			float				m_fRange;

			float				m_fCurrentPositionValue;

			LineAnimBackAndForth();
			virtual void OnFrameMove( double fTime, float fElapsedTime );
		};


		struct LineAnimViking : LineAnimData
		{
			D3DXVECTOR3		m_vRotationAxis;
			D3DXVECTOR3		m_vEquilibriumPosition;		// ���ڰ� �������¿��� �������� �ʰ� ���� ���� ��ġ, rotation Center�� �߽����� �����ġ
			float			m_fPeriod;					// ���ڿ �ѹ� �պ��ϴµ� �ɸ� �ð�, ��, �ֱ�
			float			m_fMaxRotationRadian;
			
			float			m_fCurrentTime;

			LineAnimViking();
			virtual void OnFrameMove( double fTime, float fElapsedTime );
			void CalcLinePosition( float fCurrentTime );
		};



		struct LineAnimCircle : LineAnimData
		{
			D3DXVECTOR3		m_vRotationAxis;
			D3DXVECTOR3		m_vPositionVector;			// ���� �߽ɿ��� ������ �߽���ġ�� ����Ű�� ����
			float 			m_fRotationSpeed;
			
			float			m_fCurrentRotationRadian;

			LineAnimCircle();
			virtual void OnFrameMove( double fTime, float fElapsedTime );
			void CalcLinePosition( float fCurrentRotationRadian );
		};


		struct LineAnimMoveIfSteppedOn : LineAnimData
		{
			D3DXVECTOR3			m_vMovingDirection;
			float 				m_fAccelerationWhenSteppedOn;
			float				m_fMaxMovingRange;					// 0�̸� ������ ���� 
			float				m_fMaxMovingSpeed;					// 0�̸� ������ ���� 
			float				m_fBackwardSpeed;					// �������� ĳ���Ͱ� ���� �� ������ ����ġ�� ���ư��� �ӵ�

			bool				m_bSteppedOnForward;
			bool				m_bSteppedOnBackward;
			float				m_fCurrentMovingSpeed;
			float				m_fCurrentPostionValue;


			LineAnimMoveIfSteppedOn();
			virtual void OnFrameMove( double fTime, float fElapsedTime );
		};

#ifdef LINEMAP_RECT_TEST
		struct RectData
		{
			bool			m_bEnable;
			
			D3DXVECTOR3		m_vLeftTop;
			D3DXVECTOR3		m_vRightBottom;
			D3DXVECTOR3		m_vLeftBottom;
		};
#endif LINEMAP_RECT_TEST



#ifdef NEW_VILLAGE_RENDERING_TEST
		struct CameraData
		{
			LINE_CAMERA_TYPE m_eType;
			
			D3DXVECTOR3		m_vEye;
			D3DXVECTOR3		m_vLookAt;
			bool			m_bFocusUnit;
			D3DXVECTOR3		m_vRelativeEye;
			float			m_fCameraRepositionSpeed;

			CameraData() 
			: m_eType( LCT_INVALID )
			, m_vEye( 0.f, 0.f, 0.f )
			, m_vLookAt( 0.f, 0.f, 0.f )
			, m_bFocusUnit( true )
			, m_vRelativeEye( 0.f, 0.f, 0.f )
			, m_fCameraRepositionSpeed( 100.f )
			{
			}
		};

#endif NEW_VILLAGE_RENDERING_TEST

		struct LineGroup
		{
			std::vector<LineData*> vecpLineData;
			D3DXVECTOR3 vStartPos;
			D3DXVECTOR3 vEndPos;
			
			D3DXVECTOR3 GetStartPos() const { return vStartPos; }
			D3DXVECTOR3 GetEndPos() const { return vEndPos; }
			
			LineGroup()
			{
				vStartPos = D3DXVECTOR3(0, 0, 0);
				vEndPos = D3DXVECTOR3(0, 0, 0);
			}
		};


	private:
		CXSLLineMap();
		virtual ~CXSLLineMap(void);

	public:
		static CXSLLineMap* CreateLineMap() { return new CXSLLineMap; }
		bool LoadLineMap( std::wstring fileName );

		__forceinline bool IsOnLineConnect( D3DXVECTOR3 pos, int* pLastLineIndex, float fRadius = 1.0f, D3DXVECTOR3* pOutPos = NULL, bool bIsMonster = false )
		{
			D3DXVECTOR3	temp;
			float		length;
			LineData*	pLineData;

            pLineData = m_LineList[*pLastLineIndex];
#ifdef MONSTER_ROAD
            if( bIsMonster == false && pLineData->lineType == LT_MONSTER_ROAD )
                return false;
#endif

#ifdef UNIT_ROAD
            if( bIsMonster == true && pLineData->lineType == LT_UNIT_ROAD )
                return false;
#endif

//{{ kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����
#ifdef	FIX_LINE_TYPE_WALL
			if ( LT_WALL == pLineData->lineType )
				return false;
#endif	FIX_LINE_TYPE_WALL
//}} kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����
			
			length = GetNearest( pos, &temp, *pLastLineIndex );

			if( length < fRadius )
			{
				if( pOutPos != NULL )
					*pOutPos = temp;
				return true;
			}
			else
			{
				float fDistanceToStart	= 999999;
				float fDistanceToEnd	= 999999;
				//pLineData = m_LineList[*pLastLineIndex];

				fDistanceToStart	= GetDistance( pos, pLineData->startPos );
				fDistanceToEnd		= GetDistance( pos, pLineData->endPos );

				LineData*	pBeforeLineData = m_LineList[pLineData->beforeLine];
				LineData*	pNextLineData   = m_LineList[pLineData->nextLine];

				if( fDistanceToStart < fDistanceToEnd )
				{
					if( pLineData->beforeLine != -1
						&& pLineData->beforeLine != -2 
						&& true == pBeforeLineData->bEnable )
					{
#ifdef MONSTER_ROAD
                        if( bIsMonster == false && pBeforeLineData->lineType == LT_MONSTER_ROAD )
                            return false;
#endif
#ifdef UNIT_ROAD
                        if( bIsMonster == true && pBeforeLineData->lineType == LT_UNIT_ROAD )
                            return false;
#endif
						//{{ kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����
					#ifdef	FIX_LINE_TYPE_WALL
						if ( LT_WALL == pLineData->lineType )
							return false;
					#endif	FIX_LINE_TYPE_WALL
						//}} kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����

						if( pOutPos != NULL )
							*pOutPos = m_LineList[pLineData->beforeLine]->endPos;
						*pLastLineIndex = pLineData->beforeLine;
						return true;
					}
					else
					{
						//if( outPos != NULL )
						//	*outPos = lineData.startPos;
						return false;
					}
				}
				else
				{
					if( pLineData->nextLine != -1
						&& pLineData->nextLine != -2 
						&& true == pNextLineData->bEnable )
					{
#ifdef MONSTER_ROAD
                        if( bIsMonster == false && pNextLineData->lineType == LT_MONSTER_ROAD )
                            return false;
#endif
#ifdef UNIT_ROAD
                        if( bIsMonster == true && pNextLineData->lineType == LT_UNIT_ROAD )
                            return false;
#endif
						//{{ kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����
					#ifdef	FIX_LINE_TYPE_WALL
						if ( LT_WALL == pLineData->lineType )
							return false;
					#endif	FIX_LINE_TYPE_WALL
						//}} kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����

						if( pOutPos != NULL )
							*pOutPos = m_LineList[pLineData->nextLine]->startPos;
						*pLastLineIndex = pLineData->nextLine;
						return true;
					}
					else
					{
						//if( outPos != NULL )
						//	*outPos = lineData.endPos;
						return false;
					}
				}
			}
		}
		__forceinline bool IsOnLine( D3DXVECTOR3 pos, float fRadius = 1.0f, D3DXVECTOR3* pOutPos = NULL, int* pLineIndex = NULL, bool bIsMonster = false )
		{
			D3DXVECTOR3	temp;
			float		length;
			int			index;

			length = GetNearest( pos, &temp, &index, bIsMonster );

			if( length < fRadius )
			{
				if( pOutPos != NULL )
					*pOutPos = temp;
				if( pLineIndex != NULL )
					*pLineIndex = index;
				return true;
			}
			else
			{
				return false;
			}
		}

		__forceinline bool IsDownLine( D3DXVECTOR3 pos, float fRadius = 1.0f, D3DXVECTOR3* pOutPos = NULL, int* pLineIndex = NULL, bool bIsMonster = false )
		{
			D3DXVECTOR3	temp;
			float		length;
			int			index;

			length = GetDownNearest( pos, &temp, &index );

			if( length < fRadius )
			{
				if( pOutPos != NULL )
					*pOutPos = temp;
				if( pLineIndex != NULL )
					*pLineIndex = index;
				return true;
			}
			else
			{
				return false;
			}
		}

		__forceinline LineData* GetLineData( int index );
		__forceinline const int GetNumLineData() { return (int)m_LineList.size(); }
		__forceinline const float GetLandHeight() { return m_fLandHeight; }		
#ifdef NEW_VILLAGE_RENDERING_TEST
		__forceinline const int GetNumCameraData() { return static_cast<int>(m_vecCameraData.size()); }
		__forceinline const CameraData * GetCameraDataList( int index )
		{
			if( 0 <= index && GetNumCameraData() > index )
				return &(m_vecCameraData[index]);

			return NULL;
		}
#endif NEW_VILLAGE_RENDERING_TEST
#ifdef WORLD_TOOL
		__forceinline CameraData * GetCameraDataListByTool( int index )
		{
			if( 0 <= index && GetNumCameraData() > index )
				return &(m_vecCameraData[index]);

			return NULL;
		}
#endif

		int GetRedTeamStartPosNum(){ return m_RedTeamStartPos.size(); }
		int GetBlueTeamStartPosNum(){ return m_BlueTeamStartPos.size(); }
		int GetStartPosNum(){ return m_mapStartPos.size(); }
		std::map<int, D3DXVECTOR3>& GetStartPosMap() { return m_mapStartPos; }
		
		D3DXVECTOR3 GetRedTeamStartPosition( int index )
		{
			if( index < 0 || index >= (int)m_RedTeamStartPos.size() )
				return D3DXVECTOR3( 0, 0, 0 );

			return m_RedTeamStartPos[index];
		}
		D3DXVECTOR3 GetBlueTeamStartPosition( int index )
		{ 
			if( index < 0 || index >= (int)m_BlueTeamStartPos.size() )
				return D3DXVECTOR3( 0, 0, 0 );

			return m_BlueTeamStartPos[index]; 
		}
		D3DXVECTOR3 GetStartPosition( int key )
		{ 
			std::map<int, D3DXVECTOR3>::iterator it = m_mapStartPos.find( key );
			if( it != m_mapStartPos.end() )
			{
				return it->second;
			}

			return D3DXVECTOR3( 0, 0, 0 );
		}

		bool GetRedTeamStartRight( int index )
		{ 
			if( index < 0 || index >= (int)m_RedTeamStartRight.size() )
				return true;

			return m_RedTeamStartRight[index]; 
		}
		bool GetBlueTeamStartRight( int index )
		{ 
			if( index < 0 || index >= (int)m_BlueTeamStartRight.size() )
				return true;

			return m_BlueTeamStartRight[index]; 
		}
		bool GetStartRight( int key )
		{ 
			std::map<int, bool>::iterator it = m_mapStartRight.find( key );
			if( it != m_mapStartRight.end() )
			{
				return it->second;
			}

			return true;
		}

		int GetRedTeamStartLineIndex( int index )
		{ 
			if( index < 0 || index >= (int)m_RedTeamStartLineIndex.size() )
				return -1;

			return m_RedTeamStartLineIndex[index]; 
		}
		int GetBlueTeamStartLineIndex( int index )
		{ 
			if( index < 0 || index >= (int)m_BlueTeamStartLineIndex.size() )
				return -1;

			return m_BlueTeamStartLineIndex[index]; 
		}
		int GetStartLineIndex( int key )
		{
			std::map<int, int>::iterator it = m_mapStartLineIndex.find( key );
			if( it != m_mapStartLineIndex.end() )
			{
				return it->second;
			}

			return -1;
		}

		int GetRandomStartPosIndex();
				
        D3DXVECTOR3 GetNearest_LUA(D3DXVECTOR3 inPos);

		bool Step( int lineIndex, D3DXVECTOR3 pos );

		void ReleaseLineGroup();
		void ReBuildLineGroup();
		int GetLineSetIndex( int lineIndex );
		LineGroup* GetLineGroupIncludesLineData( CXSLLineMap::LineData* pLineData );
		LineGroup* GetLineGroup(int index) { return m_vecLineGroupList[index]; }
		void EnableLineData( CXSLLineMap::LineData* pLineData, bool bEnable );
		void EnableLineSet( int iLineSetIndex, bool bEnable );
		void DisableAllLineData();
		

		//LUA
		void SetLandHeight( float fLandHeight ){ m_fLandHeight = fLandHeight; }
		void SetTeamStartPos( bool bRed, D3DXVECTOR3 pos, bool bRight, int iLineIndex )
		{
			if( bRed == true )
			{
				m_RedTeamStartPos.push_back( pos );
				m_RedTeamStartRight.push_back( bRight );
				m_RedTeamStartLineIndex.push_back( iLineIndex );
			}
			else
			{
				m_BlueTeamStartPos.push_back( pos );
				m_BlueTeamStartRight.push_back( bRight );
				m_BlueTeamStartLineIndex.push_back( iLineIndex );
			}
		}
		void AddStartPos( int key, D3DXVECTOR3 pos, bool bRight, int iLineIndex )
		{
			m_mapStartPos[key]			= pos;
			m_mapStartRight[key]		= bRight;
			m_mapStartLineIndex[key]	= iLineIndex;
		}		
		bool AddLine_LUA();


		// �����̴� ���θ�
		bool AddLineAnim_LUA();
		bool AssignLineAnimDataToLine( CXSLLineMap::LineData* pLineData, int iLineAnimDataIndex );		

#ifdef LINEMAP_RECT_TEST
		bool AddRect_LUA();
		bool IsInAnyRect( const D3DXVECTOR3& vPoint, D3DXVECTOR3* pNearestOutPoint = NULL );
#endif LINEMAP_RECT_TEST

		bool AddCameraData_LUA();


#ifdef NEW_VILLAGE_RENDERING_TEST

		const CameraData* GetCameraData( int iLineIndex ) const
		{
			if( iLineIndex < 0 ||
				iLineIndex >= (int) m_LineList.size() )
			{
				return NULL;
			}

			LineData* pLineData = m_LineList[ iLineIndex ];
			
			if( NULL == pLineData )
				return NULL;

			if( pLineData->m_iCameraIndex < 0 ||
				pLineData->m_iCameraIndex >= (int) m_vecCameraData.size() )
			{
				return NULL;
			}

			return &m_vecCameraData[ pLineData->m_iCameraIndex ];
		}

		void AddCameraData( CameraData cameraData ) { m_vecCameraData.push_back(cameraData); }

#endif NEW_VILLAGE_RENDERING_TEST


        // ��� ���ε� �߿��� pos�� ���� ����� ������ ã�� �ִ� �Ÿ� ������ pOutPos�� ����Ѵ�.
        // ���ϰ� : pos�� ���� ����� ������ index
        __forceinline int		GetNearestLine(	IN const D3DXVECTOR3& pos, OUT D3DXVECTOR3* pOutPos = NULL, IN bool bIsMonster = false )
        {
            int			touchLine		= -1;
            float		touchDistance	= 9999999.0f;
            D3DXVECTOR3	touchPos;

            //��� ���� ���ٸ� ��� ���� �˻��Ѵ�
            for( int i = 0; i < (int)m_LineList.size(); i++ )
            {
                LineData* pLineData = m_LineList[i];

                //�������� ���Ǹ� Ȯ���Ѵ�
#if defined(MONSTER_ROAD) || defined(UNIT_ROAD)
                if( VerifyLine( i, bIsMonster ) == false )
                    continue;
#else                
                if( VerifyLine( i ) == false )
                    continue;
#endif

                //���� �������� �ִ� �Ÿ��� ���Ѵ�.
                D3DXVECTOR3 projectionPoint;
                float		fFinalLength;
                fFinalLength = GetLengthToLine( pos, i, &projectionPoint );

                if( fFinalLength <= touchDistance )
                {
                    touchLine		= i;
                    touchDistance	= fFinalLength;
                    touchPos		= projectionPoint;
                }
            }

            if( pOutPos != NULL )
                *pOutPos = touchPos;

            return touchLine;
        }

		LineData* GetAnyEnabledNormalLine();

		bool GetAnyEnabledLine();	// SERV_PET_SYSTEM


		float GetXMin() const { return m_fXMin; }
		float GetXMax() const { return m_fXMax; }
		float GetYMin() const { return m_fYMin; }
		float GetYMax() const { return m_fYMax; }


		float GetStopTime() const { return m_fStopTime; }
		void SetStopTime(float val) { m_fStopTime = val; }


		static void	MakeLine( LineData* pLineData );

//{{ kimhc // 2010.7.6 // ���Ͱ� ������ ���θ�
#ifdef	LINE_MAP_CREATED_BY_MONSTER
		void EnableLineData( int iIndex, bool bEnable )
		{
			LineData* pLineData =  GetLineData( iIndex );

			if ( pLineData != NULL )
				EnableLineData( pLineData, bEnable );
		}

		void EnableLineData_LUA( int iIndex, bool bEnable )
		{
			EnableLineData( iIndex, bEnable );
		}

		void SetStartEndPosAtThisLineMap_LUA( int iIndex, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vEndPos );
		int GetNumofLineMapCreatedByMonster() const { return m_iNumofLineMapCreatedByMonster; }
		void SetNumofLineMapCreatedByMonster(int val) { m_iNumofLineMapCreatedByMonster = val; }
#endif	LINE_MAP_CREATED_BY_MONSTER
//}} kimhc // 2010.7.6 // ���Ͱ� ������ ���θ�
		
	private:
        // lineIndex�� ������ �������� �������� �����Ѵ�. lineIndex�� ���� ���� ���ε��� �ε��� ���� ���� �־�� �Ѵ�.
        // MONSTER_ROAD �� ���ǵǾ� ������ bIsMonster �� false �� �� ���� �ε� Ÿ���� ���ε��� �������� ������ �ƴϴ�.
        // UNIT_ROAD �� ���ǵǾ� ������ bIsMonster�� true �� �� ���� �ε� Ÿ���� ���ε��� �������� ������ �ƴϴ�.
        // �� Ÿ���� ������ �������� ������ �ƴϴ�.
        // ���ϰ� : �������� ���������� ����
		__forceinline bool	VerifyLine( IN int lineIndex, IN bool bIsMonster = false )
		{
			//������ �����̸� TRUE
			if( lineIndex < 0 || (int)m_LineList.size() <= lineIndex )
				return false;

			LineData* pLineData = m_LineList[lineIndex];

            if( !pLineData->bEnable )
            {
                return false;
            }

#ifdef MONSTER_ROAD
            if( !bIsMonster && pLineData->lineType == LT_MONSTER_ROAD )
                return false;
#endif
#ifdef UNIT_ROAD
            if( bIsMonster && pLineData->lineType == LT_UNIT_ROAD )
                return false;
#endif
			if( pLineData->lineType == LT_WALL )
			{
				return false;
			}

			return true;
		}       

        // pos ���� lineIndex�� ���α����� �ִ� �Ÿ��� ���ϰ� ���� ���� �ִ� �Ÿ� ������ pOutPos �� ����Ѵ�.
        // ���ϰ� : lineIndex�� ������ �������� ������ �ƴ� ��� -1.0f�� �����Ѵ�. �������� �����̸� �ִ� �Ÿ���
        //          �����Ѵ�.
		__forceinline float	GetLengthToLine( IN const D3DXVECTOR3& pos, IN int lineIndex, OUT D3DXVECTOR3* pOutPos = NULL )
		{
			LineData* pLineData = m_LineList[lineIndex];

			//�������� ���Ǹ� Ȯ���Ѵ�
			if( VerifyLine( lineIndex ) == false )
				return -1.0f;

			//���� �������� �ִ� �Ÿ��� ���Ѵ�.
			float fLineLength				= GetDistance( pLineData->startPos, pLineData->endPos );
			float fLineStartToPointLength	= GetDistance( pLineData->startPos, pos );

			D3DXVECTOR3 lineDir = pLineData->endPos - pLineData->startPos;
			D3DXVec3Normalize( &lineDir, &lineDir );
			D3DXVECTOR3 pointDir = pos - pLineData->startPos;

			D3DXVECTOR3 projectionPoint;
			float		fProjectionLength	= D3DXVec3Dot( &lineDir, &pointDir );

			//���� �ִܰŸ��� �� ���ΰ� �ƴ϶�� �� �����ڸ��� �����Ѵ�.
			if( fProjectionLength <= 0.0f )
				projectionPoint = pLineData->startPos;
			else if( fProjectionLength >= fLineLength )
				projectionPoint = pLineData->endPos;
			else
				projectionPoint		= lineDir * fProjectionLength + pLineData->startPos;

			float fFinalLength		= GetDistance( pos, projectionPoint );

			if( pOutPos != NULL )
				*pOutPos = projectionPoint;

			return fFinalLength;
		}

        // ��� ���ε� ���� ���� �߿��� inPos���� ���� ����� ���� ã�� �װ��� pOutPos�� ����Ѵ�.
        // MONSTER_ROAD �� ���ǵǾ� ������ bIsMonster �� false �� �� ���� �ε� Ÿ���� ���ε��� �����ϰ�,
        // UNIT_ROAD �� ���ǵǾ� ������ bIsMonster�� true �� �� ���� �ε� Ÿ���� ���ε��� �����Ѵ�.
        // ���ϰ� : inPos���� pOutPos������ �Ÿ��� �����Ѵ�.
		__forceinline float GetNearest( IN D3DXVECTOR3 inPos, OUT D3DXVECTOR3* pOutPos, IN bool bIsMonster = false )
		{
			D3DXVECTOR3 target;
			float		fProjectionLength;
			float		outLength = 999999;
			float		tempLength;
			D3DXVECTOR3	tempPos;

			for( int i = 0; i < (int)m_LineList.size(); i++ )
			{
				LineData* pLineData = m_LineList[i];
				if( false == pLineData->bEnable )
					continue;

#ifdef MONSTER_ROAD
                if( bIsMonster == false && pLineData->lineType == LT_MONSTER_ROAD )
                    continue;
#endif
#ifdef UNIT_ROAD
                if( bIsMonster == true && pLineData->lineType == LT_UNIT_ROAD )
                    continue;
#endif

				//{{ kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����
			#ifdef	FIX_LINE_TYPE_WALL
				if ( LT_WALL == pLineData->lineType )
					continue;
			#endif	FIX_LINE_TYPE_WALL
				//}} kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����

				if( (inPos.x < pLineData->startPos.x && inPos.x < pLineData->endPos.x)
					|| (inPos.x > pLineData->startPos.x && inPos.x > pLineData->endPos.x) )
				{
					if( (inPos.y < pLineData->startPos.y && inPos.y < pLineData->endPos.y)
						|| (inPos.y > pLineData->startPos.y && inPos.y > pLineData->endPos.y) )
					{
						if( (inPos.z < pLineData->startPos.z && inPos.z < pLineData->endPos.z)
							|| (inPos.z > pLineData->startPos.z && inPos.z > pLineData->endPos.z) )
						{
							continue;
						}
					}
				}

				target				= inPos - pLineData->startPos;
				fProjectionLength	= D3DXVec3Dot( &pLineData->dirVector, &target );
				if( fProjectionLength >= 0.0f && fProjectionLength < pLineData->fLength )
				{
					tempPos		= fProjectionLength * pLineData->dirVector + pLineData->startPos;
					tempLength	= GetDistance( tempPos, inPos );
					if( tempLength < outLength )
					{
						outLength	= tempLength;
						*pOutPos	= tempPos;
					}
				}
			}
			return outLength;
		}

        // inPos���� ���� ����� ������ ã�� �� ���λ��� ���� �� inPos�� ���� ����� ���� pOutPos�� ����Ѵ�.
        // MONSTER_ROAD �� ���ǵǾ� ������ bIsMonster �� false �� �� ���� �ε� Ÿ���� ���ε��� �����ϰ�,
        // UNIT_ROAD �� ���ǵǾ� ������ bIsMonster�� true �� �� ���� �ε� Ÿ���� ���ε��� �����Ѵ�.
        // ���ϰ� : ���� inPos�� ������ ������ �ִٸ� inPos���� pOutPos������ �Ÿ��� �����Ѵ�.
        //          �׷��� ������ 999999�� �����Ѵ�.
		__forceinline float GetNearest( IN D3DXVECTOR3 inPos, OUT D3DXVECTOR3* pOutPos, OUT int* pLlineIndex, IN bool bIsMonster = false )
		{
			D3DXVECTOR3 target;
			float		fProjectionLength;
			float		outLength = 999999;
			float		tempLength;
			D3DXVECTOR3	tempPos;

			//*lineIndex = -1;

			for( int i = 0; i < (int)m_LineList.size(); i++ )
			{
				LineData* pLineData = m_LineList[i];
				if( false == pLineData->bEnable )
					continue;
#ifdef MONSTER_ROAD
                if( bIsMonster == false && pLineData->lineType == LT_MONSTER_ROAD )
                    continue;
#endif
#ifdef UNIT_ROAD
                if( bIsMonster == true && pLineData->lineType == LT_UNIT_ROAD )
                    continue;
#endif

				//{{ kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����
			#ifdef	FIX_LINE_TYPE_WALL
				if ( LT_WALL == pLineData->lineType )
					continue;
			#endif	FIX_LINE_TYPE_WALL
				//}} kimhc // 2010.8.3 // WALL Ÿ���� ���θ� ó�� ����

				if( (inPos.x < pLineData->startPos.x && inPos.x < pLineData->endPos.x)
					|| (inPos.x > pLineData->startPos.x && inPos.x > pLineData->endPos.x) )
				{
					if( (inPos.y < pLineData->startPos.y && inPos.y < pLineData->endPos.y)
						|| (inPos.y > pLineData->startPos.y && inPos.y > pLineData->endPos.y) )
					{
						if( (inPos.z < pLineData->startPos.z && inPos.z < pLineData->endPos.z)
							|| (inPos.z > pLineData->startPos.z && inPos.z > pLineData->endPos.z) )
						{
							continue;
						}
					}
				}

				target				= inPos - pLineData->startPos;
				fProjectionLength	= D3DXVec3Dot( &pLineData->dirVector, &target );
				if( fProjectionLength >= 0.0f && fProjectionLength < pLineData->fLength )
				{
					tempPos		= fProjectionLength * pLineData->dirVector + pLineData->startPos;
					tempLength	= GetDistance( tempPos, inPos );
					if( tempLength < outLength )
					{
						outLength	= tempLength;
						*pOutPos		= tempPos;
						*pLlineIndex	= i;
					}
				}
			}
			return outLength;
		}

        // lineIndex�� ���λ󿡼� inPos �� ���� ����� ���� ã�� pOutPos�� ����Ѵ�.
        // ���ϰ� : ���� inPos�� ������ ������ �ִٸ� inPos���� pOutPos������ �Ÿ��� �����Ѵ�.
        //          �׷��� ������ 999999�� �����Ѵ�.
		__forceinline float GetNearest( IN D3DXVECTOR3 inPos, OUT D3DXVECTOR3* pOutPos, IN int lineIndex )
		{
			D3DXVECTOR3 target;
			float		fProjectionLength;
			float		outLength = 999999;
			float		tempLength;
			D3DXVECTOR3	tempPos;

			target				= inPos - m_LineList[lineIndex]->startPos;
			fProjectionLength	= D3DXVec3Dot( &m_LineList[lineIndex]->dirVector, &target );
			if( fProjectionLength >= 0.0f && fProjectionLength < m_LineList[lineIndex]->fLength )
			{
				tempPos		= fProjectionLength * m_LineList[lineIndex]->dirVector + m_LineList[lineIndex]->startPos;
				tempLength	= GetDistance( tempPos, inPos );
				if( tempLength < outLength )
				{
					outLength	= tempLength;
					*pOutPos		= tempPos;
				}
			}

			return outLength;
		}

		//{{ kimhc // 2010.7.6 // ���Ͱ� ������ ���θ�
#ifdef	LINE_MAP_CREATED_BY_MONSTER
	public:
#endif	LINE_MAP_CREATED_BY_MONSTER
		//}} kimhc // 2010.7.6 // ���Ͱ� ������ ���θ�

        // inPos���� �Ʒ��� �ִ� ���ε� �߿��� ���� ����� ������ ã�� �� ���λ��� ���� �� inPos�� ���� ����� ���� pOutPos�� ����Ѵ�.
        // ���ϰ� : ���� inPos�� ������ ������ �ִٸ� inPos���� pOutPos������ �Ÿ��� �����Ѵ�.
        //          �׷��� ������ 999999�� �����Ѵ�.
		__forceinline float GetDownNearest( IN D3DXVECTOR3 inPos, OUT D3DXVECTOR3* pOutPos, OUT int* pLlineIndex )
		{
			D3DXVECTOR3 target;
			float		fProjectionLength;
			float		outLength = 999999;
			float		tempLength;
			D3DXVECTOR3	tempPos;

			//*lineIndex = -1;

			for( int i = 0; i < (int)m_LineList.size(); i++ )
			{
				LineData* pLineData = m_LineList[i];
				if( false == pLineData->bEnable )
					continue;

				if( inPos.y < pLineData->startPos.y && inPos.y < pLineData->endPos.y )
					continue;

				if( (inPos.x < pLineData->startPos.x && inPos.x < pLineData->endPos.x)
					|| (inPos.x > pLineData->startPos.x && inPos.x > pLineData->endPos.x) )
				{
					if( (inPos.y < pLineData->startPos.y && inPos.y < pLineData->endPos.y)
						|| (inPos.y > pLineData->startPos.y && inPos.y > pLineData->endPos.y) )
					{
						if( (inPos.z < pLineData->startPos.z && inPos.z < pLineData->endPos.z)
							|| (inPos.z > pLineData->startPos.z && inPos.z > pLineData->endPos.z) )
						{
							continue;
						}
					}
				}

				target				= inPos - pLineData->startPos;
				fProjectionLength	= D3DXVec3Dot( &pLineData->dirVector, &target );
				if( fProjectionLength >= 0.0f && fProjectionLength < pLineData->fLength )
				{
					tempPos		= fProjectionLength * pLineData->dirVector + pLineData->startPos;
					tempLength	= GetDistance( tempPos, inPos );
					if( tempLength < outLength )
					{
						outLength		= tempLength;
						*pOutPos		= tempPos;
						*pLlineIndex	= i;
					}
				}
			}
			return outLength;
		}
		
#ifdef ATTACH_MESH_LINE
		void UpdateMoveLine(int iLineIndex, D3DXVECTOR3 &vPivotPos);
#endif

	private:
		std::vector<LineData*>			m_LineList;
		std::vector<LineData*>			m_WallList;

#ifdef LINEMAP_RECT_TEST
		std::vector<RectData*>			m_RectList;
#endif LINEMAP_RECT_TEST

		LPDIRECT3DVERTEXBUFFER9			m_pLineMapVB;

		float							m_fLandHeight;
		std::vector<D3DXVECTOR3>		m_RedTeamStartPos;
		std::vector<D3DXVECTOR3>		m_BlueTeamStartPos;
		std::vector<bool>				m_RedTeamStartRight;
		std::vector<bool>				m_BlueTeamStartRight;
		std::vector<int>				m_RedTeamStartLineIndex;
		std::vector<int>				m_BlueTeamStartLineIndex;

		std::map<int, D3DXVECTOR3>		m_mapStartPos;
		std::map<int, bool>				m_mapStartRight;
		std::map<int, int>				m_mapStartLineIndex;
	

		std::vector<LineGroup*>							m_vecLineGroupList;
		std::map<CXSLLineMap::LineData*, LineGroup*>	m_mapLineData2LineGroup;

#ifdef NEW_VILLAGE_RENDERING_TEST
		vector<CameraData>				m_vecCameraData;
#endif NEW_VILLAGE_RENDERING_TEST
		
		float							m_fXMin;
		float							m_fXMax;
		float							m_fYMin;
		float							m_fYMax;

		float							m_fStopTime;
};



