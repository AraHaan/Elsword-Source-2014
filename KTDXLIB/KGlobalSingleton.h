#pragma once

/** @file   KGlobalSingleton.h
    @brief  interface of KGlobalSingleton
    @author robobeg@kogstudios.com
    @since  2008-05-28 ���� 7:29

    @note   �׻� WinMain ������ global ��ü�� �����ǰ� WinMain�� �������� �� �Ҹ�Ǵ� ���� ����
            WinMain ������ �ٸ� global variable �ʱ�ȭ �������� Singleton()�� ������ ��쿡�� ��ü�� ����, �ʱ�ȭ���� ����

    @note   KGlobalSingleton ������ ������ destruction sequence�� ���� ������ �߻��� �� �ִ�.
            KGlobalSingleton �� destructor������ ������ �ƹ� �ϵ� ���� ���� ���α׷� ���� ���� ��� ��ó���� ��ġ��.

            [] �ı� ������ ������ �߻��ϴ� ����� ��:
                A,B�� ������ singleton ��ü�� ��, A��ü�� �ı��ڿ��� B��ü�� �����Ѵ�.
                ������, B��ü�� �̹� �ı��ǰ� ���� ���¶�� ������ �߻��Ѵ�.
                �̷��� ��Ȳ�� �߻����� �ʵ��� �ڵ��ϴ� ���� �ٶ����ϸ�, �̷��� ������ ����ؾ�
                �Ѵٸ�, A��ü�� �����ڿ��� B.Singleton()�� ȣ���� �ش�.
                �׷���, B�� A���� ���� �����ǹǷ�, �ı��� ���� �ݴ�� A�� ���� �ı��ȴ�.
                - jintaeks on 2008-05-29, 19:50
*/

// ������ Ŭ���� Ÿ��A�� ���� KGlobalSingleton<A>::Singleton() �� �̱��� ��ü �Ѱ��� �����Ѵ�.

// KGlobalSingleton�κ��� ������ Ŭ������ ��ü�δ� ���� �Ұ����� singleton���� ������� ���� �ΰ����� �߰��ؾ� �Ѵ�.
// 1. Ŭ���� ���ο� IMPLEMENT_KGLOBALSINGLETON( Ŭ������ ) �߰�
// 2. Ŭ������ ��� constructor�� private Ȥ�� protected �� ����

#ifdef CONVERSION_VS
#define IMPLEMENT_KGLOBALSINGLETON(T) \
    private: \
        friend class KGlobalSingleton<T>::KSingletonCreator;// \
        //T(const T&); \
        //T& operator = ( const T& );
#else CONVERSION_VS
#define IMPLEMENT_KGLOBALSINGLETON(T) \
	private: \
	friend class KGlobalSingleton<T>::KSingletonCreator; \
	T(const T&); \
	T& operator = ( const T& );
#endif CONVERSION_VS

template<typename T>
class   KGlobalSingleton
{
public:

    typedef T   Type;

    inline static Type&    Singleton() { return KSingletonCreator::Access(); }

    inline KGlobalSingleton() {}

private:

    KGlobalSingleton( const KGlobalSingleton& );
    KGlobalSingleton& operator = ( const KGlobalSingleton& );

#ifdef CONVERSION_VS
	protected:
#else CONVERSION_VS
	private:
#endif CONVERSION_VS

    class   KSingletonCreator
    {
    public:

        inline KSingletonCreator()
        {
            Access();
        }//KSingletonCreator()

        inline static Type&    Access()
        {
            /** inline Ȯ��Ǵ� �Լ��� ���ο� ����� static ������ instance�� �������� ����
                ���׸� �ذ��ϱ� ����, singleton�� instance�� ��� �Լ����� inline Ȯ�����
                �ʴ� _NoInlineInstance()�Լ��� reference�� �޾Ƽ� ����ϵ��� �ϴ�.
                - jintaeks on 2008-05-28, 9:45 */
            static  Type&  ref = _NoInlineAccess();
            return  ref;
        }//Access()

    private:

        inline void _DoNothing() {}

        __declspec(noinline) static Type& _NoInlineAccess()
        {
            static Type obj;
            /** WinMain()���� ���� Singleton()�� ȣ������ �ʴ��� ��ü��
                �����Ǵ� ���� �����ϱ� ����,
                ���� ��ü�� �����ڰ� _NoInlineInstance()�� ȣ���ϵ��� �ϴ�.
                ms_kSingletonCreator��ü�� ����Ǿ�����, �����ϴ� ���� �����Ƿ�
                ��ü�� �������� �ʴ´�. ������, KSingletonCreator�� �����ڰ�
                Access()�� ȣ���� ��, ó�� ȣ��Ǵ� _NoInlineAccess();�Լ���
                ���ο� ms_kSingletonCreator�� ��� �Լ�, _DoNothing()�� ȣ���ϴ�
                �ڵ尡 �����Ƿ�, ms_kSingletonCreator�� �����ؾ� �ϸ�,
                �̰��� _NoInlineAccess()���ο� �����,
                static Type obj;�� ������ �������� �ǹ��Ѵ�.
                - jintaeks on 2008-05-28, 9:48 */
            ms_kSingletonCreator._DoNothing();
            return  obj;
        }//_NoInlineAccess()

    private:
        static KSingletonCreator    ms_kSingletonCreator;

    };//struct  KSingletonCreator

};//template<typename T>


template <typename T>
/*static*/ typename KGlobalSingleton<T>::KSingletonCreator  KGlobalSingleton<T>::KSingletonCreator::ms_kSingletonCreator;
