#ifndef _CurseFilter_h_
#define _CurseFilter_h_
//#ifndef bool
//#define bool int
//#define false 0
//#define true (!false)
//#endif
//{{ 060103. kkurrung.
/*
--Ŀ�� ���Ϳ��� �Ϻ��� �κ� �߰� ����--
�Ϻ��� �߿� '��'�� '��' ������ ����� ����ϴ�.
���߿� '��'�� ���� ���� '��'���� �ٲ㼭 ����ϰų�
�ݴ�ε� ����ϴ� ��찡 �ֱ� ������ Ŀ�� ���Ϳ����� ��� 
'��' �� '��' �� '��'�� �ٲپ ���͸��Ѵ�.
�� �������̳� ���̳ʽ� �Ǵ� ���� �ݰ��� ���� ǥ�� ���ڷ� ��ȹ��
������ ����ϴ� ����鵵 �ֱ� ������ �̷��� '-' �� ������ ���ڵ���
��� �����ϰ� '��'�� '��'�� '��'�� ��ü�ؼ� ���͸� �Ѵ�.
*/
//}} kkurrung.

void LoadNickNameFilter( const char* szFilename );
bool IsNickNameString( const wchar_t* szStringSrc );

//extern void LoadCurses(const char *filename);
//extern bool IsCurse(const char *str);
//extern char *ConvertString(char *str, int max_len);


#endif
