#pragma once

//{{ 060128. kkurrung.
/*
{
.INI������ �о�鿩�� ���ϴ� ���ڿ��� �����°��� ����
�ణ�� Ȯ�强�� ���ؼ� �Ʒ��� ������ ������ ����ϰڴ�.

map<"section",map >
              ---
               |
               ---> map< key, data >
                              ----
                                |
                                ---> struct
                                     {
                                        std::wstring key;
                                        std::wstring value;
                                     }
strings.ini
------------------
[section1]
key1 = value1
key2 = value2
key3 = value3
    :
    :
[section2]
key1 = value1
key2 = value2
key3 = value3
    :
    :
-----------------

�̷��� ������ ������ ������ �ְ�
���� �����̴�.

protocol Version �� ���� ������� ���� ����..
(����� �������� ������ �б� ���� ����������. �̰��� �б⸸ ���� �ϰ� �� ������.
 ��, ������ ������ �о� ���� �����̱� ������ �������� Int���� ������ ���� �ְ� ������ �����ؾ� �ϱ� ������
 �� Ŀ����, �ܼ������� ������ ���Ƽ���.. + �����Ƽ�.. )

����Ҷ��� ���Ѵ� ���ǰ� Ű�� �Է����� �ް� �ɰ��̰�
�׿� �ش��ϴ� ���� ��ȯ�� �����̴�.

�׸��� Current Section�� �����ؼ�
������ ���ǿ� �׻� ���� �� �ִ� ��븦 �����Ҽ� �ְԵ� �Ұ��̴�.

���ǳ����� Ű�� �ߺ��� �̸� ���ؾ� �Ѵ�.(������ �߻��Ұ��̴�.)
������ �̸��� �ߺ��ص� �����ϰ� �Ұ��̴�.(�ǵ��� ���ϴ� ���� �������̴�)
�׷��� �������̸��� ���ǳ����� ������ Ű�� ���ĵ���̴�.

������ ���ǰ� Ű�� �������� �ʴ´ٸ� �������� ini�� ������ �ְ� �ұ� ����?(����)
(����.. ���� �б� ������ 32k characters�� �ѹ� �����غ���)

(AddIni()) -- ����....
LoadIni();
std::wstring GetValue( sectionName, Key );
bool SetCurrentSection( sectionName);
std::wstring GetValue(Key);
void Dump()

�������� �Լ��� ���� �� �����̴�.

�Ƹ��� SimLayer�� Has a�� .....
}
*/
//}} kkurrung.

#include <map>
#include <string>

#include "Event.h"
#include "CommonPacket.h" // CON_COPYCON_ASSIGNOP
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include <KncUtil.h> // IN, OUT

class KStringTable
{
protected:
    struct Key_value
    {
        CON_COPYCON_ASSIGNOP( Key_value, right )
        {
            m_strKey    = right.m_strKey;
            m_strValue  = right.m_strValue;
            return *this;
        };

        std::wstring    m_strKey;
        std::wstring    m_strValue;
    };

    struct Section
    {
        CON_COPYCON_ASSIGNOP( Section, right )
        {
            m_strSection    = right.m_strSection;
            m_mapKeyValue   = right.m_mapKeyValue;
            return *this;
        };

        std::wstring                        m_strSection;
        std::map< std::wstring, Key_value > m_mapKeyValue; //<key, keyValue>
    };

    typedef std::map<std::wstring, Section>     MAPSECTION;
    typedef MAPSECTION::iterator                MITSECTION;

    typedef std::map<std::wstring, Key_value>   MAPKEYVALUE;
    typedef MAPKEYVALUE::iterator               MITKEYVALUE;

    
public:
    KStringTable(void);
    ~KStringTable(void);
public:
    bool LoadIni( IN const wchar_t* szFileName );
    std::wstring GetValue( IN const wchar_t* szSectionName, IN const wchar_t* Key );
    std::wstring GetValue( IN const wchar_t* Key);
    bool SetCurrentSection( IN const wchar_t* szSectionName);
    std::wstring GetCurrentSection(){ return m_strCurrentSection;};
    void Dump( std::wostream& stm )const;
protected:
    bool AddAndBuildSection( IN const wchar_t* szSectionName, IN const wchar_t* szFileName, IN DWORD dwFileSize );
    bool AddAndBuildKeyvalue( IN const wchar_t* szKey_value, Section& section );
    bool FindSection( IN const wchar_t* szSectionName, OUT MITSECTION& itSection );
    bool FindKey_value( IN MITSECTION& itSection, IN const wchar_t* szKeyName,
                        OUT MITKEYVALUE& keyValue );

protected:
    MAPSECTION                          m_mapSectionTable;
    std::wstring                        m_strCurrentSection;

};
