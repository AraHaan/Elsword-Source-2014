// ������ �����ϰ� ǥ������ �� �� �ִ� ������ �� �� �ΰ�
// �� �ٲ�� �ϴ� ������ �� ���� define�� ������ �մϴ�. by ������ 13.11.12

#define MAX_RETRY						64                          // �ִ� ��õ� Ƚ��
#define MAX_DELAY						128                         // ��õ� ���� ����

#define LOAD_CUR_FILE_BAR_X				48
#define LOAD_CUR_FILE_BAR_Y				517
#define LOAD_CUR_FILE_BAR_WIDTH			530
#define LOAD_CUR_FILE_BAR_HEIGHT		31

#define LOAD_CUR_FILE_PER_X				449
#define LOAD_CUR_FILE_PER_Y				285
#define LOAD_CUR_FILE_PER_WIDTH			60
#define LOAD_CUR_FILE_PER_HEIGHT		30

#define LOAD_NEW_CUR_FILE_BAR_X			119
#define LOAD_NEW_CUR_FILE_BAR_Y			493
#define LOAD_NEW_CUR_FILE_BAR_WIDTH		535
#define LOAD_NEW_CUR_FILE_BAR_HEIGHT	12

#define LOAD_ALL_FILE_BAR_X				48
#define LOAD_ALL_FILE_BAR_Y				517
#define LOAD_ALL_FILE_BAR_WIDTH			530
#define LOAD_ALL_FILE_BAR_HEIGHT		31

#define LOAD_NEW_ALL_FILE_BAR_X			119
#define LOAD_NEW_ALL_FILE_BAR_Y			517
#define LOAD_NEW_ALL_FILE_BAR_WIDTH		535
#define LOAD_NEW_ALL_FILE_BAR_HEIGHT	12

#define LOAD_ALL_FILE_PER_X				449
#define LOAD_ALL_FILE_PER_Y				305
#define LOAD_ALL_FILE_PER_WIDTH			60
#define LOAD_ALL_FILE_PER_HEIGHT		30

/// ������ ũ��
#define GAUGE_NEW_BAR_WIDTH				536
#define GAUGE_NEW_BAR_HEIGHT			11

#define BANNER_WIDTH					722
#define BANNER_HEIGHT					391
#define BANNER_X						9
#define BANNER_Y						45


#define PATCHER_FONT					L"Tahoma"
#define PATCHER_FONT_SIZE				14



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(PATCHER_COUNTRY_DE) || (PATCHER_COUNTRY_ES) || (PATCHER_COUNTRY_FR) || (PATCHER_COUNTRY_IT) || (PATCHER_COUNTRY_PL) || (PATCHER_COUNTRY_EN)
	#define PATCHER_COUNTRY_EU
#elif defined(PATCHER_COUNTRY_TW) || (PATCHER_COUNTRY_HK)
	#define PATCHER_COUNTRY_TWHK
#endif //PATCHER_COUNTRY_XX

// elsword.exe ��ü ��ġ�ϴ� ���. �⺻ on
#define PATCHER_SELF_PATCH

#ifdef PATCHER_COUNTRY_EU

	#define MUTEX_NAME "ElswordX2_EU"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "pxk19slammsu286nfha02kpqnf729ck"		// argv[]�� ���� �ڰ� �� ��Ʈ���� �¾ƾ� ���.
	#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_JP)

	// ���� ��ü ��ġ ��� �Ϻ� off
	#undef PATCHER_SELF_PATCH

	#define MUTEX_NAME "ElswordX2_JP"
	#define FOREIGN_CONFIG_FILE		// �ܺ����Ϸ� ��� �д� �κ� �߰�

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_TWHK)

	#undef PATCHER_FONT
	#undef PATCHER_FONT_SIZE
	#define PATCHER_FONT		L"DFPPOP1W7U-B5"
	#define PATCHER_FONT_SIZE	15

	#ifdef _SERVICE_
		#define BLOCK_MANUAL_LOGIN

		#ifdef _OPEN_TEST_
			#define BLOCK_MANUAL_LOGIN_BYPASS_FOR_TEST_SERVER
		#endif _OPEN_TEST_
	#endif

	#define LOAD_WEB_MAIN		// Main â�� Web �ε� ���� ������
	#ifdef PATCHER_COUNTRY_HK
		#define MUTEX_NAME "ElswordX2_HK"
		const WCHAR g_pcBannerLink[] = L"http://www.elsonline.com.hk/launcher/launcher.aspx";	// ȫ�� ���̺� URL
	#else //PATCHER_COUNTRY_HK
		#define MUTEX_NAME "ElswordX2_TW"
		const WCHAR g_pcBannerLink[] = L"https://tw.event.beanfun.com/ELSWORD/PATCH/PATCH.ASPX";		// �븸 ���̺� URL
	#endif //PATCHER_COUNTRY_HK

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_US)

	#define MUTEX_NAME "ElswordX2_US"

	//-- 2010. 12. 13.  ���� : XTRAP ���� flag
	#ifdef _SERVICE_
		#define LAUNCHER_USE_XTRAP
		#define PATCHER_RUN_ONLY "pxk19slammsu286nfha02kpqnf729ck"		// Ŭ�� ktdx.h�� ���Ͻ�ų ��. argv[]�� ���� �ڿ� �� ��Ʈ���� �ٿ��� Ŭ�� �����Ѵ�.
	#endif _SERVICE_

	#define LOAD_WEB_MAIN		// Main â�� Web �ε� ���� ������
	#if !defined(_SERVICE_)
		const WCHAR g_pcBannerLink[] = L"";														// �系 ��ĭ
	#elif defined(_OPEN_TEST_)
		const WCHAR g_pcBannerLink[] = L"http://launchertest.elswordonline.com/";	// �̱� �׼�
	#else //defined(_SERVICE_)
		const WCHAR g_pcBannerLink[] = L"http://launcher.elswordonline.com/";		// �̱� ����
	#endif //defined(_SERVICE_)

	//#define PATCHER_CHANNELING_AERIA// �Ƹ��� ä�θ� ���� �۾�. �Ƹ��� �߰� ���� ������ elsworld.exe �ٷ� ���� �ȵǵ���

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_TH)

	#define MUTEX_NAME "ElswordX2_TH"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "8f9slxa02nkp29ak1u26mqpcmshnf7k"		// argv[]�� ���� �ڰ� �� ��Ʈ���� �¾ƾ� ���.
	#endif _SERVICE_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_ID)

	#define MUTEX_NAME "ElswordX2_ID"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "8f9slxa02nkp29ak1u26mqpcmshnf7k"		// argv[]�� ���� �ڰ� �� ��Ʈ���� �¾ƾ� ���.
	#endif _SERVICE_

	#define LOAD_WEB_MAIN		// Main â�� Web �ε� ���� ������
	const WCHAR g_pcBannerLink[] = L"http://elsword.netmarble.co.id/launcher/launcher.html";

	#define DLL_COPY_RUN_ELSWORD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_BR)

	#define MUTEX_NAME "ElswordX2_BR"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "pxk19slammsu286nfha02kpqnf729ck"		// argv[]�� ���� �ڰ� �� ��Ʈ���� �¾ƾ� ���.
	#endif _SERVICE_

	#define LOAD_WEB_MAIN		// Main â�� Web �ε� ���� ������
	const WCHAR g_pcBannerLink[] = L"http://levelupgames.uol.com.br/Launchers/Elsword";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_PH)

	#define MUTEX_NAME "ElswordX2_PH"

	#ifdef _SERVICE_
		#define PATCHER_RUN_GARENA_MESSENGER // 2013.07.31 lygan_������ // �����ƽþ� ��ó ��������� ������ �޽����� ���ؼ� �������� �޾Ƽ� ó�� �ؾ� �Ѵ�.
	#endif _SERVICE_

	#define DLL_COPY_RUN_ELSWORD

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_IN)

	#define MUTEX_NAME "ElswordX2_IN"

	#ifdef _SERVICE_
		#define PATCHER_RUN_ONLY "8f9slxa02nkp29ak1u26mqpcmshnf7k"		// argv[]�� ���� �ڰ� �� ��Ʈ���� �¾ƾ� ���.
	#endif _SERVICE_

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else //PATCHER_COUNTRY_XX

	#define MUTEX_NAME "ElswordX2"

#endif //PATCHER_COUNTRY_XX
