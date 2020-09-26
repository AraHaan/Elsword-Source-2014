#pragma once

// �����ڴ� ��������� protected�� �̵��ϱ� ����
// ��ũ�ο� �������� �ʴ´�. �̱������� ����ϰ��� �ϴ�
// Ŭ������ ������ ������ ��������� �����ڸ� protected�� �Ű��־�� �Ѵ�.
#define DeclareSingleton( classname ) \
    public: \
        static classname* GetInstance(); \
        static void ReleaseInstance(); \
    protected: \
        static classname*   ms_selfInstance

#define DefSingletonInline(ClassName)   \
    inline ClassName* Si##ClassName##() { return ClassName::GetInstance(); }

#define ImplementSingleton( classname ) \
    classname* classname::ms_selfInstance = NULL; \
    classname* classname::GetInstance() \
    { \
        if( ms_selfInstance == NULL ) \
            ms_selfInstance = new classname; \
        return ms_selfInstance; \
    } \
    void classname::ReleaseInstance() \
    { \
        if( ms_selfInstance != NULL ) \
        { \
            delete ms_selfInstance; \
            ms_selfInstance = NULL; \
        } \
    }

// instance keeping : �̱��� ������ ����� �ϰ� ������, ��ü ���� ������ �Ҹ������ ��Ȯ�� ������.
// ���� �����ؼ� �����ص� �ν��Ͻ����� ������ �����ϴٴ� ������ �̱��ϰ� �����ϰ� ó����.
// KObj : Keeped Object - �����Ǿ� �������� ��ü(�ν��Ͻ�).

#define DeclInstanceKeeping( classname ) \
    public: \
        static classname* GetKObj(); \
        static void ReleaseKObj(); \
        template<class T> static classname* KeepObject() { ms_object = new T; return ms_object; } \
    protected: \
        static classname*   ms_object

#define DefKObjectInline( child, parent )   \
    inline child* Get##child##() { return (child*)parent::GetKObj(); }

#define ImplInstanceKeeping( classname ) \
    classname* classname::ms_object = NULL; \
    classname* classname::GetKObj() \
    { \
        return ms_object; \
    } \
    void classname::ReleaseKObj() \
    { \
        if( ms_object != NULL ) \
        { \
            delete ms_object; \
            ms_object = NULL; \
        } \
    }