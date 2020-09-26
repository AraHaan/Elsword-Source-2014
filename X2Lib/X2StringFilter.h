#pragma once

class CX2StringFilter
{
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
		
//#ifdef SERV_STRING_FILTER_USING_DB
		enum FILTER_WORD_TYPE
		{
			FWT_NICKNAME = 0,
			FWT_NICKNAMEWORD,
			FWT_WORD,
			FWT_POSTWORD,
		};
//#endif //SERV_STRING_FILTER_USING_DB

		CX2StringFilter();
		virtual ~CX2StringFilter(void);

		bool CheckIsValidString( FILTER_TYPE filterType, const WCHAR* pString );		//��ȿ�� ��Ʈ���ΰ�?
		wstring FilteringChatString( const WCHAR* pString, WCHAR wcharForReplace );
        wstring FilteringNoteString( const WCHAR* pString, WCHAR wcharForReplace );
		//{{ ��ȫ�� [2012.05.16] Ŭ���̾�Ʈ ����ȭ // CLIENT_USE_NATION_FLAG �ʼ� ������
#ifdef UNITED_CLIENT_EU
		std::wstring GetWstrStringFilterFile();
#endif UNITED_CLIENT_EU
		//}}
#ifdef SERV_POST_BAN_WORD_FILTER
		bool CheckIsValidPostWordString( FILTER_TYPE filterType, const WCHAR* pString, OUT wstring &pOutString );	// 2012.09.10 lygan_������ // ���� ���� ��Ģ�� �ɷ����� � ������ �ɸ����� ǥ�ÿ�
		void AddPostWordFilter_LUA( char* szFilter );
#endif //SERV_POST_BAN_WORD_FILTER

		bool CheckIsKoreanNEnglishNNumber( const WCHAR* pString );
#ifdef SEARCH_UNIT_NICKNAME_TW
		bool CheckIsUseableUnicodeCharacterNickname( const WCHAR* pString );
#endif SEARCH_UNIT_NICKNAME_TW

		void	AddNickNameFilter_LUA( char* szFilter );
		//{{ 2009. 2. 17  ������	�г��� ���� ����
		void	AddNickNameWordFilter_LUA( char* szFilter );
		//}}
		void	AddWordFilter_LUA( char* szFilter );
		
	protected:

		//KLuaManager m_KLuaManager;

		vector< wstring > m_BanNickNameList;
		//{{ 2009. 2. 17  ������	�г��� ���� ����
		vector< wstring > m_BanNickNameWordList;
		//}}
		vector< wstring > m_BanWordList;
		//{{ //2012.09.10 lygan_������ // ���� ��� ��Ī ���� �ܾ� ���
#ifdef SERV_POST_BAN_WORD_FILTER
		vector< wstring > m_BanPostWordList;
#endif //SERV_POST_BAN_WORD_FILTER
		//}}
};
