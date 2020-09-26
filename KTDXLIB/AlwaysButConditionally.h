#pragma once 








#ifdef IN_TESTING_THEMIDA_BY_TOOL_TEAM
#	pragma message( "THEMIDA_BY_TOOL_TEAM, REFACTORING_BY_TOOL_TEAM, CPPUNIT_BY_TOOL_TEAM, NO_ALL_KILL_HACKING_TEST ���� ����ǰ� �ֽ��ϴ�. ������ ��ġ�Ǹ� �ȵ˴ϴ�!")
// �������� themida ���� �׽�Ʈ �� �Դϴ�.
#	define THEMIDA_BY_TOOL_TEAM
// themida�� �����ϱ� ���� �ʿ��� refactoring �۾� �������Դϴ�. 
#	define REFACTORING_BY_TOOL_TEAM
// cppunit unittest library 
//#	define CPPUNIT_BY_TOOL_TEAM
// ���� ��� ���̴� ��ŷ ���� �׽�Ʈ, REFACTORING_BY_TOOL_TEAM�� ���� ���� �Ǿ�� �մϴ�.
//#	define NO_ALL_KILL_HACKING_TEST  
#endif IN_TESTING_THEMIDA_BY_TOOL_TEAM




#ifdef IN_TESTING_THEMIDA_BY_TOOL_TEAM
#else IN_TESTING_THEMIDA_BY_TOOL_TEAM
#	pragma message( "DAMAGE_HISTORY, VIRTUALIZER_CODE�� ����˴ϴ�.")
#	define DAMAGE_HISTORY	// ������ �����丮(�ٹ�����)
#	define VERIFY_NPC_HP	// �� ǥ�� ���� HP ��ȿ�� �˻� (������ �����丮�� ���� ����)
//{{ hoon.2009-10-26. �ڵ� �κ���ȣȭ ���̺귯��.
#	define VIRTUALIZER_CODE
//}} hoon.2009-10-26.
#endif IN_TESTING_THEMIDA_BY_TOOL_TEAM













#ifdef VIRTUALIZER_CODE
#	include "../X2Lib/VirtualizerSDK.h"
#	define ELSWORD_VIRTUALIZER_START	VIRTUALIZER_START
#	define ELSWORD_VIRTUALIZER_END		VIRTUALIZER_END
#else VIRTUALIZER_CODE
#	define ELSWORD_VIRTUALIZER_START
#	define ELSWORD_VIRTUALIZER_END
#endif VIRTUALIZER_CODE






#ifdef THEMIDA_BY_TOOL_TEAM
#	pragma message( "�������� themida ���� �׽�Ʈ �� �Դϴ�." )
#	include "../X2Lib/ThemidaSDK.h"
#	define THEMIDA_VM_START					VM_START
#	define THEMIDA_VM_END					VM_END
#	define THEMIDA_CODEREPLACE_START		CODEREPLACE_START
#	define THEMIDA_CODEREPLACE_END			CODEREPLACE_END
//#	define THEMIDA_ENCODE_START				ENCODE_START // ũ���� �ڲ� ���ܼ� ���� �Ƚ�Ŵ
//#	define THEMIDA_ENCODE_END				ENCODE_END
#	define THEMIDA_ENCODE_START			
#	define THEMIDA_ENCODE_END
#	define THEMIDA_CLEAR_START				CLEAR_START
#	define THEMIDA_CLEAR_END				CLEAR_END            
#	define THEMIDA_CHECK_PROTECTION			CHECK_PROTECTION
#	define THEMIDA_CHECK_CODE_INTEGRITY		CHECK_CODE_INTEGRITY
#	define THEMIDA_REPORT_IF_TAMPERED( variableName, variableValue, reportString ) if( variableValue != variableName ) { g_pKTDXApp->SetFindHacking( true, reportString ); }
#else THEMIDA_BY_TOOL_TEAM
#	define THEMIDA_VM_START				
#	define THEMIDA_VM_END				
#	define THEMIDA_CODEREPLACE_START	
#	define THEMIDA_CODEREPLACE_END		
#	define THEMIDA_ENCODE_START			
#	define THEMIDA_ENCODE_END
#	define THEMIDA_CLEAR_START
#	define THEMIDA_CLEAR_END
#	define THEMIDA_CHECK_PROTECTION( a, b )
#	define THEMIDA_CHECK_CODE_INTEGRITY( a, b )
#	define THEMIDA_REPORT_IF_TAMPERED( variableName, variableValue, reportString )
#endif THEMIDA_BY_TOOL_TEAM


#ifdef CPPUNIT_BY_TOOL_TEAM
#	if defined(DEBUG) || defined(_DEBUG)
#		pragma comment( lib, "cppunitd.lib" )
#	else
#		pragma comment( lib, "cppunit.lib" )
#	endif 
#endif CPPUNIT_BY_TOOL_TEAM

