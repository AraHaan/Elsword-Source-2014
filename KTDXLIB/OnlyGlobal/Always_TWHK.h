// undef ����
// ���� ��� �¿ɿ� ������ �־ undef
// ��� ����(�ϸ� �߰� �� ��� ����) �� ���� undef Ǯ����� �մϴ�.
#undef ADJUST_SECRET_ITEM_OPTION
// undef ��

#if defined( _SERVICE_ ) || defined( _OPEN_TEST_ )	// �׽�Ʈ �� ���̺� ������ ����
#define	HACK_SHIELD
//#define ALWAYS_CHECK_KOM_LIVE_CHECK
#endif

#if defined( _SERVICE_ ) && !defined( _OPEN_TEST_ )	// ���̺� ������ ����
#define BEANFUN_UPGRADE_HOMEPAGE
#endif

#if !defined( _SERVICE_ )
#define SHOW_ATTACK_BOX_DUMMY
#endif

#ifdef HACK_SHIELD
#define	HS_LMP
#define HS_NO_ADMIN_NEED
#define HACKSHIELD_AUTO_UPDATE
#define HACKSHIELD_MORNITORING
#define ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
//#define ADDITIONAL_HS							// �޽��� ��ŷ ���� ������. �� �������� ����ϸ� �븸 IME�� �޽��� ��ŷ�� ���� ä���� �� �� �����Ƿ� ������� ����.
#define HS_REQUEST_OPTION
#endif HACK_SHIELD

#undef REMOVE_KR_SERVER_TEXTURE
#undef PC_BANG_BENEFIT_GUIDE
#undef NEXON_CASH_CHARGE_NEW
#undef NO_PRESENT_BUTTON
#undef PATCHER_RUN_ONLY

#define NO_GAMEGUARD
#define SCRIPT_SELECT
#define NEW_LOGIN_PROCESS						// �ؿ��� �α��� ���
#define _LANGUAGE_FIX_CHINESE_					// �߱���� ���ľ� �ϴ� �κ� ( �ϴ��� �г��� ���͸����� �ѱ�����->�߱������� )
#define _LANGUAGE_FIX_TW_HK						// ��� ��� ���� ������ ��ġ�� �κ�
#define ARGUMENT_LOGIN							// argv[1], [2] ������ ����α���, argv[1]�� ������ ��ó�� ����� ���� üũ �� �Ŵ��� �α���. �� ������ ����
#define USE_MAXLEVEL_LIMIT_VAL const int g_iMaxLevel = 70;	// MAXLevel ���� �� GameSysValTable.lua������ �� �����ؾ���
#define USE_CHANNEL_SERVER_IP_AT_SCRIPT			// ����� : [2012/6/14] //	SERVER_GROUP_UI_ADVANCED ���� �߱� ���� ������ ���� , �߱��� ���Ŀ��� �ƱԸ�Ʈ�� �޾ƿ�
#define CHEAT_WCHART_TO_UTF8					// 2012. 07. 06 lygan_������ // �߹� �� ĳ���� �г������� ġƮ ���� ���� ������ ġƮ �ȵǴ� ���� ����
#define HIDE_HAIR_BY_EP_AC_FACE2				// Ư�� ������(�Ǽ����� ��(��))�� ������ �� ������ �ʵ��� �ϱ� ���� �۾�(�ƴ����� ����)
#define SEARCH_UNIT_NICKNAME_TW					// �г������� ģ�� ã��
#define USE_FREE_TYPE_SUB_FONT					// ����Ÿ�� ���� ��Ʈ ���

#ifdef CLIENT_COUNTRY_TW
#undef REFORM_ENTRY_POINT_FOR_ONE_SERVER_GROUP
#endif CLIENT_COUNTRY_TW

#ifdef CLIENT_COUNTRY_HK
#undef SERVER_INTEGRATION_CHANNEL_NAME_FIX
#define REMOVE_INVITE_PARTY_SERVER_BUTTON		// �۾�����: ��Ƽ �ʴ� �˾����� �ַ��� , ���̾� ��ư ����
#endif CLIENT_COUNTRY_HK

//////////////////////////////////////////////////////////////////////////
// �̸�: ������
// ���̵�: ninil
// �۾����۳�¥: 2013-04-02
// ��ġ��¥: 2013-04-24
// �۾�����: ���� ī�� �̺�Ʈ. ����Ȯ���� ���� ��ȯ�Ǵ� ī��
// ��������: 9�� 5�� �ֹ�ö ���� �������� ��ȯ�ǰ� ����
/*
#define EVENT_MONSTER_CARD_SUMMON_ENEMY
#define EVENT_MONSTER_CARD_ALLY_ID	782	// �Ʊ� ���� ID
#define EVENT_MONSTAR_CARD_RATE		50		// ���� ��ȯ�� Ȯ��
#define EVENT_MONSTER_CARD_ENEMY_ID	783	// �� ���� ID
#define EVENT_MONSTER_CARD_MAX_ENENY	4	// ��ȯ�� ���� ��
*/
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �̸�: ����ȯ
// ���̵�: jhkim86
// �۾����۳�¥: 2013-05-21\
// ��ġ��¥: ����
// �۾�����: �ƴ㽺�� ����
//#define ALWAYS_EVENT_ADAMS_UI_SHOP
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �̸�: �ֹ�ö
// ���̵�: hohocmc
// �۾����۳�¥: 2014-01-07
// ��ġ��¥: 2014-01-15
// �۾�����: ���� Ư�� ���� ���� ������
#define EVENT_FIRE_CRACKER_THROW_ITEM
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �̸�: �ֹ�ö
// ���̵�: hohocmc
// �۾����۳�¥: 2013-05-10
// ��ġ��¥: 	
// �۾�����: �ؿ��� ���� ���κ극��Ŀ 
#define CLIENT_GLOBAL_LINEBREAK
//////////////////////////////////////////////////////////////////////////

// 2012.09.12 ��ȫ�� ������
#define ELLIPSE_GLOBAL

//////////////////////////////////////////////////////////////////////////
// �̸�: ������
// ���̵�: darkstarbt
// �۾����۳�¥: 2013-05-29
// ��ġ��¥: 2013-06-19
// �۾�����: Īȣ ���� �κ� ��ũ�� �ٴ� ���
#define TITLE_DESCRIPTION_RENEWAL
//////////////////////////////////////////////////////////////////////////