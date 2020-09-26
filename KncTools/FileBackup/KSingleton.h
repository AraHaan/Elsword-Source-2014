#ifndef  _KOG_KSINGLETON_H_
#define  _KOG_KSINGLETON_H_ "$Id: KSingleton.h,v 1.4 2003/01/15 12:54:56 cvs Exp $"
#pragma once // for _MSC_VER > 1000

//{ added by florist
// code from <lua.h> -> <Common/KbDllApi.h>
#ifndef _DLLAPI
#if defined(_USRDLL) // DLL library compile ��
#define	_DLLAPI	__declspec(dllexport)
#elif defined(_DLL) // DLL library ���
#define	_DLLAPI	__declspec(dllimport)
#else
#define _DLLAPI
#endif
#endif

//}


/** Template to implement singleton classes.
Singleton�� ���α׷� ��ü���� ���� �ϳ��� �����ؾ� �ϴ� ���� �ǹ��Ѵ�.
�� template�� �̹� ������ class�� wrapping�ؼ�, Singleton �Ӽ���
�������� ����� �ش�. �̹� KLogManager�� class�� ����������,
<i>KSingleton<KLogManager>::GetInstance()</i> ȣ���� ���ο� (�׸���
������) KLogManager�� ����ų�, �̹� �����ϴ� KLogManager�� ��������
������ �ϰ�, <i>KSingleton<KLogManager>::GetInstance()->Log(...)</i> ��
���� ���·� KLogManager���� ����� ���� ����� �� �ִ�. ����� ������
���� Singleton�� ���α׷� ��ü�� ���� �ÿ�
<i>KSingleton<LogManager>::ReleaseInstance()</i> �Լ��� �ҷ��� ������
������ �Ѵ�.
*/
template <typename TYPE>
class KSingleton
{
private:
    KSingleton(void) { } // disable construction of this class !
public:
    /** Get the singleton instance. */
    static TYPE* GetInstance(void);
    /** Release the instance. */
    static void ReleaseInstance(void);
private:
    static TYPE* ms_selfInstance; ///< The real class for singleton.
};

template <typename TYPE>
TYPE* KSingleton<TYPE>::ms_selfInstance = 0;

template <typename TYPE>
TYPE* KSingleton<TYPE>::GetInstance(void)
{
    if (ms_selfInstance == 0)
    {
        ms_selfInstance = new TYPE();
    }
    return ms_selfInstance;
}

template <typename TYPE>
void KSingleton<TYPE>::ReleaseInstance(void)
{
    if (ms_selfInstance != 0)
    {
        delete ms_selfInstance;
        ms_selfInstance = 0;
    }
}


#endif // _KOG_KSINGLETON_H_
