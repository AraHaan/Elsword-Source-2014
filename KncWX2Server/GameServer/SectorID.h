#pragma once

#include <vector>
#include <KncSmartPtr.h>

#define MAX_SECTOR	255
#define INVALID_SECTOR 0xffffffff

typedef	__int32	SectorID;
typedef boost::shared_ptr< std::vector< SectorID > > SectorsPtr;

/*    32 bit ( 4 byte )

| ff ff ff ff | 

| 00 ff 00 00 | X pos index
| 00 00 ff 00 | Y pos index
| 00 00 00 ff | Z pos index

*/

namespace KSectorID
{
	int		ExtractX( SectorID iSectorID );
	int		ExtractY( SectorID iSectorID );
	int		ExtractZ( SectorID iSectorID );

    // iNowSector�� �����Ͽ� iNowSector�� ������ ���͵��� �������� ���ĵǾ� ��µȴ�. vecAdjacentSector�� ��¹��̴�.
	int		ExtractAdjacentSector( SectorID iNowSector, int iMaxX, int iMaxY, int iMaxZ, std::vector< SectorID >& vecAdjacentSector );

    // vecSrcSectorGroup�� vecDestSectorGroup�� ������������ ���ĵǾ� �־�� �Ѵ�.
    // �����, ExtractAdjacentSector�� ����� vecAdjacentSector�� �������� ���ĵȴ�.
    void    FindRemainder( const std::vector< SectorID >& vecSrcSectorGroup,
                           const std::vector< SectorID >& vecDestSectorGroup,
                           std::vector< SectorID >& vecSrcRemainder,
                           std::vector< SectorID >& vecDestRemainder );

	void	SetX( SectorID& iDestID, int iX );
	void	SetY( SectorID& iDestID, int iY );
	void	SetZ( SectorID& iDestID, int iZ );
	void	SetIndex( SectorID& iDestID, int iX, int iY, int iZ );
}
