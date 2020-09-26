#include "StdAfx.h"
#include ".\x2rewardtable.h"



CX2RewardTable::CX2RewardTable(void)
{	
}

CX2RewardTable::~CX2RewardTable(void)
{
}

bool CX2RewardTable::OpenScriptFile( const wchar_t* pFileName )
{
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	return true;
#else //SERV_EVENT_DB_CONTROL_SYSTEM
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pKRewardTable", this );

	return g_pKTDXApp->LoadLuaTinker( pFileName );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
}

void CX2RewardTable::GetRewardItem( IN const KPostItemInfo& kPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const
{
	// ������ ���� �޽����� ��� ���
	switch( kPostItemInfo.m_cScriptType )
	{
	case KPostItemInfo::LT_POST_OFFICE:
	case KPostItemInfo::LT_MESSAGE:
	case KPostItemInfo::LT_BROKEN_ITEM:
	case KPostItemInfo::LT_WEB_POINT_EVENT:
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	case KPostItemInfo::LT_QUEST_REWARD:
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
#ifdef SERV_INDEMNIFICATION_POST_DB_ONLY
	case KPostItemInfo::LT_INDEMNIFICATION_DB_ONLY:
#endif //SERV_INDEMNIFICATION_POST_DB_ONLY
		return;

	default:
		break;
	}

	std::map< int, std::vector< KRewardData > >::const_iterator mit;
	mit = m_mapRewardData.find( kPostItemInfo.m_iScriptIndex );
	if( mit == m_mapRewardData.end() )
	{
		//START_LOG( cerr, L"�������� �ʴ� RewardID�Դϴ�." )
		//	<< BUILD_LOG( kPostItemInfo.m_iScriptIndex )
		//	<< END_LOG;
		//ErrorLog( KEM_ERROR359 );
		return;
	}

	std::vector< KRewardData >::const_iterator vitReward;
	for( vitReward = mit->second.begin(); vitReward != mit->second.end(); ++vitReward )
	{		
		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( vitReward->m_iRewardItemID );
		if( pItemTemplet == NULL )
		{
			//START_LOG( cerr, L"������ ���ø� ������ ã�� �� �����ϴ�." )
			//	<< BUILD_LOG( vitReward->m_iRewardItemID )
			//	<< END_LOG;
			//ErrorLog( KEM_ERROR359 );
			continue;
		}

		// ���� ������
		KItemInfo kRewardItemInfo;
		kRewardItemInfo.m_iItemID		= vitReward->m_iRewardItemID;
		kRewardItemInfo.m_iQuantity		= vitReward->m_iQuantity;
		kRewardItemInfo.m_cUsageType	= pItemTemplet->GetPeriodType();
		kRewardItemInfo.m_sEndurance	= pItemTemplet->GetEndurance();

		// �Ⱓ�� �������� ��� �Ⱓ ����
		if( pItemTemplet->GetPeriodType() == CX2Item::PT_INFINITY )
		{
			kRewardItemInfo.m_sPeriod	= vitReward->m_sPeriod;
		}

		// ������ ItemID�� ���� �ÿ��� ������ �÷��ش�.
		std::map< int, KItemInfo >::iterator mitFindSame;
		mitFindSame = mapRewardItem.find( kRewardItemInfo.m_iItemID );
		if( mitFindSame == mapRewardItem.end() )
		{
			mapRewardItem.insert( std::make_pair( kRewardItemInfo.m_iItemID, kRewardItemInfo ) );
		}
		else
		{
			mitFindSame->second.m_iQuantity += kRewardItemInfo.m_iQuantity;
		}
	}
}

void CX2RewardTable::GetRewardItem( IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, KItemInfo >& mapRewardItem ) const
{
	mapRewardItem.clear();

	std::vector< KPostItemInfo >::const_iterator vit;
	for( vit = vecPostItemInfo.begin(); vit != vecPostItemInfo.end(); ++vit )
	{
		GetRewardItem( *vit, mapRewardItem );
	}
}

bool CX2RewardTable::GetRewardInfo( IN int iRewardID, OUT std::map< int, int >& mapRewardInfo ) const
{
	std::map< int, std::vector< KRewardData > >::const_iterator mit;
	mit = m_mapRewardData.find( iRewardID );
	if( mit == m_mapRewardData.end() )
	{
		//ErrorLog( KEM_ERROR359 );
		return false;
	}

	std::vector< KRewardData >::const_iterator vitReward;
	for( vitReward = mit->second.begin(); vitReward != mit->second.end(); ++vitReward )
	{
		std::map< int, int >::iterator mitFindSame;
		mitFindSame = mapRewardInfo.find( vitReward->m_iRewardItemID );
		if( mitFindSame == mapRewardInfo.end() )
		{
			mapRewardInfo.insert( std::make_pair( vitReward->m_iRewardItemID, vitReward->m_iQuantity ) );
		}
		else
		{
			mitFindSame->second += vitReward->m_iQuantity;
		}
	}

	return true;
}

void CX2RewardTable::GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::map< int, int >& mapRewardInfo ) const
{
	mapRewardInfo.clear();

	std::vector< KPostItemInfo >::const_iterator vit;
	for( vit = vecPostItemInfo.begin(); vit != vecPostItemInfo.end(); ++vit )
	{
		if( vit->m_cScriptType != static_cast<char>(iRewardType) )
			continue;

		if( GetRewardInfo( vit->m_iScriptIndex, mapRewardInfo ) == false )
			continue;
	}
}

void CX2RewardTable::GetRewardInfo( IN int iRewardType, IN const std::vector< KPostItemInfo >& vecPostItemInfo, OUT std::vector< std::wstring >& vecMessage ) const
{
	vecMessage.clear();

	std::vector< KPostItemInfo >::const_iterator vit;
	for( vit = vecPostItemInfo.begin(); vit != vecPostItemInfo.end(); ++vit )
	{
		if( vit->m_cScriptType != static_cast<char>(iRewardType) )
			continue;

		vecMessage.push_back( vit->m_wstrMessage );
	}
}

void CX2RewardTable::MakeSystemLetterTitle( const std::wstring& wstrNickName, KPostItemTitleInfo& kPostTitle )
{
	switch( kPostTitle.m_cScriptType )
	{
	case KPostItemInfo::LT_RECOMMEND:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_3693 );
		}
		break;

	case KPostItemInfo::LT_TUTOL:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_3694 );
		}
		break;

	case KPostItemInfo::LT_EVENT:
		{

			// ���� index�� ���� ó��
			switch( kPostTitle.m_iScriptIndex )
			{
			case 60: // 300�� �̺�Ʈ - iRewardID
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle    = GET_STRING( STR_ID_3695 );
				}
				break;

			case 74:
				{
					kPostTitle.m_wstrFromNickName.clear();
				}
				break;

			case 85:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle    = GET_STRING( STR_ID_3696 );
				}
				break;

			case 136: // �ں� Ư�� ���� ť��
				{
					kPostTitle.m_wstrFromNickName = GET_STRING( STR_ID_370 );
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_3697 );
				}
				break;

				//{{ 2010. 04. 28  ������	õ���� �߸�
			case 10019:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle    = GET_STRING( STR_ID_5081 );
				}
				break;
				//}}

				//{{ 2011. 06. 01	������	�̺�Ʈ ����
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
			case 10126:
			case 10127:
			case 10128:
			case 10129:
			case 10130:
			case 10131:
			case 10132:
			case 10133:
			case 10134:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_12340 );
				}
				break;
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
				//}}

				//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
#ifdef CLIENT_COUNTRY_JP
			case 650:		// �ؿ��� �Ϻ� �߰�
#endif
			case 10135:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_12548 );
				}break;
#endif SERV_SECOND_SECURITY
				//}}

				//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
			case 10143:
			case 10144:
			case 10145:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_12583 );
				}break;
#endif SERV_COME_BACK_USER_REWARD
				//}} 
				//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
			case 10198:		// ���� ������ ������ Ǯ ��Ʈ(15�ϱ�) ť��
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_12794 );
				}break;
			case 10196:		// �ѿ��� ��� ť��
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_12792 );
				}break;
			case 10273:		// �Ѱܿ� ��� ť��
			case 10282:		// �Ѱܿ� �մ�� ť��
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_15329 );
				}break;
#endif SERV_NAVER_PROMOTIONS
				//}} 

				//{{ 2011. 11. 26  ���� ���� �ȴ� �� : ����Ʈ ���� ������ ����Ʈ
#ifdef SERV_SWORD_WAY_QUEST_COMPLETE_REWARD
			case 10258: // �ҵ� ������ ��ǥ�� ������ ������ �Ƹ��� 7 ���� ���� �̺�Ʈ
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_14812 );
				}break;
#endif SERV_SWORD_WAY_QUEST_COMPLETE_REWARD
				//}}
			case 10307: // ����¡ ����� ť��
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_16258 );
				}break;
			case 10308: // ������ ����
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_16259 );
				}break;

				//{{ 2012. 04. 05	�ڼ���	�ְ��� ���ݼ��� ��ǰ
#ifdef SERV_EVENT_THE_BEST_ALCHEMIST_WORK
			case 10333: // �̴� ���� Ÿ��Ʈ 30��
				kPostTitle.m_wstrFromNickName.clear();
				kPostTitle.m_wstrTitle = GET_STRING( STR_ID_16423 );
				break;
#endif SERV_EVENT_THE_BEST_ALCHEMIST_WORK
				//}}

				//{{ 2012. 05. 22	�ڼ���	[�̺�Ʈ] ������ũ�� �����մϴ�!
//#ifdef SERV_EVENT_APINK
			case 10341: // +6 ������ũ�� Ǯ��Ʈ ť��
			case 10342:
			case 10343:
			case 10344:
			case 10345:
			case 10346:
			case 10347:
			case 10348:
			case 10349:
			case 10350:
			case 10351:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17383 );
				}break;
//#endif SERV_EVENT_APINK
				//}}

				//{{ 2012. 06. 12	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
			case 10359:	// �縮�� ť�� �����
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17581 );
				}break;
			case 10360:	// �縮�� ť�� �ݿ���
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17582 );
				}break;
			case 10361:	// �縮�� ť�� �����
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17583 );
				}break;
			case 10362:	// �縮�� ť�� �Ͽ���
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17584 );
				}break;
			case 10363:	// �縮�� ť�� ������
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17578 );
				}break;
			case 10364:	// �縮�� ť�� ȭ����
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17579 );
				}break;
			case 10365:	// �縮�� ť�� ������
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17580 );
				}break;
			case 10366:	// �Ƹ��� ť�� �����
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17567 );
				}break;
			case 10367:	// �Ƹ��� ť�� �ݿ���
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17568 );
				}break;
			case 10368:	// �Ƹ��� ť�� �����
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17569 );
				}break;
			case 10369:	// �Ƹ��� ť�� �Ͽ���
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17570 );
				}break;
			case 10370:	// �Ƹ��� ť�� ������
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17564 );
				}break;
			case 10371:	// �Ƹ��� ť�� ȭ����
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17565 );
				}break;
			case 10372:	// �Ƹ��� ť�� ������
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17566 );
				}break;
#endif SERV_EVENT_DAILY_GIFT_BOX
				//}}
//{{ ���� �ø��� ��� ����       ��μ� 2012-07-23
			case 10393:	// ������ ����
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_18056 );
				}break;
//}} ���� �ø��� ��� ����

				//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ ��ȯ �α�
//#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
			case 10396:	// ��õ�� ���� �̴Ϲ�(��)
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_18272 );
				}break;
//#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
				//}}
#ifdef SERV_PRESENT_SKILL_INIT_ITEM// �۾���¥: 2013-06-25	// �ڼ���
#ifdef SERV_PRESENT_SKILL_INIT_ITEM_INT
			case _CONST_PRESENT_SKILL_INIT_ITEM_INT_::iReachLimitLevel:
#else //SERV_PRESENT_SKILL_INIT_ITEM_INT
			case 10553:
#endif //SERV_PRESENT_SKILL_INIT_ITEM_INT
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_25284 );
				}break;
#endif // SERV_PRESENT_SKILL_INIT_ITEM


#ifdef SERV_PRESENT_SKILL_INIT_ITEM
				//ĳ�÷� ���� �޼� �Ҷ� 
#ifdef SERV_PRESENT_SKILL_INIT_ITEM_INT
			case _CONST_PRESENT_SKILL_INIT_ITEM_INT_::iChangeFirstJob:
#else //SERV_PRESENT_SKILL_INIT_ITEM_INT
			case 10554:
#endif //SERV_PRESENT_SKILL_INIT_ITEM_INT
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_25282 ); // 1�� ���� �޼� ���� ����!
				}break;

#ifdef SERV_PRESENT_SKILL_INIT_ITEM_INT
			case _CONST_PRESENT_SKILL_INIT_ITEM_INT_::iChangeSecondJob:
#else //SERV_PRESENT_SKILL_INIT_ITEM_INT
			case 10555:
#endif //SERV_PRESENT_SKILL_INIT_ITEM_INT
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_25283 ); // 2�� ���� �޼� ���� ����!
				}break;
#endif //SERV_PRESENT_SKILL_INIT_ITEM

			case 10582:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_26338 ); // ������ ��� �˼��մϴ�.
				}break;

			case 10607:
			case 10608:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_28384 );
				}break;

			case 10614:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_29330 );
				}break;

			case 10640:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_29955 );
				}break;

			case 10643:
			case 10644:
			case 10645:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_30271 );
				} break;

			default:
				{
					kPostTitle.m_wstrFromNickName.clear();
					kPostTitle.m_wstrTitle = GET_STRING( STR_ID_3698 );
				}
				break;
			}
		}
		break;

	case KPostItemInfo::LT_RANKING_TITLE:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_3698 );
		}
		break;

	case KPostItemInfo::LT_MESSAGE:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_4923 );
		}
		break;

		//{{ 2010. 02. 24  ������	�� ����Ʈ �̺�Ʈ
	case KPostItemInfo::LT_WEB_POINT_EVENT:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_4921 );
		}
		break;
		//}}

	case KPostItemInfo::LT_BROKEN_ITEM:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_3699 );
		}
		break;

		//{{ 2009. 11. 2  ������	��尭����ü
	case KPostItemInfo::LT_GUILD_DISBAND:
		{
			// ��� ��ü
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_4724 );
		}
		break;

	case KPostItemInfo::LT_GUILD_KICK:
		{
			// ��� ����
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_4729 );
		}
		break;
		//}}

		//{{ 2010. 02. 01  ������	��� �Խ���
	case KPostItemInfo::LT_GUILD_JOIN:
		{
			// ��� ����
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_4908 );
		}
		break;
		//}}

		//{{ 2011. 05. 11	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	case KPostItemInfo::LT_STOP_SALE_PSHOP:
		{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// �� -> �Խ���
			kPostTitle.m_wstrFromNickName = GET_STRING( STR_ID_3723 );
#else // SERV_UPGRADE_TRADE_SYSTEM
			kPostTitle.m_wstrFromNickName = GET_STRING( STR_ID_12247 );
#endif // SERV_UPGRADE_TRADE_SYSTEM
			
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_12248 );
		}
		break;

	case KPostItemInfo::LT_SELL_ITEM_PSHOP:
		{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // ����ȯ
			/// �� -> �Խ���
			kPostTitle.m_wstrFromNickName = GET_STRING( STR_ID_3723 );
#else // SERV_UPGRADE_TRADE_SYSTEM
			kPostTitle.m_wstrFromNickName = GET_STRING( STR_ID_12247 );
#endif // SERV_UPGRADE_TRADE_SYSTEM
			
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_12250 );
		}
		break;
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	case KPostItemInfo::LT_COME_BACK_USER_NOTIFY:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_12579 );
		}
		break;
#endif SERV_COME_BACK_USER_REWARD
		//}}
		//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	case KPostItemInfo::LT_OUT_USER_RETANING_REWARD:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_13711 );
		}break;
#endif SERV_NEXON_SESSION_PROTOCOL
		//}} 
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	case KPostItemInfo::LT_QUEST_REWARD:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_17697 );
		} break;
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
		
		//{{ 2012. 12. 17	�ڼ���	���� �̺�Ʈ
#ifdef SERV_EVENT_JACKPOT
	case KPostItemInfo::LT_JACKPOT_EVENT_REWARD:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_21320 );
		} break;
#endif SERV_EVENT_JACKPOT
		//}}

#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM	 //��Ը� �̺�Ʈ ���� �� ���� ���� ĳ���Ϳ��� �����ϴ� �ý���
	case KPostItemInfo::LT_DB_REWARD:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_22431 );

		}break;
	case KPostItemInfo::LT_SYSTEM_ERROR_REWARD:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_22432 );

		}break;
#endif //SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM

#ifdef SERV_INDEMNIFICATION_POST_DB_ONLY
	case KPostItemInfo::LT_INDEMNIFICATION_DB_ONLY:
		{
			kPostTitle.m_wstrFromNickName.clear();
			kPostTitle.m_wstrTitle = GET_STRING( STR_ID_4923 );
		} break;
#endif //SERV_INDEMNIFICATION_POST_DB_ONLY


#ifdef SERV_RELATIONSHIP_SYSTEM
	case KPostItemInfo::LT_WEDDING_INVITATION :
		{
			kPostTitle.m_wstrTitle = GET_REPLACED_STRING( ( STR_ID_24661, "L", kPostTitle.m_wstrFromNickName ) );
		}
		break;
	case KPostItemInfo::LT_WEDDING_RESERVE :
		{
			kPostTitle.m_wstrTitle = GET_STRING ( STR_ID_24662 );
		}
		break;
	case KPostItemInfo::LT_WEDDING_REWARD :
		{
			switch( kPostTitle.m_iScriptIndex )
			{
			case 160765:	// �ǻ� ť��
				{
					if( NULL != g_pData->GetItemManager() )
					{
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kPostTitle.m_iScriptIndex );
						if( pItemTemplet != NULL )
						{
							kPostTitle.m_wstrTitle = pItemTemplet->GetName();
						}
					}
				}
				break;
			default:
				{
					kPostTitle.m_wstrTitle = GET_STRING ( STR_ID_24664 );
				}break;
			}
		}
		break;
	case KPostItemInfo::LT_BREAK_UP :
		{			
			kPostTitle.m_wstrTitle = GET_STRING ( STR_ID_24665 );
		}
		break;
#endif // SERV_RELATIONSHIP_SYSTEM

	default:
		break;
	}
}

void CX2RewardTable::MakeSystemLetter( const std::wstring& wstrNickName, KPostItemInfo& kPostItem )
{
	KPostItemTitleInfo kTitle;
	kTitle.m_iPostNo		= kPostItem.m_iPostNo;
	kTitle.m_cScriptType	= kPostItem.m_cScriptType;
	kTitle.m_iScriptIndex	= kPostItem.m_iScriptIndex;
	kTitle.m_cEnchantLevel	= kPostItem.m_cEnchantLevel;
	kTitle.m_wstrRegDate	= kPostItem.m_wstrRegDate;
	kTitle.m_wstrFromNickName = kPostItem.m_wstrFromNickName;
	kTitle.m_wstrTitle		= kPostItem.m_wstrTitle;
	kTitle.m_bRead			= kPostItem.m_bRead;
	MakeSystemLetterTitle( wstrNickName, kTitle );

	// �������, ����
	kPostItem.m_wstrFromNickName = kTitle.m_wstrFromNickName;
	kPostItem.m_wstrTitle = kTitle.m_wstrTitle;

	// ���� ����
	switch( kPostItem.m_cScriptType )
	{
	case KPostItemInfo::LT_RECOMMEND:
		{
			// �г��Ӱ� ���� ��Ʈ�� �и�
			int lastPos = kPostItem.m_wstrMessage.find( L";" );
			if( lastPos < 0 )
			{
				// ��Ʈ�������� ���� ���� ����
				break;
			}

			std::wstring wstrRecommenderNickName = kPostItem.m_wstrMessage.substr( 0, lastPos );
			std::wstring wstrLevel = kPostItem.m_wstrMessage.substr( lastPos + 1, kPostItem.m_wstrMessage.size() );

			// ȹ�� ������ ��Ʈ��
			std::wstring wstrGetItemList;
			AssignStringFromRewardTable( kPostItem.m_iScriptIndex, wstrGetItemList );

			// ���� ���� ����			
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_3701, "LLLL", wstrNickName, wstrRecommenderNickName, wstrLevel, wstrGetItemList ) );
		}
		break;

	case KPostItemInfo::LT_TUTOL:
		{
			// �г��Ӱ� ���� ��Ʈ�� �и�
			int lastPos = kPostItem.m_wstrMessage.find( L";" );
			if( lastPos < 0 )
			{
				// ��Ʈ�������� ���� ���� ����
				break;
			}

			std::wstring wstrTutoNickName = kPostItem.m_wstrMessage.substr( 0, lastPos );
			std::wstring wstrLevel = kPostItem.m_wstrMessage.substr( lastPos + 1, kPostItem.m_wstrMessage.size() );

			// ȹ�� ������ ��Ʈ��
			std::wstring wstrGetItemList;
			AssignStringFromRewardTable( kPostItem.m_iScriptIndex, wstrGetItemList );

			// ���� ���� ����			
			if( kPostItem.m_iScriptIndex == 6 )
			{
				// ��������
				kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_3702, "LLLL", wstrNickName, wstrTutoNickName, wstrLevel, wstrGetItemList ) );
			}
			else
			{
				// ����������
				kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_3703, "LLLL", wstrNickName, wstrTutoNickName, wstrLevel, wstrGetItemList ) );
			}
		}
		break;

	case KPostItemInfo::LT_EVENT:
		{
			switch( kPostItem.m_iScriptIndex )
			{
			case 60: // 300�� �̺�Ʈ
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_3704 );
					AssignStringFromRewardTable( kPostItem.m_iScriptIndex, kPostItem.m_wstrMessage );
				}
				break;

			case 85: // ����� �̺�Ʈ
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_3705 );
				}
				break;

			case 136: // ���� ���� �̺�Ʈ
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_3706 );
				}
				break;

				//{{ 2010. 04. 28  ������	õ���� �߸�
			case 10019:
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_5081 );
				}
				break;
				//}}

				//{{ 2011. 06. 01	������	�̺�Ʈ ����
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
			case 10126:
			case 10127:
			case 10128:
			case 10129:
			case 10130:
			case 10131:
			case 10132:
			case 10133:
			case 10134:
				{
					// ȹ�� ������ ��Ʈ��
					std::wstring wstrGetItemList;
					AssignStringFromRewardTable( kPostItem.m_iScriptIndex, wstrGetItemList, true );

					kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_12341, "L", wstrGetItemList ) );
				}
				break;
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
				//}}

				//{{ 2011. 05. 02  ��μ�	2�� ���� �ý���
#ifdef SERV_SECOND_SECURITY
			case 10135:
				{
					// ȹ�� ������ ��Ʈ��
					std::wstring wstrGetItemList;
					AssignStringFromRewardTable( kPostItem.m_iScriptIndex, wstrGetItemList, true );

					kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_12547, "L", wstrGetItemList ) );
				}
				break;
#endif SERV_SECOND_SECURITY
				//}}

				//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
			case 10143:
			case 10144:
			case 10145:
				{
					// ȹ�� ������ ��Ʈ��
					std::wstring wstrGetItemList;
					AssignStringFromRewardTable( kPostItem.m_iScriptIndex, wstrGetItemList );

					kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_12582, "L", wstrGetItemList ) );

				}break;
#endif SERV_COME_BACK_USER_REWARD
				//}} 
				//{{ 2011. 07. 04    ��μ�    ���̹� ���θ��
#ifdef SERV_NAVER_PROMOTIONS
			case 10198:         // ���� ������ ������ Ǯ ��Ʈ(15�ϱ�) ť��
			case 10196:		// �ѿ��� ��� ť��
			case 10273:		// �Ѱܿ� ��� ť��
			case 10282:		// �Ѱܿ� �մ�� ť��
				{
					// ȹ�� ������ ��Ʈ��
					std::wstring wstrGetItemList;
					AssignStringFromRewardTable( kPostItem.m_iScriptIndex, wstrGetItemList );

					kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_12793, "L", wstrGetItemList ) );

				}break;
#endif SERV_NAVER_PROMOTIONS
				//}} 

				//{{ 2011. 11. 26  ���� ���� �ȴ� �� : ����Ʈ ���� ������ ����Ʈ
#ifdef SERV_SWORD_WAY_QUEST_COMPLETE_REWARD
			case 10258: // �ҵ� ������ ��ǥ�� ������ ������ �Ƹ��� 7 ���� ���� �̺�Ʈ
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_14813 );
				}break;
#endif SERV_SWORD_WAY_QUEST_COMPLETE_REWARD
				//}}

				//{{ 2012. 05. 22	�ڼ���	[�̺�Ʈ] ������ũ�� �����մϴ�!
//#ifdef SERV_EVENT_APINK
			case 10341: // +6 ������ũ�� Ǯ��Ʈ ť��
			case 10342:
			case 10343:
			case 10344:
			case 10345:
			case 10346:
			case 10347:
			case 10348:
			case 10349:
			case 10350:
			case 10351:
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17384 );
				}break;
//#endif SERV_EVENT_APINK
				//}}

				//{{ 2012. 06. 12	�ڼ���	���ϸ��� ���� ����
#ifdef SERV_EVENT_DAILY_GIFT_BOX
			case 10359:	// �縮�� ť�� �����
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17588 );
				}break;
			case 10360:	// �縮�� ť�� �ݿ���
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17589 );
				}break;
			case 10361:	// �縮�� ť�� �����
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17590 );
				}break;
			case 10362:	// �縮�� ť�� �Ͽ���
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17591 );
				}break;
			case 10363:	// �縮�� ť�� ������
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17585 );
				}break;
			case 10364:	// �縮�� ť�� ȭ����
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17586 );
				}break;
			case 10365:	// �縮�� ť�� ������
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17587 );
				}break;
			case 10366:	// �Ƹ��� ť�� �����
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17574 );
				}break;
			case 10367:	// �Ƹ��� ť�� �ݿ���
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17575 );
				}break;
			case 10368:	// �Ƹ��� ť�� �����
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17576 );
				}break;
			case 10369:	// �Ƹ��� ť�� �Ͽ���
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17577 );
				}break;
			case 10370:	// �Ƹ��� ť�� ������
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17571 );
				}break;
			case 10371:	// �Ƹ��� ť�� ȭ����
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17572 );
				}break;
			case 10372:	// �Ƹ��� ť�� ������
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_17573 );
				}break;
#endif SERV_EVENT_DAILY_GIFT_BOX
				//}}
				//}}
//{{ ���� �ø��� ��� ����       ��μ� 2012-07-23
			case 10393:	// ������ ����
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_18057 );
				}break;
//}} ���� �ø��� ��� ����

				//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ ��ȯ �α�
//#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
			case 10396:	// ��õ�� ���� �̴Ϲ�(��)
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_18273 );
				}break;
//#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
				//}}
#ifndef SERV_INT_ENTRY_POINT_LEVEL_UP_EVENT
				//{{ 2012. 08. 14	�ڼ���	��õ���� ��ȭ ��ȯ �α�
			case 10405:	// ��ǳ �̽� '����� ����(1�ϱ�)'
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_18424 );
				}break;
				//}}
#endif SERV_INT_ENTRY_POINT_LEVEL_UP_EVENT
			case 10518:
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_23796 );
				}break;

#ifdef SERV_PRESENT_SKILL_INIT_ITEM// �۾���¥: 2013-06-25	// �ڼ���
#ifdef SERV_PRESENT_SKILL_INIT_ITEM_INT
			case _CONST_PRESENT_SKILL_INIT_ITEM_INT_::iReachLimitLevel:
#else //SERV_PRESENT_SKILL_INIT_ITEM_INT
			case 10553:
#endif //SERV_PRESENT_SKILL_INIT_ITEM_INT
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_25281 );
				}break;

				//ĳ�÷� ���� �޼� �Ҷ� 
#ifdef SERV_PRESENT_SKILL_INIT_ITEM_INT
			case _CONST_PRESENT_SKILL_INIT_ITEM_INT_::iChangeFirstJob:
			case _CONST_PRESENT_SKILL_INIT_ITEM_INT_::iChangeSecondJob:
#else //SERV_PRESENT_SKILL_INIT_ITEM_INT
			case 10554:
			case 10555:
#endif //SERV_PRESENT_SKILL_INIT_ITEM_INT
				{
					if( NULL != g_pData && NULL != g_pData->GetItemManager() )
					{				
						int ItemID = static_cast<int>( _wtoi( kPostItem.m_wstrMessage.c_str() ) );
						const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( ItemID );
						if( pItemTemplet != NULL )
						{
							// [@1]��������\n�������ּż� �����մϴ�.\n���� ������ ������\n���� ���θ�� ����\n��ų ��ü �ʱ�ȭ �������� �帳�ϴ�.
							kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_25280, "S", pItemTemplet->GetFullName_() ) );
						}
					}
				}break;
#endif //SERV_PRESENT_SKILL_INIT_ITEM

			case 10582:
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_26339 );	// 7/21 ����������� ������ ��� �˼��մϴ�.\n����� ���� ��� ����� ����� ť�ꡱ �� ���� �帳�ϴ�!\n���� �Ϸ� ��������!
				}break;

			case 10607:
			case 10608:
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_28385 );
				}break;

			case 10614:
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_29331 );
				}break;

			case 10640:
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_29956 );
				}break;

			case 10643:
			case 10644:
			case 10645:
				{
					kPostItem.m_wstrMessage = GET_STRING( STR_ID_30272 );
				} break;

			default: // �⺻ �̺�Ʈ
				{
					// ȹ�� ������ ��Ʈ��
					std::wstring wstrGetItemList;
					AssignStringFromRewardTable( kPostItem.m_iScriptIndex, wstrGetItemList );

					kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_3707, "L", wstrGetItemList ) );
				}
				break;
			}
		}
		break;

	case KPostItemInfo::LT_RANKING_TITLE:
		{
			// kPostItem.m_wstrMessage -> Īȣ
			//{{ 2011.11.20 ��ȫ��	���� ����ȭ // SERV_USE_NATION_FLAG �ʼ� ������
#ifdef SERV_UNITED_SERVER_EU
			int iTitleID = _wtoi(kPostItem.m_wstrMessage.c_str());
			const CX2TitleManager::TitleInfo* pTitleInfo = g_pData->GetTitleManager()->GetTitleInfo( iTitleID );
			if( pTitleInfo == NULL )
			{
				START_LOG( cerr, L"�������� �ʴ� Īȣ�Դϴ�!" )
					<< BUILD_LOG( iTitleID )
					<< END_LOG;
			}
			else
			{
				kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_3708, "L", pTitleInfo->m_wstrTitleName ) );
			}			
#else SERV_UNITED_SERVER_EU
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_3708, "L", kPostItem.m_wstrMessage ) );
#endif SERV_UNITED_SERVER_EU
			//}}
		}
		break;
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM	
		case KPostItemInfo::LT_DB_REWARD:
		{
		std::wstring wstrItemName;
		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kPostItem.m_iScriptIndex );
		if( pItemTemplet != NULL )
		{
		wstrItemName = pItemTemplet->GetName();
		}

		kPostItem.m_wstrMessage.clear();
		kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_22415, "L", wstrItemName ) );

		}break;
		case KPostItemInfo::LT_SYSTEM_ERROR_REWARD:
		{
		std::wstring wstrItemName;
		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( kPostItem.m_iScriptIndex );
		if( pItemTemplet != NULL )
		{
		wstrItemName = pItemTemplet->GetName();
		}

		kPostItem.m_wstrMessage.clear();
		kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_22416, "L", wstrItemName ) );

		}break;
#endif //SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM

	case KPostItemInfo::LT_MESSAGE:
		{
			//{{ 2009. 9. 23  ������	���� ���θ�� ť��
			switch( kPostItem.m_iScriptIndex )
			{
			case 109999: // 1�� ���� ���θ�� ť��
			case 110850: // 2�� ���� ���θ�� ť��
				{
					std::wstring wstrItemName;
					const int iItemID = _wtoi( kPostItem.m_wstrMessage.c_str() );
					const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );
					if( pItemTemplet != NULL )
					{
						wstrItemName = pItemTemplet->GetName();
					}

					kPostItem.m_wstrMessage.clear();
					kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_4517, "L", wstrItemName ) );
				}
				break;
			}
			//}}
		}
		break;

		//{{ 2010. 02. 24  ������	�� ����Ʈ �̺�Ʈ
	case KPostItemInfo::LT_WEB_POINT_EVENT:
		{
			std::wstring wstrItemName;
			const int iItemID = _wtoi( kPostItem.m_wstrMessage.c_str() );
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );
			if( pItemTemplet != NULL )
			{
				wstrItemName = pItemTemplet->GetName();
			}

			kPostItem.m_wstrMessage.clear();
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_4922, "L", wstrItemName ) );
		}
		break;
		//}}

	case KPostItemInfo::LT_BROKEN_ITEM:
		{
			kPostItem.m_wstrMessage = GET_STRING( STR_ID_3709 );
		}
		break;

		//{{ 2009. 11. 2  ������	��尭����ü
	case KPostItemInfo::LT_GUILD_DISBAND:
		{
			// ��� ���� ��ü
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_4725, "L", kPostItem.m_wstrMessage ) );
		}
		break;

	case KPostItemInfo::LT_GUILD_KICK:
		{
			// ��� ����
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_4730, "L", kPostItem.m_wstrMessage ) );
		}
		break;
		//}}

		//{{ oasis907 : ����� [2010.2.2] // ��� �Խ���
	case KPostItemInfo::LT_GUILD_JOIN:
		{
			// ������ ��� ���� ����
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_4911, "L", kPostItem.m_wstrMessage ) );
		}
		break;
		//}}


		//{{ 2011. 05. 11	������	�븮 ����
#ifdef SERV_PSHOP_AGENCY
	case KPostItemInfo::LT_STOP_SALE_PSHOP:
		{
			kPostItem.m_wstrMessage = GET_STRING( STR_ID_12251 );
		}
		break;

	case KPostItemInfo::LT_SELL_ITEM_PSHOP:
		{
			std::wstring wstrItemName;
			const int iItemID = _wtoi( kPostItem.m_wstrMessage.c_str() );
			const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( iItemID );
			if( pItemTemplet != NULL )
			{
				wstrItemName = pItemTemplet->GetName();
			}

			kPostItem.m_wstrMessage.clear();
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_12252, "L", wstrItemName ) );
		}
		break;
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 05. 27    ��μ�    �޸� ���� ���� ����
#ifdef SERV_COME_BACK_USER_REWARD
	case KPostItemInfo::LT_COME_BACK_USER_NOTIFY:
		{
			kPostItem.m_wstrMessage.clear();
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_12578, "L", kPostItem.m_wstrMessage ) );
		}
		break;
#endif SERV_COME_BACK_USER_REWARD
		//}}
		//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
	case KPostItemInfo::LT_OUT_USER_RETANING_REWARD:
		{
			kPostItem.m_wstrMessage.clear();
			kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_13712, "LL", wstrNickName, wstrNickName ) );
		}break;
#endif SERV_NEXON_SESSION_PROTOCOL
		//}} 
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	case KPostItemInfo::LT_QUEST_REWARD:
		{
			const int iQuestID = _wtoi( kPostItem.m_wstrMessage.c_str() );
			if( NULL != g_pData && NULL != g_pData->GetQuestManager() )
			{
				const CX2QuestManager::QuestTemplet* pQuestTemplet = g_pData->GetQuestManager()->GetQuestTemplet(iQuestID);

				if( NULL != pQuestTemplet )
				{
					kPostItem.m_wstrMessage.clear();
					kPostItem.m_wstrMessage = GET_REPLACED_STRING( ( STR_ID_17698, "L", pQuestTemplet->m_wstrTitle ) );
				}
			}
		} break;
#endif //SERV_ENTER_FIELD_QUEST_CLEAR
#ifdef SERV_RELATIONSHIP_SYSTEM
	case KPostItemInfo::LT_BREAK_UP :
		{
			kPostItem.m_wstrMessage = GET_STRING ( STR_ID_24666 );
		}
		break;
#endif // SERV_RELATIONSHIP_SYSTEM
	default:
		break;
	}	
}

void CX2RewardTable::AssignStringFromRewardTable( IN const int iRewardID, OUT std::wstring& wstrMessage, IN const bool bOnlyItemName /*= false*/ )
{
	std::map< int, int > mapRewardItem;
	std::map< int, int >::const_iterator mit;
	GetRewardInfo( iRewardID, mapRewardItem );

	for( mit = mapRewardItem.begin(); mit != mapRewardItem.end(); ++mit )
	{
		const CX2Item::ItemTemplet* pItemTemplet = g_pData->GetItemManager()->GetItemTemplet( mit->first );
		if( pItemTemplet == NULL )
			continue;
		if( bOnlyItemName )
		{
			wstrMessage += pItemTemplet->GetName(); // �޽��� ���ڿ� ����
		}
		else
		{
			wstrMessage += GET_REPLACED_STRING( ( STR_ID_3692, "Si", pItemTemplet->GetName(), mit->second ) ); // �޽��� ���ڿ� ����
		}
	}
}

bool CX2RewardTable::AddRewardData_LUA( int iRewardID, int iItemID, int iQuantity, short sPeriod )
{
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
#else //SERV_EVENT_DB_CONTROL_SYSTEM
	if( iRewardID <= 0  ||  iItemID <= 0  ||  iQuantity <= 0  ||  sPeriod < 0 )
	{
		//ErrorLog( KEM_ERROR359 );
		return false;
	}
	
	KRewardData kRewardData;
	kRewardData.m_iRewardItemID = iItemID;
	kRewardData.m_iQuantity		= iQuantity;
	kRewardData.m_sPeriod		= sPeriod;	
	
	std::map< int, std::vector< KRewardData > >::iterator mit;
	mit = m_mapRewardData.find( iRewardID );
	if( mit == m_mapRewardData.end() )
	{
		// ���ٸ�..
		std::vector< KRewardData > vecRewardData;
		vecRewardData.push_back( kRewardData );
		m_mapRewardData.insert( std::make_pair( iRewardID, vecRewardData ) );
	}
	else
	{
		mit->second.push_back( kRewardData );
	}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	return true;;
}

//{{ 2011. 06. 22    ��μ�    ��Ż ���� �� - NEXON ���� �ý��� ����
#ifdef SERV_NEXON_SESSION_PROTOCOL
void CX2RewardTable::SetOutUserRetainingRewardItemInfo_LUA( int iSlot, int iItemID, int iQuantity )
{
	// ���� �ڵ�.
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 


//{{ 2011. 10.26    ��μ�	ĳ���� ������ �̺�Ʈ
#ifdef SERV_CHAR_LEVEL_UP_EVENT
void CX2RewardTable::AddLevelUpRewardData_LUA( u_char ucLevel, int iRewardID )
{
	// ���� �ڵ�
}
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}