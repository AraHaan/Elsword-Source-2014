#pragma once

#include "ServerDefine.h"
#include <vector>
#include <set>
#include "Event.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "CacheData.h"
#include "RefreshSingleton.h"

#define IN
#define OUT

//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
#include <KNCSingleton.h>
#include <boost/timer.hpp>

struct KJackPotCuponState
{
	std::wstring	m_wstrCuponNumber;
	UidType			m_iUserUID;
	UidType			m_iUnitUID;
	int				m_iState;

	KJackPotCuponState();

	KJackPotCuponState( IN const std::wstring& wstrCuponNumber, IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iState )
		: m_wstrCuponNumber( wstrCuponNumber )
		, m_iUserUID( iUserUID )
		, m_iUnitUID( iUnitUID )
		, m_iState( iState )
	{
	}
};

struct KJackPotEndProcessInfo
{
	std::wstring	m_wstrBeginDate;
	std::wstring	m_wstrEndDate;
	int				m_iValidCuponCount;

	KJackPotEndProcessInfo()
		: m_iValidCuponCount( 0 )
	{
	}

	KJackPotEndProcessInfo( IN const std::wstring& wstrBeginDate
						  , IN const std::wstring& wstrEndDate
						  , IN int iValidCuponCount
						  )
		: m_wstrBeginDate( wstrBeginDate )
		, m_wstrEndDate( wstrEndDate )
		, m_iValidCuponCount( iValidCuponCount )
	{
	}
};

class KJackPotEvent
{
public:
	enum CuponState
	{
		STATE0	= 0,
		STATE1	= 1,
	};

	KJackPotEvent( IN const std::wstring& wstrBeginDate, IN const std::wstring& wstrEndDate )
		: m_wstrBeginDate( wstrBeginDate )
		, m_wstrEndDate( wstrEndDate )
		, m_iValidCuponCount( 0 )
		, m_iBeginProcessGab( 0 )
		, m_bEventStart( false )
	{
	}
	~KJackPotEvent(){}

	bool	CheckTheBeginEndDate( IN const std::wstring& wstrBeginDate, IN const std::wstring& wstrEndDate ) const;
	void	EndProcess( OUT std::vector<KJackPotEndProcessInfo>& vecJackPotEndProcessInfo ) const;
	void	AddCupon( IN const std::wstring& wstrCuponNumber, IN const UidType iUserUID, IN const UidType iUnitUID, IN const int iState );
	bool	IsValidEvent( void ) const;
	bool	ExtractCuponNumber( IN const UidType iUserUID, IN const UidType iUnitUID, OUT std::wstring& wstrCuponNumber, OUT int& iState );
	//		- ���� �� Manager�� ��� �̺�Ʈ���� ������� ( ����� �� ��� )
	bool	BeginProcess( void );
	void	ConfirmProcess( void );

private:
	std::vector<KJackPotCuponState>	m_vecCuponInfo;
	std::wstring					m_wstrBeginDate;
	std::wstring					m_wstrEndDate;
	int								m_iValidCuponCount;
	
	boost::timer					m_tTimer;
	__int64							m_iBeginProcessGab;
	bool							m_bEventStart;
};

class KJackPotEventManager
{
	DeclareSingleton( KJackPotEventManager );

protected:
	KJackPotEventManager(){}
	~KJackPotEventManager(){}

public:
	bool	Init( IN const std::vector<KJackPotEventInitInfo>& vecJackpotEventInitInfo );
	void	Tick( OUT std::vector<KJackPotEventWinInfo>& vecJackPotEventWinInfo, OUT std::vector<KJackPotEndProcessInfo>& vecJackPotEndProcessInfo );

	void	AddUser( IN const UidType iUserUID, IN const UidType iUnitUID );
	void	RemoveUser( IN const UidType iUserUID, IN const UidType iUnitUID );
	bool	LotteryUser( OUT UidType& iUserUID, OUT UidType& iUnitUID );
	void	ReInsertUser( IN const UidType iUserUID );

	void	ReInsert( IN const KJackPotEventWinInfo& kJackPotEventWinInfo );
	void	FaildProcess( IN const UidType iUserUID, IN const UidType iUnitUID, IN const std::wstring& wstrCuponNumber );

private:
	std::vector<KJackPotEvent>	m_vecJackPotEvent;					// �Ⱓ�� ���� �̺�Ʈ ����

	std::map<UidType, UidType>	m_mapConnectUserAndUnitUID;			// ���� ���� ���� ���� ����
	std::set<UidType>			m_setWinnerUserUID;					// ��÷�� ����Ʈ
	std::map<UidType, UidType>	m_mapConnectUserInfoExceptWinner;	// ��÷�ڸ� ������ ���� ���� ���� ���� ( UserUID, UnitUID )

	boost::timer				m_tTimer;
};

DefSingletonInline( KJackPotEventManager );
#endif SERV_EVENT_JACKPOT
//}}

//{{ 2012. 09. 22	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_BINGO
class KGSBingoEventInfo
{
	DeclareRefreshSingleton( KGSBingoEventInfo );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

	KGSBingoEventInfo();
	~KGSBingoEventInfo();

public:
	void	AddPackageItemPriceForGift_LUA( IN unsigned long ulProductNo, IN unsigned long ulSalePrice );
	void	GetPackageItemPriceForGift( IN unsigned long ulProductNo, OUT unsigned long& ulSalePrice ) const;
	void	AddPackageItemPrice_LUA( IN unsigned long ulProductNo, IN unsigned long ulSalePrice );
	void	GetPackageItemPrice( IN unsigned long ulProductNo, OUT unsigned long& ulSalePrice ) const;
	void	AddPresentInfo_LUA( IN int iItemID, IN unsigned long ulProductNo, IN bool bIsBanPresent = false );
	bool	GetPresentProductNoFromItemID( IN const int iItemID, OUT unsigned long& ulProductNo ) const;
	int		GetBingoEventItemID( IN const unsigned long ulProductNo ) const;

	void	AddBanSpotInfo_LUA( IN byte bytePos );

	void	SetBillingIP( IN const std::string& strBillingIP ){	m_strBillingIP = strBillingIP;	}
	bool	CheckBillingIP_LUA( IN const char* szBillingIP ){	return m_strBillingIP == std::string( szBillingIP );	}

	void	GetPresentInfo( OUT std::vector<int>& vecPresent );
	void	GetBanPresentInfo( OUT std::vector<int>& vecBanPresent );
	bool	IsBanSpot( IN const byte byteSpotIndex );
	bool	IsBingoPresent( IN unsigned long ulProductNo );

private:
	std::map<unsigned long, unsigned long>	m_mapPackageItemPriceForGift;
	std::map<unsigned long, unsigned long>	m_mapPackageItemPrice;
	std::map<int, unsigned long>			m_mapAllPresentItemID;
	std::map<unsigned long, int>			m_mapAllPresentProductNo;
	std::set<unsigned long>					m_setPresent;
	std::set<unsigned long>					m_setBanPresent;
	std::set<byte>							m_setBanSpot;

	std::string								m_strBillingIP;
};
DefRefreshSingletonInline( KGSBingoEventInfo );

class KGSBingoEvent
{
	/*
	�� ���� ������ ���� ��ȭ�� ����� ����Ǵ����� Ȯ�� �غ��ƾ� �Ѵ�. 5x5 ȯ�濡�� �����ϴ� �͸��� ������.
	Ư�� ���ۿ��� �����Ѵٸ�, ���� DB�� ��ϵǾ� �ִ� ������ �ѹ�����...
	*/
public:
	enum BingoSetValue
	{
		BSV_BINGOBOARD_LINE_SIZE	= 5,
		BSV_BINGOBOARD_SLOT_NUM		= BSV_BINGOBOARD_LINE_SIZE * BSV_BINGOBOARD_LINE_SIZE,
		BSV_PRESENT_NUM				= 12,
		BSV_NUM_MIN					= 0,
		BSV_NUM_MAX					= 100,
		BSV_LOTTERY_BALANCE			= 900,
		BSV_MIX_RESTRICT_OPEN_COUNT	= 10,
		BSV_ACTION_LOG_LOTTERY		= 0,
		BSV_ACTION_LOG_MIX			= 1,
		BSV_ACTION_LOG_RESET		= 2,
		BSV_ACTION_LOG_INIT			= 3,
		BSV_ACTION_LOG_REWARD		= 4,
		BSV_MIX_ITEM_ID				= 257280,
		BSV_RESET_ITEM_ID			= 257290,
	};

	enum BingoState
	{
		BS_OK,
		BS_BUSY,
		BS_UNKNOWN,
		BS_NONEDATA,
		BS_NONE_CHANCE,
		BS_ERR_MIX_RESTRICT,
		BS_ERR_EXISTDATA,
		BS_ERR_MIX_THE_PRESENT,
	};

	static	bool	IsMixItem( IN const int iItemID ){	return iItemID == BSV_MIX_ITEM_ID;	}
	static	bool	IsResetItem( IN const int iItemID ){	return iItemID == BSV_RESET_ITEM_ID;	}

	KGSBingoEvent();

	bool		CheckBingoEventReward( IN const unsigned long& ulProductNo );
	bool		IsBingoComplete( void );

	// DB ������ �а� ����
	BingoState	GetBingoEventInfoForDB( IN const unsigned short usHandlingEventID, OUT KDBE_BINGO_EVENT_INFO_WRITE_REQ& kPacket );
	bool		Complete( IN const KDBE_BINGO_EVENT_INFO_WRITE_ACK& kPacket_ );


	BingoState	GetBingoEventInfo( OUT KEGS_BINGO_EVENT_INFO_ACK& kPacket ) const;
	// Set �Ǳ� ���� ������ ��ǰ�� ���� DB ������Ʈ�� �����Ѵٸ�, ������ ������ ���� ������ �ʴ´�.
	BingoState	SetBingoEventInfo( IN const UidType iUnitUID , IN const KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket_ );

	BingoState	GetBingoBoard( OUT std::vector<byte>& vecBingoBoard ) const;
	BingoState	GetPresentInfo( OUT std::vector<int>& vecPresentInfo ) const;
	
	BingoState	GetLotteryNum( IN const UidType iUnitUID );
	BingoState	MixTheBoard( IN const UidType iUnitUID );
	BingoState	ResetTheBoard( IN const UidType iUnitUID );
	BingoState	InitTheBoard( IN const UidType iUnitUID );

	void		AddBalance( IN const int iBalance );
	void		AddMixChance( IN const unsigned short iQuantity );
	void		AddResetChance( IN const unsigned short iQuantity );

	int			GetBalance( void ){	return m_iBalance;	}

	BingoState	GetProductNo( IN const byte iPos, OUT unsigned long& ulProductNo );
	int			GetProductItemID( IN const byte iPos );
	
	void		SetPresentInfoForBuy( IN const std::vector<byte>& vecAcquiredPresent );
	//void		GetPresentInfoForBuy( OUT std::vector<byte>& vecAcquiredPresent );

	KEGS_BINGO_EVENT_LOTTERY_ACK&	GetLotteryAck( void ){	return m_TempLotteryAck;	}
	KEGS_BINGO_EVENT_MIX_ACK&		GetMixAck( void ){		return m_TempMixAck;		}
	KEGS_BINGO_EVENT_RESET_ACK&		GetResetAck( void ){	return m_TempResetAck;		}
	KEGS_BINGO_EVENT_INIT_ACK&		GetInitAck( void ){		return m_TempInitAck;		}

	// ���� ���� �������� ���� ó����
	void		AddOrderNoListForPresentedItem( IN const unsigned long ulOrderNo ){	m_setPresentOrderNoList.insert( ulOrderNo );	}
	void		ClearOrderNoListForPresentedItem( void ){	m_setPresentOrderNoList.clear();	}
	bool		IsPresentedItem( IN const unsigned long ulOrderNo ) const{	return m_setPresentOrderNoList.find( ulOrderNo ) != m_setPresentOrderNoList.end();	}

	void		AddOrderNoForPresentedItem( IN const unsigned long ulOrderNo ){	m_vecPresentOrderNo.push_back( ulOrderNo );	}
	bool		CheckAndRemoveIsPresentedItem( IN const unsigned long ulOrderNo );

	void		AddCurrentOrderNo( IN const unsigned long ulOrderNo ){	m_setCurrentOrderNoList.insert( ulOrderNo );	}
	void		ClearCurrentOrderNoList( void ){	m_setCurrentOrderNoList.clear();	}
	bool		IsCurrentOrderNoItem( IN const unsigned long ulOrderNo ) const{	return m_setCurrentOrderNoList.find( ulOrderNo ) != m_setCurrentOrderNoList.end();	}

	// ���� ������ ���� ���� ���� ������ ó����
	bool		BingoEventRewardRepeater( IN const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo );
	bool		IsBingoEventRewardRepeater( IN const std::vector< KNXBTPurchaseReqInfo >& vecPurchaseReqInfo );

private:
	void		_MixTheBoard( IN const bool bOnlyMove );
	void		_MixTheBoard( IN const bool bOnlyMove, OUT std::vector<byte>& vecOpenNum );
	BingoState	_MixThePresent( void );
	int			_InsertLotteryNum( IN const byte byteNum, IN OUT std::vector<byte>& vecAcquiredPresent );
	
	void		SetTheBingoBoard( IN const std::vector<byte>& vecBingoBoard );
	void		SetThePresentInfo( IN const std::vector< std::pair<int, bool> >& vecPresentInfo );

	void		CopyTheBingoBoard( OUT std::vector<byte>& vecBingoBoard ) const;
	void		CopyThePresentInfo( OUT std::vector< std::pair<int, bool> >& vecPresentInfo ) const;

	void		WriteEventLog(	IN const UidType iUnitUID, 
								IN const byte iActionType, 
								IN const char iOpenNum,	
								IN const char iPos, 
								IN const int iItemID
								);
private:
	// üũ ����
	bool															m_bBusy;
	std::set<int>													m_setGetThePresent;

	// ���� ����
	KCacheData<int>													m_iBalance;			// �ܾ� ����
	KCacheData<int>													m_iLotteryChance;	// �̱� ���� Ƚ��
	KCacheData<int>													m_iLotteryRecord;	// ���� Ƚ��
	KCacheData<int>													m_iMixChance;		// �ڼ��� ���� Ƚ��
	KCacheData<int>													m_iResetChance;		// �ʱ�ȭ ���� Ƚ��
	std::vector< KCacheData<byte> >									m_vecBingoBoard;	// ���� ���� ������
	std::vector< std::pair< KCacheData<int>, KCacheData<bool> > >	m_vecPresentInfo;	// ���� ���� ����
	std::vector<KBingoEventLog>										m_vecLog;			// �α� ����

	// Ŭ�󿡰� ������ ���� �ӽ� ����
	KEGS_BINGO_EVENT_LOTTERY_ACK									m_TempLotteryAck;
	KEGS_BINGO_EVENT_MIX_ACK										m_TempMixAck;
	KEGS_BINGO_EVENT_RESET_ACK										m_TempResetAck;
	KEGS_BINGO_EVENT_INIT_ACK										m_TempInitAck;

	// �˻翡 �ʿ��� �ӽ� ����
	// 1. ĳ�� �κ��丮 ����Ʈ�� �޾ƿ� ������ �ʱ�ȭ �� ���� ���� ������ ����Ʈ �ӽ� ����
	// 2. �������� �õ��� ��, ���� ���� �������̶��, �ӽ� ����
	// 3. �������� �Ϸ� ��, �õ� �� ������ ���� ���� �������̶��, ���� ó��
	std::set<unsigned long>											m_setCurrentOrderNoList;	// ���� ���� �ִ� �������� ĳ�� �����۸� ������ �� �ִ�.
	std::set<unsigned long>											m_setPresentOrderNoList;	// ���� ���� �������� ���� ó���� ( ���� ���� �����ۿ��� ���� ��ȸ�� �����Ǹ� �ȵȴ�. )
	std::vector<unsigned long>										m_vecPresentOrderNo;		// ���� ���� �������� ���� ó���� ( ���� ���� �����ۿ��� ���� ��ȸ�� �����Ǹ� �ȵȴ�. )

	// ���� ������ ���� ���� ���� ������ ó����
	std::vector< std::pair< std::vector< KNXBTPurchaseReqInfo >, byte > >	m_vecRewardRepeateInfo;
};
#endif SERV_EVENT_BINGO
//}}