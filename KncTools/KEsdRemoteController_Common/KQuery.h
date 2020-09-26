#pragma once

#include "KSingleton.h"

#include <sql.h>
#include <sqlext.h>
#include <windows.h>

#include <string>
using namespace std;

#pragma comment ( lib, "odbc32.lib" )

class KQuery
{

public:
	// �ִ� �÷���, BLOB ����� ����, NULL �ʵ尪
	enum
	{
		eMAXCOL		= 100,
		eBLOBBATCH	= 10000,

		eQueryNULL	= -100,
		eQueryEOF	= -101, 
		eQueryNOCOL	= -102,
		eQueryERROR	= -103,
		eQueryOK	= 1
	};

	enum
	{
		eConnectType_MDB = 1,
		eConnectType_SQL,
		eConnectType_SQLDirect,
		eConnectType_Max
	};

protected:
	SQLHENV		m_hEnv;							// ȯ�� �ڵ�
	SQLHDBC		m_hDbc;							// ���� �ڵ�
	SQLRETURN	m_hRet;							// ���� ������ SQL���� �����
	char		m_acCol[eMAXCOL][255];				// ���ε��� �÷� ����

	// �������� ���� ��
	int			m_iType;
	char		m_pcConStr[255];
	char		m_pcUID[255];
	char		m_pcPWD[255];

public:
	SQLINTEGER	m_iAffectCount;					// ������� ���ڵ� ����
	SQLHSTMT	m_hhStmt;						// ��� �ڵ�. ���� ����� ���� �����Ƿ� public���� ����
	SQLSMALLINT m_iCol;							// �÷� ����
	SQLCHAR		m_pcColName[eMAXCOL][50];			// �÷��� �̸���
	SQLINTEGER	m_alCol[eMAXCOL];					// �÷��� ����/���� ����

	KQuery();									// ������
	~KQuery();									// �ı���:���� �ڵ��� �����Ѵ�.

	void PrintDiag();							// ���� ���� ���
	BOOL Connect(int Type, char* ConStr, 
		char* UID = NULL, char* PWD = NULL);	// ������ �ҽ��� �����Ѵ�.
	BOOL Exec(LPCTSTR szSQL);					// SQL���� �����Ѵ�.
	int ExecGetInt(LPCTSTR szSQL, int& iVal);	// SQL���� �����ϰ� ù��° �÷��� ���� ����
	int ExecGetStr(LPCTSTR szSQL, char* buf);	// SQL���� �����ϰ� ù��° �÷��� ���ڿ� ����
	SQLRETURN Fetch();							// �� �� ��������
	void Clear();								// Ŀ�� ���� �� �� ���ε�
	int FindCol(char* name);					// �÷��� �̸����κ��� ��ȣ�� ã���ش�.

	//////////////////////////////////////////////////////////////////////////
	// �÷� �б�
	int GetInt(int nCol, int& iVal);
	int GetInt(char* sCol, int& iVal);
	int GetFloat(int nCol, float& fVal);
	int GetFloat(char* sCol, float& fVal);
	int GetStr(int nCol, char* buf);
	int GetStr(char* sCol, char* buf);
	int GetStr(char* sCol, std::string& strBuf);

	int ReadBlob(LPCTSTR szSQL, void* buf);
	void WriteBlob(LPCTSTR szSQL, void* buf, int size);

	// yyyy-mm-dd �������� ���ڿ��� ��¥ ����ü�� �ִ´�.
	void StrToTime(char* str, SYSTEMTIME* st);
	void GetDate(int nCol, SYSTEMTIME* st);
	void GetDate(char *sCol, SYSTEMTIME* st);

};

KDeclareGlobalVal( KQuery );