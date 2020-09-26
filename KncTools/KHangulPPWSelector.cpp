#include "StdAfx.h"

#include "KHangulPPWSelector.h"

bool KHanSungDivProcess( WORD kUnicode, char* pcResultChar )
{
	int  iChoSung, iJungSung, iJongSung;

	static char acChoSungTbl[] = "��������������������������������������";
	static char acJungSungTbl[] = "�������¤äĤŤƤǤȤɤʤˤ̤ͤΤϤФѤҤ�";
	static char acJongSungTbl[] = "  ������������������������������������������������������";

	static int iUnicodeBase = 0xAC00;
	static int iUnicodeLast = 0xD79F;

	// �ѱ��� �������� ����� 
	if (kUnicode < iUnicodeBase || kUnicode > iUnicodeLast)
		return false;

	kUnicode = kUnicode - iUnicodeBase;
	iChoSung = kUnicode / (21 * 28);
	kUnicode = kUnicode % (21 * 28);
	iJungSung = kUnicode / 28;
	kUnicode = kUnicode % 28;
	iJongSung = kUnicode;

	if ( pcResultChar != NULL )
	{
		int nX = 0;	
		strncpy( &pcResultChar[nX] , acChoSungTbl + iChoSung * 2 , 2); nX+=2;
		strncpy( &pcResultChar[nX] , acJungSungTbl + iJungSung * 2, 2 ); nX+=2;
		strncpy( &pcResultChar[nX] , acJongSungTbl + iJongSung * 2, 2 ); 
	}

	return true;
}

bool KHanSungDivWideChar( char* pcBuff, char* pcResultChar )
{
	WCHAR pcUnicode[1] = {0};

	// �ش� ���ڸ� �̿��ؼ� �����ڵ� ��ȣ�� ��´�.
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, pcBuff, 2, pcUnicode, 1 );

	return KHanSungDivProcess( (WORD) *pcUnicode, pcResultChar );
}

bool KHanSungDivUnicode( WCHAR* pcWord, char* pcResultChar )
{
	return KHanSungDivProcess( pcWord[0], pcResultChar );
}

bool KHanSungPostWordWideChar( const char* pcBuff_ )
{
	char* pcBuff = (char*) pcBuff_;
	int iLen = (int) strlen( pcBuff );
	pcBuff += iLen;

	for ( int iC = 0; iC < iLen; ++iC )
	{
		pcBuff -= 2;
		if ( *pcBuff != ' ' )
			break;
	}

	char szResult[7] = {0};
	if ( KHanSungDivWideChar( pcBuff, szResult ) == false )
		return false;

	return !( szResult[4] == ' ' && szResult[5] == ' ' );
}

bool KHanSungPostWordUnicode( const WCHAR* pcWord_ )
{
	WCHAR* pcWord = (WCHAR*) pcWord_;
	int iLen = (int) wcslen( pcWord );
	pcWord += iLen;

	for ( int iC = 0; iC < iLen; ++iC )
	{
		pcWord--;
		if ( *pcWord != L' ' )
			break;
	}

	char szResult[7] = {0};
	if ( KHanSungDivUnicode( pcWord, szResult ) == false )
		return false;

	return !( szResult[4] == ' ' && szResult[5] == ' ' );
}
