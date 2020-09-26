#pragma once


#define KOGANIMATIONCONTROLLER_FORMAT_MAGIC           (MAKEFOURCC('K','A','C',' '))
#define KOGANIMATIONCONTROLLER_FORMAT_VERSION         0x0100

namespace   KTDXAnimationController
{
    // D3DXLoadMeshHierarchyFromX �Լ��� ���� ������ ID3DXAnimationController�� ������ ��ü�������� �����ϴ� �Լ�
    // �Ϲ����� ID3DXAnimationController ���� ����� �� ����.
    HRESULT WriteToFileSerializer( KFileSerializer& kInOutFile_, const CKTDXDeviceXSkinMesh* pInSkinMesh_, LPD3DXANIMATIONCONTROLLER pInAnimCon_ );

    LPD3DXANIMATIONCONTROLLER   CloneOptimizedAC( const CKTDXDeviceXSkinMesh* pInSkinMesh_, LPD3DXANIMATIONCONTROLLER pInOutAnimCon_ );

    HRESULT ReadFromMemory( DWORD dwInSize_, const void* pInData_, CKTDXDeviceXSkinMesh::XSkinMeshProxy& proxy_, LPD3DXANIMATIONCONTROLLER* ppInAC_ );

}//namespace   KTDXAnimationController
