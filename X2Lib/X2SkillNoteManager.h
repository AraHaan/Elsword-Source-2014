#pragma once

/*
	#���� ���õ� ĳ���� Ŭ������ �޸� ����Ʈ�� ���� �ϴ� Ŭ����
*/
#ifdef REFORM_SKILL_NOTE_UI
class CX2SkillNoteManager
{
public:
	struct MemoData
	{
		int iMemoItemID;
		int iSkillID;
		int iMemoItemNum;	// �޸� ������ ������
		char cClassLine;		// ������ ���� Ŭ���� ���� ����
			// ����~2�� (0~2)
	};

	// �޸� ����Ʈ ���� Ŭ����
	struct MemoListSort : public std::binary_function< MemoData, MemoData, bool >
	{
		bool operator()( const MemoData& _Left, const MemoData& _Right ) const
		{
			if( (_Left.iMemoItemNum > 0) == (_Right.iMemoItemNum > 0) )
			{
				// ���� ���� �� �켱
				if( _Left.cClassLine < _Right.cClassLine )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if ( (_Left.iMemoItemNum > 0) > (_Right.iMemoItemNum > 0) )
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};

	struct MemoListClassSort : public std::binary_function< MemoData, MemoData, bool >
	{
		bool operator()( const MemoData& _Left, const MemoData& _Right ) const
		{
			// ���� ���� �� �켱
			if( _Left.cClassLine < _Right.cClassLine )
			{
				return true;
			}
			else
			{
				return false;
			}
		};
	};
public:
	static CX2SkillNoteManager* GetInstance();
	static void ResetSkillNoteManager();
	static void DestroyInstance();
	///////////////////////////////////////////////////////

	// ��ũ��Ʈ �Ľ� ����
	void AddSkillNoteTemplet_LUA();
	bool OpenScriptFile();

	// Ŭ���� ����
	void SetUnitClass( CX2Unit::UNIT_CLASS eUnitClass_ ) { m_eUnitClass = eUnitClass_; }
	CX2Unit::UNIT_CLASS GetUnitClass() const { return m_eUnitClass; } 

	// �޸� ����Ʈ ����
	void ResetMemoList(); 
	void UpdateMemoPossession();
	void UpdateSortMemoList();

	// �޸� ����Ʈ ����
	const vector<MemoData>& GetUsableMemoListIfIHave(){ return m_vecUsableMemoListIfIhave; }
	int GetSkillIDByMemoItemID( int iMemoItemID_, bool bIsMySkill_ = true ) const;

	// ~End Public Function
private:

	// ~End Private Function
private:
	CX2SkillNoteManager();
	~CX2SkillNoteManager();
	
	static CX2SkillNoteManager*							m_pManager;
	//////////////////////////////////////////////////////

	// ���� ���õ� ĳ������ ���� Ŭ����
	CX2Unit::UNIT_CLASS									m_eUnitClass; 

	// key : UnitClass,  val : ( �޸������ID, ��ųID)	
	map< CX2Unit::UNIT_CLASS, vector<pair<int, int>> >	m_mapSkillNoteTemplet;	

	// (���� ���� ��)���� ���õ� ĳ������ Ŭ������ ��� ������ �޸� ����Ʈ	
	vector<MemoData>									m_vecUsableMemoListIfIhave;	
};

#endif // REFORM_SKILL_NOTE_UI

