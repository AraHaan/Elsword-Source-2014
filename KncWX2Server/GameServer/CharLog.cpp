#include "CharLog.h"
#include "x2data/XSLUnit.h"

KModeLogInfo::KModeLogInfo()
{
    m_nModeID       = 0;
    m_nModeCount    = 0;
    m_nPlayTime     = 0;
    m_nWin          = 0;
    m_nLose         = 0;
    m_nDraw         = 0;
}

KCharLogInfo::KCharLogInfo()
{
    m_nCharUID      = 0;
    m_ucCharType    = CXSLUnit::UT_ELSWORD;
    m_ucLevel       = 1;
    m_nEDGain       = 0;
    m_nEDSell       = 0;
    m_nEDSpend      = 0;
    m_nVP           = 0;
    m_nExp          = 0;
    m_nVP2Exp       = 0;
}

void KCharLogInfo::AddModeLog( int nModeID, int nPlayTime, eModeResult eResult )
{
    // ������ �ش� ��忡 ���� �����Ͱ� �־������� �˻��� �ʿ� ����.
    // operator[]�� �ǵ����� ������� ���� ������ �߰��� �ڵ����� �Ͼ�� �Ѵ�.
    m_mapModeLog[ nModeID ].m_nModeID = nModeID;
    m_mapModeLog[ nModeID ].m_nModeCount++;
    m_mapModeLog[ nModeID ].m_nPlayTime += nPlayTime;
    switch( eResult )
    {
    case MR_WIN:
        break;
    case MR_DRAW:
        break;
    case MR_LOSE:
        break;
    }

    std::map<int,KModeLogInfo>::iterator mit;
    mit = m_mapModeLog.find( nModeID );

    if( mit == m_mapModeLog.end() ) // ������ �ش� ��忡 ���� ������ ���� - �߰��۾� ����
    {
        m_mapModeLog[ nModeID ].m_nModeCount++;
    }
}