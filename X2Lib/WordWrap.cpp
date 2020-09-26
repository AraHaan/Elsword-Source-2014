#include "StdAfx.h"
//{{ 2010/10/27 ��ȿ��	�ϴ� �ӽ÷� WordWrap include ���⿡�� ��
#include "WordWrap.h"
//}}


CWordLineHandler::CWordLineHandler(void)
{
}

CWordLineHandler::~CWordLineHandler(void)
{
}

//---------------------------------------------------------//
/*wstring CWordLineHandler::GetStrByLineBreak( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor, const bool bCutRedundantEmptyLine )
{
	wstring wstrCopiedText = wstrText;

	LineBreakInX2Main( wstrCopiedText, pFont, iTextWidth, wstrTextColor, bEnableColor, bCutRedundantEmptyLine  );

	return wstrCopiedText;
}
*/
//---------------------------------------------------------//
/*int CWordLineHandler::LineBreakInChatBox( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor )
{
	KTDXPROFILE();

	if( NULL == pFont )
		return 0;	// return 0./	// � �Լ��� �����ϳ� ���� 1�� ��


	// pFont �� NULL �� ���� bool bCutRedundantEmptyLine �� false �� ��Ȳ  �� �����ϰ�� �Լ� �� ����
	bool bCutRedundantEmptyLine = true;
	return LineBreakInX2Main( wstrText, pFont, iTextWidth, wstrTextColor, bEnableColor, bCutRedundantEmptyLine  );
}*/

/*int CWordLineHandler::LineBreakInDungeonRoom( wstring& wstrText, CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor )
{
	KTDXPROFILE();

	if( NULL == pFont )
		return 0;	// return 0./	// � �Լ��� �����ϳ� ���� 1�� ��


	// pFont �� NULL �� ���� bool bCutRedundantEmptyLine �� false �� ��Ȳ  �� �����ϰ�� �Լ� �� ����
	bool bCutRedundantEmptyLine = true;
	return LineBreakInX2Main( wstrText, pFont, iTextWidth, wstrTextColor, bEnableColor, bCutRedundantEmptyLine  );
}*/


int CWordLineHandler::LineBreakInX2Main( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor, const bool bCutRedundantEmptyLine  )
{
	KTDXPROFILE();

	if( NULL == pFont )
		return -1;		//LineBreakInChatBox �� �����ϳ� �̺κ��� �� 1����?

	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};


	
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;
#endif CLIENT_GLOBAL_EU_WORDWRAP

	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowLineCount = 0;
	int iNowCharIndex = 0;
	wstring wstrLineBuf = L"";
	wstring wstrColorCode = L"";


	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 



	if( false == wstrTextColor.empty() )
	{
		wstrLineBuf += wstrTextColor;
	}

	while( iNowCharIndex < (int) wstrText.length() )
	{
		WCHAR wszCharBuf = wstrText[iNowCharIndex];
		WCHAR wszNextCharBuf = L'';
		
		if( (iNowCharIndex+1) < (int) wstrText.length() )   
			wszNextCharBuf = wstrText[iNowCharIndex+1];


		wstring wstrCharBuf = wstrText.substr( iNowCharIndex, 1 );

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					if( true == bEnableColor )
					{
						wstrLineBuf += wstrCharBuf;
						eNowCharState = Q_SHARP;
					}
					else
					{
						wstrLineBuf += L"*"; 
						iNowCharWidth = pFont->GetWidth( L"*" );
						iNowLineWidth += iNowCharWidth;
					}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					// �� �ٲ�� ���� �κе� Ŀ�� ����Ʈ, �� ���� ���ϸ� #�� Ŀ�� ����Ʈ					
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					// 2010/10/27 ��ȿ�� ���Ⱑ �ٸ�... bCutRedundantEmptyLine ��� ���� �� �ʿ伺... 
					// 09.05.30 �¿� : 2�� �̻��� \n�� �������� ���� ��� �߸��� ���� ����
					if( bIsFirstCharOfLine == false || bCutRedundantEmptyLine == false )
					{
						wstrLineBuf += wstrCharBuf;
						//wstrLineBuf += L" ";	// ���� �̺κ� �ּ� ó�� �� ����...
						iNowLineCount++;
						iNowLineWidth = 0;
						bIsFirstCharOfLine = true;

						if( 0 != wstrTextColor.length() )
						{
							wstrLineBuf += wstrTextColor;
						}

						if( 0 != wstrColorCode.length() )
						{
							wstrLineBuf += wstrColorCode;
						}
					}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					//
#endif CLIENT_GLOBAL_EU_WORDWRAP

				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;					
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
						//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
						if( IsSpaceLatter(wszCharBuf) )
						{
							iLastCutPoint = wstrLineBuf.length();
							iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
						}
						else if ( IsSplitLatter(wszCharBuf) && (IsSpaceLatter(wszNextCharBuf) || IsSplitLatter(wszNextCharBuf)) )
						{
							// Ư���� ��쿡�� ���� ���ڵ� ������ �����ؾ� �������� �����Ѵ�. //
							iLastCutPoint = wstrLineBuf.length();
							iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
						}
						else
						{
							iLastCharWidth = iNowCharWidth;
							iNowLineWidthFromLastCurPoint += iNowCharWidth;
						}
						//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP

					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else
				{
					if( true == bIsFirstCharOfLine )
						bIsFirstCharOfLine = false;

					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					if( IsSpaceLatter(wszCharBuf) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else if ( IsSplitLatter(wszCharBuf) && (IsSpaceLatter(wszNextCharBuf) || IsSplitLatter(wszNextCharBuf)) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else
					{
						iLastCharWidth = iNowCharWidth;
						iNowLineWidthFromLastCurPoint += iNowCharWidth;
					}
					//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP

				}
			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrColorCode = L"";

					if( 0 != wstrTextColor.length() )
					{
						wstrLineBuf += wstrTextColor;
					}

					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					// note!! color code�� �ùٸ��� üũ�ϴ� �ڵ� �־�� ��. �ϴ� color code�� ������ �ùٸ������� ����
					wstrColorCode = L"#C";
					wstrColorCode += wstrText.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrText.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
				//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
				// �� �ٲ�� ������ �׻� Ŀ�� ����Ʈ�̴�.
				iLastCutPoint = wstrLineBuf.length();
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
				//
#endif CLIENT_GLOBAL_EU_WORDWRAP

			} break;
		}




		if( iNowLineWidth >= iTextWidth && Q_NORMAL == eNowCharState )
		{
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			// �� ��� �ش� ������ ��� �ڸ� ����Ʈ�� ���� ����̴�. 
			if( iNowLineWidth <= iNowLineWidthFromLastCurPoint )
			{
				
				wstring wsTemp = L"-\n";

				if( 0 != wstrTextColor.length() )
				{
					wsTemp += wstrTextColor;
				}
				if( 0 != wstrColorCode.length() )
				{
					wsTemp += wstrColorCode;
				}

				
				if( wstrLineBuf.length() != 0 )
				{
					// �� ��� ������ ���� �տ� ������ �־��ش�. //
					wstring::iterator tmp_it = wstrLineBuf.end() -1;
					wstrLineBuf.insert(tmp_it, wsTemp.begin(), wsTemp.end());
				}

				// iNowLineWidth �� iNowLineWidthFromLastCurPoint ��ġȭ
				iNowLineWidth = iLastCharWidth;
				iNowLineWidthFromLastCurPoint = iLastCharWidth;

			}
			//
			else
			{
				wstring wsTemp = L"\n";

				if( 0 != wstrTextColor.length() )
				{
					wsTemp += wstrTextColor;
				}
				if( 0 != wstrColorCode.length() )
				{
					wsTemp += wstrColorCode;
				}

				// end()�����ؼ� == �� ������ insert ������
				if( (int)wstrLineBuf.length() >= iLastCutPoint )
				{
					wstring::iterator tmp_it = wstrLineBuf.begin() + iLastCutPoint;
					wstrLineBuf.insert(tmp_it, wsTemp.begin(), wsTemp.end());
				}

				// iNowLineWidth �� iNowLineWidthFromLastCurPoint ��ġȭ
				iNowLineWidth = iNowLineWidthFromLastCurPoint;
			}

			iNowLineCount++;

			// �� ���� ù��° ���ڰ� �ִ� ��Ȳ�̴�. 
			bIsFirstCharOfLine = false;
			
			
			//}}
#else CLIENT_GLOBAL_EU_WORDWRAP

			wstrLineBuf += L"\n";
			//wstrLineBuf += L" ";
			iNowLineCount++;
			iNowLineWidth = 0;
			bIsFirstCharOfLine = true;

			if( 0 != wstrTextColor.length() )
			{
				wstrLineBuf += wstrTextColor;
			}

			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}
#endif CLIENT_GLOBAL_EU_WORDWRAP
		}

		iNowCharIndex++;
	}


	wstrText = wstrLineBuf;

	return iNowLineCount;
}

//// �ѱ��� WordWrap
// int CWordLineHandler::LineBreakInX2Main( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor, const bool bCutRedundantEmptyLine  )
// {
// 	KTDXPROFILE();
// 
// 	if( NULL == pFont )
// 		return 1;		//LineBreakInChatBox �� �����ϳ� �̺κ��� �� 1����?
// 
// 	enum CHAR_STATE
// 	{
// 		Q_NORMAL,
// 		Q_SHARP,
// 		Q_SHARP_C,
// 	};
// 
// 
// 	int iNowCharWidth = 0;		
// 	int iNowLineWidth = 0;
// 	int iNowLineCount = 0;
// 	int iNowCharIndex = 0;
// 	wstring wstrLineBuf = L"";
// 	wstring wstrColorCode = L"";
// 
// 	CHAR_STATE eNowCharState = Q_NORMAL;
// 	bool bIsFirstCharOfLine = true; 
// 
// 
// 	if( false == wstrTextColor.empty() )
// 	{
// 		wstrLineBuf += wstrTextColor;
// 	}
// 
// 	while( iNowCharIndex < (int) wstrText.length() )
// 	{
// 		WCHAR wszCharBuf = wstrText[iNowCharIndex];
// 		wstring wstrCharBuf = wstrText.substr( iNowCharIndex, 1 );
// 
// 		switch( eNowCharState )
// 		{
// 		case Q_NORMAL:
// 			{
// 				if( 0 == wstrCharBuf.compare( L"#" ) )
// 				{
// 					if( true == bEnableColor )
// 					{
// 						wstrLineBuf += wstrCharBuf;
// 						eNowCharState = Q_SHARP;
// 					}
// 					else
// 					{
// 						wstrLineBuf += L"*"; 
// 						iNowCharWidth = pFont->GetWidth( L"*" );
// 						iNowLineWidth += iNowCharWidth;
// 					}
// 				}
// 				else if( 0 == wstrCharBuf.compare( L"\n" ) )
// 				{
// 					// 2010/10/27 ��ȿ�� ���Ⱑ �ٸ�... bCutRedundantEmptyLine ��� ���� �� �ʿ伺... 
// 					// 09.05.30 �¿� : 2�� �̻��� \n�� �������� ���� ��� �߸��� ���� ����
// 					if( bIsFirstCharOfLine == false || bCutRedundantEmptyLine == false )
// 					{
// 						wstrLineBuf += wstrCharBuf;
// 						//wstrLineBuf += L" ";	// ���� �̺κ� �ּ� ó�� �� ����...
// 						iNowLineCount++;
// 						iNowLineWidth = 0;
// 						bIsFirstCharOfLine = true;
// 
// 						if( 0 != wstrTextColor.length() )
// 						{
// 							wstrLineBuf += wstrTextColor;
// 						}
// 
// 						if( 0 != wstrColorCode.length() )
// 						{
// 							wstrLineBuf += wstrColorCode;
// 						}
// 					}
// 				}
// 				else
// 				{
// 					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
// 					{
// 						if( true == bIsFirstCharOfLine )
// 							bIsFirstCharOfLine = false;
// 						wstrLineBuf += wstrCharBuf;					
// 						iNowCharWidth = pFont->GetWidth( wszCharBuf );
// 						iNowLineWidth += iNowCharWidth;
// 					}
// 				}
// 
// 			} break;
// 
// 		case Q_SHARP:
// 			{
// 				if( 0 == wstrCharBuf.compare( L"C" ) )
// 				{
// 					wstrLineBuf += wstrCharBuf;
// 					eNowCharState = Q_SHARP_C;
// 				}
// 				else
// 				{
// 					if( true == bIsFirstCharOfLine )
// 						bIsFirstCharOfLine = false;
// 
// 					wstrLineBuf += wstrCharBuf;
// 					iNowCharWidth = pFont->GetWidth( wszCharBuf );
// 					iNowLineWidth += iNowCharWidth;
// 
// 					eNowCharState = Q_NORMAL;
// 				}
// 			} break;
// 
// 		case Q_SHARP_C:
// 			{
// 				if( 0 == wstrCharBuf.compare( L"X" ) )
// 				{
// 					wstrLineBuf += wstrCharBuf;
// 					wstrColorCode = L"";
// 
// 					if( 0 != wstrTextColor.length() )
// 					{
// 						wstrLineBuf += wstrTextColor;
// 					}
// 
// 					eNowCharState = Q_NORMAL;
// 				}
// 				else // RGB 
// 				{
// 					// note!! color code�� �ùٸ��� üũ�ϴ� �ڵ� �־�� ��. �ϴ� color code�� ������ �ùٸ������� ����
// 					wstrColorCode = L"#C";
// 					wstrColorCode += wstrText.substr( iNowCharIndex, 6 );
// 					wstrLineBuf += wstrText.substr( iNowCharIndex, 6 );
// 					iNowCharIndex += 5;
// 					eNowCharState = Q_NORMAL;
// 				}
// 
// 			} break;
// 		}
// 
// 		if( iNowLineWidth >= iTextWidth && Q_NORMAL == eNowCharState )
// 		{
// 			wstrLineBuf += L"\n";
// 			//wstrLineBuf += L" ";
// 			iNowLineCount++;
// 			iNowLineWidth = 0;
// 			bIsFirstCharOfLine = true;
// 
// 			if( 0 != wstrTextColor.length() )
// 			{
// 				wstrLineBuf += wstrTextColor;
// 			}
// 
// 			if( 0 != wstrColorCode.length() )
// 			{
// 				wstrLineBuf += wstrColorCode;
// 			}
// 		}
// 
// 		iNowCharIndex++;
// 	}
// 
// 
// 	wstrText = wstrLineBuf;
// 
// 	return iNowLineCount;
//
// }


//---------------------------------------------------------//

/*int CWordLineHandler::LineBreakInChatWindow(wstring ChatDataMessage, CKTDGFontManager::CUKFont* pFont, int limitX )
{
	// LineBreakInTrainingGameUI ������ ��ü �����ؼ� ��ü�� //
	return LineBreakInTrainingGameUI( pFont, limitX, ChatDataMessage, L"\n", L"" );
}
*/

//int CWordLineHandler::LineBreakInSlideShot( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor /*= L""*/ )
//{
	// LineBreakInTrainingGameUI ������ ��ü �����ؼ� ��ü�� //
//	return LineBreakInTrainingGameUI( pFont, iWidth, wstrSpeech, pNextLineString, wstrColor );
//}


//int CWordLineHandler::LineBreakInEventScene( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor /*= L""*/ )
//{
	// �Լ� ȣ�� �κ� ���� ���� 
	// 	if( NULL == m_pDLGSlideShot )
	// 		return -1;

	// LineBreakInTrainingGameUI ������ ��ü �����ؼ� ��ü�� //
//	return LineBreakInTrainingGameUI( pFont, iWidth, wstrSpeech, pNextLineString, wstrColor );
//}

// ���� : ���� Ŭ���� ���� - 
//#ifdef LINE_BREAK_IN_SPEECH
//int CWordLineHandler::LineBreakInDungeonSpeech( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor /*= L""*/ )
//{
//	return LineBreakInTrainingGameUI( pFont, iWidth, wstrSpeech, pNextLineString, wstrColor );
//}
//#endif


//int CWordLineHandler::LineBreakInTrainingGameUI( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor /*= L""*/ )
/*{
	//	if( NULL == m_pDLGSlideShot )	//�ش� �κ� �Լ� ȣ��÷� ����
	//		return -1;


	const wstring wstrNextLine = pNextLineString;

	

	int addRow = 0;
	int iUniCharSize = 0;		// pixel ����??
	int iStringSize = 0;		// pixel ����??


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;
	//
#endif CLIENT_GLOBAL_EU_WORDWRAP

	// �ش� �κ� �Լ� ȣ�� �÷� ����
	// 	CKTDGUIStatic* pStatic_Speech	= (CKTDGUIStatic*) m_pDLGSlideShot->GetControl( L"Talk" );
	// 	if( NULL == pStatic_Speech )
	// 		return -1;
	// 
	// 	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_Speech->GetString(0)->fontIndex );
	// 	if( NULL == pFont )
	// 		return -1; 

	//������ ũ��(m_Size)�� ���� ������ ���� ������ ������
	for( UINT i=0; i<wstrSpeech.length(); i++ )
	{
		WCHAR wChar = wstrSpeech[i];
		iUniCharSize = pFont->GetWidth( wChar );
		iStringSize += iUniCharSize;


		WCHAR wNextChar = L'';
		if( (i+1) < (int) wstrSpeech.length() )   
			wNextChar = wstrSpeech[i+1];



		if( wChar == L'\n' )
		{
			iStringSize = 0;


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
			// LastCutPoint�� ������ ���ų� �� ��ġ�̴� 
			// ���� ����� ���� +1 ����
			iLastCutPoint = i;	
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
			//
#endif CLIENT_GLOBAL_EU_WORDWRAP

			continue;
		}


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
		//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
		if( IsSpaceLatter(wChar) )
		{
			iLastCutPoint = i+1;
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
		}
		else if ( IsSplitLatter(wChar) && (IsSpaceLatter(wNextChar) || IsSplitLatter(wNextChar)) )
		{
			iLastCutPoint = i+1;
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
		}
		else
		{
			iLastCharWidth = iUniCharSize;
			iNowLineWidthFromLastCurPoint += iUniCharSize;
		}
		//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP

		


		if( iStringSize >= iWidth  )
		{
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			if( iStringSize <= iNowLineWidthFromLastCurPoint )
			{
				wstring wsTemp = L"-\n";
				
				//
				wsTemp += wstrColor;
				
				if( i != 0 )
				{
					wstring::iterator tmp_it = wstrSpeech.begin() + (i-1);
					wstrSpeech.insert(tmp_it, wsTemp.begin(), wsTemp.end());
				}


				i += ( 2 + wstrColor.length() );

				// iStringSize�� iNowLineWidthFromLastCurPoint�� ��ġȭ
				iStringSize = iLastCharWidth;
				iNowLineWidthFromLastCurPoint = iLastCharWidth;
				
			}
			//
			else
			{
				wstring wsTemp = L"\n";

				//
				wsTemp += wstrColor;


				// end()�����ؼ� == �� ������ insert ������
				if( wstrSpeech.size() >= iLastCutPoint )
				{
					wstring::iterator tmp_it = wstrSpeech.begin() + iLastCutPoint;
					wstrSpeech.insert(tmp_it, wsTemp.begin(), wsTemp.end());
				}

				
				i += ( 1+ wstrColor.length() );

				// iStringSize�� iNowLineWidthFromLastCurPoint�� ��ġȭ
				iStringSize = iNowLineWidthFromLastCurPoint;
			}

			addRow++;

#else CLIENT_GLOBAL_EU_WORDWRAP

			wstrSpeech.insert( wstrSpeech.begin()+i, wstrNextLine.begin(), wstrNextLine.end() );
			i++;
			addRow++;
			iStringSize = 0;

			// ���⵵ length�� empty üũ�ϴ°� ���� ������
			wstrSpeech.insert( wstrSpeech.begin()+i, wstrColor.begin(), wstrColor.end() );
			i += wstrColor.length();
#endif CLIENT_GLOBAL_EU_WORDWRAP
		}
	}


	if( 0 != wstrColor.length() )
	{
		wstrSpeech.insert( wstrSpeech.begin(), wstrColor.begin(), wstrColor.end() );
	}

	return addRow;
}*/

//---------------------------------------------------------//

void CWordLineHandler::LineBreakInTalkBoxManagerImp( CKTDGFontManager::CUKFont* pFont,  D3DXVECTOR2 m_Size,
											 int& nRow, int& iColumnSize, wstring& wstrSpeech, bool bTrade
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
											 , D3DXVECTOR2 m_ResSize
#endif CLIENT_GLOBAL_EU_WORDWRAP
											 )
{
	KTDXPROFILE();

	const wstring wstrNextLine = L"\n";

	UINT uMaxStringLength = wstrSpeech.length();
	if( (int)uMaxStringLength > MAX_ROOM_CHAT_STR_LEN )
	{
		uMaxStringLength = MAX_ROOM_CHAT_STR_LEN;
	}

	nRow = 1;
	int iUniCharSize = 0;		// pixel ����??
	int iStringSize = 0;		// pixel ����??

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;
	//
#endif CLIENT_GLOBAL_EU_WORDWRAP


	D3DXVECTOR2 vMaxTalkBoxSize = g_pKTDXApp->ConvertByResolution( m_Size );

	if( vMaxTalkBoxSize.x > m_Size.x )
		vMaxTalkBoxSize.x = m_Size.x;



//--------------------------------------------------//

	//������ ũ��(m_Size)�� ���� ������ ���� ������ ������
	for( UINT i=0; i<wstrSpeech.length(); i++ )
	{
		WCHAR wChar = wstrSpeech[i];

		WCHAR wNextChar = L'';
		if( (i+1) < (int) wstrSpeech.length() )   
			wNextChar = wstrSpeech[i+1];


		iUniCharSize = pFont->GetWidth( wChar );		//	'a' - 7, '��' - 14
		//iUniCharSize = m_pUKFont->GetWidth( wChar );		//	'a' - 7, '��' - 14

		iStringSize += iUniCharSize;


#ifdef NUMBER_TO_LANGUAGE
		if( (wChar == L'\n') && false == bTrade )
#else
		if( wChar == L'\n' ) // fix!!
#endif NUMBER_TO_LANGUAGE
		{
			

			nRow++;
			iStringSize = 0;

			
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
			// tempText size �������� for���� ���⿡ +1 �ص� �ִ� tempText.end()�� �Ѱ���
			iLastCutPoint = i;
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
			//
#endif CLIENT_GLOBAL_EU_WORDWRAP
			continue;
		}


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
		//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
		if( IsSpaceLatter(wChar) )
		{
			iLastCutPoint = i+1;
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
		}
		else if ( IsSplitLatter(wChar) && (IsSpaceLatter(wNextChar) || IsSplitLatter(wNextChar)) )
		{
			iLastCutPoint = i+1;
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
		}
		else
		{
			iLastCharWidth = iUniCharSize;
			iNowLineWidthFromLastCurPoint += iUniCharSize;
		}
		//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
		if( iStringSize >= m_ResSize.x  )
#else //CLIENT_GLOBAL_EU_WORDWRAP
		if( iStringSize >= vMaxTalkBoxSize.x  )
#endif //CLIENT_GLOBAL_EU_WORDWRAP
		{
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			if( iStringSize <= iNowLineWidthFromLastCurPoint )
			{
				wstring wsTemp = L"-\n";

				//
				//wsTemp += wstrColor;
				
				if( i != 0 )
				{
					wstring::iterator tmp_it = wstrSpeech.begin() + (i-1);
					wstrSpeech.insert(tmp_it, wsTemp.begin(), wsTemp.end());
				}


				i += 2;
				//i += ( 2 + wstrColor.length() );

				// iStringSize�� iNowLineWidthFromLastCurPoint�� ��ġȭ
				iStringSize = iLastCharWidth;
				iNowLineWidthFromLastCurPoint = iLastCharWidth;

			}
			//
			else
			{
				wstring wsTemp = L"\n";

				//
				//wsTemp += wstrColor;


				// end()�����ؼ� == �� ������ insert ������
				if( wstrSpeech.size() >= iLastCutPoint )
				{
					wstring::iterator tmp_it = wstrSpeech.begin() + iLastCutPoint;
					wstrSpeech.insert(tmp_it, wsTemp.begin(), wsTemp.end());
				}

				i += 1;
				//i += ( 1+ wstrColor.length() );

				// iStringSize�� iNowLineWidthFromLastCurPoint�� ��ġȭ
				iStringSize = iNowLineWidthFromLastCurPoint;
			}

			nRow++;

#else CLIENT_GLOBAL_EU_WORDWRAP
			
			wstrSpeech.insert( wstrSpeech.begin()+i, wstrNextLine.begin(), wstrNextLine.end() );
			i++;

			nRow++;
			iStringSize = 0;
			
#endif CLIENT_GLOBAL_EU_WORDWRAP
		}
		
	}

//--------------------------------------------------//

	// ������ ��� �ڸ� �ڸ���
	// ���� �� ������ ����. ���� 2011.10.06 ��ȫ��
	// ������ �� �ڵ� ���� ������ ©���� �����̴�.(�������� ������?)
#ifdef BUG_FIX_PET_FOOD_DESC
	if( wstrSpeech.length() > 240 )
	{
		wstrSpeech = wstrSpeech.substr( 0, 240 );
	}
#else BUG_FIX_PET_FOOD_DESC
	if( wstrSpeech.length() > (UINT)MAX_ROOM_CHAT_STR_LEN )
	{
		wstrSpeech = wstrSpeech.substr( 0, MAX_ROOM_CHAT_STR_LEN );
	}
#endif BUG_FIX_PET_FOOD_DESC

	//{ JHKang / ������ / 2010.08.30 / ��üȭ�� ��� ��ȭ�� ���� �ػ󵵺� ��ǳ�� ũ�� ����
	/**	@brief	: ��� ������ 1024x768�� ���缭 ������ �ִ�. 
				  Ư�� m_Size�� ��� ���� ��� ���� ������ ������ ��Ȯ�� �ǹ̸� �˱� ��ƴ�.
				  �̷� ���� 1024������ ��ǳ���� ����ϰ� �� ������, �ٸ� �ػ󵵿����� ũ�Ⱑ
				  ����� ������ �ʴ´�. �̸� �����ϱ� ���� ��ǳ���� ũ�⸦ �ػ󵵿� �´� ������
				  �ٽ� ����� �־�����, �Ѱ谪�� ���ؼ� ���� ���� ũ�Ⱑ �������� �Ͽ���.
	*/
#ifdef RESIZE_BALLON
	float resRate = 0.f;
	if (true == g_pMain->GetGameOption().GetOptionList().m_bFullScreen)
	{
		int	iScreenX = GetSystemMetrics( SM_CXSCREEN );
		resRate = 1024.f / (float)iScreenX;
	}
	else
	{
		D3DXVECTOR2 vOriginalResolution = g_pMain->GetGameOption().GetOptionList().m_vResolution;
		resRate = 1024.f / vOriginalResolution.x;
	}

	// ���� ���� 0.8 ���ϸ� ���� �� ��ǳ���� ���ڰ� ��ġ�� ���� �߻�
	if (resRate < 0.8f)
		resRate = 0.8f;

	if( nRow > 1 )
	{
		if (resRate > 1.0f)
			resRate = 1.0f;

		iColumnSize = int(m_Size.x * resRate);
	}
	else
	{
		iColumnSize = int(iStringSize * resRate); // + (int)m_fMarginLeft + (int)m_fMarginRight;
	}
#else
	if( nRow > 1 )
	{
		iColumnSize = (int)m_Size.x;
	}
	else
	{
		iColumnSize = iStringSize; // + (int)m_fMarginLeft + (int)m_fMarginRight;
	}
#endif RESIZE_BALLON
	//}} JHKang / ������ / 2010.08.30 / ��üȭ�� ��� ��ȭ�� ���� �ػ󵵺� ��ǳ�� ũ�� ����
}


wstring CWordLineHandler::GetStrByLineBreakInX2Main( const WCHAR* pOrgStr, int width, int fontIndex )
{
	if ( pOrgStr == NULL )
		return L"";

	wstring tempText = pOrgStr;
	const int constTextMaxLen = (int)(g_pKTDXApp->GetResolutionScaleX() * width);

	int nowTextLen = 0;


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;
	//
#endif CLIENT_GLOBAL_EU_WORDWRAP

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( fontIndex );
	if( pFont != NULL )
	{
		for ( int i = 0; i < (int)tempText.length(); i++ )
		{
			WCHAR tmpWChar = tempText[i];

			WCHAR tmpNextWChar = L'';
			if( (i+1) < (int)tempText.length() )
				tmpNextWChar = tempText[i+1];

			
			if( tmpWChar == L'\n' )
			{
				nowTextLen = 0;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
				//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
				// tempText size �������� for���� ���⿡ +1 �ص� �ִ� tempText.end()�� �Ѱ���
				iLastCutPoint = i;
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
				//
#endif CLIENT_GLOBAL_EU_WORDWRAP

				continue;
			}


			if ( tempText[i] == L'#' && i + 1 < (int)tempText.size() && ( tempText[i+1] == L'C' || tempText[i+1] == L'c' ) )
			{
				if ( i + 2 < (int)tempText.size() )
				{
					if( tempText[i+2] == L'x' || tempText[i+2] == L'X' )
					{
						i += 2;
					}
					else
					{
						i += 7;
					}
					continue;
				}
			}

			int iUniCharSize = 0;

			iUniCharSize = pFont->GetWidth( tmpWChar );
			nowTextLen += iUniCharSize;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
			if( IsSpaceLatter(tmpWChar) )
			{
				iLastCutPoint = i+1;
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
			}
			else if ( IsSplitLatter(tmpWChar) && (IsSpaceLatter(tmpNextWChar) || IsSplitLatter(tmpNextWChar)) )
			{
				iLastCutPoint = i+1;
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
			}
			else
			{
				iLastCharWidth = iUniCharSize;
				iNowLineWidthFromLastCurPoint += iUniCharSize;
			}
			//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP



			if ( nowTextLen >= constTextMaxLen )
			{
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
				if( nowTextLen <= iNowLineWidthFromLastCurPoint )
				{
					wstring wsTemp = L"-\n";

					//
					//wsTemp += wstrColor;

					if( i != 0 )
					{
						wstring::iterator tmp_it = tempText.begin() + (i-1);
						tempText.insert(tmp_it, wsTemp.begin(), wsTemp.end());
					}


					i += 2;
					//i += ( 2 + wstrColor.length() );

					//iNowLineWidthFromLastCurPoint�� nowTextLen�� ��ġȭ
					nowTextLen = iLastCharWidth;
					iNowLineWidthFromLastCurPoint = iLastCharWidth;
				}
				//
				else
				{
					wstring wsTemp = L"\n";

					//
					//wsTemp += wstrColor;


					// end()�����ؼ� == �� ������ insert ������
					if( tempText.size() >= iLastCutPoint )
					{
						wstring::iterator tmp_it = tempText.begin() + iLastCutPoint;
						tempText.insert(tmp_it, wsTemp.begin(), wsTemp.end());
					}

					i += 1;
					//i += ( 1+ wstrColor.length() );

					//iNowLineWidthFromLastCurPoint�� nowTextLen�� ��ġȭ
					nowTextLen = iNowLineWidthFromLastCurPoint;
				}


#else CLIENT_GLOBAL_EU_WORDWRAP
				nowTextLen = 0;
				wstring enterChar = L"\n";
				if ( i + 1 < (int)tempText.size() ) 
					tempText.insert( tempText.begin() + i + 1, enterChar.begin(), enterChar.end() );
				else
					tempText.insert( tempText.begin() + i, enterChar.begin(), enterChar.end() );
				i++;
#endif CLIENT_GLOBAL_EU_WORDWRAP

			}


			if ( i >= (int)tempText.length() )
			{
				break;
			}
		}


	}

	return tempText;
}

wstring CWordLineHandler::BasicLineBreak( const WCHAR* pOrgStr, int width, int fontIndex )
{
	if ( pOrgStr == NULL )
		return L"";

	wstring tempText = pOrgStr;
	const int constTextMaxLen = (int)(g_pKTDXApp->GetResolutionScaleX() * width);

	int nowTextLen = 0;


//#ifdef CLIENT_GLOBAL_EU_WORDWRAP
//	// 2010/11/01 ��ȿ�� ���� ó�� 
//	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
//	int iLastCutPoint = 0;
//	int iLastCharWidth = 0;
//	int iNowLineWidthFromLastCurPoint = 0;
//	//
//#endif CLIENT_GLOBAL_EU_WORDWRAP

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( fontIndex );
	if( pFont != NULL )
	{
		for ( int i = 0; i < (int)tempText.length(); i++ )
		{
			WCHAR tmpWChar = tempText[i];

			WCHAR tmpNextWChar = L'';
			if( (i+1) < (int)tempText.length() )
				tmpNextWChar = tempText[i+1];


			if( tmpWChar == L'\n' )
			{
				nowTextLen = 0;

//#ifdef CLIENT_GLOBAL_EU_WORDWRAP
//				//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
//				// tempText size �������� for���� ���⿡ +1 �ص� �ִ� tempText.end()�� �Ѱ���
//				iLastCutPoint = i;
//				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
//				//
//#endif CLIENT_GLOBAL_EU_WORDWRAP

				continue;
			}


			if ( tempText[i] == L'#' && i + 1 < (int)tempText.size() && ( tempText[i+1] == L'C' || tempText[i+1] == L'c' ) )
			{
				if ( i + 2 < (int)tempText.size() )
				{
					if( tempText[i+2] == L'x' || tempText[i+2] == L'X' )
					{
						i += 2;
					}
					else
					{
						i += 7;
					}
					continue;
				}
			}

			int iUniCharSize = 0;

			iUniCharSize = pFont->GetWidth( tmpWChar );
			nowTextLen += iUniCharSize;

//#ifdef CLIENT_GLOBAL_EU_WORDWRAP
//			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
//			if( IsSpaceLatter(tmpWChar) )
//			{
//				iLastCutPoint = i+1;
//				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
//			}
//			else if ( IsSplitLatter(tmpWChar) && (IsSpaceLatter(tmpNextWChar) || IsSplitLatter(tmpNextWChar)) )
//			{
//				iLastCutPoint = i+1;
//				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
//			}
//			else
//			{
//				iLastCharWidth = iUniCharSize;
//				iNowLineWidthFromLastCurPoint += iUniCharSize;
//			}
//			//}}
//#endif CLIENT_GLOBAL_EU_WORDWRAP



			if ( nowTextLen >= constTextMaxLen )
			{
//#ifdef CLIENT_GLOBAL_EU_WORDWRAP
//				if( nowTextLen <= iNowLineWidthFromLastCurPoint )
//				{
//					wstring wsTemp = L"-\n";
//
//					//
//					//wsTemp += wstrColor;
//
//					if( i != 0 )
//					{
//						wstring::iterator tmp_it = tempText.begin() + (i-1);
//						tempText.insert(tmp_it, wsTemp.begin(), wsTemp.end());
//					}
//
//
//					i += 2;
//					//i += ( 2 + wstrColor.length() );
//
//					//iNowLineWidthFromLastCurPoint�� nowTextLen�� ��ġȭ
//					nowTextLen = iLastCharWidth;
//					iNowLineWidthFromLastCurPoint = iLastCharWidth;
//				}
//				//
//				else
//				{
//					wstring wsTemp = L"\n";
//
//					//
//					//wsTemp += wstrColor;
//
//
//					// end()�����ؼ� == �� ������ insert ������
//					if( tempText.size() >= iLastCutPoint )
//					{
//						wstring::iterator tmp_it = tempText.begin() + iLastCutPoint;
//						tempText.insert(tmp_it, wsTemp.begin(), wsTemp.end());
//					}
//
//					i += 1;
//					//i += ( 1+ wstrColor.length() );
//
//					//iNowLineWidthFromLastCurPoint�� nowTextLen�� ��ġȭ
//					nowTextLen = iNowLineWidthFromLastCurPoint;
//				}
//
//
//#else CLIENT_GLOBAL_EU_WORDWRAP
				nowTextLen = 0;
				wstring enterChar = L"\n";
				if ( i + 1 < (int)tempText.size() ) 
					tempText.insert( tempText.begin() + i + 1, enterChar.begin(), enterChar.end() );
				else
					tempText.insert( tempText.begin() + i, enterChar.begin(), enterChar.end() );
				i++;
//#endif CLIENT_GLOBAL_EU_WORDWRAP

			}


			if ( i >= (int)tempText.length() )
			{
				break;
			}
		}


	}

	return tempText;
}

#ifdef FIX_TOOLTIP
wstring CWordLineHandler::GetStrByLineBreakColorInX2Main( const WCHAR* pOrgStr, int width, int fontIndex )
{
	if ( pOrgStr == NULL )
		return L"";

	wstring tempText = pOrgStr;
	const int constTextMaxLen = (int)(g_pKTDXApp->GetResolutionScaleX() * width);

	int nowTextLen = 0;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;
	//
#endif CLIENT_GLOBAL_EU_WORDWRAP

	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( fontIndex );
	if( pFont != NULL )
	{
		wstring wstrPrevColorkey = L"";
		for ( int i = 0; i < (int)tempText.size(); i++ )
		{

			WCHAR tmpWChar = tempText[i];

			WCHAR tmpNextWChar = L'';
			if( (i+1) < (int)tempText.length() )
				tmpNextWChar = tempText[i+1];

			
			//if ( nowTextLen < constTextMaxLen && tempWchar == L'\n' )
			if ( tmpWChar == L'\n' )
			{
				nowTextLen = 0;
				tempText.insert( tempText.begin() + i + 1, wstrPrevColorkey.begin(), wstrPrevColorkey.end() );
				wstrPrevColorkey = L"";
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
				//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
				// tempText size �������� for���� ���⿡ +1 �ص� �ִ� tempText.end()�� �Ѱ���
				iLastCutPoint = i;
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
				//
#endif CLIENT_GLOBAL_EU_WORDWRAP

				continue;
			}

			if ( tempText[i] == L'#' && i + 1 < (int)tempText.size() && ( tempText[i+1] == L'C' || tempText[i+1] == L'c' ) )
			{
				if ( i + 2 < (int)tempText.size() )
				{
					if( tempText[i+2] == L'x' || tempText[i+2] == L'X' )
					{
						wstrPrevColorkey = L"";
						i += 2;
					}
					else
					{
						wstrPrevColorkey = tempText.substr(i, 8);
						i += 7;
					}
					continue;
				}
			}

			int iUniCharSize = 0;

			iUniCharSize = pFont->GetWidth( tmpWChar );
			nowTextLen += iUniCharSize;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
			if( IsSpaceLatter(tmpWChar) )
			{
				iLastCutPoint = i+1;
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
			}
			else if ( IsSplitLatter(tmpWChar) && (IsSpaceLatter(tmpNextWChar) || IsSplitLatter(tmpNextWChar)) )
			{
				iLastCutPoint = i+1;
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
			}
			else
			{
				iLastCharWidth = iUniCharSize;
				iNowLineWidthFromLastCurPoint += iUniCharSize;
			}
			//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP


			if ( nowTextLen >= constTextMaxLen )
			{
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
				if( nowTextLen <= iNowLineWidthFromLastCurPoint )
				{
					wstring wsTemp = L"-\n";

					//
					wsTemp += wstrPrevColorkey;

					if( i != 0 )
					{
						wstring::iterator tmp_it = tempText.begin() + (i-1);
						tempText.insert(tmp_it, wsTemp.begin(), wsTemp.end());
					}


					//i += 2;
					i += ( 2 + wstrPrevColorkey.length() );

					// iNowLineWidthFromLastCurPoint�� nowTextLen ��ġȭ
					nowTextLen = iLastCharWidth;
					iNowLineWidthFromLastCurPoint = iLastCharWidth;
				}
				//
				else
				{
					wstring wsTemp = L"\n";

					//
					wsTemp += wstrPrevColorkey;


					// end()�����ؼ� == �� ������ insert ������
					if( tempText.size() >= iLastCutPoint )
					{
						wstring::iterator tmp_it = tempText.begin() + iLastCutPoint;
						tempText.insert(tmp_it, wsTemp.begin(), wsTemp.end());
					}

					//i += 1;
					i += ( 1+ wstrPrevColorkey.length() );

					// iNowLineWidthFromLastCurPoint�� nowTextLen ��ġȭ
					nowTextLen = iNowLineWidthFromLastCurPoint;
				}


#else CLIENT_GLOBAL_EU_WORDWRAP

				nowTextLen = 0;
				wstring enterChar = L"\n";

				if ( i + 1 < (int)tempText.size() )
				{	
					if ( tempText[i + 1] != L'\n' )
					{
						tempText.insert( tempText.begin() + i + 1, enterChar.begin(), enterChar.end() );

						// ���� ���� ù ���ڰ� ������ ��� ����
						if ( i + 2 < (int)tempText.size() && tempText[i + 2] == L' ' )
						{
							tempText.erase( i + 2, 1 );
						}

						if(wstrPrevColorkey != L"")
						{
							tempText.insert( tempText.begin() + i + 2, wstrPrevColorkey.begin(), wstrPrevColorkey.end() );
							wstrPrevColorkey = L"";
						}
					}
				}
				else
				{
					if ( tempText[i] != L'\n' && tempText[i] != L')' && tempText[i] != L'.' )
					{
						tempText.insert( tempText.begin() + i, enterChar.begin(), enterChar.end() );

						if(wstrPrevColorkey != L"")
						{
							tempText.insert( tempText.begin() + i + 1, wstrPrevColorkey.begin(), wstrPrevColorkey.end() );
							wstrPrevColorkey = L"";
						}
					}
				}
				i++;

#endif CLIENT_GLOBAL_EU_WORDWRAP

			}


			if ( i >= (int)tempText.size() )
			{
				break;
			}
		}
	}

	return tempText;
}
#endif FIX_TOOLTIP





//================================================================================//

void CWordLineHandler::ProcessQuestDescInUIQuestNew( const wstring& wstrDesc, const int iLineWidth, CKTDGFontManager::CUKFont* pFont,D3DXVECTOR2 m_vQuestDescSize, vector<wstring>&  m_vecQuestDesc)
{
	// �ϴ� wstrDesc�� ������-_-
	// Ŭ���� ���� -> 
	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;
	//
#endif CLIENT_GLOBAL_EU_WORDWRAP
    //m_vQuestDescSize = D3DXVECTOR2( (float) m_pDLGUIQuestDetail->GetDummyInt( 0 ), (float) m_pDLGUIQuestDetail->GetDummyInt( 1 ) );

    const int MAGIC_MAX_WIDTH = (int) (g_pKTDXApp->GetResolutionScaleX() * iLineWidth);

	m_vecQuestDesc.resize(0);

	if( wstrDesc.length() == 0 )
	{
		return;
	}

	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowCharIndex = 0;
	wstring wstrLineBuf = L"";
	wstring wstrPageBuf = L"";
	wstring wstrColorCode = L"";

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 


	int iDescLength = (int) wstrDesc.length();
	while( iNowCharIndex < iDescLength )
	{
		WCHAR wszCharBuf = wstrDesc[iNowCharIndex];
		WCHAR wszNextCharBuf = L'';

		if( (iNowCharIndex+1) < (int) wstrDesc.length() )   
			wszNextCharBuf = wstrDesc[iNowCharIndex+1];


		wstring wstrCharBuf = wstrDesc.substr( iNowCharIndex, 1 ); 
		

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					// �� �ٲ�� ���� �κе� Ŀ�� ����Ʈ, �� ���� ���ϸ� #�� Ŀ�� ����Ʈ
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					//
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrPageBuf += wstrLineBuf;
					wstrLineBuf = L"";
					iNowLineWidth = 0;

					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}
					if(false == bIsFirstCharOfLine)
					{
						m_vecQuestDesc.push_back( wstrPageBuf );
					}
					bIsFirstCharOfLine = true;
					wstrPageBuf = L"";


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					//
#endif CLIENT_GLOBAL_EU_WORDWRAP

				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;					
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
						//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
						if( IsSpaceLatter(wszCharBuf) )
						{
							iLastCutPoint = wstrLineBuf.length();
							iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
						}
						else if ( IsSplitLatter(wszCharBuf) && (IsSpaceLatter(wszNextCharBuf) || IsSplitLatter(wszNextCharBuf)) )
						{
							iLastCutPoint = wstrLineBuf.length();
							iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
						}
						else
						{
							iLastCharWidth = iNowCharWidth;
							iNowLineWidthFromLastCurPoint += iNowCharWidth;
						}
						//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP

					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else if( 0 == wstrCharBuf.compare( L"m" ) )
				{
					wstrLineBuf.erase( wstrLineBuf.end()-1 ); // '#'�� ������
					wstrPageBuf += wstrLineBuf;
					if(false == bIsFirstCharOfLine)
					{
						m_vecQuestDesc.push_back( wstrPageBuf );
					}
					wstrLineBuf = L"";
					wstrPageBuf = L"";
					bIsFirstCharOfLine = true;
					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}

					eNowCharState = Q_NORMAL;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					// # �������鼭 ColorCode ���� Ŀ�� ������ ������ 
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					//
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
				else
				{
					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					if( IsSpaceLatter(wszCharBuf) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else if ( IsSplitLatter(wszCharBuf) && (IsSpaceLatter(wszNextCharBuf) || IsSplitLatter(wszNextCharBuf)) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else
					{
						iLastCharWidth = iNowCharWidth;
						iNowLineWidthFromLastCurPoint += iNowCharWidth;
					}
					//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP

				}

			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrColorCode = L"";
					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					wstrColorCode = L"#C";
					wstrColorCode += wstrDesc.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrDesc.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
				//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
				// �� �ٲ�� ������ �׻� Ŀ�� ����Ʈ�̴�.
				iLastCutPoint = wstrLineBuf.length();
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
				//
#endif CLIENT_GLOBAL_EU_WORDWRAP

			} break;
		}

		if( iNowLineWidth >= MAGIC_MAX_WIDTH && Q_NORMAL == eNowCharState )
		{
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			wstring wsTempForNewPage = L"";

			// �� ��� �ش� ������ ��� �ڸ� ����Ʈ�� ���� ����̴�. 
			if( iNowLineWidth <= iNowLineWidthFromLastCurPoint )
			{

				wstring wsTemp = L"-\n";
				if( 0 != wstrColorCode.length() )
				{
					wsTemp += wstrColorCode;
				}

				if( wstrLineBuf.length() != 0 )
				{
					int tmp_index = (int)wstrLineBuf.size() -1 +2; // +2 L"-\n"

					wstring::iterator tmp_it = wstrLineBuf.end() -1;
					wstrLineBuf.insert(tmp_it, wsTemp.begin(), wsTemp.end());

					wsTempForNewPage.insert(wsTempForNewPage.begin(), wstrLineBuf.begin()+tmp_index, wstrLineBuf.end());

					wstrLineBuf.erase( wstrLineBuf.begin()+tmp_index, wstrLineBuf.end() );
				}

				// iNowLineWidth�� iNowLineWidthFromLastCurPoint ��ġȭ 
				iNowLineWidth = iLastCharWidth;
				iNowLineWidthFromLastCurPoint = iLastCharWidth;
			}
			//
			else
			{
				wstring wsTemp = L"\n";
				if( 0 != wstrColorCode.length() )
				{
					wsTemp += wstrColorCode;
				}

				// end()�����ؼ� == �� ������ insert ������
				if( (int)wstrLineBuf.length() >= iLastCutPoint )
				{
					int tmp_index = iLastCutPoint +1; // +1  L"\n";

					wstring::iterator tmp_it = wstrLineBuf.begin() + iLastCutPoint;
					wstrLineBuf.insert(tmp_it, wsTemp.begin(), wsTemp.end());

					wsTempForNewPage.insert(wsTempForNewPage.begin(), wstrLineBuf.begin()+tmp_index, wstrLineBuf.end());
					
					wstrLineBuf.erase( wstrLineBuf.begin()+tmp_index, wstrLineBuf.end() );
				}

				// iNowLineWidth�� iNowLineWidthFromLastCurPoint ��ġȭ 
				iNowLineWidth = iNowLineWidthFromLastCurPoint;
			}

			//iNowLineCount++;
			wstrPageBuf += wstrLineBuf;
			wstrLineBuf = wsTempForNewPage;


			// �� ���� ù��° ���ڰ� �ִ� ��Ȳ�̴�. 
			bIsFirstCharOfLine = false;
			m_vecQuestDesc.push_back( wstrPageBuf );
			wstrPageBuf = L"";

			//}}
#else CLIENT_GLOBAL_EU_WORDWRAP

			wstrLineBuf += L"\n";
			wstrPageBuf += wstrLineBuf;
			wstrLineBuf = L"";
			iNowLineWidth = 0;
			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}

			if(false == bIsFirstCharOfLine)
			{
				m_vecQuestDesc.push_back( wstrPageBuf );
			}
			bIsFirstCharOfLine = true;
			wstrPageBuf = L"";
#endif CLIENT_GLOBAL_EU_WORDWRAP
		}

		iNowCharIndex++;
	}

	if( wstrLineBuf.length() > 0 )
	{
		wstrPageBuf += wstrLineBuf;
	}

	if( wstrPageBuf.length() > 0 )
	{
		m_vecQuestDesc.push_back( wstrPageBuf );
	}


}



/*void CWordLineHandler::UpdateQuestDescInX2TFieldNpcShop( const wstring& wstrDesc,CKTDGFontManager::CUKFont* pFont, D3DXVECTOR2 m_vQuestDescSize, vector<wstring>&  m_vecQuestDesc)
{
	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

// �Լ� ȣ���ϴ� �κ����� �̵�
// 	m_vQuestDescSize = D3DXVECTOR2( (float) m_pDlgMessage->GetDummyInt( 0 ), (float) m_pDlgMessage->GetDummyInt( 1 ) );
// 	m_vQuestDescSize = g_pKTDXApp->ConvertByResolution( m_vQuestDescSize );

// 	CKTDGUIStatic* pStatic_QuestDesc = (CKTDGUIStatic*) m_pDlgMessage->GetControl( L"npcMessage" );
// 	if( NULL == pStatic_QuestDesc )
// 		return;

// 	CKTDGFontManager::CUKFont* pFont 
// 		= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pStatic_QuestDesc->GetString(0)->fontIndex );
// 	if( NULL == pFont )
// 		return; 


	const int MAGIC_MAX_ROW = (int) (m_vQuestDescSize.y / (float ) pFont->GetHeight() * 0.5f);
	const int MAGIC_MAX_WIDTH = (int) m_vQuestDescSize.x;

	m_vecQuestDesc.resize(0);

// �Լ� ȣ���ϴ� �κ����� �̵�
// 	if( wstrDesc.length() == 0 )
// 	{
// 		m_iNowQuestDescPage = 0;
// 		m_iMaxQuestDescPage = 0;
// 		return;
// 	}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;
	//
#endif CLIENT_GLOBAL_EU_WORDWRAP

	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowLineCount = 0;
	int iNowCharIndex = 0;
	wstring wstrLineBuf = L"";
	wstring wstrPageBuf = L"";
	wstring wstrColorCode = L"";

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 

	int iDescLength = (int) wstrDesc.length();
	while( iNowCharIndex < iDescLength )
	{
		WCHAR wszCharBuf = wstrDesc[iNowCharIndex];
		WCHAR wszNextCharBuf = L'';

		if( (iNowCharIndex+1) < (int) wstrDesc.length() )   
			wszNextCharBuf = wstrDesc[iNowCharIndex+1];

		wstring wstrCharBuf = wstrDesc.substr( iNowCharIndex, 1 ); 

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					// �� �ٲ�� ���� �κе� Ŀ�� ����Ʈ, �� ���� ���ϸ� #�� Ŀ�� ����Ʈ					
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrPageBuf += wstrLineBuf;
					iNowLineCount++;
					wstrLineBuf = L"";
					iNowLineWidth = 0;
					bIsFirstCharOfLine = true;

					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}

					if( iNowLineCount >= MAGIC_MAX_ROW )
					{
						m_vecQuestDesc.push_back( wstrPageBuf );
						wstrPageBuf = L"";
						iNowLineCount = 0;
					}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					//
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;					
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
						//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
						if( IsSpaceLatter(wszCharBuf) )
						{
							iLastCutPoint = wstrLineBuf.length();
							iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
						}
						else if ( IsSplitLatter(wszCharBuf) && (IsSpaceLatter(wszNextCharBuf) || IsSplitLatter(wszNextCharBuf)) )
						{
							iLastCutPoint = wstrLineBuf.length();
							iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
						}
						else
						{
							iLastCharWidth = iNowCharWidth;
							iNowLineWidthFromLastCurPoint += iNowCharWidth;
						}
						//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP
					}
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else if( 0 == wstrCharBuf.compare( L"m" ) )
				{
					wstrLineBuf.erase( wstrLineBuf.end()-1 ); // '#'�� ������
					wstrPageBuf += wstrLineBuf;
					m_vecQuestDesc.push_back( wstrPageBuf );
					wstrLineBuf = L"";
					wstrPageBuf = L"";
					iNowLineCount = 0;
					bIsFirstCharOfLine = true;
					if( 0 != wstrColorCode.length() )
					{
						wstrLineBuf += wstrColorCode;
					}

					eNowCharState = Q_NORMAL;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					//
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
				else
				{
					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					if( IsSpaceLatter(wszCharBuf) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else if ( IsSplitLatter(wszCharBuf) && (IsSpaceLatter(wszNextCharBuf) || IsSplitLatter(wszNextCharBuf)) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else
					{
						iLastCharWidth = iNowCharWidth;
						iNowLineWidthFromLastCurPoint += iNowCharWidth;
					}
					//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}

			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;
					wstrColorCode = L"";
					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					wstrColorCode = L"#C";
					wstrColorCode += wstrDesc.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrDesc.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
				//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
				// �� �ٲ�� ������ �׻� Ŀ�� ����Ʈ�̴�.
				iLastCutPoint = wstrLineBuf.length();
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
				//
#endif CLIENT_GLOBAL_EU_WORDWRAP

			} break;
		}

		if( iNowLineWidth >= MAGIC_MAX_WIDTH && Q_NORMAL == eNowCharState )
		{
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
#ifdef CLIENT_GLOBAL_EU_WORDWRAP

			wstring wsTempForNewPage = L"";

			// �� ��� �ش� ������ ��� �ڸ� ����Ʈ�� ���� ����̴�. 
			if( iNowLineWidth <= iNowLineWidthFromLastCurPoint )
			{
				wstring wsTemp = L"-\n";

				if( 0 != wstrColorCode.length() )
				{
					wsTemp += wstrColorCode;
				}

				if( wstrLineBuf.length() != 0 )
				{
					int tmp_index = (int)wstrLineBuf.size() -1 +2; //L"-\n";

					// �� ��� ������ ���� �տ� ������ �־��ش�. //
					wstring::iterator tmp_it = wstrLineBuf.end() -1;
					wstrLineBuf.insert(tmp_it, wsTemp.begin(), wsTemp.end());

					wsTempForNewPage.insert(wsTempForNewPage.begin(), wstrLineBuf.begin()+tmp_index, wstrLineBuf.end());
				}

				// iNowLineWidthFromLastCurPoint�� iNowLineWidth�� ��ġȭ
				iNowLineWidth = iLastCharWidth;
				iNowLineWidthFromLastCurPoint = iLastCharWidth;
			}
			//
			else
			{
				wstring wsTemp = L"\n";

				if( 0 != wstrColorCode.length() )
				{
					wsTemp += wstrColorCode;
				}

				// end()�����ؼ� == �� ������ insert ������
				if( (int)wstrLineBuf.length() >= iLastCutPoint )
				{
					int tmp_index = iLastCutPoint +1; //L"\n";

					wstring::iterator tmp_it = wstrLineBuf.begin() + iLastCutPoint;
					wstrLineBuf.insert(tmp_it, wsTemp.begin(), wsTemp.end());

					wsTempForNewPage.insert(wsTempForNewPage.begin(), wstrLineBuf.begin()+tmp_index, wstrLineBuf.end());
				}

				// iNowLineWidthFromLastCurPoint�� iNowLineWidth�� ��ġȭ
				iNowLineWidth = iNowLineWidthFromLastCurPoint;

			}

			// �� ���� ù��° ���ڰ� �ִ� ��Ȳ�̴�. 
			bIsFirstCharOfLine = false;

			iNowLineCount++;


			if( iNowLineCount >= MAGIC_MAX_ROW )
			{
				m_vecQuestDesc.push_back( wstrPageBuf );
				wstrPageBuf = L"";
				iNowLineCount = 0;
			}


			//}}
#else CLIENT_GLOBAL_EU_WORDWRAP
			wstrLineBuf += L"\n";
			wstrPageBuf += wstrLineBuf;
			iNowLineCount++;
			wstrLineBuf = L"";
			iNowLineWidth = 0;
			bIsFirstCharOfLine = true;
			if( 0 != wstrColorCode.length() )
			{
				wstrLineBuf += wstrColorCode;
			}

			if( iNowLineCount >= MAGIC_MAX_ROW )
			{
				m_vecQuestDesc.push_back( wstrPageBuf );
				wstrPageBuf = L"";
				iNowLineCount = 0;
			}
#endif CLIENT_GLOBAL_EU_WORDWRAP

		}

		iNowCharIndex++;
	}

	if( wstrLineBuf.length() > 0 )
	{
		wstrPageBuf += wstrLineBuf;
	}

	if( wstrPageBuf.length() > 0 )
	{
		m_vecQuestDesc.push_back( wstrPageBuf );
	}

	// �Լ� �� �Լ� ȣ�� �ķ� �ű�
// 	m_iMaxQuestDescPage = (int) m_vecQuestDesc.size();
// 	m_iNowQuestDescPage = 1;
}*/




// �ش� �Լ��� ��������. ���� ��𿡼� �������� ��������� 
void CWordLineHandler::LineBreakInX2Community(wstring tempChatMsg, CKTDGFontManager::CUKFont* pFont, wstring chatColor, vector<wstring>& m_vecTalk, CKTDGUIListBox* pListBoxMessenger, bool  bApplyColor, bool bIsSameChatSession)
{
	int iUniCharSize = 0;		// pixel ����??
	int iStringSize = 0;		// pixel ����??

	wstring tempString = L"";

// #ifdef CLIENT_GLOBAL_EU_WORDWRAP
// 	// 2010/11/01 ��ȿ�� ���� ó�� 
// 	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
// 	int iLastCutPoint = 0;
// 	int iLastCharWidth = 0;
// 	int iNowLineWidthFromLastCurPoint = 0;
// 	//
// #endif CLIENT_GLOBAL_EU_WORDWRAP

	for( UINT i=0; i<tempChatMsg.length(); i++ )
	{
		WCHAR wChar = tempChatMsg[i];

		if ( wChar == '\n' )
		{
			if ( bApplyColor == true )
			{
				wstring colorString = chatColor;
				colorString += tempString;
				m_vecTalk.push_back( colorString );
			}
			else
				m_vecTalk.push_back( tempString );

			if ( bIsSameChatSession )
			{
				if ( bApplyColor == true )
				{
					wstring colorString = chatColor;
					colorString += tempString;
					pListBoxMessenger->AddItem( colorString.c_str(), NULL );
				}
				else
				{
					pListBoxMessenger->AddItem( tempString.c_str(), NULL );
				}
				pListBoxMessenger->SetScrollBarEndPos();
			}
			tempString.clear();
			continue;
		}

		tempString += wChar;


		iUniCharSize = pFont->GetWidth( wChar );

		iStringSize += iUniCharSize;


// #ifdef CLIENT_GLOBAL_EU_WORDWRAP
// 		//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
// 		if( IsSpaceLatter(wChar) || IsSplitLatter(wChar))
// 		{
// 			iLastCutPoint = i+1;
// 			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
// 		}
// 		else
// 		{
// 			iLastCharWidth = iUniCharSize;
// 			iNowLineWidthFromLastCurPoint += iUniCharSize;
// 		}
// 		//}}
// #endif CLIENT_GLOBAL_EU_WORDWRAP



		const int iStringWidth = (int)(238 * g_pKTDXApp->GetResolutionScaleX());

		if( iStringSize >= iStringWidth  )
		{
			if ( bApplyColor == true )
			{
				wstring colorString = chatColor;
				colorString += tempString;
				m_vecTalk.push_back( colorString );
			}
			else
				m_vecTalk.push_back( tempString );

			//if ( pChatSession == m_pOpendChatSession )
			{
				if ( bApplyColor == true )
				{
					wstring colorString = chatColor;
					colorString += tempString;
					pListBoxMessenger->AddItem( colorString.c_str(), NULL );
				}
				else
				{
					pListBoxMessenger->AddItem( tempString.c_str(), NULL );
				}
				pListBoxMessenger->SetScrollBarEndPos();
			}
			tempString.clear();


			iStringSize = 0;
		}
	}

	if ( tempString.empty() == false )
	{
		if ( bApplyColor == true )
		{
			wstring colorString = chatColor;
			colorString += tempString;
			m_vecTalk.push_back( colorString );
		}
		else
			m_vecTalk.push_back( tempString );

		//if ( pChatSession == m_pOpendChatSession )
		{
			if ( bApplyColor == true )
			{
				wstring colorString = chatColor;
				colorString += tempString;
				pListBoxMessenger->AddItem( colorString.c_str(), NULL );
			}
			else
			{
				pListBoxMessenger->AddItem( tempString.c_str(), NULL );
			}
			pListBoxMessenger->SetScrollBarEndPos();
		}
		tempString.clear();
	}
}



int CWordLineHandler::LineBreakInX2MainMsgBox(wstring& tempText, CKTDGFontManager::CUKFont* pFont, const int constTextMaxLen)
{
	int lineNum = 1;
	int nowTextLen = 0;
	

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;
	//
#endif CLIENT_GLOBAL_EU_WORDWRAP

	for ( int i = 0; i < (int)tempText.size(); i++ )
	{
		WCHAR tmpWChar = tempText[i];

		WCHAR tmpNextWChar = L'';
		if( (i+1) < (int)tempText.length() )
			tmpNextWChar = tempText[i+1];


		if( tmpWChar == L'\n' )
		{
			if ( i != (int)tempText.size() - 1 )
			{
				lineNum++;
			}

			nowTextLen = 0;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
			// LastCutPoint�� ������ ���ų� �� ��ġ�̴� 
			// ���� ����� ���� +1 ����
			iLastCutPoint = i;	
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
			//
#endif CLIENT_GLOBAL_EU_WORDWRAP

			continue;
		}


		//{{ 2010. 11. 19 ��ȿ�� �߰� ���� 
		if ( tempText[i] == L'#' && i + 1 < (int)tempText.size() && ( tempText[i+1] == L'C' || tempText[i+1] == L'c' ) )
		{
			if ( i + 2 < (int)tempText.size() )
			{
				if( tempText[i+2] == L'x' || tempText[i+2] == L'X' )
				{
					//wstrPrevColorkey = L"";
					i += 2;
				}
				else
				{
					//wstrPrevColorkey = tempText.substr(i, 8);
					i += 7;
				}
				continue;
			}
		}
		//}}



		int iUniCharSize = 0;
		//		uniBuffer.Clear();
		//		uniBuffer.InsertChar( 0, tempWchar );
		//		uniBuffer.CPtoX(uniBuffer.GetTextSize(), FALSE, &iUniCharSize);

		iUniCharSize = pFont->GetWidth( tmpWChar );
		nowTextLen += iUniCharSize;


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
		//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
		if( IsSpaceLatter(tmpWChar) )
		{
			iLastCutPoint = i+1;
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
		}
		else if ( IsSplitLatter(tmpWChar) && (IsSpaceLatter(tmpNextWChar) || IsSplitLatter(tmpNextWChar)) )
		{
			iLastCutPoint = i+1;
			iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
		}
		else
		{
			iLastCharWidth = iUniCharSize;
			iNowLineWidthFromLastCurPoint += iUniCharSize;
		}
		//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP


		if ( nowTextLen >= constTextMaxLen )
		{
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			if( nowTextLen <= iNowLineWidthFromLastCurPoint )
			{
				wstring wsTemp = L"-\n";

				//
				//wsTemp += wstrColor;

				if( i != 0 )
				{
					wstring::iterator tmp_it = tempText.begin() + (i-1);
					tempText.insert(tmp_it, wsTemp.begin(), wsTemp.end());
				}


				i += 2;
				//i += ( 2 + wstrColor.length() );

				// iNowLineWidthFromLastCurPoint�� nowTextLen�� ��ġȭ
				nowTextLen = iLastCharWidth;
				iNowLineWidthFromLastCurPoint = iLastCharWidth;
			}
			//
			else
			{
				wstring wsTemp = L"\n";

				//
				//wsTemp += wstrColor;


				// end()�����ؼ� == �� ������ insert ������
				if( tempText.size() >= iLastCutPoint )
				{
					wstring::iterator tmp_it = tempText.begin() + iLastCutPoint;
					tempText.insert(tmp_it, wsTemp.begin(), wsTemp.end());
				}

				i += 1;
				//i += ( 1+ wstrColor.length() );

				// iNowLineWidthFromLastCurPoint�� nowTextLen�� ��ġȭ
				nowTextLen = iNowLineWidthFromLastCurPoint;

			}

			lineNum++;

#else CLIENT_GLOBAL_EU_WORDWRAP

			if ( i != (int)tempText.size() - 1 )
			{
				lineNum++;
			}

			nowTextLen = 0;
			wstring enterChar = L"\n";
			tempText.insert( tempText.begin() + i, enterChar.begin(), enterChar.end() );
			i++;
#endif CLIENT_GLOBAL_EU_WORDWRAP
		}

		if ( i >= (int)tempText.size() )
		{
			break;
		}
	}

	return lineNum;
}

//{{ ����� : [2009/7/20] //	�ް����� LineBreak, ���� ������ �ٷ� �����Ѵ�.
bool CWordLineHandler::MegaLineBreakAdd( wstring wstrText, CKTDGFontManager::CUKFont* pFont, int iTextWidth, CKTDGUIListBox* pListBox , int iMegaID, wstring wstrSenderName)
{
	enum CHAR_STATE
	{
		Q_NORMAL,
		Q_SHARP,
		Q_SHARP_C,
	};

//  �ش� �κ� �Լ� ȣ�� �÷� �� 
// 	CKTDGUIListBox* pListBoxChatContent = (CKTDGUIListBox*)m_pDLGChatWindow->GetControl( L"ListBox_All" );
// 	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( pListBoxChatContent->GetString()->fontIndex );

	int iNowCharWidth = 0;		
	int iNowLineWidth = 0;
	int iNowLineCount = 0;
	int iNowCharIndex = 0;
	wstring wstrColorCode = L"";
	wstring wstrLineBuf = L"";

	const float MAGIC_CHAT_SHOW_TIME = 50.f;
	const int MAX_LINE = 100;

//  �ش� �κ� �Լ� ȣ�� �÷� �� 
//	//	�ް��� ID ����
//	m_iMegaID = (m_iMegaID + 1) % 100000000;

	CHAR_STATE eNowCharState = Q_NORMAL;
	bool bIsFirstCharOfLine = true; 
	bool bIsMyMessage = false;

	const int iLastTrackPos = pListBox->GetScrollBar()->GetTrackPos() + pListBox->GetScrollBar()->GetPageSize();
	const int iLastTrackEndPos = pListBox->GetScrollBar()->GetTrackEndPos();


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
	// 2010/11/01 ��ȿ�� ���� ó�� 
	// last cut point : cutting ���� �ڿ� ������ ���� �Ͽ� ���´�.
	int iLastCutPoint = 0;
	int iLastCharWidth = 0;
	int iNowLineWidthFromLastCurPoint = 0;

	//
#endif CLIENT_GLOBAL_EU_WORDWRAP

	//	�ڱ� �޽������� ����
	if( wstrSenderName == g_pData->GetMyUser()->GetSelectUnit()->GetNickName() )
	{
		bIsMyMessage = true;
	}

	while( pListBox->GetSize() > MAX_LINE )
	{
		pListBox->RemoveItem(0);
	}

	while( iNowCharIndex < (int) wstrText.length() )
	{
		WCHAR wszCharBuf = wstrText[iNowCharIndex];
		WCHAR wszNextCharBuf = L'';

		if( (iNowCharIndex+1) < (int) wstrText.length() )   
			wszNextCharBuf = wstrText[iNowCharIndex+1];


		wstring wstrCharBuf = wstrText.substr( iNowCharIndex, 1 );

		switch( eNowCharState )
		{
		case Q_NORMAL:
			{
				if( 0 == wstrCharBuf.compare( L"#" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP;

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					// �� �ٲ�� ���� �κе� Ŀ�� ����Ʈ, �� ���� ���ϸ� #�� Ŀ�� ����Ʈ
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					//
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
				else if( 0 == wstrCharBuf.compare( L"\n" ) )
				{
					if( bIsFirstCharOfLine == false )
					{
						wstrLineBuf += wstrCharBuf;
						wstrLineBuf += L"  ";
						iNowLineCount++;
						iNowLineWidth = 0;
						bIsFirstCharOfLine = true;

						if( 0 != wstrColorCode.length() )
						{
							wstrLineBuf += wstrColorCode;
						}
					}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					iLastCutPoint = wstrLineBuf.length();
					iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					//
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
				else
				{
					if( 0 != wstrCharBuf.compare( L" " ) || false == bIsFirstCharOfLine )
					{
						if( true == bIsFirstCharOfLine )
							bIsFirstCharOfLine = false;
						wstrLineBuf += wstrCharBuf;
						iNowCharWidth = pFont->GetWidth( wszCharBuf );
						iNowLineWidth += iNowCharWidth;
					}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					if( IsSpaceLatter(wszCharBuf) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else if ( IsSplitLatter(wszCharBuf) && (IsSpaceLatter(wszNextCharBuf) || IsSplitLatter(wszNextCharBuf)) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else
					{
						iLastCharWidth = iNowCharWidth;
						iNowLineWidthFromLastCurPoint += iNowCharWidth;
					}
					//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}

			} break;

		case Q_SHARP:
			{
				if( 0 == wstrCharBuf.compare( L"C" ) )
				{
					wstrLineBuf += wstrCharBuf;
					eNowCharState = Q_SHARP_C;
				}
				else
				{
					if( true == bIsFirstCharOfLine )
						bIsFirstCharOfLine = false;

					wstrLineBuf += wstrCharBuf;
					iNowCharWidth = pFont->GetWidth( wszCharBuf );
					iNowLineWidth += iNowCharWidth;

					eNowCharState = Q_NORMAL;


#ifdef CLIENT_GLOBAL_EU_WORDWRAP
					//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
					if( IsSpaceLatter(wszCharBuf) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else if ( IsSplitLatter(wszCharBuf) && (IsSpaceLatter(wszNextCharBuf) || IsSplitLatter(wszNextCharBuf)) )
					{
						iLastCutPoint = wstrLineBuf.length();
						iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
					}
					else
					{
						iLastCharWidth = iNowCharWidth;
						iNowLineWidthFromLastCurPoint += iNowCharWidth;
					}
					//}}
#endif CLIENT_GLOBAL_EU_WORDWRAP
				}
			} break;

		case Q_SHARP_C:
			{
				if( 0 == wstrCharBuf.compare( L"X" ) )
				{
					wstrLineBuf += wstrCharBuf;

					eNowCharState = Q_NORMAL;
				}
				else // RGB 
				{
					// fix!! color code�� �ùٸ��� üũ�ϴ� �ڵ� �־�� ��. �ϴ� color code�� ������ �ùٸ������� ����
					wstrColorCode = L"#C";
					wstrColorCode += wstrText.substr( iNowCharIndex, 6 );
					wstrLineBuf += wstrText.substr( iNowCharIndex, 6 );
					iNowCharIndex += 5;
					eNowCharState = Q_NORMAL;
				}

#ifdef CLIENT_GLOBAL_EU_WORDWRAP
				//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
				// �� �ٲ�� ������ �׻� Ŀ�� ����Ʈ�̴�.
				iLastCutPoint = wstrLineBuf.length();
				iNowLineWidthFromLastCurPoint = 0;	// �ʱ�ȭ
				//
#endif CLIENT_GLOBAL_EU_WORDWRAP
			} break;
		}

		if( iNowLineWidth >= iTextWidth && Q_NORMAL == eNowCharState )
		{
			//{{ 2010/10/28 ��ȿ��  WordWrap ���� �۾� 
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
			// �� ��� �ش� ������ ��� �ڸ� ����Ʈ�� ���� ����̴�. 
			if( iNowLineWidth <= iNowLineWidthFromLastCurPoint )
			{

				wstring wsTemp = L"-\n";

				
// 				if( 0 != wstrColorCode.length() )
// 				{
// 					wsTemp += wstrColorCode;
// 				}


				if( wstrLineBuf.length() != 0 )
				{
					int icuttingIndex = wstrLineBuf.size() - 1;
				
					// ���� ���� ���� ���� string�� addItem �Ǳ����� ����� �� �� ��Ʈ������ �������� �ʱ�ȭ�� �̿��
					wstring wstr_temp = L"";
					wstr_temp.insert(wstr_temp.begin(), wstrLineBuf.begin()+icuttingIndex, wstrLineBuf.end());


					wstrLineBuf.insert(wstrLineBuf.begin()+icuttingIndex, wsTemp.begin(), wsTemp.end());
					
					
					int ieraseIndex = icuttingIndex + wsTemp.size();
					wstrLineBuf.erase(wstrLineBuf.begin()+ieraseIndex, wstrLineBuf.end());

					pListBox->AddItem( wstrLineBuf.c_str(), NULL, iMegaID, wstrSenderName, bIsMyMessage );

					// �ʱ�ȭ //
					wstrLineBuf = L"  #CFFAE00";
					wstrLineBuf += wstr_temp;
				}

				// iNowLineWidth�� iNowLineWidthFromLastCurPoint ��ġȭ
				iNowLineWidth = iLastCharWidth;
				iNowLineWidthFromLastCurPoint = iLastCharWidth;
			}
			//
			else
			{
				wstring wsTemp = L"\n";

				
// 				if( 0 != wstrColorCode.length() )
// 				{
// 					wsTemp += wstrColorCode;
// 				}

				// end()�����ؼ� == �� ������ insert ������
				if( (int)wstrLineBuf.length() >= iLastCutPoint )
				{
					int icuttingIndex = iLastCutPoint;

					// ���� ���� ���� ���� string�� addItem �Ǳ����� ����� �� �� ��Ʈ������ �������� �ʱ�ȭ�� �̿��
					wstring wstr_temp = L"";
					wstr_temp.insert(wstr_temp.begin(), wstrLineBuf.begin()+icuttingIndex, wstrLineBuf.end());


					wstrLineBuf.insert(wstrLineBuf.begin()+icuttingIndex, wsTemp.begin(), wsTemp.end());


					int ieraseIndex = icuttingIndex + wsTemp.size();
					wstrLineBuf.erase(wstrLineBuf.begin()+ieraseIndex, wstrLineBuf.end());

					pListBox->AddItem( wstrLineBuf.c_str(), NULL, iMegaID, wstrSenderName, bIsMyMessage );

					// �ʱ�ȭ //
					wstrLineBuf = L"  #CFFAE00";
					wstrLineBuf += wstr_temp;
					
				}

				// iNowLineWidth�� iNowLineWidthFromLastCurPoint ��ġȭ
				iNowLineWidth = iNowLineWidthFromLastCurPoint;
			}

			iNowLineCount++;

			// �� ���� ù��° ���ڰ� �ִ� ��Ȳ�̴�. 
			bIsFirstCharOfLine = false;


			//}}
#else CLIENT_GLOBAL_EU_WORDWRAP
			pListBox->AddItem( wstrLineBuf.c_str(), NULL, iMegaID, wstrSenderName, bIsMyMessage );

			wstrLineBuf = L"  #CFFAE00";
			iNowLineCount++;
			iNowLineWidth = 0;
			bIsFirstCharOfLine = true;
#endif CLIENT_GLOBAL_EU_WORDWRAP

			CKTDGUIListBox::ListBoxItem* pItem = pListBox->GetLastItem();

			if( NULL != pItem )
			{
				pItem->m_bShow = true;
				pItem->m_fTimeLeftShow = MAGIC_CHAT_SHOW_TIME; 
			}

		}

		iNowCharIndex++;
	}

	if(iNowLineWidth != 0)
	{
		pListBox->AddItem( wstrLineBuf.c_str(), NULL, iMegaID, wstrSenderName, bIsMyMessage );
	}

	CKTDGUIListBox::ListBoxItem* pItem = pListBox->GetLastItem();
	if( NULL != pItem )
	{
		pItem->m_bShow = true;
		pItem->m_fTimeLeftShow = MAGIC_CHAT_SHOW_TIME; 
	}

	if( iLastTrackPos == iLastTrackEndPos ) 
	{
		pListBox->SetScrollBarEndPos();
	}

	return true;
}

bool CWordLineHandler::IsSpaceLatter( WCHAR wszCharBuf )
{
	if( wszCharBuf == L'' )
		return false;

	std::wstring wsplitLatters = L" \t";
	std::wstring::size_type nextPos = 0;

	std::wstring::size_type spos = wsplitLatters.find(wszCharBuf,nextPos);

	// �´� ������ ã���� �� //
	if( spos != std::wstring::npos ) 
	{
		return true;
	}

	return false;
}

// space �� �����ϰ� SplitLatter ������ ���� ã�� ���̴�. 
bool CWordLineHandler::IsSplitLatter( WCHAR wszCharBuf )
{

	if( wszCharBuf == L'' )
		return false;

	//std::wstring wsplitLatters = L"~!@#$%^&*()_+{}|:\"<>?`-=[]\\;',./";
	// ��ȣ�� WordWrap ���� ������ ��. ��� �ظ��ϸ� ��ȣ �տ� �����̽��� �־�����
	std::wstring wsplitLatters = L"~!@#$%^&*_+{}|:\"<>?`-=[]\\;',./";
	std::wstring::size_type nextPos = 0;

	std::wstring::size_type spos = wsplitLatters.find(wszCharBuf,nextPos);

	// �´� ������ ã���� �� //
	if( spos != std::wstring::npos ) 
	{
		return true;
	}

	return false;
}

#if defined(ELLIPSE_GLOBAL) || defined(ELLIPSE_CN)
std::wstring CWordLineHandler::GetStrByLineBreakInX2MainWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse)
{
	wstring wstrText = CWordLineHandler::GetStrByLineBreakInX2Main(pOrgStr, width, fontIndex);

	if(nLineNum < 1)
	{
		return wstrText;
	}

	wstring wstrTempName;
	int ilastCutPoint = wstrText.find(L"\n");
	int iPos = wstrText.find(L"\n");
	int lineNum = 0;

	while(1)
	{
		if(lineNum < nLineNum - 1)
		{
			if(iPos == wstring::npos || (int)wstrText.size() == iPos + 1)
			{
				wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, (int)wstrText.size() - ((ilastCutPoint + 1) * lineNum));

				bEllipse = false;
				break;
			}
			else
			{
				wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, ilastCutPoint + 1);
				iPos = wstrText.find(L"\n", ++iPos);
				lineNum++;
			}
		}
		else
		{
			if(iPos == wstring::npos || (int)wstrText.size() == iPos + 1)
			{
				if( (int)wstrText.size() > ((ilastCutPoint + 1) * lineNum) )
				{
					wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, (int)wstrText.size() - ((ilastCutPoint + 1) * lineNum));
				}				

				bEllipse = false;
				break;
			}
			else
			{
				wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, iPos - (ilastCutPoint + 1) * lineNum - 3);
				wstrTempName += L"...";

				bEllipse = true;
				break;
			}
		}
	}

	return wstrTempName;
}

#ifdef ELLIPSE_FIX
std::wstring CWordLineHandler::CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse, const WCHAR* pEllipseStr /*= L"..."*/)
#else // ELLIPSE_FIX
std::wstring CWordLineHandler::CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse)
#endif // ELLIPSE_FIX
{
	wstring wstrText = CWordLineHandler::BasicLineBreak(pOrgStr, width, fontIndex);

	if(nLineNum < 1)
	{
		return wstrText;
	}

	wstring wstrTempName;
	int ilastCutPoint = wstrText.find(L"\n");
	int iPos = wstrText.find(L"\n");
	int lineNum = 0;

	while(1)
	{
		if(lineNum < nLineNum - 1)
		{
			if(iPos == wstring::npos || (int)wstrText.size() == iPos + 1)
			{
				wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, (int)wstrText.size() - ((ilastCutPoint + 1) * lineNum));

				bEllipse = false;
				break;
			}
			else
			{
				wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, ilastCutPoint + 1);
				iPos = wstrText.find(L"\n", ++iPos);
				lineNum++;
			}
		}
		else
		{
			if(iPos == wstring::npos || (int)wstrText.size() == iPos + 1)
			{
				wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, (int)wstrText.size() - ((ilastCutPoint + 1) * lineNum));

				bEllipse = false;
				break;
			}
			else
			{
#ifdef ELLIPSE_FIX
				CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( fontIndex );
				if( pFont != NULL )
				{
					int iEllipseSize = pFont->GetWidth(pEllipseStr)*g_pKTDXApp->GetResolutionScaleX();
					int size = wstrText.length();
					if(wstrText.length() >= iPos - (ilastCutPoint + 1) * lineNum)
					{
						for(int i = 1;i < wstrText.length();i++)
						{
							if( wstrText[iPos - (ilastCutPoint + 1) * lineNum - i] == L'')
								int i = 0;

							WCHAR tmpWChar = wstrText[iPos - (ilastCutPoint + 1) * lineNum - i];
							int iUniCharSize = 0;
							iUniCharSize += pFont->GetWidth( tmpWChar )*g_pKTDXApp->GetResolutionScaleX();

							if( iUniCharSize >= iEllipseSize)
							{
								wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, iPos - (ilastCutPoint + 1) * lineNum - i);
								wstrTempName += pEllipseStr;

								bEllipse = true;
								break;
							}
						}
					}
				}				
#else // ELLIPSE_FIX
				wstrTempName += wstrText.substr((ilastCutPoint + 1) * lineNum, iPos - (ilastCutPoint + 1) * lineNum - 3);
				wstrTempName += L"...";

				bEllipse = true;
#endif // ELLIPSE_FIX
				break;
			}
		}
	}

	return wstrTempName;
}

#ifdef ELLIPSE_FIX
std::wstring CWordLineHandler::CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, const WCHAR* pEllipseStr/* = L"..."*/)
{
	bool bTemp = false;
	return CutStringWithEllipse(pOrgStr, width, fontIndex, nLineNum, bTemp, pEllipseStr);
}
#endif // ELLIPSE_FIX

#endif //defined(ELLIPSE_GLOBAL) || defined(ELLIPSE_CN)