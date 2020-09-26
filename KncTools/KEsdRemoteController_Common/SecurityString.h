#pragma once
#include <map>
#include <string>

typedef std::wstring strType;

//////////////////////////////////////////////////////////////////////////
// ��� : ��ü �Ҹ�� heap memory ������ �ʱ�ȭ ��Ű�� string class
class KClearString
{
public:
	KClearString()										{}	
	KClearString( const strType::value_type* _str )		{ m_strBuf = _str; }
	KClearString( const strType& _str )					{ m_strBuf = _str; }
	~KClearString()										{ clear(); }

	const strType& GetString() const					{ return m_strBuf; }
	strType::iterator begin()							{ return m_strBuf.begin(); }
	strType::iterator end()								{ return m_strBuf.end(); }

	strType::reference operator[](strType::difference_type _Off) { return m_strBuf[_Off]; }

	const strType::value_type* c_str() const			{ return m_strBuf.c_str(); }
	void clear()										{ fill( m_strBuf.begin(), m_strBuf.end(), 0 ); m_strBuf.clear(); }
	int length() const									{ return static_cast<int>( m_strBuf.length() ); }

	void operator=( const strType& _str )				{ m_strBuf = _str; }
	void operator=( const KClearString& _str )			{ m_strBuf = _str.c_str(); }
	
	void operator+=( const strType::value_type _str )	{ m_strBuf += _str; }	
	void operator+=( const strType::value_type* _str )	{ m_strBuf += _str; }
	void operator+=( const strType& _str )				{ m_strBuf += _str; }
	void operator+=( KClearString& _str )				{ m_strBuf += _str.c_str(); }

private:
	strType m_strBuf;
};


//////////////////////////////////////////////////////////////////////////
// ��� : string������ �޸� ������ �л���� �ܺη� ������� �ʵ�����
// Ư¡ : �Ϲ����� string class�� ���� ������ ������
class KSecurityString
{
public:
	KSecurityString( bool bSrand = false );
	KSecurityString( const strType& strObj, bool bSrand = false );
	~KSecurityString(void);

	void SRand();

	void Init( const strType& strObj );
	//void Init( const KClearString& strObj );

	strType GetString() const;
	KClearString GetClearString() const;	

	bool IsEmpty() { return m_mapProtectBuf.empty(); }

private:
	void Encrypt() {}
	void Decrypt() {}

private:
	std::map< unsigned int, strType::value_type > m_mapProtectBuf;
};


