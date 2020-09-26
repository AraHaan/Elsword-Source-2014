// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� ����� 
// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� _EXPORTS ��ȣ��
// �����ϵǸ�, �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������ 
// NETMARBLESIGNCHECKER_API �Լ��� DLL���� �������� ������ ����, �� DLL��
// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.
#ifdef NETMARBLESIGNCHECKER_EXPORTS
#define NETMARBLESIGNCHECKER_API __declspec(dllexport)
#else
#define NETMARBLESIGNCHECKER_API __declspec(dllimport)
#endif

class CNetmarbleSignCheckerImpl ;
// �� Ŭ������ NetmarbleSignChecker.dll���� ������ ���Դϴ�.
class NETMARBLESIGNCHECKER_API CNetmarbleSignChecker {
public:
	CNetmarbleSignChecker(void);
	~CNetmarbleSignChecker() ; 
	// TODO: ���⿡ �޼��带 �߰��մϴ�.
	// ServerString , password string must be UTF-8 string.
	void parseParameter( char *ServerString , char *password ) ; 
	bool isSignIn() ;
	char *getValue(char *token , char *name) ; 
	int  getErrorCode();
	char *getErrorMessage() ; 

private:
	CNetmarbleSignCheckerImpl *impl_;
};

extern NETMARBLESIGNCHECKER_API int nNetmarbleSignChecker;

NETMARBLESIGNCHECKER_API int fnNetmarbleSignChecker(void);
