#pragma once

#include <KNCSingleton.h>
#include <set>
#include <map>
#include "Lottery.h"
#include "CommonPacket.h"

////////////////////////////////////////
//2013.10.22 lygan_������ // DB���� Eventdata.lua�� Rewardtable.lua ���� �о� ���� �ϴ� �۾��� ���� GameEventScriptManager Ŭ���� �״�� ���ͼ� ������
////////////////////////////////////////
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
class KGameEventDBManager
{

	DeclareSingleton( KGameEventDBManager );


public:
	KGameEventDBManager(void);
	~KGameEventDBManager(void);


	const EVENT_DATA* GetEventDBData( IN int iScriptID ) const;

	bool			 SetEventDBData(std::map<int, std::string> mapEventDBData);

	const std::map< int, EVENT_DATA >& GetMapEventDBData() const { return m_mapEventDBData; }

	std::string		MakeUpperCase(std::string str);
	

	
protected:

	std::vector<std::string> split ( const std::string& s, const std::string& delim, const bool keep_empty = true);

	int str2AsciiInt(const std::string string2Ascii);

private:

	std::map< int, EVENT_DATA >				m_mapEventDBData;

};

DefSingletonInline( KGameEventDBManager );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
