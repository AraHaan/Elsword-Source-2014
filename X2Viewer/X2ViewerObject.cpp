/**	@file	: X2ViewerMesh.h
	@desc	: �� ��ü ����
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- �ʵ� ���� ���� ����
	@date	JHKang on 2013-09-17,
		- ���� ���� �� ���ȭ
*/

#include "dxstdafx.h"
#include ".\x2viewerobject.h"

#pragma region CX2ViewerObject
CX2ViewerObject::CX2ViewerObject(void)
	: m_ObjectStyle( OS_NONE )
{
}
#pragma endregion ������

#pragma region ~CX2ViewerObject
CX2ViewerObject::~CX2ViewerObject(void)
{
	m_ObjectStyle = OS_NONE;
}
#pragma endregion �Ҹ���