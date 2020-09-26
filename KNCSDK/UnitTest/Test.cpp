#include <boost/test/included/unit_test_framework.hpp>

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

void TestLua();
void TestThread();
void TestSerializer();
void TestCrypto();      // Security ��⿡�� ���� ��ȣȭ �κи��� �׽�Ʈ�Ѵ�.
void TestSecurity();    // IPSec�� ��ɵ��� ������ ���� ��ü ��� �׽�Ʈ.
void TestDebug();
void TestTimeStamp();
boost::unit_test_framework::test_suite* init_unit_test_suite(int, char*[])
{
    boost::unit_test_framework::test_suite* pTestSuite = BOOST_TEST_SUITE( "KNCSDK Test" );
	
    pTestSuite->add( BOOST_TEST_CASE( &TestLua ) );
#ifdef _MT
    pTestSuite->add( BOOST_TEST_CASE( &TestThread ) );
#endif
    pTestSuite->add( BOOST_TEST_CASE( &TestSerializer ) );
    pTestSuite->add( BOOST_TEST_CASE( &TestCrypto ) );
    pTestSuite->add( BOOST_TEST_CASE( &TestSecurity ) );
    pTestSuite->add( BOOST_TEST_CASE( &TestDebug ) );
    pTestSuite->add( BOOST_TEST_CASE( &TestTimeStamp ) );
    

    return pTestSuite;
}
