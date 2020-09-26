/**@file : X2BuffTempletManager.h
   @breif : ���� ���ø��� �����ϴ� �Ŵ��� Ŭ���� ���� ���
*/

#pragma once

/** @class : CX2BuffTempletManager
	@brief : ���� ���ø��� �����ϴ� �Ŵ��� Ŭ����
	@date : 2012/7/17/
*/
class CX2BuffTempletManager
{
public:
	static bool GetCombination( KLuaManager& luaManager_, OUT vector<UINT>& vecType_ );

public:
	static CX2BuffTempletManager* GetInstance()
	{
		if ( NULL == pInstance )
			pInstance = new CX2BuffTempletManager;
		
		return pInstance;
	}

	static void DestroyInstance() { SAFE_DELETE( pInstance ); }

private:
	static CX2BuffTempletManager* pInstance;

public:
	void OpenScriptFileForTemplet();
	void OpenScriptFileForFactor();

	void AddBuffTemplet_LUA();
	void AddBuffFactor_LUA();

	CX2BuffTempletPtr GetBuffTempletPtr( const BUFF_TEMPLET_ID eBuffTempletID_ ) const;
	CX2BuffFactorPtr GetBuffFactorPtr( const UINT uiBuffFactorNumber_ ) const;

	void GetBuffFactorPtrFromBuffFactorList( IN KLuaManager& luaManager_, OUT vector<CX2BuffFactorPtr>& vecBuffFactorPtr_, OUT vector<UINT>& vecBuffFactorID_ );

private:
	CX2BuffTempletManager()
	{}

private:

	map<BUFF_TEMPLET_ID, CX2BuffTempletPtr>	m_mapBuffTempletPtr;
	vector<CX2BuffFactorPtr>				m_vecBuffFactorPtr;		/// ������ ���ؼ� �߻��ϴ� ���� ���Ϳ�
	
};