#pragma once

//{{ 2010. 10. 29	������	NetErrorID Ÿ�� ����
typedef int		NetErrorID;	// NetErrorID Ÿ�� ����
//}}

namespace NetError
{

#   undef NET_ERROR
#   define NET_ERROR( id, comment ) id,

#ifdef SERV_SEPARATE_NET_ERROR

#if defined( SERV_COUNTRY_TWHK )
	#include "NetError/NetError_def_TW.h"
#elif defined( SERV_COUNTRY_JP )
	#include "NetError/NetError_def_JP.h"
#elif defined( SERV_COUNTRY_EU )
	#include "NetError/NetError_def.h"
#elif defined( SERV_COUNTRY_US )
	#include "NetError/NetError_def_US.h"
#elif defined( SERV_COUNTRY_CN )
	#include "NetError/NetError_def_CN.h"
#elif defined( SERV_COUNTRY_TH )
	#include "NetError/NetError_def_TH.h"
#elif defined( SERV_COUNTRY_ID )
	#include "NetError/NetError_def_ID.h"
#elif defined( SERV_COUNTRY_BR )
	#include "NetError/NetError_def_BR.h"
#elif defined( SERV_COUNTRY_PH )
	#include "NetError/NetError_def_PH.h"
#elif defined( SERV_COUNTRY_IN )
	#include "NetError/NetError_def_IN.h"
#endif

#else //SERV_SEPARATE_NET_ERROR
	#include "NetError_def.h"
#endif //SERV_SEPARATE_NET_ERROR

	//{{ ��ȫ�� [2012.05.16] Ŭ���̾�Ʈ ����ȭ // CLIENT_USE_NATION_FLAG �ʼ� ������
#ifdef UNITED_CLIENT_EU
	#include "NetError/NetError_def_DE.h"
	#include "NetError/NetError_def_FR.h"
	#include "NetError/NetError_def_IT.h"
	#include "NetError/NetError_def_PL.h"
	#include "NetError/NetError_def_ES.h"
	#include "NetError/NetError_def_EN.h"
#endif UNITED_CLIENT_EU
	//}}
    //enum NETWORK_ERROR 
    //{
        //#include "NetError_def.h"
    //};

	//09. 08. 17 ������ Ŭ���̾�Ʈ���� �ܱ����� �״�� ����
    wchar_t* GetErrStrF( NetErrorID errorID );
	wchar_t* GetErrStr( NetErrorID errorID );

    void SetLastError( NetErrorID errorID );
    wchar_t* GetLastErrMsg();
    NetErrorID GetLastError();
}

#define SET_ERROR( id ) NetError::SetLastError( NetError::##id )