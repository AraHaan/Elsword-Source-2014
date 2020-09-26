#pragma once

/*
�� room / square info�� �����ͷ� �ϴ� ������ �Ŵ������� ������������ �ϰ�
�� ä�ο��� �渮��Ʈ ������ �ϱ� �����̸� �Ŵ����� map�� �̿��� ����������
���� ���� �������� ����ϰ� ä���� vector�� �渮��Ʈ ������ ���ϱ⶧��.

�� �Ŵ������� �θ������͸� ������ �ֱ⶧���� �ڽ�Ŭ�������� ����ϰԵ� ����Լ���
���������Լ��� �����Ѵ�.
*/

#include <vector>
#include "UserManager.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLSquareGame.h"

// class
#define KRoomListPtr KRoomList*
#undef KRoomListPtr
SmartPointer( KRoomList );

#define KPVPListPtr KPVPList*
#undef KPVPListPtr
SmartPointer( KPVPList );

#define KSquareListPtr KSquareList*
#undef KSquareListPtr
SmartPointer( KSquareList );

// struct
#define KRoomInfoPtr KRoomInfo*
#undef KRoomInfoPtr
SmartPointers( KRoomInfo );

#define KSquareInfoPtr KSquareInfo*
#undef KSquareInfoPtr
SmartPointers( KSquareInfo );

class KRoomList
{
	DeclToStringW;
	NiDeclareRTTI;

public:
	enum ROOM_LIST_TYPE
	{
		RLT_NONE = 0,
		RLT_PVP,
		RLT_FREE_PVP,
		RLT_SQUARE,
		RLT_PARTY,

		RLT_END,
	};

public:
	KRoomList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum );
	virtual ~KRoomList(void);

	UINT			GetID()										{ return m_uiRoomListID; }

	void			SetType( ROOM_LIST_TYPE eRoomListType )		{ m_eRoomListType = eRoomListType; }
	ROOM_LIST_TYPE	GetType()									{ return m_eRoomListType; }

	UINT			GetPageNum()								{ return m_uiPageNum; }

public:
	//////////////////////////////////////////////////////////////////////////
	//����Ŭ�������� ����ϰԵ� �Լ����.
	virtual UidType		GetWaitingRoomUID( IN KEGS_QUICK_JOIN_REQ& kReq )			{ return 0; }
	virtual void		SetVSPoint( IN int iMin, IN int iMax )		{}
	virtual void		GetVSPoint( OUT int& iMin, OUT int& iMax )	{}

	virtual bool		AddRoomInfo( KRoomInfoPtr spRoomInfo )		{ return false; }
	virtual bool		DelRoomInfo( KRoomInfoPtr spRoomInfo )		{ return false; }
	virtual bool		GetRoomInfoPage( IN KEGS_ROOM_LIST_REQ& kReq, OUT KEGS_ROOM_LIST_ACK& kAck ){ return false; }

	virtual bool		AddSquareInfo( KSquareInfoPtr spSquareInfo ){ return false; }
	virtual bool		DelSquareInfo( KSquareInfoPtr spSquareInfo ){ return false; }
	virtual bool		GetSquareInfoPage( IN KEGS_SQUARE_LIST_REQ& kReq, OUT KEGS_SQUARE_LIST_ACK& kAck ){ return false; }

	virtual void		SetPVPChannelClass( IN int iPVPChannelClass ){}
	virtual int			GetPVPChannelClass()						{ return 0; }
	//////////////////////////////////////////////////////////////////////////

protected:
	template< class T >	void PreInsertVector( std::vector<T>& vecList, T data );

protected:
	UINT			m_uiRoomListID;
	ROOM_LIST_TYPE	m_eRoomListType;
	UINT			m_uiPageNum;		// ���������� ��� �ٰ�����..!
};

template< class T >
void KRoomList::PreInsertVector( std::vector<T>& vecList, T data )
{
	if( vecList.empty() == true )
	{
		vecList.push_back( data );
	}
	else
	{
		vecList.insert( vecList.begin(), data );
	}
}

DeclOstmOperatorW( KRoomList );



