#pragma once

#include <string>

//-----------------------------------------------------------------------------
inline int KFindFirstNotOf(const char* pcSrc, const char* pcDest)
{
	unsigned int uiSrcLen = strlen(pcSrc);
	unsigned int uiDestLen = strlen(pcDest);

	unsigned int iC;
	for (iC = 0;; ++iC)
	{
		if (iC >= uiSrcLen || iC >= uiDestLen)
			break;

		if (pcSrc[iC] != pcDest[iC])
			return iC;
	}

	return iC;
}
//-----------------------------------------------------------------------------
inline void KRemoveBlankString(std::string& strChunk)
{
	int iPos;

	// ������ ���� ����
	iPos = strChunk.find_last_not_of(" \t\v\n");
	if (iPos != std::string::npos)
		strChunk.replace(iPos + 1, strChunk.length() - iPos, "");

	// ���� ���� ����
	iPos = strChunk.find_first_not_of(" \t\v\n");
	if (iPos != std::string::npos)
		strChunk.replace(0, iPos, "");
}
//-----------------------------------------------------------------------------
