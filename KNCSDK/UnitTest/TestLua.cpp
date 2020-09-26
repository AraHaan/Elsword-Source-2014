#include <boost/test/unit_test.hpp>

#include <KncLua.h>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <iostream>

void GlobalFunction( int iInput )  { std::cout << "GlobalFunction. input : " << iInput << std::endl; }

class KLuaBindTest
{
public:
    KLuaBindTest()          { std::cout << "KLuaBindTest is constructed." << std::endl; }
    ~KLuaBindTest()         { std::cout << "KLuaBindTest is destructed." << std::endl; }

    void MemberFunction()   { std::cout << "KLuaBindTest::MemberFunction is called." << std::endl; }
    static void StaticFunction() { std::cout << "KLuaBindTest::StaticFunction is called." << std::endl; }

public:
    int             m_iMember;
    std::string     m_strMember;
};

std::ostream& operator<<( std::ostream& stm_, const KLuaBindTest& kObj_ )
{
    return stm_ << "KLuaBindTest dump " << std::endl
                << "    m_iMember   : " << kObj_.m_iMember << std::endl
                << "    m_strMember : " << kObj_.m_strMember << std::endl;
}

void TestLua()
{
    // original LUA script ��� �׽�Ʈ.
    lua_State* lua = lua_open();
    
    boost::shared_ptr<lua_State> splua;
    splua = boost::shared_ptr<lua_State>(lua, lua_close);

    BOOST_CHECK_MESSAGE( splua != NULL, "lua ��ü�� ������ ���߽��ϴ�." );

    // luabind ��� ���� �׽�Ʈ.
    luabind::open( splua.get() );
    luabind::module( splua.get() )
    [
        luabind::def( "GlobalFunction", &GlobalFunction ),
        luabind::def( "StaticFunction", &KLuaBindTest::StaticFunction ),
        luabind::class_<KLuaBindTest>( "KLuaBindTest" )
            .def( luabind::constructor<>() )
            .def( luabind::tostring(luabind::self) )
            .def_readwrite( "m_iMember", &KLuaBindTest::m_iMember )
            .def_readwrite( "m_strMember", &KLuaBindTest::m_strMember )
            .def( "MemberFunction", &KLuaBindTest::MemberFunction )
    ];

    luaopen_base( splua.get() );    // open base library : for using 'print' function.

    char* pszScript = "GlobalFunction(1234) \n"
        "object = KLuaBindTest() \n"
        "object.m_iMember = 8012 \n"
        "object.m_strMember = '�����ٶ󸶹ٻ�' \n"
        "print(object) \n";

    BOOST_CHECK_EQUAL( lua_dostring( splua.get(), pszScript ), 0 );

    // console Ÿ���� ��ȭ�� �������̽��� interpreter ��� �׽�Ʈ.
    lua_interpreter( splua.get() );

    //lua_close( lua ); // interpreter ���ο��� lua_close�� ȣ���Ѵ�.
}