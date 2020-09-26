#pragma once


/*
* LineBreak �� cutting ������ ���� ���� ����, �Ǵ� ������ Ư������ ���� ������ �������� ��´�.
*
*/

class CWordLineHandler
{
public:
	CWordLineHandler(void);
	~CWordLineHandler(void);

	//static wstring GetStrByLineBreak( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor, const bool bCutRedundantEmptyLine = true );

	//--------//
	// ���� 1 //
	//--------//
	//static int LineBreakInChatBox( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor );
	//static int LineBreakInDungeonRoom( wstring& wstrText, CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor );
	// �ϰ��� ���� ù��° ���ڿ� 2��° ���� ���� �ٲ� //
	static int LineBreakInX2Main( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor, const bool bCutRedundantEmptyLine = true );

	
	//--------//
	// ���� 2 //
	//--------//
	//static int LineBreakInChatWindow(wstring ChatDataMessage, CKTDGFontManager::CUKFont* pFont, int limitX );
	//static int LineBreakInSlideShot( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
	//static int LineBreakInEventScene( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
	//static int LineBreakInTrainingGameUI( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );

#ifdef LINE_BREAK_IN_SPEECH
	//static int LineBreakInDungeonSpeech( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
#endif



	// �ϴ� ���� �ȵǼ� �ڵ� �����ؿ�
	// �⺻ LineBreak ���ľ� �ҵ�
	//3
	static void LineBreakInTalkBoxManagerImp( CKTDGFontManager::CUKFont* pFont,  D3DXVECTOR2 m_Size, int& nRow, int& iColumnSize, wstring& wstrSpeech, bool bTrade 
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
												, D3DXVECTOR2 m_ResSize
#endif CLIENT_GLOBAL_EU_WORDWRAP
		);

	
	//4
//==============================================//
	// �Լ� �̸� Lien ��Ÿ �־ Line���� ������
	static std::wstring	GetStrByLineBreakInX2Main( const WCHAR* pOrgStr, int width, int fontIndex );
	static std::wstring	BasicLineBreak( const WCHAR* pOrgStr, int width, int fontIndex );
	
	//5
#ifdef FIX_TOOLTIP
	static std::wstring	GetStrByLineBreakColorInX2Main( const WCHAR* pOrgStr, int width, int fontIndex );
#endif FIX_TOOLTIP
	//6
#if defined(ELLIPSE_GLOBAL) || defined(ELLIPSE_CN) //...��� 
	static std::wstring GetStrByLineBreakInX2MainWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse);
#ifdef ELLIPSE_FIX
	static std::wstring CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse, const WCHAR* pEllipseStr = L"..."); //�ܾ������ ¥���� �ʰ� ... ��� ����ϰ� ������. ex ������ ����â
	static std::wstring CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, const WCHAR* pEllipseStr = L"...");
#else // ELLIPSE_FIX
	static std::wstring CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse); //�ܾ������ ¥���� �ʰ� ... ��� ����ϰ� ������. ex ������ ����â
#endif // ELLIPSE_FIX
#endif //defined(ELLIPSE_GLOBAL) || defined(ELLIPSE_CN)

//=============================================//
	//7
	static void ProcessQuestDescInUIQuestNew( const wstring& wstrDesc, const int iLineWidth, CKTDGFontManager::CUKFont* pFont,D3DXVECTOR2 m_vQuestDescSize, vector<wstring>&  m_vecQuestDesc);

	//static void UpdateQuestDescInX2TFieldNpcShop( const wstring& wstrDesc,CKTDGFontManager::CUKFont* pFont, D3DXVECTOR2 m_vQuestDescSize, vector<wstring>&  m_vecQuestDesc);


	//8
	static void LineBreakInX2Community(wstring tempChatMsg, CKTDGFontManager::CUKFont* pFont, wstring chatColor, vector<wstring>& m_vecTalk, CKTDGUIListBox* pListBoxMessenger, bool  bApplyColor, bool bIsSameChatSession);
	
	//9
	static int LineBreakInX2MainMsgBox(wstring& tempText, CKTDGFontManager::CUKFont* pFont, const int constTextMaxLen);

	//10
	static bool MegaLineBreakAdd( wstring wstrText, CKTDGFontManager::CUKFont* pFont, int iTextWidth, CKTDGUIListBox* pListBox , int iMegaID, wstring wstrSenderName);

//=============================================//
	
	static bool IsSpaceLatter( WCHAR wszCharBuf );
	static bool IsSplitLatter( WCHAR wszCharBuf );

//=============================================//
	
	
};
