/** @file   KProfileMan.h
    @brief  interface of KProfileMan
    @author seojt@kogstudios.com
    @since  2006-4-17, 17:50:40
*/

#pragma once

#ifndef _KPROFILEMAN_H
#define _KPROFILEMAN_H

//#include "KJohnGen.h"
#include <d3dx9.h> 
#include "KStaticSingleton.h"
#include "jprofile/RHProfile.h"
//#include "KTimer.h"



///-----------------------------------------------------------------------------
/// @class KProfileMan
///
class KProfileMan : public KStaticSingleton<KProfileMan>
{
private:
	RHProfileIterator*  m_pitorRefRoot;
    RHProfileIterator*  m_pitorRef;
    int         m_iNumNodes;    ///< number of current nodes
    int         m_iCursor;      ///< current highlighted node
    float       m_fx;           ///< display left
    float       m_fy;           ///< display top
    float       m_fUpdateInterval;
    float       m_fUpdateTime;

    float       m_fThisTime;
    char        m_buf[1024];
    float       m_fBarScaleFactor;
    float       m_fMaxTime;

	bool		m_bEnableProfile;

	bool		m_bReservedClearRoot;

public:
    KProfileMan();
    ~KProfileMan();

private:
    void _Init();

public:
    void SafeDelete();
    void SetProfileIterator(RHProfileIterator* pitorRef_);
    void SetLeftTop(float fx, float fy) { m_fx = fx; m_fy = fy; }
    void Reset();

    void OnPreFrameMove(float fElapsedTime);
    BOOL OnFrameRender(IDirect3DDevice9* pd3dDevice, float fElapsedTime);
	void OnKeyboardProc(UINT nChar, bool bKeyDown, bool bAltDown);

	// dmlee, 2007/06/12
	void OnCreateDevice( IDirect3DDevice9* pd3dDevice );
	void OnResetDevice( IDirect3DDevice9* pd3dDevice );
	void OnLostDevice();
	void OnDestroyDevice();

};//class KProfileMan

#endif // _KPROFILEMAN_H


/** @example    KProfileMan
    @code

  1) �ʿ��� ���ϵ�:

    jprofile/KBlockProfile.cpp
    jprofile/KBlockProfile.h
    jprofile/KProfile.cpp
    jprofile/KProfile.h
    jprofile/RHProfile.cpp
    jprofile/RHProfile.h

    KJohnGen.h
    KJohnStrUtil.cpp
    KJohnStrUtil.h
    KMessageConsole.cpp
    KMessageConsole.h
    KProfileMan.cpp
    KProfileMan.h
    KStaticSingleton.h
    KTimer.cpp
    KTimer.h

  2) ������Ʈ ����: Project -> Properties -> C/C++ -> Preprocessor�� �Ʒ��� ���ڿ��� ����

    PROJECT_PCH=dxstdafx.h

    ��) WIN32;_DEBUG;_WINDOWS;PROFILE;PROJECT_PCH=dxstdafx.h

  3) include:

    #include "KProfileMan.h"

  4) ���� ����

    RHProfileIterator*  g_pitor = NULL;

  5) OnCreateDevice():

    g_pitor = RHProfileManager::Get_Iterator();
    ASSERT( g_pitor );
    KProfileMan::GetSingleton().SetProfileIterator( g_pitor );

  6) OnFrameMove():

    // �ݵ�� OnFrameMove()�� ���� ó���� ��ġ�ؾ� �Ѵ�.
    // �� �Լ����� OnFrameRender()���̿��� profile�� �����ϴ�.
    //
    KProfileMan::GetSingleton().OnPreFrameMove( fElapsedTime );

  7) OnFrameRender()�� EndScene()�ٷ� ����:

    KProfileMan::GetSingleton().OnFrameRender( pd3dDevice, fElapsedTime );


  8) KeyboardProc():

    KProfileMan::GetSingleton().OnKeyboardProc( nChar, bKeyDown, bAltDown );

  9) OnDestroyDevice():

    ASSERT( g_pitor );
    RHProfileManager::Release_Iterator( g_pitor );
    g_pitor = NULL;
    KProfileMan::GetSingleton().SetProfileIterator( NULL );

  10) �������� �߰�:

    block�� ó���� PROFILE()��ũ�θ� �߰��ϸ�, �Լ��̸��� �������� ��尡 ����� ��
    �̸��� ���ϰ� ������ PROFILE2("�̸�") ������ ��ũ�θ� ����ϸ� ��.

    @endcode
*/
