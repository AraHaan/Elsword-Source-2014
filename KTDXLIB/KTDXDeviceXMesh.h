/** @file   KTDXDeviceXMesh.h
    @date   jintaeks on 2008-10-15, 15:24
            - managed�Ǵ� ���ҽ��� local memory�� preload�ϴ� ����� �߰��ϴ�.
*/
#pragma once

//{{ robobeg : 2008-01-06
// ���ҵ忡�� ��� �ȵǴ� �κ��̹Ƿ� �ڸ�Ʈ ó��
//#define ProgressiveGap 10
//#define D3DFVF_BOUNDING_BOX_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
//}} robobeg : 2008-01-06

#define KOGMESH_FORMAT_MAGIC        (MAKEFOURCC('K','M','E',' '))
#define KOGMESH_FORMAT_VERSION      0x0200

#define D3DFVF_MESHVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


class CKTDXDeviceXMesh	: public CKTDXDevice 
{
	public:
		struct MESH_VERTEX
		{
			float x, y, z;
			float nx, ny, nz;
			DWORD color;
			float u,v;
		};

	public:
		CKTDXDeviceXMesh( LPDIRECT3DDEVICE9 pd3dDevice, std::wstring fileName );



        /// managed�� ���ҽ��� local memory�� preload�Ѵ�.
        /// - jintaeks on 2008-10-15, 15:24
        void            PreLoad();

#ifndef _KSMTOOL

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
		virtual HRESULT	Render( CKTDXDeviceXET* pTexChangeXET = NULL, CKTDXDeviceXET* pMultiTexXET = NULL, 
			const CKTDXDeviceXET::AniData* pAniData = NULL, float fAniTime = 0, const std::vector<bool>* pvecDrawSubset = NULL );
#else//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
		virtual HRESULT	Render( CKTDXDeviceXET* pTexChangeXET = NULL, CKTDXDeviceXET* pMultiTexXET = NULL, 
			const CKTDXDeviceXET::AniData* pAniData = NULL, float fAniTime = 0 );
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

#endif // #ifndef _KSMTOOL
		
		virtual float			GetRadius(){ return m_fRadius; }
		virtual D3DXVECTOR3		GetCenter(){ return m_vCenter; }

		LPD3DXBASEMESH			GetMesh();

#ifndef _KSMTOOL

		CKTDXDeviceXET*			GetXET(){ return m_pXET; }


#endif // #ifndef _KSMTOOL

       HRESULT					WriteToFileSerializer( KFileSerializer& kInOutFile_ ) const;

	   void						SetLOD( int lodPercent );

//{{ robobeg : 2008-01-06
// Ȥ�ö� CKTDXDeviceXMesh���� Ŭ������ �����ؼ� �����ϸ� ���� ����� ����� �浹�� ���̹Ƿ� private���� ����
	//protected:
    protected:

		virtual ~CKTDXDeviceXMesh(void);

		virtual HRESULT _Load( bool bSkipStateCheck = false
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD
				, bool bBackgroundQueueing = false
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD			
			);
		virtual HRESULT _UnLoad();
	private:

#ifndef _KSMTOOL


		CKTDXDeviceBaseTexture* SetNowTexture( CKTDXDeviceBaseTexture* orgTex, int stage,
											CKTDXDeviceXET* pTexChangeXET = NULL, CKTDXDeviceXET* pMultiTexXET = NULL, 
											const CKTDXDeviceXET::AniData* pAniData = NULL, float fAniTime = 0 );
		
#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	public:
		struct KSubsetCullingInfo
		{
            KSubsetCullingInfo()
                : m_vCenter(0,0,0)
                , m_fRadius(0)
            {
            }

			KSubsetCullingInfo( const D3DXVECTOR3& vCenter, float fRadius )
				: m_vCenter( vCenter ), m_fRadius( fRadius )
			{}

			D3DXVECTOR3 m_vCenter;
			float m_fRadius;
		};
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET

    private:
        struct  XMeshProxy
        {
            wstring		            m_DeviceID;
		    LPD3DXMESH				m_pMesh;
		    DWORD					m_dwNumMaterials;
            D3DXMATERIAL*           m_pdxMaterials;
#ifndef _KSMTOOL
		    CKTDXDeviceTexture**	m_ppTextures;
		    CKTDXDeviceXET*			m_pXET;
#endif  _KSMTOOL
            DWORD                   m_dwNumAttrGroups;
            LPD3DXATTRIBUTERANGE    m_pAttributeRange;
		    D3DXVECTOR3				m_vCenter;
		    float					m_fRadius;

            XMeshProxy( const std::wstring& strDeviceID_ );
            ~XMeshProxy();
            HRESULT                 LoadD3DXMesh( const void* pInData_, DWORD dwInSize_ );
            HRESULT                 LoadKMEMesh( const void* pInData_, DWORD dwInSize_ );
            void                    LoadTextures();
            HRESULT                 LoadXET();
#endif // #ifndef _KSMTOOL

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
			std::vector< KSubsetCullingInfo > m_vecSubsetCullingInfo;
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
        };//struct  XMeshProxy

		D3DXVECTOR3				m_vCenter;
		float					m_fRadius;

		DWORD					m_dwNumMaterials;
        D3DXMATERIAL*           m_pdxMaterials;

        DWORD                   m_dwNumAttrGroups;
        LPD3DXATTRIBUTERANGE    m_pAttributeRange;

#ifndef _KSMTOOL

		CKTDXDeviceTexture**	m_ppTextures;
		CKTDXDeviceXET*			m_pXET;

#endif // #ifndef _KSMTOOL

//{{ robobeg : 2008-01-06
// 		DWORD					m_cPMeshes;
// 		LPD3DXPMESH*			m_ppPMeshes;
// 		LPD3DXPMESH				m_pPMeshFull;
// 		int						m_iPMeshCur;
// 		bool					m_bLoadProgressiveMesh;
//}} robobeg : 2008-01-06
		LPD3DXMESH				m_pMesh;

#ifdef X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
	public:
		const std::vector< KSubsetCullingInfo >& GetSubsetCullingInfoVec() { return m_vecSubsetCullingInfo; }

	private:
		std::vector< KSubsetCullingInfo > m_vecSubsetCullingInfo;
#endif//X2OPTIMIZE_CULLING_WORLDOBJECTMESH_SUBSET
};//class CKTDXDeviceXMesh	: public CKTDXDevice
