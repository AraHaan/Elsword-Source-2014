#pragma once
#include "ServerDefine.h"
#include <map>


//{{ 2012. 10. 04	������		ED&������ ����͸� �ý���
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM


class KItemQuantityVariation
{
public:
    KItemQuantityVariation();
    virtual ~KItemQuantityVariation();

	void Clear();
    void IncreaseQuantity( const int iItemID, const int iQuantity );
	void DecreaseQuantity( const int iItemID, const int iQuantity );
	
protected:
	std::map< int, int >	m_mapIncreaseQuantity;
	std::map< int, int >	m_mapDecreaseQuantity;
};


#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}