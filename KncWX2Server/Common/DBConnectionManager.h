#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
#include "odbc/DBUtil.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

//{{ 2012. 1. 9	������		DB���� ��ȣ ����ȭ
#ifdef SERV_DB_CONNECTION_SECURITY	// ���� ������ �ؿ��� �߰�

class KDBConnectionManager
{
public:
    KDBConnectionManager();
    virtual ~KDBConnectionManager(void);

	bool GetDBConnectionInfoFromManagerDB( IN const int iServerType, IN const wchar_t* szConnStr, IN const bool bDBConnStr, OUT std::vector< KDBConnectionInfo >& vecDBConnectionInfo );
	bool DecryptDBString( IN const std::wstring& wstrDBString, OUT std::wstring& wstrDecryptString );

protected:
	KODBC			m_kODBC;
};

#endif SERV_DB_CONNECTION_SECURITY	// ���� ������ �ؿ��� �߰�
//}}