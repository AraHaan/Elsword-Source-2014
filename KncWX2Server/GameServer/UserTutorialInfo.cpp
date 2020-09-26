#include "GSUser.h"
#include ".\usertutorialinfo.h"
#include "TutorialManager.h"
#include "RewardTable.h"
#include "NetError.h"
//{{ 2012. 07. 27	������		�����ý��� EXP ���ʽ� ���� ����
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
	#include "GSFSM.h"
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
//}}



KUserTutorialInfo::KUserTutorialInfo(void)
:
m_TutorUnitType( TUT_NONE )
{
}

KUserTutorialInfo::~KUserTutorialInfo(void)
{
}

bool KUserTutorialInfo::IsExistUnit( UidType iUnitUID )
{
	std::vector<KTutorialUnitInfo>::iterator vit;

	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == iUnitUID )
			return true;
	}
	
	return false;
}

bool KUserTutorialInfo::IsExistbyTutorData()
{
	if( m_vecTutorInfo.size() > 0 )
		return true;

	return false;
}

int KUserTutorialInfo::GetTutorialUnitNum()
{
	return static_cast<int>(m_vecTutorInfo.size());
}

void KUserTutorialInfo::GetTutorUnitUIDList(std::vector<UidType>& vecUIDList )
{
	std::vector<KTutorialUnitInfo>::iterator vit;

	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		vecUIDList.push_back( vit->m_iUnitUID );
	}
}

bool KUserTutorialInfo::OnSelectUnitAck( KGSUserPtr spUser, std::vector<KTutorialDBUnitInfo>& vecTutorialUnitInfo )
{
	ResetData();

	if( spUser->IsGuestUser() == true )
		return true;

	if( spUser->GetLevel() < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
	{
		m_TutorUnitType = TUT_STUDENT;
	}
	else if( spUser->GetLevel() >= KTutorialManager::UNIT_LEVEL_TEACHER_MIN )
	{
		m_TutorUnitType = TUT_TEACHER;
	}
	else
	{
		//�����ý��ۿ� ������ �ʴ� ������ ������.
		m_TutorUnitType = TUT_NONE;
		return true;
	}

	std::vector<KTutorialDBUnitInfo>::iterator vit;
	for( vit = vecTutorialUnitInfo.begin(); vit != vecTutorialUnitInfo.end(); ++vit )
	{
		CTime LastTime;
		KTutorialUnitInfo kTutorialUnitInfo;

		if( KncUtil::ConvertStringToCTime( vit->m_wstrLastDate, LastTime ) == false )
		{
			START_LOG( cerr, L"���ڿ� �ð����� ����.!(;��; ũ��Ƽ���ѵ�..)" )
				<< BUILD_LOG( vit->m_wstrLastDate )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUID() )
				<< END_LOG;

			continue;
		}
		//��Ŷ ��ȯ.
		kTutorialUnitInfo.m_iUnitUID = vit->m_kTutorialUnitInfo.m_iUnitUID;
		kTutorialUnitInfo.m_ucLevel = vit->m_kTutorialUnitInfo.m_ucLevel;
		kTutorialUnitInfo.m_wstrNickName = vit->m_kTutorialUnitInfo.m_wstrNickName;
		kTutorialUnitInfo.m_bIsOnline = vit->m_kTutorialUnitInfo.m_bIsOnline;

		m_vecTutorInfo.push_back( kTutorialUnitInfo );

		//�������� 7���̻� �������� �˻��Ѵ�.
		bool bIsDelete = false;
		if( m_TutorUnitType == TUT_TEACHER )
		{
			CTimeSpan tsTemp( KTutorialManager::STUDENT_DISCONNECT_DAY, 0, 0, 0 );
			CTime tNowDate = CTime::GetCurrentTime();
			if( (LastTime + tsTemp) < tNowDate )
			{
				KDBE_DEL_TUTORIAL_REQ kPacket;

				kPacket.m_iTeacherUID = spUser->GetCharUID();
				kPacket.m_iStudentUID = vit->m_kTutorialUnitInfo.m_iUnitUID;
				kPacket.m_cReason = KTutorialManager::TDR_DISCONNECT_DAY;

				spUser->SendToGameDB( DBE_DEL_TUTORIAL_REQ, kPacket );

				bIsDelete = true;
			}
		}
		
		//���ӿ��� ������Ʈ ������.
		if( bIsDelete == false )
		{
			SendUpdateUnitInfo( spUser, vit->m_kTutorialUnitInfo.m_iUnitUID, true, true );
		}
	}


	return true;
}

bool KUserTutorialInfo::OnDelTutorialReq( KGSUserPtr spUser, KEGS_DEL_TUTORIAL_REQ& kInfo )
{
	KEGS_DEL_TUTORIAL_ACK kAck;	

	if( spUser->IsGuestUser() == true )
	{
		kAck.m_iOK = NetError::NET_OK;
		spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
		return true;
	}

	KDBE_DEL_TUTORIAL_REQ kDBReq;
	kDBReq.m_cReason = KTutorialManager::TDR_REFUSAL_USER;

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == kInfo.m_iUnitUID )
		{
			switch( GetTutorUnitType() )
			{
			case TUT_TEACHER:
				{
					kDBReq.m_iTeacherUID = spUser->GetCharUID();
					kDBReq.m_iStudentUID = kInfo.m_iUnitUID;
				}
				break;
			case TUT_STUDENT:
				{
					kDBReq.m_iTeacherUID = kInfo.m_iUnitUID;
					kDBReq.m_iStudentUID = spUser->GetCharUID();
				}
				break;

			default:
				{
					START_LOG( cerr, L"���� Ÿ���� �̻���.!" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( kInfo.m_iUnitUID )
						<< END_LOG;

					return false;
				}
			}

			spUser->SendToGameDB( DBE_DEL_TUTORIAL_REQ, kDBReq );

			return true;
		}
	}

	kAck.m_iOK = NetError::ERR_TUTORIAL_11;
	spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
	return false;
}

bool KUserTutorialInfo::OnDelTutorialAck( KGSUserPtr spUser, KDBE_DEL_TUTORIAL_ACK& kInfo )
{
	KEGS_DEL_TUTORIAL_ACK kAck;	

	if( spUser->IsGuestUser() == true )
	{
		kAck.m_iOK = NetError::NET_OK;
		spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
		return true;
	}
	
	UidType iTempUID = 0;

	//���� �������忡 ���� �˻��� uid�� ������.
	switch( GetTutorUnitType() )
	{
	case TUT_TEACHER:
		iTempUID = kInfo.m_iStudentUID;
		break;

	case TUT_STUDENT:
		{
			iTempUID = kInfo.m_iTeacherUID;

			//���ó��.
			if( spUser->GetLevel() >= KTutorialManager::UNIT_LEVEL_STUDENT_MAX && spUser->GetLevel() < KTutorialManager::UNIT_LEVEL_TEACHER_MIN )
			{
				if( kInfo.m_cReason == KTutorialManager::TDR_LEVEL_UP_END )
				{
					SetTutorUnitType( TUT_NONE );
				}
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"���� Ÿ���� �̻���.!" )
				<< BUILD_LOG( kInfo.m_iTeacherUID )
				<< BUILD_LOG( kInfo.m_iStudentUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_TUTORIAL_11;
			spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
			return false;
		}
	}

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == iTempUID )
		{
			KEGS_DEL_TUTORIAL_NOT kPacket;
			//�����Դ� ���� uid�� ��������̰�
			kPacket.m_iUnitUID = iTempUID;
			//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
			kPacket.m_iReason = SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kInfo.m_cReason) );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
			kPacket.m_wstrReason = SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kInfo.m_cReason) );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
			//}}

			spUser->SendPacket( EGS_DEL_TUTORIAL_NOT, kPacket );

			if( vit->m_bIsOnline == true )
			{
				//���濡�Դ� �� uid�� ��������̴�.
				KELG_DEL_TUTORIAL_NOT kNot;
				kNot.m_iReceiverUnitUID = vit->m_iUnitUID;
				kNot.m_iDeleteUnitUID = spUser->GetCharUID();
				//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
				kNot.m_iReason = SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kInfo.m_cReason) );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
				kNot.m_wstrReason = SiKTutorialManager()->GetDelReason( static_cast<KTutorialManager::TUTORIAL_DEL_REASON>(kInfo.m_cReason) );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
				//}}
				spUser->SendToLoginServer( ELG_DEL_TUTORIAL_NOT, kNot );
			}

			m_vecTutorInfo.erase( vit );
			
			kAck.m_iOK = NetError::NET_OK;
			spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
			return true;
		}
	}

	kAck.m_iOK = NetError::ERR_TUTORIAL_11;
	spUser->SendPacket( EGS_DEL_TUTORIAL_ACK, kAck );
	return false;
}

bool KUserTutorialInfo::OnDelTutorialNot( KGSUserPtr spUser, KEGS_DEL_TUTORIAL_NOT& kInfo )
{
	if( spUser->IsGuestUser() == true )
		return true;

	//{{ 2012. 07. 27	������		�����ý��� EXP ���ʽ� ���� ����
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
	if( spUser->GetStateID() == KGSFSM::S_ROOM  &&  spUser->GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			{
                KERM_UPDATE_TUTORIAL_INFO_NOT kPacketToRm;
				spUser->GetTutorUnitUIDList( kPacketToRm.m_vecStudentUnitUID );
				spUser->SendToCnRoom( ERM_UPDATE_TUTORIAL_INFO_NOT, kPacketToRm );
			}
			break;
		}
	}
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
	//}}

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( vit->m_iUnitUID == kInfo.m_iUnitUID )
		{
			spUser->SendPacket( EGS_DEL_TUTORIAL_NOT, kInfo );

			m_vecTutorInfo.erase( vit );

			return true;
		}
	}

	return false;
}

bool KUserTutorialInfo::OnTutorialUpdateUnitInfoNot( KGSUserPtr spUser, KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT& kInfo )
{
	if( spUser->IsGuestUser() == true )
		return true;

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		if( kInfo.m_kTutorialUnitInfo.m_iUnitUID == vit->m_iUnitUID )
		{
			*vit = kInfo.m_kTutorialUnitInfo;

			spUser->SendPacket( EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT, kInfo );

			//������Ʈ�� �Դµ� ���� ������ �ٽ� ������Ʈ ��û�Ҷ� �����ش�.
			if( kInfo.m_bIsReply == true )
			{
				SendUpdateUnitInfo( spUser, kInfo.m_kTutorialUnitInfo.m_iUnitUID );
			}

			return true;
		}
	}

	//ó�� �������踦 �δ� ���ڰ� ������Ʈ�� �������.
	if( m_vecTutorInfo.empty() == true )
	{
		m_vecTutorInfo.push_back( kInfo.m_kTutorialUnitInfo );

		spUser->SendPacket( EGS_TUTORIAL_UPDATE_UNIT_INFO_NOT, kInfo );

		return true;
	}

	return false;
}

void KUserTutorialInfo::SendUpdateUnitInfo( KGSUserPtr spUser, UidType iReceiverUID, bool bIsOnline /* = true */, bool bIsReply /* = false  */)
{
	if( spUser->IsGuestUser() == true )
		return;

	KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT kNot;
	kNot.m_bIsReply		= bIsReply;
	kNot.m_iReceiverUID	= iReceiverUID;

	kNot.m_kTutorialUnitInfo.m_bIsOnline	= bIsOnline;
	kNot.m_kTutorialUnitInfo.m_iUnitUID		= spUser->GetCharUID();
	kNot.m_kTutorialUnitInfo.m_ucLevel		= spUser->GetLevel();
	kNot.m_kTutorialUnitInfo.m_wstrNickName	= spUser->GetCharName();

	spUser->SendToLoginServer( ELG_TUTORIAL_UPDATE_UNIT_INFO_NOT, kNot );
}

void KUserTutorialInfo::OnDeleteUnitAck( KGSUserPtr spUser )
{
	if( spUser->IsGuestUser() == true )
		return;

	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		KEGS_DEL_TUTORIAL_NOT kPacket;
		kPacket.m_iUnitUID = vit->m_iUnitUID;
		//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		kPacket.m_iReason = SiKTutorialManager()->GetDelReason( KTutorialManager::TDR_DELETE_UNIT );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
		kPacket.m_wstrReason = SiKTutorialManager()->GetDelReason( KTutorialManager::TDR_DELETE_UNIT );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}

		spUser->SendPacket( EGS_DEL_TUTORIAL_NOT, kPacket );

		if( vit->m_bIsOnline == true )
		{
			KELG_DEL_TUTORIAL_NOT kNot;
			kNot.m_iReceiverUnitUID = vit->m_iUnitUID;
			kNot.m_iDeleteUnitUID = m_iDeleteUnitUID;
			//{{ 2011.09.16 ��ȿ��  �� ���� ��Ʈ���� Ŭ���̾�Ʈ�ʿ��� �����ͼ� ���
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
			kNot.m_iReason = SiKTutorialManager()->GetDelReason( KTutorialManager::TDR_DELETE_UNIT );
#else SERV_NETERROR_STR_GET_FROM_CLIENT
			kNot.m_wstrReason = SiKTutorialManager()->GetDelReason( KTutorialManager::TDR_DELETE_UNIT );
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
			//}}
			spUser->SendToLoginServer( ELG_DEL_TUTORIAL_NOT, kNot );
		}
	}

	ResetData();
}

void KUserTutorialInfo::DisconnectUpdateUnitInfo( KGSUserPtr spUser )
{
	if( spUser->IsGuestUser() == true )
		return;

	//��������� ���� �������� �����鿡�� �������Ḧ �˸���. 
	std::vector<KTutorialUnitInfo>::iterator vit;
	for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
	{
		SendUpdateUnitInfo( spUser, vit->m_iUnitUID, false );
	}
}

void KUserTutorialInfo::OnInsertTutorialAck( KGSUserPtr spUser, KTutorialUnitInfo& kInfo )
{
	if( spUser->IsGuestUser() == true )
		return;

	m_vecTutorInfo.push_back( kInfo );

	//���濡�� ������Ʈ ���� �����ֱ�.
	SendUpdateUnitInfo( spUser, kInfo.m_iUnitUID );
}

void KUserTutorialInfo::ProcCharLevelUP( u_char ucBeforeCharLv, KGSUserPtr spUser )
{
	if( spUser->IsGuestUser() == true )
		return;

	//�����ϰ�츸 ó���� ������.
	if( GetTutorUnitType() == TUT_STUDENT )
	{
		if( GetTutorialUnitNum() > KTutorialManager::TEACHER_NUM )
		{
			START_LOG( cerr, L"������ �ʹ� ����.?" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( GetTutorialUnitNum() )
				<< END_LOG;
		}

		//�Ϲ� ��ü������.
		if( ( ucBeforeCharLv + 1 ) < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		{
			//{{ 2009. 2. 20  ������	�ѹ��� 2���� �̻� ������ ��� ����ó�� ����
			for( u_char ucLv = ucBeforeCharLv + 1; ucLv <= spUser->GetLevel(); ++ucLv )
			{
				int iRewardID = 0;
				switch( ucLv )
				{
				case 2:
				case 3:
					iRewardID = 9;
					break;

				case 4:
				case 5:
				case 6:
					iRewardID = 10;
					break;

				case 7:
				case 8:
				case 9:
					iRewardID = 11;
					break;

				case 10:
					// [����] 8�������� �ѹ��� 10������ �ö󰡴°��
					break;

				default:					
					START_LOG( cerr, L"������ �̻��ϴ�?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( static_cast<int>(ucLv) )
						<< END_LOG;
					break;
				}

				if( iRewardID != 0 )
				{
					std::vector<KTutorialUnitInfo>::iterator vit;
					for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
					{
						//{{ 2008. 4. 8  ������  ������ ����
						KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
						kPacketToDB.m_iFromUnitUID = spUser->GetCharUID();
						kPacketToDB.m_iToUnitUID   = vit->m_iUnitUID;
						kPacketToDB.m_iRewardType  = KPostItemInfo::LT_TUTOL;
						kPacketToDB.m_iRewardID	   = iRewardID; // ������ ���� ť��						

						CStringW cwstrMessage;
						cwstrMessage.Format( L"%s;%d", spUser->GetCharName().c_str(), ucLv );
						kPacketToDB.m_wstrMessage = static_cast<LPCTSTR>(cwstrMessage);

						spUser->SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
						//}}

						//�켱 ������Ʈ ������ ���¿��� �˷��ְ�.
						SendUpdateUnitInfo( spUser, vit->m_iUnitUID );
					}
				}
			}
			//}}
		}

		//�����̸� ����ó���� ��񿡼��� �����ش�.
		if( spUser->GetLevel() >= KTutorialManager::UNIT_LEVEL_STUDENT_MAX  &&  ucBeforeCharLv < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		{
			std::vector<KTutorialUnitInfo>::iterator vit;
			for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
			{
				//{{ 2008. 4. 8  ������  ���� ���� ����
				KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
				kPacketToDB.m_iFromUnitUID = spUser->GetCharUID();
				kPacketToDB.m_iToUnitUID   = vit->m_iUnitUID;
				kPacketToDB.m_iRewardType  = KPostItemInfo::LT_TUTOL;
				kPacketToDB.m_iRewardID	   = 6; // ��Ȱ�� 5��				

				CStringW cwstrMessage;
				cwstrMessage.Format( L"%s;%d", spUser->GetCharName().c_str(), spUser->GetLevel() );
				kPacketToDB.m_wstrMessage = static_cast<LPCTSTR>(cwstrMessage);

				spUser->SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				//}}

				//�������踦 �����ش�.
				KDBE_DEL_TUTORIAL_REQ kPacket;

				if( GetTutorUnitType() == TUT_STUDENT )
				{
					kPacket.m_iTeacherUID = vit->m_iUnitUID;
					kPacket.m_iStudentUID = spUser->GetCharUID();
				}
				else
				{
					kPacket.m_iTeacherUID = spUser->GetCharUID();
					kPacket.m_iStudentUID = vit->m_iUnitUID;

					START_LOG( cerr, L"+��+ �� ���ڶ� ������..!" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( vit->m_iUnitUID )
						<< END_LOG;
				}

				kPacket.m_cReason = KTutorialManager::TDR_LEVEL_UP_END;
				spUser->SendToGameDB( DBE_DEL_TUTORIAL_REQ, kPacket );
			}

			//���� ���¸� ��ü�����ش�. �������� �̴ϱ�..
			if( IsExistbyTutorData() == false )
			{
				SetTutorUnitType( TUT_NONE );
			}
		}
	}
}

void KUserTutorialInfo::ResetData()
{
	m_vecTutorInfo.clear();
	m_TutorUnitType = TUT_NONE;
	m_iDeleteUnitUID = 0;
}

#ifdef SERV_JUMPING_CHARACTER// �۾���¥: 2013-07-12	// �ڼ���
void KUserTutorialInfo::ProcJumpingCharLevelUP( IN const u_char ucBeforeCharLv, IN const KGSUserPtr spUser )
{
	if( spUser->IsGuestUser() == true )
		return;

	//�����ϰ�츸 ó���� ������.
	if( GetTutorUnitType() == TUT_STUDENT )
	{
		if( GetTutorialUnitNum() > KTutorialManager::TEACHER_NUM )
		{
			START_LOG( cerr, L"������ �ʹ� ����.?" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( GetTutorialUnitNum() )
				<< END_LOG;
		}

		//�Ϲ� ��ü������.
		if( ( ucBeforeCharLv + 1 ) < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		{
			//{{ 2009. 2. 20  ������	�ѹ��� 2���� �̻� ������ ��� ����ó�� ����
			for( u_char ucLv = ucBeforeCharLv + 1; ucLv <= spUser->GetLevel(); ++ucLv )
			{
				int iRewardID = 0;
				switch( ucLv )
				{
				case 2:
				case 3:
					iRewardID = 9;
					break;

				case 4:
				case 5:
				case 6:
					iRewardID = 10;
					break;

				case 7:
				case 8:
				case 9:
					iRewardID = 11;
					break;

				case 10:
					// [����] 8�������� �ѹ��� 10������ �ö󰡴°��
					break;

				default:
					break;
				}

				if( iRewardID != 0 )
				{
					std::vector<KTutorialUnitInfo>::iterator vit;
					for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
					{
						//�켱 ������Ʈ ������ ���¿��� �˷��ְ�.
						SendUpdateUnitInfo( spUser, vit->m_iUnitUID );
					}
				}
			}
			//}}
		}

		//�����̸� ����ó���� ��񿡼��� �����ش�.
		if( spUser->GetLevel() >= KTutorialManager::UNIT_LEVEL_STUDENT_MAX  &&  ucBeforeCharLv < KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		{
			std::vector<KTutorialUnitInfo>::iterator vit;
			for( vit = m_vecTutorInfo.begin(); vit != m_vecTutorInfo.end(); ++vit )
			{
				//�������踦 �����ش�.
				KDBE_DEL_TUTORIAL_REQ kPacket;

				if( GetTutorUnitType() == TUT_STUDENT )
				{
					kPacket.m_iTeacherUID = vit->m_iUnitUID;
					kPacket.m_iStudentUID = spUser->GetCharUID();
				}
				else
				{
					kPacket.m_iTeacherUID = spUser->GetCharUID();
					kPacket.m_iStudentUID = vit->m_iUnitUID;

					START_LOG( cerr, L"+��+ �� ���ڶ� ������..!" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( vit->m_iUnitUID )
						<< END_LOG;
				}

				kPacket.m_cReason = KTutorialManager::TDR_LEVEL_UP_END;
				spUser->SendToGameDB( DBE_DEL_TUTORIAL_REQ, kPacket );
			}

			//���� ���¸� ��ü�����ش�. �������� �̴ϱ�..
			if( IsExistbyTutorData() == false )
			{
				SetTutorUnitType( TUT_NONE );
			}
		}
	}
}
#endif // SERV_JUMPING_CHARACTER
