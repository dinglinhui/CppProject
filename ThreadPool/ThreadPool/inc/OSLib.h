/*
 * LibApp.h
 *
 *  Created on: May 3, 2014
 *      Author: Kevin
 */

#ifndef OSLIB_H_
#define OSLIB_H_

#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SHR_MAX_FILE_SIZE 255

typedef enum
{
	rsbaudate_110 = 0,
	rsbaudate_300,
	rsbaudate_600,
	rsbaudate_1200,
	rsbaudate_2400,
	rsbaudate_4800,
	rsbaudate_9600,
	rsbaudate_19200,
	rsbaudate_38400,
	rsbaudate_57600,
	rsbaudate_115200,
} rsbaudate;

typedef enum
{
	rsparity_none = 0, rsparity_even, rsparity_odd, rsparity_space,
} rsparity;

typedef enum
{
	rsdatabit_5 = 0, rsdatabit_6, rsdatabit_7, rsdatabit_8,
} rsdatabit;

typedef enum
{
	rsstopbit_1 = 0, rsstopbit_1_5, rsstopbit_2,
} rsstopbit;

typedef struct
{
	int fBinary;
	rsbaudate baudrate;
	rsdatabit databit;
	rsstopbit stopbit;
	rsparity parity;
} rsport_attr;

#ifdef OS_WINDOWS

#include <winsock2.h>
#include <Windows.h>
#include <io.h>
#include <process.h>
#include <direct.h>

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <string.h>

/* �ļ�ӳ�� */
#define SHM_HANDLE		HANDLE

/* ���� */
#define PROS_HANDLE		HANDLE

/* ���� */
#define	TASK_ID			unsigned long
#define TASK_HANDLE		HANDLE
#define	TASK_FUN		unsigned int

/* ������ */
#define HMUTEX			CRITICAL_SECTION

/* �������� */
#define HCOND			HANDLE /* ���ź������ͬ�� */

/* �ź��� */
#define HSEM			HANDLE	/* ���� */
#define HNSEM			HANDLE	/* ���� */

/* �ܵ� */
#define PIPE_HANDLE		HANDLE

/* ��� */
#define SOCK_LEN		ST_INT
/* ���� */
#define HRSPORT			HANDLE

/* ʱ��ṹ */
typedef SYSTEMTIME lib_systime;

/* ʱ��ṹ */
typedef struct
{
	unsigned int ticksPerSec; //ʱ��Ƶ��
	unsigned int lastCheckTime;
	unsigned int wrapCount;
	double initMsTime; //��ʼʱ��
} lib_time_ms;

/**
 * @brief	��̬�⵼���ؼ��֡�
 */
#define SHR_DLL_EXPORT __declspec( dllexport )

/**
 * @brief	�������Եĳ������塣
 */
#define OUTPUTAPI __stdcall

/**
 * @name		SHR_THREAD_ROUTINE
 * @brief	�̴߳��������͵Ķ���
 */
typedef unsigned int (OUTPUTAPI *LIB_TASK_ROUTINE)(void *);

#define GTW_PATH_SPACE_MARK "\\"//Ŀ¼�������
#define GTW_CFG_PATH "./"		//����Ŀ¼ini·��
#define OSC_COMTRADE_PATH ".\\"	//�Ŷ�¼��COMTRADE·��
#endif

#ifdef OS_LINUX

#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/times.h>/* for struct tms */
#include <sys/timeb.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <netdb.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <dirent.h>

#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <dlfcn.h>
#include <termios.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

/* �ļ�ӳ�� */
#define SHM_HANDLE		int

/* ���� */
#define PROS_HANDLE		pid_t

/* ���� */
#define	TASK_ID			pthread_t
#define TASK_HANDLE		pthread_t
#define	TASK_FUN		void *

/* ������ */
#define HMUTEX			pthread_mutex_t

/* �������� */
#define HCOND			pthread_cond_t

/* �ź��� */
#define HSEM			int		/* ���� */
#define HNSEM			sem_t	/* ���� */

/* �ܵ� */
#define PIPE_HANDLE		int

/* ��� */
#define HANDLE			void*

/* ���� */
#define HRSPORT			int

/* �׽��� */
#define	SOCKET			int
#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1
#define SOCK_LEN		socklen_t

/* ʱ��ṹ */
typedef struct
{
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDayOfWeek;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned short wSecond;
	unsigned short wMilliseconds;
}lib_systime;

/* ʱ��ṹ */
typedef struct
{
	unsigned int ticksPerSec; //ʱ��Ƶ��
	unsigned int lastCheckTime;
	unsigned int wrapCount;
	double initMsTime;//��ʼʱ��
}lib_time_ms;

/**
 * @brief	��̬�⵼���ؼ��֣�Ϊ�˼��ݿ�ƽ̨��
 */
#define SHR_DLL_EXPORT

/**
 * @brief	�������Եĳ������塣
 */
#define OUTPUTAPI

/**
 * @name		SHR_THREAD_ROUTINE
 * @brief	�̴߳��������͵Ķ���
 */
typedef void *(*LIB_TASK_ROUTINE)(void *);

#define GTW_PATH_SPACE_MARK "/"	//Ŀ¼�������
#define GTW_CFG_PATH "/opt/"	//����Ŀ¼ini·��
#define OSC_COMTRADE_PATH "./"	//�Ŷ�¼��COMTRADE·��
#define INFINITE  0xFFFFFFFF  // Infinite timeout
#ifndef BOOL
#define BOOL unsigned char
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#endif /*OS_LINUX*/

#ifdef OS_MAC

#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/times.h>/* for struct tms */
#include <sys/timeb.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <netdb.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <dirent.h>

#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <dlfcn.h>
#include <termios.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

/* �ļ�ӳ�� */
#define SHM_HANDLE		int

/* ���� */
#define PROS_HANDLE		pid_t

/* ���� */
#define	TASK_ID			pthread_t
#define TASK_HANDLE		pthread_t
#define	TASK_FUN		void *

/* ������ */
#define HMUTEX			pthread_mutex_t

/* �������� */
#define HCOND			pthread_cond_t

/* �ź��� */
#define HSEM			int		/* ���� */
#define HNSEM			sem_t	/* ���� */

/* �ܵ� */
#define PIPE_HANDLE		int

/* ��� */
#define HANDLE			void*

/* ���� */
#define HRSPORT			int

/* �׽��� */
#define	SOCKET			int
#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1
#define SOCK_LEN		socklen_t

/* ʱ��ṹ */
typedef struct
{
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDayOfWeek;
	unsigned short wDay;
	unsigned short wHour;
	unsigned short wMinute;
	unsigned short wSecond;
	unsigned short wMilliseconds;
}lib_systime;

/* ʱ��ṹ */
typedef struct
{
	unsigned int ticksPerSec; //ʱ��Ƶ��
	unsigned int lastCheckTime;
	unsigned int wrapCount;
	double initMsTime;//��ʼʱ��
}lib_time_ms;

/**
 * @brief	��̬�⵼���ؼ��֣�Ϊ�˼��ݿ�ƽ̨��
 */
#define SHR_DLL_EXPORT

/**
 * @brief	�������Եĳ������塣
 */
#define OUTPUTAPI

/**
 * @name		SHR_THREAD_ROUTINE
 * @brief	�̴߳��������͵Ķ���
 */
typedef void *(*LIB_TASK_ROUTINE)(void *);

#define GTW_PATH_SPACE_MARK "/"	//Ŀ¼�������
#define GTW_CFG_PATH "/opt/"	//����Ŀ¼ini·��
#define OSC_COMTRADE_PATH "./"	//�Ŷ�¼��COMTRADE·��
#define INFINITE  0xFFFFFFFF  // Infinite timeout
#ifndef BOOL
#define BOOL unsigned char
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#endif /*OS_MAC*/

typedef struct lib_file_attri
{
	char name[SHR_MAX_FILE_SIZE + 1];	//�ļ�����
	time_t last_modified;					//����޸�ʱ��
	unsigned int size;							//�ļ���С
} SHR_FILE_ATTRI;

void lib_get_float32(unsigned char *pbuff, float *out);

//process

/**
 * @name		lib_process_create
 * @brief	����һ�����̡�
 * @param	phandle		���̾����
 * @param	pfilename	�������ơ�
 * @param	pcmdline	�����С�
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_process_create(PROS_HANDLE *phandle, char *pfilename, char *pcmdline);

/**
 * @name		lib_process_destroy
 * @brief	�ر�һ�����̡�
 * @param	handle		���̾����
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_process_destroy(PROS_HANDLE handle, int iSignal);

int lib_processV2_create(PROS_HANDLE *phandle, void (*pfun)(char *pname), char *pname);

//thread

/**
 * @name		lib_task_create
 * @brief	����һ���̡߳�Ĭ��״̬�ǿɻ��(join)�ģ��ȷǷ���(detach)״̬��
 * @param	phandle		�߳̾����ָ�롣
 * @param	pid			�߳�id��ָ�롣
 * @param	routine		�̴߳�������
 * @param	param		�̴߳������Ĳ���ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_task_create(char *pname, int priority, int stackSize, TASK_HANDLE *phandle, LIB_TASK_ROUTINE routine, void *param);

/**
 * @name		lib_task_detach
 * @brief	��һ���߳̽��з��룬�˺���߳̽���ʱ�Զ��ͷ���Դ�ȡ�
 * @param	phandle	�߳̾����
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_task_detach(TASK_HANDLE phandle);

/**
 * @name		lib_task_join
 * @brief	�����ڵȴ�һ���ɻ���̵߳Ľ�����
 * @param	phandle		�߳̾����
 * @param	*pretcode	�˳��롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_task_join(TASK_HANDLE phandle, void **pretcode);

/**
 * @name		lib_task_exit
 * @brief	�˳�һ���̵߳�ִ�С�
 * @param  retval	�˳��롣
 * @return	�ޡ�
 * @note
 */
void lib_task_exit(unsigned int retval);

//������

/**
 * @name		lib_mutex_init
 * @brief	��ʼ��һ���ɵݹ�Ļ�������
 * @param	mutex	�����������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		��������ǰ��һ��Ҫ������ʼ����
 */
int lib_mutex_init(HMUTEX *mutex);

/**
 * @name		lib_mutex_lock
 * @brief	������
 * @param	mutex	�����������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_mutex_lock(HMUTEX *mutex);

/**
 * @name		lib_mutex_unlock
 * @brief	������
 * @param	mutex	�����������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_mutex_unlock(HMUTEX *mutex);

/**
 * @name		lib_mutex_destroy
 * @brief	���ٻ�������
 * @param	mutex	�����������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		�������ú�һ��Ҫ�������١�
 */
int lib_mutex_destroy(HMUTEX *mutex);

//��������

/**
 * @name		lib_cond_init
 * @brief	��ʼ��һ������������
 * @param	cond	�������������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		����������ǰ��һ��Ҫ������ʼ����
 */
int lib_cond_init(HCOND *cond);

/**
 * @name		lib_cond_wait
 * @brief	�ȴ�һ�������������ȴ�ʱ��Ϊmilliseconds���롣
 * @param	mutex			�����������ָ�룬������֤�����������Ļ�����ʡ�
 * @param	cond			�������������ָ�롣
 * @param	milliseconds	�ȴ��ĺ�������Ϊ-1��ʾ�����ȴ���ֱ���������㡣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_cond_wait(HMUTEX *mutex, HCOND *cond, int milliseconds);

/**
 * @name		lib_cond_signal
 * @brief	�������ڵȴ��������������̣߳�����ж���߳��ڵȴ�����һ���߳�
 �����������̵߳ĵ��Ȳ����������ġ�
 * @param	cond	�������������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_cond_signal(HCOND *cond);

/**
 * @name		lib_cond_broadcast
 * @brief	�����������б���������������cond�ϵ��̣߳���Щ�̱߳����Ѻ��ٴξ���
 ��Ӧ�Ļ����������Ա���С��ʹ�����������
 * @param	cond	�������������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_cond_broadcast(HCOND *cond);

/**
 * @name		lib_cond_destroy
 * @brief	��������������
 * @param	cond	�������������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		���������ú�һ��Ҫ��������
 */
int lib_cond_destroy(HCOND *cond);

//�����ź���

/**
 * @name		lib_noname_sem_create
 * @brief	��ʼ��һ�������ź�����ֻ��Ϊ��ǰ���̵������̹߳���value�������ź���
 �ĳ�ʼֵ��
 * @param	sem		�ź��������ָ�롣
 * @param	value	�ź����ĳ�ʼֵ��
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		�ź�����ǰ��һ��Ҫ������ʼ����
 */
int lib_noname_sem_create(HNSEM *sem, unsigned int value);

/**
 * @name		lib_noname_sem_destroy
 * @brief	����һ�������ź�����
 * @param	sem		�ź��������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		�ź����ú�һ��Ҫ��������
 */
int lib_noname_sem_destroy(HNSEM *sem);

/**
 * @name		lib_noname_sem_wait
 * @brief	�ȴ�һ�������ź���������ź���ֵ<=0����һֱ�ȴ���ֱ��ֵ>0��
 * @param	sem		�ź��������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_noname_sem_wait(HNSEM *sem);

/**
 * @name		lib_noname_sem_post
 * @brief	�������������ź�����ֵ�������߳�����������ź�����ʱ�������������
 ��ʹ���е�һ���̲߳���������ѡ�����ͬ�������̵߳ĵ��Ȳ��Ծ����ġ�
 * @param	sem		�ź��������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_noname_sem_post(HNSEM *sem);

/**
 * @name		lib_noname_sem_getvalue
 * @brief	��������ź�����ֵ��
 * @param	sem		�ź��������ָ�롣
 * @return	>=0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_noname_sem_getvalue(HNSEM *sem);

//�����ź���

/**
 * @name		lib_name_sem_create
 * @brief	����һ�������ź�����max_numΪ�������ֵ��init_numΪ�ź����ĳ�ʼֵ��
 * @param	pfilename	�ļ����ơ�
 * @param	max_num	    �ź��������ֵ��
 * @param	init_num	�ź����ĳ�ʼֵ��
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		�ź�����ǰ��һ��Ҫ������ʼ����
 */
int lib_name_sem_create(char *pfilename, unsigned int max_val,
		unsigned int init_val, HSEM *phandle);

/**
 * @name		lib_name_sem_open
 * @brief	��һ�������ź�����
 * @param	pfilename	�ļ����ơ�
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		�ź�����ǰ��һ��Ҫ������ʼ����
 */
int lib_name_sem_open(char *pfilename, HSEM *phandle);

/**
 * @name		lib_name_sem_close
 * @brief	�ر�һ�������ź�����
 * @param	hSem	�ź��������
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		�ź����ú�һ��Ҫ��������
 */
int lib_name_sem_close(HSEM hSemObj);

/**
 * @name		lib_name_sem_destroy
 * @brief	����һ�������ź�����
 * @param	hSem	�ź��������
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		�ź����ú�һ��Ҫ��������
 */
int lib_name_sem_destroy(HSEM hSemObj);

/**
 * @name		lib_noname_sem_wait
 * @brief	�ȴ�һ�������ź���������ź���ֵ<=0����һֱ�ȴ���ֱ��ֵ>0��
 * @param	hSem			�ź��������
 * @param	dwMilliseconds	�ź��������
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_name_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds);

/**
 * @name		lib_noname_sem_wait
 * @brief	�ȴ������ź���������ź���ֵ<=0����һֱ�ȴ���ֱ��ֵ>0��
 * @param	hSem			�ź��������
 * @param	dwMilliseconds	�ź��������
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_beat_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds);

/**
 * @name		lib_name_sem_post
 * @brief	�������������ź�����ֵ�������߳�����������ź�����ʱ�������������
 ��ʹ���е�һ���̲߳���������ѡ�����ͬ������OS�ĵ��Ȳ��Ծ����ġ�
 * @param	hSem	�ź��������
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_name_sem_post(HSEM hSemObj);

/**
 * @name		lib_beat_sem_post
 * @brief	�������������ź�����ֵ,����ź���ֵ>0����ֱ�ӷ��سɹ�������ִ��semop����+1

 * @param	hSem	�ź��������
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_beat_sem_post(HSEM hSemObj);

/**
 * @name		lib_name_sem_getvalue
 * @brief	��������ź�����ֵ��
 * @param	hSem	�ź��������
 * @return	>=0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_name_sem_getvalue(HSEM hSemObj);

/**
 * @name		lib_shm_create
 * @brief	���������ڴ档
 * @param	pfilename �ļ����ơ�
 * @param	max_size  �ڴ��С��
 * @return	-1ʧ�ܣ�0�ɹ�
 * @note
 */
int lib_shm_create(char *pfilename, unsigned int max_size, SHM_HANDLE *phandle);

/**
 * @name		lib_shm_open
 * @brief	�򿪹����ڴ档
 * @param	pfilename �ļ����ơ�
 * @return	-1ʧ�ܣ�0�ɹ�
 * @note
 */
int lib_shm_open(char *pfilename, SHM_HANDLE *phandle);

/**
 * @name		lib_shm_map
 * @brief	ӳ�乲���ڴ档
 * @param	hShmObj	  �ڴ�����
 * @return	NULLʧ�ܣ������ɹ���
 * @note
 */
void* lib_shm_map(SHM_HANDLE hShmObj);

/**
 * @name		lib_shm_unmap
 * @brief	�رչ����ڴ档
 * @param	hMapAddr �ڴ��ַ��
 * @return	0�ɹ�������ʧ�ܡ�
 * @note
 */
int lib_shm_unmap(void *hMapAddr);

/**
 * @name		lib_shm_close
 * @brief	ɾ�������ڴ档
 * @param	hMapObj ӳ�����
 * @return	0�ɹ�������ʧ�ܡ�
 * @note
 */
int lib_shm_close(SHM_HANDLE hMapObj);

/**
 * @name		lib_shm_destroy
 * @brief	ɾ�������ڴ档
 * @param	hMapObj ӳ�����
 * @return	0�ɹ�������ʧ�ܡ�
 * @note
 */
int lib_shm_destroy(SHM_HANDLE hMapObj);

//0 success; -1 error
//int lib_pipe_init(PIPE_HANDLE *pipeRead,
//				  PIPE_HANDLE *pipeWrit,
//				  ST_UINT32 uMessages,
//				  ST_UINT32 uMsgBytes
//				  );

//0 success; -1 error
//int lib_pipe_read(PIPE_HANDLE *pipeRead, ST_CHAR *pBuff, ST_UINT32 uLen);

//0 success; -1 error
//int lib_pipe_write(PIPE_HANDLE *pipeWrit, ST_CHAR *pBuff, ST_UINT32 uLen);

//0 success; -1 error
//int lib_pipe_destroy(PIPE_HANDLE *pipeRead, PIPE_HANDLE *pipeWrit);

//����������

/**
 * @name		lib_load_library
 * @brief	����һ������⣬�硰libmytest.so����Ҫָ�����������·����
 * @param	filename	��������ơ�
 * @return	��NULLΪ�ɹ���NULLΪʧ�ܡ�
 * @note		ʹ�ù����ǰ���������ظÿ⡣
 */
HANDLE lib_load_library(const char *filename);

/**
 * @name		*lib_get_addr
 * @brief	�ӹ��������һ��ģ�飬�纯���ĵ�ַ�ȡ�
 * @param	handle	���������������xj_load_library()���صľ����
 * @param	module	ģ��ָ�롣
 * @return	��NULLΪ�ɹ���NULLΪʧ�ܡ�
 * @note
 */
void *lib_get_addr(HANDLE handle, const char *module);

/**
 * @name		lib_free_library
 * @brief	�ͷ�һ������⡣
 * @param	handle	���������
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note		ʹ�ù����⣬�����ͷŸÿ⡣
 */
int lib_free_library(HANDLE handle);

/**
 * @name		*lib_get_liberror
 * @brief	��ò��������Ĵ���������
 * @return	����������
 * @note
 */
char *lib_get_liberror();

/**
 * @name		lib_sleep
 * @brief	˯�ߺ�����
 * @param	milliseconds	˯�ߵĺ�����Ŀ������>=0��
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_sleep(unsigned int milliseconds);

/**
 * @name		lib_getlocaltime
 * @brief	ȡϵͳʱ�亯����
 * @param	psystime	���ʱ�仺������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_getlocaltime(lib_systime *psystime);

/**
 * @name		lib_setlocaltime
 * @brief	�޸�ϵͳʱ�亯����
 * @param	psystime	���ʱ�仺������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_setlocaltime(const lib_systime *psystime);

/**
 * @name		lib_getcurrenttime
 * @brief	��ȡ��ǰ����ͺ��룬ʱ����1970.1.1.0.0.0��ʼ��
 * @param	pSeconds	  ����뻺������ָ�롣
 * @param	pMilliSeconds ��ź��뻺������ָ�롣
 * @return	0Ϊ�ɹ�������Ϊ��������롣
 * @note
 */
int lib_getcurrenttime(time_t *pSeconds, unsigned short *pMilliSeconds);

/**
 * @name		lib_time_init
 * @brief	��ʼ��ʱ�������
 * @param	ptTime	ʱ�������ָ�롣
 * @return	0Ϊ�ɹ���-1Ϊ����
 * @note
 */
int lib_time_init(lib_time_ms *ptTime);

/**
 * @name		_lib_time_get
 * @brief	��ȡ����ʱ�䡣
 * @param	ptTime	ʱ�������ָ�롣
 * @return	���صĺ�������
 * @note
 */
double _lib_time_get(lib_time_ms *ptTime);

/**
 * @name		lib_create_dir
 * @brief	�ּ�����Ŀ¼��
 * @param	pdir	Ŀ¼���ơ�
 * @return	0Ϊ�ɹ�����0Ϊʧ�ܡ�
 * @note
 */
int lib_create_dir(const char *pdir);

/**
 * @name		lib_pathfile_exist
 * @brief	�ж��ļ���Ŀ¼�Ƿ���ڡ�
 * @param	pathfile	�ļ���Ŀ¼���ơ�
 * @return	0Ϊ���ڣ���0Ϊ�����ڡ�
 * @note
 */
int lib_pathfile_exist(const char *pathfile);

/**
 * @name		lib_pathfile_exec
 * @brief	�ж϶��ļ���Ŀ¼�Ƿ���ִ��Ȩ�ޡ�
 * @param	pathfile	�ļ���Ŀ¼���ơ�
 * @return	0Ϊ�У���0Ϊ�ޡ�
 * @note
 */
//int lib_pathfile_exec(const char *pathfile);
/**
 * @name		lib_pathfile_rw
 * @brief	�ж϶��ļ���Ŀ¼�Ƿ��ж�дȨ�ޡ�
 * @param	pathfile	�ļ���Ŀ¼���ơ�
 * @return	0Ϊ�У���0Ϊ�ޡ�
 * @note
 */
//int lib_pathfile_rw(const char *pathfile);
/**
 * @name		lib_get_file_size
 * @brief	���ָ���ļ��Ĵ�С����λ��byte��
 * @param	file_name	�ļ����ơ�
 * @return	>=0Ϊ�ļ���ʵ�ʴ�С��-1Ϊʧ�ܡ�
 * @note
 */
int lib_get_file_size(const char *file_name);

/**
 * @name		lib_get_filenum_of_dir
 * @brief	���ָ��Ŀ¼�µ��ļ���Ŀ��
 * @param	pathfile	Ŀ¼�ľ���·����
 * @return	>=0Ϊ�ļ���Ŀ��-1Ϊʧ�ܡ�
 * @note
 */
int lib_get_filenum_of_dir(const char *pathfile);

/**
 * @name		lib_get_filelist_of_dir
 * @brief	���ָ��Ŀ¼�µ��ļ������б�
 * @param	pathfile	Ŀ¼�ľ���·����
 * @param	pfilelist	�ļ������б�
 * @param	maxnum		�б������Ŀ��
 * @return	>=0Ϊ�б���Ŀ��-1Ϊʧ�ܡ�
 * @note
 */
int lib_get_filelist_of_dir(const char *pathfile, SHR_FILE_ATTRI *pfilelist,
		int maxnum);

/**
 * @name		lib_rsport_open
 * @brief	��ָ���Ĵ��ڡ�
 * @param	nr ���ںš�
 * @return	��NULL�ɹ���NULLʧ�ܡ�
 * @note
 */
HRSPORT lib_rsport_open(unsigned int nr);

/**
 * @name		lib_rsport_close
 * @brief	��ָ���Ĵ��ڡ�
 * @param	rsport ���ھ����
 * @return	0�ɹ�����0ʧ�ܡ�
 * @note
 */
int lib_rsport_close(HRSPORT rsport);

/**
 * @name		lib_rsport_setup
 * @brief	����ָ�����ڵĲ����ʡ�����λ��ֹͣλ�����ԡ�
 * @param	rsport ���ھ����
 * @param	attr   ����ָ�롣
 * @return	0�ɹ�����0ʧ�ܡ�
 * @note
 */
int lib_rsport_setup(HRSPORT rsport, const rsport_attr* pattr);

/**
 * @name		lib_rsport_set_timeout
 * @brief	����ָ�����ڵĶ�д��ʱ�����
 * @param	rsport      ���ھ����
 * @param	rxTimeout   ����ʱ����λ�����롣
 * @param	txTimeout   д��ʱ����λ�����롣
 * @return	0�ɹ�����0ʧ�ܡ�
 * @note
 */
int lib_rsport_set_timeout( HRSPORT rsport, unsigned int rxTimeout,
		unsigned int txTimeout);

/**
 * @name		lib_rsport_write
 * @brief	��ָ������rsportд�����ݡ�
 * @param	rsport           ���ھ����
 * @param	pbuff			 д��������
 * @param	bytes_to_write   д�ֽ�����
 * @return	>=0д����ֽ�����<0ʧ�ܡ�
 * @note
 */
int lib_rsport_write(HRSPORT rsport, const char* pbuff, int bytes_to_write);

/**
 * @name		lib_rsport_read
 * @brief	��ָ������rsport��ȡ���ݡ�
 * @param	rsport           ���ھ����
 * @param	pbuff			 ����������
 * @param	bytes_to_read    ���ֽ�����
 * @return	>=0��ȡ���ֽ�����<0ʧ�ܡ�
 * @note
 */
int lib_rsport_read(HRSPORT rsport, char* pbuff, int bytes_to_read);

/**
 * @name		lib_rsport_purge
 * @brief	���ָ������rsport�Ķ�д��������
 * @param	rsport      ���ھ����
 * @param	read		��ն���־��
 * @param	write		���д��־��
 * @return	0�ɹ�����0ʧ�ܡ�
 * @note
 */
int lib_rsport_purge(HRSPORT rsport, int read, int write);

/**
 * @name		lib_modify_time
 * @brief	�����ں�ʱ����к��뼶���޸ģ������Ӻͼ��������㡣
 * @param	psystime		���޸����ں�ʱ���ָ�룬Ĭ��ֵΪNULL��
 * @param	wMilliseconds	�����޸ĵĺ�������Ĭ��ֵΪ0��
 * @param	op				���㣺'+'��'-'��Ĭ��Ϊ'+'��
 * @return	0Ϊ�ɹ�����0Ϊʧ�ܡ�
 * @note		ֻ���޸�1900��1��1��0ʱ0��0��֮���ʱ�䡣
 */
int lib_modify_time(lib_systime *psystime, unsigned short wMilliseconds,
		char op);

/**
 * @name		shrtime_to_timet
 * @brief	��ϵͳʱ��ת��Ϊ�롣
 * @param	psystime		��ת��ʱ���ָ�룬Ĭ��ֵΪNULL��
 * @return	��ת�����롣
 * @note		��
 */
time_t shrtime_to_timet(lib_systime *psystime);

/**
 * @name		lib_time_get
 * @brief	��ȡ���ʱ�䡣
 * @param	ptTime	ʱ�������ָ�롣
 * @return	���صĺ�������
 * @note
 */
double lib_time_get(lib_time_ms *ptTime);

#ifdef __cplusplus
}
#endif

#endif /* LIBAPP_H_ */
