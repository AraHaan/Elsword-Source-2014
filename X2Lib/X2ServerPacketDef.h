#ifdef DECL_DATA
#undef DECL_DATA
#endif

#define _DECL_DATA(name)    struct name; \
	SERIALIZE_DEFINE_TAG(name, eTAG_USERCLASS); \
	SERIALIZE_DECLARE_PUTGET(name); \
struct name
#define  DECL_DATA(name)    _DECL_DATA( name ) : public boost::noncopyable

#define COPYCON_ASSIGNOP( type, t ) type(const type& t) { *this = t; } \
	type& operator=(const type& t)

#pragma pack( push, 1 )

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_CONNECT_REQ )
{
	char		gameMainVersion;
	char		gameSubVersion;
	char		gamePatchVersion;
};

DECL_DATA( KEVENT_X2_SERVER_CONNECT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_MAIN_VERSION_DIF,		// ���� ������ �ٸ�
		EC_SUB_VERSION_DIF,			// ���� ������ �ٸ�
		EC_GAMEPATCH_VERSION_DIF,	// GAME PATCH ������ �ٸ�
		EC_AUTHENTIC_CODE_FAIL,		// �������� �ڵ尡 �߸��Ǿ���.
	};

	bool			bResult;
	char			errCode;
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_NEW_USER_JOIN_REQ )
{
	std::wstring	ID;
	std::wstring	password;
	std::wstring	nickName;
	std::wstring	name;
};

DECL_DATA( KEVENT_X2_SERVER_NEW_USER_JOIN_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_IDENTICAL_ID,		// ���� ID�� ����.
		EC_IDENTICAL_NICKNAME,	// ���� NICK NAME ����.
		EC_EMPTY_ID,			// ID �Է��� ����.
		EC_READ_DB_FAIL,		// DB �б� ����
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_AUTHENTIC_CODE_FAIL,	// �������� �ڵ尡 �߸��Ǿ���.
		EC_DB_UNKNOWN,				// DB Ʈ������ ����
	};

	bool			bResult;
	char			errCode;
	int				userUID;
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_LOGIN_REQ )
{
	std::wstring	ID;
	std::wstring	password;
};

DECL_DATA( KEVENT_X2_SERVER_LOGIN_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_USER_DONOT_EXIST,		// ������ �������� ����
		EC_PW_DIF,					// �н����尡 Ʋ��
		EC_DELETE_USER,				// ������ ������
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_EMPTY_ID,				// ID �Է��� ����.
		EC_DB_READ_USERDATA_FAIL,	// ���� ������ �о���� ����
		EC_PRE_USER_FAIL,			// ���� �����ڰ� ����.
	};

	bool			bResult;
	char			errCode;
	int				userUID;
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_USER_INFO_REQ )
{
	int				userUID;
};

DECL_DATA( KEVENT_X2_SERVER_USER_INFO_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_USERUID_DIF,				// SERVER/CLIENT ���� UID�� �ٸ�
		EC_DB_READ_USERDATA_FAIL,	// ���� ������ �о���� ����
	};

	bool			bResult;
	char			errCode;

	int				userUID;			//������ ����ũ ���̵�
	std::wstring	userID;				//���̵�
	std::wstring	userNickName;		//�г���

	char			maxUnitCount;		//���� ���� �� �ִ� ĳ���� ��(������ �ִ°� ����)
	int				cash;				//����ĳ��

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_USER_INFO_ACK, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		userUID			= right.userUID;
		userID			= right.userID;
		userNickName	= right.userNickName;

		maxUnitCount	= right.maxUnitCount;
		cash			= right.cash;

		return *this;
	}
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_UNIT_INFO_REQ )
{
	int				unitUID;
};

DECL_DATA( KEVENT_X2_SERVER_UNIT_INFO_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_DB_READ_UNITDATA_FAIL,	// DB�� UNIT ������ ����
		EC_UNITUID_INFOR_FAIL,		// UNITUID�� UNIT ������ ����
	};

	bool				bResult;
	char				errCode;

	int					userUID;
	int					unitUID;
	int					unitClass;

	std::wstring		IP;
    std::wstring		nickName;

	int					GP;
	char				level;
	int					EXP;

	short				inventorySize;
	char				quickSlotSize;

	char				Str;
	char				Dex;
	char				Int;
	char				Vit;
	char				Luck;

	char				remainStatPoint;
	std::vector<int>	nowEqipItemUIDList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_UNIT_INFO_ACK, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		userUID			= right.userUID;
		unitUID			= right.unitUID;
		unitClass		= right.unitClass;

		IP				= right.IP;
		nickName		= right.nickName;

		GP				= right.GP;
		level			= right.level;
		EXP				= right.EXP;

		inventorySize	= right.inventorySize;
		quickSlotSize	= right.quickSlotSize;

		Str				= right.Str;
		Dex				= right.Dex;
		Int				= right.Int;
		Vit				= right.Vit;
		Luck			= right.Luck;

		remainStatPoint	= right.remainStatPoint;
		nowEqipItemUIDList.clear();
		for( int i = 0; i < (int)right.nowEqipItemUIDList.size(); i++ )
		{
			nowEqipItemUIDList.push_back( right.nowEqipItemUIDList[i] );
		}
		return *this;
	}

	void Init()
	{
		bResult			= true;
		errCode			= EC_NO_ERROR;

		userUID			= -1;
		unitUID			= -1;
		unitClass		= 0;

		IP				= L"";
		nickName		= L"";

		GP				= 0;
		level			= 0;
		EXP				= 0;

		inventorySize	= 0;
		quickSlotSize	= 0;

		Str				= 0;
		Dex				= 0;
		Int				= 0;
		Vit				= 0;
		Luck			= 0;

		remainStatPoint	= 0;
		nowEqipItemUIDList.clear();
	}
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_UNIT_INFO_LIST_REQ )
{
	int				userUID;
};

DECL_DATA( KEVENT_X2_SERVER_UNIT_INFO_LIST_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_USERUID_DIF,			// SERVER/CLIENT ���� UID�� �ٸ�
	};

	bool			bResult;
	char			errCode;

	std::vector<KEVENT_X2_SERVER_UNIT_INFO_ACK>	unitInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_UNIT_INFO_LIST_ACK, right )
	{
		bResult		= right.bResult;
		errCode		= right.errCode;

		unitInfoList.clear();
		for( int i = 0; i < (int)right.unitInfoList.size(); i++ )
		{
			unitInfoList.push_back( right.unitInfoList[i] );
		}

		return *this;
	}
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_ITEM_INFO_REQ )
{
	int				itemUID;
};

DECL_DATA( KEVENT_X2_SERVER_ITEM_INFO_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ITEMUID_INFOR_FAIL,	// ITEMUID�� ������ ����
		EC_ITEM_INFOR_FAIL,		// ITEM ������ ����
	};
	bool			bResult;
	char			errCode;

	int				itemUID;				//UID
	int				itemID;

	int				ownerUserUID;
	int				ownerUnitUID;

	short			hyperGage;				//������ ��� ���� ��ġ
	short			endurance;				//������
	short			quantity;				//����

	char			enchantAttribute;
	char			enchantLevel;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_ITEM_INFO_ACK, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		itemUID			= right.itemUID;
		itemID			= right.itemID;

		ownerUserUID	= right.ownerUserUID;
		ownerUnitUID	= right.ownerUnitUID;

		hyperGage		= right.hyperGage;
		endurance		= right.endurance;
		quantity		= right.quantity;

		enchantAttribute	= right.enchantAttribute;
		enchantLevel		= right.enchantLevel;

		return *this;
	}
};


//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_UNIT_INVENTORY_ITEM_LIST_REQ )
{
	int				unitUID;
};

DECL_DATA( KEVENT_X2_SERVER_UNIT_INVENTORY_ITEM_LIST_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_USERUID_INVENITEM_FAIL,	// USERUID�� �κ��丮 �������� ����
		EC_UNIT_INFO_FAIL,			// UNIT ������ ����
	};

	bool			bResult;
	char			errCode;

	std::vector<KEVENT_X2_SERVER_ITEM_INFO_ACK>	inventoryItemList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_UNIT_INVENTORY_ITEM_LIST_ACK, right )
	{
		bResult		= right.bResult;
		errCode		= right.errCode;

		inventoryItemList.clear();
		for( int i = 0; i < (int)right.inventoryItemList.size(); i++ )
		{
			inventoryItemList.push_back( right.inventoryItemList[i] );
		}

		return *this;
	}
};


//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_UNIT_SELECT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
	};
	bool			bResult;
	char			errCode;
};


//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_CREATE_UNIT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
	};
	bool			bResult;
	char		errCode;
};



//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_UNIT_SELECT_REQ )
{
	int				unitUID;
};

_DECL_DATA( KEVENT_X2_SERVER_UNIT_SELECT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_SELECT_UNIT_FAIL,	// ������ UNIT UID�� SERVER�� ����
	};

	bool			bResult;
	char			errCode;

	int				unitUID;
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_CREATE_UNIT_REQ )
{
	std::wstring	nickName;
	int				unitClass;
};

_DECL_DATA( KEVENT_X2_SERVER_CREATE_UNIT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_USERUID_DB_SUCCESS_FAIL,	// DB�� ������û�� USER�� UID�� ����
		EC_IDENTICAL_NICKNAME,		// ���� NICK NAME ����.
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_READ_UNITDATA_FAIL,		// LUA������ UNIT ������ �������� ������
		EC_DB_READ_UNITINFO_FAIL,	// DB���� UNIT������ �������� ������.
		EC_CREATE_UNIT_FAIL,		// UNIT ��������
		EC_DB_UNKNOWN,				// DB Ʈ������ ����
	};

	bool			bResult;
	char			errCode;

	int				unitUID;
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_DELETE_UNIT_REQ )
{
	int				unitUID;
};

DECL_DATA( KDB_EVENT_X2_SERVER_DELETE_UNIT_REQ )
{
	int				userUID;
	int				unitUID;
};

_DECL_DATA( KEVENT_X2_SERVER_DELETE_UNIT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_UNIT_DB_SUCCESS_FAIL,	// �ش� UNIT�� DB�� ����
		EC_UNIT_SERVER_SUCCESS_FAIL,// �ش� UNIT�� SERVER�� ����
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_DB_UNKNOWN,				// DB Ʈ������ ����
	};

	bool			bResult;
	char			errCode;

	int				unitUID;
};
/*
//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_MAIN_LOBBY_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
	};

	bool			bResult;
	char			errCode;
};

*/

//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_VILLAGE_MAP_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
	};

	bool			bResult;
	char			errCode;
};

//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_LOCAL_MAP_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
	};

	bool			bResult;
	char			errCode;
};

//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_WORLD_MAP_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
	};

	bool			bResult;
	char			errCode;
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_CHANNEL_INFO )
{
	char			ID;
	std::wstring	name;
	char			minLevel;
	char			maxLevel;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_CHANNEL_INFO, right )
	{
		ID			= right.ID;
		name		= right.name;
		minLevel	= right.minLevel;
		maxLevel	= right.maxLevel;

		return *this;
	}
};

DECL_DATA( KEVENT_X2_SERVER_GET_PVP_CHANNEL_INFO_LIST_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_CHANNELINFOR_READ_FAIL,	// ä������ �б� ����
	};

	bool			bResult;
	char			errCode;

	std::vector<KEVENT_X2_CHANNEL_INFO>	channelInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_GET_PVP_CHANNEL_INFO_LIST_ACK, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		channelInfoList.clear();
		for( int i = 0; i < (int)right.channelInfoList.size(); i++ )
		{
			channelInfoList.push_back( right.channelInfoList[i] );
		}

		return *this;
	}
};


//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_JOIN_PVP_CHANNEL_REQ )
{
	bool			bInitChannel;
	char		channelID;
};

_DECL_DATA( KEVENT_X2_SERVER_JOIN_PVP_CHANNEL_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_ENTER_CHANNEL_FAIL,		// ä������ ����
	};

	bool			bResult;
	char			errCode;
	char			channelID;
};


//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_PVP_LOBBY_ACK )
{
	bool			bResult;
	char			errCode;
};

//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_REQ )
{
	short			pageNum;
};

DECL_DATA( KEVENT_X2_SERVER_ROOM_SIMPLE_INFO )
{
	int				roomNum;
	std::wstring	roomName;

	char			roomState;
	char			gameType;
	short			worldID;

	char			maxSlot;
	char			joinSlot;

	char			winGameNum;
	char			winKillNum;

	bool			bPossibleIntrude;	// ���԰��ɿ���
	bool			bIsOpen;			// ����/��� Ȯ�� ����

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_ROOM_SIMPLE_INFO, right )
	{
		roomNum		= right.roomNum;
		roomName	= right.roomName;

		roomState	= right.roomState;
		gameType	= right.gameType;
		worldID		= right.worldID;

		maxSlot		= right.maxSlot;
		joinSlot	= right.joinSlot;

		winGameNum	= right.winGameNum;
		winKillNum	= right.winKillNum;

		bPossibleIntrude	= right.bPossibleIntrude;
		bIsOpen				= right.bIsOpen;

		return *this;
	}
};

DECL_DATA( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_ROOMLIST_FAIL,			// �������� ����(�־�� �ȵɿ���)
	};

	bool			bResult;
	char			errCode;

	int				maxPageNum;
	std::vector<KEVENT_X2_SERVER_ROOM_SIMPLE_INFO> roomSimpleInfoList;	

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_ACK, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		maxPageNum		= right.maxPageNum;

		roomSimpleInfoList.clear();
		for( int i = 0; i < (int)right.roomSimpleInfoList.size(); i++ )
		{
			roomSimpleInfoList.push_back( right.roomSimpleInfoList[i] );
		}

		return *this;
	}
};

DECL_DATA( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_ROOM_LIST_FAIL,	// ������ ���� ������ ����(�̿����� �־�� �ȵȴ�.)
	};

	bool			bResult;
	char			errCode;

	int				maxPageNum;
	std::vector<KEVENT_X2_SERVER_ROOM_SIMPLE_INFO> roomSimpleInfoList;	

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_GET_PVP_ROOM_SIMPLE_INFO_LIST_NOT, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		maxPageNum		= right.maxPageNum;
		roomSimpleInfoList.clear();
		for( int i = 0; i < (int)right.roomSimpleInfoList.size(); i++ )
		{
			roomSimpleInfoList.push_back( right.roomSimpleInfoList[i] );
		}

		return *this;
	}
};




//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_PVP_ROOM_INFO )
{
	int							roomNum;
	std::wstring				roomName;

	char						hostSlot;

	char						roomState;
	char						gameType;

	int							winGameNum;
	int							winKillNum;

	int							worldID;

	bool						bPossibleIntrude;	// ���԰��ɿ���
	bool						bIsOpen;			// ����/��� Ȯ�� ����

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_PVP_ROOM_INFO, data )
	{
		roomNum			= data.roomNum;
		roomName		= data.roomName;

		hostSlot		= data.hostSlot;

		roomState		= data.roomState;
		gameType		= data.gameType;

		winGameNum		= data.winGameNum;
		winKillNum		= data.winKillNum;

		worldID			= data.worldID;

		bPossibleIntrude	= data.bPossibleIntrude;
		bIsOpen				= data.bIsOpen;

		return *this;
	}
};

DECL_DATA( KEVENT_X2_SERVER_SLOT_INFO )
{
	char			slotState;
	char			slotEvent;		// �������� �������� ���泻���� �˷��ֱ� ���� �÷���.

	char			teamNum;				// �� �ٲٱ�
	char			slotUnitSpecialMotion;	// ����ǿ��� ĳ���� ��� �ٲٱ�

	KEVENT_X2_SERVER_UNIT_INFO_ACK				unitInfo;
	std::vector<KEVENT_X2_SERVER_ITEM_INFO_ACK>	eqipItemList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_SLOT_INFO, data )
	{
		slotState	= data.slotState;
		slotEvent	= data.slotEvent;

		teamNum		= data.teamNum;
		slotUnitSpecialMotion	= data.slotUnitSpecialMotion;

		unitInfo	= data.unitInfo;

		eqipItemList.clear();
		for( int i = 0; i < (int)data.eqipItemList.size(); i++ )
		{
			eqipItemList.push_back( data.eqipItemList[i] );
		}

		return *this;
	}

	void Init()
	{
		slotState		= 1;
		slotEvent		= 0;

		teamNum			= 0;
		slotUnitSpecialMotion	= 0;

		unitInfo.Init();
		eqipItemList.clear();
	}
};




//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_CREATE_PVP_ROOM_REQ )
{
	std::wstring			roomName;
	char					gameMode;

	int						winGameNum;
	int						winKillNum;

	char					unitNum;

	bool					bIsOpen;
	std::wstring			RoomPasswd;

	bool					bPossibleIntrude;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_CREATE_PVP_ROOM_REQ, data )
	{
		roomName		= data.roomName;
		gameMode		= data.gameMode;

		winGameNum		= data.winGameNum;
		winKillNum		= data.winKillNum;

		unitNum			= data.unitNum;

		bIsOpen			= data.bIsOpen;
		RoomPasswd		= data.RoomPasswd;

		bPossibleIntrude	= data.bPossibleIntrude;

		return *this;
	}
};

DECL_DATA( KEVENT_X2_SERVER_CREATE_PVP_ROOM_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_CREATE_ROOM_FAIL,		// ROOM ���� ����
		EC_ENTER_ROOM_FAIL,			// ������ ���� ����
	};

	bool			bResult;
	char			errCode;

	KEVENT_X2_SERVER_PVP_ROOM_INFO		roomInfo;
	std::vector<KEVENT_X2_SERVER_SLOT_INFO>	slotInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_CREATE_PVP_ROOM_ACK, data )
	{
		bResult		= data.bResult;
		errCode		= data.errCode;

		roomInfo	= data.roomInfo;

		slotInfoList.clear();
		for( int i = 0; i < (int)data.slotInfoList.size(); i++ )
		{
			slotInfoList.push_back( data.slotInfoList[i] );
		}

		return *this;
	}
};



//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_JOIN_PVP_ROOM_REQ )
{
	int				roomNum;

	std::wstring	RoomPasswd;
};

DECL_DATA( KEVENT_X2_SERVER_JOIN_PVP_ROOM_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_LOST_ROOM,				// ������ ���� ����
		EC_CLOSE_ROOM,				// ���� ������.
		EC_ENTER_ROOM_FAIL,			// �ο��� Ǯ�̰ų� ������
		EC_HOSTUSER_FAIL,			// ������ ����.(������ ���� ���ӽ���)
		EC_PASSWORD_FAIL,			// ����� Ʋ����.
		EC_UNKNOWN,					// �˼����� ����
	};

	bool		bResult;
	char		errCode;

	KEVENT_X2_SERVER_PVP_ROOM_INFO		roomInfo;
	std::vector<KEVENT_X2_SERVER_SLOT_INFO>	slotInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_JOIN_PVP_ROOM_ACK, data )
	{
		bResult		= data.bResult;
		errCode		= data.errCode;

		roomInfo	= data.roomInfo;

		slotInfoList.clear();
		for( int i = 0; i < (int)data.slotInfoList.size(); i++ )
		{
			slotInfoList.push_back( data.slotInfoList[i] );
		}

		return *this;
	}
};



//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_EXIT_PVP_ROOM_ACK )
{
	bool		bResult;
	char		errCode;
};








//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_INFO_REQ )
{
	enum CHANGE_ROOMINF
	{
		CR_WINGAMENUM	= 0,
		CR_WINKILLNUM,
		CR_POSSIBLEINTRUDE,
		CR_WORLDID,
	};

	char			changeFlag;
	int				nChangeData;
	bool			bPossibleIntrude;	// ���԰��ɿ���

	//KEVENT_X2_SERVER_PVP_ROOM_INFO	roomInfo;
	//int							winGameNum;
	//int							winKillNum;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_INFO_REQ, data )
	{
		//changeRoomInf	= data.changeRoomInf;
		//roomInfo		= data.roomInfo;
		//winGameNum			= data.winGameNum;
		//winKillNum			= data.winKillNum;
		changeFlag			= data.changeFlag;
		nChangeData			= data.nChangeData;
		bPossibleIntrude	= data.bPossibleIntrude;
	}
};

_DECL_DATA( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_INFO_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_NOT_HOSTUSER,			// ������ �ƴѵ� ������ ���� ��û�� ����..
		EC_FLAG_FAIL,				// ������ ������ �ĺ� �÷��װ� �߸��Ǿ���(CHANGE_ROOMINF)
	};

	bool		bResult;
	char		errCode;
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_SLOT_INFO_REQ )
{
	enum SLOT_CHANGE_INF
	{
		SCI_NONE	= 0,
		SCI_GAME_READY,
		SCI_TEAM_CHANGE,
		SCI_SLOT_ONOFF,
		SCI_MOTION_CHANGE,
	};

	//note : Client ���� Server�� Slot������ �ٲ� ���� �÷���
	char			slotChangeFlag;

	char			slotIndex;				// ���� Open/Close �ɼ� ������ ����

	KEVENT_X2_SERVER_SLOT_INFO	slotInfo;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_SLOT_INFO_REQ, data )
	{
		slotChangeFlag	= data.slotChangeFlag;
		slotIndex		= data.slotIndex;

		slotInfo = data.slotInfo;
	}
};

_DECL_DATA( KEVENT_X2_SERVER_CHANGE_PVP_ROOM_SLOT_INFO_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_SLOTINFOR_SET_FAIL,		// �ش� USER�� SLOT ������ ����.
		EC_SLOT_MOVE_FAIL,			// �����̵�����..
	};

	bool		bResult;
	char		errCode;
};






//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_INFO_NOT )
{
	KEVENT_X2_SERVER_PVP_ROOM_INFO	roomInfo;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_INFO_NOT, data )
	{
		roomInfo = data.roomInfo;
	}
};

DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_SLOT_INFO_LIST_NOT )
{
	std::vector<char>						slotIndex;
	std::vector<KEVENT_X2_SERVER_SLOT_INFO>	slotInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_SLOT_INFO_LIST_NOT, data )
	{
		slotIndex.clear();
		for( int i = 0; i < (int)data.slotIndex.size(); i++ )
		{
			slotIndex.push_back( data.slotIndex[i] );
		}

		slotInfoList.clear();
		for( int i = 0; i < (int)data.slotInfoList.size(); i++ )
		{
			slotInfoList.push_back( data.slotInfoList[i] );
		}
	}
};






//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_PVP_ROOM_GAME_START_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_ROOMFSM_FAIL,			// ������� FSM���°� �߸��Ǿ���.
		EC_GAME_START_FAIL,			// ������Ŷ�� �� ����ڰ� ������ �ƴ�
		EC_NOT_READY,				// ������ ������ ���������� READY�� �Ǿ����� ����.
		EC_UNKNOWN,					// �˼����� ����
	};

	bool		bResult;
	char		errCode;
};


_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_START_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
	};

	bool		bResult;
	char		errCode;
};




//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_PVP_ROOM_GAME_HOST_READY_REQ )
{
	short		port;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_PVP_ROOM_GAME_HOST_READY_REQ, data )
	{
		port = data.port;
	}
};

_DECL_DATA( KEVENT_X2_SERVER_PVP_ROOM_GAME_HOST_READY_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
	};

	bool		bResult;
	char		errCode;
};

DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_HOST_READY_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
	};

	bool			bResult;
	char			errCode;

	std::wstring	IP;
	short			port;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_HOST_READY_NOT, data )
	{
		bResult		= data.bResult;
		errCode		= data.errCode;
		IP			= data.IP;
		port		= data.port;
	}
};




//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_PVP_ROOM_GAME_LOADING_COMPLETE_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
		EC_UNKNOWN,				// �˼����� ����
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_ALL_UNIT_LOADING_COMPLETE_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
	};

	bool		bResult;
	char		errCode;
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_GAME_ONE_UNIT_LOADING_COMPLETE_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
	};

	bool		bResult;
	char		errCode;
	char		slotIndex;
};


//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_PVP_GAME_RESULT_REQ )
{
	char	winTeam;
};

_DECL_DATA( KEVENT_X2_SERVER_PVP_GAME_RESULT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
		EC_NOT_HOSTUSER,
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_GAME_RESULT_NOT )
{
	char	winTeam;
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_PVP_GAME_RESULT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
		EC_NOT_HOSTUSER,
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_STATE_CHANGE_PVP_GAME_RESULT_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
	};

	bool		bResult;
	char		errCode;

	KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_SLOT_INFO_LIST_NOT slotList;
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_EXIT_PVP_GAME_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_STATE_CHANGE_FAIL,	// ���� ������ ���� ������Ʈ ���濡��
	};

	bool		bResult;
	char		errCode;
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_PVP_ROOM_CHAT_REQ )
{
	enum CHAT_EVENT
	{
		CE_NONE		= 0,
		CE_SECRET,
	};

	char			MsgType;
	char			mySlotIndex;
	std::wstring	strMsg;

	char			toSlotIndex;
};

_DECL_DATA( KEVENT_X2_SERVER_PVP_ROOM_CHAT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// �� ������Ʈ(FSM) ����
		EC_CHAT_TYPE_FAIL,		// ä�� �޼��� Ÿ���� �߸��Ǿ���.
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_CHAT_NOT )
{
	char			slotIndex;
	std::wstring	strMsg;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_PVP_ROOM_CHAT_NOT, data )
	{
		slotIndex		= data.slotIndex;
		strMsg			= data.strMsg;
	}
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_PVP_GAME_CHAT_REQ )
{
	enum CHAT_EVENT
	{
		CE_NONE		= 0,
		CE_SECRET,
	};

	char			MsgType;
	char			mySlotIndex;
	std::wstring	strMsg;

	char			toSlotIndex;
};

_DECL_DATA( KEVENT_X2_SERVER_PVP_GAME_CHAT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// �� ������Ʈ(FSM) ����
		EC_CHAT_TYPE_FAIL,		// ä�� �޼��� Ÿ���� �߸��Ǿ���.
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_PVP_GAME_CHAT_NOT )
{
	char			slotIndex;
	std::wstring	strMsg;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_PVP_GAME_CHAT_NOT, data )
	{
		slotIndex		= data.slotIndex;
		strMsg			= data.strMsg;
	}
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_RELAY )
{
	int		size;
	char	buffer[300];

	KEVENT_X2_SERVER_BROADCAST_RELAY()
	{
		ZeroMemory( buffer, sizeof(char) * 300 );
	}

	COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_RELAY, data )
	{
		size		= data.size;
		memcpy( buffer, data.buffer, sizeof(char) * 300 );
	}
};


//////////////////////////////////////////////////////////////////////////
//_DECL_DATA( KEVENT_X2_SERVER_END_PVP_GAME_REQ )
//{
//	char		winTeam;
//};
//
//_DECL_DATA( KEVENT_X2_SERVER_END_PVP_GAME_ACK )
//{
//	enum ERROR_CODE
//	{
//		EC_NO_ERROR			= 0,
//		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
//		EC_ROOM_STATE_FAIL,		// �� ������Ʈ ����
//		EC_NOT_HOSTUSER,		// ������ �ƴ�
//	};
//
//	bool		bResult;
//	char		errCode;
//};
//
//_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_END_PVP_GAME_NOT )
//{
//	enum ERROR_CODE
//	{
//		EC_NO_ERROR			= 0,
//	};
//
//	bool		bResult;
//	char		errCode;
//};

//////////////////////////////////////////////////////////////
//_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_PVP_ROOM_ACK )
//{
//	enum ERROR_CODE
//	{
//		EC_NO_ERROR			= 0,
//		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
//	};
//	bool			bResult;
//	char			errCode;
//};




// DUNGEON_TEST!!
DECL_DATA( KEVENT_X2_SERVER_GET_DUNGEON_CHANNEL_INFO_LIST_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_CHANNELINFOR_READ_FAIL,	// ä������ �б� ����
	};

	bool			bResult;
	char			errCode;

	std::vector<KEVENT_X2_CHANNEL_INFO>	channelInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_GET_DUNGEON_CHANNEL_INFO_LIST_ACK, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		channelInfoList.clear();
		for( int i = 0; i < (int)right.channelInfoList.size(); i++ )
		{
			channelInfoList.push_back( right.channelInfoList[i] );
		}

		return *this;
	}
};

//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_JOIN_DUNGEON_CHANNEL_REQ )
{
	bool			bInitChannel;
	char		channelID;
};

_DECL_DATA( KEVENT_X2_SERVER_JOIN_DUNGEON_CHANNEL_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_ENTER_CHANNEL_FAIL,		// ä������ ����
	};

	bool			bResult;
	char			errCode;
	char			channelID;
};


//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_DUNGEON_LOBBY_ACK )
{
	bool			bResult;
	char			errCode;
};

//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_REQ )
{
	short			pageNum;
};

DECL_DATA( KEVENT_X2_SERVER_DUNGEON_ROOM_SIMPLE_INFO )
{
	int				roomNum;
	std::wstring	roomName;

	char			roomState;
	char			difficultyLevel;
	short			dungeonID;

	char			maxSlot;
	char			joinSlot;

	bool			bIsOpen;			// ����/��� Ȯ�� ����

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_DUNGEON_ROOM_SIMPLE_INFO, right )
	{
		roomNum		= right.roomNum;
		roomName	= right.roomName;

		roomState	= right.roomState;
		difficultyLevel = right.difficultyLevel;
		dungeonID = right.dungeonID;

		maxSlot		= right.maxSlot;
		joinSlot	= right.joinSlot;

		bIsOpen		= right.bIsOpen;

		return *this;
	}
};

DECL_DATA( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_ROOMLIST_FAIL,			// �������� ����(�־�� �ȵɿ���)
	};

	bool			bResult;
	char			errCode;

	int				maxPageNum;
	std::vector<KEVENT_X2_SERVER_DUNGEON_ROOM_SIMPLE_INFO> roomSimpleInfoList;	

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		maxPageNum		= right.maxPageNum;

		roomSimpleInfoList.clear();
		for( int i = 0; i < (int)right.roomSimpleInfoList.size(); i++ )
		{
			roomSimpleInfoList.push_back( right.roomSimpleInfoList[i] );
		}

		return *this;
	}
};

DECL_DATA( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_ROOM_LIST_FAIL,	// ������ ���� ������ ����(�̿����� �־�� �ȵȴ�.)
	};

	bool			bResult;
	char			errCode;

	int				maxPageNum;
	std::vector<KEVENT_X2_SERVER_DUNGEON_ROOM_SIMPLE_INFO> roomSimpleInfoList;	

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_NOT, right )
	{
		bResult			= right.bResult;
		errCode			= right.errCode;

		maxPageNum		= right.maxPageNum;
		roomSimpleInfoList.clear();
		for( int i = 0; i < (int)right.roomSimpleInfoList.size(); i++ )
		{
			roomSimpleInfoList.push_back( right.roomSimpleInfoList[i] );
		}

		return *this;
	}
};




//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_DUNGEON_ROOM_INFO )
{
	int				roomNum;
	std::wstring	roomName;

	char			hostSlot;

	char			roomState;
	char			difficultyLevel;
	short			dungeonID;

	bool			bIsOpen;			// ����/��� Ȯ�� ����
	bool			bViewComics;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_DUNGEON_ROOM_INFO, data )
	{
		roomNum			= data.roomNum;
		roomName		= data.roomName;

		hostSlot		= data.hostSlot;

		roomState		= data.roomState;
		difficultyLevel = data.difficultyLevel;
		dungeonID		= data.dungeonID;

		bIsOpen			= data.bIsOpen;
		bViewComics		= data.bViewComics;

		return *this;
	}
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_REQ )
{
	std::wstring			roomName;
//	char					gameMode;

	char					difficultyLevel;
	short					dungeonID;

	char					unitNum;

	bool					bIsOpen;
	std::wstring			RoomPasswd;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_REQ, data )
	{
		roomName		= data.roomName;
//		gameMode		= data.gameMode;

		difficultyLevel = data.difficultyLevel;
		dungeonID		= data.dungeonID;

		unitNum			= data.unitNum;

		bIsOpen			= data.bIsOpen;
		RoomPasswd		= data.RoomPasswd;

		return *this;
	}
};

DECL_DATA( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_CREATE_ROOM_FAIL,		// ROOM ���� ����
		EC_ENTER_ROOM_FAIL,			// ������ ���� ����
	};

	bool			bResult;
	char			errCode;

	KEVENT_X2_SERVER_DUNGEON_ROOM_INFO		roomInfo;
	std::vector<KEVENT_X2_SERVER_SLOT_INFO>	slotInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_ACK, data )
	{
		bResult		= data.bResult;
		errCode		= data.errCode;

		roomInfo	= data.roomInfo;

		slotInfoList.clear();
		for( int i = 0; i < (int)data.slotInfoList.size(); i++ )
		{
			slotInfoList.push_back( data.slotInfoList[i] );
		}

		return *this;
	}
};



//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_REQ )
{
	int				roomNum;

	std::wstring	RoomPasswd;
};

DECL_DATA( KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_LOST_ROOM,				// ������ ���� ����
		EC_CLOSE_ROOM,				// ���� ������.
		EC_ENTER_ROOM_FAIL,			// �ο��� Ǯ�̰ų� ������
		EC_HOSTUSER_FAIL,			// ������ ����.(������ ���� ���ӽ���)
		EC_PASSWORD_FAIL,			// ����� Ʋ����.
		EC_UNKNOWN,					// �˼����� ����
	};

	bool		bResult;
	char		errCode;

	KEVENT_X2_SERVER_DUNGEON_ROOM_INFO		roomInfo;
	std::vector<KEVENT_X2_SERVER_SLOT_INFO>	slotInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_ACK, data )
	{
		bResult		= data.bResult;
		errCode		= data.errCode;

		roomInfo	= data.roomInfo;

		slotInfoList.clear();
		for( int i = 0; i < (int)data.slotInfoList.size(); i++ )
		{
			slotInfoList.push_back( data.slotInfoList[i] );
		}

		return *this;
	}
};



//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_EXIT_DUNGEON_ROOM_ACK )
{
	bool		bResult;
	char		errCode;
};








//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_INFO_REQ )
{
	enum CHANGE_ROOMINF
	{
		CR_WINGAMENUM	= 0,
		CR_WINKILLNUM,
		CR_POSSIBLEINTRUDE,
		CR_WORLDID,
	};

	char			changeFlag;
	int				nChangeData;
	//bool			bPossibleIntrude;	// ���԰��ɿ���

	//KEVENT_X2_SERVER_DUNGEON_ROOM_INFO	roomInfo;
	//int							winGameNum;
	//int							winKillNum;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_INFO_REQ, data )
	{
		//changeRoomInf	= data.changeRoomInf;
		//roomInfo		= data.roomInfo;
		//winGameNum			= data.winGameNum;
		//winKillNum			= data.winKillNum;
		changeFlag			= data.changeFlag;
		nChangeData			= data.nChangeData;
		//bPossibleIntrude	= data.bPossibleIntrude;
	}
};

_DECL_DATA( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_INFO_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_NOT_HOSTUSER,			// ������ �ƴѵ� ������ ���� ��û�� ����..
		EC_FLAG_FAIL,				// ������ ������ �ĺ� �÷��װ� �߸��Ǿ���(CHANGE_ROOMINF)
	};

	bool		bResult;
	char		errCode;
};

//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_SLOT_INFO_REQ )
{
	enum SLOT_CHANGE_INF
	{
		SCI_NONE	= 0,
		SCI_GAME_READY,
		SCI_TEAM_CHANGE,
		SCI_SLOT_ONOFF,
		SCI_MOTION_CHANGE,
	};

	//note : Client ���� Server�� Slot������ �ٲ� ���� �÷���
	char			slotChangeFlag;

	char			slotIndex;				// ���� Open/Close �ɼ� ������ ����

	KEVENT_X2_SERVER_SLOT_INFO	slotInfo;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_SLOT_INFO_REQ, data )
	{
		slotChangeFlag	= data.slotChangeFlag;
		slotIndex		= data.slotIndex;

		slotInfo		= data.slotInfo;
	}
};

_DECL_DATA( KEVENT_X2_SERVER_CHANGE_DUNGEON_ROOM_SLOT_INFO_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_SLOTINFOR_SET_FAIL,		// �ش� USER�� SLOT ������ ����.
		EC_SLOT_MOVE_FAIL,			// �����̵�����..
	};

	bool		bResult;
	char		errCode;
};






//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_INFO_NOT )
{
	KEVENT_X2_SERVER_DUNGEON_ROOM_INFO	roomInfo;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_INFO_NOT, data )
	{
		roomInfo = data.roomInfo;
	}
};

DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_SLOT_INFO_LIST_NOT )
{
	std::vector<char>						slotIndex;
	std::vector<KEVENT_X2_SERVER_SLOT_INFO>	slotInfoList;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_SLOT_INFO_LIST_NOT, data )
	{
		slotIndex.clear();
		for( int i = 0; i < (int)data.slotIndex.size(); i++ )
		{
			slotIndex.push_back( data.slotIndex[i] );
		}

		slotInfoList.clear();
		for( int i = 0; i < (int)data.slotInfoList.size(); i++ )
		{
			slotInfoList.push_back( data.slotInfoList[i] );
		}
	}
};






//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_START_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,				// ������Ʈ(FSM) ����
		EC_ROOMFSM_FAIL,			// ������� FSM���°� �߸��Ǿ���.
		EC_GAME_START_FAIL,			// ������Ŷ�� �� ����ڰ� ������ �ƴ�
		EC_NOT_READY,				// ������ ������ ���������� READY�� �Ǿ����� ����.
		EC_UNKNOWN,					// �˼����� ����
	};

	bool		bResult;
	char		errCode;
};


_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_START_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
	};

	bool		bResult;
	char		errCode;
};




//////////////////////////////////////////////////////////////
DECL_DATA( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_HOST_READY_REQ )
{
	short		port;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_HOST_READY_REQ, data )
	{
		port = data.port;
	}
};

_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_HOST_READY_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
	};

	bool		bResult;
	char		errCode;
};

DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_HOST_READY_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
	};

	bool			bResult;
	char			errCode;

	std::wstring	IP;
	short			port;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_HOST_READY_NOT, data )
	{
		bResult		= data.bResult;
		errCode		= data.errCode;
		IP			= data.IP;
		port		= data.port;
	}
};




//////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_ROOM_GAME_LOADING_COMPLETE_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
		EC_UNKNOWN,				// �˼����� ����
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_ALL_UNIT_LOADING_COMPLETE_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
	};

	bool		bResult;
	char		errCode;
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_GAME_ONE_UNIT_LOADING_COMPLETE_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
	};

	bool		bResult;
	char		errCode;
	char		slotIndex;
};


//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_GAME_RESULT_REQ )
{
	char	winTeam;
};

_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_GAME_RESULT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
		EC_NOT_HOSTUSER,
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_GAME_RESULT_NOT )
{
	char	winTeam;
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_STATE_CHANGE_DUNGEON_GAME_RESULT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// ROOM ������Ʈ(FSM) ����
		EC_NOT_HOSTUSER,
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_STATE_CHANGE_DUNGEON_GAME_RESULT_NOT )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
	};

	bool		bResult;
	char		errCode;

	KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_SLOT_INFO_LIST_NOT slotList;
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_EXIT_DUNGEON_GAME_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_STATE_CHANGE_FAIL,	// ���� ������ ���� ������Ʈ ���濡��
	};

	bool		bResult;
	char		errCode;
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_ROOM_CHAT_REQ )
{
	enum CHAT_EVENT
	{
		CE_NONE		= 0,
		CE_SECRET,
	};

	char			MsgType;
	char			mySlotIndex;
	std::wstring	strMsg;

	char			toSlotIndex;
};

_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_ROOM_CHAT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// �� ������Ʈ(FSM) ����
		EC_CHAT_TYPE_FAIL,		// ä�� �޼��� Ÿ���� �߸��Ǿ���.
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_CHAT_NOT )
{
	char			slotIndex;
	std::wstring	strMsg;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_CHAT_NOT, data )
	{
		slotIndex		= data.slotIndex;
		strMsg			= data.strMsg;
	}
};

//////////////////////////////////////////////////////////////////////////
_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_GAME_CHAT_REQ )
{
	enum CHAT_EVENT
	{
		CE_NONE		= 0,
		CE_SECRET,
	};

	char			MsgType;
	char			mySlotIndex;
	std::wstring	strMsg;

	char			toSlotIndex;
};

_DECL_DATA( KEVENT_X2_SERVER_DUNGEON_GAME_CHAT_ACK )
{
	enum ERROR_CODE
	{
		EC_NO_ERROR			= 0,
		EC_STATE_FAIL,			// ������Ʈ(FSM) ����
		EC_ROOM_STATE_FAIL,		// �� ������Ʈ(FSM) ����
		EC_CHAT_TYPE_FAIL,		// ä�� �޼��� Ÿ���� �߸��Ǿ���.
	};

	bool		bResult;
	char		errCode;
};

_DECL_DATA( KEVENT_X2_SERVER_BROADCAST_DUNGEON_GAME_CHAT_NOT )
{
	char			slotIndex;
	std::wstring	strMsg;

	CON_COPYCON_ASSIGNOP( KEVENT_X2_SERVER_BROADCAST_DUNGEON_GAME_CHAT_NOT, data )
	{
		slotIndex		= data.slotIndex;
		strMsg			= data.strMsg;
	}
};








#pragma pack( pop )
