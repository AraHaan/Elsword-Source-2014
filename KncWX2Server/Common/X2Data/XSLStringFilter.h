#pragma once

#include "XSLStat.h"

//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	#include "RefreshSingleton.h"
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
//}}

class CXSLStringFilter
{
	//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	DeclareRefreshSingleton( CXSLStringFilter );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;
#else
	DeclareSingleton( CXSLStringFilter );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

public:

	enum FILTER_TYPE
	{
		//FT_WORD,		//�ش� String�ӿ� ������ �ܾ �ִ°�? --"�̷� ������!!"
		//FT_SENTENCE,	//�ش� String�� �ϳ��� �ܾ�� ���� ������ �ִ°�? -- "���"
		FT_NICKNAME = 0,
		FT_CHAT,
//#ifdef SERV_POST_BAN_WORD_FILTER
		FT_POST,
//#endif //SERV_POST_BAN_WORD_FILTER
	};

#ifdef SERV_STRING_FILTER_USING_DB
	enum FILTER_WORD_TYPE
	{
		FWT_NICKNAME = 0,
		FWT_NICKNAMEWORD,
		FWT_WORD,
		FWT_POSTWORD,
	};
#endif //SERV_STRING_FILTER_USING_DB

	CXSLStringFilter();
	virtual ~CXSLStringFilter(void);

	//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
#else
	bool OpenScript( const char* pFileName );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	bool CheckIsValidString( FILTER_TYPE filterType, std::wstring& wstringForCheck );		//��ȿ�� ��Ʈ���ΰ�?
	//std::wstring FilteringChatString( const WCHAR* pString, WCHAR wcharForReplace ); -- ���������� �Ⱦ����ڵ�

	void	AddNickNameFilter_LUA( char* szFilter );
	//{{ 2009. 2. 17  ������	�г��� ���� ����
	void	AddNickNameWordFilter_LUA( char* szFilter );
	//}}
	void	AddWordFilter_LUA( char* szFilter );
	
#ifdef SERV_POST_BAN_WORD_FILTER
	void	AddPostWordFilter_LUA( char* szFilter );
#endif //SERV_POST_BAN_WORD_FILTER

#ifdef SERV_STRING_FILTER_USING_DB
	void	GetStringFilter( OUT StringFilterVector& vecStringFilter );
#endif //SERV_STRING_FILTER_USING_DB

protected:

	inline void MakeUpperCase(std::wstring &str) 
	{
		for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
			*i = (WCHAR)towupper(*i);
	}

	//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
#else
	KLuaManager m_KLuaManager;
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	std::vector< std::wstring > m_BanNickNameList;
	//{{ 2009. 2. 17  ������	�г��� ���� ����
	std::vector< std::wstring > m_BanNickNameWordList;
	//}}
	std::vector< std::wstring > m_BanWordList;
#ifdef SERV_POST_BAN_WORD_FILTER // 2012.09.07 lygan_������ // �������� ���� �ٿ��ֱ�� ���� ��� ��Ī �ܾ� ���͸�
	std::vector< std::wstring > m_BanPostWordList;
#endif //SERV_POST_BAN_WORD_FILTER
};

//{{ 2012. 01. 27	�ڼ���	String Filter �ǽð� ��ũ��Ʈ ����
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	DefRefreshSingletonInline( CXSLStringFilter );
#else
	DefSingletonInline( CXSLStringFilter );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
//}}
