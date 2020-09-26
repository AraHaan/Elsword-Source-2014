// Image.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Image.h"

#define _abs(x) ((x)>0?(x):-(x))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImage::CImage()
{
	m_pstInfo = NULL;
	int nMax = 80;

	for(int i=0 ; i<MAX_TABLE ; ++i)
	{
		m_pMaxTable[i] = nMax;
		nMax += 80;
	}
}

CImage::~CImage()
{

}

//////////////////////////////////////////////////////////////////////
// Note : �ʱ�ȭ
//////////////////////////////////////////////////////////////////////
void CImage::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

//////////////////////////////////////////////////////////////////////
// Note : �޸� DC�� �̹����� �����Ѵ� - �Ϸ� ���� ��ȭ���� ��Ʈ �Լ�.
//////////////////////////////////////////////////////////////////////
void CImage::CreateImage_ForDay( HDC hdc, const wchar_t* pToday, const int* pData, int nMaxData)
{
	int nYMax;				//íƮ�� Y�� �ִ밪
	int nX;					//�ӽ� ���� X��ǥ
	int nY;					//�ӽ� ���� Y��ǥ
	int nPlus;				//�ӽ� ����ġ
	int i;					//for������
	int nDiff;				//Y�� ����
	int nYCoord;			//�ӽ� ���� Y�� ��
	wchar_t strYCoord[20];	//�ӽ�

	HDC /*hdc, */hMemDC;
	HBITMAP hOldBitmap;
	HFONT hFont, hOldFont;
	HPEN hPen, hOldPen;

	//hdc = GetDC(m_hWnd);										//ȭ��DC�� ��´�
	m_hBitmap = CreateCompatibleBitmap(hdc, 550, 400);		//��Ʈ�ʻ���
	hMemDC = CreateCompatibleDC(hdc);						//�޸�DC�� ��´�
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);	//�޸�DC�� ��Ʈ���� �ø���

	//íƮ �׵θ�
	Rectangle(hMemDC, 0, 0, 550, 400);

	//íƮ ��ü
	SelectObject(hMemDC, GetStockObject(GRAY_BRUSH));
	Rectangle(hMemDC, 70, 70, 520, 330);
	
	nY = 70;
	for(i=0 ; i<7 ; ++i)
	{
		nY += 32;
		MoveToEx(hMemDC, 70, nY, NULL);
		LineTo(hMemDC, 520, nY);
	}

	//��Ʈ����
	hFont = CreateFont(14, 0, 900, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		VARIABLE_PITCH | FF_SWISS, L"�׷���");

	hOldFont = (HFONT)SelectObject(hMemDC, hFont);

	////////////////////
	//90�� ȸ���ؽ�Ʈ
	////////////////////
	//������
	TextOut(hMemDC, 10, 200, L"������", 6);

	//íƮ�� X�� ���ذ� (2�ð� ����)
	SetTextAlign(hMemDC, TA_RIGHT);
	nX = 65;
	nY = 335;
	nPlus = 37;

	for(i=0 ; i<=24 ; i+=2)
	{
		WCHAR strTime[20];
		wsprintf(strTime, L"%d:00", i);
		TextOut(hMemDC, nX, nY, strTime, static_cast<int>(wcslen(strTime)));
		nX += nPlus;
	}

	SetTextAlign(hMemDC, TA_LEFT);

	////////////////////
	//ȸ������ �ؽ�Ʈ
	////////////////////
	//��
	SelectObject(hMemDC, hOldFont);
	TextOut(hMemDC, 300, 380, L"min", 3);
	
	//��¥
	TextOut(hMemDC, 250, 10, pToday, static_cast<int>(wcslen(pToday)));

	//íƮ�� Y�� ���ذ� (�ӽ�)
	SetTextAlign(hMemDC, TA_RIGHT);

	//���̺�κ��� nYMax���� ���´�
	if(nMaxData > m_pMaxTable[MAX_TABLE-1])
		nYMax = m_pMaxTable[MAX_TABLE-1];
	else{
		for(i=0 ; i<MAX_TABLE ; ++i)
		{
			if(m_pMaxTable[i] >= nMaxData)
			{
				nYMax = m_pMaxTable[i];
				break;
			}
		}
	}

	//íƮ�� Y�� ���ذ�
	nDiff = nYMax / 8;
	nYCoord = nYMax;

	nPlus = 32;		//�ؽ�Ʈ ��ǥ ����ġ
	nX = 65;		//�ؽ�Ʈ x��ǥ �����ȼ�
	nY = 60;		//�ؽ�Ʈ y��ǥ �����ȼ�

	for(i=0 ; i<9 ; ++i)
	{
		wsprintf(strYCoord, L"%d", nYCoord);
		TextOut(hMemDC, nX, nY, strYCoord, static_cast<int>(wcslen(strYCoord)));

		nY += nPlus;
		nYCoord -= nDiff;
	}
	SetTextAlign(hMemDC, TA_LEFT);

	//DB�κ��� ���� �����͸� �׷����� ��Ÿ����.
	//������ �׷����� ����������
	SetMapMode(hMemDC, MM_ANISOTROPIC);
	SetWindowExtEx(hMemDC, 1440, nYMax, NULL);	//��ǥ����
	SetViewportExtEx(hMemDC, 450, 260, NULL);	//ȭ����� ���뿵��
	SetViewportOrgEx(hMemDC, 70, 329, NULL);	//���� ����
	
	hPen = CreatePen(PS_SOLID, 1, 0x88ff55);
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);

	MoveToEx(hMemDC, 0, -pData[0], NULL);
	for(i=0 ; i<1440 ; ++i)
	{
		//Y��ǥ ����ġ�� -����
		LineTo(hMemDC, i, -pData[i]);
	}

	BitBlt( hdc, 10, 10, 550, 500, hMemDC, -120, -200, SRCCOPY );

	//íƮ �����Ϸ�
	//�޸� ����
	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hPen);
	DeleteObject(hFont);
	DeleteDC(hMemDC);										//�޸�DC ����
	//ReleaseDC(m_hWnd, hdc);									//ȭ��DC ������
}

//////////////////////////////////////////////////////////////////////
// Note : �޸� DC�� �̹����� �����Ѵ� - �Ѵ� ���� ��ȭ���� ��Ʈ �Լ�.
//////////////////////////////////////////////////////////////////////

void CImage::CreateImage_ForMonth(const wchar_t* pToday, wchar_t* _pTerm[30], const int* pData, int nMaxData, int iDiff)
{
	int nYMax;				//íƮ�� Y�� �ִ밪
	int nX;					//�ӽ� ���� X��ǥ
	int nY;					//�ӽ� ���� Y��ǥ
	int nPlus;				//�ӽ� ����ġ
	int i;					//for������
	int nDiff;				//Y�� ����
	int nYCoord;			//�ӽ� ���� Y�� ��
	wchar_t strYCoord[20];		//�ӽ�

	HDC hdc, hMemDC;
	HBITMAP hOldBitmap;
	HFONT hFont, hOldFont;
	HPEN hPen1,hPen2, hOldPen;

	hdc = GetDC(m_hWnd);									//ȭ��DC�� ��´�
	m_hBitmap = CreateCompatibleBitmap(hdc, 550, 405);		//��Ʈ�ʻ���
	hMemDC = CreateCompatibleDC(hdc);						//�޸�DC�� ��´�
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);	//�޸�DC�� ��Ʈ���� �ø���

	//íƮ �׵θ�
	Rectangle(hMemDC, 0, 0, 550, 405);

	//íƮ ��ü
	SelectObject(hMemDC, GetStockObject(GRAY_BRUSH));
	Rectangle(hMemDC, 70, 70, 520, 330);

	nY = 70;
	for(i=0 ; i<7 ; ++i) //���� ���� 7ĭ���� �����Ѵ�.
	{
		nY += 32;
		MoveToEx(hMemDC, 70, nY, NULL);
		LineTo(hMemDC, 520, nY);
	}

	//��Ʈ����
	hFont = CreateFont(14, 0, 900, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		VARIABLE_PITCH | FF_SWISS, L"�׷���");

	hOldFont = (HFONT)SelectObject(hMemDC, hFont);

	////////////////////
	//90�� ȸ���ؽ�Ʈ
	////////////////////
	//������
	TextOut(hMemDC, 10, 200, L"�ִ� ����", 12);

	//íƮ�� X�� ���ذ�
	SetTextAlign(hMemDC, TA_RIGHT);
	nX = 65;
	nY = 335;
	nPlus = 15;

	//for( i = 30 ; i > 0 ; --i)
	for( i = 0 ; i < 30 ; i++ )
	{
		//SYSTEMTIME	st; //�ý��� Ÿ������ �켱 ���� ���� �ð��� ���Ѵ�.
		//FILETIME	ft; //���� Ÿ������ �ٲپ ��� ���ϰ� �ϱ�.
		//GetLocalTime(&st);
		//st.wHour = st.wMinute = st.wSecond = st.wMilliseconds = st.wDayOfWeek = 0; //�Ϸ������ �����.
		//st.wDay += iDiff;
		//SystemTimeToFileTime(&st,&ft);

		//INT64 i64; //64��Ʈ ���������� �ϳ��� ���ڷ� ǥ���Ѵ�.
		//i64 = ((INT64)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
		//i64 -= 864000000000*(i);

		//ft.dwHighDateTime = (DWORD)(i64 >> 32);
		//ft.dwLowDateTime  = (DWORD)(i64 & 0xffffffff);
		//FileTimeToSystemTime( &ft , &st );

		//char strDate[MAX_PATH];
		//sprintf( strDate, "%d%2d%2d", st.wYear, st.wMonth, st.wDay);
		//TextOut(hMemDC, nX, nY, strDate, strlen(strDate));
		//nX += nPlus;

		TextOut( hMemDC, nX, nY, _pTerm[i], 10 );
		nX += nPlus;
	}

	SetTextAlign(hMemDC, TA_LEFT);

	////////////////////
	//ȸ������ �ؽ�Ʈ
	////////////////////
	SelectObject(hMemDC, hOldFont);

	//��¥
	TextOut(hMemDC, 250, 10, pToday, static_cast<int>(wcslen(pToday)));

	//íƮ�� Y�� ���ذ� (�ӽ�)
	SetTextAlign(hMemDC, TA_RIGHT);

	//���̺�κ��� nYMax���� ���´�
	if(nMaxData > m_pMaxTable[MAX_TABLE-1])
		nYMax = m_pMaxTable[MAX_TABLE-1];
	else
	{
		for(i=0 ; i<MAX_TABLE ; ++i)
		{
			if(m_pMaxTable[i] >= nMaxData)
			{
				nYMax = m_pMaxTable[i];
				break;
			}
		}
	}

	//íƮ�� Y�� ���ذ�
	nDiff = nYMax / 8;
	nYCoord = nYMax;

	nPlus = 32;		//�ؽ�Ʈ ��ǥ ����ġ
	nX = 65;		//�ؽ�Ʈ x��ǥ �����ȼ�
	nY = 60;		//�ؽ�Ʈ y��ǥ �����ȼ�

	for(i=0 ; i<9 ; ++i)
	{
		wsprintf(strYCoord, L"%d", nYCoord);
		TextOut(hMemDC, nX, nY, strYCoord, static_cast<int>(wcslen(strYCoord)));

		nY += nPlus;
		nYCoord -= nDiff;
	}
	SetTextAlign(hMemDC, TA_LEFT);

	//DB�κ��� ���� �����͸� �׷����� ��Ÿ����.
	//������ �׷����� ����������
	SetMapMode(hMemDC, MM_TEXT);
	SetViewportOrgEx(hMemDC, 70, 70, NULL);	//���� ����

	hPen1 = CreatePen(PS_SOLID, 2, 0x88ff55);
	hPen2 = CreatePen(PS_SOLID, 2, RGB(255,0,0));//��� ���� �׸� ���̴�.
	hOldPen = (HPEN)SelectObject(hMemDC, hPen1);

	int iTop = 0;
	int iHeight = 260;

	MoveToEx(hMemDC, 0, iHeight - (int)(pData[0]*iHeight/nYMax) , NULL);
	for( i = 0 ; i < 30 ; i++ )
	{		
		//Y��ǥ ����ġ�� -����
		LineTo(hMemDC, 15*i, iHeight -(int)(pData[i]*iHeight/nYMax)  );
	}

	hOldPen = (HPEN)SelectObject(hMemDC, hPen2);

	int nSum = 0;
	for ( i=0 ; i < 30 ; i++ )
	{
		nSum += pData[i];
	}
	int nAverage = nSum / 30;
	MoveToEx(hMemDC,0,iHeight -(int)(nAverage*iHeight/nYMax),NULL);
	LineTo(hMemDC , 435,iHeight -(int)(nAverage*iHeight/nYMax));

	//íƮ �����Ϸ�
	//�޸� ����
	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hPen1);
	DeleteObject(hPen2);
	DeleteObject(hFont);
	DeleteDC(hMemDC);										//�޸�DC ����
	ReleaseDC(m_hWnd, hdc);									//ȭ��DC ������
}

//////////////////////////////////////////////////////////////////////
// Note : �޸� DC�� �̹����� �����Ѵ� - Mode Play Count�� ��Ʈ �Լ�.
//////////////////////////////////////////////////////////////////////
void CImage::CreateImage_ModeCount(const wchar_t * _pToday, wchar_t * _pppTable[128][2], int _iNumberOfData, int _iMaxY, int _total)
{
	const int ZERO		= 0;
	int iImageWidth		= 550;									// ������ �׸��� ��
	int iImageHeight	= 500;									// ������ �׸��� ����
	int iChartBegin_X	= 75;									// �׷��� ��Ʈ�� ������ X��ǥ
	int iChartEnd_X		= 525;									// �׷��� ��Ʈ�� ���� X��ǥ
	int iChartBegin_Y	= 50;									// �׷��� ��Ʈ�� ������ Y��ǥ
	int iChartEnd_Y		= 350;									// �׷��� ��Ʈ�� ���� Y��ǥ
	int iChartWidth		= ( iChartEnd_X - iChartBegin_X );		// �׷��� ��Ʈ�� ��
	int iChartHeight	= ( iChartEnd_Y - iChartBegin_Y );		// �׷��� ��Ʈ�� ����
	int iChartMaxNum	= ( (_iMaxY/100)*100+100 );				// ��Ʈ�� �ִ� �󸶱��� ǥ���� ���ΰ� ��Ÿ��
	int iSpace_X = 0;											// X�� ������ �� ��꿡 ������ ��������
	int	iSpace_Y = 0;											// Y�� ������ �� ��꿡 ������ ��������
	int iTmp_X = 0;												// X�� ���ϴ� �� ��꿡 ������ ��������
	int iTmp_Y = 0;												// Y�� ���ϴ� �� ��꿡 ������ ��������
	wchar_t strTmp[300];											// �ӽ� ���ڿ� ���� ����

	HDC hdc, hMemDC;
	HBITMAP hOldBitmap;
	HFONT hFont, hOldFont;
	HPEN hPen, hOldPen;

	hdc = GetDC(m_hWnd);													//ȭ��DC�� ��´�
	m_hBitmap = CreateCompatibleBitmap(hdc, iImageWidth, iImageHeight);		//��Ʈ�ʻ��� : 550 X 500
	hMemDC = CreateCompatibleDC(hdc);										//�޸�DC�� ��´�
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);					//�޸�DC�� ��Ʈ���� �ø���

	// íƮ �׵θ� : 550 X 500
	Rectangle(hMemDC, ZERO, ZERO, iImageWidth, iImageHeight);

	// íƮ ��ü	: 450 X 300
	SelectObject(hMemDC, GetStockObject(LTGRAY_BRUSH));
	Rectangle(hMemDC, iChartBegin_X, iChartBegin_Y, iChartEnd_X, iChartEnd_Y);
	Rectangle(hMemDC, (iChartBegin_X+1), (iChartBegin_Y+1), (iChartEnd_X-1), (iChartEnd_Y-1));

	// ��Ʈ�� ���� ���ؼ� �߱� : 9�� -> 10ĭ
	iSpace_Y = (iChartHeight/10);
	iTmp_Y	 = iChartBegin_Y;
	for( int i = 0 ; i < 9 ; i++ )
	{
		iTmp_Y += iSpace_Y;

		MoveToEx(hMemDC, iChartBegin_X, iTmp_Y, NULL);
		LineTo(hMemDC, iChartEnd_X, iTmp_Y);
	}

	//��Ʈ����
	hFont = CreateFont(12, 0, 900, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					   DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, L"�׷���");
	hOldFont = (HFONT)SelectObject(hMemDC, hFont);

	////////////////////
	//90�� ȸ���ؽ�Ʈ
	////////////////////
	// Mode Count ���
	TextOut(hMemDC, 8, 200, L"Mode Count", 10);

	// íƮ�� X�� ���ذ� ���
	if( _iNumberOfData != 0 )
	{
		SetTextAlign(hMemDC, TA_RIGHT);
		iTmp_X	 = iChartBegin_X + 5;
		iTmp_Y	 = iChartEnd_Y + 3;
		iSpace_X = (iChartWidth/_iNumberOfData);
		int iVal = 0;
		double dVal = 0.0;
		char strVal[10]="";
		for( int i = 0 ; i < _iNumberOfData ; i++ )
		{
			// Mode Name ����ϴ� �κ�
			iVal = _wtoi( _pppTable[i][1] );
			dVal = (double)iVal;
			wsprintf( strTmp, L"%s (%2.2f)", (_pppTable[i][0]), ((dVal/_total)*100) );
			TextOut(hMemDC, iTmp_X, iTmp_Y, strTmp, static_cast<int>(wcslen(strTmp)));

			iTmp_X += iSpace_X;
		}
	}
	SetTextAlign(hMemDC, TA_LEFT);

	////////////////////
	//ȸ������ �ؽ�Ʈ
	////////////////////
	SelectObject(hMemDC, hOldFont);

	// ���� ��¥ ���
	TextOut(hMemDC, 280, 9, _pToday, static_cast<int>(wcslen(_pToday)));

	// Total Mode Play Count ���
	wsprintf( strTmp, L"Total Mode played Count : %d", _total );
	TextOut(hMemDC, 200, 25, strTmp, 32);


	//íƮ�� Y�� ���ذ�
	SetTextAlign(hMemDC, TA_RIGHT);
	iTmp_X	 = iChartBegin_X - 5;
	iTmp_Y	 = iChartBegin_Y - 3;
	iSpace_Y = (iChartHeight/10);
	int iTmpMaxNum = iChartMaxNum ;
	for( int i = 0 ; i < 11 ; i++ )
	{
		wsprintf( strTmp, L"%d", iTmpMaxNum );
		TextOut(hMemDC, iTmp_X, iTmp_Y, strTmp, static_cast<int>(wcslen(strTmp)));

		iTmp_Y += iSpace_Y;					// ��� �� �� ���ΰ�
		iTmpMaxNum -= (iChartMaxNum/10);	// ������ �� �� ���ΰ�
	}

	//////////////////////////////////
	//ǥ �� �׷����� ���� �׷��� ���
	//////////////////////////////////
	hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(0,128,255));
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);

	int iTickness = 9;			// ������� ����
	int ihalfTickness = (iTickness/2);

	if( _iNumberOfData != 0 )
	{
		iTmp_X = ( iChartBegin_X + (11 - ihalfTickness) );
		iSpace_X = (iChartWidth/_iNumberOfData);
		double dTmp = 0.0;

		for( int i = 0 ; i < _iNumberOfData ; i++ )
		{
			// ������� ���̸� ���ؼ�
			wsprintf( strTmp, (_pppTable[i][1]) );
			dTmp = _wtoi( strTmp );
			iSpace_Y = (int)( (dTmp/iChartMaxNum)*iChartHeight - 4 );

			if( iSpace_Y < 1 )
			{	continue;	}

			// ���� �׸���.
			for( int j = 0 ; j < iTickness ; j++ )
			{
				iTmp_Y = ( iChartEnd_Y-3 );
				MoveToEx(hMemDC, (iTmp_X+j), iTmp_Y, NULL);
				iTmp_Y -= iSpace_Y;
				LineTo(hMemDC, (iTmp_X+j), iTmp_Y );
			}

			iTmp_X += iSpace_X;
		}
	}

	//íƮ �����Ϸ�
	//�޸� ����

	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hPen);
	DeleteObject(hFont);
	DeleteDC(hMemDC);										//�޸�DC ����
	ReleaseDC(m_hWnd, hdc);									//ȭ��DC ������
}

//////////////////////////////////////////////////////////////////////
// Note : �޸� DC�� �̹����� �����Ѵ� - Frame Rate�� �̹��� íƮ
//////////////////////////////////////////////////////////////////////
void CImage::createImage_FrameRate(const wchar_t* _pToday, wchar_t* _pTerm[30], double _dDataTable[6][30], int _iMaxData, int _iMinData, int _Index)
{
	const int ZERO		= 0;
	int iImageWidth		= 600;									// ������ �׸��� ��
	int iImageHeight	= 530;									// ������ �׸��� ����
	int iChartBegin_X	= 45;									// �׷��� ��Ʈ�� ������ X��ǥ
	int iChartEnd_X		= 500;									// �׷��� ��Ʈ�� ���� X��ǥ
	int iChartBegin_Y	= 40;									// �׷��� ��Ʈ�� ������ Y��ǥ
	int iChartEnd_Y		= 440;									// �׷��� ��Ʈ�� ���� Y��ǥ
	int iChartWidth		= ( iChartEnd_X - iChartBegin_X );		// �׷��� ��Ʈ�� ��
	int iChartHeight	= ( iChartEnd_Y - iChartBegin_Y );		// �׷��� ��Ʈ�� ����
	int iSpace_X = 0;											// X�� ������ �� ��꿡 ������ ��������
	int	iSpace_Y = 0;											// Y�� ������ �� ��꿡 ������ ��������
	int iTmp_X = 0;												// X�� ���ϴ� �� ��꿡 ������ ��������
	int iTmp_Y = 0;												// Y�� ���ϴ� �� ��꿡 ������ ��������
	double dTmp = 0.0;											// double�� ������ �� �� ���� �� ���� ����
	wchar_t strTmp[300];											// �ӽ� ���ڿ� ���� ����
	int iRGB_Set[6][3] = {{0,255,0},{255,128,0},{0,255,255},{255,0,255},{0,64,128},{128,0,0}};	// �׷��� �׸� �� ���� �� ����
	// �տ��� ���� : ���, ��Ȳ��, �ϴû�,  ��ũ��, ��û��, ����

	HDC hdc, hMemDC;
	HBITMAP hOldBitmap;
	HFONT hFont, hOldFont;
	HPEN hPen, hOldPen;

	hdc = GetDC(m_hWnd);													//ȭ��DC�� ��´�
	m_hBitmap = CreateCompatibleBitmap(hdc, iImageWidth, iImageHeight);		//��Ʈ�ʻ��� : 600 X 530
	hMemDC = CreateCompatibleDC(hdc);										//�޸�DC�� ��´�
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);					//�޸�DC�� ��Ʈ���� �ø���

	// íƮ �׵θ� : 550 X 500
	Rectangle(hMemDC, ZERO, ZERO, iImageWidth, iImageHeight);

	// íƮ ��ü	: 450 X 400
	SelectObject(hMemDC, GetStockObject(LTGRAY_BRUSH));
	Rectangle(hMemDC, iChartBegin_X-1, iChartBegin_Y-1, iChartEnd_X+1, iChartEnd_Y+1);
	Rectangle(hMemDC, iChartBegin_X, iChartBegin_Y, iChartEnd_X, iChartEnd_Y);

	// íƮ ���ؼ� �߱�
	int iMaxValue = static_cast<int>(_dDataTable[4][_iMaxData]);
	iMaxValue = ((iMaxValue/10)*10)+10;

	iSpace_Y = (iChartHeight/(iMaxValue/10));
	iTmp_Y	 = iChartBegin_Y;
	int iNum = (iMaxValue/10);
	for( int i = 0 ; i < iNum ; i++ )
	{
		iTmp_Y += iSpace_Y;

		MoveToEx(hMemDC, iChartBegin_X, iTmp_Y, NULL);
		LineTo(hMemDC, iChartEnd_X, iTmp_Y);
	}

	// ���� �ڽ�
	int iIntroBox_Begin_X	= iChartEnd_X+15;
	int iIntroBox_Begin_Y	= iChartBegin_Y;
	int iIntroBox_End_X		= iImageWidth-15;
	int iIntroBox_End_Y		= iChartBegin_Y+200;
	int iIntroBox_Height	= iIntroBox_End_Y - iIntroBox_Begin_Y;
	dTmp = iIntroBox_Height;

	SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));
	Rectangle(hMemDC, iIntroBox_Begin_X, iIntroBox_Begin_Y, iIntroBox_End_X, iIntroBox_End_Y);
	for( int i = 1 ; i < 7 ; i++ )
	{
		hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(iRGB_Set[i-1][0],iRGB_Set[i-1][1],iRGB_Set[i-1][2]));
		hOldPen = (HPEN)SelectObject(hMemDC, hPen);

		Rectangle(hMemDC, (iIntroBox_Begin_X+5), iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*i)-3,
				 (iIntroBox_Begin_X+12), iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*i)+3 );
		Rectangle(hMemDC, (iIntroBox_Begin_X+6), iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*i)-2,
				 (iIntroBox_Begin_X+10), iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*i)+2 );
	}

	//��Ʈ����
	hFont = CreateFont(12, 0, 900, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH | FF_SWISS, L"�׷���");
	hOldFont = (HFONT)SelectObject(hMemDC, hFont);

	////////////////////
	//90�� ȸ���ؽ�Ʈ
	////////////////////
	// Frame Rate ���
	TextOut(hMemDC, 8, 250, L"Frame Rate", 10);

	// íƮ�� X�� ���ذ�
	SetTextAlign(hMemDC, TA_RIGHT);
	iTmp_X	 = iChartBegin_X ;
	iTmp_Y	 = iChartEnd_Y + 5;

	dTmp = 0.0;
	TextOut( hMemDC, iTmp_X, iTmp_Y, _pTerm[0], 10 );
	for( int i = 0 ; i < _Index ; i++ )
	{
		dTmp = i;
		iTmp_X = iChartBegin_X + static_cast<int>((dTmp/(_Index-1))*iChartWidth) - 2;

		TextOut( hMemDC, iTmp_X, iTmp_Y, _pTerm[i], 10 );
	}

	///////////////////
	//ȸ������ �ؽ�Ʈ
	////////////////////
	SelectObject(hMemDC, hOldFont);

	// ���� ��¥ ���
	TextOut(hMemDC, 280, 9, _pToday, static_cast<int>(wcslen(_pToday)));

	// íƮ�� Y�� ���ذ�
	iMaxValue	= static_cast<int>(_dDataTable[4][_iMaxData]);
	iMaxValue	= ((iMaxValue/10)*10)+10;
	iSpace_Y	= (iChartHeight/(iMaxValue/10));
	iTmp_X		= iChartBegin_X - 5;
	iTmp_Y		= iChartBegin_Y - 5;
	iNum = (iMaxValue/10);
	for( int i = 0 ; i < iNum ; i++ )
	{
		wsprintf( strTmp, L"%d", iMaxValue );
		TextOut(hMemDC, iTmp_X, iTmp_Y, strTmp, 2);

		iTmp_Y += iSpace_Y;
		iMaxValue -= 10;
	}

	// ���� �ڽ��� ���� ����
	SetTextAlign(hMemDC, TA_LEFT);
	dTmp = iIntroBox_Height;
	int iIntroBox_Begin_Text_X = ( iIntroBox_Begin_X+18 );
	wsprintf( strTmp, L"- 20  " );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*1)-9, strTmp, 6);
	wsprintf( strTmp, L"20 - 30" );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*2)-9, strTmp, 7);
	wsprintf( strTmp, L"30 - 40" );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*3)-9, strTmp, 7);
	wsprintf( strTmp, L"40 - 50" );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*4)-9, strTmp, 7);
	wsprintf( strTmp, L"50 - 60" );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*5)-9, strTmp, 7);
	wsprintf( strTmp, L"60 -  " );
	TextOut(hMemDC, iIntroBox_Begin_Text_X, iIntroBox_Begin_Y+static_cast<int>((dTmp/7)*6)-9, strTmp, 6);


	////////////////////////
	// �׷��� �޺��� �׸���
	// ( ���� �׸��� ���� �׳��� �� ����ؼ� ���� �׸� : ������ �� �������� �߿����� �����Ƿ� )
	// 40-50 & 50-60 �׷����� �ְ�, ����, ����� ���ϰ� ���´�.
	int iBigGaps[30] = {0};		// ���� ���� ���̰� 5%�̻� ���� �� �� �׾��� ���
	int iBigGap_Index = 0;		// �������� �� ���� ����ִ��� �˷��� �ε���
	double d3_highest	= 0;	// 40-50 �׷������� �ְ�
	double d3_lowest	= 100;	// 40-50 �׷������� ������
	double d3_average	= 0;	// 40-50 �׷������� ��հ�
	double d4_highest	= 0;	// 50-60 �׷������� �ְ�
	double d4_lowest	= 100;	// 50-60 �׷������� ������
	double d4_average	= 0;	// 50-60 �׷������� ��հ�

	// 50-60 ������ �׷����� �����Ѵ�. �� �׷��� �������� Ȯ���ϸ� �� �� �� �����Ƿ�.
	for( int i = 0 ; i < _Index ; i++ )
	{
		if( i != 0 )
		{
			if( 3 < _abs(_dDataTable[4][i] - _dDataTable[4][i-1]) )
			{
				iBigGaps[iBigGap_Index] = (i-1);
				iBigGap_Index++;
			}
		}
		// 40-50 % 50-60 �׷��������� �ְ�, ����, ��հ��� ���Ѵ�.
		d3_average += _dDataTable[3][i];
		d4_average += _dDataTable[4][i];
		if( d3_highest < _dDataTable[3][i] ){	d3_highest = _dDataTable[3][i];   }
		if( d3_lowest > _dDataTable[3][i] ){	d3_lowest = _dDataTable[3][i];   }
		if( d4_highest < _dDataTable[4][i] ){	d4_highest = _dDataTable[4][i];   }
		if( d4_lowest > _dDataTable[4][i] ){	d4_lowest = _dDataTable[4][i];   }
	}
	// d3_average & d4_average ������ ����ִ� ���� _Index�� ������ ��հ��� ���ðشϴ�.
	d3_average /= _Index;
	d4_average /= _Index;

    // ������ ���� �������� ��ȭ���� ���� - �ְ�, ����, ���
	int iBrief_X = ( iChartEnd_X + 10 );
	int iBrief_Y = ( iIntroBox_End_Y + 10 );
	int iBrief_Y_Gap = 17;
	SetTextColor( hMemDC, RGB(iRGB_Set[4][0],iRGB_Set[4][1],iRGB_Set[4][2]) );	// ��û��
	wsprintf( strTmp, L"�ְ� %.2f", d4_highest );
	TextOut( hMemDC, iBrief_X, iBrief_Y, strTmp, 10 );
	wsprintf( strTmp, L"���� %.2f", d4_lowest );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(iBrief_Y_Gap), strTmp, 10 );
	wsprintf( strTmp, L"��� %.2f", d4_average );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(2*iBrief_Y_Gap), strTmp, 10 );
	SetTextColor( hMemDC, RGB(iRGB_Set[3][0],iRGB_Set[3][1],iRGB_Set[3][2]) );	// ��ũ��
	wsprintf( strTmp, L"�ְ� %.2f", d3_highest );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(3*iBrief_Y_Gap)+5, strTmp, 10 );
	wsprintf( strTmp, L"���� %.2f", d3_lowest );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(4*iBrief_Y_Gap)+5, strTmp, 10 );
	wsprintf( strTmp, L"��� %.2f", d3_average );
	TextOut( hMemDC, iBrief_X, iBrief_Y+(5*iBrief_Y_Gap)+5, strTmp, 10 );

	iMaxValue	= static_cast<int>(_dDataTable[4][_iMaxData]);		// �� ������ �ִ밪�� ���� ������, �� ������ �ִ밪�� �� ������ ������ �װ� ���� �߸��ȰŴ�.
	iMaxValue	= ((iMaxValue/10)*10)+10;

	//////////
	// �� �߱�

	// �Ͼ��� �߱� (���� ���̶� ���� 5% �̻� ���� ���)
	hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(255,255,255));
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);
	for( int i = 0 ; i < iBigGap_Index ; i++ )
	{
		dTmp = iBigGaps[i];
		iTmp_X = iChartBegin_X + static_cast<int>(((dTmp)/(_Index-1))*iChartWidth);
		iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[4][iBigGaps[i]]/iMaxValue)*iChartHeight);
		MoveToEx(hMemDC, iTmp_X, iTmp_Y, NULL);
		LineTo(hMemDC, iTmp_X, iChartEnd_Y);
	}

	// ������ �߱� ( 4�� �׷��������� ������ )
	hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(255,0,0));
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);
	dTmp = _iMinData;
	iTmp_X = iChartBegin_X + static_cast<int>((dTmp/(_Index-1))*iChartWidth);
	iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[4][_iMinData]/iMaxValue)*iChartHeight);
	MoveToEx(hMemDC, iTmp_X, iTmp_Y, NULL);
	LineTo(hMemDC, iTmp_X, iChartEnd_Y);

	// �Ķ��� �߱� ( 4�� �׷��������� �ְ� )
	hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(0,0,255));
	hOldPen = (HPEN)SelectObject(hMemDC, hPen);
	dTmp = _iMaxData;
	iTmp_X = iChartBegin_X + static_cast<int>((dTmp/(_Index-1))*iChartWidth);
	iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[4][_iMaxData]/iMaxValue)*iChartHeight);
	MoveToEx(hMemDC, iTmp_X, iTmp_Y, NULL);
	LineTo(hMemDC, iTmp_X, iChartEnd_Y);

	
	///////////////////////
	// ��Ʈ�� �׷��� �׸��� 
	iSpace_X	= (iChartWidth/_Index);
	dTmp = 0.0;
	for( int i = 0 ; i < 6 ; i++ )
	{
		hPen = CreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE, 1, RGB(iRGB_Set[i][0],iRGB_Set[i][1],iRGB_Set[i][2]));
		hOldPen = (HPEN)SelectObject(hMemDC, hPen);

		iTmp_X = iChartBegin_X;
		iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[i][0]/iMaxValue)*iChartHeight);
		if( iTmp_Y > iChartEnd_Y )
		{	iTmp_Y = iChartEnd_Y;   }

		Rectangle(hMemDC, (iTmp_X-2), (iTmp_Y-2), (iTmp_X+2), (iTmp_Y+2) );

		MoveToEx(hMemDC, iTmp_X, iTmp_Y, NULL);
		for( int j = 1 ; j < _Index ; j++ )
		{
			dTmp = j;
			iTmp_X = iChartBegin_X + static_cast<int>((dTmp/(_Index-1))*iChartWidth);
			iTmp_Y = iChartEnd_Y - static_cast<int>((_dDataTable[i][j]/iMaxValue)*iChartHeight);
			if( iTmp_Y > iChartEnd_Y )
			{	iTmp_Y = iChartEnd_Y-1;   }

			Rectangle(hMemDC, (iTmp_X-2), (iTmp_Y-2), (iTmp_X+2), (iTmp_Y+2) );
			LineTo(hMemDC, iTmp_X, iTmp_Y);
		}		
	}


	//íƮ �����Ϸ�
	//�޸� ����

	SelectObject(hMemDC, hOldPen);
	SelectObject(hMemDC, hOldBitmap);

	DeleteObject(hFont);
	DeleteDC(hMemDC);										//�޸�DC ����
	ReleaseDC(m_hWnd, hdc);									//ȭ��DC ������
}


//////////////////////////////////////////////////////////////////////
// Note : �޸� DC�� ������ BITMAP�������� �����Ѵ�
//////////////////////////////////////////////////////////////////////
void CImage::SaveBitmap(const wchar_t *strFilename)
{
	DDBToDIB();

	HANDLE hFile;
	DWORD dwWritten;

	//������ �����ϰ� ���� ����� ���� ����ü, �ȷ�Ʈ, ������ �����͸� ����Ѵ�
	hFile = CreateFile(strFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &m_stFileHeader, sizeof(m_stFileHeader), &dwWritten, NULL);
	WriteFile(hFile, m_pstInfo, m_dwSize, &dwWritten, NULL);

	CloseHandle(hFile);

	Delete();
}

//////////////////////////////////////////////////////////////////////
// Note : ��Ʈ�� ������ DIB���·� �����Ѵ�
//////////////////////////////////////////////////////////////////////
void CImage::DDBToDIB()
{
	BITMAP bit;
	HDC hdc;

	//ȭ�鿡 ���� DC�� ���Ѵ�
	hdc = GetDC(m_hWnd);

	//��Ʈ�� �ڵ�κ��� ��������ü�� �ʱ�ȭ�Ѵ�
	GetObject(m_hBitmap, sizeof(BITMAP), &bit);

	ZeroMemory(&m_stInfoHeader, sizeof(BITMAPINFOHEADER));

	m_stInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_stInfoHeader.biWidth = bit.bmWidth;
	m_stInfoHeader.biHeight = bit.bmHeight;
	m_stInfoHeader.biPlanes = 1;
	m_stInfoHeader.biBitCount = bit.bmPlanes*bit.bmBitsPixel;

	if(m_stInfoHeader.biBitCount > 8) 
		m_stInfoHeader.biBitCount = 24;

	m_stInfoHeader.biCompression = BI_RGB;

	//��������ü + �ȷ�Ʈ ũ�⸸ŭ �޸𸮸� �Ҵ��ϰ� �� ���ۿ�
	//��������ü�� �����Ѵ�
	m_nPalSize = (m_stInfoHeader.biBitCount == 24 ? 0:1 << m_stInfoHeader.biBitCount) * 
		sizeof(RGBQUAD);
	m_pstInfo = (BITMAPINFO*)malloc(m_stInfoHeader.biSize+m_nPalSize);
	m_pstInfo->bmiHeader = m_stInfoHeader;

	//��Ʈ���� ũ�⸦ ���Ѵ�
	GetDIBits(hdc, m_hBitmap, 0, bit.bmHeight, NULL, m_pstInfo, DIB_RGB_COLORS);
	m_stInfoHeader = m_pstInfo->bmiHeader;

	//��Ʈ�� ũ�Ⱑ �������� �ʾ������ ���۾����� ���� ����Ѵ�
	if(m_stInfoHeader.biSizeImage == 0){
		m_stInfoHeader.biSizeImage = ((((m_stInfoHeader.biWidth*m_stInfoHeader.biBitCount)
			+31) & ~31) >> 3) * m_stInfoHeader.biHeight;
	}

	//������ �����͸� �б����� �޸𸮸� ���Ҵ��Ѵ�
	m_dwSize = m_stInfoHeader.biSize + m_nPalSize + m_stInfoHeader.biSizeImage;
	m_pstInfo = (BITMAPINFO*)realloc(m_pstInfo, m_dwSize);

	//������ �����͸� �о���δ�
	GetDIBits(hdc, m_hBitmap, 0, bit.bmHeight, 
		(PBYTE)m_pstInfo+m_stInfoHeader.biSize+m_nPalSize,
		m_pstInfo, DIB_RGB_COLORS);

	//���� ����� �����
	m_stFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_nPalSize;
	m_stFileHeader.bfReserved1 = 0;
	m_stFileHeader.bfReserved2 = 0;
	m_stFileHeader.bfSize = m_dwSize + sizeof(BITMAPFILEHEADER);
	m_stFileHeader.bfType = 0x4d42;

	ReleaseDC(m_hWnd, hdc);
}

//////////////////////////////////////////////////////////////////////
// Note : �޸� ����
//////////////////////////////////////////////////////////////////////
void CImage::Delete()
{
	delete [] m_pstInfo;
	m_pstInfo = NULL;

	DeleteObject(m_hBitmap);
}