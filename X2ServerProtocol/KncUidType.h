#pragma once

typedef __int64 UidType;

/*    64 bit ( 8 byte )

| ff ff ff ff | ff ff ff ff |

| 00 00 00 ff | ff ff ff ff | ���� uid ����. ( 5byte = 40bit. 2^40 = 1 0995 1162 7776 )
| 80 00 00 00 | 00 00 00 00 | ��ȣ��Ʈ�� ���ܵ�
| 40 00 00 00 | 00 00 00 00 | ���ÿ��� �ӽ÷� ����Ǵ� uid.
| 3f 00 00 00 | 00 00 00 00 | ������ uid.
| 00 ff 00 00 | 00 00 00 00 | ����(ä��) uid.
| 00 00 ff 00 | 00 00 00 00 | reserved.

*/

namespace KncUid {

    unsigned int    Die32();
    UidType         GetTempUID();
    UidType         ExtractPureUID( UidType iUID_ );
    UidType         ExtractServerGroupID( UidType iUID_ );
    UidType         ExtractServerID( UidType iUID_ );
    UidType         ExtractReservedID( UidType iUID_ );
    void            SetPureUID( UidType& iDestUID_, UidType iSrcUID_ );
    void            SetServerGroupID( UidType& iDestUID_, UidType iSrcUID_ );
    void            SetServerID( UidType& iDestUID_, UidType iSrcUID_ );
    void            SetReservedID( UidType& iDestUID_, UidType iSrcUID_ );
}