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

/* 文件映射 */
#define SHM_HANDLE		HANDLE

/* 进程 */
#define PROS_HANDLE		HANDLE

/* 任务 */
#define	TASK_ID			unsigned long
#define TASK_HANDLE		HANDLE
#define	TASK_FUN		unsigned int

/* 互斥锁 */
#define HMUTEX			CRITICAL_SECTION

/* 条件变量 */
#define HCOND			HANDLE /* 用信号量完成同步 */

/* 信号量 */
#define HSEM			HANDLE	/* 有名 */
#define HNSEM			HANDLE	/* 无名 */

/* 管道 */
#define PIPE_HANDLE		HANDLE

/* 句柄 */
#define SOCK_LEN		ST_INT
/* 串口 */
#define HRSPORT			HANDLE

/* 时间结构 */
typedef SYSTEMTIME lib_systime;

/* 时间结构 */
typedef struct
{
	unsigned int ticksPerSec; //时钟频率
	unsigned int lastCheckTime;
	unsigned int wrapCount;
	double initMsTime; //初始时间
} lib_time_ms;

/**
 * @brief	动态库导出关键字。
 */
#define SHR_DLL_EXPORT __declspec( dllexport )

/**
 * @brief	函数属性的常量定义。
 */
#define OUTPUTAPI __stdcall

/**
 * @name		SHR_THREAD_ROUTINE
 * @brief	线程处理函数类型的定义
 */
typedef unsigned int (OUTPUTAPI *LIB_TASK_ROUTINE)(void *);

#define GTW_PATH_SPACE_MARK "\\"//目录间隔符号
#define GTW_CFG_PATH "./"		//配置目录ini路径
#define OSC_COMTRADE_PATH ".\\"	//扰动录波COMTRADE路径
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

/* 文件映射 */
#define SHM_HANDLE		int

/* 进程 */
#define PROS_HANDLE		pid_t

/* 任务 */
#define	TASK_ID			pthread_t
#define TASK_HANDLE		pthread_t
#define	TASK_FUN		void *

/* 互斥锁 */
#define HMUTEX			pthread_mutex_t

/* 条件变量 */
#define HCOND			pthread_cond_t

/* 信号量 */
#define HSEM			int		/* 有名 */
#define HNSEM			sem_t	/* 无名 */

/* 管道 */
#define PIPE_HANDLE		int

/* 句柄 */
#define HANDLE			void*

/* 串口 */
#define HRSPORT			int

/* 套接字 */
#define	SOCKET			int
#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1
#define SOCK_LEN		socklen_t

/* 时间结构 */
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

/* 时间结构 */
typedef struct
{
	unsigned int ticksPerSec; //时钟频率
	unsigned int lastCheckTime;
	unsigned int wrapCount;
	double initMsTime;//初始时间
}lib_time_ms;

/**
 * @brief	动态库导出关键字，为了兼容跨平台。
 */
#define SHR_DLL_EXPORT

/**
 * @brief	函数属性的常量定义。
 */
#define OUTPUTAPI

/**
 * @name		SHR_THREAD_ROUTINE
 * @brief	线程处理函数类型的定义
 */
typedef void *(*LIB_TASK_ROUTINE)(void *);

#define GTW_PATH_SPACE_MARK "/"	//目录间隔符号
#define GTW_CFG_PATH "/opt/"	//配置目录ini路径
#define OSC_COMTRADE_PATH "./"	//扰动录波COMTRADE路径
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

/* 文件映射 */
#define SHM_HANDLE		int

/* 进程 */
#define PROS_HANDLE		pid_t

/* 任务 */
#define	TASK_ID			pthread_t
#define TASK_HANDLE		pthread_t
#define	TASK_FUN		void *

/* 互斥锁 */
#define HMUTEX			pthread_mutex_t

/* 条件变量 */
#define HCOND			pthread_cond_t

/* 信号量 */
#define HSEM			int		/* 有名 */
#define HNSEM			sem_t	/* 无名 */

/* 管道 */
#define PIPE_HANDLE		int

/* 句柄 */
#define HANDLE			void*

/* 串口 */
#define HRSPORT			int

/* 套接字 */
#define	SOCKET			int
#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1
#define SOCK_LEN		socklen_t

/* 时间结构 */
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

/* 时间结构 */
typedef struct
{
	unsigned int ticksPerSec; //时钟频率
	unsigned int lastCheckTime;
	unsigned int wrapCount;
	double initMsTime;//初始时间
}lib_time_ms;

/**
 * @brief	动态库导出关键字，为了兼容跨平台。
 */
#define SHR_DLL_EXPORT

/**
 * @brief	函数属性的常量定义。
 */
#define OUTPUTAPI

/**
 * @name		SHR_THREAD_ROUTINE
 * @brief	线程处理函数类型的定义
 */
typedef void *(*LIB_TASK_ROUTINE)(void *);

#define GTW_PATH_SPACE_MARK "/"	//目录间隔符号
#define GTW_CFG_PATH "/opt/"	//配置目录ini路径
#define OSC_COMTRADE_PATH "./"	//扰动录波COMTRADE路径
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
	char name[SHR_MAX_FILE_SIZE + 1];	//文件名称
	time_t last_modified;					//最后修改时间
	unsigned int size;							//文件大小
} SHR_FILE_ATTRI;

void lib_get_float32(unsigned char *pbuff, float *out);

//process

/**
 * @name		lib_process_create
 * @brief	创建一个进程。
 * @param	phandle		进程句柄。
 * @param	pfilename	程序名称。
 * @param	pcmdline	命令行。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_process_create(PROS_HANDLE *phandle, char *pfilename, char *pcmdline);

/**
 * @name		lib_process_destroy
 * @brief	关闭一个进程。
 * @param	handle		进程句柄。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_process_destroy(PROS_HANDLE handle, int iSignal);

int lib_processV2_create(PROS_HANDLE *phandle, void (*pfun)(char *pname), char *pname);

//thread

/**
 * @name		lib_task_create
 * @brief	创建一个线程。默认状态是可汇合(join)的，既非分离(detach)状态。
 * @param	phandle		线程句柄的指针。
 * @param	pid			线程id的指针。
 * @param	routine		线程处理函数。
 * @param	param		线程处理函数的参数指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_task_create(char *pname, int priority, int stackSize, TASK_HANDLE *phandle, LIB_TASK_ROUTINE routine, void *param);

/**
 * @name		lib_task_detach
 * @brief	对一个线程进行分离，此后该线程结束时自动释放资源等。
 * @param	phandle	线程句柄。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_task_detach(TASK_HANDLE phandle);

/**
 * @name		lib_task_join
 * @brief	阻塞在等待一个可汇合线程的结束。
 * @param	phandle		线程句柄。
 * @param	*pretcode	退出码。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_task_join(TASK_HANDLE phandle, void **pretcode);

/**
 * @name		lib_task_exit
 * @brief	退出一个线程的执行。
 * @param  retval	退出码。
 * @return	无。
 * @note
 */
void lib_task_exit(unsigned int retval);

//互斥锁

/**
 * @name		lib_mutex_init
 * @brief	初始化一个可递归的互斥锁。
 * @param	mutex	互斥锁句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note		互斥锁用前，一定要对它初始化。
 */
int lib_mutex_init(HMUTEX *mutex);

/**
 * @name		lib_mutex_lock
 * @brief	上锁。
 * @param	mutex	互斥锁句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_mutex_lock(HMUTEX *mutex);

/**
 * @name		lib_mutex_unlock
 * @brief	开锁。
 * @param	mutex	互斥锁句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_mutex_unlock(HMUTEX *mutex);

/**
 * @name		lib_mutex_destroy
 * @brief	销毁互斥锁。
 * @param	mutex	互斥锁句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note		互斥锁用后，一定要将它销毁。
 */
int lib_mutex_destroy(HMUTEX *mutex);

//条件变量

/**
 * @name		lib_cond_init
 * @brief	初始化一个条件变量。
 * @param	cond	条件变量句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note		条件变量用前，一定要对它初始化。
 */
int lib_cond_init(HCOND *cond);

/**
 * @name		lib_cond_wait
 * @brief	等待一个条件变量，等待时间为milliseconds毫秒。
 * @param	mutex			互斥锁句柄的指针，用来保证对条件变量的互斥访问。
 * @param	cond			条件变量句柄的指针。
 * @param	milliseconds	等待的毫秒数，为-1表示持续等待，直到条件满足。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_cond_wait(HMUTEX *mutex, HCOND *cond, int milliseconds);

/**
 * @name		lib_cond_signal
 * @brief	唤醒正在等待该条件变量的线程；如果有多个线程在等待，哪一个线程
 被唤醒是由线程的调度策略所决定的。
 * @param	cond	条件变量句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_cond_signal(HCOND *cond);

/**
 * @name		lib_cond_broadcast
 * @brief	用来唤醒所有被阻塞在条件变量cond上的线程，这些线程被唤醒后将再次竞争
 相应的互斥锁，所以必须小心使用这个函数。
 * @param	cond	条件变量句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_cond_broadcast(HCOND *cond);

/**
 * @name		lib_cond_destroy
 * @brief	销毁条件变量。
 * @param	cond	条件变量句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note		条件变量用后，一定要销毁它。
 */
int lib_cond_destroy(HCOND *cond);

//无名信号量

/**
 * @name		lib_noname_sem_create
 * @brief	初始化一个无名信号量，只能为当前进程的所有线程共享，value给出了信号量
 的初始值。
 * @param	sem		信号量句柄的指针。
 * @param	value	信号量的初始值。
 * @return	0为成功；其它为具体错误码。
 * @note		信号量用前，一定要对它初始化。
 */
int lib_noname_sem_create(HNSEM *sem, unsigned int value);

/**
 * @name		lib_noname_sem_destroy
 * @brief	销毁一个无名信号量。
 * @param	sem		信号量句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note		信号量用后，一定要销毁它。
 */
int lib_noname_sem_destroy(HNSEM *sem);

/**
 * @name		lib_noname_sem_wait
 * @brief	等待一个无名信号量。如果信号量值<=0，则一直等待，直到值>0。
 * @param	sem		信号量句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_noname_sem_wait(HNSEM *sem);

/**
 * @name		lib_noname_sem_post
 * @brief	用来增加无名信号量的值。当有线程阻塞在这个信号量上时，调用这个函数
 会使其中的一个线程不在阻塞，选择机制同样是由线程的调度策略决定的。
 * @param	sem		信号量句柄的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_noname_sem_post(HNSEM *sem);

/**
 * @name		lib_noname_sem_getvalue
 * @brief	获得无名信号量的值。
 * @param	sem		信号量句柄的指针。
 * @return	>=0为成功；其它为具体错误码。
 * @note
 */
int lib_noname_sem_getvalue(HNSEM *sem);

//有名信号量

/**
 * @name		lib_name_sem_create
 * @brief	创建一个有名信号量，max_num为最大允许值，init_num为信号量的初始值。
 * @param	pfilename	文件名称。
 * @param	max_num	    信号量的最大值。
 * @param	init_num	信号量的初始值。
 * @return	0为成功；其它为具体错误码。
 * @note		信号量用前，一定要对它初始化。
 */
int lib_name_sem_create(char *pfilename, unsigned int max_val,
		unsigned int init_val, HSEM *phandle);

/**
 * @name		lib_name_sem_open
 * @brief	打开一个有名信号量。
 * @param	pfilename	文件名称。
 * @return	0为成功；其它为具体错误码。
 * @note		信号量用前，一定要对它初始化。
 */
int lib_name_sem_open(char *pfilename, HSEM *phandle);

/**
 * @name		lib_name_sem_close
 * @brief	关闭一个有名信号量。
 * @param	hSem	信号量句柄。
 * @return	0为成功；其它为具体错误码。
 * @note		信号量用后，一定要销毁它。
 */
int lib_name_sem_close(HSEM hSemObj);

/**
 * @name		lib_name_sem_destroy
 * @brief	销毁一个有名信号量。
 * @param	hSem	信号量句柄。
 * @return	0为成功；其它为具体错误码。
 * @note		信号量用后，一定要销毁它。
 */
int lib_name_sem_destroy(HSEM hSemObj);

/**
 * @name		lib_noname_sem_wait
 * @brief	等待一个有名信号量。如果信号量值<=0，则一直等待，直到值>0。
 * @param	hSem			信号量句柄。
 * @param	dwMilliseconds	信号量句柄。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_name_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds);

/**
 * @name		lib_noname_sem_wait
 * @brief	等待心跳信号量。如果信号量值<=0，则一直等待，直到值>0。
 * @param	hSem			信号量句柄。
 * @param	dwMilliseconds	信号量句柄。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_beat_sem_wait(HSEM hSemObj, unsigned int dwMilliseconds);

/**
 * @name		lib_name_sem_post
 * @brief	用来增加有名信号量的值。当有线程阻塞在这个信号量上时，调用这个函数
 会使其中的一个线程不在阻塞，选择机制同样是由OS的调度策略决定的。
 * @param	hSem	信号量句柄。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_name_sem_post(HSEM hSemObj);

/**
 * @name		lib_beat_sem_post
 * @brief	用来增加心跳信号量的值,如果信号量值>0，则直接返回成功，否则将执行semop操作+1

 * @param	hSem	信号量句柄。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_beat_sem_post(HSEM hSemObj);

/**
 * @name		lib_name_sem_getvalue
 * @brief	获得有名信号量的值。
 * @param	hSem	信号量句柄。
 * @return	>=0为成功；其它为具体错误码。
 * @note
 */
int lib_name_sem_getvalue(HSEM hSemObj);

/**
 * @name		lib_shm_create
 * @brief	创建共享内存。
 * @param	pfilename 文件名称。
 * @param	max_size  内存大小。
 * @return	-1失败，0成功
 * @note
 */
int lib_shm_create(char *pfilename, unsigned int max_size, SHM_HANDLE *phandle);

/**
 * @name		lib_shm_open
 * @brief	打开共享内存。
 * @param	pfilename 文件名称。
 * @return	-1失败，0成功
 * @note
 */
int lib_shm_open(char *pfilename, SHM_HANDLE *phandle);

/**
 * @name		lib_shm_map
 * @brief	映射共享内存。
 * @param	hShmObj	  内存句柄。
 * @return	NULL失败，其它成功。
 * @note
 */
void* lib_shm_map(SHM_HANDLE hShmObj);

/**
 * @name		lib_shm_unmap
 * @brief	关闭共享内存。
 * @param	hMapAddr 内存地址。
 * @return	0成功，其它失败。
 * @note
 */
int lib_shm_unmap(void *hMapAddr);

/**
 * @name		lib_shm_close
 * @brief	删除共享内存。
 * @param	hMapObj 映射对象。
 * @return	0成功，其它失败。
 * @note
 */
int lib_shm_close(SHM_HANDLE hMapObj);

/**
 * @name		lib_shm_destroy
 * @brief	删除共享内存。
 * @param	hMapObj 映射对象。
 * @return	0成功，其它失败。
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

//共享库的下载

/**
 * @name		lib_load_library
 * @brief	下载一个共享库，如“libmytest.so”。要指明共享库的相对路径。
 * @param	filename	共享库名称。
 * @return	非NULL为成功；NULL为失败。
 * @note		使用共享库前，必须下载该库。
 */
HANDLE lib_load_library(const char *filename);

/**
 * @name		*lib_get_addr
 * @brief	从共享库下载一个模块，如函数的地址等。
 * @param	handle	共享库句柄，必须是xj_load_library()返回的句柄。
 * @param	module	模块指针。
 * @return	非NULL为成功；NULL为失败。
 * @note
 */
void *lib_get_addr(HANDLE handle, const char *module);

/**
 * @name		lib_free_library
 * @brief	释放一个共享库。
 * @param	handle	共享库句柄。
 * @return	0为成功；其它为具体错误码。
 * @note		使用共享库库，必须释放该库。
 */
int lib_free_library(HANDLE handle);

/**
 * @name		*lib_get_liberror
 * @brief	获得操作共享库的错误描述。
 * @return	错误描述。
 * @note
 */
char *lib_get_liberror();

/**
 * @name		lib_sleep
 * @brief	睡眠函数。
 * @param	milliseconds	睡眠的毫秒数目，必须>=0。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_sleep(unsigned int milliseconds);

/**
 * @name		lib_getlocaltime
 * @brief	取系统时间函数。
 * @param	psystime	存放时间缓冲区的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_getlocaltime(lib_systime *psystime);

/**
 * @name		lib_setlocaltime
 * @brief	修改系统时间函数。
 * @param	psystime	存放时间缓冲区的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_setlocaltime(const lib_systime *psystime);

/**
 * @name		lib_getcurrenttime
 * @brief	获取当前的秒和毫秒，时间自1970.1.1.0.0.0开始。
 * @param	pSeconds	  存放秒缓冲区的指针。
 * @param	pMilliSeconds 存放毫秒缓冲区的指针。
 * @return	0为成功；其它为具体错误码。
 * @note
 */
int lib_getcurrenttime(time_t *pSeconds, unsigned short *pMilliSeconds);

/**
 * @name		lib_time_init
 * @brief	初始化时间参数。
 * @param	ptTime	时间参数的指针。
 * @return	0为成功；-1为错误。
 * @note
 */
int lib_time_init(lib_time_ms *ptTime);

/**
 * @name		_lib_time_get
 * @brief	获取绝对时间。
 * @param	ptTime	时间参数的指针。
 * @return	返回的毫秒数。
 * @note
 */
double _lib_time_get(lib_time_ms *ptTime);

/**
 * @name		lib_create_dir
 * @brief	分级创建目录。
 * @param	pdir	目录名称。
 * @return	0为成功；非0为失败。
 * @note
 */
int lib_create_dir(const char *pdir);

/**
 * @name		lib_pathfile_exist
 * @brief	判断文件或目录是否存在。
 * @param	pathfile	文件或目录名称。
 * @return	0为存在；非0为不存在。
 * @note
 */
int lib_pathfile_exist(const char *pathfile);

/**
 * @name		lib_pathfile_exec
 * @brief	判断对文件或目录是否有执行权限。
 * @param	pathfile	文件或目录名称。
 * @return	0为有；非0为无。
 * @note
 */
//int lib_pathfile_exec(const char *pathfile);
/**
 * @name		lib_pathfile_rw
 * @brief	判断对文件或目录是否有读写权限。
 * @param	pathfile	文件或目录名称。
 * @return	0为有；非0为无。
 * @note
 */
//int lib_pathfile_rw(const char *pathfile);
/**
 * @name		lib_get_file_size
 * @brief	获得指定文件的大小，单位：byte。
 * @param	file_name	文件名称。
 * @return	>=0为文件的实际大小；-1为失败。
 * @note
 */
int lib_get_file_size(const char *file_name);

/**
 * @name		lib_get_filenum_of_dir
 * @brief	获得指定目录下的文件数目。
 * @param	pathfile	目录的绝对路径。
 * @return	>=0为文件数目；-1为失败。
 * @note
 */
int lib_get_filenum_of_dir(const char *pathfile);

/**
 * @name		lib_get_filelist_of_dir
 * @brief	获得指定目录下的文件属性列表。
 * @param	pathfile	目录的绝对路径。
 * @param	pfilelist	文件属性列表。
 * @param	maxnum		列表最大数目。
 * @return	>=0为列表数目；-1为失败。
 * @note
 */
int lib_get_filelist_of_dir(const char *pathfile, SHR_FILE_ATTRI *pfilelist,
		int maxnum);

/**
 * @name		lib_rsport_open
 * @brief	打开指定的串口。
 * @param	nr 串口号。
 * @return	非NULL成功；NULL失败。
 * @note
 */
HRSPORT lib_rsport_open(unsigned int nr);

/**
 * @name		lib_rsport_close
 * @brief	打开指定的串口。
 * @param	rsport 串口句柄。
 * @return	0成功；非0失败。
 * @note
 */
int lib_rsport_close(HRSPORT rsport);

/**
 * @name		lib_rsport_setup
 * @brief	设置指定串口的波特率、数据位、停止位等属性。
 * @param	rsport 串口句柄。
 * @param	attr   属性指针。
 * @return	0成功；非0失败。
 * @note
 */
int lib_rsport_setup(HRSPORT rsport, const rsport_attr* pattr);

/**
 * @name		lib_rsport_set_timeout
 * @brief	设置指定串口的读写超时间隔。
 * @param	rsport      串口句柄。
 * @param	rxTimeout   读超时，单位：毫秒。
 * @param	txTimeout   写超时，单位：毫秒。
 * @return	0成功；非0失败。
 * @note
 */
int lib_rsport_set_timeout( HRSPORT rsport, unsigned int rxTimeout,
		unsigned int txTimeout);

/**
 * @name		lib_rsport_write
 * @brief	向指定串口rsport写入数据。
 * @param	rsport           串口句柄。
 * @param	pbuff			 写缓冲区。
 * @param	bytes_to_write   写字节数。
 * @return	>=0写入的字节数；<0失败。
 * @note
 */
int lib_rsport_write(HRSPORT rsport, const char* pbuff, int bytes_to_write);

/**
 * @name		lib_rsport_read
 * @brief	从指定串口rsport读取数据。
 * @param	rsport           串口句柄。
 * @param	pbuff			 读缓冲区。
 * @param	bytes_to_read    读字节数。
 * @return	>=0读取的字节数；<0失败。
 * @note
 */
int lib_rsport_read(HRSPORT rsport, char* pbuff, int bytes_to_read);

/**
 * @name		lib_rsport_purge
 * @brief	清空指定串口rsport的读写缓冲区。
 * @param	rsport      串口句柄。
 * @param	read		清空读标志。
 * @param	write		清空写标志。
 * @return	0成功；非0失败。
 * @note
 */
int lib_rsport_purge(HRSPORT rsport, int read, int write);

/**
 * @name		lib_modify_time
 * @brief	对日期和时间进行毫秒级的修改，包括加和减两种运算。
 * @param	psystime		待修改日期和时间的指针，默认值为NULL。
 * @param	wMilliseconds	用来修改的毫秒数，默认值为0。
 * @param	op				运算：'+'或'-'，默认为'+'。
 * @return	0为成功；非0为失败。
 * @note		只能修改1900年1月1日0时0分0秒之后的时间。
 */
int lib_modify_time(lib_systime *psystime, unsigned short wMilliseconds,
		char op);

/**
 * @name		shrtime_to_timet
 * @brief	把系统时间转换为秒。
 * @param	psystime		待转换时间的指针，默认值为NULL。
 * @return	被转换的秒。
 * @note		。
 */
time_t shrtime_to_timet(lib_systime *psystime);

/**
 * @name		lib_time_get
 * @brief	获取相对时间。
 * @param	ptTime	时间参数的指针。
 * @return	返回的毫秒数。
 * @note
 */
double lib_time_get(lib_time_ms *ptTime);

#ifdef __cplusplus
}
#endif

#endif /* LIBAPP_H_ */
