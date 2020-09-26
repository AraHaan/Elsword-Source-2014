#pragma once

#include "XSLStat.h"

class CXSLUser
{
	public:
		struct UserData
		{
			int					userUID;			//������ ����ũ ���̵�
			std::wstring		userID;				//���̵�
			std::wstring		userNickName;		//�г���
			std::wstring		userName;			//�Ǹ�

			int			maxUnitCount;		//���� ���� �� �ִ� ĳ���� ��(������ �ִ°� ����)
			int			cash;				//����ĳ��

			UserData()
			{
				userUID			= 0;
				maxUnitCount	= 0;
				cash			= 0;
			}

			UserData( const UserData& data ){ *this = data; }

			UserData& operator=( const UserData& data )
			{
				userUID			= data.userUID;				//������ ����ũ ���̵�
				userID			= data.userID;				//���̵�
				userNickName	= data.userNickName;		//�г���
				userName		= data.userName;

				maxUnitCount	= data.maxUnitCount;		//���� ���� �� �ִ� ĳ���� ��(������ �ִ°� ����)
				cash			= data.cash;

				return *this;
			}

			//UserData& operator=( const KDB_EVENT_X2_SERVER_READ_USER_DATA_ACK& data )
			//{
			//	userUID			= data.userInfor.userUID;			//������ ����ũ ���̵�
			//	userID			= data.userInfor.userID;			//���̵�
			//	userNickName	= data.userInfor.userNickName;		//�г���
			//	userName		= data.userName;					//�Ǹ�

			//	maxUnitCount	= data.userInfor.maxUnitCount;		//���� ���� �� �ִ� ĳ���� ��(������ �ִ°� ����)
			//	cash			= data.userInfor.cash;

			//	return *this;
			//}
		};

	public:
		CXSLUser(void);
		~CXSLUser(void);
};
