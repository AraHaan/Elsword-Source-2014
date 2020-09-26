#pragma once

//////////////////////////////////////////////////////////////////////////
// 081020.hoons.
//des : ���Ӽ����� ��Ŵ�����
//		- ���������� �ʵ����
//		- ��������Ʈ���� �̷�� ���� ��Ƽ���� ���
//		������ ���� 1�� ������.
//////////////////////////////////////////////////////////////////////////

#include "Field.h"
#include "Room/RoomManager.h"
#include "KncSend.h"

class KFieldManager : public KRoomManager
{
	NiDeclareRTTI;
	DeclDump;
	DeclToStringW;

	//�Ѥ�;; �������Ļ� �Ľ��س��� init���� �����Ҷ��..
	struct sFieldInfo
	{
		int	m_iID;
		KTileInfo	m_kTileInfo;
		KMapInfo	m_kMapInfo;
	};

public:
	KFieldManager();
	virtual ~KFieldManager();

	virtual void Run();	// derived from KThread

	bool	Init();
	void	Tick();

	void	RegToLua();
	void	AddFieldInfo_LUA();	
	void	SetFieldManagerDump_LUA( bool bVal ) { m_bIsFieldManagerDump = bVal; }

	UidType OpenField();
	bool	CloseField( UidType iRoomUID );

	// derived from KRoomManager
	virtual void QueueingEventToRoom( const KEventPtr& spEvent );
	virtual void QueueingToAll( int iRoomType, const KEventPtr& spEvent );
	virtual void QueueingToAll( const KEventPtr& spEvent );

	// packet send function
	template < class T > void SendToCharacter( UidType nTo, unsigned short usEventID, const T& data );
	void SendToCharacter( UidType nTo, unsigned short usEventID );

public:
	KFieldPtr	Get( UidType iRoomUID );

protected:
	std::map<UidType,KFieldPtr>		m_mapFiledList;
	std::map<UidType,UidType>		m_mapEmptyFieldList;
	FSMclassPtr						m_spFieldFSM;

	std::vector<sFieldInfo>			m_vecFieldInfo;

	mutable	KncCriticalSection		m_csEmptyFieldList;

	//////////////////////////////////////////////////////////////////////////
	boost::timer					m_tDumpTimer;
	bool							m_bIsFieldManagerDump;
	//////////////////////////////////////////////////////////////////////////
};

DefKObjectInline( KFieldManager, KRoomManager );

template < class T >
void KFieldManager::SendToCharacter( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_FIELD, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

/*    64 bit ( 8 byte )

| ff ff ff ff | ff ff ff ff |

| 00 00 00 ff | ff ff ff ff | ���� uid ����(SetPureUID). ( 5byte = 40bit. 2^40 = 1 0995 1162 7776 )
| 80 00 00 00 | 00 00 00 00 | ��ȣ��Ʈ�� ���ܵ�
| 40 00 00 00 | 00 00 00 00 | ���ÿ��� �ӽ÷� ����Ǵ� uid.
| 3f 00 00 00 | 00 00 00 00 | ������ uid.
| 00 ff ff 00 | 00 00 00 00 | code => villiage id

*/
