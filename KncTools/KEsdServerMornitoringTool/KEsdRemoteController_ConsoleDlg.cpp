// KEsdRemoteController_ConsoleDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "KEsdRemoteController_ConsoleDlg.h"
#include "KAccountDlg.h"
#include "KOptionDlg.h"
#include "KDateInfoDlg.h"
#include ".\kesdremotecontroller_consoledlg.h"
#include "CreateDBConnFileDlg.h"
#include "DBConnectionIDPWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKEsdRemoteController_ConsoleDlg ��ȭ ����

CKEsdRemoteController_ConsoleDlg::CKEsdRemoteController_ConsoleDlg(CWnd* pParent /*=NULL*/)
: CDialog(CKEsdRemoteController_ConsoleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_iTotalCCU = 0;
	m_iGuestIDTotalCCU = 0;
	m_iAuthLevel = 0;
	m_bEsdListUpdateLock = false;
}

void CKEsdRemoteController_ConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DATE_INFO, m_dateInfoBtn);
	DDX_Control(pDX, IDC_BUTTON_LAUNCH_CO2, m_centerOnlyBtn);
	DDX_Control(pDX, IDC_CHECK1, m_yesterdayCheck);
	DDX_Control(pDX, IDC_CHECK2, m_todayCheck);
	DDX_Control(pDX, IDC_TOTAL_CHECK, m_totalCheck);
	DDX_Control(pDX, IDC_AVERAGE_CHECK, m_averageCheck);
	DDX_Control(pDX, IDC_GUEST_CHECK, m_guestIDCheck);
	DDX_Control(pDX, IDC_LIST_ESD, m_listCtrlEx);
	DDX_Control(pDX, IDC_TOTAL_EDIT, m_editTotalCCU);
	DDX_Control(pDX, IDC_TOP_EDIT, m_editTopCCU);
	DDX_Control(pDX, IDC_GUEST_USER_EDIT, m_editGuestIDTotalCCU);
	DDX_Control(pDX, IDC_GUEST_TOP_EDIT, m_editGuestIDTopCCU);
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
	DDX_Control(pDX, IDC_YES_TOTAL, m_yesTotal);
	DDX_Control(pDX, IDC_TODAY_TOTAL, m_todayTotal);
	DDX_Control(pDX, IDC_TOTAL_STATIC, m_totalStatic);
	DDX_Control(pDX, IDC_TOP_STATIC, m_topStatic);
	DDX_Control(pDX, IDC_GUEST_USER_CCU, m_GuestIDStatic);
}

BEGIN_MESSAGE_MAP(CKEsdRemoteController_ConsoleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH2, OnBnClickedButtonLaunch)
	ON_BN_CLICKED(IDC_BUTTON_PATCH2, OnBnClickedButtonPatch)
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN2, OnBnClickedButtonShutdown)	
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH_CO2, OnBnClickedButtonLaunchCo)	
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN2, OnBnClickedButtonLogin2)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ESD, OnNMClickListEsd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ESD, OnLvnItemchangedListEsd)
	ON_COMMAND(ID_MAIN_EXIT, OnMainExit)
	ON_COMMAND(ID_MAIN_EXIT_2, OnMainExit)
	ON_COMMAND(ID_ACCOUNT_ACCOUNTDIALOG, OnAccountAccountdialog)
	
	ON_BN_CLICKED(IDC_TOTAL_CHECK, OnBnClickedTotalCheck)
	ON_BN_CLICKED(IDC_ALL_CHECK, OnBnClickedAllCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTcnSelchangeTab)
	ON_COMMAND(ID_MAIN_OPTION, OnMainOption)
	ON_COMMAND(ID_MAIN_OPTION32776, OnMainOption)
	ON_BN_CLICKED(IDC_BUTTON_DATE_INFO, OnBnClickedButtonDateInfo)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, OnBnClickedConnectButton)
	ON_BN_CLICKED(IDC_AVERAGE_CHECK, OnBnClickedAverageCheck)
ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickListEsd)
ON_BN_CLICKED(IDC_GUEST_CHECK, OnBnClickedGuestCheck)
ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnBnClickedButtonDisconnect)
ON_BN_CLICKED(IDC_BUTTON_HSB_UPDATE, OnBnClickedButtonHsbUpdate)
ON_BN_CLICKED(IDC_BUTTON_SVN_UPDATE, OnBnClickedButtonSvnUpdate)
ON_BN_CLICKED(IDC_BUTTON_WINDOWS_UPDATE, OnBnClickedButtonWindowsUpdate)
ON_BN_CLICKED(IDC_BUTTON_VIRUS_CHECK, OnBnClickedButtonVirusCheck)
ON_BN_CLICKED(IDC_BUTTON_DB_IDPW, OnBnClickedButtonDbIdpw)
END_MESSAGE_MAP()


// CKEsdRemoteController_ConsoleDlg �޽��� ó����

BOOL CKEsdRemoteController_ConsoleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	if ( theApp.m_kImp.Initialize() == false )
	{
		MessageBox( L"������ ������ �� �����ϴ�", L"����" );

		PostQuitMessage( 0 );
		return TRUE;
	}

	SetTimer( 0, TIMER_SEC, NULL );	

	InitEsdListCtrl();

	// ���� �׷��� �ʱ�ȭ	
	m_PushGraph.SetPeekRange(0, theApp.m_kImp.GetGraphMaxValue() );
	m_PushGraph.CreateFromStatic(IDC_STATICGRAPH, this);
	m_PushGraph.ModifyStyle(0, WS_THICKFRAME);
	m_PushGraph.SetGridSize( 20 ); // 24ĭ -> 1ĭ/20�ȼ�

	// ���� ���� �׷��� ���
	DrawLineYesterdayGraph();

	// ���� ���ݽð����� ���� �׷��� ���
	DrawLineTodayGraph();

	// �д� �ѹ��� ���� �׷��� ����	
	SetTimer( 1, theApp.m_kImp.GetGraphRefreshTime(), NULL );

	m_todayCheck.SetCheck( true );
	m_yesterdayCheck.SetCheck( true );
	m_totalCheck.SetCheck( true );

	// Tab �ʱ�ȭ
	std::vector< KESDServerGroupInfo >::const_iterator vit = theApp.m_kImp.GetServerGroupList().begin();
	for( ; vit != theApp.m_kImp.GetServerGroupList().end(); vit++ )
	{
		const KESDServerGroupInfo& info = *vit;
		m_tabCtrl.InsertItem( info.m_iServerGroupID, info.m_wstrGroupName.c_str() );
	}
	m_iServerGroupID = m_tabCtrl.GetCurSel();

	// ��ü ���� �׷��� ����
	m_yesTotal.SetBkColor( theApp.m_kImp.GetLineColor( theApp.m_kImp.GetTotalCCUYesLineColor() ) );
	m_todayTotal.SetBkColor( theApp.m_kImp.GetLineColor( theApp.m_kImp.GetTotalCCUTodayLineColor() ) );

	// AuthLevel�� Dialog �������.
	HWND hWnd;
	switch( m_iAuthLevel )
	{
	case AUTH_LEVEL::AL_NORMAL:
		{
			hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_PATCH2 );
			::ShowWindow( hWnd, SW_HIDE );
			hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_LOGIN2 );
			::ShowWindow( hWnd, SW_HIDE );
			hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_LAUNCH_CO2 );
			::ShowWindow( hWnd, SW_HIDE );
			hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_LAUNCH2 );
			::ShowWindow( hWnd, SW_HIDE );
			hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_SHUTDOWN2 );
			::ShowWindow( hWnd, SW_HIDE );
			hWnd = ::GetDlgItem( m_hWnd, IDC_STATIC_ONOFF );
			::ShowWindow( hWnd, SW_HIDE );
		}

	case AUTH_LEVEL::AL_SUPER:
		{
			CMenu newMenu;
			newMenu.LoadMenu(IDR_MENU1);
			AfxGetMainWnd()->SetMenu( &newMenu );
			// �޴��ٸ� �ٽ� �׷��ش�.
			AfxGetMainWnd()->DrawMenuBar();
			newMenu.Detach();
		}

	case AUTH_LEVEL::AL_ADMIN:		
		break;

	default:
		break;
	}

#ifdef COMPILE_CANT_CONNECT_SERVER
	hWnd = ::GetDlgItem( m_hWnd, IDC_CONNECT_BUTTON );
	::ShowWindow( hWnd, SW_HIDE );
#endif

#ifdef COMPILE_CANT_DATE_INFO
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_DATE_INFO );
	::ShowWindow( hWnd, SW_HIDE );
#endif COMPILE_CANT_DATE_INFO

#ifdef COMPILE_CANT_HSB_UPDATE
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_HSB_UPDATE );
	::ShowWindow( hWnd, SW_HIDE );	
#endif COMPILE_CANT_HSB_UPDATE

#ifdef COMPILE_CANT_CONTROL_SERVER
	//hWnd = ::GetDlgItem( m_hWnd, IDC_STATIC_STATS );
	//::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_DISCONNECT );
	::ShowWindow( hWnd, SW_HIDE );	

	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_PATCH2 );
	::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_LOGIN2 );
	::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_LAUNCH_CO2 );
	::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_LAUNCH2 );
	::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_SHUTDOWN2 );
	::ShowWindow( hWnd, SW_HIDE );
	//hWnd = ::GetDlgItem( m_hWnd, IDC_STATIC_ONOFF );
	//::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_SVN_UPDATE );
	::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_VIRUS_CHECK );
	::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_DB_IDPW );
	::ShowWindow( hWnd, SW_HIDE );
	hWnd = ::GetDlgItem( m_hWnd, IDC_BUTTON_WINDOWS_UPDATE );
	::ShowWindow( hWnd, SW_HIDE );
	//hWnd = ::GetDlgItem( m_hWnd, IDC_STATIC_STATS2 );
	//::ShowWindow( hWnd, SW_HIDE );

	CMenu newMenu;
	newMenu.LoadMenu(IDR_MENU1);
	AfxGetMainWnd()->SetMenu( &newMenu );
	// �޴��ٸ� �ٽ� �׷��ش�.
	AfxGetMainWnd()->DrawMenuBar();
	newMenu.Detach();
#endif

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CKEsdRemoteController_ConsoleDlg::DrawLineYesterdayGraph()
{
	std::vector< KESDServerInfo >& vecServerList = theApp.m_kImp.GetEsdServerList();
	std::vector< KESDServerInfo >::iterator vit = vecServerList.begin();

	int iCCU;
	for( ; vit != vecServerList.end(); vit++ )
	{
		KESDServerInfo& serverInfo = *vit;

		if( serverInfo.m_serverUID == -1 )
			continue;

		// �׷��� �׷��� ����
		m_PushGraph.RemoveYesterdayLine( serverInfo.m_serverUID );
		
		// �����ڼ� �α� ������ ���
		GraphIterator mit = m_mapYesterdayGraph.find( serverInfo.m_serverUID );
		if( mit == m_mapYesterdayGraph.end() )
		{	
			std::vector< CCUInfo > vecCCU;
			if( theApp.m_kImp.GetYesterdayConcurrentUserFromDB( serverInfo.m_serverUID, serverInfo.m_iFileDsnID, vecCCU, iCCU ) == false )
				continue;

			m_mapYesterdayGraph.insert( std::make_pair( serverInfo.m_serverUID, vecCCU ) );
			mit = m_mapYesterdayGraph.find( serverInfo.m_serverUID );
		}

		// ���� ����
		m_PushGraph.AddYesterdayLine( serverInfo.m_serverUID, theApp.m_kImp.GetLineColor( serverInfo.m_iYesterdayLineColorID ) );

		int average = 0, aveCnt = 0;
		std::vector< CCUInfo >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end(); vit++ )
		{
			average += vit->m_CCUcount;

			if( aveCnt < 2 )
			{
				aveCnt++;
			}
			else
			{
				average /= 3;
				m_PushGraph.PushYesterdayLine( average, serverInfo.m_serverUID );
				average = 0;
				aveCnt = 0;
			}
		}
	}

#ifdef COMPILE_FOR_MAIN_SERVER
	{
		// ��ü ������ ��
		m_PushGraph.RemoveYesterdayLine( TOTAL_CCU_ID );

		// ��ü �����ڼ� �α� ������ ���
		GraphIterator mit = m_mapYesterdayGraph.find( TOTAL_CCU_ID );
		if( mit == m_mapYesterdayGraph.end() )
		{	
			std::vector< CCUInfo > vecCCU;
			if( theApp.m_kImp.GetYesterdayConcurrentUserFromDB( TOTAL_CCU_ID, 2, vecCCU, iCCU ) == false )
			{
				MessageBox( L"DB�� ���� ���������� ��� ����!" );
				return;
			}

			m_mapYesterdayGraph.insert( std::make_pair( TOTAL_CCU_ID, vecCCU ) );
			mit = m_mapYesterdayGraph.find( TOTAL_CCU_ID );
		}

		// ���� ����
		m_PushGraph.AddYesterdayLine( TOTAL_CCU_ID, theApp.m_kImp.GetLineColor( theApp.m_kImp.GetTotalCCUYesLineColor() ) );

		int average = 0, aveCnt = 0;
		std::vector< CCUInfo >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end(); vit++ )
		{
			average += vit->m_CCUcount;

			if( aveCnt < 2 )
			{
				aveCnt++;
			}
			else
			{
				average /= 3;
				m_PushGraph.PushYesterdayLine( average, TOTAL_CCU_ID );
				average = 0;
				aveCnt = 0;
			}
		}
	}
#endif	

	// �׷��� ������Ʈ
	m_PushGraph.YesterdayLineUpdate();
}

void CKEsdRemoteController_ConsoleDlg::DrawLineTodayGraph()
{
	std::vector< KESDServerInfo >& vecServerList = theApp.m_kImp.GetEsdServerList();
	std::vector< KESDServerInfo >::iterator vit = vecServerList.begin();

	m_iTotalCCU = 0; // ��ü ������ �� �ʱ�ȭ
	int iCCU, iTopCCU = 0, iGuestIDTopCCU = 0;

	// ���� �ð� ���ϱ�
	time_t t0 = time(0);
	tm* t = localtime(&t0);

	// �ð��� ������ �Ǹ� �׷��� ���� �ٽ� ����
	if( t->tm_hour >= 23  &&  t->tm_min > 55  ||
		t->tm_hour <= 0  &&  t->tm_min < 5 )
	{
		// ���� �׷���
		m_mapYesterdayGraph.clear();
		DrawLineYesterdayGraph();

		// ���� �׷���
		m_mapTodayGraph.clear();

		// ü��ID �׷���
		m_mapGuestUserGraph.clear();
	}

	// ���� �� ������ ��
	for( ; vit != vecServerList.end(); vit++ )
	{
		KESDServerInfo& serverInfo = *vit;

		if( serverInfo.m_serverUID == -1 )
			continue;

		// �׷��� �׷��� ����
		m_PushGraph.RemoveLine( serverInfo.m_serverUID );
		
		// �����ڼ� �α� ������ ���
		GraphIterator mit = m_mapTodayGraph.find( serverInfo.m_serverUID );
		if( mit == m_mapTodayGraph.end() )
		{
			std::vector< CCUInfo > vecCCU;
			if( theApp.m_kImp.GetTodayConcurrentUserFromDB( serverInfo.m_serverUID, serverInfo.m_iFileDsnID, vecCCU, iCCU ) == false )
				continue;

			m_mapTodayGraph.insert( std::make_pair( serverInfo.m_serverUID, vecCCU ) );
			mit = m_mapTodayGraph.find( serverInfo.m_serverUID );
		}
		else
		{
			std::vector< CCUInfo >& vecCCU = mit->second;
			if( !vecCCU.empty() )
			{
				CCUInfo ccuInfo = vecCCU.back();

				if( ccuInfo.m_date.wHour < t->tm_hour  ||  ccuInfo.m_date.wMinute < t->tm_min )
				{
					std::wstring wstrDate = BOOST_FORMAT( ( L"%d-%02d-%02d %02d:%02d:00" )
						% ( t->tm_year + 1900 )
						% ( t->tm_mon + 1 )
						% t->tm_mday
						% ccuInfo.m_date.wHour
						% ccuInfo.m_date.wMinute );

					std::vector< CCUInfo > vecNowCCU;
					if( theApp.m_kImp.GetTimeConcurrentUserFromDB( serverInfo.m_serverUID, wstrDate, serverInfo.m_iFileDsnID, vecNowCCU, iCCU ) == false )
						continue;

					while( vecNowCCU.empty() == false )
					{
						CCUInfo ccuNewInfo = vecNowCCU.front();

						if( ccuInfo.m_date.wHour < ccuNewInfo.m_date.wHour  ||
							ccuInfo.m_date.wMinute < ccuNewInfo.m_date.wMinute )
						{
							vecCCU.push_back( ccuNewInfo );
							vecNowCCU.erase( vecNowCCU.begin() );
						}
						else
						{
							vecNowCCU.erase( vecNowCCU.begin() );
						}
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////

		// ���� ����
		m_PushGraph.AddLine( serverInfo.m_serverUID, theApp.m_kImp.GetLineColor( serverInfo.m_iTodayLineColorID ) );
		
		int average = 0, aveCnt = 0;
		std::vector< CCUInfo >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end(); vit++ )
		{
			average += vit->m_CCUcount;

			if( aveCnt < 2 )
			{
				aveCnt++;
			}
			else
			{
				average /= 3;
				m_PushGraph.Push( average, serverInfo.m_serverUID );
				average = 0;
				aveCnt = 0;

				// ������ �ð� üũ
				theApp.m_kImp.SetLastCheckTime( vit->m_date );
			}

			// �ִ� ������ ��
			if( iTopCCU < vit->m_CCUcount )
				iTopCCU = vit->m_CCUcount;
		}
		
		// �α��� ������� 0���� ����
		if( serverInfo.m_bIsLoginServer == true )
		{
			serverInfo.m_iNowTimeCCU = 0;
			continue;
		}
		
		// �����ӽź� ���ð� ������ ��
		if( mit->second.empty() == false )
		{
			

			//int iNowTimeCount = ( t->tm_mday * 24 ) + ( t->tm_hour * 60 ) + t->tm_min - CCU_TIME_GAP;
			//int iLastTimeCount = ( mit->second.back().m_date.wDay * 24 ) + ( mit->second.back().m_date.wHour * 60 ) + mit->second.back().m_date.wMinute;

			//if( iNowTimeCount > iLastTimeCount )
			//{
			//	serverInfo.m_iNowTimeCCU = 0;
			//}
			//else
			//{
			//	serverInfo.m_iNowTimeCCU = mit->second.back().m_CCUcount;
			//	m_iTotalCCU += mit->second.back().m_CCUcount;
			//}

			if( serverInfo.m_bOnlineGmServer )
			{
				serverInfo.m_iNowTimeCCU = mit->second.back().m_CCUcount;
				m_iTotalCCU += mit->second.back().m_CCUcount;
			}
			else
			{
				serverInfo.m_iNowTimeCCU = 0;
			}
		}
	}

#ifdef COMPILE_FOR_MAIN_SERVER
	int iAverageCCU = 0;
	{
		// ��ü ������ ��
		m_PushGraph.RemoveLine( TOTAL_CCU_ID );
		
		// �����ڼ� �α� ������ ���
		GraphIterator mit = m_mapTodayGraph.find( TOTAL_CCU_ID );
		if( mit == m_mapTodayGraph.end() )
		{
			std::vector< CCUInfo > vecCCU;
			if( theApp.m_kImp.GetTodayConcurrentUserFromDB( TOTAL_CCU_ID, 2, vecCCU, iCCU ) == false )
			{
				MessageBox( L"DB�� ���� ���������� ��� ����!" );
				return;
			}

			m_mapTodayGraph.insert( std::make_pair( TOTAL_CCU_ID, vecCCU ) );
			mit = m_mapTodayGraph.find( TOTAL_CCU_ID );
		}
		else
		{
			std::vector< CCUInfo >& vecCCU = mit->second;
			CCUInfo ccuInfo = vecCCU.back();

			if( ccuInfo.m_date.wHour < t->tm_hour  ||  ccuInfo.m_date.wMinute < t->tm_min )
			{
				std::wstring wstrDate = BOOST_FORMAT( ( L"%d-%02d-%02d %02d:%02d:00" )
					% ( t->tm_year + 1900 )
					% ( t->tm_mon + 1 )
					% t->tm_mday
					% ccuInfo.m_date.wHour
					% ccuInfo.m_date.wMinute );

				std::vector< CCUInfo > vecNowCCU;
				if( theApp.m_kImp.GetTimeConcurrentUserFromDB( TOTAL_CCU_ID, wstrDate, 2, vecNowCCU, iCCU ) == false )
				{
					MessageBox( L"DB�� ���� ���������� ��� ����!" );
					return;
				}

				while( vecNowCCU.empty() == false )
				{
					CCUInfo ccuNewInfo = vecNowCCU.front();

					if( ccuInfo.m_date.wHour < ccuNewInfo.m_date.wHour  ||
						ccuInfo.m_date.wMinute < ccuNewInfo.m_date.wMinute )
					{
						vecCCU.push_back( ccuNewInfo );
						vecNowCCU.erase( vecNowCCU.begin() );
					}
					else
					{
						vecNowCCU.erase( vecNowCCU.begin() );
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////

		// ���� ����
		m_PushGraph.AddLine( TOTAL_CCU_ID, theApp.m_kImp.GetLineColor( theApp.m_kImp.GetTotalCCUTodayLineColor() ) );

		int iTotalCount = 0;
		int average = 0, aveCnt = 0;
		std::vector< CCUInfo >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end(); vit++ )
		{
			// ��� ���� �׷����� ���� ī��Ʈ
			iTotalCount += vit->m_CCUcount;

			average += vit->m_CCUcount;

			if( aveCnt < 2 )
			{
				aveCnt++;
			}
			else
			{
				average /= 3;
				m_PushGraph.Push( average, TOTAL_CCU_ID );
				average = 0;
				aveCnt = 0;
			}

			// �ִ� ������ ��
			if( iTopCCU < vit->m_CCUcount )
				iTopCCU = vit->m_CCUcount;
		}

		// ���� ���ð� ������ ��
		m_iTotalCCU = mit->second.back().m_CCUcount;

		// ��� ���� ���ϱ�
		iAverageCCU = iTotalCount / static_cast<int>(mit->second.size());
	}

	{
		// ��ü ������ ��
		m_PushGraph.RemoveAverageLine( AVERAGE_CCU_ID );

		// ���� ����
		m_PushGraph.AddAverageLine( AVERAGE_CCU_ID, theApp.m_kImp.GetLineColor( theApp.m_kImp.GetAverageCCULineColor() ) );

		for( int iCnt = 0; iCnt < 480; iCnt++ )
		{
			m_PushGraph.PushAverageLine( iAverageCCU, AVERAGE_CCU_ID );
		}

		m_PushGraph.AverageLineUpdate();
	}
#endif	

#ifdef COMPILE_SHOW_GUEST_ID_CCU
	m_mapGuestUserGraph.clear();
	for( vit = vecServerList.begin(); vit != vecServerList.end(); vit++ )
	{
		KESDServerInfo& kESDServerInfo = *vit;

		if( !kESDServerInfo.m_bIsLoginServer )
			continue;

		// ��ü ������ ��
		m_PushGraph.RemoveGuestUserLine( kESDServerInfo.m_serverUID );

		// �����ڼ� �α� ������ ���
		GraphIterator mit = m_mapGuestUserGraph.find( kESDServerInfo.m_serverUID );
		if( mit == m_mapGuestUserGraph.end() )
		{
			std::vector< CCUInfo > vecCCU;
			if( theApp.m_kImp.GetGuestIDConcurrentUserFromDB( kESDServerInfo.m_serverUID, 2, vecCCU, iCCU ) == false )
			{
				MessageBox( L"DB�� ���� ���������� ��� ����!" );
				return;
			}

			m_mapGuestUserGraph.insert( std::make_pair( kESDServerInfo.m_serverUID, vecCCU ) );
			mit = m_mapGuestUserGraph.find( kESDServerInfo.m_serverUID );
		}
		//else
		//{
		//	std::vector< CCUInfo >& vecCCU = mit->second;
		//	CCUInfo ccuInfo = vecCCU.back();

		//	if( ccuInfo.m_date.wHour < t->tm_hour  ||  ccuInfo.m_date.wMinute < t->tm_min )
		//	{
		//		std::string strDate = BOOST_FORMAT( ( "%d-%02d-%02d %02d:%02d:00" )
		//			% ( t->tm_year + 1900 )
		//			% ( t->tm_mon + 1 )
		//			% t->tm_mday
		//			% ccuInfo.m_date.wHour
		//			% ccuInfo.m_date.wMinute );

		//		std::vector< CCUInfo > vecNowCCU;
		//		if( theApp.m_kImp.GetTimeConcurrentUserFromDB( kESDServerInfo.m_serverUID, strDate, 2, vecNowCCU, iCCU ) == false )
		//		{
		//			MessageBox( "DB�� ���� ���������� ��� ����!" );
		//			return;
		//		}

		//		while( vecNowCCU.empty() == false )
		//		{
		//			CCUInfo ccuNewInfo = vecNowCCU.front();

		//			if( ccuInfo.m_date.wHour < ccuNewInfo.m_date.wHour  ||
		//				ccuInfo.m_date.wMinute < ccuNewInfo.m_date.wMinute )
		//			{
		//				vecCCU.push_back( ccuNewInfo );
		//				vecNowCCU.erase( vecNowCCU.begin() );
		//			}
		//			else
		//			{
		//				vecNowCCU.erase( vecNowCCU.begin() );
		//			}
		//		}
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////

		// ���� ����
		m_PushGraph.AddGuestUserLine( kESDServerInfo.m_serverUID, theApp.m_kImp.GetLineColor( theApp.m_kImp.GetGuestUserCCULineColor() ) );

		int iTotalCount = 0;
		int average = 0, aveCnt = 0;
		std::vector< CCUInfo >::iterator vit = mit->second.begin();
		for( ; vit != mit->second.end(); vit++ )
		{
			// ��� ���� �׷����� ���� ī��Ʈ
			iTotalCount += vit->m_CCUcount;

			average += vit->m_CCUcount;

			if( aveCnt < 2 )
			{
				aveCnt++;
			}
			else
			{
				average /= 3;
				m_PushGraph.PushGuestUserLine( average, kESDServerInfo.m_serverUID );
				average = 0;
				aveCnt = 0;
			}

			// �ִ� ������ ��
			if( iGuestIDTopCCU < vit->m_CCUcount )
				iGuestIDTopCCU = vit->m_CCUcount;
		}

		// ���� ü��ID ���ð� ������ ��
		if( !mit->second.empty() )
		{
			m_iGuestIDTotalCCU = mit->second.back().m_CCUcount;
		}		
	}
#endif

	m_PushGraph.Update();

	///////////////////////////////////////////////////////// ���ð� ������ ��
	CString strTotalCCU;
	strTotalCCU.Format( L"%d", m_iTotalCCU );
	m_editTotalCCU.SetWindowText( strTotalCCU );

	/////////////////////////////////////////////////////////// �ִ� ������ ��
	CString strTopCCU;
	strTopCCU.Format( L"%d", iTopCCU );
	m_editTopCCU.SetWindowText( strTopCCU );

	////////////////////////////////////////////////// ü��ID ���ð� ������ ��
	CString strGuestIDTotalCCU;
	strGuestIDTotalCCU.Format( L"%d", m_iGuestIDTotalCCU );
	m_editGuestIDTotalCCU.SetWindowText( strGuestIDTotalCCU );

	//////////////////////////////////////////////////// ü��ID �ִ� ������ ��
	CString strGuestIDTopCCU;
	strGuestIDTopCCU.Format( L"%d", iGuestIDTopCCU );
	m_editGuestIDTopCCU.SetWindowText( strGuestIDTopCCU );
}

void CKEsdRemoteController_ConsoleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}

	CDialog::OnPaint();
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CKEsdRemoteController_ConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKEsdRemoteController_ConsoleDlg::InitEsdListCtrl()
{
	m_listCtrlEx.SetExtendedStyle(m_listCtrlEx.GetExtendedStyle()|LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 20;
		kCol.pszText = L"";

		m_listCtrlEx.InsertColumn(CI_CHECK_BOX, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 38;
		kCol.pszText = L"����";

		m_listCtrlEx.InsertColumn(CI_YESTERDAY_COLOR, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 38;
		kCol.pszText = L"����";

		m_listCtrlEx.InsertColumn(CI_TODAY_COLOR, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 100;
		kCol.pszText = L"ServerName";

		m_listCtrlEx.InsertColumn(CI_SERVER_NAME, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 100;
		kCol.pszText = L"IP";

		m_listCtrlEx.InsertColumn(CI_IP, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 70;
		kCol.pszText = L"Connect";

		m_listCtrlEx.InsertColumn(CI_CONNECT, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 70;
		kCol.pszText = L"ä�μ���";

		m_listCtrlEx.InsertColumn(CI_CHANNEL_SERVER, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 75;
		kCol.pszText = L"�α��μ���";

		m_listCtrlEx.InsertColumn(CI_LOGIN_SERVER, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 70;
		kCol.pszText = L"���ͼ���";

		m_listCtrlEx.InsertColumn(CI_CENTER_SERVER, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 256;
		kCol.cx = 70;
		kCol.pszText = L"���Ӽ���";

		m_listCtrlEx.InsertColumn(CI_GAME_SERVER, &kCol);
	}

	{
		LV_COLUMN kCol;
		ZeroMemory( &kCol, sizeof(kCol) );
		kCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		kCol.fmt = LVCFMT_LEFT;
		kCol.cchTextMax = 254;
		kCol.cx = 73;
		kCol.pszText = L"������ ��";

		m_listCtrlEx.InsertColumn(CI_NOWTIME_CCU, &kCol);
	}
}

void CKEsdRemoteController_ConsoleDlg::UpdateEsdListCtrl( int iServerGroupID, bool bIsRemoveAllItems, bool bForceRefresh )
{
	bool bIsPacketRecv = theApp.m_kImp.IsRefreshListBoxAndReset();

	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_LIST_ESD );
	
	if( bIsRemoveAllItems == true )
	{
		pkListEsd->DeleteAllItems();
		bIsPacketRecv = true;
	}

	for ( int iS = 0, iInsertCnt = 0, iExistCnt = 0; iS < theApp.m_kImp.GetEsdServerInfoCount(); ++iS )
	{
		KESDServerInfo* kInfo = theApp.m_kImp.GetEsdServerInfoAt( iServerGroupID, iS );
		if( kInfo == NULL )
			continue;

		if( kInfo->m_wstrIP.empty() )
			continue;

		// Ư�� ���� �׷츸 ���
		if( iServerGroupID != 0  &&  kInfo->m_iServerGroupID != iServerGroupID )
			continue;

		if ( pkListEsd->GetItemCount() < iInsertCnt + 1 )
		{
			InsertEsdListCtrl( *kInfo );
			iInsertCnt++;
		}
		
		if( bIsPacketRecv == true || bForceRefresh == true )
			UpdateEsdListCtrl( iExistCnt++, *kInfo );
	}
}

void CKEsdRemoteController_ConsoleDlg::UpdateEsdListCtrl( int iIdx, KESDServerInfo& kInfo )
{
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_LIST_ESD );

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_CONNECT;

		if ( kInfo.m_bConnected == true )
			kItem.pszText = L"true";
		else
			kItem.pszText = L"false";

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT;
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_CHANNEL_SERVER;
		if ( kInfo.m_bOnlineChServer == true )
			kItem.pszText = L"Online";
		else
			kItem.pszText = L"Offline";

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_LOGIN_SERVER;
		if ( kInfo.m_bOnlineLgServer == true )
			kItem.pszText = L"Online";
		else
			kItem.pszText = L"Offline";

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_CENTER_SERVER;
		if ( kInfo.m_bOnlineCnServer == true )
			kItem.pszText = L"Online";
		else
			kItem.pszText = L"Offline";

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT;
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_GAME_SERVER;
		if ( kInfo.m_bOnlineGmServer == true )
			kItem.pszText = L"Online";
		else
			kItem.pszText = L"Offline";

		pkListEsd->SetItem( &kItem );
	}

	{
		WCHAR strCCU[128] = L"";
		wsprintf( strCCU, L"%d", kInfo.m_iNowTimeCCU );

		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_NOWTIME_CCU;
		kItem.pszText = strCCU;

		pkListEsd->SetItem( &kItem );
	}

	//{
	//	LV_ITEM kItem;
	//	ZeroMemory( &kItem, sizeof(kItem) );
	//	kItem.mask = LVIF_TEXT;
	//	kItem.iItem = iIdx;
	//	kItem.iSubItem = CI_CONDITION;
	//	if ( kInfo.m_bCrashLgServer == true || kInfo.m_bCrashCnServer == true || kInfo.m_bCrashGmServer == true )
	//	{
	//		kItem.pszText = "���� �ٿ�!!!";
	//	}		
	//	else
	//	{
	//		kItem.pszText = "";
	//	}

	//	pkListEsd->SetItem( &kItem );
	//}
}

void CKEsdRemoteController_ConsoleDlg::InsertEsdListCtrl( KESDServerInfo& kInfo )
{
	WCHAR buf[256] = L"";
	if( kInfo.m_strYesterdayColor.empty() == true )
	{
		wsprintf( buf, L"%d %%", theApp.m_kImp.GetLineColor( kInfo.m_iYesterdayLineColorID ) );
		kInfo.m_strYesterdayColor = buf;
	}

	if( kInfo.m_strTodayColor.empty() == true )
	{
		wsprintf( buf, L"%d %%", theApp.m_kImp.GetLineColor( kInfo.m_iTodayLineColorID ) );
		kInfo.m_strTodayColor = buf;
	}
	
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_LIST_ESD );
	int iIdx = pkListEsd->GetItemCount();

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_CHECK_BOX;
		kItem.pszText = L"";

		pkListEsd->InsertItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_YESTERDAY_COLOR;
		kItem.pszText = const_cast<wchar_t*>(kInfo.m_strYesterdayColor.c_str());

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_TODAY_COLOR;
		kItem.pszText = const_cast<wchar_t*>(kInfo.m_strTodayColor.c_str());

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT;
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_SERVER_NAME;
		kItem.pszText = (wchar_t*) kInfo.m_wstrName.c_str();

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_IP;
		kItem.pszText = (wchar_t*) kInfo.m_wstrIP.c_str();

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_CONNECT;
		kItem.pszText = L"false";

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_CHANNEL_SERVER;
		kItem.pszText = L"Offline";

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_LOGIN_SERVER;
		kItem.pszText = L"Offline";

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_CENTER_SERVER;
		kItem.pszText = L"Offline";

		pkListEsd->SetItem( &kItem );
	}

	{
		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_GAME_SERVER;
		kItem.pszText = L"Offline";

		pkListEsd->SetItem( &kItem );
	}

	{
		WCHAR strCCU[128] = L"";
		wsprintf( strCCU, L"%d", kInfo.m_iNowTimeCCU );

		LV_ITEM kItem;
		ZeroMemory( &kItem, sizeof(kItem) );
		kItem.mask = LVIF_TEXT; 
		kItem.iItem = iIdx;
		kItem.iSubItem = CI_NOWTIME_CCU;
		kItem.pszText = strCCU;

		pkListEsd->SetItem( &kItem );
	}

	//m_listCtrlEx.InsertItem( 0, "" );

	//m_listCtrlEx.SetItemText( 0, CI_YESTERDAY_COLOR, const_cast<char*>(kInfo.m_strYesterdayColor.c_str()) );

	//m_listCtrlEx.SetItemText( 0, CI_TODAY_COLOR, const_cast<char*>(kInfo.m_strTodayColor.c_str()) );

	//m_listCtrlEx.SetItemText( 0, CI_SERVER_NAME, const_cast<char*>(kInfo.m_strName.c_str()) );

	//m_listCtrlEx.SetItemText( 0, CI_IP, const_cast<char*>(kInfo.m_strIP.c_str()) );

	//m_listCtrlEx.SetItemText( 0, CI_CONNECT, "false" );

	//m_listCtrlEx.SetItemText( 0, CI_LOGIN_SERVER, "Offline" );

	//m_listCtrlEx.SetItemText( 0, CI_CENTER_SERVER, "Offline" );

	//m_listCtrlEx.SetItemText( 0, CI_GAME_SERVER, "Offline" );

	//{
	//	LV_ITEM kItem;
	//	ZeroMemory( &kItem, sizeof(kItem) );
	//	kItem.mask = LVIF_TEXT; 
	//	kItem.iItem = iIdx;
	//	kItem.iSubItem = CI_CONDITION;
	//	kItem.pszText = "";

	//	pkListEsd->SetItem( &kItem );
	//}
}

void CKEsdRemoteController_ConsoleDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent == 0 ) // ��Ŷ ���� Ÿ�̸�
	{
		theApp.m_kImp.Update();

		UpdateEsdListCtrl( m_iServerGroupID );
	}
	else if( nIDEvent == 1 ) // �׷��� ���� Ÿ�̸�
	{
		DrawLineTodayGraph();

		UpdateEsdListCtrl( m_iServerGroupID, false, true );
	}

	CDialog::OnTimer(nIDEvent);
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonLaunch()
{
	theApp.m_kImp.OnEsdLaunch();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonPatch()
{
	//std::string strBuf = "[�˸�] ���õ� ��� ������ P4 Sync�� �޽��ϴ�!\n\n";
	//strBuf += "���� Ư�� �����̳� ��ũ��Ʈ�� Sync�� �ް�����ôٸ� �������� P4 Sync�� �����ñ� �ٶ��ϴ�!\n\n\n";
	//strBuf += "���� ���õ� ��� ������ P4 Sync�� �����ðڽ��ϱ�?!\n\n";

	//if( MessageBox( strBuf.c_str(), "�˸�!", MB_YESNO ) == IDNO )
	//	return;

	//theApp.m_kImp.OnEsdPatch();

	theApp.m_kImp.OnEsdLaunch_CH();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonShutdown()
{
	std::wstring strBuf = L"�ٽ� �ѹ� Ȯ���մϴ�.\n\n";
	bool bIsSelected = false;

	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_LIST_ESD );
	for ( int iL = 0; iL < pkListEsd->GetItemCount(); ++iL )
	{
		KESDServerInfo* kInfo = theApp.m_kImp.GetEsdServerInfoAt( m_iServerGroupID, iL );
		if( kInfo == NULL )
			continue;

		// m_bChecked�� true�� �������� ������ ��Ŷ�� ���۵ǹǷ� ����üũ�� �߿��ϴ�.
		if( kInfo->m_bChecked == true )
		{
			strBuf += kInfo->m_wstrName;
			strBuf += L"\n";
			bIsSelected = true;
		}
	}

	if( bIsSelected == true )
	{
		strBuf += L"\n���� �������� �����Ű�ڽ��ϱ�?";

		if( MessageBox( strBuf.c_str(), L"���!", MB_YESNO ) == IDNO )
			return;

		theApp.m_kImp.OnEsdShutdown();
		return;
	}

	MessageBox( L"�����ų ������ �������� �ʾҽ��ϴ�!", L"�˸�", MB_OK );
	return;
}

void CKEsdRemoteController_ConsoleDlg::OnNMClickListEsd(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CKEsdRemoteController_ConsoleDlg::OnLvnItemchangedListEsd(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);	

	*pResult = 0;
	bool bIsGraphUpdate = false;

	if ( m_bEsdListUpdateLock == true )
		return;

	KESDServerInfo* kInfo = theApp.m_kImp.GetEsdServerInfoAt( m_iServerGroupID, pNMLV->iItem );
	if( kInfo == NULL )
		return;

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		kInfo->m_bGraphSelected = true;
		bIsGraphUpdate = true;
	}
	else
	{
		kInfo->m_bGraphSelected = false;
		bIsGraphUpdate = true;
	}

	// üũ �Ǿ��� ��� ó��
	if ( pNMLV->uNewState & 0x00001000 )
	{
		kInfo->m_bChecked = false;
	}
	else if ( pNMLV->uNewState & 0x00002000 )
	{
		kInfo->m_bChecked = true;
	}

	if( bIsGraphUpdate == false )
		return;
	
	// ���õ� ���θ� �׷����� �׸����� -> �ϴ��� ��ü������ �׷������� ����
	std::vector< int >& vecSelectedLines = m_PushGraph.GetSelectedLines();
	vecSelectedLines.clear();
	std::vector< KESDServerInfo > vecServerList = theApp.m_kImp.GetEsdServerList();
	std::vector< KESDServerInfo >::iterator vit = vecServerList.begin();
	for( ; vit != vecServerList.end(); vit++ )
	{
		if( vit->m_bGraphSelected == true )
			vecSelectedLines.push_back( vit->m_serverUID );
	}
	vecSelectedLines.push_back( TOTAL_CCU_ID );
	vecSelectedLines.push_back( AVERAGE_CCU_ID );

	m_PushGraph.YesterdayLineUpdate();
	m_PushGraph.Update();
	UpdateEsdListSelected();
}

void CKEsdRemoteController_ConsoleDlg::UpdateEsdListSelected()
{
	m_bEsdListUpdateLock = true;

	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_LIST_ESD );
	for ( int iL = 0; iL < pkListEsd->GetItemCount(); ++iL )
	{
		KESDServerInfo* kInfo = theApp.m_kImp.GetEsdServerInfoAt( m_iServerGroupID, iL );
		if( kInfo == NULL )
			continue;

		if ( kInfo->m_bGraphSelected == true )
		{
			pkListEsd->SetItemState( iL, LVIS_SELECTED, LVIS_SELECTED );	
		}
		else
		{
			pkListEsd->SetItemState( iL, 0, LVIS_SELECTED );
		}
	}

	m_bEsdListUpdateLock = false;
}


BOOL CKEsdRemoteController_ConsoleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if( pMsg->message == WM_KEYDOWN )
	{       
		if ( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonLaunchCo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	theApp.m_kImp.OnEsdLaunch_CO();
}

void CKEsdRemoteController_ConsoleDlg::OnMainExit()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	EndDialog(IDCANCEL);
}

void CKEsdRemoteController_ConsoleDlg::OnAccountAccountdialog()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CKAccountDlg accountDlg;
	accountDlg.DoModal();
}
void CKEsdRemoteController_ConsoleDlg::OnBnClickedCheck1()
{
	bool bCheck = ( m_yesterdayCheck.GetCheck() == 0 ? false : true );

	m_PushGraph.SetDrawYesterdayLine( bCheck );
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedCheck2()
{
	bool bCheck = ( m_todayCheck.GetCheck() == 0 ? false : true );

	m_PushGraph.SetDrawTodayLine( bCheck );
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonLogin2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.m_kImp.OnEsdLaunch_LG();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedTotalCheck()
{
	bool bCheck = ( m_totalCheck.GetCheck() == 0 ? false : true );

	m_PushGraph.SetTotalCCUDraw( bCheck );
	m_PushGraph.YesterdayLineUpdate();
	m_PushGraph.Update();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedAverageCheck()
{
	bool bCheck = ( m_averageCheck.GetCheck() == 0 ? false : true );
	
	m_PushGraph.SetDrawAverageLine( bCheck );
	m_PushGraph.AverageLineUpdate();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedAllCheck()
{
	
}

void CKEsdRemoteController_ConsoleDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	ChangeTabInfo( m_tabCtrl.GetCurSel() );
	*pResult = 0;
}

void CKEsdRemoteController_ConsoleDlg::ChangeTabInfo( int iTabIndex )
{
	// �ٸ� �������� �����ߴٸ� ��� ������ connect üũ�� �ʱ�ȭ �Ѵ�
	ResetAllServerChecked(); // �ݵ�� ȣ���Ұ�!

	m_iServerGroupID = iTabIndex;
	UpdateEsdListCtrl( m_iServerGroupID, true );

	// �ٸ����� �����ص� üũ�ڽ��� ��� ���� �ǵ��� ���� [����� ���� ����]
	//CheckingCheckBox();

	//if( iTabIndex == 0 )
	//{
	//	m_editTotalCCU.ShowWindow( SW_SHOW );
	//	m_editTopCCU.ShowWindow( SW_SHOW );
	//	m_totalStatic.ShowWindow( SW_SHOW );
	//	m_topStatic.ShowWindow( SW_SHOW );
	//	m_totalCheck.ShowWindow( SW_SHOW );
	//	m_yesTotal.ShowWindow( SW_SHOW );
	//	m_todayTotal.ShowWindow( SW_SHOW );

	//	m_editGuestIDTotalCCU.ShowWindow( SW_SHOW );
	//	m_editGuestIDTopCCU.ShowWindow( SW_SHOW );
	//	m_guestIDCheck.ShowWindow( SW_SHOW );
	//	m_GuestIDStatic.ShowWindow( SW_SHOW );
	//}
	//else
	//{
	//	m_editTotalCCU.ShowWindow( SW_HIDE );
	//	m_editTopCCU.ShowWindow( SW_HIDE );
	//	m_totalStatic.ShowWindow( SW_HIDE );
	//	m_topStatic.ShowWindow( SW_HIDE );
	//	m_totalCheck.ShowWindow( SW_HIDE );
	//	m_yesTotal.ShowWindow( SW_HIDE );
	//	m_todayTotal.ShowWindow( SW_HIDE );

	//	m_editGuestIDTotalCCU.ShowWindow( SW_HIDE );
	//	m_editGuestIDTopCCU.ShowWindow( SW_HIDE );
	//	m_guestIDCheck.ShowWindow( SW_HIDE );
	//	m_GuestIDStatic.ShowWindow( SW_HIDE );
	//}
}

void CKEsdRemoteController_ConsoleDlg::CheckingCheckBox()
{
	CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_LIST_ESD );
	for ( int iL = 0; iL < pkListEsd->GetItemCount(); ++iL )
	{
		KESDServerInfo* kInfo = theApp.m_kImp.GetEsdServerInfoAt( m_iServerGroupID, iL );
		if( kInfo == NULL )
			continue;

		if( kInfo->m_bChecked == true )
		{
			pkListEsd->SetItemState( iL, 0x00002000, 0xffffffff );
		}
		else
		{
			pkListEsd->SetItemState( iL, 0x00001000, 0xffffffff );
		}
	}
}

void CKEsdRemoteController_ConsoleDlg::OnMainOption()
{
	KOptionDlg optionDlg;
	
	optionDlg.SetGraphMaxValue( m_PushGraph.GetMaxPeek() );
	INT_PTR nResponse = optionDlg.DoModal();

	if (nResponse == IDOK)
	{
		theApp.m_kImp.SetGraphMaxValue( optionDlg.GetGraphMaxValue() );
		m_PushGraph.SetPeekRange( 0, optionDlg.GetGraphMaxValue() );
		DrawLineYesterdayGraph();
		DrawLineTodayGraph();
	}
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonDateInfo()
{
	RECT mainWindowPos;
	GetWindowRect( &mainWindowPos );

	KDateInfoDlg dateInfoDlg( (int)theApp.m_kImp.GetGraphMaxValue() );
	dateInfoDlg.SetMainWindowPos( mainWindowPos );
	dateInfoDlg.DoModal();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedConnectButton()
{
	theApp.m_kImp.OnConnect();
}

void CKEsdRemoteController_ConsoleDlg::ResetAllServerChecked()
{
	std::vector< KESDServerInfo >& vecServerList = theApp.m_kImp.GetEsdServerList();
	std::vector< KESDServerInfo >::iterator vit = vecServerList.begin();

	for( ; vit != vecServerList.end(); ++vit )
	{
		KESDServerInfo& serverInfo = *vit;
		serverInfo.m_bChecked = false;
	}
}

void CKEsdRemoteController_ConsoleDlg::OnHdnItemclickListEsd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	
	if( phdr->iItem == 0 )
	{
		CListCtrl* pkListEsd = (CListCtrl*) GetDlgItem( IDC_LIST_ESD );
		for ( int iL = 0; iL < pkListEsd->GetItemCount(); ++iL )
		{
			KESDServerInfo* kInfo = theApp.m_kImp.GetEsdServerInfoAt( m_iServerGroupID, iL );
			if( kInfo == NULL )
				continue;

			if( kInfo->m_bChecked == true )
			{
				kInfo->m_bChecked = false;
				pkListEsd->SetItemState( iL, 0x00001000, 0xffffffff );
			}
			else
			{
				kInfo->m_bChecked = true;
				pkListEsd->SetItemState( iL, 0x00002000, 0xffffffff );
			}
		}
	}	

	*pResult = 0;
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedGuestCheck()
{
	bool bCheck = ( m_guestIDCheck.GetCheck() == 0 ? false : true );

	m_PushGraph.SetDrawGuestUserLine( bCheck );
	m_PushGraph.GuestUserLineUpdate();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonDisconnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CCreateDBConnFileDlg kCreateDBConnFileDlg;
	kCreateDBConnFileDlg.DoModal();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonHsbUpdate()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.m_kImp.OnEsdHSBUpdate();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonSvnUpdate()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdSVNUpdate();
}


void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonWindowsUpdate()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdWindowsUpdate();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonVirusCheck()
{
	// TODO: Add your control notification handler code here
	theApp.m_kImp.OnEsdVirusCheck();
}

void CKEsdRemoteController_ConsoleDlg::OnBnClickedButtonDbIdpw()
{
	// TODO: Add your control notification handler code here
	CDBConnectionIDPWDlg kDlg;
	kDlg.DoModal();

	std::string strID;
	std::string stdPW;
	kDlg.GetStringID( strID );
	kDlg.GetStringPW( stdPW );

	theApp.m_kImp.OnEsdDBConnectionIDPW( strID, stdPW );
}

