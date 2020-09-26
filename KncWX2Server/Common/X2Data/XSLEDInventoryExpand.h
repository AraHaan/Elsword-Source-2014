#pragma once
#include "ServerDefine.h"

#include "RefreshSingleton.h"

class CXSLEDInventoryExpand
{
    DeclareRefreshSingleton( CXSLEDInventoryExpand );
    DeclareLuaScriptParser;
    DeclDumpToLogFile;
    DeclDump;
    DeclToStringW;

public:
    CXSLEDInventoryExpand(void);
    ~CXSLEDInventoryExpand(void);

    bool AddBankGradeED_LUA();
    bool AddInventoryGradeED_LUA();
    int GetNextBankUpgradeED( IN const int& iGrade_ );
    int GetNextInventoryUpgradeED( IN const int& iGrade_ );

private:
    std::map< int, int > m_mapBankGradeED; // first : ���� ��� ,  second : �Ҹ�Ǵ� ED
    std::map< int, int > m_mapInventoryGradeED; // first : �κ��丮 ���, second : �Ҹ�Ǵ� ED
};

DefRefreshSingletonInline( CXSLEDInventoryExpand );