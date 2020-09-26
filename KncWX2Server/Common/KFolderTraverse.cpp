#include "KFolderTraverse.h"

#include <algorithm>

//---------------------------------------------------------------------------
void KSetCurFolder(WCHAR* pcName)
{
    SetCurrentDirectory(pcName);

    //char acCurPath[_MAX_PATH] = {0};
    //GetCurrentDirectory(_MAX_PATH, acCurPath);
    //cout << acCurPath << endl;
}
//---------------------------------------------------------------------------
std::wstring KGetCurFolder()
{
	WCHAR acCurPath[_MAX_PATH] = {0};
	GetCurrentDirectory(_MAX_PATH, acCurPath);

	return acCurPath;
}
//---------------------------------------------------------------------------
std::wstring KMakeAbsolutePath(WCHAR* pcFile)
{
    WCHAR acCurPath[_MAX_PATH] = {0};
    GetCurrentDirectory(_MAX_PATH, acCurPath);

    std::wostringstream os;
    os << acCurPath << "\\" << pcFile;
    return os.str();
}
//---------------------------------------------------------------------------
void KDeleteFolder(WCHAR* strPath)
{
    // ���� ��η� ��ȯ
    KSetCurFolder(strPath);
    WCHAR acCurPath[_MAX_PATH] = {0};
    GetCurrentDirectory(_MAX_PATH, acCurPath);

    // ����� ����/���� ����
    WIN32_FIND_DATA	wfd;
    HANDLE hFile = FindFirstFile(L"*.*", &wfd);

    while (FindNextFile(hFile, &wfd))
    {
        if (wcscmp(wfd.cFileName, L"..") == 0 || wcscmp(wfd.cFileName, L".") == 0)
            continue;

        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            KDeleteFolder(wfd.cFileName);
        }
        else
        {
            DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
        } 
    }

    if(hFile != INVALID_HANDLE_VALUE)
        FindClose(hFile);

    KSetCurFolder(L"..");
    RemoveDirectory(acCurPath);
}
//---------------------------------------------------------------------------