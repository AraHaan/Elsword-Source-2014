#pragma once

//{{ 2012. 02. 03	�ڼ���	�̺�Ʈ �������� ó����� ���� ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB

//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
enum EVENT_REWARD_ID
{
	ERI_SECOND_SECURITY = 10135,       // 2�� ���� �ý��� ���� ����

	//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	ERI_COME_BACK_USER_TITLE				= 20150,	// Ÿ��ƲID  - �ٽ� ��� ��(�޸����� Īȣ)
	ERI_COME_BACK_USER_ITEM_CUBE_ONE		= 10143,	// reward
	ERI_COME_BACK_USER_ITEM_CUBE_TWO		= 10144,	// reward
	ERI_COME_BACK_USER_ITEM_CUBE_THREE		= 10145,	// reward
#endif SERV_COME_BACK_USER_REWARD
	//}} 
};
#endif SERV_SECOND_SECURITY
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
