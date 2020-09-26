/** @file   X2WorldManager.h
    @date   jintaeks on 2008-10-21, 14:03
            - preprocessing data�� file�� ����ϴ� ����� �߰��ϴ�.
                background loading�۾��� ���� ��ü�� world�� ��ġ�ϱ� ���� �ʿ��� ���� - bounding
                sphere radius ���� - �� ���Ϸ� write�ϴ� ����� �߰��ϴ�.
    @date   jintaeks on 2008-10-24, 13:35
            - culling ������ Ȯ���ϱ� ���Ͽ� KHelpSystem�� �߰��ϴ�.
*/
#pragma once

class CXSLWorldManager
{
    public:

#ifdef KHELP_SYSTEM_TEST // 2008-10-24
        DECLARE_HELP_SYSTEM( CXSLWorldManager );
#endif // KHELP_SYSTEM_TEST // 2008-10-24

        //{{ seojt // 2008-10-21, 14:14
        /** @enum   PREPROCESSING_INFO_TYPE
            @brief  WORLD_PREPROCESSING_INFO�� ����ϴ� ��ó�� �������� ������ ��Ÿ����.
            @date   jintaeks on 2008-10-21, 14:19 */
        enum PREPROCESSING_INFO_TYPE
        {
            X2WM_PPIT_UNKNOWN       = -1,
            X2WM_PPIT_XMESH         = 0,
            X2WM_PPIT_XMESHLOD      = 1,
            X2WM_PPIT_XSKIN_MOTION  = 2,
            X2WM_PPIT_XSKIN_MODEL   = 3,
        };//enum PREPROCESSING_INFO_TYPE

        struct WORLD_PREPROCESSING_INFO
        {
            PREPROCESSING_INFO_TYPE     m_eInfoType;        ///< ��ó�� �������� ����
            std::wstring                m_wstrName;         ///< �޽� ���� �̸�
            D3DXVECTOR3                 m_vCenter;          ///< �޽��� local center
            float                       m_fBSphereRadiua;   ///< �޽� bounding sphere�� ������
        };//struct WORLD_PREPROCESSING_INFO
        //}} seojt // 2008-10-21, 14:14

	public:
		CXSLWorldManager();
		virtual ~CXSLWorldManager(void);

		CX2World* CreateWorld( CX2World::WORLD_ID worldID, KLuabinder* pKLuabinder = NULL );
		bool OpenScriptFile( const WCHAR* pFileName );

        //{{ seojt // 2008-10-22, 10:23
        /// ���� ��ü�� �ı��ϱ� �ٷ� ���� ȣ��ȴ�.
        void OnPreDestroyWorld( CX2World* pWorld_ );
        //}} seojt // 2008-10-22, 10:23

        CX2World* GetWorldLatest() { return m_pX2WorldLatestRef; }

		bool AddWorldData_LUA();
        //{{ seojt // 2008-10-21, 15:21
        /// ���� �Ŵ����� �����ϴ� ��ü�� ��ó�� ������ �����Ѵ�.
        bool SetPreprocessingData_LUA();
        //}} seojt // 2008-10-21, 15:21

	public: 
		const std::vector<CX2World::WorldData*>& GetWorldDataList() { return m_WorldDataList; }
		CX2World::WORLD_ID GetWorldIDUsingLineMapFileName( const WCHAR* pLineMapFileName );
		const CX2World::WorldData* GetWorldData( CX2World::WORLD_ID eWorldID );

        ///-----------------------------------------------------------------------------
        //{{ ��ó�� ������ ���� interface�� ���� - jintaeks on 2008-10-21, 14:25
        //
        void    EnableWritingPreprocessingData( bool bEnable_ );
        bool    IsWritingPreprocessingData() const { return m_bWritingPreprocessingData; }
        bool    WritePreprocessingData( const WORLD_PREPROCESSING_INFO& preInfo_ );
        //
        //}} ��ó�� ������ ���� interface�� ��
        ///-----------------------------------------------------------------------------

#ifdef BACKGROUND_LOADING_TEST // 2008-10-18
        /// ������ CreateWorld()�Լ��� thread ������ �����̴�.
        /// ���ҽ� �ε��� �����忡�� �����ϱ� ����, ���ҽ� �ε� �ڵ带 �����尡 �����ϴ�
        /// ���ҽ� ��û ť�� �״� �۾��� �Ѵ�.
        /// ������ ���ҽ� �ε��� �����忡�� �̷������.
        /// - jintaeks on 2008-10-18, 12:32
		CX2World* ThreadRequest_CreateWorld( CX2World::WORLD_ID worldID_, KLuabinder* pKLuabinder_ = NULL );
#endif // BACKGROUND_LOADING_TEST // 2008-10-18

    //{{ seojt // 2008-10-24, 13:53
    public:
        void Debug_RenderHelp( int ix_, int iy_ );
    //}} seojt // 2008-10-24, 13:53

	//{{ 2007. 9. 7  ������  ����� ���� protected����
	protected:
	//}}
		CRITICAL_SECTION				m_csWorldManager;
		std::vector<CX2World::WorldData*>	m_WorldDataList;
        CX2World*                       m_pX2WorldLatestRef;    ///< ���� �ֱٿ� ������ ���� ��ü�� ���� ������

        //{{ ��ó�� ������ ���� ���� - jintaeks on 2008-10-21, 14:25
        bool                            m_bWritingPreprocessingData;
        std::string                     m_strPreprocessingDataFile;
        //}} ��ó�� ������ ���� ��
};//class CX2WorldManager


