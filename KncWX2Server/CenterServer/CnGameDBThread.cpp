#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "CnGameDBThread.h"
#include "CnServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "Enum/Enum.h"
#include "CnSimLayer.h"
#include "X2Data/XSLUnit.h"
#include "X2Data/XSLItem.h"

#define CLASS_TYPE KCnGameDBThread
ImplPfID( KCnGameDBThread, PI_CN_GAME_DB );

IMPL_PROFILER_DUMP( KCnGameDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		

		DO_QUERY_NO_PROFILE( L"exec dbo.gup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);

		continue;

end_proc:
		START_LOG( cerr, vecDump[ui].m_wstrQuery )
			<< BUILD_LOG( vecDump[ui].m_iMinTime )
			<< BUILD_LOG( vecDump[ui].m_iMaxTime )
			<< BUILD_LOG( vecDump[ui].m_iTotalTime )
			<< BUILD_LOG( vecDump[ui].m_iQueryCount )
			<< BUILD_LOG( iAvg )
			<< BUILD_LOG( vecDump[ui].m_iOver1Sec )
			<< BUILD_LOG( vecDump[ui].m_iQueryFail )
			<< END_LOG;
	}
}

KCnGameDBThread::~KCnGameDBThread(void)
{
}

void KCnGameDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
		//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
#ifdef SERV_TRADE_DDOS_DEFENCE
		CASE( DBE_DB_UPDATE_TRADE_RESULT_REQ );
#endif SERV_TRADE_DDOS_DEFENCE
		//}}
		//{{ 2012. 05. 29	��μ�       ���� ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
		CASE( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
		//}}
		//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
		CASE( DBE_WEDDING_COMPLETE_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

	default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KCnGameDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KCnGameDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

bool KCnGameDBThread::Query_InsertItemList( IN const SEnum::GET_ITEM_REASON eGetItemReason, 
										    IN const UidType iUnitUID, 
											IN const std::vector< KItemInfo >& vecItemInfo, 
											OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo, 
											IN const bool bUnSeal /*= true*/ )
{
	if( SEnum::IsValidGetItemReason( eGetItemReason ) == false )
	{
		START_LOG( cerr, L"��ȿ�������� ������ ���� �����Դϴ�!" )
			<< BUILD_LOG( eGetItemReason )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( vecItemInfo.size() )
			<< END_LOG;
	}

	BOOST_TEST_FOREACH( const KItemInfo&, kNewItemInfo, vecItemInfo )
	{
		KItemInfo kNewItemInfoResult = kNewItemInfo; // ���纻 ����

		//////////////////////////////////////////////////////////////////////////
		// ������ ����
		UidType iItemUID = 0;
		int iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.gup_insert_item", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% iUnitUID
			% kNewItemInfo.m_iItemID
			% static_cast<int>(kNewItemInfo.m_cUsageType)
			% kNewItemInfo.m_iQuantity
			% kNewItemInfo.m_sEndurance
			% kNewItemInfo.m_sPeriod
			% static_cast<int>(kNewItemInfo.m_cEnchantLevel)
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
			% static_cast<int>(eGetItemReason)
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK
				>> iItemUID
				>> kNewItemInfoResult.m_wstrExpirationDate );

			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"������ ���� ����." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( kNewItemInfo.m_iItemID )
				<< BUILD_LOGc( kNewItemInfo.m_cUsageType )
				<< BUILD_LOG( kNewItemInfo.m_iQuantity )
				<< BUILD_LOG( kNewItemInfo.m_sEndurance )
				<< BUILD_LOG( kNewItemInfo.m_sPeriod )
				<< BUILD_LOGc( kNewItemInfo.m_cEnchantLevel )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
				<< END_LOG;
			goto end_proc;
		}

		//////////////////////////////////////////////////////////////////////////		
		//{{ 2009. 8. 27  ������	�к�
		if( kNewItemInfo.m_ucSealData > 0 )
		{
			//{{ 2009. 9. 2  ������		�к�
			if( kNewItemInfoResult.IsSealedItem()  &&  bUnSeal )
			{
				kNewItemInfoResult.UnsealItem(); // �к� ����
			}
			//}}

			DO_QUERY( L"exec dbo.gup_update_item_seal", L"%d, %d", % iItemUID % static_cast<int>(kNewItemInfoResult.m_ucSealData) );

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"�к� ���� ������Ʈ ����." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOGc( kNewItemInfoResult.m_ucSealData )
					<< END_LOG;
				goto end_proc;
			}
		}
		//}}

		//////////////////////////////////////////////////////////////////////////		
		// ���� ������ �����Ѵٸ� ���� DB�� insert����!
#ifdef SERV_BATTLE_FIELD_BOSS// �۾���¥: 2013-12-11	// �ڼ���
		{
			bool bNeedSocketInfoDBUpdate = false;

			if( 0 < kNewItemInfo.m_byteExpandedSocketNum )
			{
				bNeedSocketInfoDBUpdate = true;
			}
			else if( kNewItemInfo.m_vecItemSocket.empty() == false )
			{
				for( std::vector<int>::const_iterator it = kNewItemInfo.m_vecItemSocket.begin(); it != kNewItemInfo.m_vecItemSocket.end(); ++it )
				{
					if( 0 < *it )
					{
						bNeedSocketInfoDBUpdate = true;
						break;
					}
				}
			}

			if( bNeedSocketInfoDBUpdate == true )
			{
				const byte byteArraySize = 5;
				int arrSocketInfo[byteArraySize]; // DB ���̺��� �������̺� ����
				memset( arrSocketInfo, 0, sizeof(int) * byteArraySize );

				std::vector<int>::const_iterator it = kNewItemInfo.m_vecItemSocket.begin();
				for( int i = 0; i < byteArraySize; ++i )
				{
					if( it == kNewItemInfo.m_vecItemSocket.end() )
						continue;

					arrSocketInfo[i] = *it;
					++it;
				}

				DO_QUERY( L"exec dbo.P_GItemSocket_SET", L"%d, %d, %d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					% arrSocketInfo[4]			// @iSoket5 smallint
					% kNewItemInfo.m_byteExpandedSocketNum
					);

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"������ ���� ���� ������Ʈ ����." )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( iItemUID )
						<< BUILD_LOG( arrSocketInfo[0] )
						<< BUILD_LOG( arrSocketInfo[1] )
						<< BUILD_LOG( arrSocketInfo[2] )
						<< BUILD_LOG( arrSocketInfo[3] )
						<< BUILD_LOG( arrSocketInfo[4] )
						<< END_LOG;
				}
			}
		}
#else // SERV_BATTLE_FIELD_BOSS
		if( kNewItemInfo.m_vecItemSocket.empty() == false )
		{
			bool bSocketExist = false;
			int arrSocketInfo[4] = {0,0,0,0}; // DB ���̺��� �������̺� ����
			std::vector< int >::const_iterator vitSocket = kNewItemInfo.m_vecItemSocket.begin();
			for( int iIdx = 0; iIdx < 4; ++iIdx )
			{
				if( vitSocket == kNewItemInfo.m_vecItemSocket.end() )
					continue;

				if( *vitSocket > 0 )
				{
					bSocketExist = true;
				}
				arrSocketInfo[iIdx] = *vitSocket;
				++vitSocket;
			}

			if( bSocketExist )
			{
				DO_QUERY( L"exec dbo.gup_update_Socket", L"%d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					);

					if( m_kODBC.BeginFetch() )
					{
						FETCH_DATA( iOK );
						m_kODBC.EndFetch();
					}

					if( iOK != NetError::NET_OK )
					{
						START_LOG( cerr, L"������ ���� ���� ������Ʈ ����." )
							<< BUILD_LOG( iOK )
							<< BUILD_LOG( iUnitUID )
							<< BUILD_LOG( iItemUID )
							<< BUILD_LOG( arrSocketInfo[0] )
							<< BUILD_LOG( arrSocketInfo[1] )
							<< BUILD_LOG( arrSocketInfo[2] )
							<< BUILD_LOG( arrSocketInfo[3] )
							<< END_LOG;
					}
			}
		}
#endif // SERV_BATTLE_FIELD_BOSS

		// �����Ǵ� ������ ������ ��� DB�� insert�Ͽ����Ƿ� �����̳ʿ��� ����!
		mapInsertedItemInfo.insert( std::make_pair( iItemUID, kNewItemInfoResult ) );
	}

	return true;

end_proc:
	return false;
}

bool KCnGameDBThread::Query_UpdateItemQuantity( IN const UidType iUnitUID, 
											    IN const std::map< UidType, int >& mapUpdated, 
												OUT std::map< UidType, int >& mapFailed, 
												OUT bool& bUpdateFailed )
{
	//{{ 2009. 11. 17  ������	DB��������	
	bool bQuerySuccess = true;
	//}}
	bUpdateFailed = false;
	mapFailed.clear();

	std::map< UidType, int >::const_iterator mit = mapUpdated.begin();
	while( mit != mapUpdated.end() )
	{
		int iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.gup_update_item", L"%d, %d, %d, %d", 
			% iUnitUID 
			% mit->first 
			% CXSLItem::PT_QUANTITY 
			% mit->second
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"������ ���� ������Ʈ ����." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
					<< END_LOG;

				//{{ 2008. 10. 23  ������	���а� �����ѹ� ��ƺ���~!
				if( iOK == -5 )
				{
					START_LOG( cout, L"���а� ������Ʈ ���� �߻�!" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( mit->first )
						<< BUILD_LOG( mit->second );

					bUpdateFailed = true;
				}
				//}}
			}
		}
		else
		{
			START_LOG( cerr, L"������ ���� ������Ʈ�� BeginFetch ����." );
		}

end_proc:
		if( iOK != NetError::NET_OK  &&  iOK != -5 )
		{
			//{{ 2009. 11. 17  ������	DB��������
			bQuerySuccess = false;
			//}}

			mapFailed.insert( std::make_pair( mit->first, mit->second ) );
		}
		mit++;
	}

	return bQuerySuccess;
}

void KCnGameDBThread::Query_DeleteItem( IN const std::vector< KDeletedItemInfo >& vecDeleted, OUT std::vector< KDeletedItemInfo >& vecFailed )
{
	vecFailed.clear();

	std::vector< KDeletedItemInfo >::const_iterator vit = vecDeleted.begin();
	while( vit != vecDeleted.end() )
	{
		int iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.gup_delete_item", L"%d, %d", % vit->m_iItemUID % (int)vit->m_ucDeleteReason );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"������ ���� ����." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( vit->m_iItemUID )
					<< BUILD_LOGc( vit->m_ucDeleteReason )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"������ ���� �� BeginFetch ����." );
		}
end_proc:
		if( iOK != NetError::NET_OK )
		{
			vecFailed.push_back( *vit );
		}
		++vit;
	}
}

//{{ 2012. 05. 08	��μ�       ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
IMPL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_REQ )
{
	KDBE_DB_UPDATE_TRADE_RESULT_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_ODBC_01;

	KERM_DB_UPDATE_TRADE_RESULT_NOT UnitA;
	KERM_DB_UPDATE_TRADE_RESULT_NOT UnitB;

	// SP ������ ����� ����
	int iTemp_Type = 6;			// ���� �ŷ� sp Ÿ��
	//{{ 2013. 05. 28	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Trade_New ";
#else
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Trade ";
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}	
	wstrQuerySP += boost::str( boost::wformat( L"%d" ) % iTemp_Type );
	
	std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >::iterator vitDBUpdate;

	bool bFirstUnit = true;
	for( vitDBUpdate = kPacket_.m_vecDBUpdate.begin() ; vitDBUpdate != kPacket_.m_vecDBUpdate.end() ; ++vitDBUpdate )
	{
		if( bFirstUnit == true )
		{
			UnitA = (*vitDBUpdate);
			bFirstUnit = false;
		}
		else
		{
			UnitB = (*vitDBUpdate);
			break;
		}
	}

	// UnitUID �� ���� ���� ����
	
	for( vitDBUpdate = kPacket_.m_vecDBUpdate.begin() ; vitDBUpdate != kPacket_.m_vecDBUpdate.end() ; ++vitDBUpdate )
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % vitDBUpdate->m_iUnitUID );
	}

	// ed �� ���� ���� ����
	for( vitDBUpdate = kPacket_.m_vecDBUpdate.begin() ; vitDBUpdate != kPacket_.m_vecDBUpdate.end() ; ++vitDBUpdate )
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % vitDBUpdate->m_iChangedED );
	}

	// ���ֺ� ������ ����
	vitDBUpdate = kPacket_.m_vecDBUpdate.begin();
	int iTemp_Zero = 0;
	bool bCheck = false;
	int index = 0;
	for(  ; vitDBUpdate != kPacket_.m_vecDBUpdate.end() ; ++vitDBUpdate )
	{
		std::vector<KInventoryItemInfo>::iterator vvit = vitDBUpdate->m_vecUpdated.begin();
		for( ; vvit != vitDBUpdate->m_vecUpdated.end() ; ++vvit, ++index )
		{
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % vvit->m_iItemUID );
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % vvit->m_kItemInfo.m_iQuantity );
		}

		for( ; index < 10 ; ++index )
		{
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );	
		}
	}

	if( 10 <= index && index < 20 )
	{
		for( ; index < 20 ; ++index )
		{
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );	
		}
	}
	
	DO_QUERY_NO_ARG( wstrQuerySP.c_str() );

	// �������� 10�� + 10�� �̹Ƿ� 20���� �����Ѵ�.
	UidType iItemUID[20] = {0,};

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacketAck.m_iOK
			>> iItemUID[0]			// A unit -> B
			>> iItemUID[1]
			>> iItemUID[2]
			>> iItemUID[3]
			>> iItemUID[4]
			>> iItemUID[5]
			>> iItemUID[6]
			>> iItemUID[7]
			>> iItemUID[8]
			>> iItemUID[9]
			>> iItemUID[10]			// B unit -> A
			>> iItemUID[11]
			>> iItemUID[12]
			>> iItemUID[13]
			>> iItemUID[14]
			>> iItemUID[15]
			>> iItemUID[16]
			>> iItemUID[17]
			>> iItemUID[18]
			>> iItemUID[19]	);

		m_kODBC.EndFetch();

		if( kPacketAck.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���� �ŷ� SP ȣ���� ���� �߽��ϴ�." )
				<< BUILD_LOG( kPacketAck.m_iOK )
				<< END_LOG;

			goto end_proc;
		}
		
		UnitB.m_vecInsertDBItem.clear();
		std::vector<KInventoryItemInfo>::iterator vitA = UnitA.m_vecUpdated.begin();
		for( int indexA = 0 ; (indexA < 10 && vitA != UnitA.m_vecUpdated.end()) ; ++indexA )	
		{
			if( vitA != UnitA.m_vecUpdated.end() )
			{
				vitA->m_iItemUID = iItemUID[indexA];
				vitA->m_sSlotID = -1;					// ������ �κ��丮�� �ִ� ���� �̹Ƿ� �ʱ�ȭ ����

				// �ŷ��� ���� ���´� SP ���� �����Ѵ�.
				if( vitA->m_kItemInfo.IsSealedItem() == true )
				{
					vitA->m_kItemInfo.UnsealItem();
				}

				UnitB.m_vecInsertDBItem.push_back( *vitA );
				++vitA;				
			}
		}

		UnitA.m_vecInsertDBItem.clear();
		std::vector<KInventoryItemInfo>::iterator vitB = UnitB.m_vecUpdated.begin();
		for( int indexB = 10 ; (indexB < 20 && vitB != UnitB.m_vecUpdated.end()) ; ++indexB )	
		{
			if( vitB != UnitB.m_vecUpdated.end() )
			{
				vitB->m_iItemUID = iItemUID[indexB];
				vitB->m_sSlotID = -1;				// ������ �κ��丮�� �ִ� ���� �̹Ƿ� �ʱ�ȭ ����

				// �ŷ��� ���� ���´� SP ���� �����Ѵ�.
				if( vitB->m_kItemInfo.IsSealedItem() == true )
				{
					vitB->m_kItemInfo.UnsealItem();
				}

				UnitA.m_vecInsertDBItem.push_back( *vitB );
				++vitB;
			}
		}
	}

	kPacketAck.m_vecDBUpdate.push_back( UnitA );
	kPacketAck.m_vecDBUpdate.push_back( UnitB );

end_proc:

	SendToRoom( FIRST_SENDER_UID, DBE_DB_UPDATE_TRADE_RESULT_ACK, kPacketAck );
}
#else

//{{ 2012. 03. 28	������	���ΰŷ� DDOS ���
#ifdef SERV_TRADE_DDOS_DEFENCE
IMPL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_REQ )
{
	KDBE_DB_UPDATE_TRADE_RESULT_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;

	BOOST_TEST_FOREACH( const KERM_DB_UPDATE_TRADE_RESULT_NOT&, kPacketReq, kPacket_.m_vecDBUpdate )
	{
		KERM_DB_UPDATE_TRADE_COMPLETE_NOT kPacketResult;
		kPacketResult.m_iUnitUID = kPacketReq.m_iUnitUID;
		kPacketResult.m_iChangedED = kPacketReq.m_iChangedED;
		kPacketResult.m_mapInsertedItem = kPacketReq.m_mapInsertedItem;
		kPacketResult.m_vecUpdatedInventorySlot = kPacketReq.m_vecUpdatedInventorySlot;

		int iOK = NetError::ERR_ODBC_00;

		// ED���� ������Ʈ!
		DO_QUERY( L"exec dbo.gup_update_unit_info", L"%d, %d, %d, %d, %d, %d, %d, %d",
			% kPacketReq.m_iUnitUID
			% 0
			% 0
			% kPacketReq.m_iChangedED
			% 0
			% 0
			% 0
			% false
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"���ΰŷ� ��� ó�� ED���� ������Ʈ ����!" )
				<< BUILD_LOG( kPacketReq.m_iUnitUID )
				<< BUILD_LOG( kPacketReq.m_iChangedED )
				<< END_LOG;
		}

		// ������ ���� ������Ʈ!
		bool bUpdateFailed = false;
		LIF( Query_UpdateItemQuantity( kPacketReq.m_iUnitUID, kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange, kPacketResult.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
		Query_DeleteItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted, kPacketResult.m_kItemQuantityUpdate.m_vecDeleted );

		if( Query_InsertItemList( SEnum::GIR_TRADE, kPacketReq.m_iUnitUID, kPacketReq.m_vecItemInfo, kPacketResult.m_mapItemInfo ) == false )
		{
			START_LOG( cerr, L"������ ������Ʈ ����!" )
				<< BUILD_LOG( kPacketReq.m_iUnitUID )
				<< END_LOG;
		}

		kPacketAck.m_vecDBUpdate.push_back( kPacketResult );
	}

	SendToRoom( FIRST_SENDER_UID, DBE_DB_UPDATE_TRADE_RESULT_ACK, kPacketAck );
}
#endif SERV_TRADE_DDOS_DEFENCE
//}}

#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2012. 05. 29	��μ�       ���� ���� �ŷ� ���� ����
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
IMPL_ON_FUNC( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ )
{
	KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;

	// SP ������ ����� ����
	int iTemp_Type = 5;		// ���� ���� sp Ÿ��
	//{{ 2013. 05. 28	������	������ ����
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Trade_New ";
#else
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Trade ";
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	wstrQuerySP += boost::str( boost::wformat( L"%d" ) % iTemp_Type );

	// �Ǹ���
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iSellUnitUID );
	
	// ������
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iBuyUnitUID );

	// �Ǹ��� ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iSellUnitEDIN );		// �����ᰡ �ٸ��� ������ ���� ó�� �ؾ��Ѵ�.

	// ������ ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iBuyUnitEDOUT );		// �����ᰡ �ٸ��� ������ ���� ó�� �ؾ��Ѵ�.

	// �Ǹ��� ������
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_kSellItemInfo.m_iItemUID );
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_kSellItemInfo.m_kItemInfo.m_iQuantity );

	// ������ ������ - �����ڴ� �������� �����⸸ �ϱ� ������ ����.
	int index = 1;			// ���� �ϳ��� ��� �����Ƿ� 1�� �ʱ�ȭ�Ѵ�.
	int iTemp_Zero = 0;
	for( ; index < 20 ; ++index )
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
	}

	DO_QUERY_NO_ARG( wstrQuerySP.c_str() );

	// �������� 10�� + 10�� �̹Ƿ� 20���� �����Ѵ�.
	UidType iItemUID[20] = {0,};

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacketAck.m_iOK
			>> iItemUID[0]			// �ǸŵǴ� ������
			>> iItemUID[1]
			>> iItemUID[2]
			>> iItemUID[3]
			>> iItemUID[4]
			>> iItemUID[5]
			>> iItemUID[6]
			>> iItemUID[7]
			>> iItemUID[8]
			>> iItemUID[9]
			>> iItemUID[10]			
			>> iItemUID[11]
			>> iItemUID[12]
			>> iItemUID[13]
			>> iItemUID[14]
			>> iItemUID[15]
			>> iItemUID[16]
			>> iItemUID[17]
			>> iItemUID[18]
			>> iItemUID[19]	);

			m_kODBC.EndFetch();

			if( kPacketAck.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"���� �ŷ� SP ȣ���� ���� �߽��ϴ�." )
					<< BUILD_LOG( kPacketAck.m_iOK )
					<< END_LOG;

				goto end_proc;
			}

			// ���� ������ �ϳ��� ��ǰ�� ���� �����ϹǷ� ���������� �Ѵ�.
			kPacket_.m_kBuyItemInfo.m_iItemUID = iItemUID[0];
			// �ŷ��� ���� ���´� SP ���� �����Ѵ�.
			if( kPacket_.m_kBuyItemInfo.m_kItemInfo.IsSealedItem() == true )
			{
				kPacket_.m_kBuyItemInfo.m_kItemInfo.UnsealItem();
			}

			kPacketAck.m_kResult = kPacket_;
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK, kPacketAck );
}
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
//}}

//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( DBE_WEDDING_COMPLETE_REQ )
{
	KDBE_WEDDING_COMPLETE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_wstrWeddingDate = kPacket_.m_wstrWeddingDate;

	std::wstring wstrLoveWord;

	// ��ȥ ���� - ���� ������Ʈ
	DO_QUERY( L"exec dbo.P_GCouple_Info_INT_UPD", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
		% 2
		% kPacket_.m_iGroom
		% kPacket_.m_iBride
		% kPacket_.m_wstrWeddingDate
		% wstrLoveWord
		% 0 );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		DO_QUERY( L"exec dbo.P_GCouple_WeddingInfo_UPD", L"%d", % kPacket_.m_iWeddingUID );
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );

			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"��ȥ�� ���� ��� ����!" )
				<< BUILD_LOG( kPacket_.m_iWeddingUID )
				<< BUILD_LOG( kPacket_.m_iGroom )
				<< BUILD_LOG( kPacket_.m_iBride )
				<< BUILD_LOG( kPacket_.m_wstrWeddingDate )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"��ȥ�ϱ� ���� ��� ����!" )
			<< BUILD_LOG( kPacket_.m_iWeddingUID )
			<< BUILD_LOG( kPacket_.m_iGroom )
			<< BUILD_LOG( kPacket_.m_iBride )
			<< BUILD_LOG( kPacket_.m_wstrWeddingDate )
			<< END_LOG;
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_WEDDING_COMPLETE_ACK, kPacket );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}