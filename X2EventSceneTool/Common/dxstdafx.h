//--------------------------------------------------------------------------------------
// File: DxStdAfx.h
//
// Desc: Standard includes and precompiled headers for DXUT
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef DXSDK_STDAFX_H
#define DXSDK_STDAFX_H

//////////////////////////////////////////////////////////////////////////
//## KTDX Engine begin
//## Desc : ������ ����ϰ� �� ���� ���̺귯�� �߰�.

#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4311)

//#include "KTDX.h"
#include "X2Main.h"

#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4311)

//## KTDX Engine End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//## User source begin
//## Desc : ����� �ڵ� ����.. 
#pragma NOTE( "Inhouse3.h �� �ִ� #define EVENT_SCENE | EVENT_SCENE_TOOL �� �ּ����� �Ǿ� �־�� ��" )

#define ERRORMSG(str)	MessageBox( NULL, str, L"error", MB_ICONERROR | MB_OK);

#include "..\CommonObject.hpp"
#include "..\EventScriptParsing.h"
#include "..\RenderFrm.h"
#include "..\DlgProc.h"

extern CRenderFrm * g_pRenderFrm;

inline CRenderFrm * GetRenderFrm(void)
{
	if( g_pRenderFrm == NULL )
	{
		//assert(g_pRenderFrm == NULL);
		g_pRenderFrm = new CRenderFrm();
	}

	return g_pRenderFrm;
}

#define MAX_SCRIPT_BUFFER 5048
//## User source end
//////////////////////////////////////////////////////////////////////////

extern CDXUTDialogResourceManager g_DialogResourceManager;

#endif // !defined(DXSDK_STDAFX_H)
