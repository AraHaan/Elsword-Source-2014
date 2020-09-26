#include "stdafx.h"

#include "KQuery.h"

KImplementGlobalVal( KQuery );

///////////////////////////////////////////////////////////////////////////////
/*
KQuery Ŭ����:ODBC�� ���� Ŭ����. API ���ؿ��� ���� �����ϵ��� �ۼ��Ͽ���.
SQL���� ���ϰ� �����ϰ� ����� ���� �дµ� ������ ���߾����� SQL ������ 
�׼������� �Ϻ��ϰ� �׽�Ʈ�Ǿ���.
255�� ������ ����, ���ڿ� �÷��� ���ؼ��� ��� �����ϸ� �� �̻��� ���̸� ��
���� �ʵ�� "������ �߸�" ���� ��� �߻��ϹǷ� �� Ŭ������ ���� �� ������
ODBC API �Լ��� ���� ����ؾ� �Ѵ�.
�ִ� �÷� ������ 100���̴�. �ʰ��� �������� ���� �޽��� �ڽ��� ����ϹǷ� �� 
�ѵ��� �˾Ƽ� ���� �ʵ��� �����ؾ� �Ѵ�. Ư�� �ʵ���� ���� ���̺��� select *
�� �д� ���� �ﰡ�ϴ� ���� ����.
���� ���� Ŀ���� ����ϹǷ� �̹� ���� ���ڵ�� ������ �ٽ� �����ؾ߸� ���� ��
�ִ�. 

����
1.KQuery Ŭ������ ��ü�� �����Ѵ�. �������̸� �������� �����ϴ� ���� ������
  �ʿ��� ����ŭ ��ü�� �����. ���� �� �� ������ ����ϴ�.
2.������ Connect �Լ��� ȣ���Ͽ� ������ �ҽ��� �����Ѵ�.
3.Exec �Լ��� SQL���� �����Ѵ�. ���� ó���� Exec �Լ� ���ο��� ó���Ѵ�.
4.Fetch �Լ��� ��� �� �ϳ��� �����´�. �������� ������� �ִ� ���� while
  ������ ���鼭 ���ʴ�� ������ �� �ִ�.
5.Get* �Լ��� �÷� ���� �д´�.
6.Clear �Լ��� ��� �ڵ��� �ݴ´�.(Select���� ��츸)
7.��ü�� �ı��Ѵ�. ���� ��ü�� ���� �Ϻη� �ı��� �ʿ䰡 ����.
*/

//-----------------------------------------------------------------------------
// ������:���� �ʱ�ȭ�� ����Ѵ�.
KQuery::KQuery()
{
	m_iAffectCount = -1; 
	m_hRet = SQL_SUCCESS; 
	char str[256] = { 0 };

	// ȯ�� �ڵ��� �Ҵ��ϰ� ���� �Ӽ��� �����Ѵ�.
	m_hRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	if (m_hRet != SQL_SUCCESS)
	{
		wsprintfA(str, "m_hRet : %d", (LPCTSTR) m_hRet);
		::MessageBoxA(NULL, str, "�ʱ�ȭ, SQLAllocHandle() ����!", 0); 
		return;
	}

	m_hRet = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER); 
	if (m_hRet != SQL_SUCCESS)
	{
		wsprintfA(str, "m_hRet : %d", (LPCTSTR) m_hRet);
		::MessageBoxA(NULL, str, "�ʱ�ȭ, SQLSetEnvAttr() ����!", 0); 
		return;
	}
}
//-----------------------------------------------------------------------------
// �ı���:���� �ڵ��� �����Ѵ�.
KQuery::~KQuery()
{
	if (m_hhStmt) SQLFreeHandle(SQL_HANDLE_STMT, m_hhStmt); 
	if (m_hDbc) SQLDisconnect(m_hDbc); 
	if (m_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc); 
	if (m_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv); 
}
//-----------------------------------------------------------------------------
// ���� �ڵ��� �Ҵ��ϰ� ������ �� ����ڵ���� ���� �Ҵ��Ѵ�.
// Type = 1:ConStr�� MDB ������ ��θ� ������. ��� ������ ���� ���丮���� MDB�� ã�´�.
// Type = 2:ConStr�� SQL ������ ���� ������ ������ DSN ������ ��θ� ������. 
//        ��δ� �ݵ�� ���� ��η� �����ؾ� �Ѵ�.
// Type = 3:SQLConnect �Լ��� DSN�� ���� �����Ѵ�.
// ���� �Ǵ� ��� �ڵ� �Ҵ翡 �����ϸ� FALSE�� �����Ѵ�.
BOOL KQuery::Connect(int Type, char* ConStr, char* UID, char* PWD)
{
	SQLCHAR InCon[255] = { 0 };
	SQLCHAR OutCon[255] = { 0 };
    SQLSMALLINT cbOutCon; 

	int ii = 1; 
	//SQLRETURN Ret; 
	//SQLINTEGER NativeError; 
	SQLCHAR SqlState[6] = { 0 };
	SQLCHAR Msg[255] = { 0 };
	//SQLSMALLINT MsgLen; 
	char str[256] = { 0 };

	// ���� Ÿ�Կ� ���� MDB �Ǵ� SQL ����, �Ǵ� DSN�� �����Ѵ�.
	m_hRet = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc); 
	if (m_hRet != SQL_SUCCESS)
	{
		wsprintfA(str, "m_hRet : %d", (LPCTSTR) m_hRet);
		::MessageBoxA(NULL, str, "Connect, SQLAllocHandle() ����!", 0); 
	}

	switch (Type)
	{
	case eConnectType_MDB:
		wsprintfA((char*) InCon, "DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;", ConStr); 
		m_hRet = SQLDriverConnectA(m_hDbc, NULL, (SQLCHAR*) InCon, sizeof(InCon), OutCon, 
			sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT); 
		break; 
	case eConnectType_SQL:
		wsprintfA((char*) InCon, "FILEDSN=%s", ConStr); 
		m_hRet = SQLDriverConnectA(m_hDbc, NULL, (SQLCHAR*) InCon, sizeof(InCon), OutCon, 
			sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT); 
		break; 
	case eConnectType_SQLDirect:
		wsprintfA((char*) InCon, "%s", ConStr);
		m_hRet = SQLDriverConnectA(m_hDbc, NULL, (SQLCHAR*) InCon, sizeof(InCon), OutCon, 
			sizeof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
		break; 
	}

	// ���� ������ ���� ������ �����ش�.
	if ((m_hRet != SQL_SUCCESS) && (m_hRet != SQL_SUCCESS_WITH_INFO))
	{
		wsprintfA(str, "m_hRet : %d", (LPCTSTR) m_hRet); 
		::MessageBoxA(NULL, str, "KQuery::Connect() ����!", 0); 
		return FALSE; 
	}

	// ��� �ڵ��� �Ҵ��Ѵ�.
	m_hRet = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hhStmt); 
	if ((m_hRet != SQL_SUCCESS) && (m_hRet != SQL_SUCCESS_WITH_INFO)) 
	{
		m_hhStmt = 0; 
		return FALSE; 
	}

	// �������� ���� ���� ������ ����� ������ ���´�.
	this->m_iType = Type; 
	lstrcpyA(this->m_pcConStr, ConStr); 
	if (UID)
		lstrcpyA(this->m_pcUID, UID); 
	if (PWD)
		lstrcpyA(this->m_pcPWD, PWD); 
	return TRUE; 
}
//-----------------------------------------------------------------------------
// SQL���� �����Ѵ�. ���н� ���� ������ ����ϰ� FALSE�� �����Ѵ�.
BOOL KQuery::Exec(LPCTSTR szSQL)
{
#ifdef DEBUG
	KDbgStm.SetLogFilename("ServiceInfoLog.txt"); 
	KDbgStm << "Call KQuery::Exec() - " << szSQL << fileout; 
#endif

	int c; 

	// SQL���� �����Ѵ�. SQL_NO_DATA�� ������ ��쵵 �ϴ� �������� ����Ѵ�. 
	// �� ��� Fetch���� EOF�� �����ϵ��� �Ǿ� �ֱ� ������ ���� ������ ����� �ʿ�� ����.
	m_hRet = SQLExecDirect(m_hhStmt, (SQLWCHAR*) szSQL, SQL_NTS); 
	if ((m_hRet != SQL_SUCCESS) && (m_hRet != SQL_SUCCESS_WITH_INFO) && (m_hRet != SQL_NO_DATA)) 
	{
		PrintDiag(); 
		return FALSE; 
	}

	// Update, Delete, Insert ��ɽ� ������� ���ڵ� ������ ���� ���´�.
	SQLRowCount(m_hhStmt, &m_iAffectCount); 

	SQLNumResultCols(m_hhStmt, &m_iCol); 
	if (m_iCol > eMAXCOL) 
	{
		::MessageBoxA(NULL, "�ִ� �÷� ���� �ʰ��߽��ϴ�", "KQuery ����", MB_OK); 
		return FALSE; 
	}

	// nCol�� 0�� ���� Select�� �̿��� �ٸ� ����� ������ ����̹Ƿ� 
	// ���ε��� �� �ʿ䰡 ����.
	if (m_iCol == 0) 
	{
		Clear(); 
		return TRUE; 
	}

	// ��� �÷��� ���ڿ��� ���ε��� ���´�. Col�迭�� zero base, 
	// �÷� ��ȣ�� one base�ӿ� ������ ��
	for (c = 0; c < m_iCol; c++) 
	{
		SQLBindCol(m_hhStmt, c+1, SQL_C_CHAR, m_acCol[c], 255, &m_alCol[c]); 
		SQLDescribeColA(m_hhStmt, c+1, m_pcColName[c], 30, NULL, NULL, NULL, NULL, NULL); 
	}
	return TRUE; 
}
//-----------------------------------------------------------------------------
// SQL���� �����ϰ� ������� ù Į������ �������� �о� ������ �ش�. 
// ����� ���� �� �������� �� �ش�.
int KQuery::ExecGetInt(LPCTSTR szSQL, int& iVal)
{
	if (Exec(szSQL) == FALSE) 
	{
		return eQueryERROR; 
	}

	// �����Ͱ� ���� ��� EOF�� �����Ѵ�.
	if (Fetch() == SQL_NO_DATA)
	{
		Clear(); 
		return eQueryEOF; 
	}

	GetInt(1, iVal); 
	Clear(); 
	return eQueryOK; 
}
//-----------------------------------------------------------------------------
// SQL���� �����ϰ� ������� ù Į������ ���ڿ��� �о� ������ �ش�.
int KQuery::ExecGetStr(LPCTSTR szSQL, char* buf)
{
	// SQL�� ������ ������ �߻��� ��� ���ڿ� ���ۿ� ������ ���� ������.
	if (Exec(szSQL) == FALSE)
	{
		return eQueryERROR; 
	}

	// �����Ͱ� ���� ��� EOF�� �����Ѵ�.
	if (Fetch() == SQL_NO_DATA)
	{
		Clear(); 
		return eQueryEOF; 
	}

	GetStr(1, buf); 
	Clear(); 
	return eQueryOK; 
}
//-----------------------------------------------------------------------------
// ����¿��� �� ���� �����´�.
SQLRETURN KQuery::Fetch()
{
	m_hRet = SQLFetch(m_hhStmt); 
	return m_hRet; 
}
//-----------------------------------------------------------------------------
// Ŀ���� �ݰ� ���ε� ������ �����Ѵ�.
void KQuery::Clear()
{
	SQLCloseCursor(m_hhStmt); 
	SQLFreeStmt(m_hhStmt, SQL_UNBIND); 
}
//-----------------------------------------------------------------------------
// �÷� �̸����κ��� �÷� �ε����� ã�´�. ���� ��� -1�� �����Ѵ�.
int KQuery::FindCol(char* name)
{
	int i; 
	for (i = 0; i < m_iCol; i++)
	{
		if (stricmp(name, (const char*) m_pcColName[i]) == 0)
			return i+1; 
	}
	return -1; 
}
//-----------------------------------------------------------------------------
// nCol�� �÷����� ������ �о��ش�. NULL�� ��� KQueryNULL�� �����Ѵ�.
int KQuery::GetInt(int nCol, int& iVal)
{
	if (nCol > this->m_iCol)
		return eQueryNOCOL; 

	if (m_alCol[nCol-1] == SQL_NULL_DATA)
	{
		return eQueryNULL; 
	}
	else
	{
		iVal = atoi(m_acCol[nCol-1]); 
		return eQueryOK; 
	}
}
//-----------------------------------------------------------------------------
// sCol�� �÷����� ������ �о��ش�.
int KQuery::GetInt(char* sCol, int& iVal)
{
	int n; 
	n = FindCol(sCol); 
	if (n == -1)
	{
		return eQueryNOCOL; 
	}

	return GetInt(n, iVal); 
}
//-----------------------------------------------------------------------------
// nCol�� �÷����� �Ǽ��� �о��ش�. NULL�� ��� KQueryNULL�� �����Ѵ�.
int KQuery::GetFloat(int nCol, float& fVal)
{
	if (nCol > this->m_iCol)
		return eQueryNOCOL; 

	if (m_alCol[nCol-1] == SQL_NULL_DATA)
	{
		return eQueryNULL; 
	}
	else
	{
		fVal = (float) atof(m_acCol[nCol-1]); 
		return eQueryOK; 
	}
}
//-----------------------------------------------------------------------------
// sCol�� �÷����� �Ǽ��� �о��ش�.
int KQuery::GetFloat(char* sCol, float& fVal)
{
	int n; 
	n = FindCol(sCol); 
	if (n == -1)
	{
		return eQueryNOCOL; 
	}

	return GetFloat(n, fVal); 
}
//-----------------------------------------------------------------------------
// nCol�� �÷����� ���ڿ��� �о��ش�. NULL�� ��� ���ڿ��� NULL�� ä���ش�. 
// buf�� ���̴� �ּ��� 256�̾�� �ϸ� ���� ������ ���� �ʴ´�.
int KQuery::GetStr(int nCol, char* buf)
{
	if (nCol > this->m_iCol)
	{
		return eQueryNOCOL; 
	}

	if (m_alCol[nCol-1] == SQL_NULL_DATA)
	{
		return eQueryNULL; 
	}
	else
	{
		lstrcpyA(buf, m_acCol[nCol-1]); 
		return eQueryOK; 
	}
}
//-----------------------------------------------------------------------------
// sCol�� �÷����� ���ڿ��� �о��ش�.
int KQuery::GetStr(char* sCol, char* buf)
{
	int n; 
	n = FindCol(sCol); 
	if (n == -1)
	{
		return eQueryNOCOL; 
	}

	return GetStr(n, buf); 
}
//-----------------------------------------------------------------------------
// ���ڿ��� �÷� �б�
int KQuery::GetStr(char* sCol, std::string& strBuf)
{
	int nCol; 
	nCol = FindCol(sCol); 
	if (nCol == -1)
	{
		return eQueryNOCOL; 
	}
	else
	{
		if (nCol > this->m_iCol)
		{
			return eQueryNOCOL; 
		}

		if (m_alCol[nCol-1] == SQL_NULL_DATA)
		{
			return eQueryNULL; 
		}
	}

	strBuf = m_acCol[nCol-1]; 
	return eQueryOK; 
}
//-----------------------------------------------------------------------------
// ���� �߻��� ���� ������ ����� �ش�.
void KQuery::PrintDiag()
{
	int ii; 
	SQLRETURN Ret; 
	SQLINTEGER NativeError; 
	SQLCHAR SqlState[6], Msg[255]; 
	SQLSMALLINT MsgLen; 
	char str[256]; 

	ii = 1; 
	while (Ret = SQLGetDiagRecA(SQL_HANDLE_STMT, m_hhStmt, ii, SqlState, &NativeError, 
		Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		wsprintfA(str, "SQLSTATE:%s, ��������:%s", (LPCTSTR) SqlState, (LPCTSTR) Msg); 
		::MessageBoxA(NULL, str, "���� ����", 0); 
		ii++; 
	}

	// ���� �Ұ����� ������ �߻��� ��� ���α׷��� �����Ѵ�. �ش��� ���� ó���� ���ʿ���
	// ���� �� �ڵ带 �ּ� ó���ϰų� �����ϰ� �ٸ� ��ƾ���� �ٲ�� �Ѵ�.
	if (ii > 1) 
	{
		::MessageBoxA(NULL, "���� ������ ��µǾ����ϴ�. �� ������ ��µ� ���� ��Ʈ�� ����, DB ���� ���� ����\r\n"
			"���� �Ұ����� ������ �߻��� ����̸� ���α׷� ������ ����� �� �����ϴ�.\r\n"
			"������ �����Ͻ� �� ���α׷��� �ٽ� ������ �ֽʽÿ�.", "Critical Error", MB_OK | MB_ICONERROR); 

		// ���� �� �� �ϳ��� ������ ��
		PostQuitMessage(0); 
		// ExitProcess(0); 
	}
}
//-----------------------------------------------------------------------------
// BLOB �����͸� buf�� ä���ش�. �̶� buf�� ����� ũ���� �޸𸮸� �̸� �Ҵ��� 
// ���ƾ� �Ѵ�. NULL�� ��� 0�� �����ϰ� ���� �߻��� -1�� �����Ѵ�. ������ ���� 
// �� ����Ʈ ���� �����Ѵ�. szSQL�� �ϳ��� BLOB �ʵ带 �д� Select SQL���̾�� �Ѵ�.
int KQuery::ReadBlob(LPCTSTR szSQL, void* buf)
{
	SQLCHAR BinaryPtr[eBLOBBATCH]; 
	SQLINTEGER LenBin; 
	char *p; 
	int nGet; 
	int TotalGet = 0; 

	m_hRet = SQLExecDirect(m_hhStmt, (SQLWCHAR*) szSQL, SQL_NTS); 
	if (m_hRet != SQL_SUCCESS) 
	{
		PrintDiag(); 
		return -1; 
	}

	while ((m_hRet = SQLFetch(m_hhStmt)) != SQL_NO_DATA) 
	{
		p = (char*) buf; 
		while ((m_hRet = SQLGetData(m_hhStmt, 1, SQL_C_BINARY, BinaryPtr, sizeof(BinaryPtr), 
			&LenBin)) != SQL_NO_DATA) 
		{
			if (LenBin == SQL_NULL_DATA) 
			{
				Clear(); 
				return 0; 
			}
			if (m_hRet == SQL_SUCCESS)
				nGet = LenBin; 
			else
				nGet = eBLOBBATCH; 
			TotalGet += nGet; 
			memcpy(p, BinaryPtr, nGet); 
			p += nGet; 
		}
	}
	Clear(); 
	return TotalGet; 
}
//-----------------------------------------------------------------------------
// buf�� BLOB �����͸� �����Ѵ�. szSQL�� �ϳ��� BLOB �����͸� �����ϴ� Update, Insert
// SQL���̾�� �Ѵ�.
void KQuery::WriteBlob(LPCTSTR szSQL, void* buf, int size)
{
	SQLINTEGER cbBlob; 
	char tmp[eBLOBBATCH], *p; 
	SQLPOINTER pToken; 
	int nPut; 

	cbBlob = SQL_LEN_DATA_AT_EXEC(size); 
	SQLBindParameter(m_hhStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_LONGVARBINARY, 
		size, 0, (SQLPOINTER)1, 0, &cbBlob); 
	SQLExecDirect(m_hhStmt, (SQLWCHAR*) szSQL, SQL_NTS); 
	m_hRet = SQLParamData(m_hhStmt, &pToken); 
	while (m_hRet == SQL_NEED_DATA)
	{
		if (m_hRet == SQL_NEED_DATA) 
		{
			if ( reinterpret_cast<int>(pToken) == 1)
			{
				for (p = (char*) buf; p < (char*) buf+size; p += eBLOBBATCH) 
				{
					nPut = min(eBLOBBATCH, (char*) buf+size-p); 
					memcpy(tmp, p, nPut); 
					SQLPutData(m_hhStmt, (PTR)tmp, nPut); 
				}
			}
		}
		m_hRet = SQLParamData(m_hhStmt, &pToken); 
	}
	Clear(); 
}
//-----------------------------------------------------------------------------
void KQuery::StrToTime(char* str, SYSTEMTIME* st)
{
	char Year[5]	 = {0, }; 
	char Month[3]	 = {0, }; 
	char Day[3]		 = {0, }; 
	char Hour[3]	 = {0, };
	char Minute[3]	 = {0, };
	char Second[3]	 = {0, };
	
	// ��¥������ �ƴ� ��� ���� ó��
	if(strlen(str) < 10)
		return; 
	
	// ��¥���� �����ϹǷ� �ð������� ���½��� �־�� �Ѵ�.
	// �� �׷��� ��û�� DTP ��Ʈ���� ��¥�� ��Ʈ�� ���� �ʴ´�.
	memset(st, 0, sizeof(SYSTEMTIME)); 
	strncpy(Year, str, 4); 
	strncpy(Month, str+5, 2); 
	strncpy(Day, str+8, 2);
	strncpy(Hour, str+11, 2);
	strncpy(Minute, str+14, 2);
	strncpy(Second, str+17, 2);

	st->wYear = atoi(Year); 
	st->wMonth = atoi(Month); 
	st->wDay = atoi(Day);
	st->wHour = atoi(Hour);
	st->wMinute = atoi(Minute);
	st->wSecond = atoi(Second);
}
//-----------------------------------------------------------------------------
void KQuery::GetDate(int nCol, SYSTEMTIME* st)
{
	if (nCol > this->m_iCol) 
	{
		memset(st, 0, sizeof(SYSTEMTIME)); 
		return; 
	}

	if (m_alCol[nCol-1] == SQL_NULL_DATA) 
	{
		memset(st, 0, sizeof(SYSTEMTIME)); 
	}
	else
	{
		StrToTime(m_acCol[nCol-1], st); 
	}
}
//-----------------------------------------------------------------------------
void KQuery::GetDate(char* sCol, SYSTEMTIME* st)
{
	int n; 
	n = FindCol(sCol); 
	if (n == -1) 
	{
		memset(st, 0, sizeof(SYSTEMTIME)); 
	}
	else
	{
		GetDate(n, st); 
	}
}
//-----------------------------------------------------------------------------
