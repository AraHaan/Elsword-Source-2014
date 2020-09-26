// NNClientAuthLib.h: interface for the CSSOWebLib class.
//
/// @file	NNClientAuthLib.h
/// @brief	Client Authentication Library Header
/// @author	Jeong Yeosang<ysjeong76@cj.net>
/// @date 	2012.09.25
//
//////////////////////////////////////////////////////////////////////

#define COUNTRYCODE_INDONESIA	1
#define _DLL_EXPORTFUNC_ extern "C" _declspec(dllexport)


/**
@brif - ������ ���õ� ��� ����Ÿ ������ �ʱ�ȭ �Ѵ�.
@returns ����
*/
_DLL_EXPORTFUNC_ void NMClientAuthDLL_ClearAuthenticationData();

/**
@brif - ������ ���õ� �������� �����Ͽ� ������ �����Ѵ�.
@returns ���� ���θ� �����Ѵ�.
@param[in] userID - ����� ���̵�
@param[in] password - �н�����
@param[in] gameCode - �����ϴ� ���� �ڵ�
@param[in] redirectUrl - Redirect �� URL
@param[in] isPersisted - �α��� ���� ��� ��� ����
@param[in] isGameToken - ���� ������ ���� ��ū �߱� ����
@param[in] domain - ���� ������
@param[in] xmlURL - ���� ���� URL
*/
_DLL_EXPORTFUNC_ BOOL NMClientAuthDLL_SetLoginData(	LPCSTR userID,
																						LPCSTR password,
																						LPCSTR gameCode,
																						LPCSTR redirectUrl,
																						BOOL isPersisted,
																						BOOL isGameToken,
																						LPCSTR domain,
																						LPCSTR xmlURL);

/**
@brif - ���� ������ ����� �Ͽ� ���� ���������� �����ϰ� ���� �� ��������Ÿ���� ȯ�溯���� �����Ѵ�.
@returns ���� ���θ� �����Ѵ�.
*/
_DLL_EXPORTFUNC_ BOOL NMClientAuthDLL_LoginProcessExecution();

/**
@brif - �����̿Ϸ�Ǹ� ȣ���Ͽ� �ν¼��� ���ο� �����ڵ�, �����޽���, �������� ������� �ڵ带 �޴´�.
@returns ���� ���θ� �����Ѵ�. MBCS ������ �޽����� ���Ϲޱ⸦ ���� �� ����Ѵ�.
@param[in] loginSuccess - ���� ���� ����
@param[in] errorMessage - ���� �޽���(MBCS)
@param[in] errorCode - ���� �ڵ�(MBCS)
@param[in] queryInfoStatusCode - �Լ��� HTTP ��� ������ ���ϰ�
*/
_DLL_EXPORTFUNC_ BOOL NMClientAuthDLL_IsLoginComplete(	BOOL& loginSuccess,
																							LPCSTR& errorMessage,
																							LPCSTR& errorCode,
																							int& queryInfoStatusCode);

/**
@brif - �����̿Ϸ�Ǹ� ȣ���Ͽ� �ν¼��� ���ο� �����ڵ�, �����޽���, �������� ������� �ڵ带 �޴´�.
@returns ���� ���θ� �����Ѵ�. UNICODE ������ �޽����� ���Ϲޱ⸦ ���� �� ����Ѵ�.
@param[in] loginSuccess - ���� ���� ����
@param[in] errorMessage - ���� �޽���(UNICODE)
@param[in] errorCode - ���� �ڵ�(UNICODE)
@param[in] queryInfoStatusCode - �Լ��� HTTP ��� ������ ���ϰ�
*/
_DLL_EXPORTFUNC_ BOOL NMClientAuthDLL_IsLoginComplete_UTF16(	BOOL& loginSuccess,
																							LPCWSTR& errorMessage,
																							LPCWSTR& errorCode,
																							int& queryInfoStatusCode);