#pragma once

#include "KncUidType.h"
#include <string>
#include <map>

enum eModeResult {  // KCharLogInfo::AddModeLog(...) �Լ� ȣ���� ���ؼ��� ���.
    MR_WIN = 0,
    MR_DRAW,
    MR_LOSE,
};

struct KModeLogInfo
{
    KModeLogInfo();     // ������� �⺻�� ������ ���� ������. 

    int     m_nModeID;
    int     m_nModeCount;
    int     m_nPlayTime;
    int     m_nWin;         // ����-��, ����-Ŭ����
    int     m_nLose;        // ����-��, ����-����
    int     m_nDraw;        // ����-���, ����-����
};

// ĳ���� 1���� ���� ������
struct KCharLogInfo 
{
    UidType         m_nCharUID;

    unsigned char   m_ucCharType;
    unsigned char   m_ucLevel;
    std::wstring    m_strNickName;
    int             m_nEDGain;      // ȹ���� ���ӸӴ�
    int             m_nEDSell;      // ������ �Ǹŷ� ���� ���ӸӴ�
    int             m_nEDSpend;     // ����� ���ӸӴ� ��
    int             m_nVP;
    int             m_nExp;
    int             m_nVP2Exp;
    
    std::map<int,KModeLogInfo>  m_mapModeLog;

    KCharLogInfo();
    KCharLogInfo& operator=( const KCharLogInfo& right )
    {
        m_nCharUID      = right.m_nCharUID;
        m_ucCharType    = right.m_ucCharType;
        m_ucLevel       = right.m_ucLevel;
        m_strNickName   = right.m_strNickName;
        m_nEDGain       = right.m_nEDGain;
        m_nEDSell       = right.m_nEDSell;
        m_nEDSpend      = right.m_nEDSpend;
        m_nVP           = right.m_nVP;
        m_nExp          = right.m_nExp;
        m_nVP2Exp       = right.m_nVP2Exp;
        m_mapModeLog    = right.m_mapModeLog;

        return *this;
    }

    void AddModeLog( int nModeID, int nPlayTime, eModeResult eResult );
};