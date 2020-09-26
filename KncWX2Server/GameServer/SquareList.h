#pragma once

#include "RoomList.h"

class KSquareList : public KRoomList
{
	DeclToStringW;
	NiDeclareRTTI;

public:
	KSquareList( UINT uiRoomListID, ROOM_LIST_TYPE eRoomListType, UINT uiPageNum );
	virtual ~KSquareList(void);

	//���帮��Ʈ �����Լ�
	virtual bool		AddSquareInfo( KSquareInfoPtr spSquareInfo );
	virtual bool		DelSquareInfo( KSquareInfoPtr spSquareInfo );
	virtual bool		GetSquareInfoPage( IN KEGS_SQUARE_LIST_REQ& kReq, OUT KEGS_SQUARE_LIST_ACK& kAck );

protected:
	std::vector< KSquareInfoPtr >		m_vecSquareList;
};

DeclOstmOperatorW( KSquareList );