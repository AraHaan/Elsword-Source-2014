#pragma once

#include "XSLUnit.h"

class CXSLRoom
{
	public:
		enum ROOM_TYPE
		{
			//{{ 2012. 04. 20	�ڼ���	����� �� ����
//#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
			RT_NONE				= -1,	// UserBuffManager���� RoomType�� ������ �ʰ� ��� ������ ������ �� ����ϰ� ����.
//#endif SERV_REFORM_THE_GATE_OF_DARKNESS
			//}}
			RT_PVP				= 0,
			RT_DUNGEON			= 1,
			RT_SQUARE			= 2,
			RT_TRADE			= 3,
			RT_TRAININGCENTER	= 4,
			RT_PERSONAL_SHOP	= 5,
			//{{ 2011. 04. 06	������	�븮 ����
//#ifdef SERV_PSHOP_AGENCY
			RT_PSHOP_AGENCY		= 6,
//#endif SERV_PSHOP_AGENCY
			//}}
			//{{ 2011. 10. 27	������	��Ʋ�ʵ� �ý���
//#ifdef SERV_BATTLE_FIELD_SYSTEM
			RT_BATTLE_FIELD		= 7,
//#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			//{{ 2013. 04. 01	 �ο� �ý��� - ��μ�
//#ifdef SERV_RELATIONSHIP_SYSTEM
			RT_WEDDING_HALL		= 8,
//#endif SERV_RELATIONSHIP_SYSTEM
			//}

            RT_TOTAL_NUM,
		};

		enum ROOM_STATE
		{
			RS_INIT		= 1,
			RS_CLOSED,
			RS_WAIT,
			RS_LOADING,
			RS_PLAY,
		};

		enum PVP_ROOM_STATE
		{
			PRS_WAIT = 0,
			PRS_PLAY,
		};

		enum PVP_GAME_TYPE
		{
			PGT_NULL = -1,
			PGT_TEAM = 0,
			PGT_TEAM_DEATH,
			PGT_SURVIVAL,
			PGT_TAG,
			PGT_END,
		};

		enum TEAM_NUM
		{
			TN_NONE		= -1,
			TN_RED		=  0,
			TN_BLUE		=  1,
			TN_MONSTER	=  2,
		};

		enum DUNGEON_GET_ITEM_TYPE
		{
			DGIT_NONE	= 0,
			DGIT_RANDOM,
			DGIT_PERSON,
			DGIT_END,
		};

		struct PVPRoomSimpleData
		{
			int					roomNum;
			std::wstring		roomName;

			PVP_ROOM_STATE		roomState;
			PVP_GAME_TYPE		gameType;
			int					worldID;

			int					maxSlot;
			int					joinSlot;
		};

		enum SLOT_STATE
		{
			SS_EMPTY	= 1,
			SS_CLOSE,
			SS_WAIT,
			SS_LOADING,
			SS_PLAY,
		};

		enum SLOT_EVENT
		{
			SE_NO_CHANGE = 0,//���� ���� ���� ����.
			SE_CHANGE,		 //���� ������ �����. 
			SE_NEW_JOIN,	 //���ο� ������ ������.
			SE_EXIT_ROOM,	 //������ �濡�� ����. 
			SE_EXIT_GAME,	//������ ���ӿ��� ���� ������ ��.
			SE_DISCONNECT,	 //���� ������ �����. 
			SE_OPEN_CLOSE,	 //������ ���ų� ����. 
			SE_READY,		 //�غ� �ϰų� ����.
			SE_TEAM_CHANGE,	 //�� �ٲ�.
			SE_MOTION_CHANGE, //��� �ٲ�.
		}; 

		enum SLOT_UNIT_SPECIAL_MOTION
		{
			SUSM_NONE = 0,
			SUSM_WAIT1,
			SUSM_WAIT2,
			SUSM_END,
		};

		enum ROOM_CHAT_TYPE
		{
			RCT_ALL = 0,
			RCT_TEAM,
			RCT_WHISPER,
		};

		//{{ 2009. 7. 21  ������	��н�������
		enum ENTER_SECRET_STAGE
		{
			ESS_NONE = 0,
			ESS_NOT_AGREE,
			ESS_AGREE,
		};
		//}}

		struct PVPSlotData
		{
			SLOT_STATE		slotState;
			int				teamNum;
			CXSLUnit*		pUnit;
		};

		struct PVPRoomData
		{
			int						roomNum;
			std::wstring			roomName;

			PVP_ROOM_STATE			roomState;
			PVP_GAME_TYPE			gameType;

			int						winGameNum;
			int						winKillNum;

			int						worldID;
			std::vector<PVPSlotData>	slotDataList;
		};

	public:
		CXSLRoom(void);
		~CXSLRoom(void);

	static ROOM_TYPE GetRoomType( UidType iRoomUID ) { return static_cast<ROOM_TYPE>(KncUid::ExtractReservedID( iRoomUID )); }
	//{{ 2009. 10. 22  ������	������ ���׼���	
	static bool IsValidWinKillNum( char cPvpGameType, char cWinKillNum );
	static bool IsValidPvpPlayTime( char cPvpGameType, int iPlayTime );
	//}}
	//{{ 2011. 06. 20	������	���� ����
#ifdef SERV_PVP_NEW_SYSTEM
	static bool IsValidPvpGameType( const PVP_GAME_TYPE ePvpGameType );
#endif SERV_PVP_NEW_SYSTEM
	//}}
};



