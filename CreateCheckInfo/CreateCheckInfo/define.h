

#if defined(TAIWAN)
#define PATCH_ORG_SRC       "http://elsword.nefficient.co.kr/elsword/SERVICE/PATCH/"	// �ؽ� ��ġ ���
#define PATCH_TEST_SRC		"http://210.208.90.241/Patch/"                  // �븸�系����
#else
#if defined (_OPEN_TEST_)
#	define PATCH_ORG_SRC       "http://elsword-nexon17.ktics.co.kr/BETA/Patch/"	// �����׼�
#elif defined (_SERVICE_)
#	define PATCH_ORG_SRC       "http://elsword-nexon17.ktics.co.kr/SERVICE/PATCH/"	// ����
#else
#	define PATCH_ORG_SRC       "http://elsword-nexon17.ktics.co.kr/SERVICE/PATCH/"	//  �ؽ� ��ġ ���
#endif
#define PATCH_TEST_SRC		"http://sekinternal.kog.co.kr/Patch/"                  // �����系����
#endif 


#if defined (KR_SERVICE_PATCHER)
#	define PATCH_PATH_FILE			"PatchPath.dat"     // ����
#elif defined (KR_OPENTEST_PATCHER)
#	define PATCH_PATH_FILE			"PatchPathBeta.dat"   // �����׼�
#else
#	define PATCH_PATH_FILE			"PatchPath_.dat"     // �׽�Ʈ 
#endif


#define MAX_RETRY           64                          // �ִ� ��õ� Ƚ��
#define MAX_DELAY           128                         // ��õ� ���� ����


#define LOAD_CUR_FILE_BAR_X			72
#define LOAD_CUR_FILE_BAR_Y			285
#define LOAD_CUR_FILE_BAR_WIDTH		373 - 5
#define LOAD_CUR_FILE_BAR_HEIGHT	473 - 462

#define LOAD_CUR_FILE_PER_X			449
#define LOAD_CUR_FILE_PER_Y			285
#define LOAD_CUR_FILE_PER_WIDTH		60
#define LOAD_CUR_FILE_PER_HEIGHT	30

#define LOAD_ALL_FILE_BAR_X			72
#define LOAD_ALL_FILE_BAR_Y			305
#define LOAD_ALL_FILE_BAR_WIDTH		373 - 5
#define LOAD_ALL_FILE_BAR_HEIGHT	473 - 462

#define LOAD_ALL_FILE_PER_X			449
#define LOAD_ALL_FILE_PER_Y			305
#define LOAD_ALL_FILE_PER_WIDTH		60
#define LOAD_ALL_FILE_PER_HEIGHT	30
