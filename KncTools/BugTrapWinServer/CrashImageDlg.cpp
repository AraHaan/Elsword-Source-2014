
#include "stdafx.h"
#include "BugTrapWinServer.h"
#include "CrashImageDlg.h"

#define HIMETRIC_INCH        2540

// CCrashImageDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCrashImageDlg, CDialog)
CCrashImageDlg::CCrashImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrashImageDlg::IDD, pParent)
{
    mp_image_object = NULL;
    m_width_size = m_height_size = 0;

}

CCrashImageDlg::~CCrashImageDlg()
{
    if(mp_image_object != NULL) mp_image_object->Release();
}

void CCrashImageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PIC, m_pic);

    
}


BEGIN_MESSAGE_MAP(CCrashImageDlg, CDialog)
    ON_WM_PAINT()

END_MESSAGE_MAP()

void CCrashImageDlg::OnPaint()
{
    Draw();
}


char CCrashImageDlg::LoadImage(const char *parm_path)
{
    // ������ �׸��� �о ��ü�� ������� �ִٸ� �ش� ��ü�� �����Ѵ�.
    if(mp_image_object != NULL) mp_image_object->Release();

    char ok_flag = 0;
    // ������ �׸������� CreateFile �Լ��� ����Ͽ� ����.
    HANDLE h_file = CreateFile(parm_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(INVALID_HANDLE_VALUE != h_file){
        // ������ ũ�⸦ ��´�.
        int file_size = GetFileSize(h_file, NULL);
        if(-1 != file_size){
            // ������ ũ�⸸ŭ ���� �޸𸮸� �Ҵ��Ѵ�.
            HGLOBAL h_global = GlobalAlloc(GMEM_MOVEABLE, file_size);
            if(NULL != h_global){
                // �ڵ� ���·� �Ҵ�� ���� �޸𸮸� ����ϱ� ���ؼ� ���� �����͸� ��´�.
                void *p_data = GlobalLock(h_global);
                if(NULL != p_data){
                    unsigned long read_size = 0;
                    // �׸� ���� ������ �о� ���δ�.
                    ReadFile(h_file, p_data, file_size, &read_size, NULL);
                    GlobalUnlock(h_global);

                    LPSTREAM p_stream = NULL;
                    // �о���� ���������� �̿��Ͽ� ��Ʈ�� ��ü�� �����Ѵ�.
                    HRESULT h_result = CreateStreamOnHGlobal(h_global, TRUE, &p_stream);
                    if(SUCCEEDED(h_result) && p_stream){
                        // ��Ʈ�� ��ü�� ������ �������� �׸�(Picture) ��ü�� �����Ѵ�.
                        // ������ �׸� ��ü�� �����ʹ� mp_image_object�� ����ȴ�.
                        ::OleLoadPicture(p_stream, file_size, FALSE,
                            IID_IPicture, (LPVOID *)&mp_image_object);
                        // ��Ʈ�� ��ü�� �����Ѵ�.
                        p_stream->Release();
                        if(SUCCEEDED(h_result) && mp_image_object){
                            // �׸���ü�� ���������� �����Ǿ��ٸ� �׸� ũ�������� ��´�
                            mp_image_object->get_Width(&m_width_size);
                            mp_image_object->get_Height(&m_height_size);
                        }
                        ok_flag = 1;
                    }
                }
            }
            GlobalFree(h_global);
        }
        CloseHandle(h_file);
    }
    return ok_flag;


}

void CCrashImageDlg::Draw()
{
    HRESULT hr;
    //CClientDC dc(&m_pic);
    CDC *dc = m_pic.GetDC();
    

    // �׸��� ����� ���� ��ǥ�� (100, 120)�̰� ���� ���̸� �̹����� ũ�⿡ ���� ����Ѵ�.
    int x_size = MulDiv(m_width_size, dc->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
    int y_size = MulDiv(m_height_size, dc->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);

    
    hr = mp_image_object->Render(dc->m_hDC, 10, 10, x_size, y_size, 0,
        m_height_size, m_width_size, -m_height_size, NULL);

    m_pic.Invalidate();
    // ����� �׸� ��ü�� �����ϱ� ���ؼ� �ش� Ŭ������ ��ü �ı��ڳ� WM_DESTROY�� �Ʒ��� ����
    // �ڵ带 �߰��ϸ� �ȴ�.
    //if(mp_image_object != NULL) mp_image_object->Release();

}

// CCrashImageDlg �޽��� ó�����Դϴ�.
