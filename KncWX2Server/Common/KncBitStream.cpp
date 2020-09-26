#include "KncBitStream.h"
#include <Serializer/SerVector.h>

KncBitStream::KncBitStream(void)
{
}

KncBitStream::~KncBitStream(void)
{
}

bool KncBitStream::Get( size_t nIndex_ ) const
{
    size_t nVecIndex = nIndex_ / vec_element_bit_size; // ������ �ε���
    size_t nBitIndex = nIndex_ % vec_element_bit_size;

    if( size() <= nVecIndex ) return false;

    return (at( nVecIndex ) & (1 << nBitIndex)) > 0;
}

void KncBitStream::Set( size_t nIndex_ )
{
    size_t nVecIndex = nIndex_ / vec_element_bit_size;
    size_t nBitIndex = nIndex_ % vec_element_bit_size;

    if( size() <= nVecIndex ) resize( nVecIndex + 1 );

    (*this)[nVecIndex] |= (1 << nBitIndex);
    //operator[]( nVecIndex ) |= (1 << nBitIndex);
}

void KncBitStream::Reset( size_t nIndex_ )
{
    size_t nVecIndex = nIndex_ / vec_element_bit_size;
    size_t nBitIndex = nIndex_ % vec_element_bit_size;

    if( size() <= nVecIndex ) return;

    (*this)[nVecIndex] &= ~(1 << nBitIndex);
    //operator[]( nVecIndex ) |= (1 << nBitIndex);

    std::vector<value_type>::reverse_iterator vit;
    for( vit = rbegin(); vit != rend(); vit++ )
    {
        if( *vit != 0 ) // �ڿ������� ���ʷ� 0�� �ƴ� ���� ������ �Ǹ�
        {
            // ���� ����Ű�� ���� ��ĭ ��(iter/rev_iter�� ���̶����� �ٷ� ��ĭ�� ����Ű�� �ȴ�) ����
            // �����̳��� ������ ��� ����.
            erase( vit.base(), end() );
            break;
        }
    }

    if( vit == rend() )     // ���� ��� bit�� 0�� �� ���. ���� ���ǿ��� compaction ���� �ʴ´�.
        clear();
}

SERIALIZE_DEFINE_PUT( KncBitStream, obj, ks )
{
    return ks.Put( (std::vector<KncBitStream::vec_element_type>&)obj );
}

SERIALIZE_DEFINE_GET( KncBitStream, obj, ks )
{
    return ks.Get( (std::vector<KncBitStream::vec_element_type>&)obj );
}

void KncBitStream::Dump( OUT std::ostream& stm_ )
{
    stm_ << std::hex;
    std::copy( begin(), end(), std::ostream_iterator<int>( stm_, " " ) );
    stm_ << std::endl << std::dec;
}