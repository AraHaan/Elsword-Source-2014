#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include <map>

//{{ ���� ���� �ý��� - ��μ�
#ifdef SERV_SERVER_BUFF_SYSTEM


class KBuffManager
{
	DeclareSingleton( KBuffManager );
	DeclDump;
	DeclToStringW;

public:
	KBuffManager(void);
	~KBuffManager(void);

	// ���� ���� ����
	bool AddUnitBuffInfo( IN const UidType& iUnitUID, IN const std::vector<KBuffInfo>& vecBuffInfo );
	bool DeleteUnitBuffInfo( IN UidType& iUnitUID, IN std::vector<int>& vecBuffID );
#ifdef _CONVERT_VS_2010
	bool DeleteUnitAllBuffInfo( IN UidType iUnitUID );
#else
	bool DeleteUnitAllBuffInfo( IN UidType& iUnitUID );
#endif _CONVERT_VS_2010
	
	// ���ŵ� ���� �ð� �߰�
	bool AddRenewBuffTime( IN const UidType& iUnitUID, IN const __int64& iNewTime, IN const __int64 iOleTime = 0 );

	// ���� �ð� ����
	void DeleteBuffTime( IN __int64& iTime, IN UidType iUnitUID );

	// ������ ���� ���� ���
	bool GetUnitAllBuffInfo( IN UidType& iUnitUID, OUT KUnitBuffInfo& kUnitBuffInfo );

	// ���� �ð� ���� ���� �����ֱ�
	void CheckEndBuff();

private:

	std::map< UidType, KUnitBuffInfo >					m_mapUnitBuffList;			// ���Ϻ� ���� ����					<����, ���ֹ�������>
	std::map< __int64, std::vector<UidType> >			m_mapUnitTime;				// ���� ���õ� �ð��� ������ ����	<���� ��� �ð�, ���ֵ�>

};

DefSingletonInline( KBuffManager );


#endif SERV_SERVER_BUFF_SYSTEM
//}