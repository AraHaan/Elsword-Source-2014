#include <dbg/dbg.hpp>

#include "GSLogDBThread2nd.h"
#include "GameServer.h"

#include "NetError.h"


#define CLASS_TYPE KGSLogDBThread2nd

ImplPfID( KGSLogDBThread2nd, PI_GS_LOG_DB_2ND );

IMPL_PROFILER_DUMP( KGSLogDBThread2nd )
{

}

KGSLogDBThread2nd::~KGSLogDBThread2nd()
{

}

void KGSLogDBThread2nd::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
        CASE( DBE_CHANNEL_LIST_REQ );

    default:
        START_LOG( cerr, L"�̺�Ʈ �ڵ鷯�� ���ǵ��� �ʾ���. " << spEvent_->GetIDStr() );

    }
}

void KGSLogDBThread2nd::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}


void KGSLogDBThread2nd::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

IMPL_ON_FUNC( DBE_CHANNEL_LIST_REQ )
{
    KDBE_CHANNEL_LIST_ACK kPacket;

    // DB�κ��� ���� ����Ʈ �����͸� �޾ƿ´�
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelList_SEL_ByServerSetID", L"%d", % kPacket_.m_iServerGroupID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelList_SEL", L"%d", % kPacket_.m_iServerGroupID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_channel_list", L"%d", % kPacket_.m_iServerGroupID );
#endif //SERV_RENEWAL_SP

    while( m_kODBC.Fetch() )
    {
        KChannelInfo kInfo;

        FETCH_DATA( kInfo.m_iServerUID
            >> kInfo.m_iChannelID
            >> kInfo.m_wstrChannelName
            >> kInfo.m_iServerGroupID
            >> kInfo.m_wstrIP
            >> kInfo.m_usMasterPort
            >> kInfo.m_usNCUDPPort
            >> kInfo.m_iMaxUser
            >> kInfo.m_iCurrentUser
            >> kInfo.m_iCurPartyCount
            >> kInfo.m_iPlayGamePartyCount
            );

        kPacket.m_mapChannelList.insert( std::make_pair( kInfo.m_iChannelID, kInfo ) );
    }

    //{{ 2012. 06. 29	�ڼ���	ä�� UI ���������� ���� ������ �ý��� ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
    {
        std::map<int, KChannelInfo>::const_iterator mit;
        for( mit = kPacket.m_mapChannelList.begin(); mit != kPacket.m_mapChannelList.end(); ++mit )
        {
            KChannelBonusInfo tempChannelBonusInfo;

#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_LChannelReward_SEL", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID() 
				% mit->first );
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.lup_get_channel_reward", L"%d", % mit->first );
#endif //SERV_RENEWAL_SP

            if( m_kODBC.BeginFetch() )
            {
                int iChannelID = 0;
                FETCH_DATA( iChannelID
                    >> tempChannelBonusInfo.m_iBeginLv
                    >> tempChannelBonusInfo.m_iEndLv
                    >> tempChannelBonusInfo.m_iPerExp
                    >> tempChannelBonusInfo.m_iPerED
                    );
                m_kODBC.EndFetch();

                if( iChannelID != mit->first )
                {
                    START_LOG( cerr, L"ä�� ���ʽ� ������ �̻��մϴ�" )
                        << BUILD_LOG( iChannelID )
                        << BUILD_LOG( mit->first )
                        << END_LOG;
                }

                //if( tempChannelBonusInfo.m_iPerExp > 0  ||  tempChannelBonusInfo.m_iPerED > 0 )
                {
                    // ���ʽ����� ������ enable
                    tempChannelBonusInfo.m_bEnable = true;
                    kPacket.m_mapChannelBonusList.insert(std::make_pair(mit->first, tempChannelBonusInfo));
                }
            }
        }
    }
#endif SERV_CHANNEL_LIST_RENEWAL
    //}}

end_proc:
    SendToServer( DBE_CHANNEL_LIST_2ND_ACK, kPacket );
}