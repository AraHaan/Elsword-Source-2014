// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.


//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif 

#include <windows.h>
#include <crtdbg.h>
#include <strsafe.h>

//#ifdef _DEBUG // 2006-1-12, 14:32:55
//#ifndef D3D_DEBUG_INFO
//#define D3D_DEBUG_INFO

//#endif // #ifndef D3D_DEBUG_INFO
//#endif // _DEBUG
#include <d3dx9.h> // KJohnGen.h uses DirectX9 library if required


//#include <d3dx9.h>
//#pragma warning(push)
#pragma warning(disable: 4995)
#include <list>
//#include <string>


#include <vector>
#include <malloc.h>
#pragma warning(push)
#pragma warning(disable: 4276)



#include "KJohnGen.h"
#include "KJohnStrUtil.h"

#include "KTimer.h"
#include "./jprofile/KProfile.h"
#include "./jprofile/KBlockProfile.h"
#include "./jprofile/RHProfile.h"


#include "KMessageConsole.h"


#include "KStaticSingleton.h"

#include "KProfileMan.h"
