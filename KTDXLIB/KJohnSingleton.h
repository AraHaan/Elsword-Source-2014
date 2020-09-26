/** @file   KJohnSingleton.h
	@brief  interface of KJohnSingleton
	@author seojt@kogstudios.com
	@since  2003-12-08 ���� 6:06:27

    @date   jintaeks on 2007-04-12, 9:56
            - IMPLEMENT_KJOHNSINGLETON_FUN(T) ��ũ�� �߰�
*/

#ifndef _KJOHNSINGLETON_H
#define _KJOHNSINGLETON_H

#include "KJohnGen.h"

//#define IMPLEMENT_KJOHNSINGLETON(T)     public: inline static T* GetSingleton() { ASSERT(ms_singleton); return ms_singleton; }
#define IMPLEMENT_KJOHNSINGLETON(T)     
#define IMPLEMENT_KJOHNSINGLETON_FUN(T) inline T* _##T() { return T::GetSingleton(); }


//------------------------------------------------------------------------------
/// @class  KJohnSingleton<>
/// @desc   KJohnSingleton<>�� �ֵ� ������ singleton ������ ����ϸ鼭, ��ü�� ���� �Ҹ� ������
///         ����ڰ� manual�ϰ� �����Ϸ��� ���̴�.
/// @note   you can control singleton creation time
/**
            [] ��ü�� ������ singleton interface�� ���:

            ���� ��ü �������� KJohnSingleton<>�� ��ӹ��� ��ü�� ���� ���� �ְ�,
            CreateSingleton() ���� �Լ��� ����Ͽ� �������� ��ü�� ������ �� �� �ִ�.
            ��� ������� ��ü�� ��������� ����,
            �̱��� ��ü�� ����ϴ� �������� ��ü�̸��� �ƴ϶�,
            KJohnSingleton<>�� �����ϴ� interface�� ����Ͽ� singleton ��ü�� ���������ν�
            ��ü�� �̸��� independent�� interface�� ����� �� �ְ� �ȴ�.
            - �ڸ�Ʈ �ۼ� jintaeks on 2008-04-23, 16:00
*/
template<typename T>
class KJohnSingleton
{
protected:
    static T*   ms_singleton;
    static bool ms_bInternalCreation;   ///< CreateSingleton()�� ȣ���Ͽ� ��ü�� ������ ��� true�̴�.

public:
    /// constructor.
	KJohnSingleton()
    {
        ASSERT( NULL == ms_singleton );
        ms_singleton = (T*)this;
    }//KJohnSingleton()

    /// destructor.
    /// 'KJohnSingleton*'�� �ٷ� ����Ͽ� ��ü�� �ı��� ���� ���ٰ� �����ϰ�
    /// �ı��ڸ� �������� ������ �ʴ´�. - jintaeks on 2008-02-12, 11:20
	/*virtual*/ ~KJohnSingleton()
    {
        ASSERT( ms_singleton );
        if ( ms_bInternalCreation )
            DestroySingleton();
        ms_singleton = NULL;
    }//KJohnSingleton()

    static T* CreateSingleton()
    {
        if ( ms_singleton != NULL )
            return ms_singleton;

        ms_bInternalCreation = true;
        return ::new T();
    }//CreateSingleton()

    static void DestroySingleton()
    {
        ::delete ms_singleton;
        ms_bInternalCreation = false;
    }//DestroySingleton()

    static T* GetSingleton() { ASSERT(ms_singleton); return ms_singleton; }
};//class KJohnSingleton

template<typename T> T* KJohnSingleton<T>::ms_singleton = NULL;
template<typename T> bool KJohnSingleton<T>::ms_bInternalCreation = false;

#endif // _KJOHNSINGLETON_H


/** @example    KJohnSingleton<>
    @code

    #include <iostream>
    #include "KJohnSingleton.h"

    class KManager : public KJohnSingleton<KManager>
    {
        IMPLEMENT_KJOHNSINGLETON(KManager)

    public:
        void Print() { std::cout << "hello singleton" << std::endl; }
    };//class KManager

    KManager    instance;

    void main()
    {
        KManager::GetSingleton()->Print();
    }//main()

    @endcode
*/
