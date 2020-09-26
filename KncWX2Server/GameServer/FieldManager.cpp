#include "FieldManager.h"
#include "Lua/KLuaManager.h"
#include "NetError.h"
#include "FieldFSM.h"

NiImplementRTTI( KFieldManager, KRoomManager );

ImplToStringW( KFieldManager )
{
	stm_ << L"======[Field Manager Dump]======" << std::endl << std::endl;

	// �ʵ� ���� �̺�Ʈ ������
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, DMP_DUMP_EVENT_NOT );
	spEvent->m_kDestPerformer.AddUID( 0 );

	std::map< UidType, KFieldPtr >::const_iterator mit;
	for( mit = m_mapFiledList.begin(); mit != m_mapFiledList.end(); ++mit )
	{
		KFieldPtr spField = mit->second;
		if( spField == NULL )
			continue;

		spField->QueueingEvent( spEvent->Clone() );
	}

	return stm_;
}

KFieldManager::KFieldManager()
:
m_bIsFieldManagerDump( false )
{
}

KFieldManager::~KFieldManager()
{
}

void KFieldManager::Run()
{
	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 20 );       // 0.02s ����

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;

	}

	return;
}

bool KFieldManager::Init()
{
	// create field..
	m_spFieldFSM = boost::shared_ptr<KFieldFSM>( new KFieldFSM );
	for( int i = 0; i < static_cast<int>(m_vecFieldInfo.size()); ++i )
	{
		UidType	iFieldUID = 0;
		KFieldPtr spField(new KField);

		//UID create
		KncUid::SetPureUID( iFieldUID, static_cast<UidType>(i) );
		KncUid::SetCodeID( iFieldUID, static_cast<UidType>(m_vecFieldInfo[i].m_iID) ); // RoomUID�� 0�̵Ǹ� �ȵǹǷ� �־��ش�!

		spField->SetUID( iFieldUID );
		spField->SetFSM( m_spFieldFSM );

		m_mapFiledList[iFieldUID] = spField;
		m_mapEmptyFieldList[iFieldUID] = iFieldUID;

		// �ʵ� ���� ������Ʈ ---> �ϴ� �ʵ� ���� ������Ʈ �Ҷ� mapid�� ���� ������Ʈ �Ѵ�.
		boost::static_pointer_cast<KField>(spField)->SetFieldInfo( m_vecFieldInfo[i].m_iID, m_vecFieldInfo[i].m_kTileInfo, m_vecFieldInfo[i].m_kMapInfo );

		// �ʵ� ���� �̺�Ʈ ������
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_FIELD, NULL, ERM_OPEN_FIELD_NOT );
		spEvent->m_kDestPerformer.AddUID( 0 );
		QueueingEventToRoom( spEvent );
	}

	return KThread::Begin();
}

void KFieldManager::Tick()
{
	std::map<UidType,KFieldPtr>::iterator mit;
	for( mit = m_mapFiledList.begin(); mit != m_mapFiledList.end(); ++mit )
	{
		if( mit->second )
		{
			mit->second->Tick();
		}
		else
		{
			START_LOG( cerr, L"field pointer �̻�.!" )
				<< END_LOG;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//if( !m_bIsFieldManagerDump )
	//	return;

	//if( m_tDumpTimer.elapsed() < 60.0 )
	//	return;

	//Dump();

	//m_tDumpTimer.restart();
	//////////////////////////////////////////////////////////////////////////

	return;
}

#include <KncLua.h>
#include <lua_tinker.h>

void KFieldManager::RegToLua()
{
	// success lua func
	lua_tinker::class_add<KFieldManager>( g_pLua, "KFieldManager" );
	lua_tinker::class_def<KFieldManager>( g_pLua, "AddFieldInfo",		&KFieldManager::AddFieldInfo_LUA );
	lua_tinker::class_def<KFieldManager>( g_pLua, "dump",				&KFieldManager::Dump );
	lua_tinker::class_def<KFieldManager>( g_pLua, "IsDump",				&KFieldManager::SetFieldManagerDump_LUA );

	lua_tinker::decl( g_pLua, "FieldManager", this );
}

UidType KFieldManager::OpenField()
{
	KLocker lock( m_csEmptyFieldList );

	if( m_mapEmptyFieldList.empty() )
	{
		START_LOG( cerr, L"�� �ʵ尡 ����." )
			<< END_LOG;

		return 0;
	}

	std::map< UidType, UidType >::iterator mit;
	for( mit = m_mapEmptyFieldList.begin(); mit != m_mapEmptyFieldList.end(); mit++ )
	{
		KFieldPtr spField = Get( mit->first );
		if( spField == NULL )
		{
			START_LOG( cerr, L"�� �ʵ� �ε����� �̻���." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}
		else
		{
			// ��� �ʵ� FSM ���� INIT, CLOSE, WAIT �� ������ �̵��� ���� ����. 
			if( spField->GetStateID() == KFieldFSM::S_INIT )
			{
				break;
			}
			else
			{
				START_LOG( cerr, L"empty field ����Ʈ�� ������ �� ���°� �̻���." )
					<< BUILD_LOG( spField->GetStateID() )
					<< BUILD_LOG( spField->GetStateIDString() )
					<< END_LOG;
			}
		}
	}

	if( mit == m_mapEmptyFieldList.end() )
	{
		START_LOG( cerr, L"empty field �� ã�� �� ����." )
			<< END_LOG;

		return 0;
	}

	UidType iFieldUID = mit->first;
	m_mapEmptyFieldList.erase( mit );

	return iFieldUID;
}

bool KFieldManager::CloseField( UidType iFieldUID )
{
	KLocker lock( m_csEmptyFieldList );

	std::map< UidType, UidType >::iterator mit;
	mit = m_mapEmptyFieldList.find( iFieldUID );
	if( mit != m_mapEmptyFieldList.end() )
	{
		START_LOG( cerr, L"empty field ����Ʈ�� �������� ID�� ���� �̹� ����." )
			<< BUILD_LOG( iFieldUID )
			<< END_LOG;

		return false;
	}

	KFieldPtr spField = Get( iFieldUID );
	if( !spField )
	{
		START_LOG( cerr, L"�ش� UID�� field �� ����." )
			<< BUILD_LOG( iFieldUID )
			<< END_LOG;

		return false;
	}

	// ��� field FSM ���� INIT, CLOSE, WAIT �� ������ �̵��� ���� ����.
	if( spField->GetStateID() != KFieldFSM::S_CLOSE )
	{
		START_LOG( cerr, L"field ���� �̻�." )
			<< BUILD_LOG( spField->GetStateID() )
			<< BUILD_LOG( iFieldUID )
			<< END_LOG;

		return false;
	}

	// ��� field FSM ���� INIT, CLOSE, WAIT �� ������ �̵��� ���� ����.
	spField->StateTransition( KFieldFSM::I_TO_INIT );
	m_mapEmptyFieldList.insert( std::make_pair( iFieldUID, iFieldUID ) );

	return true;
}

void KFieldManager::QueueingEventToRoom( const KEventPtr& spEvent_ )
{
	UidType iFieldUID;
	switch( spEvent_->m_usEventID )
	{
	case ERM_OPEN_FIELD_NOT:
		iFieldUID = OpenField();
		break;

	default:
		iFieldUID = spEvent_->m_kDestPerformer.GetFirstUID();
		break;
	}

	if( iFieldUID <= 0 )
	{
		switch( spEvent_->m_usEventID )
		{
		case ERM_OPEN_FIELD_NOT:
			{
				START_LOG( cerr, L"�ʵ� ���� ����.!" )
					<< BUILD_LOG( iFieldUID )
					<< END_LOG;
				return;
			}

		default:
			START_LOG( cerr, L"�̺�Ʈ�� ��ǥ �� UID�� �̻���." )
				<< BUILD_LOG( iFieldUID )
				<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
				<< END_LOG;
			return;
		}
	}

	KFieldPtr spField = Get( iFieldUID );
	if( spField == NULL )
	{
		START_LOG( cerr, L"field �˻� ����.!" )
			<< BUILD_LOG( iFieldUID )
			<< END_LOG;
		return;
	}

	spField->QueueingEvent( spEvent_ );

	return;
}

void KFieldManager::QueueingToAll( int iRoomType, const KEventPtr& spEvent_ )
{
}

void KFieldManager::QueueingToAll( const KEventPtr& spEvent_ )
{
}

void KFieldManager::SendToCharacter( UidType nTo, unsigned short usEventID )
{
	SendToCharacter( nTo, usEventID, char() );
}

void KFieldManager::AddFieldInfo_LUA()
{
	KLuaManager	lM( g_pLua );
	sFieldInfo	sInfo;

	LUA_GET_VALUE( lM,	L"ID",	sInfo.m_iID, -1 );

	if( sInfo.m_iID < 0 )
	{
		START_LOG( cerr, L"FIELD ID FAIL .!" )
			<< BUILD_LOG( sInfo.m_iID )
			<< END_LOG;
		return;
	}

	if( lM.BeginTable( "TILE_INFO" ) == S_OK )
	{
		LUA_GET_VALUE( lM,	L"NUM_X",	sInfo.m_kTileInfo.m_NumX,	-1 );
		LUA_GET_VALUE( lM,	L"NUM_Y",	sInfo.m_kTileInfo.m_NumY,	-1 );
		LUA_GET_VALUE( lM,	L"NUM_Z",	sInfo.m_kTileInfo.m_NumZ,	-1 );

		LUA_GET_VALUE( lM,	L"SIZE_X",	sInfo.m_kTileInfo.m_SizeX,	-1 );
		LUA_GET_VALUE( lM,	L"SIZE_Y",	sInfo.m_kTileInfo.m_SizeY,	-1 );
		LUA_GET_VALUE( lM,	L"SIZE_Z",	sInfo.m_kTileInfo.m_SizeZ,	-1 );

		lM.EndTable();
	}
	else
	{
		START_LOG( cerr, L"FIELD IN TILE TABLE FAIL .!" )
			<< END_LOG;
	}

	if( lM.BeginTable( "MAP_INFO" ) == S_OK )
	{
		LUA_GET_VALUE( lM,	L"MIN_X",	sInfo.m_kMapInfo.m_MinX,	-1 );
		LUA_GET_VALUE( lM,	L"MIN_Y",	sInfo.m_kMapInfo.m_MinY,	-1 );
		LUA_GET_VALUE( lM,	L"MIN_Z",	sInfo.m_kMapInfo.m_MinZ,	-1 );

		LUA_GET_VALUE( lM,	L"MAX_X",	sInfo.m_kMapInfo.m_MaxX,	-1 );
		LUA_GET_VALUE( lM,	L"MAX_Y",	sInfo.m_kMapInfo.m_MaxY,	-1 );
		LUA_GET_VALUE( lM,	L"MAX_Z",	sInfo.m_kMapInfo.m_MaxZ,	-1 );
	}
	else
	{
		START_LOG( cerr, L"FIELD IN MAP TABLE FAIL .!" )
			<< END_LOG;
	}

	//FIELD INFO : �Ѱ��� �߸���� �ð�� �ʵ带 �����ؼ��� �ȵȴ�.
	//�� �ȵǸ� ���ڷ� ����������.?
	if( sInfo.m_iID != -1 && 

		sInfo.m_kTileInfo.m_NumX != -1.0f &&
		sInfo.m_kTileInfo.m_NumY != -1.0f &&
		sInfo.m_kTileInfo.m_NumZ != -1.0f &&

		sInfo.m_kTileInfo.m_SizeX != -1.0f &&
		sInfo.m_kTileInfo.m_SizeY != -1.0f &&
		sInfo.m_kTileInfo.m_SizeZ != -1.0f &&

		sInfo.m_kMapInfo.m_MinX != -1.0f &&
		sInfo.m_kMapInfo.m_MinY != -1.0f &&
		sInfo.m_kMapInfo.m_MinZ != -1.0f &&

		sInfo.m_kMapInfo.m_MaxX != -1.0f &&
		sInfo.m_kMapInfo.m_MaxY != -1.0f &&
		sInfo.m_kMapInfo.m_MaxZ != -1.0f
		)
	{
		m_vecFieldInfo.push_back( sInfo );

		START_LOG( clog2, L"::: Add Field info.! :::" )
			<< BUILD_LOG( sInfo.m_iID );
	}
	else
	{
		START_LOG( cerr, L"FIELD INFO �߸��Ǿ���.!(���ٽ� Ȯ��)" )
			<< BUILD_LOG( sInfo.m_iID )
			<< BUILD_LOG( sInfo.m_kTileInfo.m_NumX )
			<< BUILD_LOG( sInfo.m_kTileInfo.m_NumY )
			<< BUILD_LOG( sInfo.m_kTileInfo.m_NumZ )
			<< BUILD_LOG( sInfo.m_kTileInfo.m_SizeX )
			<< BUILD_LOG( sInfo.m_kTileInfo.m_SizeY )
			<< BUILD_LOG( sInfo.m_kTileInfo.m_SizeZ )
			<< BUILD_LOG( sInfo.m_kMapInfo.m_MinX )
			<< BUILD_LOG( sInfo.m_kMapInfo.m_MinY )
			<< BUILD_LOG( sInfo.m_kMapInfo.m_MinZ )
			<< BUILD_LOG( sInfo.m_kMapInfo.m_MaxX )
			<< BUILD_LOG( sInfo.m_kMapInfo.m_MaxY )
			<< BUILD_LOG( sInfo.m_kMapInfo.m_MaxZ )
			<< END_LOG;
	}
}

KFieldPtr KFieldManager::Get( UidType iFieldUID )
{
	std::map<UidType,KFieldPtr>::iterator mit;
	mit = m_mapFiledList.find( iFieldUID );
	if( mit == m_mapFiledList.end() )
	{
		return KFieldPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"field pointer �̻�.!" )
			<< BUILD_LOG( iFieldUID )
			<< END_LOG;

		return KFieldPtr();
	}

	return mit->second;
}


