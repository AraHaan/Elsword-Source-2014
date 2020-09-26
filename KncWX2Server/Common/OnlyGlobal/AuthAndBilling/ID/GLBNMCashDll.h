// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� ����� 
// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� _EXPORTS ��ȣ��
// �����ϵǸ�, ������ DLL�� ����ϴ� �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������ 
// GLBNMCASHDLL_API �Լ��� DLL���� �������� ������ ����, �� DLL��
// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.
#ifdef GLBNMCASHDLL_EXPORTS
#define GLBNMCASHDLL_API __declspec(dllexport)
#else
#define GLBNMCASHDLL_API __declspec(dllimport)
#endif


#if !defined(_LIB) && !defined(_GLBNMCASHDLL)
	#ifndef _GLBNMCASHDLLNAME
		#ifdef	_WIN64
			#define _GLBNMCASHDLLNAME "GLBNMCashDll_x64"
		#else
			#define _GLBNMCASHDLLNAME "GLBNMCashDll_x86"
		#endif
	#endif //#ifndef _GLBNMCASHDLLNAME

	#ifdef _DLL
		// multithread DLL
		#pragma comment(lib, _GLBNMCASHDLLNAME "_MD")		
	#else
		// multithread
		#pragma comment(lib, _GLBNMCASHDLLNAME "_MT")		
	#endif // #ifdef _DLL
#endif //#if !defined(_LIB) && !defined(_GLBNMCASHDLL)


#ifdef GLBNMCASHDLL_EXPORTS
#define GLBNMCASHDLL_API __declspec(dllexport)
#else
#define GLBNMCASHDLL_API __declspec(dllimport)
#endif

namespace MarbleWorks
{
	namespace GlobalBillCash
	{
		namespace API
		{
			GLBNMCASHDLL_API int REQ_CIS__Balance( PCHAR  ShopCode,	PCHAR UserNo,	PCHAR RemainAmount, int AmtLen,
													PCHAR  CashType,	int TypeLen,	PCHAR ResCD,		int ResLen );
			GLBNMCASHDLL_API int REQ_CIS__Cancel( PCHAR ShopCode,		PCHAR  OrderNo, 		PCHAR UserNo,
												   PCHAR DeductNo,		PCHAR  CancelAmount,	PCHAR UserIP,
												   PCHAR CntryCD,		PCHAR  Fee,         	PCHAR UseAmount,
												   PCHAR AdminID,		PCHAR  CancelCode,		wchar_t* CancelReason,
												   PCHAR DeductType,	PCHAR ResCD,			int ResLen );
			GLBNMCASHDLL_API int REQ_CIS__Add( PCHAR	ShopCode,	PCHAR OrderNo,	PCHAR UserNo,
												wchar_t* UserID,	PCHAR UserIP,	PCHAR CntryCD,
												PCHAR EvtCode,		PCHAR PrjCode,  PCHAR AuthKey,
												PCHAR AddAmount,	int AmtLen,		PCHAR ResCD, int ResLen );
			GLBNMCASHDLL_API int REQ_CIS__Deduct( PCHAR ShopCode,		PCHAR OrderNo,		PCHAR DeductType,
												   PCHAR UserNo,		wchar_t* UserID,	PCHAR UserIP,
												   PCHAR CntryCD,		PCHAR SaleCode,		PCHAR SaleCond,
												   wchar_t* SaleName,	wchar_t* PKgInfo,	wchar_t* DeductList,
												   PCHAR DeductNoList,	int ListLen,		PCHAR ResCD, int ResLen );
			GLBNMCASHDLL_API int REQ_CIS__Gift( PCHAR ShopCode,		PCHAR OrderNo,			PCHAR DeductType,
												 PCHAR UserNo,			wchar_t* UserID,		PCHAR UserIP,
												 PCHAR CntryCD,			PCHAR SaleCode,			PCHAR SaleCond,
												 wchar_t* SaleName,		PCHAR RcvUserNo,		wchar_t* RcvUserID,
												 wchar_t* ServerName,	wchar_t* CharName,		PCHAR CharLevel,
												 wchar_t* PKgInfo,		wchar_t* DeductList,	PCHAR DeductNoList,
												 int ListLen,			PCHAR ResCD,			int ResLen );
		};
	};
};


