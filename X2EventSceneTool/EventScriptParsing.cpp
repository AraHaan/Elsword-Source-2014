#include "dxstdafx.h"
#include ".\eventscriptparsing.h"

CEventScriptParsing::CEventScriptParsing(void)
{
}

CEventScriptParsing::~CEventScriptParsing(void)
{
}

void CEventScriptParsing::OpenScriptFile( IN const WCHAR * pFileName )
{
	WCHAR wszPath[MAX_PATH] = L"";
	WCHAR wszFileName[MAX_PATH] = L"";

	// Ǯ�н��� ��ο� �����̸����� �и�
	if( true == _EST_OBJECT_::DecomposePathAndFileName(wszPath, wszFileName, pFileName) )
	{
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( wszFileName );
		// �����̸����� �ε��� �����ϸ�
		if( Info == NULL )
		{
			std::wstring wstrDirectory = wszPath;

			// ��θ� �߰���
			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wstrDirectory);

			// ���ο� ��� �߰��� ���� �ٽ� ���Ͽ���.
			Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( wszFileName );
			if( Info == NULL )
			{
				std::wstring wstrMsg = L"���� ���� ����.! ";
				wstrMsg += pFileName;
				ERRORMSG(wstrMsg.c_str());
				return;
			}
			else
			{
				ASSERT( NULL != Info->pRealData );
				if( NULL == Info->pRealData )
				{
					ERRORMSG(L"��ũ��Ʈ ������ ������ ����.!");
					return;
				}

				if( false == Parsing(Info->pRealData, Info->size) )
				{
					std::wstring wstrMsg = L"��ũ��Ʈ �м�����.! ";
					wstrMsg += pFileName;
					ERRORMSG(wstrMsg.c_str());
					return;
				}
			}
		}
		else
		{
			if( false == Parsing(Info->pRealData, Info->size) )
			{
				std::wstring wstrMsg = L"��ũ��Ʈ �м�����.! ";
				wstrMsg += pFileName;
				ERRORMSG(wstrMsg.c_str());
				return;
			}
		}
	}
	else
	{
		std::wstring wstrMsg = L"���� ��ΰ� �߸��Ǿ���.! ";
		wstrMsg += pFileName;
		ERRORMSG(wstrMsg.c_str());
	}
}

void CEventScriptParsing::GetEventIDList( _EST_OBJECT_::EventIDList & eventIDList )
{
	_EST_OBJECT_::EventSceneData::iterator mit;

	mit = m_mapEventSceneData.begin();
	while( mit != m_mapEventSceneData.end() )
	{
		eventIDList.push_back(mit->first);
		++mit;
	}
}

bool CEventScriptParsing::GetUTF8byEventScriptData( IN std::wstring wstrEventID, OUT std::wstring &wstrScript )
{
	char charBuf[255] = {0,};
	::WideCharToMultiByte( CP_ACP, 0, wstrEventID.c_str(), -1, charBuf, 255, NULL, NULL );

	_EST_OBJECT_::EventSceneData::iterator mit;
	mit = m_mapEventSceneData.find(charBuf);

	if( mit == m_mapEventSceneData.end() )
		return false;

	WCHAR *pwszScript = new WCHAR[mit->second.size()];

	::MultiByteToWideChar( CP_UTF8, 0, mit->second.c_str(), -1, pwszScript, mit->second.size() );

	wstrScript = pwszScript;

	delete[] pwszScript;

	return true;
}

bool CEventScriptParsing::ScriptRefresh( IN const WCHAR *pScrpt, IN const long iLength )
{
	int iBufferSize = iLength * 2;
	//int iResultSize = 0;

	char *pcBuffer = new char[iBufferSize]; // �ּ����� �ѱ��� ����������� 2����Ʈ�� �ʿ��ϱ� ������ *2 �� ��. 

	
	/*iResultSize = */::WideCharToMultiByte( CP_UTF8, 0, pScrpt, -1, pcBuffer, iBufferSize, NULL, NULL );

	bool bResult = Parsing(pcBuffer, strlen(pcBuffer)/*iResultSize*/);

	if( false == bResult )
		ERRORMSG(L"Script �Ľ̽���.!");

	// Lua refresh
	// Refresh �� ���������� ��Ƶ� ���Ӱ� �Ľ����ش�.
	if( true == bResult )
	{
		lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pEventScene", GetRenderFrm()->GetX2EventScene() );
		bResult = lua_tinker::dostring(g_pKTDXApp->GetLuaBinder()->GetLuaState(), pcBuffer);
		//bResult = g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncript(pTemp, iResultSize + 3);

		if( false == bResult )
			ERRORMSG(L"Script ��� ��ε� ����.!");
	}

	SAFE_DELETE_ARRAY(pcBuffer);

	return bResult;
}

void CEventScriptParsing::Clear()
{
	m_mapEventSceneData.clear();
}

bool CEventScriptParsing::SaveScript()
{
	std::wstring wstrFileName;

	if( true == _EST_OBJECT_::GetDlgFileName(g_pKTDXApp->GetHWND(), wstrFileName, false) )
	{
		WCHAR wszBuffer[MAX_SCRIPT_BUFFER] = L"";
		char szBuffer[MAX_SCRIPT_BUFFER] = "";

		::WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, szBuffer, MAX_PATH, NULL, NULL );

		FILE *file = NULL;
		file = fopen(szBuffer, "w");

		if( NULL == file )
		{
			ERRORMSG(L"�������� ����.!")
				return false;
		}

		fputc( 0xef, file );
		fputc( 0xbb, file );
		fputc( 0xbf, file );

		wcscpy(wszBuffer, L"-- lua header. UTF-8 ���ڵ� �ν��� ���� �� ���� ������ ������.");
		::WideCharToMultiByte( CP_UTF8, 0, wszBuffer, -1, szBuffer, MAX_SCRIPT_BUFFER, NULL, NULL);
		fprintf( file, "%s", szBuffer );

		fprintf( file, "\n" );
		fprintf( file, "\n" );

		_EST_OBJECT_::EventSceneData::iterator mit;
		mit = m_mapEventSceneData.begin();

		size_t sizeWrote = 0;
		while( mit != m_mapEventSceneData.end() )
		{
			//::MultiByteToWideChar( CP_ACP, 0, mit->second.c_str(), -1, wszBuffer, MAX_SCRIPT_BUFFER );
			//::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, MAX_SCRIPT_BUFFER, NULL, NULL);
			//fprintf( file, "%s", szBuffer );
			fprintf( file, "%s", mit->second.c_str() );

			fprintf( file, "\n" );
			fprintf( file, "\n" );

			++mit;
		}

		fclose(file);
	}

	return true;
}

bool CEventScriptParsing::Parsing( IN const char * pScript, IN const long iLength )
{
	const char* p = pScript;

	//{{begin} �Ѻ��� ��ũ��Ʈ �Ľ��� ������ �ʱ�ȭ �Ǿ�� �� ������..
	std::string strID;
	std::string strScript;
	// true : pScript ������ g_pEventScene: �̸�
	// false : pScript �� } �� ������ �Ǿ��ٸ�.
	bool bIsParsing = false;
	int iBlockCount = -1;
	//{{end} �Ѻ��� ��ũ��Ʈ �Ľ��� ������ �ʱ�ȭ �Ǿ�� �� ������..

	//{{begin} ���̵� �Ľ��� ������ �ʱ�ȭ �Ǿ�� �� ������(��ũ��Ʈ�� �Ľ̽����Ŀ� ����)
	std::string strIDName;
	bool bWaitforIDParsing = false;
	bool bBeginforIsIDParsing = false;
	//{{end} ���̵� �Ľ��� ������ �ʱ�ȭ �Ǿ�� �� ������(��ũ��Ʈ�� �Ľ̽����Ŀ� ����)

	while( (*p) != 0 ) // check EOF
	{
		// ��ũ��Ʈ �Ľ� ����üũ.
		if( false == bIsParsing &&	// ���� �Ľ����̰�
			(*p) == 'g' )	// �ҹ��� g �̸� üũ����.
		{
			std::string strOrg, strPar;
			strOrg = "g_pEventScene:";

			const char * pTemp = p;

			while( strOrg.size() > strPar.size() )
			{
				if( (*p) != 0 )
				{
					strPar += (*pTemp);
				}
				else
					break;	// while end for EOF

				++pTemp;
			}

			if( strOrg.compare(strPar) == 0 )
				bIsParsing = true;
		}

		// �Ľ̽����̸�..
		if( bIsParsing == true )
		{
			strScript += (*p);

			//{{begin} ��ũ��Ʈ �Ľ� ���� üũ
			if( (*p) == '{' ) 
			{	
				++iBlockCount; if( iBlockCount == 0 ) ++iBlockCount; 
			}
			else
			if( (*p) == '}' ) 
			{
				--iBlockCount;
			}
			//{{end} ��ũ��Ʈ �Ľ� ���� üũ

			//{{begin} EVENT_SCENE_ID �� üũ.
			if( iBlockCount == 1 && strID.empty() == true )	// ���̵� �Ľ� ����üũ
			{
				if( isupper((*p)) != 0 || islower((*p)) != 0 || (*p) == '_' ) // ��,�ҹ��� �Ǵ� '_' �ϰ��
				{
					strIDName += (*p);
				}
				else
				if( isspace((*p)) != 0 || 
					(*p) == ':' || 
					(*p) == '{' || 
					(*p) == '}' ||
					(*p) == '=' || 
					(*p) == ',' ||
					(*p) == '-'
					)
				{
					if( strIDName.empty() == false && strIDName.compare("EVENT_SCENE_ID") == 0 )
					{
						bWaitforIDParsing = true;
						strIDName.clear();
					}
				}
			}

			if( (*p) == '\"' ) // ���̵� �Ľ� ����,���� üũ.
			{
				if( bWaitforIDParsing == true && bBeginforIsIDParsing == true )	// ����ǥ(") �� ������ �κ��̸� ���ֱ�.
				{
					bWaitforIDParsing = false;
					bBeginforIsIDParsing = false;
				}

				if( bWaitforIDParsing == true && bBeginforIsIDParsing == false )
					bBeginforIsIDParsing = true;
			}

			if( bWaitforIDParsing == true && bBeginforIsIDParsing == true )
			{
				if( (*p) != '\"' )
					strID += (*p);
			}
			//{{end} EVENT_SCENE_ID �� üũ.

			if( iBlockCount == 0 ) // ��ũ��Ʈ �Ľ��� ������� ������ �Ľ�����.
			{
				bIsParsing = false;
				iBlockCount = -1;

				strScript += '\0';

				// Refresh �����ε� �Լ��� ���Ǳ� ������ ���� ���̵� ������ ������ �־��ش�.
				_EST_OBJECT_::EventSceneData::iterator mit;
				mit = m_mapEventSceneData.find(strID);
				if( mit != m_mapEventSceneData.end() )
					m_mapEventSceneData.erase(mit);

				// ��� ó���� �������� �־��ش�.
				m_mapEventSceneData.insert(std::make_pair(strID, strScript));

				strID.clear();
				strScript.clear();
			}
		}

		++p;
	}

	return true;
}