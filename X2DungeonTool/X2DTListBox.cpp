#include "dxstdafx.h"
#include ".\x2dtlistbox.h"

KX2DTListBox::KX2DTListBox( CDXUTDialog *pDialog ) : CDXUTListBox( pDialog )
{
}

KX2DTListBox::~KX2DTListBox(void)
{
}

void KX2DTListBox::RemoveAllItems( bool bIsRemoveSelectd )
{
	for( int i = 0; i < m_Items.GetSize(); ++i )
	{
		DXUTListBoxItem *pItem = m_Items.GetAt( i );
		delete pItem;
	}

	m_Items.RemoveAll();
	m_ScrollBar.SetTrackRange( 0, 1 );

	// ��� ����Ʈ ������ ���� �� ���õ� �ε����� -1�� 0���� ���� �ʱ�ȭ (ũ������ ����)
	if( bIsRemoveSelectd )
	{
		m_nSelected = -1;
		m_nSelStart = 0;
	}
}

void KX2DTListBox::LastItemSelect()
{
    m_nSelected = m_Items.GetSize() - 1;
	m_nSelStart = m_nSelected;
}