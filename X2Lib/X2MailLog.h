#pragma once

/* ������ // 2013-08-09
*/
#ifdef NEW_MAIL_LOG
class CX2MailLogManager
{
public:
	enum MAIL_LOG_ID
	{
		MLI_CLOSE_PARTY_BUG = 0,     // ���������� ���� ��Ƽ Ż�� �̷������ �� ���� �߼�
		MLI_MASS_FILE_DUPLICATION,	 // ���ҽ� �ߺ� ���Ϸ� ���� ���� ���� ���� �� ���� �߼�
		MLI_USE_GAME_EDIT,			 // �������� ġƮ ��� �� ���� �߼�
		MLI_SERVER_TIME_OUT,		 // ��Ŷ ���� ���� ������ �� �α�
		MLI_SEARCH_CHECK_KOM_FILTER, // ���������� Kom���� �߰ߵ��� �� ���� �߼�
	};

public:
	static CX2MailLogManager* GetInstance();
	static void ResetMailLogManager();
	static void DestroyInstance();

	void AddMailLog( MAIL_LOG_ID eMailLogID, const WCHAR* pLog )
	{
		std::string strLog;
		ConvertWCHARToChar( strLog, pLog );
		return AddMailLog( eMailLogID, strLog.c_str() );
	}
	void AddMailLog( MAIL_LOG_ID eMailLogID, const CHAR* pLog );
	void SendMail();

private:
	string GetSubjectString( MAIL_LOG_ID eMailLogID_ );
private:
	CX2MailLogManager();
	~CX2MailLogManager();
	
	static CX2MailLogManager* m_pMailLog;

	CX2SMTPMail* m_pX2SMTPMail;

	// MAIL_LOG_ID, �α� ���ڿ�
	std::map< MAIL_LOG_ID, string > m_mapLogList;

	bool m_bIsSending;  // ���� ���� ����
};

#endif // NEW_MAIL_LOG

