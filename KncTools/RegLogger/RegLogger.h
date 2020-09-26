#pragma once

#include <string>
#include <windows.h>    // DWORD
#include "CmdLineTool.h"

class KRegLogger : public KCmdLineTool
{
public:
    KRegLogger(void);
    virtual ~KRegLogger(void);

    virtual bool Init();
    virtual bool Run();

protected:
    inline void AddEventSource();
    inline void RemoveEventSource();

public:
    std::string     m_strPath;              // ����� �������� ���.
    std::string     m_strName;              // ������Ʈ���� ��ϵ� ��� ��.
    DWORD           m_dwCategoryCount;      // �̺�Ʈ ī�װ� ����.
    bool            m_bIsRegisterAction;    // '/unreg' �϶� false.
};

DECL_ON_FUNC( reg );
DECL_ON_FUNC( unreg );
DECL_ON_FUNC( name );
DECL_ON_FUNC( cc );
DECL_ON_FUNC( path );